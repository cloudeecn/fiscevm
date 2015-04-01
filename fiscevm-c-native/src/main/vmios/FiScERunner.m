/**
 *  Copyright 2010-2015 Yuxuan Huang. All rights reserved.
 *
 * This file is part of fiscevm
 *
 * fiscevm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * fiscevm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with fiscevm  If not, see <http://www.gnu.org/licenses/>.
 */

#import "FiScERunner.h"
#import "FiScEVM.h"

typedef NS_ENUM(int32_t, VM_STATUS){
    PAUSED,
    RUNNING,
    PAUSE_PENDING
};

static void gc_prepare(void *data);
static void gc_protect(void *data, int32_t *count, int32_t **handles);
static void gc_finish(void *data);


@interface FiScERunner ()

@end

@implementation FiScERunner{
    NSLock *gcLock;
    NSObject *vmLock;
    
    FiScEMessage *message;
    void (^tick)();
    VM_STATUS status;
    NSMutableDictionary *nativeHandlers;
    
    boolean_t initialized;
    
    FiScEVM *vm;
    NSString *statusFileToLoad;
    int32_t *handlesToKeepInGC;
    
    int32_t *protectList;
    int32_t protectLength;
    int32_t protectMax;
}

- (void)runAfterSeconds:(NSTimeInterval)delay{
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, delay * NSEC_PER_SEC), self.runnerQueue, tick);
}

- (instancetype)init{
    self = [super init];
    if(self){
        vmLock = [[NSObject alloc] init];
        message = [[FiScEMessage alloc] init];
        protectMax = 256;
        protectLength = 0;
        protectList = malloc(sizeof(int32_t) * protectMax);
        status = PAUSED;
        nativeHandlers = [NSMutableDictionary dictionary];
        __weak FiScERunner *weakSelf=self;
        tick=^{
            NSError *error;
            FiScERunner *strongSelf = weakSelf;
            if(strongSelf){
                
                @synchronized(strongSelf->vmLock){
                    switch(strongSelf->status){
                        case PAUSE_PENDING:
                            strongSelf->status = PAUSED;
                            return;
                        case RUNNING:
                            break;
                        default:
                            @throw [NSException exceptionWithName:@"Illegal status" reason:[NSString stringWithFormat:@"%d", strongSelf->status] userInfo:nil];
                    }
                    
                    if(strongSelf->statusFileToLoad){
                        NSLog(@"Restart VM to load status");
                        strongSelf->vm = nil;
                        if([[NSFileManager defaultManager] fileExistsAtPath:strongSelf.autoSavePath]) {
                            [[NSFileManager defaultManager] removeItemAtPath:strongSelf.autoSavePath error:&error];
                            if(error){
                                @throw [NSException exceptionWithName:@"IO" reason:@"Can't delete auto save file" userInfo:nil];
                            }
                        }
                        strongSelf->vm = [[FiScEVM alloc] initWithClassPaths:strongSelf.classPaths];
                        [strongSelf->vm handleGcEventWithData:(__bridge void *)(strongSelf) before:gc_prepare getExtra:gc_protect after:gc_finish];
                        [strongSelf->vm bootFromDataFile:strongSelf->statusFileToLoad];
                    }else if(!strongSelf->vm){
                        strongSelf->vm = [[FiScEVM alloc] initWithClassPaths:strongSelf.classPaths];
                        
                        if([[NSFileManager defaultManager] fileExistsAtPath:strongSelf.autoSavePath]) {
                            NSLog(@"Start vm from autosave");
                            [strongSelf->vm bootFromDataFile:strongSelf.autoSavePath];
                        }else{
                            NSLog(@"Start vm from main class: %@", strongSelf.mainClassName);
                            [strongSelf->vm bootFromMainClassWithName:strongSelf.mainClassName];
                        }
                    }
                    
                    while(true){
                        [strongSelf->vm runWithMessageHolder:strongSelf->message];
                        switch(strongSelf->message.messageType){
                            case 3: // invoke native
                            {
                                FiScENativeHandler handlerBlock = [strongSelf->nativeHandlers objectForKey:strongSelf->message.nativeCallName];
                                if(handlerBlock){
                                    @try{
                                        handlerBlock(strongSelf, strongSelf->vm, strongSelf->message.threadId, strongSelf->message.paramCount,strongSelf->message.params);
                                    }@catch(NSException *e){
                                        if(strongSelf.delegate){
                                            [strongSelf.delegate runnerEncountersException:strongSelf name:@"" description:[NSString stringWithFormat:@"Exception occored: %@", e] threadId:strongSelf->message.threadId];
                                        }else{
                                            @throw e;
                                        }
                                    }
                                }else{
                                    
                                    NSLog(@"WARN: unhandled native method call: %@", strongSelf->message.nativeCallName);
                                    if(strongSelf.delegate){
                                        [strongSelf.delegate runnerEncountersException:strongSelf name:@"java/lang/UnsatisfiedLinkError" description:strongSelf->message.nativeCallName threadId:strongSelf->message.threadId];
                                    }
                                }
                            }
                                break;
                            case 4: // exception
                                //TODO 脚本娘坏掉了
                                if(strongSelf.delegate){
                                    [strongSelf.delegate runnerEncountersException:strongSelf name:strongSelf->message.exceptionName description:strongSelf->message.exceptionDesc threadId:strongSelf->message.threadId];
                                } else {
                                    @throw [NSException exceptionWithName:@"Exception" reason:[NSString stringWithFormat:@"Unhandled exception occored: %@, %@", strongSelf->message.exceptionName, strongSelf->message.exceptionDesc] userInfo:nil];
                                }
                                break;
                            case 5: // sleep
                                //                                NSLog(@"Sleep %fs", strongSelf->message.sleepTime);
                                [strongSelf runAfterSeconds:strongSelf->message.sleepTime];
                                return;
                            case 6: // dead
                                if([[NSFileManager defaultManager] fileExistsAtPath:strongSelf.autoSavePath]) {
                                    [[NSFileManager defaultManager] removeItemAtPath:strongSelf.autoSavePath error:&error];
                                    if(error){
                                        @throw [NSException exceptionWithName:@"IO" reason:@"Can't delete auto save file" userInfo:nil];
                                    }
                                }
                                strongSelf->status = PAUSED;
                                NSLog(@"VM stopped");
                                if(strongSelf.delegate){
                                    [strongSelf.delegate runnerStopped:strongSelf];
                                }
                                return;
                            default: // Illegal type
                                //TODO
                                @throw [NSException exceptionWithName:@"TODO" reason:[NSString stringWithFormat:@"Illegal message type: %d", strongSelf->message.messageType] userInfo:nil];
                        }
                    }
                }
            }
        };
    }
    return self;
}

- (void)dealloc{
    free(gc_protect);
}

- (void)handleNativeMethod:(NSString*)methodName withBlock:(FiScENativeHandler)block{
    [nativeHandlers setObject:block forKey:methodName];
}

- (void)setupDefaultProperties{
    NSError *error;
    if(initialized){
        @throw [NSException exceptionWithName:@"IllegalStatus" reason:@"Already initialized" userInfo:nil];
    }
    
    if(!self.dataPath){
        self.dataPath=[NSString stringWithFormat:@"%@/Application Support", ((NSURL*)[[[NSFileManager defaultManager] URLsForDirectory:NSLibraryDirectory inDomains:NSUserDomainMask] lastObject]).path];
    }
    
    if(!self.savePath){
        self.savePath=[NSString stringWithFormat:@"%@/saves", self.dataPath];
    }
    
    if(!self.autoSavePath){
        self.autoSavePath=[NSString stringWithFormat:@"%@/autosave.dat", self.dataPath];
    }
    
    if(!self.runnerQueue){
        self.runnerQueue=dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    }
    
    [[NSFileManager defaultManager] createDirectoryAtPath:self.dataPath withIntermediateDirectories:YES attributes:nil error:&error];
    if(error){
        @throw [NSException exceptionWithName:@"Init" reason:[error description] userInfo:nil];
    }
    
    [[NSFileManager defaultManager] createDirectoryAtPath:self.savePath withIntermediateDirectories:YES attributes:nil error:&error];
    if(error){
        @throw [NSException exceptionWithName:@"Init" reason:[error description] userInfo:nil];
    }
    initialized = true;
}

- (void)resume{
    NSLog(@"Will resume vm");
    @synchronized(vmLock){
        if(!initialized){
            [self setupDefaultProperties];
        }
        NSLog(@"Resume, status = %d", status);
        switch(status){
            case PAUSE_PENDING:
            case RUNNING:
                break;
            case PAUSED:
                [self runAfterSeconds:0];
                break;
        }
        status = RUNNING;
    }
    NSLog(@"Resume vm done");
}

- (void)start{
    NSLog(@"Will restart vm");
    @synchronized(vmLock){
        if(!initialized){
            [self setupDefaultProperties];
        }
        vm = nil;
        NSError *error;
        if([[NSFileManager defaultManager] fileExistsAtPath:self.autoSavePath]) {
            [[NSFileManager defaultManager] removeItemAtPath:self.autoSavePath error:&error];
        }
        if(error){
            @throw [NSException exceptionWithName:@"Delete autosave file" reason:[error description] userInfo:nil];
        }
        [self resume];
    }
    NSLog(@"Restart vm done");
}

- (void)loadStatus:(NSString*)fileName{
    @synchronized(vmLock){
        if(!initialized){
            [self setupDefaultProperties];
        }
        statusFileToLoad = [NSString stringWithFormat:@"%@/%@", self.savePath, fileName];
    }
}

- (void)saveStatus:(NSString*)fileName{
    @synchronized(vmLock){
        if(vm){
            [vm saveToDataFile:[NSString stringWithFormat:@"%@/%@", self.savePath, fileName]];
        }else{
            @throw [NSException exceptionWithName:@"IllegalStatus" reason:@"" userInfo:nil];
        }
    }
}

- (void)pauseSoft{
    [self pauseWithForce:false];
}

- (void)pauseForce{
    [self pauseWithForce:true];
}

- (void)pauseWithForce:(boolean_t)force{
    NSLog(force?@"Will pause vm force" : @"Will pause vm");
    @synchronized(vmLock){
        switch(status){
            case PAUSE_PENDING:
                if(force && vm){
                    vm = nil;
                }
                break;
            case PAUSED:
                if(force && vm){
                    vm = nil;
                }
                break;
            case RUNNING:
                if(vm){
                    [vm saveToDataFile:self.autoSavePath];
                    if(force){
                        vm = nil;
                    }
                }
                status = PAUSE_PENDING;
                break;
        }
    }
    NSLog(@"Pause vm done");
}

- (void)unprotectAll{
    protectLength = 0;
}

- (void)ensureProtectSize:(int32_t)size{
    int32_t max = protectMax;
    if(size > max){
        while(size > max || max < 0){
            max <<= 1;
        }
        if(max > 0){
            protectList=reallocf(protectList, size * (sizeof(int32_t)));
            if(protectList == NULL){
                @throw [NSException exceptionWithName:@"OutOfMemory" reason:[NSString stringWithFormat:@"Can't increase protect_list to size: %lu", size * (sizeof(int32_t))] userInfo:nil];
            }
        } else {
            @throw [NSException exceptionWithName:@"OutOfMemory" reason:[NSString stringWithFormat:@"Protect size exceeds max int32_t: %u", max] userInfo:nil];
        }
    }
}

- (void)protect:(int32_t)handle{
    [self ensureProtectSize:protectLength + 1];
    protectList[protectLength++]=handle;
}

- (int32_t)protectLength{
    return protectLength;
}

- (int32_t*)protectList{
    return protectList;
}

- (void)lockGC{
    [gcLock lock];
}

- (void)unlockGC{
    [gcLock unlock];
}

- (void)lockGCWithBlock:(void (^)())block{
    @try {
        [gcLock lock];
        block();
    }@finally{
        [gcLock unlock];
    }
}

@end

static void gc_prepare(void *data){
    FiScERunner *runner = (__bridge FiScERunner *)(data);
    [runner unprotectAll];
    
    if(runner.delegate){
        [runner.delegate runnerCollectsExtraObjectsForGC:runner withCollector:^(int32_t handle){
            [runner protect:handle];
        }];
    }
    [runner lockGC];
}

static void gc_protect(void *data, int32_t *count, int32_t **handles){
    FiScERunner *runner = (__bridge FiScERunner *)(data);
    *count = [runner protectLength];
    *handles = [runner protectList];
}

static void gc_finish(void *data){
    FiScERunner *runner = (__bridge FiScERunner *)(data);
    [runner unlockGC];
}

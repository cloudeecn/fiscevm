//
//  FiScERunner.m
//  fiscevm-ios
//
//  Created by Cloudee on 11/13/14.
//  Copyright (c) 2014 Cloudee. All rights reserved.
//

#import "FiScERunner.h"
#import "FiScEVM.h"

typedef NS_ENUM(int32_t, VM_STATUS){
    PAUSED,
    RUNNING,
    PAUSE_PENDING
};

@interface FiScERunner ()

@end

@implementation FiScERunner{
    NSObject *vmLock;
    FiScEMessage *message;
    void (^tick)();
    VM_STATUS status;
    NSMutableDictionary *nativeHandlers;
    
    boolean_t initialized;
    
    FiScEVM *vm;
    NSString *statusFileToLoad;
}

- (void)runAfterSeconds:(NSTimeInterval)delay{
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, delay * NSEC_PER_SEC), self.runnerQueue, tick);
}

- (instancetype)init{
    self = [super init];
    if(self){
        vmLock = [[NSObject alloc] init];
        message = [[FiScEMessage alloc] init];
        status = PAUSED;
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
                        [strongSelf->vm bootFromDataFile:strongSelf->statusFileToLoad];
                    }else if(!strongSelf->vm){
                        strongSelf->vm = [[FiScEVM alloc] init];
                        
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
                                FiScENativeHandler handlerBlock= [strongSelf->nativeHandlers objectForKey:strongSelf->message.nativeCallName];
                                if(handlerBlock){
                                    handlerBlock(strongSelf->vm, strongSelf->message.threadId, strongSelf->message.paramCount,strongSelf->message.params);
                                }else{
                                    @throw [NSException exceptionWithName:@"TODO" reason:[NSString stringWithFormat:@"Can't handle native call: %@", strongSelf->message.nativeCallName] userInfo:nil];
                                }
                            }
                                break;
                            case 4: // exception
                                //TODO
                                @throw [NSException exceptionWithName:@"TODO" reason:[NSString stringWithFormat:@"Illegal message type: %d", strongSelf->message.messageType] userInfo:nil];
                                break;
                            case 5: // sleep
//                                NSLog(@"Sleep %fs", strongSelf->message.sleepTime);
                                [strongSelf runAfterSeconds:strongSelf->message.sleepTime];
                                return;
                            case 6: // dead
                                [strongSelf start];
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

@end

//
//  fiscevm_ios.m
//  fiscevm-ios
//
//  Created by Cloudee on 10/31/14.
//  Copyright (c) 2014 Cloudee. All rights reserved.
//

#import "FiScEVM.h"
#import "fisce.h"
#import "fiscedev.h"
#import "fyc/Constants.h"

#define HANDLE_EXCEPTION if(ex->exceptionType!=exception_none){ \
@throw [[NSException alloc] initWithName:@"FyException" reason:[NSString stringWithFormat:@"Exception %s occored: %s" , ex->exceptionName , ex->exceptionDesc] userInfo:nil]; \
}

#define HANDLE_EXCEPTION_CUSTOM(C) if(ex->exceptionType!=exception_none){ \
C ; \
@throw [[NSException alloc] initWithName:@"FyException" reason:[NSString stringWithFormat:@"Exception %s occored: %s" , ex->exceptionName , ex->exceptionDesc] userInfo:nil]; \
}


@implementation FiScEClass
fy_class *clazz;

- (instancetype)initWithClass:(fy_class*)fyClass{
    self = [super init];
    char name[500];
    clazz=fyClass;
    fy_strSPrint(name, sizeof(name)-1, fyClass->className);
    self.name=[NSString stringWithUTF8String:name];
    return self;
}

- (fy_class*)clazz{
    return clazz;
}

@end

@implementation FiScEMethod
fy_method *method;

- (instancetype)initWithMethod:(fy_method*)fyMethod{
    self = [super init];
    char name[500];
    method=fyMethod;
    fy_strSPrint(name, sizeof(name)-1, fyMethod->uniqueName);
    self.uniqueName=[NSString stringWithUTF8String:name];
    return self;
}

@end

@implementation FiScEField
fy_field *field;

- (instancetype)initWithField:(fy_field*)fyField{
    self = [super init];
    char name[500];
    field=fyField;
    fy_strSPrint(name, sizeof(name)-1, fyField->uniqueName);
    self.uniqueName=[NSString stringWithUTF8String:name];
    return self;
}

@end

@implementation FiScEMessage

- (instancetype)init{
    self = [super init];
    self.messageType=message_none;
    return self;
}

@end

static void releaseValue(fy_str *key, void *value, void *additional){
    CFRelease(value);
}
static fy_int SOSWrite(struct fy_context *context, struct fy_thread *thread,
                       void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
                       fy_exception *exception) {
    FiScEVM *vm=(__bridge FiScEVM*)data;
    NSString *channel=[vm getStringFromObject:args[2].ivalue];
    char ch = (char)args[1].ivalue;
    [vm appendLog:channel content:ch];
    return ops - 1;
}

@interface FiScEDefaultLogger : NSObject <FiScELogger>

@end

@implementation FiScEDefaultLogger

- (void)logWithLevel:(int32_t)level content:(NSString*)content{
    NSLog(@"LOG %d: %@", level, content);
}

@end

@implementation FiScEVM{
    fy_context *context;
    fy_exception ex[1];
    fy_hashMap classCache[1];
    fy_hashMap methodCache[1];
    fy_hashMap fieldCache[1];
    NSMutableDictionary *stringCache;
    fy_message message[1];
    char systemOutBuf[1000];
    int32_t sobPos;
    char systemErrBuf[1000];
    int32_t sebPos;
    char **classPaths;
}

- (instancetype)initWithClassPaths:(NSArray*)cps
{
    self = [super init];
    [self setSystemErrLogger:[[FiScEDefaultLogger alloc] init]];
    [self setSystemOutLogger:[[FiScEDefaultLogger alloc] init]];
    stringCache = [NSMutableDictionary dictionary];
    
    context = fy_allocate(sizeof(fy_context), ex);
    HANDLE_EXCEPTION
    
    fisceInitContext(context, ex);
    HANDLE_EXCEPTION
    
    {
        NSUInteger count = cps.count;
        classPaths = fy_mmAllocatePerm(context->memblocks, sizeof(char**)*(count+1), ex);
        HANDLE_EXCEPTION
        
        NSUInteger i = 0;
        for (NSString *classpath in cps) {
            NSString *cp1 = [NSString stringWithFormat:@"%@/%@",[[NSBundle bundleForClass:[FiScEVM class]] bundlePath],classpath] ;
            const char *path = [cp1 UTF8String];
            size_t len = strlen(path);
            classPaths[i] = fy_mmAllocatePerm(context->memblocks, sizeof(char)*(len + 1), ex);
            strncpy(classPaths[i], [cp1 UTF8String], len);
            i++;
        }
        classPaths[i] = NULL;
    }
    
    context->isParam=classPaths;
    
    sobPos=0;
    sebPos=0;
    
    
    fy_hashMapInit(context->memblocks, classCache, 16, 14, ex);
    HANDLE_EXCEPTION
    fy_hashMapInit(context->memblocks, methodCache, 128, 14, ex);
    HANDLE_EXCEPTION
    fy_hashMapInit(context->memblocks, fieldCache, 128, 14, ex);
    HANDLE_EXCEPTION
    fy_nativeUnRegisterNativeHandler(context, "com/cirnoworks/fisce/privat/SystemOutputStream.write0.(IL"FY_BASE_STRING";)V", ex);
    HANDLE_EXCEPTION
    fy_nativeRegisterNativeHandler(context,
                                   "com/cirnoworks/fisce/privat/SystemOutputStream.write0.(IL"FY_BASE_STRING";)V", (__bridge void *)(self), SOSWrite, ex);
    HANDLE_EXCEPTION
    NSLog(@"FiScEVM initialized at %p", (__bridge void*)self);
    return self;
}

- (void)dealloc
{
    fy_hashMapEachValue(context->memblocks, fieldCache, releaseValue, NULL);
    fy_hashMapEachValue(context->memblocks, methodCache, releaseValue, NULL);
    fy_hashMapEachValue(context->memblocks, classCache, releaseValue, NULL);
    fisceDestroyContext(context);
    fy_free(context);
    NSLog(@"FiScEVM destroyed at %p", (__bridge void*)self);
}

- (void)appendLog:(NSString*)channel content:(char)content{
    if([@"error" isEqualToString:channel]){
        if(sebPos == 999 || content == '\n' || content == 0){
            systemErrBuf[sebPos]=0;
            [self.systemErrLogger logWithLevel:3 content:[NSString stringWithUTF8String:systemErrBuf]];
            if(!(content == '\n' || content == 0)){
                systemErrBuf[0]=content;
                sebPos = 1;
            }else{
                sebPos=0;
            }
        }else{
            systemErrBuf[sebPos++]=content;
        }
    }else{
        if(sebPos == 999 || content == '\n' || content == 0){
            systemOutBuf[sobPos]=0;
            [self.systemOutLogger logWithLevel:1 content:[NSString stringWithUTF8String:systemOutBuf]];
            if(!(content == '\n' || content == 0)){
                systemOutBuf[0]=content;
                sobPos = 1;
            }else{
                sobPos=0;
            }
        }else{
            systemOutBuf[sobPos++]=content;
        }
    }
}

- (void)handleGcEventWithData:(void*)data before:(void (*)(void*))beforeHandler getExtra:(void (*)(void*, int32_t*, int32_t**))getExtraHandler after:(void (*)(void*))afterHandler{
    context->gcCustomData = data;
    context->beforeGC = beforeHandler;
    context->getExtraGCKeep = getExtraHandler;
    context->afterGC = afterHandler;
}

- (void)returnIntValue:(int32_t)value toThread:(int32_t)threadId{
    fy_thread *thread=context->threads[threadId];
    fy_nativeReturnInt(context, thread, value);
}

- (void)returnHandleValue:(int32_t)value toThread:(int32_t)threadId{
    fy_thread *thread=context->threads[threadId];
    fy_nativeReturnHandle(context, thread, value);
}

- (void)returnLongValue:(int64_t)value toThread:(int32_t)threadId{
    fy_thread *thread=context->threads[threadId];
    fy_nativeReturnLong(context, thread, value);
}

- (void)returnFloatValue:(float_t)value toThread:(int32_t)threadId{
    fy_thread *thread=context->threads[threadId];
    fy_nativeReturnFloat(context, thread, value);
}

- (void)returnDoubleValue:(double_t)value  toThread:(int32_t)threadId{
    fy_thread *thread=context->threads[threadId];
    fy_nativeReturnDouble(context, thread, value);
}

- (FiScEClass*)getFiScEClassWith:(fy_class*)clazz{
    FiScEClass *ret;
    if(clazz==NULL){
        ret=nil;
    }else{
        ret=(__bridge FiScEClass *)(fy_hashMapGetConst(context->memblocks, classCache, clazz->className));
        if(ret==NULL){
            ret=[[FiScEClass alloc] initWithClass:clazz];
            fy_hashMapPutConst(context->memblocks, classCache, clazz->className, CFBridgingRetain(ret), ex);
            HANDLE_EXCEPTION
        }
    }
    return ret;
}

- (FiScEMethod*)getFiScEMethodWith:(fy_method*)method{
    FiScEMethod *ret;
    if(method==NULL){
        ret=nil;
    }else{
        ret=(__bridge FiScEMethod *)(fy_hashMapGetConst(context->memblocks, methodCache, method->uniqueName));
        if(ret==NULL){
            ret=[[FiScEMethod alloc] initWithMethod:method];
            fy_hashMapPutConst(context->memblocks, methodCache, method->uniqueName, CFBridgingRetain(ret), ex);
            HANDLE_EXCEPTION
        }
    }
    return ret;
}

- (FiScEField*)getFiScEFieldWith:(fy_field*)field{
    FiScEField *ret;
    if(field==NULL){
        ret=nil;
    }else{
        ret=(__bridge FiScEField *)(fy_hashMapGetConst(context->memblocks, fieldCache, field->uniqueName));
        if(ret==NULL){
            ret=[[FiScEField alloc] initWithField:field];
            fy_hashMapPutConst(context->memblocks, fieldCache, field->uniqueName, CFBridgingRetain(ret), ex);
        }
    }
    return ret;
}

- (NSString*)getCachedNSStringWith:(const char*)str{
    NSString *ret;
    NSNumber *key = [NSNumber numberWithLongLong:(size_t)str];
    ret = [stringCache objectForKey:[NSNumber numberWithLongLong:(size_t)str]];
    if(!ret){
        ret = [NSString stringWithUTF8String:str];
        [stringCache setObject:ret forKey:key];
    }
    return ret;
}

- (uint32_t)prepareThrowableWithClass:(NSString*)className
                              message:(NSString*)message
                            forThread:(int32_t)threadId{
    fy_thread *thread=context->threads[threadId];
    int32_t ret;
    fy_exception e;
    e.exceptionType=exception_normal;
    strncpy(e.exceptionName, [className UTF8String], sizeof(e.exceptionName));
    strncpy(e.exceptionDesc, [className UTF8String], sizeof(e.exceptionDesc));
    ret=fy_nativePrepareThrowable(context, thread, &e, ex);
    HANDLE_EXCEPTION
    return ret;
}

- (FiScEClass*)lookupClassWithName:(NSString*)name{
    fy_class *ret;
    fy_str str;
    str.content = NULL;
    fy_strInitWithUTF8(context->memblocks, &str, [name UTF8String], ex);
    HANDLE_EXCEPTION
    
    ret = fy_nativeLookupClass(context, &str, ex);
    fy_strDestroy(context->memblocks, &str);
    HANDLE_EXCEPTION;
    return [self getFiScEClassWith:ret];
}

- (FiScEField*)getFieldByName: (NSString*)name{
    fy_field *ret;
    fy_str str;
    str.content = NULL;
    fy_strInitWithUTF8(context->memblocks, &str, [name UTF8String], ex);
    HANDLE_EXCEPTION
    ret = fy_nativeGetField(context, &str);
    fy_strDestroy(context->memblocks, &str);
    return [self getFiScEFieldWith:ret];
}

- (FiScEField*)lookupFieldVirtualFromClass: (FiScEClass*)clazz name:(NSString*)name{
    fy_field *ret;
    fy_str str;
    str.content = NULL;
    fy_strInitWithUTF8(context->memblocks, &str, [name UTF8String], ex);
    HANDLE_EXCEPTION
    
    
    ret = fy_nativeLookupFieldVirtual(context, [clazz clazz], &str, ex);
    fy_strDestroy(context->memblocks, &str);
    HANDLE_EXCEPTION;
    return [self getFiScEFieldWith:ret];
}

- (FiScEMethod*)getMethodByName:(NSString*)name{
    fy_method *ret;
    fy_str str;
    str.content = NULL;
    fy_strInitWithUTF8(context->memblocks, &str, [name UTF8String], ex);
    HANDLE_EXCEPTION
    ret = fy_nativeGetMethod(context, &str);
    fy_strDestroy(context->memblocks, &str);
    return [self getFiScEMethodWith:ret];
}

- (FiScEMethod*)lookupMethodVirtualFromClass:(FiScEClass*)clazz name:(NSString*)name{
    fy_method *ret;
    fy_str str;
    str.content = NULL;
    fy_strInitWithUTF8(context->memblocks, &str, [name UTF8String], ex);
    HANDLE_EXCEPTION
    
    
    ret = fy_nativeLookupMethodVirtual(context, [clazz clazz], &str, ex);
    fy_strDestroy(context->memblocks, &str);
    HANDLE_EXCEPTION;
    return [self getFiScEMethodWith:ret];
}

- (fy_uint)allocate:(FiScEClass*) clazz{
    fy_uint ret;
    ret = fy_nativeAllocate(context, [clazz clazz], ex);
    HANDLE_EXCEPTION
    return ret;
}

- (FiScEClass*)getClassOfObject:(int32_t)handle{
    fy_class *ret;
    ret = fy_nativeGetClassOfObject(context, handle, ex);
    HANDLE_EXCEPTION
    return [self getFiScEClassWith:ret];
}

- (fy_uint)allocateArray:(FiScEClass*)clazz length:(int32_t)length{
    fy_uint ret;
    ret = fy_nativeAllocateArray(context, [clazz clazz], length, ex);
    HANDLE_EXCEPTION
    return ret;
}

- (int32_t)arrayLengthOf:(int32_t)handle{
    int32_t ret;
    ret = fy_nativeArrayLength(context, handle, ex);
    HANDLE_EXCEPTION
    return ret;
}

- (void)arrayCopyFrom:(int32_t)src fromPos:(int32_t)srcPos to:(int32_t)dest toPos:(int32_t)destPos length:(int32_t)len{
    fy_nativeArrayCopy(context, src, srcPos, dest, destPos, len, ex);
    HANDLE_EXCEPTION
}

- (int32_t)cloneObject:(int32_t)handle{
    int32_t ret;
    ret = fy_nativeClone(context, handle, ex);
    HANDLE_EXCEPTION
    return ret;
}

- (NSString*)getStringFromObject:(int32_t)handle{
    const int CHARS_ON_STACK = 512;
    fy_str str;
    char ustr[CHARS_ON_STACK];
    char *us = ustr;
    int len;
    
    NSString *ret;
    str.content = NULL;
    fy_strInit(context->memblocks, &str, 1024, ex);
    HANDLE_EXCEPTION
    fy_nativeGetString(context, handle, &str, ex);
    HANDLE_EXCEPTION_CUSTOM(fy_strDestroy(context->memblocks, &str))
    len = fy_strUtf8Count(&str);
    if(len + 1 >= CHARS_ON_STACK){
        us = fy_allocate(len + 1, ex);
        HANDLE_EXCEPTION_CUSTOM(fy_strDestroy(context->memblocks, &str))
    }
    fy_strSPrint(us, len + 1, &str);
    @try{
        ret=[self getCachedNSStringWith:us];
    }
    @finally{
        if(us != ustr){
            fy_free(us);
        }
        fy_strDestroy(context->memblocks, &str);
    }
    return ret;
}

- (int32_t)makeString:(NSString*)string{
    fy_str str;
    int32_t ret;
    str.content = NULL;
    fy_strInit(context->memblocks, &str, (int32_t)[string length], ex);
    HANDLE_EXCEPTION
    fy_strAppendUTF8(context->memblocks, &str, [string UTF8String], -1, ex);
    HANDLE_EXCEPTION_CUSTOM(fy_strDestroy(context->memblocks, &str));
    ret = fy_nativeMakeString(context, &str, ex);
    fy_strDestroy(context->memblocks, &str);
    HANDLE_EXCEPTION
    return ret;
}
- (int32_t)literalString:(NSString*)string{
    fy_str str;
    int32_t ret;
    str.content = NULL;
    fy_strInit(context->memblocks, &str, (int32_t)[string length], ex);
    HANDLE_EXCEPTION
    fy_strAppendUTF8(context->memblocks, &str, [string UTF8String], -1, ex);
    HANDLE_EXCEPTION_CUSTOM(fy_strDestroy(context->memblocks, &str));
    ret = fy_nativeLiteral(context, &str, ex);
    fy_strDestroy(context->memblocks, &str);
    HANDLE_EXCEPTION
    return ret;
}

- (void)beginProtect{
    fy_nativeBeginProtect(context);
}
- (void)endProtect{
    fy_nativeEndProtect(context);
}

- (boolean_t)getBooleanFromArray:(int32_t)handle index:(int32_t)index{
    boolean_t ret=fy_nativeGetArrayBoolean(context, handle, index, ex);
    HANDLE_EXCEPTION
    return ret;
}

- (int32_t)getHandleFromArray:(int32_t)handle index:(int32_t)index{
    int32_t ret=fy_nativeGetArrayInt(context, handle, index, ex);
    HANDLE_EXCEPTION
    return ret;
}
- (int8_t)getByteFromArray:(int32_t)handle index:(int32_t)index{
    int8_t ret=fy_nativeGetArrayByte(context, handle, index, ex);
    HANDLE_EXCEPTION
    return ret;
}
- (int16_t)getShortFromArray:(int32_t)handle index:(int32_t)index{
    int16_t ret=fy_nativeGetArrayShort(context, handle, index, ex);
    HANDLE_EXCEPTION
    return ret;
}
- (uint16_t)getCharFromArray:(int32_t)handle index:(int32_t)index{
    uint16_t ret=fy_nativeGetArrayChar(context, handle, index, ex);
    HANDLE_EXCEPTION
    return ret;
}
- (int32_t)getIntFromArray:(int32_t)handle index:(int32_t)index{
    int32_t ret=fy_nativeGetArrayInt(context, handle, index, ex);
    HANDLE_EXCEPTION
    return ret;
}
- (int64_t)getLongFromArray:(int32_t)handle index:(int32_t)index{
    int64_t ret=fy_nativeGetArrayLong(context, handle, index, ex);
    HANDLE_EXCEPTION
    return ret;
}
- (float_t)getFloatFromArray:(int32_t)handle index:(int32_t)index{
    float_t ret=fy_nativeGetArrayFloat(context, handle, index, ex);
    HANDLE_EXCEPTION
    return ret;
}
- (double_t)getDoubleFromArray:(int32_t)handle index:(int32_t)index{
    double_t ret=fy_nativeGetArrayDouble(context, handle, index, ex);
    HANDLE_EXCEPTION
    return ret;
}

- (int8_t*)getbytesFromArray:(int32_t)handle{
    int8_t* ret=fy_nativeGetArrayBytes(context, handle, ex);
    HANDLE_EXCEPTION
    return ret;
}

- (void)putBoolean:(boolean_t)value toArray:(int32_t)handle index:(int32_t)index{
    fy_nativePutArrayBoolean(context, handle, index, value, ex);
    HANDLE_EXCEPTION
}
- (void)putHandle:(int32_t)value toArray:(int32_t)handle index:(int32_t)index{
    fy_nativePutArrayInt(context, handle, index, value, ex);
    HANDLE_EXCEPTION
}
- (void)putByte:(int8_t)value toArray:(int32_t)handle index:(int32_t)index{
    fy_nativePutArrayByte(context, handle, index, value, ex);
    HANDLE_EXCEPTION
}
- (void)putShort:(int16_t)value toArray:(int32_t)handle index:(int32_t)index{
    fy_nativePutArrayShort(context, handle, index, value, ex);
    HANDLE_EXCEPTION
}
- (void)putChar:(uint16_t)value toArray:(int32_t)handle index:(int32_t)index{
    fy_nativePutArrayChar(context, handle, index, value, ex);
    HANDLE_EXCEPTION
}
- (void)putInt:(int32_t)value toArray:(int32_t)handle index:(int32_t)index{
    fy_nativePutArrayInt(context, handle, index, value, ex);
    HANDLE_EXCEPTION
}
- (void)putLong:(int64_t)value toArray:(int32_t)handle index:(int32_t)index{
    fy_nativePutArrayLong(context, handle, index, value, ex);
    HANDLE_EXCEPTION
}
- (void)putFloat:(float_t)value toArray:(int32_t)handle index:(int32_t)index{
    fy_nativePutArrayFloat(context, handle, index, value, ex);
    HANDLE_EXCEPTION
}
- (void)putDouble:(double_t)value toArray:(int32_t)handle index:(int32_t)index{
    fy_nativePutArrayDouble(context, handle, index, value, ex);
    HANDLE_EXCEPTION
}

- (boolean_t)getBooleanFromObject:(int32_t)handle field:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}
- (int32_t)getHandleFromObject:(int32_t)handle field:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}
- (int8_t)getByteFromObject:(int32_t)handle field:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}
- (int16_t)getShortFromObject:(int32_t)handle field:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}
- (uint16_t)getCharFromObject:(int32_t)handle field:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}
- (int32_t)getIntFromObject:(int32_t)handle field:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}
- (int64_t)getLongFromObject:(int32_t)handle field:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}
- (float_t)getFloatFromObject:(int32_t)handle field:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}
- (double_t)getDoubleFromObject:(int32_t)handle field:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}
- (int8_t*)getbytesFromObject:(int32_t)handle field:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}

- (void)putBoolean:(boolean_t)value toObject:(int32_t)handle field:(FiScEField*)field{/*TODO*/}
- (void)putHandle:(int32_t)value toObject:(int32_t)handle field:(FiScEField*)field{/*TODO*/}
- (void)putByte:(int8_t)value toObject:(int32_t)handle field:(FiScEField*)field{/*TODO*/}
- (void)putShort:(int16_t)value toObject:(int32_t)handle field:(FiScEField*)field{/*TODO*/}
- (void)putChar:(uint16_t)value toObject:(int32_t)handle field:(FiScEField*)field{/*TODO*/}
- (void)putInt:(int32_t)value toObject:(int32_t)handle field:(FiScEField*)field{/*TODO*/}
- (void)putLong:(int64_t)value toObject:(int32_t)handle field:(FiScEField*)field{/*TODO*/}
- (void)putFloat:(float_t)value toObject:(int32_t)handle field:(FiScEField*)field{/*TODO*/}
- (void)putDouble:(double_t)value toObject:(int32_t)handle field:(FiScEField*)field{/*TODO*/}

- (boolean_t)getBooleanFromStaticField:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}
- (int32_t)getHandleFromStaticField:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}
- (int8_t)getByteFromStaticField:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}
- (int16_t)getShortFromStaticField:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}
- (uint16_t)getCharFromStaticField:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}
- (int32_t)getIntFromStaticField:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}
- (int64_t)getLongFromStaticField:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}
- (float_t)getFloatFromStaticField:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}
- (double_t)getDoubleFromStaticField:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}
- (int8_t*)getbytesFromStaticField:(FiScEField*)field{
    @throw [[NSException alloc] initWithName:@"Unsupported" reason:@"Unsupported operation" userInfo:nil];
}

- (void)putBoolean:(boolean_t)value toStaticField:(FiScEField*)field{/*TODO*/}
- (void)putHandle:(int32_t)value toStaticField:(FiScEField*)field{/*TODO*/}
- (void)putByte:(int8_t)value toStaticField:(FiScEField*)field{/*TODO*/}
- (void)putShort:(int16_t)value toStaticField:(FiScEField*)field{/*TODO*/}
- (void)putChar:(uint16_t)value toStaticField:(FiScEField*)field{/*TODO*/}
- (void)putInt:(int32_t)value toStaticField:(FiScEField*)field{/*TODO*/}
- (void)putLong:(int64_t)value toStaticField:(FiScEField*)field{/*TODO*/}
- (void)putFloat:(float_t)value toStaticField:(FiScEField*)field{/*TODO*/}
- (void)putDouble:(double_t)value toStaticField:(FiScEField*)field{/*TODO*/}

- (void)unRegisterNativeHandlerWithName:(NSString*)name{
    fy_nativeUnRegisterNativeHandler(context, [name UTF8String], ex);
    HANDLE_EXCEPTION
}

- (void)bootFromMainClassWithName:(NSString*)name{
    fisceBootFromMain(context, [name UTF8String], ex);
    HANDLE_EXCEPTION
}

- (void)bootFromDataFile:(NSString*)name{
    context->saveloadParam=[name UTF8String];
    fisceBootFromData(context, ex);
    HANDLE_EXCEPTION
}

- (void)saveToDataFile:(NSString*)name{
    context->saveloadParam=[name UTF8String];
    fisceSave(context, ex);
    HANDLE_EXCEPTION
}

- (void)runWithMessageHolder:(FiScEMessage*)m{
    fisceRun(context, message, ex);
    if(ex->exceptionType != exception_none){
        NSLog(@"Exception occored");
    }else{
        m.messageType = message->messageType;
        if(message->threadId){
            m.threadId = message->threadId;
        }else{
            m.threadId = -1;
        }
        switch (message->messageType) {
            case message_invoke_native:
                m.nativeCallName = [self getCachedNSStringWith:message->body.call.methodName];
                m.paramCount = message->body.call.paramCount;
                m.params = (uint32_t*)message->body.call.params;
                break;
            case message_sleep:
                m.sleepTime = message->body.sleepTime/1000.0;
                break;
            case message_vm_dead:
                break;
            case message_none:
            case message_thread_dead:
            default:
                NSLog(@"Exception occored");
                break;
        }
    }
}
@end

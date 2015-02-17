//
//  fiscevm_ios.h
//  fiscevm-ios
//
//  Created by Cloudee on 10/31/14.
//  Copyright (c) 2014 Cloudee. All rights reserved.
//

#ifndef fiscevm_ios_FiScEVM_h
#define fiscevm_ios_FiScEVM_h

#import <Foundation/Foundation.h>
#import <stdint.h>

typedef NS_ENUM(int32_t, FiScEReturnType){
    FISCE_RETURN_NONE,
    FISCE_RETURN_INT,
    FISCE_RETURN_LONG,
    FISCE_RETURN_HANDLE
};

@interface FiScEClass : NSObject
@property NSString* name;
@end

@interface FiScEMethod : NSObject
@property NSString* uniqueName;
@end

@interface FiScEField : NSObject
@property NSString* uniqueName;
@end

@interface FiScEMessage : NSObject
@property (nonatomic) int32_t messageType;
@property (nonatomic) int32_t threadId;
@property (nonatomic) NSString *nativeCallName;
@property (nonatomic) int32_t paramCount;
@property (nonatomic) uint32_t *params;
@property (nonatomic) NSString *exceptionName;
@property (nonatomic) NSString *exceptionDesc;
@property (nonatomic) NSTimeInterval sleepTime;
@end

@protocol FiScELogger <NSObject>
- (void)logWithLevel:(int32_t)level content:(NSString*)content;
@end

@interface FiScEVM: NSObject

@property id<FiScELogger> systemOutLogger;
@property id<FiScELogger> systemErrLogger;

- (instancetype)initWithClassPaths:(NSArray*)cps;

- (void)setSystemOutLogger:(id<FiScELogger>)logger;
- (void)setSystemErrLogger:(id<FiScELogger>)logger;
- (void)appendLog:(NSString*)channel content:(char)content;

- (void)returnIntValue:(int32_t)value
              toThread:(int32_t)threadId;

- (void)returnHandleValue:(int32_t)value
                 toThread:(int32_t)threadId;

- (void)returnLongValue:(int64_t)value
               toThread:(int32_t)threadId;

- (void)returnFloatValue:(float_t)value
                toThread:(int32_t)threadId;

- (void)returnDoubleValue:(double_t)value
                 toThread:(int32_t)threadId;


- (uint32_t)prepareThrowableWithClass:(NSString*)className
                             message:(NSString*)message
                           forThread:(int32_t)threadId;

- (FiScEClass*)lookupClassWithName:(NSString*)name;
- (FiScEField*)getFieldByName: (NSString*)name;
- (FiScEField*)lookupFieldVirtualFromClass: (FiScEClass*)clazz name:(NSString*)name;
- (FiScEMethod*)getMethodByName: (NSString*)name;
- (FiScEMethod*)lookupMethodVirtualFromClass:(FiScEClass*)clazz name:(NSString*)name;

- (FiScEClass*)getClassOfObject:(int32_t)handle;

- (uint32_t)allocate:(FiScEClass*) clazz;
- (uint32_t)allocateArray:(FiScEClass*)clazz length:(int32_t)length;

- (int32_t)arrayLengthOf:(int32_t)handle;
- (void)arrayCopyFrom:(int32_t)src fromPos:(int32_t)srcPos to:(int32_t)dest toPos:(int32_t)destPos length:(int32_t)len;
- (int32_t)cloneObject:(int32_t)handle;

- (NSString*)getStringFromObject:(int32_t)handle;
- (int32_t)makeString:(NSString*)string;
- (int32_t)literalString:(NSString*)string;

- (void)beginProtect;
- (void)endProtect;

- (boolean_t)getBooleanFromArray:(int32_t)handle index:(int32_t)index;
- (int32_t)getHandleFromArray:(int32_t)handle index:(int32_t)index;
- (int8_t)getByteFromArray:(int32_t)handle index:(int32_t)index;
- (int16_t)getShortFromArray:(int32_t)handle index:(int32_t)index;
- (uint16_t)getCharFromArray:(int32_t)handle index:(int32_t)index;
- (int32_t)getIntFromArray:(int32_t)handle index:(int32_t)index;
- (int64_t)getLongFromArray:(int32_t)handle index:(int32_t)index;
- (float_t)getFloatFromArray:(int32_t)handle index:(int32_t)index;
- (double_t)getDoubleFromArray:(int32_t)handle index:(int32_t)index;

- (int8_t*)getbytesFromArray:(int32_t)handle;

- (void)putBoolean:(boolean_t)value toArray:(int32_t)handle index:(int32_t)index;
- (void)putHandle:(int32_t)value toArray:(int32_t)handle index:(int32_t)index;
- (void)putByte:(int8_t)value toArray:(int32_t)handle index:(int32_t)index;
- (void)putShort:(int16_t)value toArray:(int32_t)handle index:(int32_t)index;
- (void)putChar:(uint16_t)value toArray:(int32_t)handle index:(int32_t)index;
- (void)putInt:(int32_t)value toArray:(int32_t)handle index:(int32_t)index;
- (void)putLong:(int64_t)value toArray:(int32_t)handle index:(int32_t)index;
- (void)putFloat:(float_t)value toArray:(int32_t)handle index:(int32_t)index;
- (void)putDouble:(double_t)value toArray:(int32_t)handle index:(int32_t)index;

- (boolean_t)getBooleanFromObject:(int32_t)handle field:(FiScEField*)field;
- (int32_t)getHandleFromObject:(int32_t)handle field:(FiScEField*)field;
- (int8_t)getByteFromObject:(int32_t)handle field:(FiScEField*)field;
- (int16_t)getShortFromObject:(int32_t)handle field:(FiScEField*)field;
- (uint16_t)getCharFromObject:(int32_t)handle field:(FiScEField*)field;
- (int32_t)getIntFromObject:(int32_t)handle field:(FiScEField*)field;
- (int64_t)getLongFromObject:(int32_t)handle field:(FiScEField*)field;
- (float_t)getFloatFromObject:(int32_t)handle field:(FiScEField*)field;
- (double_t)getDoubleFromObject:(int32_t)handle field:(FiScEField*)field;
- (int8_t*)getbytesFromObject:(int32_t)handle field:(FiScEField*)field;

- (void)putBoolean:(boolean_t)value toObject:(int32_t)handle field:(FiScEField*)field;
- (void)putHandle:(int32_t)value toObject:(int32_t)handle field:(FiScEField*)field;
- (void)putByte:(int8_t)value toObject:(int32_t)handle field:(FiScEField*)field;
- (void)putShort:(int16_t)value toObject:(int32_t)handle field:(FiScEField*)field;
- (void)putChar:(uint16_t)value toObject:(int32_t)handle field:(FiScEField*)field;
- (void)putInt:(int32_t)value toObject:(int32_t)handle field:(FiScEField*)field;
- (void)putLong:(int64_t)value toObject:(int32_t)handle field:(FiScEField*)field;
- (void)putFloat:(float_t)value toObject:(int32_t)handle field:(FiScEField*)field;
- (void)putDouble:(double_t)value toObject:(int32_t)handle field:(FiScEField*)field;

- (boolean_t)getBooleanFromStaticField:(FiScEField*)field;
- (int32_t)getHandleFromStaticField:(FiScEField*)field;
- (int8_t)getByteFromStaticField:(FiScEField*)field;
- (int16_t)getShortFromStaticField:(FiScEField*)field;
- (uint16_t)getCharFromStaticField:(FiScEField*)field;
- (int32_t)getIntFromStaticField:(FiScEField*)field;
- (int64_t)getLongFromStaticField:(FiScEField*)field;
- (float_t)getFloatFromStaticField:(FiScEField*)field;
- (double_t)getDoubleFromStaticField:(FiScEField*)field;
- (int8_t*)getbytesFromStaticField:(FiScEField*)field;

- (void)putBoolean:(boolean_t)value toStaticField:(FiScEField*)field;
- (void)putHandle:(int32_t)value toStaticField:(FiScEField*)field;
- (void)putByte:(int8_t)value toStaticField:(FiScEField*)field;
- (void)putShort:(int16_t)value toStaticField:(FiScEField*)field;
- (void)putChar:(uint16_t)value toStaticField:(FiScEField*)field;
- (void)putInt:(int32_t)value toStaticField:(FiScEField*)field;
- (void)putLong:(int64_t)value toStaticField:(FiScEField*)field;
- (void)putFloat:(float_t)value toStaticField:(FiScEField*)field;
- (void)putDouble:(double_t)value toStaticField:(FiScEField*)field;

- (void)unRegisterNativeHandlerWithName:(NSString*)name;

- (void)bootFromMainClassWithName:(NSString*)name;
- (void)bootFromDataFile:(NSString*)name;
- (void)saveToDataFile:(NSString*)name;
- (void)runWithMessageHolder:(FiScEMessage*)message;

- (void)handleGcEventWithData:(void*)data before:(void (*)(void*))beforeHandler getExtra:(void (*)(void*, int32_t*, int32_t**))getExtraHandler after:(void (*)(void*))afterHandler;
@end

#endif

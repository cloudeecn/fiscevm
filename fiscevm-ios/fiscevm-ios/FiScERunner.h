//
//  FiScERunner.h
//  fiscevm-ios
//
//  Created by Cloudee on 11/13/14.
//  Copyright (c) 2014 Cloudee. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "FiScEVM.h"

@class FiScERunner;

typedef void (^FiScENativeHandler)(FiScERunner*, FiScEVM*, int32_t, int32_t, uint32_t*);
@class FiScERunner;

@protocol FiScERunnerDelegate <NSObject>

- (boolean_t)runneEncountsUnhandledNativeMethod:(FiScERunner*)runner name:(NSString*)name paramsCount:(int32_t)paramsCount params:(uint32_t)params;

- (void)runnerCollectsExtraObjectsForGC:(FiScERunner*)runner withCollector:(void (^)(int32_t))handleFiller;

- (void)runnerEncountersException:(FiScERunner*)runner name:(NSString*)name description:(NSString*)desc threadId:(int32_t)threadId;

@end

@interface FiScERunner : NSObject
@property NSArray *classPaths;
@property NSString *dataPath;
@property NSString *savePath;
@property NSString *autoSavePath;
@property NSString *mainClassName;
@property id<FiScERunnerDelegate> delegate;
@property dispatch_queue_t runnerQueue;

@end

@interface FiScERunner (Methods)

- (void)lockGCWithBlock:(void (^)())block;
- (void)handleNativeMethod:(NSString*)methodName withBlock:(FiScENativeHandler)block;
- (void)start;
- (void)resume;
- (void)loadStatus:(NSString*)fileName;
- (void)saveStatus:(NSString*)fileName;
- (void)pauseSoft;
- (void)pauseForce;
@end

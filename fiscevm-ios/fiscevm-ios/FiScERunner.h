//
//  FiScERunner.h
//  fiscevm-ios
//
//  Created by Cloudee on 11/13/14.
//  Copyright (c) 2014 Cloudee. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "FiScEVM.h"

typedef void (^FiScENativeHandler)(FiScEVM*, int32_t, int32_t, uint32_t*);

@interface FiScERunner : NSObject
@property NSString *dataPath;
@property NSString *savePath;
@property NSString *autoSavePath;
@property NSString *mainClassName;
@property dispatch_queue_t runnerQueue;

- (void)handleNativeMethod:(NSString*)methodName withBlock:(FiScENativeHandler)block;
- (void)start;
- (void)resume;
- (void)loadStatus:(NSString*)fileName;
- (void)saveStatus:(NSString*)fileName;
- (void)pauseSoft;
- (void)pauseForce;
@end

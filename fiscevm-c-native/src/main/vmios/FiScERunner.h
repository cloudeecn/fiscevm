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

#import <Foundation/Foundation.h>
#import "FiScEVM.h"

@class FiScERunner;

typedef void (^FiScENativeHandler)(FiScERunner*, FiScEVM*, int32_t, int32_t, uint32_t*);
@class FiScERunner;

@protocol FiScERunnerDelegate <NSObject>

- (boolean_t)runnerEncountsUnhandledNativeMethod:(FiScERunner*)runner name:(NSString*)name paramsCount:(int32_t)paramsCount params:(uint32_t)params;

- (void)runnerCollectsExtraObjectsForGC:(FiScERunner*)runner withCollector:(void (^)(int32_t))handleFiller;

- (void)runnerEncountersException:(FiScERunner*)runner name:(NSString*)name description:(NSString*)desc threadId:(int32_t)threadId;

- (void)runnerStopped:(FiScERunner*)runner;

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

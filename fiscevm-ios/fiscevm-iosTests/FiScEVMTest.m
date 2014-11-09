//
//  FiScEVMTest.m
//  fiscevm-ios
//
//  Created by Cloudee on 11/9/14.
//  Copyright (c) 2014 Cloudee. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>
#import "FiScEVM.h"
#import "fisce.h"

@interface FiScEVMTest : XCTestCase

@end

@implementation FiScEVMTest

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)hltest:(NSString*)className{
    @autoreleasepool {
        boolean_t run=true;
        FiScEVM *vm=[[FiScEVM alloc] init];
        FiScEMessage *message=[[FiScEMessage alloc] init];
        
        [vm bootFromMainClassWithName:className];
        while(run){
            @autoreleasepool {
                [vm runWithMessageHolder:message];
                switch ([message messageType]) {
                    case 3: //message_invoke_native:
                        @throw [[NSException alloc] initWithName:@"FyException" reason:[NSString stringWithFormat:@"Unhandled native invoke: %@" , [message nativeCallName]] userInfo:nil];
                        break;
                    case 4: //message_exception:
                        @throw [[NSException alloc] initWithName:@"FyException" reason:[NSString stringWithFormat:@"Exception %@ occored: %@" , [message exceptionName], [message exceptionDesc]] userInfo:nil];
                        break;
                    case 5: //message_sleep:
                        [NSThread sleepForTimeInterval:[message sleepTime]/1000.0];
                        break;
                    case 6: //message_vm_dead:
                        run=false;
                        break;
                    default:
                        @throw [[NSException alloc] initWithName:@"FyException" reason:[NSString stringWithFormat:@"Illegal message type: %d" , [message messageType]] userInfo:nil];
                }
            }
        }
    }
}

- (void)testPortable{
    XCTAssertTrue(fy_portValidate());
}

- (void)testArchitectureTest {
    [self hltest:@"EXCLUDE/fisce/test/ArchitectureTest"];
}

- (void)testHierarchyTest {
    [self hltest:@"EXCLUDE/fisce/test/HierarchyTest"];
}

- (void)testUnicodeTest {
    [self hltest:@"EXCLUDE/fisce/test/UnicodeTest"];
}

- (void)testHelloWorld {
    [self hltest:@"EXCLUDE/fisce/test/HelloWorld"];
}

- (void)testRandomBoundTest {
    [self hltest:@"EXCLUDE/fisce/test/RandomBoundTest"];
}

- (void)testTester {
    [self hltest:@"EXCLUDE/fisce/test/Tester"];
}

- (void)testArrayTest {
    [self hltest:@"EXCLUDE/fisce/test/ArrayTest"];
}

- (void)testDebugPrintStreamTest {
    [self hltest:@"EXCLUDE/fisce/test/DebugPrintStreamTest"];
}

- (void)testAutoBoxingTest {
    [self hltest:@"EXCLUDE/fisce/test/AutoBoxingTest"];
}

- (void)testRunnerTester {
    [self hltest:@"EXCLUDE/fisce/test/RunnerTester"];
}

- (void)testEnumTester {
    [self hltest:@"EXCLUDE/fisce/test/EnumTester"];
}

- (void)testExceptionTester {
    [self hltest:@"EXCLUDE/fisce/test/ExceptionTester"];
}

- (void)testForEachTest {
    [self hltest:@"EXCLUDE/fisce/test/ForEachTest"];
}

- (void)testHashMapTest {
    [self hltest:@"EXCLUDE/fisce/test/HashMapTest"];
}

- (void)testStaticTest {
    [self hltest:@"EXCLUDE/fisce/test/StaticTest"];
}

- (void)testComplexClassLayout {
    [self hltest:@"EXCLUDE/fisce/test/ComplexClassLayout"];
}

- (void)testSwitchTest {
    [self hltest:@"EXCLUDE/fisce/test/SwitchTest"];
}

- (void)testSwitchTest2 {
    [self hltest:@"EXCLUDE/fisce/test/SwitchTest2"];
}

- (void)testGCTest {
    [self hltest:@"EXCLUDE/fisce/test/GCTest"];
}

- (void)testRISTest {
    [self hltest:@"EXCLUDE/fisce/test/RISTest"];
}

- (void)testParamStoreTest {
    [self hltest:@"EXCLUDE/fisce/test/ParamStoreTest"];
}

- (void)testReflectionTest {
    [self hltest:@"EXCLUDE/fisce/test/ReflectionTest"];
}

- (void)testProxyTest {
    [self hltest:@"EXCLUDE/fisce/test/ProxyTest"];
}

- (void)testGZIPTest {
    // [self hltest:@"EXCLUDE/fisce/test/GZIPTest"];
}

- (void)testProfile {
    [self hltest:@"com/cirnoworks/fisce/privat/Profile"];
}

- (void)testLinpack {
    [self hltest:@"com/cirnoworks/fisce/privat/Linpack"];
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end

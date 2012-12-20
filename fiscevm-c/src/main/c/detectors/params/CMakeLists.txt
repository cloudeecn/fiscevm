cmake_minimum_required (VERSION 2.6)
project (params-test)

ADD_EXECUTABLE(simple simple.c)
set_target_properties(simple
    PROPERTIES
		COMPILE_FLAGS "${TEST_PARAM}"
		LINK_FLAGS "${TEST_PARAM}"
)


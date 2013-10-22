add_executable(core_test Core_Tests.cpp)	
target_link_libraries(core_test ${gtest} ${libtcmalloc} core)	
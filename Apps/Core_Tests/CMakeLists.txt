add_executable(Core_Tests Core_Tests.cpp)	
target_link_libraries(Core_Tests ${gtest} ${libtcmalloc} Core)	
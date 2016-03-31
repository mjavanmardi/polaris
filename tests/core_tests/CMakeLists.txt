add_executable(Core_Tests Core_Tests.cpp)	
target_link_libraries(Core_Tests ${GTEST_LIBRARY} ${libtcmalloc} Core)
set_target_properties(Core_Tests PROPERTIES FOLDER tests)
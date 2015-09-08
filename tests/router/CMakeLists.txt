add_executable(router_tests router_tests.cpp)	
target_link_libraries(router_tests Routing ${GTEST_LIBRARIES})
set_target_properties(router_tests PROPERTIES FOLDER tests)
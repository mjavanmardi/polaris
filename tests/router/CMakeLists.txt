add_executable(router_tests router_tests.cpp)	
target_link_libraries(router_tests ${gtest} Routing)
set_target_properties(router_tests PROPERTIES FOLDER tests)
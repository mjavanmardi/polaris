add_executable(router_tests router_tests.cpp)	
target_link_libraries(router_tests ${GTEST_LIBRARY} Routing)
set_target_properties(router_tests PROPERTIES FOLDER tests)

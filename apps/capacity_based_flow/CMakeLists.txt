file(GLOB_RECURSE cppfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)	
add_executable(capacity_based_flow ${cppfiles})
target_link_libraries(capacity_based_flow ${iolibs} Scenario_Manager)
set_target_properties(capacity_based_flow PROPERTIES FOLDER apps)
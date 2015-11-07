file(GLOB_RECURSE hppfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)	
file(GLOB_RECURSE cppfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)	
add_executable(Test_Application Test.cpp)
#target_link_libraries(Test_Application ${libtcmalloc} ${iolibs} Scenario_Manager Core Routing ${antareslibs})

if (MSVC AND Antares)
	target_link_libraries(Test_Application ${libtcmalloc} ${iolibs} Scenario_Manager Core Routing ${antareslibs} ${libtcmalloc} ${antareslibs} ${iolibs} Scenario_Manager Core Routing Visual_Traffic_Simulator)
else()
	target_link_libraries(Test_Application ${libtcmalloc} Core )
endif()

set_target_properties(Test_Application PROPERTIES FOLDER apps)
file(GLOB_RECURSE hppfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)	
file(GLOB_RECURSE cppfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)	
if (MSVC AND Antares)
	set(MODULE_NAME Fixed_Demand_Antares)
	add_executable(${MODULE_NAME} NetworkModel.cpp)
	target_link_libraries(${MODULE_NAME} ${libtcmalloc} ${antareslibs} ${iolibs} Scenario_Manager Core Routing Visual_Traffic_Simulator)
elseif(UNIX)
	set(MODULE_NAME Fixed_Demand_Simulator)
	add_executable(${MODULE_NAME} NetworkModel.cpp)
	target_link_libraries(${MODULE_NAME} ${libtcmalloc} ${iolibs} rt Scenario_Manager Core Routing)
else()
	set(MODULE_NAME Fixed_Demand_Simulator)
	add_executable(${MODULE_NAME} NetworkModel.cpp)
	target_link_libraries(${MODULE_NAME} ${libtcmalloc} ${iolibs} Scenario_Manager Core Routing)
endif()

set_target_properties(${MODULE_NAME} PROPERTIES FOLDER apps)

if(MSVC)
	add_custom_command( TARGET ${MODULE_NAME} POST_BUILD 
		COMMAND xcopy /D /I /Y \"${DEPS_BINARY_DIR}\\$<CONFIGURATION>\\*\" \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>\"		
		COMMENT "Copying third party dlls to output directory" )
endif()
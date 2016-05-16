file(GLOB_RECURSE hppfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)	
file(GLOB_RECURSE cppfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
set(SOURCE Test.cpp myStuff.cpp)
set(HEADERS version.h myAgent.h myStuff.h)
set(MODULE_NAME Randy_Test)
add_executable(${MODULE_NAME} ${SOURCE} ${HEADERS})
#target_link_libraries(${MODULE_NAME} ${libtcmalloc} ${iolibs} Scenario_Manager Core Routing ${antareslibs})

target_link_libraries(${MODULE_NAME} Core )

set_target_properties(${MODULE_NAME} PROPERTIES FOLDER apps)

if(MSVC)
	add_custom_command( TARGET ${MODULE_NAME} POST_BUILD 
		COMMAND xcopy /D /I /Y \"${DEPS_BINARY_DIR}\\$<CONFIGURATION>\\*\" \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>\"		
		COMMENT "Copying third party dlls to output directory" )
endif()
set(MODULE_NAME Demo)
add_executable(${MODULE_NAME} Demo.cpp)	
target_link_libraries(${MODULE_NAME} ${libtcmalloc} ${antareslibs} Core)
set_target_properties(${MODULE_NAME} PROPERTIES FOLDER apps)

if(MSVC)
	add_custom_command( TARGET ${MODULE_NAME} POST_BUILD 
		COMMAND xcopy /D /I /Y \"${DEPS_BINARY_DIR}\\$<CONFIGURATION>\\*\" \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>\"		
		COMMAND xcopy /D /I /Y \"${ANTARES_DEPS_BINARY_DIR}\\$<CONFIGURATION>\\*\" \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>\"		
		COMMENT "Copying third party dlls to output directory" )
endif()
file(GLOB_RECURSE hppfiles
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	*.h)	
add_library(antares Antares.cpp ${hppfiles})
target_link_libraries(antares ${iolibs} ${Glew})	
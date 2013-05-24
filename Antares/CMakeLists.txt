find_library(Glew glew32s)

MESSAGE(STATUS "GLEW_FOUND:         " ${Glew} )


file(GLOB_RECURSE hppfiles
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	*.h)	
add_library(antares Antares.cpp ${hppfiles})
target_link_libraries(antares io ${Odb} ${Sqlite3} ${Odb_Sqlite} ${Glew})	
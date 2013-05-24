file(GLOB_RECURSE cppfiles
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	*.cpp)	
file(GLOB_RECURSE hppfiles
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	*.h)	
add_executable(tmc ${cppfiles} ${hppfiles} )
target_link_libraries(tmc io ${Odb} ${Sqlite3} ${Odb_Sqlite})

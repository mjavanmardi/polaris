file(GLOB_RECURSE hppfiles
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	*.h)	
file(GLOB_RECURSE cppfiles
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	*.cpp)		
add_executable(application ${cppfiles} ${hppfiles})
target_link_libraries(application antares userspace io scenariomanager ${Odb} ${Sqlite3} ${Odb_Sqlite} ${Glew})	
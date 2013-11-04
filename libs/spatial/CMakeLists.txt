file(GLOB_RECURSE cppfiles
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	*.cpp)	
file(GLOB_RECURSE hppfiles
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	*.h)	

add_library(Spatial ${hppfiles} ${cppfiles})
target_link_libraries(Spatial ${Sqlite3})
set_target_properties(Spatial PROPERTIES LINKER_LANGUAGE CXX)

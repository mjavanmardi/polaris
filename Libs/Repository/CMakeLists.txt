file(GLOB_RECURSE hppfiles
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	*.h)	
add_library(Repository Repository.cpp ${hppfiles})
target_link_libraries(Repository)	
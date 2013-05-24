file(GLOB_RECURSE hppfiles
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	*.h)	
add_library(repository Repository.cpp RngStream.cpp ${hppfiles})
target_link_libraries(repository)	
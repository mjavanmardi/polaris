file(GLOB_RECURSE hppfiles
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	*.h)	
add_library(userspace User_Space.cpp ${hppfiles})
target_link_libraries(userspace)	
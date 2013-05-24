file(GLOB_RECURSE hppfiles
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	*.h)	
add_library(core Core.cpp ${hppfiles})
target_link_libraries(io)	
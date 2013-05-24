file(GLOB_RECURSE hppfiles
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	*.h)	
file(GLOB_RECURSE cppfiles
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	*.cpp)		
add_executable(scenariomanager ${cppfiles} ${hppfiles})
add_library(scenariomanager Repository.cpp RngStream.cpp ${hppfiles})
target_link_libraries(scenariomanager)	
file(GLOB_RECURSE hppfiles
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	*.h)	
# file(GLOB_RECURSE cppfiles
	# RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	# *.cpp)
add_library(scenariomanager cfg_reader.cpp json_reader.cpp json_value.cpp  json_writer.cpp ${hppfiles})
file(GLOB_RECURSE hppfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)	
add_library(flow_lib ${hppfiles})
set_target_properties(flow_lib PROPERTIES FOLDER libs)
SET_TARGET_PROPERTIES(flow_lib PROPERTIES LINKER_LANGUAGE CXX)
# file(GLOB_RECURSE cppfiles
	# RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	# *.cpp)	
# file(GLOB_RECURSE hppfiles
	# RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	# *.h)	
add_executable(tmc test.cpp Tmc.h DataStructures.h DFS.h WeakCC.h Detector1DU.h Clustering.h)
target_link_libraries(tmc ${iolibs})

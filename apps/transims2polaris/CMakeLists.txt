file(GLOB_RECURSE cppfiles
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	*.cpp)	
file(GLOB_RECURSE hppfiles
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	*.h)	
include_directories(C:/Users/vsokolov/usr/transims/sf-version5/branches/aecom/Transims50/SysLib/Include/)
if (NOT MT)
	find_library(Syslib NAMES SysLib)
else()
	find_library(Syslib NAMES SysLib-mt)
endif()
add_executable(transims2polaris ${cppfiles} ${hppfiles})
target_link_libraries(transims2polaris ${iolibs} ${Syslib})

file(GLOB_RECURSE hppfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)	
file(GLOB_RECURSE cppfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)	
set(MODULE_NAME Integrated_Model)
add_executable(${MODULE_NAME} Integrated_Model.cpp)	
if (MSVC AND Antares)
	target_link_libraries(${MODULE_NAME} ${libtcmalloc} ${antareslibs} ${iolibs} Scenario_Manager Core Routing Visual_Traffic_Simulator)
else()
	target_link_libraries(${MODULE_NAME} ${libtcmalloc} ${antareslibs} ${iolibs} Scenario_Manager Core Routing)
endif()

set_target_properties(${MODULE_NAME} PROPERTIES FOLDER apps)

if(MSVC)
	# Configure the template file
	MESSAGE(INFO " ${MODULE_NAME} - DEPS_BINARY_DIR=${DEPS_BINARY_DIR}")
	SET(USER_FILE ${MODULE_NAME}.vcxproj.user)
	SET(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE})
	CONFIGURE_FILE(../../ProjectUserTemplate.vcxproj ${USER_FILE})
endif()
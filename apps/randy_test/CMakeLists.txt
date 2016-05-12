file(GLOB_RECURSE hppfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)	
file(GLOB_RECURSE cppfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
set(SOURCE Test.cpp myStuff.cpp)
set(HEADERS version.h myAgent.h myStuff.h)
set(MODULE_NAME Randy_Test)
add_executable(${MODULE_NAME} ${SOURCE} ${HEADERS})
#target_link_libraries(${MODULE_NAME} ${libtcmalloc} ${iolibs} Scenario_Manager Core Routing ${antareslibs})

target_link_libraries(${MODULE_NAME} Core )

set_target_properties(${MODULE_NAME} PROPERTIES FOLDER apps)

if(MSVC)
	# Configure the template file
	MESSAGE(INFO " ${MODULE_NAME} - DEPS_BINARY_DIR=${DEPS_BINARY_DIR}")
	SET(USER_FILE ${MODULE_NAME}.vcxproj.user)
	SET(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE})
	CONFIGURE_FILE(../../ProjectUserTemplate.vcxproj ${USER_FILE})
endif()
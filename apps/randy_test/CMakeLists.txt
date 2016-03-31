file(GLOB_RECURSE hppfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)	
file(GLOB_RECURSE cppfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
set(SOURCE Test.cpp myStuff.cpp)
set(HEADERS version.h myAgent.h myStuff.h)	
add_executable(Randy_Test ${SOURCE} ${HEADERS})
#target_link_libraries(Randy_Test ${libtcmalloc} ${iolibs} Scenario_Manager Core Routing ${antareslibs})

target_link_libraries(Randy_Test Core )

set_target_properties(Randy_Test PROPERTIES FOLDER apps)
add_executable(Demo Demo.cpp)	
target_link_libraries(Demo ${libtcmalloc} ${antareslibs} Core)
set_target_properties(Demo PROPERTIES FOLDER apps)
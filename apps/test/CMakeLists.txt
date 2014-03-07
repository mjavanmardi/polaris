add_executable(Test_Application Test.cpp)	
target_link_libraries(Test_Application ${libtcmalloc} ${antareslibs} Core)
set_target_properties(Test_Application PROPERTIES FOLDER apps)
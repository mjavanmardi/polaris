add_executable(Test_Application Compute_Route.cpp)	
target_link_libraries(Test_Application ${gtest} ${libtcmalloc} Routing Core ${iolibs})
set_target_properties(Test_Application PROPERTIES FOLDER apps)
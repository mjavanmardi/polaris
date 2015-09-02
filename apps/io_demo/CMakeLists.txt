add_executable(Io_Demo io_demo.cpp)
target_link_libraries(Io_Demo ${iolibs})
set_target_properties(Io_Demo PROPERTIES FOLDER apps)


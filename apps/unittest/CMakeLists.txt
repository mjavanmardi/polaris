include_directories ("${PROJECT_SOURCE_DIR}")
add_executable(unittest unittest.cpp clustering.cpp sampling.cpp depot.cpp)

target_link_libraries(unittest ${Gtest} ${iolibs})
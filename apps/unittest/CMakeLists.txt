include_directories ("${PROJECT_SOURCE_DIR}")
add_executable(unittest main.cpp clustering.cpp)

target_link_libraries(unittest ${Gtest} ${iolibs})
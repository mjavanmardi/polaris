include_directories ("${PROJECT_SOURCE_DIR}")
add_executable(db_unittest db_unittest.cpp)

target_link_libraries(db_unittest ${Gtest} ${iolibs})
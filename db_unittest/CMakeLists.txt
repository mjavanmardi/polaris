include_directories ("${PROJECT_SOURCE_DIR}")
add_executable(db_unittest main.cpp)

target_link_libraries(db_unittest io ${Gtest} ${Odb} ${Sqlite3} ${Odb_Sqlite})
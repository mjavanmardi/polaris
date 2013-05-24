include_directories ("${PROJECT_SOURCE_DIR}")
add_executable(unittest main.cpp)

target_link_libraries(unittest io ${Gtest} ${Odb} ${Sqlite3} ${Odb_Sqlite})
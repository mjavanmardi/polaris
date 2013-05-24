PROJECT(POLARIS CXX)
cmake_minimum_required (VERSION 2.6) 
include_directories(C:/opt/polarisdeps/include)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(~/gtest-1.6.0/include)
include_directories(~/odb/include)


if (UNIX)
	set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /mnt/lustre/home/vsokolov/gtest-1.6.0:/mnt/lustre/home/vsokolov/odb/lib)
else()
	set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} c:/opt/polarisdeps/x64/lib;C:/wxWidgets-2.9.3/lib/vc_lib;C:/glew-1.7.0/lib)
endif()


find_library(Gtest NAMES gtest  HINTS /mnt/lustre/home/vsokolov/gtest-1.6.0)
find_library(Odb NAMES  odb HINTS /mnt/lustre/home/vsokolov/odb/lib)
find_library(Odb_Sqlite odb-sqlite HINTS /mnt/lustre/home/vsokolov/odb/lib)
find_library(Sqlite3 sqlite3)


MESSAGE(STATUS "Gtest_FOUND:         " ${Gtest} )
MESSAGE(STATUS "CMAKE_LIBRARY_PATH:         " ${CMAKE_LIBRARY_PATH} )

if (UNIX)
  message(status "Setting GCC flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message(status "Setting MSVC flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /openmp")
endif()

add_subdirectory (Io) 
add_subdirectory (db_unittest) 
add_subdirectory (unittest) 
add_subdirectory (Core) 
add_subdirectory (Repository) 
add_subdirectory (File_IO) 
add_subdirectory (ItsMockup) 
add_subdirectory (tmc) 
add_subdirectory (Antares) 
add_subdirectory (User_Space) 
add_subdirectory (Application) 

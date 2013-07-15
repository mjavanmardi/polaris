# CMake build script for Polaris.
# small letters are used for the folder/project names throughout the build 
PROJECT(polaris CXX)
cmake_minimum_required (VERSION 2.6) 



option(NetworkModelApplication		"Build only network model"			OFF)
option(Antares						"Build with Antares"				OFF)
option(IntegratedModelApplication	"Build integrated model"			OFF)
option(ItsMockup					"Build ItsMockup project"			OFF)
option(Tests						"Build Unit Tests"					ON)
option(MT							"Build with MSVC using /MT flag"	OFF)
option(ALL							"Build all of the projects"			OFF)

if (ALL)
	set(Antares ON)
	set(IntegratedModelApplication ON)
	set(ItsMockup ON)
	set(Tests ON)
endif()

# Define helper functions and macros
include(cmake/internal_utils.cmake)

# We require gcc version of 4.7 or greater. In the versions below 4.7 we found some problems with random number generators.
# Also in this version shared_ptr and unique_ptr used heavily in io library are in std versus tr1 
# If version < 4.7 is detected then script stops
if (CMAKE_COMPILER_IS_GNUCXX)
	execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
					OUTPUT_VARIABLE GCC_VERSION)
	if (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
			message(STATUS "Version >= 4.7")
	else()
		message(FATAL_ERROR "The version of the gcc compiler is < 4.7")
	endif()
endif()

# DBIO is the flag used for backward compatibility with the legacy File_IO
if (IntegratedModelApplication)
	ADD_DEFINITIONS( -DDBIO )
	ADD_DEFINITIONS( -DIntegratedModelApplication)
endif()
if (NetworkModelApplication)
	ADD_DEFINITIONS( -DNetworkModelApplication)	
endif()
if (Antares)
	ADD_DEFINITIONS( -DANTARES)
	set(MT ON)
endif()


if (UNIX)
	set(DEPS_DIR /mnt/lustre/home/vsokolov/opt)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
	set(DEPS_DIR C:/opt/polarisdeps)
	set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} c:/opt/polarisdeps/x64/lib;C:/glew-1.7.0/lib;C:/plplot/lib)
	link_directories( ${LINK_DIRECTORIES} C:/wxWidgets-2.9.3/lib/vc_lib )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")	
endif()
	
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${DEPS_DIR}/gtest-1.6.0/include)
include_directories(${DEPS_DIR}/odb/include)
include_directories(${DEPS_DIR}/include)

# This need to be fixed, we  need to have all of the deps on a Windows machine in the same location
if (MSVC AND Antares)
	include_directories(C:/opt/polarisdeps/include)
	include_directories(C:/glew-1.7.0/include)
	include_directories(C:/plplot/include/plplot)
	include_directories(C:/wxWidgets-2.9.3/include)
endif()

message(STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})
message(STATUS "CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR})

if (Tests)
    find_library(Gtest NAMES gtest  HINTS ${DEPS_DIR}/gtest-1.6.0)
endif()

if (NOT MT)
	find_library(Odb NAMES  odb HINTS /mnt/lustre/home/vsokolov/opt/odb/lib)
	find_library(Odb_Sqlite odb-sqlite HINTS /mnt/lustre/home/vsokolov/opt/odb/lib)
	find_library(Sqlite3 sqlite3 HINTS  /usr/lib64 REQUIRED)
	if (UNIX)
		find_library(Odb-d NAMES  odb HINTS /mnt/lustre/home/vsokolov/opt/odb/lib)
		find_library(Odb_Sqlite-d odb-sqlite HINTS /mnt/lustre/home/vsokolov/opt/odb/lib)
		find_library(Sqlite3-d sqlite3 HINTS  /usr/lib64 REQUIRED)
	else()
		find_library(Odb-d NAMES  odb-d HINTS /mnt/lustre/home/vsokolov/opt/odb/lib)
		find_library(Odb_Sqlite-d odb-sqlite-d HINTS /mnt/lustre/home/vsokolov/opt/odb/lib)
		find_library(Sqlite3-d sqlite3-d HINTS  /usr/lib64 REQUIRED)
	endif()
else()
	find_library(Odb NAMES  odb-mt HINTS /mnt/lustre/home/vsokolov/odb/lib)
	find_library(Odb-d NAMES  odb-d-mt HINTS /mnt/lustre/home/vsokolov/odb/lib)
	find_library(Odb_Sqlite odb-sqlite-mt HINTS /mnt/lustre/home/vsokolov/odb/lib)
	find_library(Odb_Sqlite-d odb-sqlite-d-mt HINTS /mnt/lustre/home/vsokolov/odb/lib)
	find_library(Sqlite3 sqlite3-mt HINTS  /usr/lib64 REQUIRED)
	find_library(Sqlite3-d sqlite3-d-mt HINTS  /usr/lib64 REQUIRED)
	
	set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
	set(CMAKE_CXX_FLAGS_RELEASE "/MT")
	set(CMAKE_CXX_FLAGS_MINSIZEREL "/MT")	 
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MTd")	 
	ADD_DEFINITIONS(-DLIBODB_STATIC_LIB)
	ADD_DEFINITIONS(-DLIBODB_SQLITE_STATIC_LIB)
	ADD_DEFINITIONS(-D_MBCS)
	# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /MT") #this is a hack and need to be fixed
endif()

set(iolibs io optimized ${Odb}  optimized ${Sqlite3} optimized ${Odb_Sqlite} debug ${Odb-d} debug ${Sqlite3-d} debug ${Odb_Sqlite-d})

if (Antares AND MSVC)
	find_library(Glew glew32s)
	add_subdirectory (Antares) 
endif()
if (Tests)
	add_subdirectory (apps/db_unittest) 
	add_subdirectory (apps/unittest) 
endif()
if (ItsMockup)
	add_subdirectory (apps/its_mockup) 
endif()
if (NetworkModelApplication OR IntegratedModelApplication)
	add_subdirectory (Application) 
endif()
if (MSVC)
	add_subdirectory (apps/transims2polaris)
endif()
add_subdirectory (Io) 
add_subdirectory (Core) 
add_subdirectory (Repository) 
add_subdirectory (File_IO) 
add_subdirectory (tmc)
add_subdirectory (spatial)
add_subdirectory (apps/lprobability)
add_subdirectory (User_Space) 

add_subdirectory (Scenario_Manager) 
add_subdirectory (apps/msevents) 


add_subdirectory (apps/depot)
add_subdirectory (apps/emptydb)

PROJECT(polaris CXX)
cmake_minimum_required(VERSION 2.8)

#Add capability to create visual studio solution folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#options to be used while running cmake
option(Antares "Build with Antares" OFF)
option(Popsyn "Build Popsyn App" OFF)
option(Statewide "Build Statewide Model App" OFF)
option(Coretests "Include Unit Tests for Core" OFF)
option(Unittest "Include Unit Tests for Other Libraries" OFF)
option(CapacityFlow "Include Traffic Flow Simulator Prototype" OFF)


set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER cmake)

#Variables designating where the output files go
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BUILD_DIR}/bin)


#Compiler flags for release mode applied to all projects
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 /MD /W2 /Oi /bigobj")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")

# %%%RLW - begine
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11")

if (MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Ob0 /MD /W2 /Oi /bigobj")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
elseif(MINGW)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj -std=c++11")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wa,-mbig-obj -std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wa,-mbig-obj -std=c++11")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations -pthread -D__forceinline=inline -m64 -std=c++11 -lrt")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
endif()
# %%%RLW - end


#Set up dependencies

#{
	#Variable to indicate where most external dependencies live
	#set(DEPS_DIR C:/opt/polarisdeps) %%%RLW
	set(DEPS_DIR $ENV{POLARIS_DEPS_DIR})

	#Variable to indicate where boost lives
	#Variable to indicate where boost lives
	set(BOOST_ROOT ${DEPS_DIR}/boost_1_60_0)
	if (MSVC10)
		set(BOOST_LIBRARYDIR ${BOOST_ROOT}/lib64-msvc-10.0)
	elseif (MSVC14)
		set(BOOST_LIBRARYDIR ${BOOST_ROOT}/lib64-msvc-14.0)
	elseif (MINGW)
		set(BOOST_LIBRARYDIR ${BOOST_ROOT}/lib64-mingw)
	endif()
	
	# boost
	set(Boost_NO_SYSTEM_PATHS true)
	#set (Boost_USE_STATIC_LIBS OFF CACHE BOOL "use static libraries from Boost")
	set (Boost_USE_STATIC_LIBS ON)
	# if (UNIX)
		set (Boost_USE_MULTITHREADED ON)
		find_package(Boost 1.60.0 REQUIRED)
		include_directories(${Boost_INCLUDE_DIRS})
	# else()
		# set (Boost_USE_MULTITHREADED ON)
		# find_package(Boost 1.60.0 REQUIRED 
		  # COMPONENTS
		  # chrono
		  # timer
		  # filesystem
		  # system
		  # program_options
		  # )
		# include_directories(${Boost_INCLUDE_DIRS})
		# link_libraries(${Boost_LIBRARIES})
	# endif()

	#TC_Malloc include and library dependencies
	#if (UNIX)
	#	include_directories(${DEPS_DIR}/gperftools-2.1/src)
	#	find_library(libtcmalloc NAMES libtcmalloc_minimal libtcmalloc_minimal.a  HINT "/usr/lib/" ${DEPS_DIR}/gperftools-2.1/Release)
	#	message(STATUS "" ${libtcmalloc})
	#endif()
	
	#if (MSVC)
	#	add_definitions(/Zi)
	#	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG")
	#	include_directories(${DEPS_DIR}/gperftools-2.1/src)
	#	find_library(libtcmalloc-r NAMES libtcmalloc_minimal  HINTS ${DEPS_DIR}/gperftools-2.1/Release)
	#	find_library(libtcmalloc-d NAMES libtcmalloc_minimal-debug  HINTS ${DEPS_DIR}/gperftools-2.1/Debug)
	#	set(libtcmalloc optimized ${libtcmalloc-r} debug ${libtcmalloc-d})
	#endif()
	
	#Google Testing Framework include and library dependencies
	#include_directories(${DEPS_DIR}/gtest-1.7.0/include)
	#find_library(gtest NAMES gtest  HINTS ${DEPS_DIR}/gtest-1.7.0/msvc/gtest-md/Release)
	#enable_testing()
	#if (UNIX)
	#	find_package(GTest REQUIRED)
	#	include_directories(${GTEST_INCLUDE_DIR})
	#	set(gtest ${GTEST_LIBRARIES})
	#endif()

	#ODB include dependencies
if (MSVC14 OR MSVC10 OR MINGW)
	#ODB include dependencies
	include_directories(${DEPS_DIR}/libodb-2.4.0)
	include_directories(${DEPS_DIR}/libodb-2.4.0/libodb-sqlite-2.4.0)
	include_directories(${DEPS_DIR}/libodb-2.4.0/sqlite-amalgamation-3110100)

	#ODB library dependencies
	set(odb ${DEPS_DIR}/libodb-2.4.0/lib64/odb.lib)
	set(odb-d ${DEPS_DIR}/libodb-2.4.0/lib64/odb-d.lib)
	set(odb_sqlite ${DEPS_DIR}/libodb-2.4.0/libodb-sqlite-2.4.0/lib64/odb-sqlite.lib)
	set(odb_sqlite-d ${DEPS_DIR}/libodb-2.4.0/libodb-sqlite-2.4.0/lib64/odb-sqlite-d.lib)
	#set(sqlite3 ${DEPS_DIR}/libodb-2.4.0/libodb-sqlite-2.4.0/etc/sqlite/lib64/sqlite3.lib)
	#set(sqlite3-d ${DEPS_DIR}/libodb-2.4.0/libodb-sqlite-2.4.0/etc/sqlite/lib64/sqlite3-d.lib)
	# set(iolibs Io ${odb} ${sqlite3} ${odb_sqlite})
	set(iolibs Io optimized ${odb}  optimized ${sqlite3} optimized ${odb_sqlite} debug ${odb-d} debug ${sqlite3-d} debug ${odb_sqlite-d})
elseif(UNIX)
	#ODB include dependencies
	include_directories(${DEPS_DIR}/libodb-2.4.0/release/include)
	include_directories(${DEPS_DIR}/libodb-2.4.0/release/odb-sqlite/include)
	include_directories(${DEPS_DIR}/libodb-2.4.0/release/sqlite3/include)

	#ODB library dependencies
	set(odb ${DEPS_DIR}/libodb-2.4.0/release/lib/libodb.so)
	set(odb-d ${DEPS_DIR}/libodb-2.4.0/debug/lib/libodb.so)
	set(odb_sqlite ${DEPS_DIR}/libodb-2.4.0/release/odb-sqlite/lib/libodb-sqlite.so)
	set(odb_sqlite-d ${DEPS_DIR}/libodb-2.4.0/debug/odb-sqlite/lib/libodb-sqlite.so)
	set(sqlite3 ${DEPS_DIR}/libodb-2.4.0/release/sqlite3/lib/libsqlite3.so)
	set(sqlite3-d ${DEPS_DIR}/libodb-2.4.0/debug/sqlite3/lib/libsqlite3.so)
	# set(iolibs Io ${odb} ${sqlite3} ${odb_sqlite})
	set(iolibs Io optimized ${odb}  optimized ${sqlite3} optimized ${odb_sqlite} debug ${odb-d} debug ${sqlite3-d} debug ${odb_sqlite-d} )
else()
	include_directories(${DEPS_DIR}/libodb-2.2.2)
	include_directories(${DEPS_DIR}/libodb-2.2.2/libodb-sqlite-2.2.0)
	include_directories(${DEPS_DIR}/libodb-2.2.2/libodb-sqlite-2.2.0/etc/sqlite)

	#ODB library dependencies
	set(odb ${DEPS_DIR}/libodb-2.2.2/lib64/odb.lib)
	set(odb-d ${DEPS_DIR}/libodb-2.2.2/lib64/odb-d.lib)
	set(odb_sqlite ${DEPS_DIR}/libodb-2.2.2/libodb-sqlite-2.2.0/lib64/odb-sqlite.lib)
	set(odb_sqlite-d ${DEPS_DIR}/libodb-2.2.2/libodb-sqlite-2.2.0/lib64/odb-sqlite-d.lib)
	set(sqlite3 ${DEPS_DIR}/libodb-2.2.2/libodb-sqlite-2.2.0/etc/sqlite/lib64/sqlite3.lib)
	set(sqlite3-d ${DEPS_DIR}/libodb-2.2.2/libodb-sqlite-2.2.0/etc/sqlite/lib64/sqlite3-d.lib)
	# set(iolibs Io ${odb} ${sqlite3} ${odb_sqlite})
	set(iolibs Io optimized ${odb}  optimized ${sqlite3} optimized ${odb_sqlite} debug ${odb-d} debug ${sqlite3-d} debug ${odb_sqlite-d})
endif()
#}




#Allow Access to Project-level Directories
include_directories(libs)
include_directories(libs/io)
include_directories(libs/core)
include_directories(libs/repository)
include_directories(libs/flow_lib)


# %%%RLW include_directories(libs/scenario_manager)
include_directories(libs/population_synthesis)
include_directories(libs/activity_simulator)
include_directories(libs/traffic_simulator)

include_directories(apps)
include_directories(tests)

include_directories(apps/integrated_capacity_based_model)

#Apply CMake over POLARIS library project structure
add_subdirectory (libs/core) 
add_subdirectory (libs/repository) 
add_subdirectory (libs/io) 

add_subdirectory (libs/scenario_manager)
add_subdirectory (libs/population_synthesis)
add_subdirectory (libs/activity_simulator)

add_subdirectory (apps/integrated_capacity_based_model)



if (MSVC AND Antares)
	include_directories(libs/visual_traffic_simulator)

	include_directories(${DEPS_DIR}/glew-1.7.0/include)
	include_directories(${DEPS_DIR}/plplot/include/plplot)
	include_directories(${DEPS_DIR}/wxWidgets-2.9.3/include)
	include_directories(${DEPS_DIR}/wxWidgets-2.9.3/include/msvc)
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${DEPS_DIR}/glew-1.7.0/lib ${DEPS_DIR}/plplot/bin)
    link_directories( ${LINK_DIRECTORIES} ${DEPS_DIR}/wxWidgets-2.9.3/lib/vc_dll )

    find_library(glew glew32 ${DEPS_DIR}/glew-1.7.0/bin)
	find_library(plplotd plplotd ${DEPS_DIR}/plplot/lib)
	find_library(plplotcxxd plplotcxxd ${DEPS_DIR}/plplot/lib)
	find_library(csirocsa csirocsa ${DEPS_DIR}/plplot/lib)
	find_library(qsastime qsastime ${DEPS_DIR}/plplot/lib)
	find_library(plplotwxwidgetsd plplotwxwidgetsd ${DEPS_DIR}/plplot/lib)
	
	ADD_DEFINITIONS( -DANTARES)
	ADD_DEFINITIONS( -D_UNICODE )
	ADD_DEFINITIONS( -DNDEBUG )
	#ADD_DEFINITIONS( -DGLEW_STATIC )
	ADD_DEFINITIONS( -DGLEW_BUILD )
	ADD_DEFINITIONS( -DWIN32 )
	ADD_DEFINITIONS( -DWINVER=0x0600 )
	ADD_DEFINITIONS( -D__WXMSW__ )
	ADD_DEFINITIONS( -D_WINDOWS )
	ADD_DEFINITIONS( -DwxUSE_GUI=1 )
	ADD_DEFINITIONS( -D_CRT_SECURE_NO_DEPRECATE )
	ADD_DEFINITIONS( -D_CRT_NONSTDC_NO_DEPRECATE )
	ADD_DEFINITIONS( -DWXUSINGDLL  )
    set(antareslibs Antares ${glew} opengl32.lib glu32.lib ${plplotd} ${plplotcxxd} ${csirocsa} ${qsastime} ${plplotwxwidgetsd})    
    add_subdirectory (libs/antares)
	add_subdirectory (apps/demo)
	add_subdirectory (libs/visual_traffic_simulator)
	#add_subdirectory (apps/test)
endif()



#Apply CMake over POLARIS application project structure
if (Popsyn)
	set(Antares OFF)
	add_subdirectory (apps/population_synthesizer)
	add_subdirectory (libs/traffic_simulator)
elseif(Statewide)
	add_subdirectory (apps/ILSTDM) 
	add_subdirectory (libs/statewide_model)
	add_subdirectory (libs/spatial) 
	add_subdirectory (libs/routing) 
	add_subdirectory (libs/traffic_simulator)
else()
	#add_subdirectory (apps/assignment_analyzer) 
	#add_subdirectory (apps/batch_router) 
	#add_subdirectory (apps/io_demo) 
	#add_subdirectory (apps/test) 
	add_subdirectory (apps/fixed_demand_simulator) 
	add_subdirectory (apps/integrated_abm)
	add_subdirectory (apps/randy_test)
	add_subdirectory (apps/atomic_test)
	#add_subdirectory (apps/dee-test)
	#add_subdirectory (apps/network_validator)
#	add_subdirectory (apps/Output_Visualizer)
	add_subdirectory (libs/traffic_simulator)
	add_subdirectory (libs/spatial) 
	add_subdirectory (libs/routing) 
endif()


if (Coretests)
	#Apply CMake over POLARIS tests project structure
	add_subdirectory (tests/core_tests)
endif()

if (CapacityFlow)
	message(${CapacityFlow})
	add_subdirectory(libs/flow_lib)
	add_subdirectory(apps/capacity_based_flow)
endif()

if (Unittest)
	# Enable ExternalProject CMake module
	include(ExternalProject)
	set(googletest_dir "${CMAKE_SOURCE_DIR}/external/googletest")
	# Download and install GoogleTest
	ExternalProject_Add(
		gtest
		# URL https://github.com/google/googletest/archive/master.zip
		SOURCE_DIR ${googletest_dir}
		# # PREFIX ${gtest_dir}
		# SOURCE_DIR "${gtest_dir}/src"
		# CONFIGURE_COMMAND
			# "${CMAKE_COMMAND}"
			# "-H${gtest_dir}/src/googletest"
			# "-B${gtest_dir}/Builds"
			# "-DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_DIR}"		
	   # BUILD_COMMAND
			# "${CMAKE_COMMAND}" --build "${gtest_dir}/Builds"
		# # INSTALL_COMMAND
			# # "${CMAKE_COMMAND}" --build "${gtest_dir}/Builds" --target install
	)
	add_library(libgtest IMPORTED STATIC GLOBAL)
	add_dependencies(libgtest gtest)
	# Set gtest properties
	ExternalProject_Get_Property(gtest source_dir binary_dir)
	set_target_properties(libgtest PROPERTIES
		"IMPORTED_LOCATION" "${binary_dir}/libgtest.a"
		"IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
	#    "INTERFACE_INCLUDE_DIRECTORIES" "${source_dir}/include"
	)
	# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
	include_directories("${source_dir}/include")	
endif()
if(Antares)
	PROJECT(polaris_antares CXX)
else()
	PROJECT(polaris CXX)
endif()

cmake_minimum_required(VERSION 2.8)

#Add capability to create visual studio solution folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#options to be used while running cmake
option(Antares "Build with Antares" OFF)
option(BatchRouter "Build BathRouter application" ON)
option(Popsyn "Build Popsyn App" OFF)
option(Statewide "Build Statewide Model App" OFF)
option(Coretests "Include Unit Tests for Core" OFF)
option(Unittests "Include Unit Tests for Other Libraries" OFF)
option(CapacityFlow "Include Traffic Flow Simulator Prototype" OFF)
option(IntegratedModel "Build Integrated Activity Based Model" ON)
option(HelloWorld "Build Hello World application" ON)
option(FixedDemandSimulator "Build Fixed Demand Simulator application" OFF)
option(CommandLineArguments "Incorporate command line arguments" OFF)
option(RandyTest "Build RandyTest application" OFF)



set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER cmake)

#Variables designating where the output files go
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BUILD_DIR}/bin)


#Compiler flags for release mode applied to all projects
if (CommandLineArguments)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOMMANDLINE_ARGS")
endif()

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Ob0 /MD /W2 /Oi")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /O2 /Ob0 /MD /W2 /Oi /Zi")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
elseif(MINGW)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj -std=c++11")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wa,-mbig-obj -std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wa,-mbig-obj -std=c++11")
else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations -pthread -D__forceinline=inline -m64 -std=c++11 -lrt")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
endif()

#Set up dependencies

#{
	#Variable to indicate where most external dependencies live
	#set(DEPS_DIR C:/opt/polarisdeps) %%%RLW
	if(NOT DEPS_DIR)
		if(DEFINED ENV{POLARIS_DEPS_DIR})
			set(DEPS_DIR $ENV{POLARIS_DEPS_DIR})
		else()
			set(DEPS_DIR C:/opt/polarisdeps)
		endif()
	endif()
	
	MESSAGE(INFO " DEPS_DIR=${DEPS_DIR}")
	if(MSVC)
		set(DEPS_BINARY_DIR ${DEPS_DIR}/bin)
		MESSAGE(INFO " DEPS_BINARY_DIR=${DEPS_BINARY_DIR}")
	endif()

	include_directories(${DEPS_DIR}/rapidjson-1.1.0/include)

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
        if (CommandLineArguments)
		set (Boost_USE_MULTITHREADED ON)
		find_package(Boost 1.60.0 REQUIRED COMPONENTS
		  chrono
		  timer
		  filesystem
		  system
		  program_options
		 )
		include_directories(${Boost_INCLUDE_DIRS})
		link_libraries(${Boost_LIBRARIES})
	else()
		set (Boost_USE_MULTITHREADED ON)
		find_package(Boost 1.60.0 REQUIRED)
		include_directories(${Boost_INCLUDE_DIRS})
	endif()

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
include_directories(libs/traveler_simulator)
include_directories(libs/its_simulator)
include_directories(libs/scenario_manager)

include_directories(apps)
include_directories(tests)

include_directories(apps/integrated_capacity_based_model)

#Apply CMake over POLARIS library project structure
add_subdirectory (libs/activity_simulator)
add_subdirectory (libs/core) 
add_subdirectory (libs/io) 
add_subdirectory (libs/its_simulator)
add_subdirectory (libs/population_synthesis)
add_subdirectory (libs/repository) 
add_subdirectory (libs/routing) 
add_subdirectory (libs/scenario_manager)
add_subdirectory (libs/spatial)
add_subdirectory (libs/traffic_simulator)
add_subdirectory (libs/traveler_simulator)



if (MSVC AND Antares)
	if(MSVC)
		set(ANTARES_DEPS_BINARY_DIR ${DEPS_DIR}/antares_bin)
		MESSAGE(INFO " ANTARES_DEPS_BINARY_DIR=${ANTARES_DEPS_BINARY_DIR}")
	endif()

	include_directories(libs/visual_traffic_simulator)

	include_directories(${DEPS_DIR}/glew-1.13.0/include)
	include_directories(${DEPS_DIR}/plplot-5.11.1/include)
	include_directories(${DEPS_DIR}/plplot-5.11.1/bindings/c++)
	include_directories(${DEPS_DIR}/plplot-5.11.1/bindings/wxwidgets)
	include_directories(${DEPS_DIR}/plplot-5.11.1/build_vs2015/release/include)
	include_directories(${DEPS_DIR}/wxWidgets-3.1.0/include/msvc)
	include_directories(${DEPS_DIR}/wxWidgets-3.1.0/include)
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${DEPS_DIR}/glew-1.13.0/build_vs2015/lib/Release ${DEPS_DIR}/plplot-5.11.1/build_vs2015/release/bindings/c++/Release)
    link_directories( ${LINK_DIRECTORIES} ${DEPS_DIR}/wxWidgets-3.1.0/lib/vc_x64_dll )

    find_library(glew glew32 ${DEPS_DIR}/glew-1.13.0/build_vs2015/lib/Release)
	find_library(plplot plplot ${DEPS_DIR}/plplot-5.11.1/build_vs2015/release/src/Release)
	find_library(plplotcxx plplotcxx ${DEPS_DIR}/plplot-5.11.1/build_vs2015/release/bindings/c++/Release)
	find_library(csirocsa csirocsa ${DEPS_DIR}/plplot-5.11.1/build_vs2015/release/lib/csa/Release)
	find_library(qsastime qsastime ${DEPS_DIR}/plplot-5.11.1/build_vs2015/release/lib/qsastime/Release)
	find_library(plplotwxwidgets plplotwxwidgets ${DEPS_DIR}/plplot-5.11.1/build_vs2015/release/bindings/wxwidgets/Release)
	find_library(shapelib shapelib ${DEPS_DIR}/shapelib-1.3.0/lib)
	find_library(freetype freetype ${DEPS_DIR}/freetype-2.6.3/build_vs2015/Release)

    find_library(glewd glew32d ${DEPS_DIR}/glew-1.13.0/build_vs2015/lib/Debug)
	find_library(plplotd plplot ${DEPS_DIR}/plplot-5.11.1/build_vs2015/debug/src/Debug)
	set(plplotcxxd ${DEPS_DIR}/plplot-5.11.1/build_vs2015/debug/bindings/c++/Debug/plplotcxx.lib)
	find_library(csirocsad csirocsa ${DEPS_DIR}/plplot-5.11.1/build_vs2015/debug/lib/csa/Debug)
	find_library(qsastimed qsastime ${DEPS_DIR}/plplot-5.11.1/build_vs2015/debug/lib/qsastime/Debug)
	find_library(plplotwxwidgetsd plplotwxwidgets ${DEPS_DIR}/plplot-5.11.1/build_vs2015/debug/bindings/wxwidgets/Debug)
	find_library(shapelibd shapelib_d ${DEPS_DIR}/shapelib-1.3.0/lib)
	find_library(freetyped freetyped ${DEPS_DIR}/freetype-2.6.3/build_vs2015/Debug)
	
	ADD_DEFINITIONS( -DANTARES )
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
    set(antareslibs Antares optimized ${glew} opengl32.lib glu32.lib optimized ${plplot} optimized ${plplotcxx} optimized ${csirocsa} optimized ${qsastime} optimized ${plplotwxwidgets} optimized ${shapelib} optimized ${freetype} debug ${glewd} debug ${plplotd} debug ${plplotcxxd} debug ${csirocsad} debug ${qsastimed} debug ${plplotwxwidgetsd} debug ${shapelibd} debug ${freetyped})
    add_subdirectory (libs/antares)
	add_subdirectory (libs/visual_traffic_simulator)

	add_subdirectory (apps/demo)
	#add_subdirectory (apps/fixed_demand_simulator) 
	#add_subdirectory (apps/integrated_abm)
	#add_subdirectory (apps/test)
else()
	if (Posysn OR FixedDemandSimulator)
		add_subdirectory (libs/traffic_simulator)
	endif()
	#Apply CMake over POLARIS application project structure
	if (Popsyn)
		set(Antares OFF)
		add_subdirectory (apps/population_synthesizer)
	elseif(Statewide)
		add_subdirectory (apps/ILSTDM) 
		add_subdirectory (libs/statewide_model)
		#add_subdirectory (libs/spatial) 
		#add_subdirectory (libs/routing) 
		#add_subdirectory (libs/traffic_simulator)
    else()
                #add_subdirectory (apps/assignment_analyzer)
		#add_subdirectory (apps/batch_router) 
		#add_subdirectory (apps/io_demo) 
	#	add_subdirectory (apps/test) 
	#	add_subdirectory (apps/integrated_abm)
	#	add_subdirectory (apps/randy_test)
	#	add_subdirectory (apps/integrated_capacity_based_model)
	#	add_subdirectory (apps/atomic_test)
		#add_subdirectory (apps/dee-test)
		#add_subdirectory (apps/network_validator)
	#	add_subdirectory (apps/Output_Visualizer)
		#add_subdirectory (apps/population_synthesizer)
		#add_subdirectory (libs/traffic_simulator)
		#add_subdirectory (libs/spatial) 
		#add_subdirectory (libs/routing) 
	endif()

	if (BatchRouter)
		add_subdirectory (apps/batch_router)
	endif()
	
	if (IntegratedModel)
		add_subdirectory (apps/integrated_abm)
		add_subdirectory (apps/network_validator)
		add_subdirectory (apps/assignment_analyzer)
	endif()

	if(FixedDemandSimulator)
		add_subdirectory (apps/fixed_demand_simulator)
	endif()

	if(HelloWorld)
		add_subdirectory (apps/hello_world)
	endif()

	if(RandyTest)
		add_subdirectory(apps/randy_test)
	endif()

	if (Coretests)
		#Apply CMake over POLARIS tests project structure
		add_subdirectory (tests/core_tests)
	endif()

	if (Routertests)
		#Apply CMake over POLARIS tests project structure
		add_subdirectory (tests/router)
	endif()

	if (CapacityFlow)
		message(${CapacityFlow})
		add_subdirectory(libs/flow_lib)
		add_subdirectory(apps/capacity_based_flow)
	endif()
endif()

if (Unittests)
	#Google Testing Framework include and library dependencies
	if (MSVC14)
		include_directories(${DEPS_DIR}/googletest-release-1.7.0/include)
		set(gtest ${DEPS_DIR}/googletest-release-1.7.0/build_msvc2015/Release/gtest.lib)
		set(gtest-d ${DEPS_DIR}/googletest-release-1.7.0/build_msvc2015/Debug/gtest.lib)
		set(GTEST_LIBRARY optimized ${gtest} debug ${gtest-d})
		enable_testing()
	elseif(UNIX)
		include_directories(${DEPS_DIR}/googletest-release-1.7.0/include)
		set(gtest ${DEPS_DIR}/googletest-release-1.7.0/build/release/libgtest.a)
		set(gtest-d ${DEPS_DIR}/googletest-release-1.7.0/build/debug/libgtest.a)
		set(GTEST_LIBRARY optimized ${gtest} debug ${gtest-d})
		enable_testing()
	endif()

	#Apply CMake over POLARIS tests project structure
	add_subdirectory(tests)
endif() #Unittests

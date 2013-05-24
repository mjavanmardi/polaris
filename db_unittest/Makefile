# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths... 
Debug_Include_Path=-I../ -I$(HOME)/odb/include/
Release_Include_Path=-I../ -I$(HOME)/odb/include/

export GTEST_DIR=/mnt/lustre/home/vsokolov/gtest-1.6.0
g++ -std=c++0x -I${GTEST_DIR}/include -I${HOME}/odb/include/ -I.. -L../gccRelease -L/usr/lib -L${HOME}/odb/lib -L${GTEST_DIR} -Wl,--start-group -lgtest -lsqlite3 -lodb -lodb-sqlite -Wl,--end-group main.cpp ../gccRelease/libio.a /mnt/lustre/home/vsokolov/gtest-1.6.0/libgtest.a -o db_unittest


# Include paths... 
Debug_Include_Path=-I../ -I$(HOME)/odb/include/
Release_Include_Path=-I../ -I$(HOME)/odb/include/

# Library paths...
Debug_Library_Path=-L$(HOME)/odb/lib -L../gccDebug -L/usr/lib
Release_Library_Path=-L$(HOME)/odb/lib -L../gccRelease -L/usr/lib

# Additional libraries...
Debug_Libraries=
Release_Libraries=-Wl,--start-group -lgtest -lsqlite3 -lodb -lodb-sqlite -lio -Wl,--end-group

# Preprocessor definitions...
Debug_Preprocessor_Definitions=-D GCC_BUILD -D _DEBUG -D _CONSOLE -D LIBODB_STATIC_LIB -D LIBODB_SQLITE_STATIC_LIB
Release_Preprocessor_Definitions=-D GCC_BUILD -D NDEBUG -D _CONSOLE  -D LIBODB_STATIC_LIB -D LIBODB_SQLITE_STATIC_LIB

# Compiler flags...
Debug_Compiler_Flags=-O0 
Release_Compiler_Flags=-O2 

# Builds all configurations for this project...
.PHONY: build_all_configurations
build_all_configurations: Debug Release 

# Builds the Debug configuration...
.PHONY: Debug
Debug: create_folders gccDebug/main.o 
	g++ gccDebug/main.o  $(Debug_Library_Path) $(Debug_Libraries) -Wl,-rpath,./ -o ../gccDebug/db_unittest.exe

# Compiles file main.cpp for the Debug configuration...
-include gccDebug/main.d
gccDebug/main.o: main.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c main.cpp $(Release_Include_Path) -o gccDebug/main.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM main.cpp $(Release_Include_Path) > gccDebug/main.d

# Builds the Release configuration...
.PHONY: Release
Release: create_folders gccRelease/main.o 
	g++ gccRelease/main.o  $(Release_Library_Path) $(Release_Libraries) -Wl,-rpath,./ -o ../gccRelease/db_unittest.exe

# Compiles file main.cpp for the Release configuration...
-include gccRelease/main.d
gccRelease/main.o: main.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c main.cpp $(Release_Include_Path) -o gccRelease/main.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM main.cpp $(Release_Include_Path) > gccRelease/main.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug
	mkdir -p ../gccDebug
	mkdir -p gccRelease
	mkdir -p ../gccRelease

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug/*.o
	rm -f gccDebug/*.d
	rm -f gccRelease/*.o
	rm -f gccRelease/*.d

# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_Include_Path=-I ~/odb/include/
Release_Include_Path=-I ~/odb/include/

# Library paths...
Debug_Library_Path=~/odb/lib
Release_Library_Path=~/odb/lib

# Additional libraries...
Debug_Libraries=
Release_Libraries=

# Preprocessor definitions...
Debug_Preprocessor_Definitions=-D GCC_BUILD -D _DEBUG -D _CONSOLE 
Release_Preprocessor_Definitions=-D GCC_BUILD -D NDEBUG -D _CONSOLE 

# Implictly linked object files...
Debug_Implicitly_Linked_Objects=
Release_Implicitly_Linked_Objects=

# Compiler flags...
Debug_Compiler_Flags=-O0 
Release_Compiler_Flags=-O2 

# Builds all configurations for this project...
.PHONY: build_all_configurations
build_all_configurations: Debug Release 

# Builds the Debug configuration...
.PHONY: Debug
Debug: create_folders gccDebug/Supply-odb.o gccDebug/Result-odb.o gccDebug/Demand-odb.o gccDebug/System-odb.o 
	ar rcs ../gccDebug/libpolaris_io.a gccDebug/Supply-odb.o gccDebug/Result-odb.o gccDebug/Demand-odb.o gccDebug/System-odb.o  $(Debug_Implicitly_Linked_Objects)

# Compiles file Supply-odb.cxx for the Debug configuration...
-include gccDebug/Supply-odb.d
gccDebug/Supply-odb.o: Supply-odb.cxx
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c Supply-odb.cxx $(Debug_Include_Path) -o gccDebug/Supply-odb.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM Supply-odb.cxx $(Debug_Include_Path) > gccDebug/Supply-odb.d

# Compiles file Result-odb.cxx for the Debug configuration...
-include gccDebug/Result-odb.d
gccDebug/Result-odb.o: Result-odb.cxx
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c Result-odb.cxx $(Debug_Include_Path) -o gccDebug/Result-odb.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM Result-odb.cxx $(Debug_Include_Path) > gccDebug/Result-odb.d

# Compiles file Demand-odb.cxx for the Debug configuration...
-include gccDebug/Demand-odb.d
gccDebug/Demand-odb.o: Demand-odb.cxx
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c Demand-odb.cxx $(Debug_Include_Path) -o gccDebug/Demand-odb.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM Demand-odb.cxx $(Debug_Include_Path) > gccDebug/Demand-odb.d

# Compiles file System-odb.cxx for the Debug configuration...
-include gccDebug/System-odb.d
gccDebug/System-odb.o: System-odb.cxx
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c System-odb.cxx $(Debug_Include_Path) -o gccDebug/System-odb.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM System-odb.cxx $(Debug_Include_Path) > gccDebug/System-odb.d

# Builds the Release configuration...
.PHONY: Release
Release: create_folders gccRelease/Supply-odb.o gccRelease/Result-odb.o gccRelease/Demand-odb.o gccRelease/System-odb.o 
	ar rcs ../gccRelease/libpolaris_io.a gccRelease/Supply-odb.o gccRelease/Result-odb.o gccRelease/Demand-odb.o gccRelease/System-odb.o  $(Release_Implicitly_Linked_Objects)

# Compiles file Supply-odb.cxx for the Release configuration...
-include gccRelease/Supply-odb.d
gccRelease/Supply-odb.o: Supply-odb.cxx
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c Supply-odb.cxx $(Release_Include_Path) -o gccRelease/Supply-odb.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM Supply-odb.cxx $(Release_Include_Path) > gccRelease/Supply-odb.d

# Compiles file Result-odb.cxx for the Release configuration...
-include gccRelease/Result-odb.d
gccRelease/Result-odb.o: Result-odb.cxx
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c Result-odb.cxx $(Release_Include_Path) -o gccRelease/Result-odb.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM Result-odb.cxx $(Release_Include_Path) > gccRelease/Result-odb.d

# Compiles file Demand-odb.cxx for the Release configuration...
-include gccRelease/Demand-odb.d
gccRelease/Demand-odb.o: Demand-odb.cxx
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c Demand-odb.cxx $(Release_Include_Path) -o gccRelease/Demand-odb.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM Demand-odb.cxx $(Release_Include_Path) > gccRelease/Demand-odb.d

# Compiles file System-odb.cxx for the Release configuration...
-include gccRelease/System-odb.d
gccRelease/System-odb.o: System-odb.cxx
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c System-odb.cxx $(Release_Include_Path) -o gccRelease/System-odb.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM System-odb.cxx $(Release_Include_Path) > gccRelease/System-odb.d

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
	rm -f ../gccDebug/*.a
	rm -f ../gccDebug/*.so
	rm -f ../gccDebug/*.dll
	rm -f ../gccDebug/*.exe
	rm -f gccRelease/*.o
	rm -f gccRelease/*.d
	rm -f ../gccRelease/*.a
	rm -f ../gccRelease/*.so
	rm -f ../gccRelease/*.dll
	rm -f ../gccRelease/*.exe


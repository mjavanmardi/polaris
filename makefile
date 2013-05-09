all: 
	cd Io; make odb_compile
	cd Io; make Release
	cd File_IO; make
	cd Scenario_Manager; make
	cd Repository; make
	cd Application; make
clean:
	cd Application; make clean
	cd Repository; make clean
	cd Scenario_Manager; make clean
	cd File_IO; make clean

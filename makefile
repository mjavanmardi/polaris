all: 
	cd Scenario_Manager; make
	cd Application; make
clean:
	cd Application; make clean
	cd Scenario_Manager; make clean

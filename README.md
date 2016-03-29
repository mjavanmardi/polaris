Overview
=========
POLARIS provides libraries and tools to help transportation 
engineers to create transportation system simulations. 
In particular to simulate a Traffic Management 
Center along with ITS infrastructure. 
It provides implementation of vented models 
in transportation community so that those 
can be reused by application developers.

The project is distributed under BSD liense.

POLARIS Dependencies
====================
The dependecies can be downloaded and built using scripts. You MUST set the POLARIS_DEPS_DIR environment variable before you download and build the dependencies.

Linux Dependencies
------------------
Here is the list of dependencies:
* Boost 1.60
* odb-2.4.0-x86_64-linux-gnu
* libodb-2.4.0
* libodb-sqlite-2.4.0
* sqlite3 (3.11.1)

The POLARIS_DEPS_DIR environment variable must be set to build dependecies and application binaries. You can do this by editing your .bashrc file (in you home directory).
For exanmple:
	<other bash stuff here>
	# User specific aliases and functions
	module load compiler/gcc510

	POLARIS_DEPS_DIR=~/polaris/deps
	export POLARIS_DEPS_DIR
	
Once the environment variable is set correctly call the shell script:
	cd pT1/polarisdeps
	./get_deps.sh
	
This will download the required compressed files, extract them and then build the dependencies. This may take a while so be patient.

Windows Dependencies
--------------------
Here is the list of dependencies:
* Boost 1.60
* odb-2.4.0-x86_64-windows
* libodb-2.4.0
* libodb-sqlite-2.4.0
* sqlite3 (3.11.1)

There is a convenience polaris_env.bat script that appends all of the paths with necessary dlls to the PATH varaible so those can be found when you run an application. Please note that the batch file MUST be copied to 
the directory where you wish to have you dependecies. The environment is only good for that command shell.

Example: (NOTE: this should be performed in a Visual Studio Native Tools Command Shell)
	copy pT1/polarisdeps/polaris_env.bat c:\opt\polarisdeps
	cd c:\opt\polarisdeps
	polaris_env.bat
	
Then run the sript to download, extract and build the dependencies:
	cd <??>/pT1/polarisdeps
	get_deps.cmd
	
This may take a while so be patient.

Building Using Cmake
====================

Linux Build
-----------
Download polaris in an appropriate directory (we'll call it pT1).

	git clone "https://github.com/anl-polaris/polaris.git" pT1
	cd pT1
	git checkout T-1
	./configure_polaris.sh
	cd build/debug
	make Fixed_Demand_Simulator

Execution binary is: pT1/build/debug/bin/Fixed_Demand_Simulator

	cd ../release
	make Fixed_Demand_Simulator
	
Execution binary is: pT1/build/release/bin/Fixed_Demand_Simulator


Windows Build
-------------
Download polaris in an appropriate directory.

	git clone "https://github.com/anl-polaris/polaris.git" pT1
	cd pT1
	git checkout T-1
	mkdir build_msvc2015
	cd build_msvc2015
	cmake -G "Visual Studio 14 Win64" ..

Open Visual Studio:
	polaris.sln
Or use msbuild: (NOTE: this should be performed in a Visual Studio Native Tools Command Shell)
	msbuild polaris.sln /p:Configuration=Debug /p:/Platform=x64 /t:Fixed_Demand_Simulator





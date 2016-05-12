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

The POLARIS_DEPS_DIR environment variable must be set to build dependecies and application binaries. You can do this by editing your .bashrc file (in your home directory).

For example:

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
-- NOTE:
	get-deps.cmd now locates the proper Visual Studio installation and set it up - so no longer REQUIRES the tools command shell.

--NOTE:
	The following step is now optional. You can specify the dependency directory as part of the get-deps.cmd call.
	
	copy pT1/polarisdeps/polaris_env.bat c:\opt\polarisdeps
	cd c:\opt\polarisdeps
	polaris_env.bat
	
Then run the sript to download, extract and build the dependencies:

	cd pT1/polarisdeps
	get_deps.cmd
	--or optionally
	get_deps.cmd c:\opt\polarisdeps_vs2015
	
This may take a while so be patient.

Building Using Cmake
====================

Linux Build
-----------
Download polaris in an appropriate directory. Be sure the POLARIS_DEPS_DIR variable is set.

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
Download polaris in an appropriate directory. Be sure the POLARIS_DEPS_DIR variable is set or supply the dependency folder to the configure_polaris acript.
The argument to the configure script will ovrride the environment variable.

	git clone "https://github.com/anl-polaris/polaris.git" pT1
	cd pT1
	git checkout T-1
	configure_polaris.cmd 
	or
	configure_polaris.cmd c:\opt\polarisdeps
	cd build_vs2015

Open Visual Studio:

	polaris.sln
	
Or use msbuild: (NOTE: this should be performed in a Visual Studio Native Tools Command Shell)

	msbuild polaris.sln /p:Configuration=Debug /p:Platform=x64
	msbuild polaris.sln /p:Configuration=Release /p:Platform=x64
	
Execution binary for debug is located in pT1\build_vs2015\bin\Debug
Execution binary for release is located in pT1\build_vs2015\bin\Release





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

Tool Chains
===========

Linux
-----
* git
* cmake 3.2.2 or newer (3.5.2 recommended)
* gcc 4.8.5 or newer

Windows
-------
* git
* cmake 3.2.2 or newer (3.5.2 recommended)
* Visual Studio 2015 (Update 2 recommended)
	- Note that there is an issue when running Visual Studio on machines older than Windows 10.
	- See https://support.microsoft.com/en-us/kb/3118401

POLARIS Dependencies
====================
The dependecies can be downloaded and built using scripts.

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

	POLARIS_DEPS_DIR=~/polaris/deps
	export POLARIS_DEPS_DIR
	
Once the environment variable is set correctly clone the repository and call the shell script:
	git clone "https://github.com/anl-polaris/polaris.git" <code_dir>
	cd <code_dir>/polarisdeps
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

Example:
Run the sript to download, extract and build the dependencies:

	git clone "https://github.com/anl-polaris/polaris.git" <code_dir>
	cd <code_dir>/polarisdeps
	get-deps.cmd <dependencies_dir>
	
This may take a while so be patient.

Building Using Cmake
====================

Linux Build
-----------
Download polaris in an appropriate directory. Be sure the POLARIS_DEPS_DIR variable is set.

	git clone "https://github.com/anl-polaris/polaris.git" <code_dir>
	cd <code_dir>
	./configure_polaris.sh
	cd build/debug
	make Fixed_Demand_Simulator

Execution binary is: <code_dir>/build/debug/bin/Fixed_Demand_Simulator

	cd ../release
	make Fixed_Demand_Simulator
	
Execution binary is: <code_dir>/build/release/bin/Fixed_Demand_Simulator


Windows Build
-------------
Download polaris in an appropriate directory. Be sure the POLARIS_DEPS_DIR variable is set or supply the dependency folder to the configure_polaris acript.
The argument to the configure script will ovrride the environment variable.

	git clone "https://github.com/anl-polaris/polaris.git" <code_dir>
	cd <code_dir>
	configure-polaris.cmd <dependencies_dir>
	cd build_vs2015

Open Visual Studio:

	polaris.sln
	
Or use msbuild: (NOTE: this should be performed in a Visual Studio Native Tools Command Shell)

	msbuild polaris.sln /p:Configuration=Debug /p:Platform=x64
	msbuild polaris.sln /p:Configuration=Release /p:Platform=x64
	-- or
	build-polaris.cmd
	
Or combine the configuration and build steps into one:
	configure-and-build-polaris.cmd <dependencies_dir>
	
Execution binary for debug is located in <code_dir>\build_vs2015\bin\Debug
Execution binary for release is located in <code_dir>\build_vs2015\bin\Release





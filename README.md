Overview
=========
POLARIS provides libraries and tools to help transportation engineers create 
transportation system simulations, and in particular to simulate a Traffic 
Management Center along with ITS infrastructure. It provides implementation 
of vented models in transportation community so that those can be reused by 
application developers.

The project is distributed under BSD license.

POLARIS can be run in a Windows or a Linux(under development) environment. 
Instructions are given below.

Windows
=======

Install This
------------
Tools needed to clone the code from GitHub, to build a project using cmake, 
and develop code in Visual Studio.

* git
* cmake 3.2.2 or newer (3.5.2 recommended - NOTE there is a bug in 3.8.0 concerning slashes in paths when locating Boost)
* Visual Studio 2015 (Update 2 recommended)
	- Note that there is an issue when running Visual Studio on machines older than Windows 10.
	- See https://support.microsoft.com/en-us/kb/3118401

Get the Dependencies
--------------------
Libraries and other dependencies can be downloaded using the provided command
script.

Here is the list of dependencies:
* Boost 1.60
* odb-2.4.0-x86_64-windows
* libodb-2.4.0
* libodb-sqlite-2.4.0
* sqlite3 (3.11.1)
* rapidjson-1.1.0

Run the command sript to download, extract and build the dependencies:

	git clone "https://github.com/anl-polaris/polaris.git" <code_dir>
	cd <code_dir>/polarisdeps
	get-deps.cmd <dependencies_dir>
	
This may take a while so be patient.

Build Code Using Cmake
----------------------
Download polaris in an appropriate directory. Be sure the POLARIS_DEPS_DIR 
variable is set or supply the dependency folder to the configure_polaris script.
The argument to the configure script will ovrride the environment variable.

	git clone "https://github.com/anl-polaris/polaris.git" <code_dir>
	cd <code_dir>
	configure-polaris.cmd <dependencies_dir>
	cd build_vs2015

Open Visual Studio:

	polaris.sln
	
Or use msbuild: (NOTE: this should be performed in a Visual Studio Native Tools 
Command Shell)

	msbuild polaris.sln /p:Configuration=Debug /p:Platform=x64
	msbuild polaris.sln /p:Configuration=Release /p:Platform=x64
	-- or
	build-polaris.cmd
	
Or combine the configuration and build steps into one:
	configure-and-build-polaris.cmd <dependencies_dir>
	
Execution binary for debug is located in <code_dir>\build_vs2015\bin\Debug
Execution binary for release is located in <code_dir>\build_vs2015\bin\Release


Linux (Not currently supported - Under Development)
===================================================


@echo off

REM if "%1" == "clean" goto clean
REM if /i %1 == clean       goto clean
REM goto usage

REM :clean
REM if exist build rd /s /q build
REM goto :eof

REM :usage
if not exist build mkdir build
REM if not exist build\int mkdir build\int
REM if not exist build\net mkdir build\net
REM if not exist build\int_ant mkdir build\int_ant
REM if not exist build\net_ant mkdir build\net_ant

cd build 

REM cd int
if exist CMakeCache.txt del CMakeCache.txt
echo *************************************************
echo ***********Building Integrated Model*************
echo *************************************************
cmake -G"Visual Studio 10 Win64" -DIntegratedModelApplication=ON -DMT=ON ..
REM cmake -G"Visual Studio 10 Win64" -DIntegratedModelApplication=ON ..
echo Running msbuild...
msbuild /m Application\application.vcxproj /property:Configuration=MinSizeRel > build-int.log
IF ERRORLEVEL 1 (
	ECHO error level is 1 or more
	ECHO see build-int.log for details
	REM EXIT /B ERRORLEVEL
)
IF ERRORLEVEL 0 copy Application\MinSizeRel\Application.exe .\Application-int.exe
REM cd ..

REM cd net
if exist CMakeCache.txt del CMakeCache.txt
echo *************************************************
echo ***********Building Network Model****************
echo *************************************************
cmake -G"Visual Studio 10 Win64" -DNetworkModelApplication=ON -DMT=ON ..
REM cmake -G"Visual Studio 10 Win64" -DNetworkModelApplication=ON ..
echo Running msbuild...
msbuild /m Application\application.vcxproj /property:Configuration=MinSizeRel > build-net.log
IF ERRORLEVEL 1 (
	ECHO error level is 1 or more
	ECHO see build-net.log for details
	REM EXIT /B ERRORLEVEL
)
IF ERRORLEVEL 0 copy Application\MinSizeRel\Application.exe .\Application-net.exe
REM cd ..

REM cd int_ant
if exist CMakeCache.txt del CMakeCache.txt
echo *************************************************
echo ********Building Integrated-Antares Model********
echo *************************************************
cmake -G"Visual Studio 10 Win64" -DIntegratedModelApplication=ON -DAntares=ON -DMT=ON ..
echo Running msbuild...
msbuild /m Application\application.vcxproj /property:Configuration=MinSizeRel > build-int-ant.log
IF ERRORLEVEL 1 (
	ECHO error level is 1 or more
	ECHO see build-int-ant.log for details
	REM EXIT /B ERRORLEVEL
)
IF ERRORLEVEL 0 copy Application\MinSizeRel\Application.exe .\Application-int-ant.exe
REM cd ..

REM cd net_ant
if exist CMakeCache.txt del CMakeCache.txt
echo *************************************************
echo ********Building Network-Antares Model***********
echo *************************************************
cmake -G"Visual Studio 10 Win64" -DNetworkModelApplication=ON -DAntares=ON -DMT=ON ..
echo Running msbuild...
msbuild /m Application\application.vcxproj /property:Configuration=MinSizeRel > build-net-ant.log
IF ERRORLEVEL 1 (
	ECHO error level is 1 or more
	ECHO see build-net-ant.log for details
	REM EXIT /B ERRORLEVEL
)
IF ERRORLEVEL 0 copy Application\MinSizeRel\Application.exe .\Application-net-ant.exe
cd ..
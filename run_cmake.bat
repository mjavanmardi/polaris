@echo off
if not exist build mkdir build
if exist build\CMakeCache.txt del build\CMakeCache.txt
cd build

rem run with the default options
cmake -G"Visual Studio 10 Win64" ..
REM cmake -G"Visual Studio 10 Win64" -DItsMockup=ON ..
REM cmake -G"Visual Studio 10 Win64" -DIntegratedModelApplication=ON -DItsMockup=ON -DAntares=ON -DMT=ON ..


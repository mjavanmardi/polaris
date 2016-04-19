@echo off

set BUILDDIR=%~dp0\build_vs2015
IF NOT EXIST %BUILDDIR% (	mkdir %BUILDDIR% )

:: configure solutions/projects
cd %BUILDDIR%
::cmake -DUnittests=1 -DCMAKE_CONFIGURATION_TYPES="Debug;Release" -DCOMMANDLINE_ARGS=1 -G "Visual Studio 14 Win64" ..
::cmake -DIntegratedModel=1 -DUnittests=1 -DCMAKE_CONFIGURATION_TYPES="Debug;Release" -DCOMMANDLINE_ARGS=1 -G "Visual Studio 14 Win64" ..
::cmake -DUnittests=1 -DCMAKE_CONFIGURATION_TYPES="Debug;Release" -DANTARES=1 -G "Visual Studio 14 Win64"
cmake -DCMAKE_CONFIGURATION_TYPES="Debug;Release" -G "Visual Studio 14 Win64" ..
cd ..
@echo off

set BUILDDIR=%~dp0\build_vs2015
IF NOT EXIST %BUILDDIR% (	mkdir %BUILDDIR% )

:: configure solutions/projects
cd %BUILDDIR%
cmake -D Unittests=1 -D Antares=1 -G "Visual Studio 14 Win64" ..
cd ..
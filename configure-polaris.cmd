@echo off

SET DEPS_DIR=
IF NOT "%1" == "" (
	set DEPS_DIR=%1
) ELSE IF NOT "%POLARIS_DEPS_DIR%" == "" (
	  set DEPS_DIR=%POLARIS_DEPS_DIR%
) ELSE (
	set DEPS_DIR=%CD%
)

:: get our slashes all straightened out :)
set a=%DEPS_DIR%
set a=%a:/=\%
::echo %a%
set DEPS_DIR=%a%

:: SET DepsDirResponse=
:: SET /p DepsDirResponse= Use %DEPS_DIR% as dependancy directory?
:: echo Response=%DepsDirResponse%
:: SET USE_IT=
:: IF "%DepsDirResponse%" == "Y" SET USE_IT=1
:: IF "%DepsDirResponse%" == "y" SET USE_IT=1
:: IF DEFINED USE_IT ( ECHO Using %DEPS_DIR% as dependency directory.) ELSE ( ECHO Dependency directory is not set. & ECHO Exiting configuration. & EXIT /B 1)

set BUILDDIR=%~dp0\build_vs2015
IF NOT EXIST %BUILDDIR% ( mkdir %BUILDDIR% )

:: configure solutions/projects
cd %BUILDDIR%

:: get additional config setting - if build.cfg file exists
SET var=
IF EXIST ../build.cfg (
	SETLOCAL EnableDelayedExpansion
	FOR /f %%i in (../build.cfg) DO (
	   SET var=!var!%%i 
	)
	echo !var!
)

SET ERRORLEVEL=
::cmake -DUnittests=1 -DCMAKE_CONFIGURATION_TYPES="Debug;Release" -DCOMMANDLINE_ARGS=1 -G "Visual Studio 14 Win64" ..
::cmake -DIntegratedModel=1 -DUnittests=1 -DCMAKE_CONFIGURATION_TYPES="Debug;Release" -DCOMMANDLINE_ARGS=1 -G "Visual Studio 14 Win64" ..
::cmake -DUnittests=1 -DCMAKE_CONFIGURATION_TYPES="Debug;Release" -DANTARES=1 -G "Visual Studio 14 Win64"
cmake -DDEPS_DIR=%DEPS_DIR% !var! -DCMAKE_CONFIGURATION_TYPES="Debug;Release" -G "Visual Studio 14 Win64" ..
IF %ERRORLEVEL% NEQ 0 (ECHO Error configuring Polaris projects. & ENDLOCAL & cd .. & EXIT /B 1)
ENDLOCAL
cd ..


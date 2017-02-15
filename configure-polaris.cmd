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

:: temporary fix for RelWithDebInfo
:: at some point it would be nice to have debug info in the 3rd party libs also
IF NOT EXIST %DEPS_DIR%\bin\RelWithDebInfo ( mkdir %DEPS_DIR%\bin\RelWithDebInfo & xcopy %DEPS_DIR%\bin\Release\* %DEPS_DIR%\bin\RelWithDebInfo\* )

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
cmake -DDEPS_DIR=%DEPS_DIR% !var! -DFixedDemandSimulator=1 -DIntegratedModel=1 -DPopsyn=1 -DCMAKE_CONFIGURATION_TYPES="Debug;Release;RelWithDebInfo" -G "Visual Studio 14 Win64" ..
IF %ERRORLEVEL% NEQ 0 (ECHO Error configuring Polaris projects. & ENDLOCAL & cd .. & EXIT /B 1)

:: now generate Antares projects/soultion
::SET ERRORLEVEL=
::cmake -DDEPS_DIR=%DEPS_DIR% !var! -DAntares=1 -DCMAKE_CONFIGURATION_TYPES="Debug;Release" -G "Visual Studio 14 Win64" ..
::IF %ERRORLEVEL% NEQ 0 (ECHO Error configuring Polaris projects. & ENDLOCAL & cd .. & EXIT /B 1)

ENDLOCAL
cd ..


@ECHO OFF

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

REM  see if Vsual Studio is already set
SET VCROOT=
IF "%VSINSTALLDIR%" == "" (
	call polarisdeps\find-msvc.bat 14
    IF ERRORLEVEL 1 exit /B %ERRORLEVEL%
) ELSE IF NOT "%VisualStudioVersion%" == "14.0" (
	echo "Visual Studio 14.0 (2015) is required"
	exit /B 1
) ELSE IF NOT "%Platform%" == "X64" (
	echo "Visual Studio 14.0 (2015) must be set for X64 platform"
	exit /B 1
)

IF NOT "%VCROOT%" == "" (
    call "%VCROOT%\vcvarsall.bat" amd64
)

call configure-polaris.cmd %DEPS_DIR%
call build-polaris.cmd
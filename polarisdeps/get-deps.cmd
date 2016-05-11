rem @ECHO OFF

IF NOT "%1" == "" (
	set BASEDIR=%1
	call polaris_env.bat %BASEDIR%
) ELSE IF NOT "%POLARIS_DEPS_DIR%" == "" (
	set BASEDIR=%POLARIS_DEPS_DIR%
) ELSE (
	set BASEDIR=%CD%
	call polaris_env.bat %BASEDIR%
)

echo %BASEDIR%
IF NOT EXIST %BASEDIR% (
	MKDIR %BASEDIR%
)
COPY polaris_env.bat %BASEDIR%

REM  see if Vsual Studio is already set
SET VCROOT=
IF "%VSINSTALLDIR%" == "" (
	call find_msvc.bat 14
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

:: get our slashes all straightened out :)
set a=%BASEDIR%
set a=%a:/=\%
echo %a%
set BASEDIR=%a%

cd /D %~dp0
call %~dp0build-boost-1.60.0.cmd %BASEDIR%
cd /D %~dp0
call %~dp0build-odb-2.4.0.cmd %BASEDIR%
cd /D %~dp0
call %~dp0build-gtest-1.7.0.cmd %BASEDIR%

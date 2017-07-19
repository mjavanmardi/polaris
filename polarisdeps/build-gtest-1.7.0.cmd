@ECHO OFF
::ECHO This is just a test
::EXIT /B 1

SETLOCAL

IF NOT "%1" == "" (
	set BASEDIR=%1
) ELSE IF NOT "%POLARIS_DEPS_DIR%" == "" (
	  set BASEDIR=%POLARIS_DEPS_DIR%
) ELSE (
	set BASEDIR=c:\opt\polarisdeps
)

:: get our slashes all straightened out :)
set a=%BASEDIR%
set a=%a:/=\%
echo %a%
set BASEDIR=%a%

REM  see if Vsual Studio is already set
SET VCROOT=
IF "%VSINSTALLDIR%" == "" (
	call find-msvc.bat 14
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

:: Download and expand source files
set GTESTZIPFILE=%BASEDIR%\release-1.7.0.zip
set GTESTDIR=%BASEDIR%\googletest-release-1.7.0

echo file=%GTESTZIPFILE%
echo dir=%GTESTDIR%

set ERRORLEVEL=
IF NOT EXIST %GTESTDIR% ( mkdir %GTESTDIR% )
cd /D %BASEDIR%
%~dp0utils\wget --show-progress=off -O %GTESTZIPFILE% "https://github.com/google/googletest/archive/release-1.7.0.zip"
%~dp0utils\unzip -o -q %GTESTZIPFILE%
IF ERRORLEVEL 1 (ECHO Download and Extract of '%GTESTZIPFILE%' failed. & ECHO STATUS: FAIL & ENDLOCAL & EXIT /B 1)

set BUILDDIR=%GTESTDIR%\build_msvc2015
mkdir %BUILDDIR%
cd /D %BUILDDIR%

set DEBUG_BUILD=0
set RELEASE_BUILD=0
set BUILD_ERROR=0

set ERRORLEVEL=
cmake -D  gtest_force_shared_crt=TRUE -G "Visual Studio 14 Win64" ..
IF ERRORLEVEL 1 (ECHO CMake was unable to generate project file. & EXIT /B 1)

set ERRORLEVEL=
msbuild gtest.sln /p:Configuration=Release /p:Platform=x64
IF ERRORLEVEL 1 (SET RELEASE_BUILD=1 & set BUILD_ERROR=1)

set ERRORLEVEL=
msbuild gtest.sln /p:Configuration=Debug /p:Platform=x64
IF ERRORLEVEL 1 (SET DEBUG_BUILD=1 & set BUILD_ERROR=1)

IF %RELEASE_BUILD% NEQ 0 (ECHO MSBuild of gtest 1.7.0 Release project - FAIL )
IF %DEBUG_BUILD% NEQ 0  (ECHO MSBuild of gtest 1.7.0 Debug project - FAIL )

cd /D %~dp0
call DisplayDate.cmd
IF %BUILD_ERROR% NEQ 0 (ECHO STATUS: FAIL & ENDLOCAL & EXIT /B 1)
ENDLOCAL
ECHO STATUS: SUCCESS

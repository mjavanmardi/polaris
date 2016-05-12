@ECHO OFF

IF NOT "%1" == "" (
	set BASEDIR=%1
) ELSE IF NOT "%POLARIS_DEPS_DIR%" == "" (
	  set BASEDIR=%POLARIS_DEPS_DIR%
) ELSE (
	set BASEDIR=%CD%
)

:: get our slashes all straightened out :)
set a=%BASEDIR%
set a=%a:/=\%
echo %a%
set BASEDIR=%a%

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

:: cal this because for some goofy reason it fails on the first call - but then works
call find_python.cmd

:: Download and expand source files
set GTESTZIPFILE=release-1.7.0.zip
set GTESTDIR=%BASEDIR%\googletest-release-1.7.0

echo file=%GTESTZIPFILE%
echo dir=%GTESTDIR%


call find_python.cmd
IF "%MYPYTHONPATH%" == "" ( ECHO "Can't find python" & EXIT /B 1)

%MYPYTHONPATH% myWget.py -u "https://github.com/google/googletest/archive/release-1.7.0.zip" -n %GTESTZIPFILE% -e %GTESTDIR% -o %BASEDIR%

set BUILDDIR=%GTESTDIR%\build_msvc2015
mkdir %BUILDDIR%
cd /D %BUILDDIR%

cmake -D  gtest_force_shared_crt=TRUE -G "Visual Studio 14 Win64" ..

msbuild gtest.sln /p:Configuration=Release /p:Platform=x64
msbuild gtest.sln /p:Configuration=Debug /p:Platform=x64

cd /D %~dp0

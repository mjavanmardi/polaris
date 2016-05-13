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
set BOOSTZIPFILE=boost_1_60_0.zip
set BOOSTDIR=%BASEDIR%\boost_1_60_0

echo file=%BOOSTZIPFILE%
echo dir=%BOOSTDIR%

call find_python.cmd
IF "%MYPYTHONPATH%" == "" ( ECHO "Can't find python" & EXIT /B 1)

set ERRORLEVEL=
%MYPYTHONPATH% myWget.py -u "http://sourceforge.net/projects/boost/files/boost/1.60.0/boost_1_60_0.zip" -n %BOOSTZIPFILE% -e %BOOSTDIR% -o %BASEDIR%
IF ERRORLEVEL 1 (ECHO Download and Extract of '%BOOSTZIPFILE%' failed. & EXIT /B 1)

:: if you want to use boost libraries (as opposed to just headers)
:: uncomment the commands here:

::cd %BOOSTDIR%
::call bootstrap.bat
::b2 address-model=64 link=shared,static variant=release install --prefix=%BOOSTDIR%
::b2 address-model=64 link=shared,static variant=debug install --prefix=%BOOSTDIR%
::cd %BOOSTDIR%
::ren lib lib64-msvc-14.0

cd %~dp0

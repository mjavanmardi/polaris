@ECHO OFF
::ECHO This is just a test
::EXIT /B 1

SETLOCAL

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

:: call this because for some goofy reason it fails on the first call - but then works
call find-python.cmd

:: Download and expand source files
set BOOSTZIPFILE=boost_1_60_0.zip
set BOOSTDIR=%BASEDIR%\boost_1_60_0

echo file=%BOOSTZIPFILE%
echo dir=%BOOSTDIR%

call find-python.cmd
IF "%MYPYTHONPATH%" == "" ( ECHO "Can't find python" & EXIT /B 1) 

set DEBUG_BUILD=0
set RELEASE_BUILD=0
set BUILD_ERROR=0

set ERRORLEVEL=
%MYPYTHONPATH% myWget.py -u "http://sourceforge.net/projects/boost/files/boost/1.60.0/boost_1_60_0.zip" -n %BOOSTZIPFILE% -e %BASEDIR% -o %BASEDIR%
IF ERRORLEVEL 1 ( ECHO Download and Extract of '%BOOSTZIPFILE%' - FAIL  & ECHO STTATUS: FAIL & ENDLOCAL & EXIT /B 1 )

:: if you want to use boost libraries (as opposed to just headers)
:: uncomment the commands here:

::cd /D %BOOSTDIR%
::call bootstrap.bat

::set ERRORLEVEL=
::b2 address-model=64 link=shared,static variant=release install --prefix=%BOOSTDIR% --without-python
::IF ERRORLEVEL 1 (SET RELEASE_BUILD=1 & set BUILD_ERROR=1)

::set ERRORLEVEL=
::b2 address-model=64 link=shared,static variant=debug install --prefix=%BOOSTDIR% --without-python
::IF ERRORLEVEL 1 (SET DEBUG_BUILD=1 & set BUILD_ERROR=1)

::cd /D %BOOSTDIR%
::ren lib lib64-msvc-14.0

::IF %RELEASE_BUILD% NEQ 0 (ECHO MSBuild of Boost 1.60.0 Release project - FAIL )
::IF %DEBUG_BUILD% NEQ 0  (ECHO MSBuild of Boost 1.60.0 Debug project - FAIL )

::cd /D %~dp0
call DisplayDate.cmd
IF %BUILD_ERROR% NEQ 0 (ECHO STATUS: FAIL & ENDLOCAL & EXIT /B 1)
ENDLOCAL
ECHO STATUS: SUCCESS

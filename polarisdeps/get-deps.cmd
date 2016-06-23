@ECHO OFF

IF NOT "%1" == "" (
	set BASEDIR=%1
	call polaris-env.bat %1
) ELSE IF NOT "%POLARIS_DEPS_DIR%" == "" (
	set BASEDIR=%POLARIS_DEPS_DIR%
) ELSE (
	set BASEDIR=%CD%
	call polaris-env.bat %CD%
)

echo %BASEDIR%
IF NOT EXIST %BASEDIR% (
	MKDIR %BASEDIR%
)
COPY polaris-env.bat %BASEDIR%

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

:: cal this because for some goofy reason it fails on the first call - but then works
call find-python.cmd

:: get our slashes all straightened out :)
set a=%BASEDIR%
set a=%a:/=\%
echo %a%
set BASEDIR=%a%

::cd /D %~dp0
::call %~dp0build-boost-1.60.0.cmd %BASEDIR%
::cd /D %~dp0
::call %~dp0build-odb-2.4.0.cmd %BASEDIR%
::cd /D %~dp0
::call %~dp0build-gtest-1.7.0.cmd %BASEDIR%

SET LOGDIR=%BASEDIR%\builds
IF NOT EXIST %LOGDIR% ( mkdir %LOGDIR% )
DEL %LOGDIR%\boost_build.log
DEL %LOGDIR%\odb_build.log
DEL %LOGDIR%\gtest_build.log
DEL %LOGDIR%\antares_build.log

set BOOST_BUILD=0
set ODB_BUILD=0
set GTEST_BUILD=0
set ANTARES_BUILD=0
set BUILD_ERROR=0

cd /D %~dp0
set ERRORLEVEL=
call %~dp0build-boost-1.60.0.cmd %BASEDIR%	| tee %LOGDIR%\boost_build.log
IF %ERRORLEVEL% NEQ 0 (SET BOOST_BUILD=1 & set BUILD_ERROR=1)

cd /D %~dp0
set ERRORLEVEL=
call %~dp0build-odb-2.4.0.cmd %BASEDIR%		| tee %LOGDIR%\odb_build.log
IF %ERRORLEVEL% NEQ 0 (SET ODB_BUILD=1 & set BUILD_ERROR=1)

cd /D %~dp0
set ERRORLEVEL=
call %~dp0build-gtest-1.7.0.cmd %BASEDIR%	| tee %LOGDIR%\gtest_build.log
IF %ERRORLEVEL% NEQ 0 (SET GTEST_BUILD=1 & set BUILD_ERROR=1)

cd /D %~dp0
set ERRORLEVEL=
::call %~dp0get-antares-deps.cmd %BASEDIR%
IF %ERRORLEVEL% NEQ 0 (SET ANTARES_BUILD=1 & set BUILD_ERROR=1)

cd /D %~dp0

:: as a convenience we copy all dll's to a single folder to save on path entries
IF NOT EXIST %BASEDIR%\bin ( mkdir %BASEDIR%\bin )

IF NOT EXIST %BASEDIR%\bin\Release ( mkdir %BASEDIR%\bin\Release )
COPY %BASEDIR%\odb-2.4.0-i686-windows\bin\odb.exe								  %BASEDIR%\bin\Release
COPY %BASEDIR%\libodb-2.4.0\bin64\odb-2.4-vc14.dll                                %BASEDIR%\bin\Release
COPY %BASEDIR%\libodb-2.4.0\bin64\odb-2.4-vc14.pdb                                %BASEDIR%\bin\Release
COPY %BASEDIR%\libodb-2.4.0\libodb-sqlite-2.4.0\bin64\odb-sqlite-2.4-vc14.dll     %BASEDIR%\bin\Release
COPY %BASEDIR%\libodb-2.4.0\libodb-sqlite-2.4.0\bin64\odb-sqlite-2.4-vc14.pdb     %BASEDIR%\bin\Release

IF NOT EXIST %BASEDIR%\bin\Debug ( mkdir %BASEDIR%\bin\Debug )
COPY %BASEDIR%\odb-2.4.0-i686-windows\bin\odb.exe								  %BASEDIR%\bin\Debug
COPY %BASEDIR%\libodb-2.4.0\bin64\odb-d-2.4-vc14.dll                              %BASEDIR%\bin\Debug
COPY %BASEDIR%\libodb-2.4.0\bin64\odb-d-2.4-vc14.pdb                              %BASEDIR%\bin\Debug
COPY %BASEDIR%\libodb-2.4.0\libodb-sqlite-2.4.0\bin64\odb-sqlite-d-2.4-vc14.dll   %BASEDIR%\bin\Debug
COPY %BASEDIR%\libodb-2.4.0\libodb-sqlite-2.4.0\bin64\odb-sqlite-d-2.4-vc14.pdb   %BASEDIR%\bin\Debug

IF %BOOST_BUILD%	NEQ 0 (ECHO Build of Boost 1.60.0 failed.)
IF %ODB_BUILD%		NEQ 0 (ECHO Build of ODB 2.4.0 failed.)
IF %GTEST_BUILD%	NEQ 0 (ECHO Build of GTest 1.7.0 failed.)
IF %ANTARES_BUILD%	NEQ 0 (ECHO Build of Antares dependencies failed.)

cd /D %~dp0
IF %BUILD_ERROR% NEQ 0 (ECHO STATUS: FAIL & EXIT /B 1)
ECHO STATUS: SUCCESS


rem @ECHO OFF

IF NOT "%1" == "" (
	set BASEDIR=%1
	call polaris_env.bat %1
) ELSE IF NOT "%POLARIS_DEPS_DIR%" == "" (
	set BASEDIR=%POLARIS_DEPS_DIR%
) ELSE (
	set BASEDIR=%CD%
	call polaris_env.bat %CD%
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

:: cal this because for some goofy reason it fails on the first call - but then works
call find_python.cmd

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

:: as a convenience we copy all dll's to a single folder to save on path entries
mkdir %BASEDIR%\bin
COPY %BASEDIR%\odb-2.4.0-i686-windows\bin\odb.exe								  %BASEDIR%\bin
COPY %BASEDIR%\libodb-2.4.0\bin64\odb-2.4-vc14.dll                                %BASEDIR%\bin
COPY %BASEDIR%\libodb-2.4.0\bin64\odb-2.4-vc14.pdb                                %BASEDIR%\bin
COPY %BASEDIR%\libodb-2.4.0\bin64\odb-d-2.4-vc14.dll                              %BASEDIR%\bin
COPY %BASEDIR%\libodb-2.4.0\bin64\odb-d-2.4-vc14.pdb                              %BASEDIR%\bin
COPY %BASEDIR%\libodb-2.4.0\libodb-sqlite-2.4.0\bin64\odb-sqlite-2.4-vc14.dll     %BASEDIR%\bin
COPY %BASEDIR%\libodb-2.4.0\libodb-sqlite-2.4.0\bin64\odb-sqlite-2.4-vc14.pdb     %BASEDIR%\bin
COPY %BASEDIR%\libodb-2.4.0\libodb-sqlite-2.4.0\bin64\odb-sqlite-d-2.4-vc14.dll   %BASEDIR%\bin
COPY %BASEDIR%\libodb-2.4.0\libodb-sqlite-2.4.0\bin64\odb-sqlite-d-2.4-vc14.pdb   %BASEDIR%\bin

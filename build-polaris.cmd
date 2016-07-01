@ECHO OFF

SETLOCAL

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

cd /D %~dp0build_vs2015

SET BUILD_ERROR=0
SET DEBUG_BUILD_ERROR=0
SET RELEASE_BUILD_ERROR=0
SET ANTARES_DEBUG_BUILD_ERROR=0
SET ANTARES_RELEASE_BUILD_ERROR=0

SET ERRORLEVEL=
msbuild polaris.sln /p:Configuration=Debug /p:Platform=x64
IF %ERRORLEVEL% NEQ 0 ( SET DEBUG_BUILD_ERROR=1 & SET BUILD_ERROR=1)

SET ERRORLEVEL=
msbuild polaris.sln /p:Configuration=Release /p:Platform=x64
IF %ERRORLEVEL% NEQ 0 ( SET RELEASE_BUILD_ERROR=1 & SET BUILD_ERROR=1)

SET ERRORLEVEL=
::msbuild polaris_antares.sln /p:Configuration=Debug /p:Platform=x64
IF %ERRORLEVEL% NEQ 0 ( SET ANTARES_DEBUG_BUILD_ERROR=1 & SET BUILD_ERROR=1)

SET ERRORLEVEL=
::msbuild polaris_antares.sln /p:Configuration=Release /p:Platform=x64
IF %ERRORLEVEL% NEQ 0 ( SET ANTARES_RELEASE_BUILD_ERROR=1 & SET BUILD_ERROR=1)

cd /D %~dp0

IF %DEBUG_BUILD_ERROR% NEQ 0 ( ECHO Building Debug Polaris - FAIL )
IF %RELEASE_BUILD_ERROR% NEQ 0 ( ECHO Building Release Polaris - FAIL )
IF %ANTARES_DEBUG_BUILD_ERROR% NEQ 0 ( ECHO Building Debug Antares Polaris - FAIL )
IF %ANTARES_RELEASE_BUILD_ERROR% NEQ 0 ( ECHO Building Release Antares Polaris - FAIL )

call polarisdeps\DisplayDate.cmd
IF %BUILD_ERROR% NEQ 0 (ECHO STATUS: FAIL & ENDLOCAL & EXIT /B 1)
ENDLOCAL
ECHO STATUS: SUCCESS



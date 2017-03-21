@ECHO OFF

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


:: Download and expand source files
set ZIPFILE=glew-1.13.0.zip
set LIBDIR=%BASEDIR%\glew-1.13.0

echo ZIPFILE=	%ZIPFILE%
echo LIBDIR=	%LIBDIR%
echo filedir=	%~dp0

set ERRORLEVEL=
%MYPYTHONPATH% myWget.py -u "https://sourceforge.net/projects/glew/files/glew/1.13.0/glew-1.13.0.zip" -n %ZIPFILE% -e %BASEDIR% -o %BASEDIR% -r
IF ERRORLEVEL 1 (ECHO Download and Extract of '%ZIPFILE%' - FAIL & cd /D %~dp0 & ENDLOCAL & EXIT /B 1)

set BUILDDIR=%LIBDIR%\build_vs2015
IF NOT EXIST %BUILDDIR% ( mkdir %BUILDDIR% )
cd %BUILDDIR%

SET ERRORLEVEL=
cmake -DCMAKE_CONFIGURATION_TYPES="Debug;Release" -G "Visual Studio 14 Win64" ..\build\cmake
IF ERRORLEVEL 1 (ECHO Error configuring Glew 1.13.0  projects. & cd /D %~dp0 & EXIT /B 1)

set DEBUG_BUILD=0
set RELEASE_BUILD=0
set BUILD_ERROR=0

set ERRORLEVEL=
msbuild glew.sln /p:Platform=x64 /p:Configuration=Release
IF ERRORLEVEL 1 (SET RELEASE_BUILD=1 & set BUILD_ERROR=1)

set ERRORLEVEL=
msbuild glew.sln /p:Platform=x64 /p:Configuration=Debug
IF %ERRORLEVEL% NEQ 0 (SET DEBUG_BUILD=1 & set BUILD_ERROR=1)

IF %RELEASE_BUILD% NEQ 0 (ECHO MSBuild of Glew 1.13.0 Release project - FAIL)
IF %DEBUG_BUILD% NEQ 0  (ECHO MSBuild of Glew 1.13.0 Debug project - FAIL)

cd /D %~dp0
call DisplayDate.cmd
IF %BUILD_ERROR% NEQ 0 (ECHO STATUS: FAIL &  ENDLOCAL & EXIT /B 1)
ENDLOCAL
ECHO STATUS: SUCCESS

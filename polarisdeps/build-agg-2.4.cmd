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


:: Download and expand source files
set ZIPFILE=agg-svn-117.zip
set LIBDIR=%BASEDIR%\agg-svn-117\agg-2.4

echo ZIPFILE=	%ZIPFILE%
echo LIBDIR=	%LIBDIR%
echo filedir=   %~dp0

set ERRORLEVEL=
%MYPYTHONPATH% myWget.py -u "https://sourceforge.net/code-snapshots/svn/a/ag/agg/svn/agg-svn-117.zip" -n %ZIPFILE% -e %BASEDIR% -o %BASEDIR% -r
IF ERRORLEVEL 1 (ECHO Download and Extract of '%FREETYPEZIPFILE%' failed. & cd /D %~dp0 & EXIT /B 1)

set BUILDDIR=%LIBDIR%\build_vs2015
IF NOT EXIST %BUILDDIR% ( mkdir %BUILDDIR% )
cd %BUILDDIR%

SET ERRORLEVEL=
cmake -DCMAKE_CONFIGURATION_TYPES="Debug;Release" -G "Visual Studio 14 Win64" ..
IF ERRORLEVEL 1 (ECHO Error configuring AGG 2.4 projects. & cd /D %~dp0 & EXIT /B 1)

set DEBUG_BUILD=0
set RELEASE_BUILD=0
set BUILD_ERROR=0

set ERRORLEVEL=
msbuild antigrain.sln /p:Platform=x64 /p:Configuration=Release
IF ERRORLEVEL 1 (SET RELEASE_BUILD=1 & set BUILD_ERROR=1)

set ERRORLEVEL=
msbuild antigrain.sln /p:Platform=x64 /p:Configuration=Debug
IF %ERRORLEVEL% NEQ 0 (SET DEBUG_BUILD=1 & set BUILD_ERROR=1)

IF %RELEASE_BUILD% NEQ 0 (ECHO MSBuild of AGG 2.4 Release project failed.)
IF %DEBUG_BUILD% NEQ 0  (ECHO MSBuild of AGG 2.4 Debug project failed.)

cd /D %~dp0
IF %BUILD_ERROR% NEQ 0 (ECHO STATUS: FAIL & EXIT /B 1)
ECHO STATUS: SUCCESS



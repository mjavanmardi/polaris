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
set FREETYPEZIPFILE=ft263.zip
set FREETYPEDIR=%BASEDIR%\freetype-2.6.3

echo FREETYPEZIPFILE=	%FREETYPEZIPFILE%
echo FREETYPEDIR=		%FREETYPEDIR%
echo filedir=           %~dp0

set ERRORLEVEL=
%MYPYTHONPATH% myWget.py -u "https://sourceforge.net/projects/freetype/files/freetype2/2.6.3/ft263.zip" -n %FREETYPEZIPFILE% -e %BASEDIR% -o %BASEDIR% -r
IF ERRORLEVEL 1 (ECHO Download and Extract of '%FREETYPEZIPFILE%' failed. & cd /D %~dp0 & EXIT /B 1)

set BUILDDIR=%FREETYPEDIR%\build_vs2015
IF NOT EXIST %BUILDDIR% ( mkdir %BUILDDIR% )
cd %BUILDDIR%

SET ERRORLEVEL=
cmake -DCMAKE_CONFIGURATION_TYPES="Debug;Release" -G "Visual Studio 14 Win64" ..
IF ERRORLEVEL 1 (ECHO Error configuring FreeType 2.6.3  projects. & cd /D %~dp0 & EXIT /B 1)

set DEBUG_BUILD=0
set RELEASE_BUILD=0
set BUILD_ERROR=0

set ERRORLEVEL=
msbuild freetype.sln /p:Platform=x64 /p:Configuration=Release
IF ERRORLEVEL 1 (SET RELEASE_BUILD=1 & set BUILD_ERROR=1)

set ERRORLEVEL=
msbuild freetype.sln /p:Platform=x64 /p:Configuration=Debug
IF %ERRORLEVEL% NEQ 0 (SET DEBUG_BUILD=1 & set BUILD_ERROR=1)

IF %RELEASE_BUILD% NEQ 0 (ECHO MSBuild of FreeType 2.6.3 Release project failed.)
IF %DEBUG_BUILD% NEQ 0  (ECHO MSBuild of FreeType 2.6.3 Debug project failed.)

cd /D %~dp0
IF %BUILD_ERROR% NEQ 0 (ECHO STATUS: FAIL & EXIT /B 1)
ECHO STATUS: SUCCESS

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
set PLPLOTZIPFILE=plplot-5.11.1.tar.gz
set PLPLOTDIR=%BASEDIR%\plplot-5.11.1

echo PLPLOTZIPFILE=		%PLPLOTZIPFILE%
echo PLPLOTDIR=		    %PLPLOTDIR%
echo filedir=           %~dp0

set ERRORLEVEL=
%MYPYTHONPATH% myWget.py -u "https://sourceforge.net/projects/plplot/files/plplot/5.11.1 Source/plplot-5.11.1.tar.gz" -n %PLPLOTZIPFILE% -e %PLPLOTDIR% -o %BASEDIR% -r
IF ERRORLEVEL 1 (ECHO Download and Extract of '%PLPLOTZIPFILE%' failed. & cd /D %~dp0 & EXIT /B 1)

echo creating build directories
set BUILDDIR=%PLPLOTDIR%\build_vs2015
IF NOT EXIST %BUILDDIR% ( mkdir %BUILDDIR% )
IF NOT EXIST %BUILDDIR%\debug ( mkdir %BUILDDIR%\debug )
IF NOT EXIST %BUILDDIR%\release ( mkdir %BUILDDIR%\release )

set DEBUG_BUILD=0
set RELEASE_BUILD=0
set BUILD_ERROR=0


cd %BUILDDIR%/release
echo running cmake WD=%PWD%
SET ERRORLEVEL=
cmake -DBUILD_TEST=OFF -DCMAKE_CONFIGURATION_TYPES="Release" -G "Visual Studio 14 Win64" ../..  -DWITH_FREETYPE=ON -DFREETYPE_INCLUDE_DIR="%BASEDIR%/freetype-2.6.3/include" -DFREETYPE_LIBRARY="%BASEDIR%/freetype-2.6.3/build_vs2015/Release/freetype.lib"
IF ERRORLEVEL 1 (ECHO Error configuring plplot projects. & cd /D %~dp0 & EXIT /B 1)

set ERRORLEVEL=
msbuild plplot.sln /p:Platform=x64 /p:Configuration=Release
IF ERRORLEVEL 1 (SET RELEASE_BUILD=1 & set BUILD_ERROR=1)

cd %BUILDDIR%/debug
SET ERRORLEVEL=
cmake -DBUILD_TEST=OFF -DCMAKE_CONFIGURATION_TYPES="Debug" -G "Visual Studio 14 Win64" ../..  -DWITH_FREETYPE=ON -DFREETYPE_INCLUDE_DIR="%BASEDIR%/freetype-2.6.3/include" -DFREETYPE_LIBRARY="%BASEDIR%/freetype-2.6.3/build_vs2015/Debug/freetyped.lib"
IF ERRORLEVEL 1 (ECHO Error configuring plplot projects. & cd /D %~dp0 & EXIT /B 1)

set ERRORLEVEL=
msbuild plplot.sln /p:Platform=x64 /p:Configuration=Debug
IF %ERRORLEVEL% NEQ 0 (SET DEBUG_BUILD=1 & set BUILD_ERROR=1)

IF %RELEASE_BUILD% NEQ 0 (ECHO MSBuild of plplot 5.11.1 Release project failed.)
IF %DEBUG_BUILD% NEQ 0  (ECHO MSBuild of plplot 5.11.1 Debug project failed.)

cd /D %~dp0
IF %BUILD_ERROR% NEQ 0 (EXIT /B 1)


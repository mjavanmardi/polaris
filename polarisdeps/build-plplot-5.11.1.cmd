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
%MYPYTHONPATH% myWget.py -u "https://sourceforge.net/projects/plplot/files/plplot/5.11.1 Source/plplot-5.11.1.tar.gz" -n %PLPLOTZIPFILE% -e %BASEDIR% -o %BASEDIR% -r
IF ERRORLEVEL 1 (ECHO Download and Extract of '%PLPLOTZIPFILE%' failed. & cd /D %~dp0 & EXIT /B 1)

https://sourceforge.net/p/plplot/bugs/_discuss/thread/b199e922/490a/attachment/patch

::apply patch
set ERRORLEVEL=
%MYPYTHONPATH% myWget.py -u "https://sourceforge.net/p/plplot/bugs/_discuss/thread/b199e922/490a/attachment/patch" -n plplot.patch -e %PLPLOTDIR% -o %PLPLOTDIR% -r
IF ERRORLEVEL 1 (ECHO Download and Extract of 'patch' failed. & cd /D %~dp0 & EXIT /B 1)

CD /D %PLPLOTDIR%
git apply --stat --apply plplot.patch

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
cmake -G "Visual Studio 14 Win64" ..\.. -DCMAKE_CONFIGURATION_TYPES="Release"  -DPL_DOUBLE=ON -DLIB_TAG="su" -DBUILD_TEST=ON -DBUILD_SHARED_LIBS=OFF -DSTATIC_RUNTIME=ON -DwxWidgets_CONFIGURATION=mswud -DHAVE_SHAPELIB=ON -DSHAPELIB_INCLUDE_DIR="%BASEDIR%/shapelib-1.3.0/include" -DSHAPELIB_LIBRARY="%BASEDIR%/shapelib-1.3.0/lib/shapelib_i.lib" -DHAVE_AGG=ON -DAGG_INCLUDE_DIR="%BASEDIR%/agg-svn-117/agg-2.4/include" -DAGG_LIBRARIES="%BASEDIR%/agg-svn-117/agg-2.4/build_vs2015/lib/Release/agg.lib" -DWITH_FREETYPE=ON -DFREETYPE_INCLUDE_DIR="%BASEDIR%/freetype-2.6.3/include" -DFREETYPE_LIBRARY="%BASEDIR%/freetype-2.6.3/build_vs205/Release/freetype.lib"
::cmake -DBUILD_TEST=OFF -DCMAKE_CONFIGURATION_TYPES="Release" -G "Visual Studio 14 Win64" ../..  -DWITH_FREETYPE=OFF 
IF ERRORLEVEL 1 (ECHO Error configuring plplot projects. & cd /D %~dp0 & EXIT /B 1)

set ERRORLEVEL=
msbuild plplot.sln /p:Platform=x64 /p:Configuration=Release
IF ERRORLEVEL 1 (SET RELEASE_BUILD=1 & set BUILD_ERROR=1)

cd %BUILDDIR%/debug
SET ERRORLEVEL=
cmake -G "Visual Studio 14 Win64" ..\.. -DCMAKE_CONFIGURATION_TYPES="Debug"  -DPL_DOUBLE=ON -DLIB_TAG="su" -DBUILD_TEST=ON -DBUILD_SHARED_LIBS=OFF -DSTATIC_RUNTIME=ON -DwxWidgets_CONFIGURATION=mswud -DHAVE_SHAPELIB=ON -DSHAPELIB_INCLUDE_DIR="%BASEDIR%/shapelib-1.3.0/include" -DSHAPELIB_LIBRARY="%BASEDIR%/shapelib-1.3.0/lib/shapelib_i_d.lib" -DHAVE_AGG=ON -DAGG_INCLUDE_DIR="%BASEDIR%/agg-svn-117/agg-2.4/include" -DAGG_LIBRARIES="%BASEDIR%/agg-svn-117/agg-2.4/build_vs2015/lib/Debug/agg.lib" -DWITH_FREETYPE=ON -DFREETYPE_INCLUDE_DIR="%BASEDIR%/freetype-2.6.3/include" -DFREETYPE_LIBRARY="%BASEDIR%/freetype-2.6.3/build_vs205/Debug/freetyped.lib"
::cmake "../.." -G "Visual Studio 14 Win64" -DPL_DOUBLE=ON -DBUILD_TEST=ON -DCMAKE_INSTALL_PREFIX="../Debug_Install" -DCMAKE_CONFIGURATION_TYPES="Debug" -DBUILD_SHARED_LIBS=OFF -DSTATIC_RUNTIME=ON
IF ERRORLEVEL 1 (ECHO Error configuring plplot projects. & cd /D %~dp0 & EXIT /B 1)

set ERRORLEVEL=
msbuild plplot.sln /p:Platform=x64 /p:Configuration=Debug
IF ERRORLEVEL 1 (SET DEBUG_BUILD=1 & set BUILD_ERROR=1)

IF %RELEASE_BUILD% NEQ 0 (ECHO MSBuild of plplot 5.11.1 Release project failed.)
IF %DEBUG_BUILD% NEQ 0  (ECHO MSBuild of plplot 5.11.1 Debug project failed.)

cd /D %~dp0
IF %BUILD_ERROR% NEQ 0 (EXIT /B 1)


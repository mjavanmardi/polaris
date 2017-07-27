@ECHO OFF

SETLOCAL

IF NOT "%1" == "" (
	set BASEDIR=%1
) ELSE IF NOT "%POLARIS_DEPS_DIR%" == "" (
	  set BASEDIR=%POLARIS_DEPS_DIR%
) ELSE (
	set BASEDIR=c:\opt\polarisdeps
)

:: get our slashes all straightened out :)
set a=%BASEDIR%
set a=%a:/=\%
echo %a%
set BASEDIR=%a%

IF "%WXWIN%" == "" ( SET WXWIN=%BASEDIR%\wxWidgets-3.1.0)
IF "%wxWidgets_ROOT_DIR%" == "" ( SET WXWIN=%BASEDIR%\wxWidgets-3.1.0)
IF "%wxWidgets_LIBRARY_DIRS%" == "" (SET wxWidgets_LIBRARY_DIRS=%WXWIN%\lib\vc_x64_dll)
ECHO WXWIN=%WXWIN%
ECHO wxWidgets_ROOT_DIR=%wxWidgets_ROOT_DIR%
ECHO wxWidgets_LIBRARY_DIRS=%wxWidgets_LIBRARY_DIRS%

:: Download and expand source files
set PLPLOTZIPFILE=plplot-5.11.1.tar.gz
set PLPLOTDIR=%BASEDIR%\plplot-5.11.1

::echo PLPLOTZIPFILE=		%PLPLOTZIPFILE%
echo PLPLOTDIR=		    %PLPLOTDIR%
echo filedir=           %~dp0

set ERRORLEVEL=
::%MYPYTHONPATH% myWget.py -u "https://sourceforge.net/projects/plplot/files/plplot/5.11.1 Source/plplot-5.11.1.tar.gz" -n %PLPLOTZIPFILE% -e %BASEDIR% -o %BASEDIR% -r
:: There are problems building the 5.11.1 release with VS2015
:: It required too many modifications for it to build correctly
:: the following commit (later than 5.11.1) does build
:: commit fdd31ce68b219dbbde39e92fcd2be976c9c6c2c6
:: so clone the reposirory andreset to that commit until the latest release is sorted
git clone "git://git.code.sf.net/p/plplot/plplot" %PLPLOTDIR%
CD /D %PLPLOTDIR%
git checkout fdd31ce68b219dbbde39e92fcd2be976c9c6c2c6
IF ERRORLEVEL 1 (ECHO Clone of PLPlot reposirory had issues - FAIL & cd /D %~dp0 & ENDLOCAL & EXIT /B 1)

echo creating build directories
set BUILDDIR=%PLPLOTDIR%\build_vs2015
IF NOT EXIST %BUILDDIR% ( mkdir %BUILDDIR% )
IF NOT EXIST %BUILDDIR%\debug ( mkdir %BUILDDIR%\debug )
IF NOT EXIST %BUILDDIR%\release ( mkdir %BUILDDIR%\release )

set DEBUG_BUILD=0
set RELEASE_BUILD=0
set BUILD_ERROR=0


cd %BUILDDIR%/release
echo running cmake WD=%CD%
SET ERRORLEVEL=
cmake -G "Visual Studio 14 Win64" ..\.. -DCMAKE_CONFIGURATION_TYPES="Release" -DBUILD_TEST:BOOL=OFF -DPL_DOUBLE=ON -DLIB_TAG="su" -DBUILD_TEST=ON -DBUILD_SHARED_LIBS=OFF -DSTATIC_RUNTIME=OFF -DwxWidgets_CONFIGURATION=mswu -DHAVE_SHAPELIB=ON -DSHAPELIB_INCLUDE_DIR="%BASEDIR%/shapelib-1.3.0/include" -DSHAPELIB_LIBRARY="%BASEDIR%/shapelib-1.3.0/lib/shapelib_i.lib" -DWITH_FREETYPE=ON -DFREETYPE_INCLUDE_DIRS="%BASEDIR%/freetype-2.6.3/include" -DFREETYPE_LIBRARY="%BASEDIR%/freetype-2.6.3/build_vs2015/Release/freetype.lib"
IF ERRORLEVEL 1 (ECHO Error configuring plplot projects. & cd /D %~dp0 & EXIT /B 1)

set ERRORLEVEL=
msbuild plplot.sln /t:plplotcxx;plplot;qsastime;csirocsa;plplotwxwidgets /p:Platform=x64 /p:Configuration=Release
IF ERRORLEVEL 1 (SET RELEASE_BUILD=1 & set BUILD_ERROR=1)

cd %BUILDDIR%/debug
SET ERRORLEVEL=
cmake -G "Visual Studio 14 Win64" ..\.. -DCMAKE_CONFIGURATION_TYPES="Debug" -DBUILD_TEST:BOOL=OFF -DPL_DOUBLE=ON -DLIB_TAG="su" -DBUILD_TEST=ON -DBUILD_SHARED_LIBS=OFF -DSTATIC_RUNTIME=OFF -DwxWidgets_CONFIGURATION=mswud -DHAVE_SHAPELIB=ON -DSHAPELIB_INCLUDE_DIR="%BASEDIR%/shapelib-1.3.0/include" -DSHAPELIB_LIBRARY="%BASEDIR%/shapelib-1.3.0/lib/shapelib_i_d.lib" -DWITH_FREETYPE=ON -DFREETYPE_INCLUDE_DIRS="%BASEDIR%/freetype-2.6.3/include" -DFREETYPE_LIBRARY="%BASEDIR%/freetype-2.6.3/build_vs2015/Debug/freetyped.lib"
IF ERRORLEVEL 1 (ECHO Error configuring plplot projects. & cd /D %~dp0 & EXIT /B 1)

set ERRORLEVEL=
msbuild plplot.sln /t:plplotcxx;plplot;qsastime;csirocsa;plplotwxwidgets /p:Platform=x64 /p:Configuration=Debug
IF ERRORLEVEL 1 (SET DEBUG_BUILD=1 & set BUILD_ERROR=1)

IF %RELEASE_BUILD% NEQ 0 (ECHO MSBuild of plplot 5.11.1 Release project - FAIL)
IF %DEBUG_BUILD% NEQ 0  (ECHO MSBuild of plplot 5.11.1 Debug project - FAIL)

cd /D %~dp0
call DisplayDate.cmd
IF %BUILD_ERROR% NEQ 0 (ECHO STATUS: FAIL & ENDLOCAL & EXIT /B 1)
ENDLOCAL
ECHO STATUS: SUCCESS

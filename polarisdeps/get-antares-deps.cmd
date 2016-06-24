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
IF NOT EXIST %BASEDIR%\polaris-env.bat ( COPY polaris-env.bat %BASEDIR% )

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
call find-python.cmd

:: get our slashes all straightened out :)
set a=%BASEDIR%
set a=%a:/=\%
echo %a%
set BASEDIR=%a%

SET LOGDIR=%BASEDIR%\builds
IF NOT EXIST %LOGDIR% ( mkdir %LOGDIR% )

set BUILD_ERROR=0


cd /D %~dp0
DEL %LOGDIR%\shapelib_build.log
set SHAPELIB_BUILD=0
set ERRORLEVEL=
call %~dp0build-shapelib-1.3.0.cmd %BASEDIR% > %LOGDIR%\shapelib_build.log 2>&1
IF %ERRORLEVEL% NEQ 0 (SET SHAPELIB_BUILD=1 & set BUILD_ERROR=1)

cd /D %~dp0
DEL %LOGDIR%\freetype_build.log
set FREETYPE_BUILD=0
set ERRORLEVEL=
call %~dp0build-freetype-2.6.3.cmd %BASEDIR% > %LOGDIR%\freetype_build.log 2>&1
IF %ERRORLEVEL% NEQ 0 (SET FREETYPE_BUILD=1 & set BUILD_ERROR=1)

cd /D %~dp0
DEL %LOGDIR%\wxWidgets_build.log
set WXWIDGETS_BUILD=0
set ERRORLEVEL=
call %~dp0build-wxWidgets-3.1.0.cmd %BASEDIR% > %LOGDIR%\wxWidgets_build.log 2>&1
IF %ERRORLEVEL% NEQ 0 (SET WXWIDGETS_BUILD=1 & set BUILD_ERROR=1)

cd /D %~dp0
DEL %LOGDIR%\agg_build.log
set AGG_BUILD=0
set ERRORLEVEL=
call %~dp0build-agg-2.4.cmd %BASEDIR% > %LOGDIR%\agg_build.log 2>&1 
IF %ERRORLEVEL% NEQ 0 (SET AGG_BUILD=1 & set BUILD_ERROR=1)


::Now that the dependencies are built go ahead and build plplot
cd /D %~dp0
DEL %LOGDIR%\plplot_build.log
SET PLPLOT_BUILD=0
set ERRORLEVEL=
call %~dp0build-plplot-5.11.1.cmd %BASEDIR% > %LOGDIR%\plplot_build.log 2>&1 
IF %ERRORLEVEL% NEQ 0 (SET PLPLOT_BUILD=1 & set BUILD_ERROR=1)


:: as a convenience we copy all dll's to a single folder to save on path entries
IF NOT EXIST %BASEDIR%\bin ( mkdir %BASEDIR%\bin )
IF NOT EXIST %BASEDIR%\bin\Debug ( mkdir %BASEDIR%\bin\Debug )
IF NOT EXIST %BASEDIR%\bin\Release ( mkdir %BASEDIR%\bin\Release )
::COPY %BASEDIR%\odb-2.4.0-i686-windows\bin\odb.exe								  %BASEDIR%\bin\Release
::COPY %BASEDIR%\libodb-2.4.0\bin64\odb-2.4-vc14.dll                                %BASEDIR%\bin\Release

::COPY %BASEDIR%\odb-2.4.0-i686-windows\bin\odb.exe								  %BASEDIR%\bin\Debug
::COPY %BASEDIR%\libodb-2.4.0\bin64\odb-d-2.4-vc14.dll                              %BASEDIR%\bin\Debug

IF %SHAPELIB_BUILD%		NEQ 0 (ECHO Build of ShapeLib 1.3.0 failed.)
IF %FREETYPE_BUILD%		NEQ 0 (ECHO Build of FreeType 2.6.3 failed.)
IF %WXWIDGETS_BUILD%	NEQ 0 (ECHO Build of wxWidgets 3.1.0 failed.)
IF %AGG_BUILD%			NEQ 0 (ECHO Build of AGG 2.4 failed.)
IF %PLPLOT_BUILD%		NEQ 0 (ECHO Build of plPlot 5.11.1 failed.)

cd /D %~dp0
IF %BUILD_ERROR% NEQ 0 (ECHO STATUS: FAIL & EXIT /B 1)
ECHO STATUS: SUCCESS


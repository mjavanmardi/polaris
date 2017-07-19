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


:: Download and expand source files
set WIDGETZIPFILE=wxWidgets-3.1.0.zip
set WIDGETDIR=%BASEDIR%\wxWidgets-3.1.0

echo WIDGETZIPFILE=		%WIDGETZIPFILE%
echo WIDGETDIR=		    %WIDGETDIR%
echo filedir=           %~dp0

set ERRORLEVEL=
%MYPYTHONPATH% myWget.py -u "https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.0/wxWidgets-3.1.0.zip" -n %WIDGETZIPFILE% -e %WIDGETDIR% -o %BASEDIR%
IF %ERRORLEVEL% NEQ 0 (ECHO Download and Extract of '%WIDGETZIPFILE%' - FAIL & ENDLOCAL & EXIT /B 1)

::mkdir %WIDGETDIR%
cd /D %WIDGETDIR%/build/msw

set DLL_RELEASE_BUILD=0
set DLL_DEBUG_BUILD=0
set DEBUG_BUILD=0
set RELEASE_BUILD=0
set BUILD_ERROR=0

::set ERRORLEVEL=
::msbuild wx_vc14.sln /p:Platform=x64 /p:Configuration="DLL Release"
::IF %ERRORLEVEL% NEQ 0 (SET DLL_RELEASE_BUILD=1 & set BUILD_ERROR=1)

::set ERRORLEVEL=
::msbuild wx_vc14.sln /p:Platform=x64 /p:Configuration="DLL Debug"
::IF %ERRORLEVEL% NEQ 0 (SET DLL_DEBUG_BUILD=1 & set BUILD_ERROR=1)

set ERRORLEVEL=
::msbuild wx_vc14.sln /p:Platform=x64 /p:Configuration=Release
nmake /f makefile.vc BUILD=release SHARED=1 TARGET_CPU=X64
IF %ERRORLEVEL% NEQ 0 (SET RELEASE_BUILD=1 & set BUILD_ERROR=1)

set ERRORLEVEL=
::msbuild wx_vc14.sln /p:Platform=x64 /p:Configuration=Debug
nmake /f makefile.vc BUILD=debug SHARED=1 TARGET_CPU=X64
IF %ERRORLEVEL% NEQ 0 (SET DEBUG_BUILD=1 & set BUILD_ERROR=1)

IF %DLL_RELEASE_BUILD% NEQ 0 (ECHO MSBuild of wxWidgets 3.1.0 DLL Release project - FAIL)
IF %DLL_DEBUG_BUILD% NEQ 0 (ECHO MSBuild of wxWidgets 3.1.0 DLL Debug project - FAIL)
IF %RELEASE_BUILD% NEQ 0 (ECHO MSBuild of wxWidgets 3.1.0 LIB Release project - FAIL)
IF %DEBUG_BUILD% NEQ 0 (ECHO MSBuild of wxWidgets 3.1.0 LIB Debug project - FAIL)

cd /D %~dp0
call DisplayDate.cmd
IF %BUILD_ERROR% NEQ 0 (ECHO STATUS: FAIL & ENDLOCAL & EXIT /B 1)
ENDLOCAL
ECHO STATUS: SUCCESS


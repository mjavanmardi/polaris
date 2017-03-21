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
set ZIPFILE=shapelib-1.3.0.zip
set LIBDIR=%BASEDIR%\shapelib-1.3.0

echo ZIPFILE=	%ZIPFILE%
echo LIBDIR=	%LIBDIR%
echo filedir=	%~dp0

set ERRORLEVEL=
%MYPYTHONPATH% myWget.py -u "http://download.osgeo.org/shapelib/shapelib-1.3.0.zip" -n %ZIPFILE% -e %BASEDIR% -o %BASEDIR% -r
IF ERRORLEVEL 1 (ECHO Download and Extract of '%ZIPFILE%' - FAIL & cd /D %~dp0 & ENDLOCAL & EXIT /B 1)

set DEBUG_BUILD=0
set RELEASE_BUILD=0
set BUILD_ERROR=0

cd /D %LIBDIR%
copy %~dp0shapelib_makefile_debug.vc .

SET ERRORLEVEL=
nmake -f makefile.vc SHARED=0 UNICODE=1
IF ERRORLEVEL 1 ( SET RELEASE_BUILD=1 )

SET ERRORLEVEL=
nmake -f shapelib_makefile_debug.vc SHARED=0 UNICODE=1
IF ERRORLEVEL 1 ( SET DEBUG_BUILD=1 )

mkdir include
copy *.h include
mkdir lib
move *.lib lib
move *.dll lib
::move *.pdb lib
mkdir bin
move *.exe bin

IF %RELEASE_BUILD% NEQ 0 (ECHO MSBuild of ShapeLib 1.3.0 Release project - FAIL )
IF %DEBUG_BUILD% NEQ 0  (ECHO MSBuild of ShapeLib 1.3.0 Debug project - FAIL )

cd /D %~dp0
call DisplayDate.cmd
IF %BUILD_ERROR% NEQ 0 (ECHO STATUS: FAIL & ENDLOCAL & EXIT /B 1)
ENDLOCAL
ECHO STATUS: SUCCESS



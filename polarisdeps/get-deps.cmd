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

cd %~dp0
call %~dp0build-boost-1.60.0.cmd %BASEDIR%
cd %~dp0
call %~dp0build-odb-2.4.0.cmd %BASEDIR%
cd %~dp0
call %~dp0build-gtest-1.7.0.cmd %BASEDIR%

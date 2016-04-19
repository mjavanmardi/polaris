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
set BOOSTZIPFILE=boost_1_60_0.zip
set BOOSTDIR=%BASEDIR%\boost_1_60_0

echo file=%BOOSTZIPFILE%
echo dir=%BOOSTDIR%

myWget.py -u "http://sourceforge.net/projects/boost/files/boost/1.60.0/boost_1_60_0.zip" -n %BOOSTZIPFILE% -e %BOOSTDIR% -o %BASEDIR%


:: if you want to use boost libraries (as opposed to just headers)
:: uncomment the commands here:

::cd %BOOSTDIR%
::call bootstrap.bat
::b2 address-model=64 link=shared,static variant=release install --prefix=%BOOSTDIR%
::b2 address-model=64 link=shared,static variant=debug install --prefix=%BOOSTDIR%
::cd %BOOSTDIR%
::ren lib lib64-msvc-14.0

cd %~dp0

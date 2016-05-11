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
set ODBZIPFILE=odb-2.4.0-i686-windows.zip
set LIBODBZIPFILE=libodb-2.4.0.zip
set ODBSQLITEBZIPFILE=libodb-sqlite-2.4.0.zip
set SQLITEZIPFILE=sqlite-amalgamation-3110100.zip

set ODBDIR=%BASEDIR%\odb-2.4.0-i686-windows
set LIBODBDIR=%BASEDIR%\libodb-2.4.0
set LIBSQLITEDIR=%LIBODBDIR%\libodb-sqlite-2.4.0
set SQLITE3DIR=%LIBODBDIR%\sqlite-amalgamation-3110100

echo ODBZIPFILE=        %ODBZIPFILE%
echo LIBODBZIPFILE=     %LIBODBZIPFILE%
echo ODBSQLITEBZIPFILE= %ODBSQLITEBZIPFILE%
echo SQLITEZIPFILE=     %SQLITEZIPFILE%
echo ODBDIR=            %ODBDIR%
echo LIBODBDIR=         %LIBODBDIR%
echo LIBSQLITEDIR=      %LIBSQLITEDIR%
echo SQLITE3DIR=        %SQLITE3DIR%
echo filedir=           %~dp0

:: GOTO END
myWget.py -u "http://www.codesynthesis.com/download/odb/2.4/odb-2.4.0-i686-windows.zip" -n %ODBZIPFILE% -e %ODBDIR% -o %BASEDIR%
myWget.py -u "http://www.codesynthesis.com/download/odb/2.4/libodb-2.4.0.zip" -n %LIBODBZIPFILE% -e %LIBODBDIR% -o %BASEDIR%
myWget.py -u "http://www.codesynthesis.com/download/odb/2.4/libodb-sqlite-2.4.0.zip" -n %ODBSQLITEBZIPFILE% -e %LIBSQLITEDIR% -o %LIBODBDIR%
myWget.py -u "https://www.sqlite.org/2016/sqlite-amalgamation-3110100.zip" -n %SQLITEZIPFILE% -e %SQLITE3DIR% -o %LIBODBDIR%

mkdir %LIBODBDIR%
echo copy %~dp0libodb-vc14.sln %LIBODBDIR%
copy %~dp0libodb-vc14.sln %LIBODBDIR%
copy %~dp0libodb-vc14.vcxproj %LIBODBDIR%\odb
cd /D %LIBODBDIR%
msbuild libodb-vc14.sln /p:Platform=x64 /p:Configuration=Release
msbuild libodb-vc14.sln /p:Platform=x64 /p:Configuration=Debug

mkdir %SQLITE3DIR%
cd /D %~dp0
copy build_sqlite3_msvc.cmd %SQLITE3DIR%
cd /D %SQLITE3DIR%
call build_sqlite3_msvc.cmd

mkdir %LIBSQLITEDIR%
copy %~dp0libodb-sqlite-vc14.sln %LIBSQLITEDIR%
copy %~dp0libodb-sqlite-vc14.vcxproj %LIBSQLITEDIR%\odb\sqlite
cd /D %LIBSQLITEDIR%
msbuild libodb-sqlite-vc14.sln /p:Platform=x64 /p:Configuration=Release
msbuild libodb-sqlite-vc14.sln /p:Platform=x64 /p:Configuration=Debug

:END
cd /D %~dp0
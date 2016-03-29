@echo off
set POLARIS_DEPS_DIR=%~dp0
::Does string have a trailing slash? if so remove it 
IF %POLARIS_DEPS_DIR:~-1%==\ SET POLARIS_DEPS_DIR=%POLARIS_DEPS_DIR:~0,-1%
set POLARIS_DEPS_DIR=%POLARIS_DEPS_DIR:\=/%
set PATH=%POLARIS_DEPS_DIR%/gperftools-2.1/Release;%PATH%
set PATH=%POLARIS_DEPS_DIR%/libodb-2.2.2/libodb-sqlite-2.2.0/etc/sqlite/bin64;%PATH%
set PATH=%POLARIS_DEPS_DIR%/libodb-2.2.2/libodb-sqlite-2.2.0/bin64;%PATH%
set PATH=%POLARIS_DEPS_DIR%/libodb-2.2.2/bin64;%PATH%

set PATH=%POLARIS_DEPS_DIR%/libodb-2.4.0/libodb-sqlite-2.4.0/etc/sqlite/bin64;%PATH%
set PATH=%POLARIS_DEPS_DIR%/libodb-2.4.0/libodb-sqlite-2.4.0/bin64;%PATH%
set PATH=%POLARIS_DEPS_DIR%/libodb-2.4.0/bin64;%PATH%

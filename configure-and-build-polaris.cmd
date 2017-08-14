@ECHO OFF

SET ERRORLEVEL=
call configure-polaris.cmd %1
IF %ERRORLEVEL% NEQ 0 (EXIT /B 1)
call build-polaris.cmd
IF %ERRORLEVEL% NEQ 0 (EXIT /B 1)

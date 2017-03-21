echo -DUnittests=1 > build.cfg
::echo -DCOMMANDLINE_ARGS=1 >> build.cfg
echo -DIntegratedModel=1 >> build.cfg
SET ERRORLEVEL=
call configure-and-build-polaris.cmd D:\opt\polarisdeps
IF %ERRORLEVEL% NEQ 0 (EXIT /B 1)
call run_tests.cmd
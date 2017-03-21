pushd %~dp0

call extract_trajectories ..\Input\2010_base detroit 1.0
call extract_trajectories ..\Input\2040_semcog detroit2040 1.0
call extract_trajectories ..\Input\2040_dfc detroit_dfc 1.0

popd
pause

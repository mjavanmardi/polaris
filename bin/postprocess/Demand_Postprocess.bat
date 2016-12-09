REM *****************************************
REM Param 1 is the output folder from the run
REM *****************************************

set DEMAND=%1
set SUPPLY=%2
set REPO=c:\Users\jauld\Desktop\Research\Projects\POLARIS\polaris_T1

pushd %~dp0

@echo Attach '%SUPPLY%' as a; > demand_final.sql
type %REPO%\bin\postprocess\demand_final.sql >> demand_final.sql

REM xcopy %DEMAND% %backup\demand_database% /y

c:\sqlite\spatialite %DEMAND% < demand_final.sql

pause
popd
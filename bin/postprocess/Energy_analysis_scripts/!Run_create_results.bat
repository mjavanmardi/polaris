pushd %~dp0
create_results_db.cmd ..\Results\test\result_test2.sqlite ..\Input\detroit-Supply.sqlite ..\Results\Results_2040_vehicles\Results_2010_base.csv ..\Results\Results_2040_vehicles\Results_2040_semcog_vehicles_2040.csv ..\Results\Results_2040_vehicles\Results_2040_dfc_vehicles_2040.csv
pause
popd
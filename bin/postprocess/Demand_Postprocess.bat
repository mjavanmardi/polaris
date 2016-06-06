
set demand_database=C:\Users\jauld\Desktop\Research\Projects\POLARIS\POLARIS_STUDIES\AnnArbor_CAV_studies\annarbor_cav_study\annarbor_cav_study_110\annarbor-Demand.sqlite
set supply_database=C:\Users\jauld\Desktop\Research\Projects\POLARIS\POLARIS_STUDIES\AnnArbor_CAV_studies\annarbor_cav_study\annarbor-Supply.sqlite

@echo Attach '%supply_database%' as supply; > demand_final.sql
type C:\Users\jauld\Desktop\Research\Projects\POLARIS\polaris_Develop\bin\postprocess\demand.sql >> demand_final.sql

pause


xcopy %demand_database% %backup\demand_database% /y

c:\sqlite\sqlite3 %demand_database% < demand_final.sql

pause

set demand_database=C:\Users\jauld\Desktop\POLARIS_DATA\AnnArbor\Ann_Arbor_test\annarbor-Demand.sqlite
set supply_database=C:\Users\jauld\Desktop\POLARIS_DATA\AnnArbor\annarbor-Supply.sqlite

@echo Attach '%supply_database%' as supply; > demand_final.sql
copy demand_final.sql + demand.sql demand_final.sql


xcopy %demand_database% %backup\demand_database% /y

c:\sqlite\sqlite3 %demand_database% < demand_final.sql
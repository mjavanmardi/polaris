REM ===================================================
REM %1 parameter is the new database filepath
REM %2 parameter is the supply database filepath 
REM %3 parameter is the base input csv file
REM %4 parameter is the forecast year 1 input csv file
REM %5 parameter is the forecast year 2 input csv file
REM ===================================================

set db_path="%~f2"
set db_path=%db_path:\=\\%
set base_file="%~f3"
set base_file=%base_file:\=\\%
set fcast1_file="%~f4"
set fcast1_file=%fcast1_file:\=\\%
set fcast2_file="%~f5"
set fcast2_file=%fcast2_file:\=\\%

set base_table=%~n3_trajectory
set fcast1_table=%~n4_trajectory
set fcast2_table=%~n5_trajectory

set base_table_result=%base_table:~0,-11%_result
set fcast1_result=%fcast1_table:~0,-11%_result
set fcast2_result=%fcast2_table:~0,-11%_result

@echo off
@echo Attach %db_path% as a; > tmp.sql
@echo create table links as select link, type, length from a.link; >> tmp.sql
@echo create UNIQUE INDEX link_idx on links(link); >> tmp.sql
@echo .mode csv	>> tmp.sql
@echo .import %base_file% %base_table% >> tmp.sql
@echo .import %fcast1_file% %fcast1_table% >> tmp.sql
@echo .import %fcast2_file% %fcast2_table% >> tmp.sql
@echo create table %base_table_result% as select cast("Link Number" as integer) as link, sum(cast("Fuel Consumption [kg]" as real)) as fuel_use from %base_table% group by "Link Number"; >> tmp.sql
@echo create table %fcast1_result% as select cast("Link Number" as integer) as link, sum(cast("Fuel Consumption [kg]" as real)) as fuel_use from %fcast1_table% group by "Link Number"; >> tmp.sql
@echo create table %fcast2_result% as select cast("Link Number" as integer) as link, sum(cast("Fuel Consumption [kg]" as real)) as fuel_use from %fcast2_table% group by "Link Number"; >> tmp.sql
@echo CREATE UNIQUE INDEX link_idx1 ON %base_table_result% (link); >> tmp.sql
@echo CREATE UNIQUE INDEX link_idx2 ON %fcast1_result% (link); >> tmp.sql
@echo CREATE UNIQUE INDEX link_idx3 ON %fcast2_result% (link); >> tmp.sql
@echo create table Results as SELECT links.link, links.length, b.fuel_use / links.length*1000.0 as fuel_use_perkm_base, c.fuel_use/ links.length*1000.0 as fuel_use_perkm_fcast1, d.fuel_use/ links.length*1000.0 as fuel_use_perkm_fcast2, (c.fuel_use - b.fuel_use)/ links.length*1000.0 as diff_fcast1_base, (d.fuel_use - b.fuel_use)/ links.length*1000.0 as diff_fcast2_base, (d.fuel_use - c.fuel_use)/ links.length*1000.0 as diff_fcast2_fcast1  >> tmp.sql
@echo FROM links >> tmp.sql
@echo LEFT JOIN %base_table_result% b ON b.link = links.link >> tmp.sql
@echo LEFT JOIN %fcast1_result% c ON c.link = links.link >> tmp.sql
@echo LEFT JOIN %fcast2_result% d ON d.link = links.link; >> tmp.sql
@echo CREATE UNIQUE INDEX link_idx4 ON "Results" (link); >> tmp.sql

sqlite %1 < tmp.sql

@echo Attach %db_path% as a; > tmp2.sql
type create_geo.sql >> tmp2.sql
@echo. >> tmp2.sql
@echo update result_points set diff_fcast1_base = diff_fcast1_base * length / 1000.0 / numgeometries(geo) >> tmp2.sql
@echo update result_points set diff_fcast2_base = diff_fcast2_base * length / 1000.0 / numgeometries(geo) >> tmp2.sql
@echo update result_points set diff_fcast2_fcast1 = diff_fcast2_fcast1 * length / 1000.0 / numgeometries(geo) >> tmp2.sql

spatialite %1 < tmp2.sql




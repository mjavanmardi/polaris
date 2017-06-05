set DIR=%1
set DB_NAME=%2
set SAMPLE=%3
set sqlite3_path=c:\sqlite\


REM================================
REM DO NOT MODIFY BELOW HERE
REM================================

pushd %~dp0
cd %DIR%

REM @echo off
@echo ATTACH DATABASE ^"%DB_NAME%-Supply.sqlite^" as supply;> export.sql
@echo .headers on >> export.sql
@echo .mode csv >> export.sql
IF %SAMPLE% LSS 1 (
	@echo .output "trajectory_%SAMPLE%.csv" >> export.sql
) ELSE (
	@echo .output "trajectory.csv" >> export.sql
)
@echo select path.id as trip, >> export.sql
@echo 	 path.vehicle, >> export.sql
@echo 	 vehicle.type as veh_type, >> export.sql
@echo 	 path_links.[index] as link_number, >> export.sql
@echo 	 path_links.value_link as link_id, >> export.sql
@echo 	 path_links.value_dir as link_dir, >> export.sql
@echo 	 path_links.value_entering_time as entering_time, >> export.sql
@echo 	 path_links.value_travel_time as travel_time, >> export.sql
@echo 	 round(path_links.value_exit_position - supply.link.length,2) as start_position, >> export.sql
@echo 	 round(supply.link.length,2) as length, >> export.sql
@echo 	 supply.link.length / (path_links.value_travel_time - path_links.value_delayed_time) as actual_speed, >> export.sql
@echo 	 case when path_links.value_dir = 0 then supply.link.fspd_ab else supply.link.fspd_ba end as free_flow_speed, >> export.sql
@echo 	 path_links.value_delayed_time as stopped_time, >> export.sql
@echo 	 path_links.value_exit_position as stop_position >> export.sql
@echo from path, path_links, supply.link, vehicle >> export.sql

IF %SAMPLE% LSS 1 (
	@echo where path.id %% ^(1/%SAMPLE%^) = 0 and path.id = path_links.object_id and path_links.value_link = supply.link.link and path_links.[index]+1 ^< path.num_links and vehicle.vehicle_id = path.vehicle and path_links.value_travel_time ^>0; >> export.sql
) ELSE (
	@echo where path.id = path_links.object_id and path_links.value_link = supply.link.link and path_links.[index]+1 ^< path.num_links and vehicle.vehicle_id = path.vehicle and path_links.value_travel_time ^>0; >> export.sql
)

@echo .exit >> export.sql

%sqlite3_path%sqlite3 %DB_NAME%-Demand.sqlite < export.sql
popd
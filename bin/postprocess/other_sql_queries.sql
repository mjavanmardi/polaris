--######################
-- FIX BROKEN DATABASE
sqlite> .mode insert
sqlite> .output mydb_export.sql
sqlite> .dump
sqlite> .exit
mv mydb.db mydb.db.original
sqlite3 mydb.db < mydb_export.sql

--############################################################
-- CREATE LOCATIONS TABLE - and add geocolumn
CREATE TABLE "Location" (
  "location" INTEGER NOT NULL PRIMARY KEY,
  "link" INTEGER,
  "dir" INTEGER DEFAULT 0 NOT NULL,
  "offset" REAL DEFAULT 0,
  "setback" REAL DEFAULT 0,
  "zone" INTEGER,
  "location_data" INTEGER,
  "x" DOUBLE,
  "y" DOUBLE, "Geo" POINT,
  CONSTRAINT "link_fk"
    FOREIGN KEY ("link")
    REFERENCES "Link" ("link")
    DEFERRABLE INITIALLY DEFERRED,
  CONSTRAINT "zone_fk"
    FOREIGN KEY ("zone")
    REFERENCES "Zone" ("zone")
    DEFERRABLE INITIALLY DEFERRED,
  CONSTRAINT "location_data_fk"
    FOREIGN KEY ("location_data")
    REFERENCES "LocationData" ("location")
    DEFERRABLE INITIALLY DEFERRED)
	
Select AddGeometryColumn ('Location', 'Geo', 26916, 'POINT', 2)
UPDATE Location SET Geometry=MakePoint(x, y, 26916)

--############################################################
-- Vehicle types in model
create view Vehicle_Types_view as select vehicle_type.type_id, vehicle_class.class_type, powertrain_type.type, fuel_type.type from vehicle_type, vehicle_class,powertrain_type, fuel_type 
where vehicle_type.type_id in (select distinct(type) from vehicle order by type) and
vehicle_type.vehicle_class = vehicle_class.class_id and
vehicle_type.powertrain_type = powertrain_type.type_id and
vehicle_type.fuel_type = fuel_type.type_id

--############################################################
-- Create vehicle trajectory for autonomoie
select path.id as trip,
	 path.vehicle,
	 vehicle.type as veh_type,
	 path_links.[index] as link_number,
	 path_links.value_link as link_id,
	 path_links.value_dir as link_dir,
	 path_links.value_entering_time as entering_time,
	 path_links.value_travel_time as travel_time,
	 round(path_links.value_exit_position * 0.3048 - link.length,2) as start_position,
	 round(link.length,2) as length,
	 link.length / (path_links.value_travel_time - path_links.value_delayed_time) as actual_speed,
	 case when path_links.value_dir = 0 then link.fspd_ab else link.fspd_ba end as free_flow_speed,
	 path_links.value_delayed_time as stopped_time,
	 path_links.value_exit_position as stop_position
from path, path_links, link, vehicle
where path.id = path_links.object_id and path_links.value_link = link.link and path_links.[index]+1 < path.num_links and vehicle.vehicle_id = path.vehicle



CREATE TABLE "LocationData" (
  "location" INTEGER NOT NULL PRIMARY KEY,
  "truck_org" INTEGER DEFAULT 0 NOT NULL,
  "truck_des" INTEGER DEFAULT 0 NOT NULL,
  "auto_org" INTEGER DEFAULT 0 NOT NULL,
  "auto_des" INTEGER DEFAULT 0 NOT NULL,
  "transit" INTEGER DEFAULT 0 NOT NULL,
  "areatype" INTEGER DEFAULT 0 NOT NULL,
  "LU_AREA" REAL DEFAULT 0 NOT NULL,
  "notes" TEXT DEFAULT '' NOT NULL,
  "census_zone" REAL DEFAULT 0,
  "x" REAL DEFAULT 0,
  "y" REAL DEFAULT 0,
  "land_use" TEXT DEFAULT '' NOT NULL)
  
--############################################################
-- IDENTIFY TRANSIT STOPS
create TABLE Transit_Stops as
select location, link, dir, zone from location where link in ( 

select link from link where type is not 'HEAVYRAIL' and type is not 'WALKWAY' and (node_a in (

select node_a from link where type = 'WALKWAY' union select node_b from link where type = 'WALKWAY'
) or node_b in (

select node_a from link where type = 'WALKWAY' union select node_b from link where type = 'WALKWAY'
) )
)
update Locationdata set land_use = 'TRANSIT_STOP'
	where Locationdata.location in (select location from transit_stops where transit_stops.location = locationdata.location)
	
--############################################################
-- Manual databases corrects: Correct for zones with no activity locations - shift nearest to zone
 update location set zone = 1405 where location = 140771
 update location set zone = 1405 where location = 64958
 update location set zone = 97 where location = 20558
 update location set zone = 97 where location = 96800
 update location set zone = 1071 where location = 59971
 update location set zone = 1071 where location = 135913
 
 
 --#########################################################################################
-- SELECT EXTERNAL-INTERNAL and INTERNAL-EXTERNAL trips from non CBD residents

-- attach the cbd database and extract the internal locations
ATTACH DATABASE 'c:\users\jauld\desktop\chicago-Supply.sqlite' as a;
create TEMP table location_cbd as select location from a.location;

-- get all trips from full model which either originate OR terminate in the CBD
create TEMP table  external_trips_all AS
select * from trip where (
destination in location_cbd
and origin not in location_cbd )
or (
origin in location_cbd
and destination not in location_cbd )
;

-- get list of non-CBD residents
create TEMP table  external_household AS
select hhold from household where location not in location_cbd;

-- select all E-I and I-E trips from non-CBD residents
create table External_Trip
select * from external_trips_all where external_trips_all.hhold in external_household;


 --#########################################################################################
-- UPDATE ORIGIN/DESTINATION activity locations for external trips for cut-down model
create table trip_temp1 as
select trip.*, a.location.zone as orig_zone from trip, a.location where origin = a.location.location;

create table trip_temp2 as
select trip_temp1.*, a.location.zone as dest_zone from trip_temp1, a.location where destination = a.location.location;


UPDATE trip_temp2
SET
      origin = (SELECT cbd_entry_points.activity_location 
                            FROM cbd_entry_points
                            WHERE cbd_entry_points.entry_zone = orig_zone );

WHERE
    orig_zone in (select entry_zone from cbd_entry_points)
	
UPDATE trip_temp2
SET
      destination  = (SELECT cbd_entry_points.activity_location 
                            FROM cbd_entry_points
                            WHERE cbd_entry_points.entry_zone = dest_zone )

WHERE
    dest_zone in (select entry_zone from cbd_entry_points);
	
--#########################################################################################
-- EXPRESSWAY LINK DELAY FOR RESULTS DATABASE	
create table Expressway_Link_Delay as
select 
	start_time, link_travel_delay * link_out_volume as delay_minutes
from 
	linkmoe
where 
	link_type = 0 or link_type = 3
Group by
	start_time
	
--#########################################################################################
-- CREATE HOUSEHOLD ACTIVITY TABLE	
SELECT HH, PER, AGE, activity.type as ACTIVITY, cast(cast(activity.start_time/86400*24 as int)as string) ||":"|| cast( cast((activity.start_time/86400*24 - cast(activity.start_time/86400*24 as int))*60 as int) as string) as START, activity.mode as MODE from activity, (select person.person as PER, household.household as HH, person.Age as AGE from person, household where person.household = household.household) where activity.person = PER order by HH, PER

DROP TABLE IF EXISTS LinkMOE_2;
CREATE TABLE IF NOT EXISTS LinkMOE_2 as
SELECT cast(link_uid/2 as int) as LINK, link_uid - 2* cast(link_uid/2 as int) as DIR, cast(start_time/3600 as int) as HR, link_speed as speed, link_travel_time as ttime, link_out_volume as volume
FROM LinkMOE;

DROP TABLE IF EXISTS LinkMOE_agg;
CREATE TABLE IF NOT EXISTS LinkMOE_agg as
SELECT LINK, DIR, HR, avg(speed) as speed, avg(ttime) as ttime, sum(volume) as volume
FROM LinkMOE_2 GROUP BY link,dir,hr;


--#########################################################################################
-- GET ALLOCATION OF LAND USE BY CENSUS TRACT
select census_zone, zone, count(locatioN) from location, zone where location.zone = zone.zone and (land_use = "RESIDENTIAL-SINGLE" or land_use = "RESIDENTIAL-MULTI" or land_use = "ALL" or land_use = "MIXED_USE") group by census_zone, zone 



DROP TABLE IF EXISTS Trip_Zone_tmp;
CREATE TABLE Trip_Zone_tmp As
SELECT Trip.*, a.Location.zone as origzone
FROM Trip
INNER JOIN a.Location
ON trip.origin=a.Location.location;

DROP TABLE IF EXISTS Trip_With_Zone;
CREATE TABLE Trip_With_Zone As
SELECT Trip_Zone_tmp.*, a.Location.zone as destzone
FROM Trip_Zone_tmp
INNER JOIN a.Location
ON Trip_Zone_tmp.destination=a.Location.location;

DROP TABLE IF EXISTS Trip_Zone_tmp;

DROP TABLE IF EXISTS Productions_tmp;
CREATE TABLE Productions_tmp As
SELECT 
  origzone, cast(start/3600 as int) as hr
FROM Trip_With_Zone
WHERE
start > 62 and mode = 0;

DROP TABLE IF EXISTS Productions;
CREATE TABLE Productions As
SELECT 
  origzone, hr, count(1) as Prod
FROM Productions_tmp
GROUP BY
  origzone,hr;
  
 DROP TABLE IF EXISTS Attractions_tmp;
CREATE TABLE Attractions_tmp As
SELECT 
  destzone, cast(start/3600 as int) as hr
FROM Trip_With_Zone
WHERE
start > 62 and mode = 0;

DROP TABLE IF EXISTS Attractions;
CREATE TABLE Attractions As
SELECT 
  destzone, hr, count(1) as Attr
FROM Attractions_tmp
GROUP BY
  destzone,hr;
  
  SELECT * FROM activity_with_zone where origzone=destzone and origzone < 78 and start_time > 62;

  
 
 
CREATE TABLE Trip_Zone_tmp As
SELECT Trip.*, a.Location.x as orig_x, a.Location.y as orig_y
FROM Trip
INNER JOIN a.Location
ON trip.origin=a.Location.location;
  
CREATE TABLE Trip_with_loc As
SELECT Trip_zone_tmp.*, a.Location.x as dest_x, a.Location.y as dest_y
FROM Trip_zone_tmp
INNER JOIN a.Location
ON trip_zone_tmp.destination=a.Location.location;
  
create table trip_with_dist as select trip_with_loc.*,sqrt(pow(orig_x-dest_x,2)+pow(orig_y-dest_y,2))/1000 as Dist, mode from trip_with_loc where start> 62



select census_zone, location.zone, sum(case when land_use = "RESIDENTIAL-MULTI" then 4 when land_use = "RESIDENTIAL-SINGLE" then 1 when land_use = "ALL" then 1 when land_use = "MIXED_USE" then 2 else 0.1 end ) as wgtd_locations, zone.pop_persons, zone.pop_households, zone.employment_total
from location, zone
where location.zone = zone.zone
group by census_zone, location.zone 
order by location.zone, census_zone


#-- fuel consumption analysis
create table Results as SELECT link.link, link.geo, b.fuel_cons, c.fuel_cons_semcog, d.fuel_cons_dfc, c.fuel_cons_semcog - b.fuel_cons as diff_semcog_base, d.fuel_cons_dfc - b.fuel_cons as diff_dfc_base, d.fuel_cons_dfc - c.fuel_cons_semcog as diff_dfc_semcog
FROM link
LEFT JOIN '2010_link_results' b ON b.link_number = link.link
LEFT JOIN '2040Semcog_link_results' c ON c.link_number = link.link
LEFT JOIN '2040DFC_link_results' d ON d.link_number = link.link


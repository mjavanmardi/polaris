--############################################################
-- Acts COUNTS BY LOCATION FOR SPECIFIED TIME
ATTACH DATABASE '..\chicago-Supply.sqlite' AS db_supply;
SELECT * FROM db_supply.Zone;


--############################################################
-- CREATE EVACUATION CHARACTERISTICS TABLE
DROP TABLE IF EXISTS Evacuation_Parameters;
CREATE TABLE Evacuation_Parameters ( Start_Time, Severity);
INSERT INTO 
	Evacuation_Parameters
VALUES
         (36000, 1);
		 
--############################################################
-- Append vehicle and O/D and zone information to the activities for later queries
DROP TABLE IF EXISTS Acts_Tmp;
CREATE TABLE Acts_Tmp as
SELECT
	Activity.*, Household.vehicles
FROM 
	Activity, Person, Household
WHERE
	Activity.person = Person.person and Person.household = Household.household;

DROP TABLE IF EXISTS Activity_TTime_tmp;
CREATE TABLE Activity_TTime_tmp As
SELECT Acts_Tmp.*, Trip.origin as origin, Trip.destination as destination
FROM Trip
INNER JOIN Acts_Tmp
ON Acts_Tmp.trip=Trip.trip_id;

DROP TABLE IF EXISTS Activity_With_Zone_tmp;
CREATE TABLE Activity_With_Zone_tmp As
SELECT Activity_TTime_tmp.*, supply.Location.zone as origzone
FROM Activity_TTime_tmp
INNER JOIN supply.Location
ON Activity_TTime_tmp.origin=supply.Location.location;

DROP TABLE IF EXISTS Acts;
CREATE TABLE Acts As
SELECT Activity_With_Zone_tmp.*, supply.Location.zone as destzone
FROM Activity_With_Zone_tmp
INNER JOIN supply.Location
ON Activity_With_Zone_tmp.destination=supply.Location.location;

DROP TABLE IF EXISTS Acts_Tmp;
DROP TABLE IF EXISTS Activity_TTime_tmp;
DROP TABLE IF EXISTS Activity_With_Zone_tmp;

DROP TABLE IF EXISTS Trips_Tmp;
CREATE TABLE Trips_Tmp as
SELECT
	Trip.*, supply.Location.zone as destzone
FROM 
	Trip, supply.Location
WHERE
	Trip.destination = supply.Location.location;
	
DROP TABLE IF EXISTS Trip;
CREATE TABLE Trip As
Select * from Trips_Tmp;
DROP TABLE IF EXISTS Trips_Tmp;
	
--############################################################
-- CREATE LIST OF LOCATION / MODE FOR EACH PERSON AT EVACUATION TIME - FOR PEOPLE ENROUTE, USE THE DESTINATION LOCATION
DROP TABLE IF EXISTS Person_Locations;
CREATE TABLE Person_Locations ( Location, Mode, Type);
INSERT INTO 
	Person_Locations
SELECT
	location_id, mode, Acts.Type
FROM 
	Acts, Evacuation_Parameters
WHERE
	Acts.start_time < Evacuation_Parameters.Start_Time AND Acts.start_time + Acts.duration >= Evacuation_Parameters.Start_Time AND Acts.type <> "HOME" AND Acts.type <> "SCHOOL" AND destzone in Evacuation_Zones;
	
INSERT INTO 
	Person_Locations
SELECT
	location_id, "TRANSIT","SCHOOL: TRANSIT EVAC"
FROM 
	Acts, Evacuation_Parameters
WHERE
	Acts.start_time < Evacuation_Parameters.Start_Time AND Acts.start_time + Acts.duration >= Evacuation_Parameters.Start_Time AND Acts.type = "SCHOOL" AND destzone in Evacuation_Zones;
	
INSERT INTO 
	Person_Locations
SELECT
	location_id, "TRANSIT","HOME: TRANSIT EVAC"
FROM 
	Acts, Evacuation_Parameters
WHERE
	Acts.start_time < Evacuation_Parameters.Start_Time AND Acts.start_time + Acts.duration >= Evacuation_Parameters.Start_Time AND Acts.type = "HOME" AND Acts.vehicles = 0 AND destzone in Evacuation_Zones;
	
INSERT INTO 
	Person_Locations
SELECT
	location_id, mode,"HOME"
FROM 
	Acts, Evacuation_Parameters
WHERE
	Acts.start_time < Evacuation_Parameters.Start_Time AND Acts.start_time + Acts.duration >= Evacuation_Parameters.Start_Time AND Acts.type = "HOME" AND Acts.vehicles > 0 AND destzone in Evacuation_Zones;
	

--############################################################
-- CREATE LIST OF LOCATION / MODE FOR EACH PERSON AT EVACUATION TIME - FOR PEOPLE ENROUTE, USE THE DESTINATION LOCATION

--# Treat auto and HOV trip enroute as auto evacuees
INSERT INTO 
	Person_Locations
SELECT
	destination, "AUTO","ENROUTE - AUTO OR HOV"
FROM 
	Trip, Evacuation_Parameters
WHERE
	Trip.start < Evacuation_Parameters.Start_Time AND Trip.end >= Evacuation_Parameters.Start_Time AND (Trip.mode = 0 or Trip.mode = 1) AND destzone in Evacuation_Zones;
	
INSERT INTO 
	Person_Locations
SELECT
	destination, "TRANSIT","ENROUTE - TRANSIT"
FROM 
	Trip, Evacuation_Parameters
WHERE
	Trip.start < Evacuation_Parameters.Start_Time AND Trip.end >= Evacuation_Parameters.Start_Time AND Trip.mode = 2  AND destzone in Evacuation_Zones;

	
--############################################################
-- EVAC-DEMAND:  CROSS-TAB LOCATION BY MODE AND GET COUNTS
DROP TABLE IF EXISTS Evacuation_Demand;
CREATE TABLE IF NOT EXISTS Evacuation_Demand As
SELECT 
	Location,
	sum(CASE WHEN mode= 'AUTO' THEN 1 END) as Auto,
	sum(CASE WHEN mode= 'HOV' or mode= 'TRANSIT' THEN 1 END) as Transit,
	sum(1) AS total
FROM 
	Person_Locations
GROUP BY 
	Location;

DROP TABLE IF EXISTS Evacuee_Types;
CREATE TABLE IF NOT EXISTS Evacuee_Types As	
SELECT 
	type,
	sum(CASE WHEN mode= 'AUTO' THEN 1 END) as Auto,
	sum(CASE WHEN mode= 'HOV' or mode= 'TRANSIT' THEN 1 END) as Transit,
	sum(1) AS total
FROM 
	Person_Locations
GROUP BY 
	Type;
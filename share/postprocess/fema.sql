--############################################################
-- ACTIVITY COUNTS BY LOCATION FOR SPECIFIED TIME
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
-- CREATE LIST OF LOCATION / MODE FOR EACH PERSON AT EVACUATION TIME - FOR PEOPLE ENROUTE, USE THE DESTINATION LOCATION
DROP TABLE IF EXISTS Person_Locations;
CREATE TABLE Person_Locations ( Location, Mode);
INSERT INTO 
	Person_Locations
SELECT
	location_id, mode
FROM 
	Activity, Evacuation_Parameters
WHERE
	Activity.start_time < Evacuation_Parameters.Start_Time AND Activity.start_time + Activity.duration >= Evacuation_Parameters.Start_Time;

INSERT INTO 
	Person_Locations
SELECT
	destination, "AUTO"
FROM 
	Trip, Evacuation_Parameters
WHERE
	Trip.start < Evacuation_Parameters.Start_Time AND Trip.end >= Evacuation_Parameters.Start_Time;

	
--############################################################
-- EVAC-DEMAND:  CROSS-TAB LOCATION BY MODE AND GET COUNTS
DROP TABLE IF EXISTS Evacuation_Demand;
CREATE TABLE IF NOT EXISTS Evacuation_Demand As
SELECT 
	Location,
	sum(CASE WHEN mode= 'AUTO' THEN Location/Location END) as Auto,
	sum(CASE WHEN mode= 'TRANSIT' THEN Location/Location END) as Transit,
	sum(Location/Location) AS total
FROM 
	Person_Locations
GROUP BY 
	Location;
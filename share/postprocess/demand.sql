--############################################################
-- GENERATE TRAVEL TIME DISTRIBUTION

DROP TABLE IF EXISTS TTime_Distribution;
CREATE TABLE IF NOT EXISTS TTime_Distribution As
SELECT 
	round((end-start)/60) as TTime_minutes,
	sum(CASE WHEN cast(start/3600 as int ) = 0 THEN 1 END) as '12AM',
	sum(CASE WHEN cast(start/3600 as int ) = 1 THEN 1 END) as '1AM',
	sum(CASE WHEN cast(start/3600 as int ) = 2 THEN 1 END) as '2AM',
	sum(CASE WHEN cast(start/3600 as int ) = 3 THEN 1 END) as '3AM',
	sum(CASE WHEN cast(start/3600 as int ) = 4 THEN 1 END) as '4AM',
	sum(CASE WHEN cast(start/3600 as int ) = 5 THEN 1 END) as '5AM',
	sum(CASE WHEN cast(start/3600 as int ) = 6 THEN 1 END) as '6AM',
	sum(CASE WHEN cast(start/3600 as int ) = 7 THEN 1 END) as '7AM',
	sum(CASE WHEN cast(start/3600 as int ) = 8 THEN 1 END) as '8AM',
	sum(CASE WHEN cast(start/3600 as int ) = 9 THEN 1 END) as '9AM',
	sum(CASE WHEN cast(start/3600 as int ) = 10 THEN 1 END) as '10AM',
	sum(CASE WHEN cast(start/3600 as int ) = 11 THEN 1 END) as '11AM',
	sum(CASE WHEN cast(start/3600 as int ) = 12 THEN 1 END) as '12PM',
	sum(CASE WHEN cast(start/3600 as int ) = 13 THEN 1 END) as '1PM',
	sum(CASE WHEN cast(start/3600 as int ) = 14 THEN 1 END) as '2PM',
	sum(CASE WHEN cast(start/3600 as int ) = 15 THEN 1 END) as '3PM',
	sum(CASE WHEN cast(start/3600 as int ) = 16 THEN 1 END) as '4PM',
	sum(CASE WHEN cast(start/3600 as int ) = 17 THEN 1 END) as '5PM',
	sum(CASE WHEN cast(start/3600 as int ) = 18 THEN 1 END) as '6PM',
	sum(CASE WHEN cast(start/3600 as int ) = 19 THEN 1 END) as '7PM',
	sum(CASE WHEN cast(start/3600 as int ) = 20 THEN 1 END) as '8PM',
	sum(CASE WHEN cast(start/3600 as int ) = 21 THEN 1 END) as '9PM',
	sum(CASE WHEN cast(start/3600 as int ) = 22 THEN 1 END) as '10PM',
	sum(CASE WHEN cast(start/3600 as int ) = 23 THEN 1 END) as '11PM',
	sum(1) AS total
FROM 
	Trip
WHERE
	Start > 62 and mode = 0
GROUP BY 
	TTime_minutes;

DROP TABLE IF EXISTS TTime_Distribution;
CREATE TABLE TTime_Distribution ( TTime_minutes INT, Count INT);
INSERT INTO 
	TTime_Distribution(TTime_minutes, Count)
SELECT
	[round((end-start)/60)], total
FROM 
	TTime_Distribution_tmp;
DROP TABLE TTime_Distribution_tmp;


--############################################################
-- GENERATE ACTIVITY TYPE BY MODAL DISTRIBUTION

DROP TABLE IF EXISTS Mode_Distribution;
CREATE TABLE IF NOT EXISTS Mode_Distribution As
SELECT 
	type,
	sum(CASE WHEN mode= 'AUTO' THEN 1 END) as Auto,
	sum(CASE WHEN mode= 'HOV' THEN 1 END) as HOV,
	sum(CASE WHEN mode= 'TRANSIT' THEN 1 END) as Transit,
	sum(1) AS total
FROM 
	Activity
WHERE
	Start_Time > 62
GROUP BY 
	type;
	

--############################################################
-- GENERATE ACTIVITY START TIME DISTRIBUTION
DROP TABLE IF EXISTS Activity_Start_Distribution;
CREATE TABLE IF NOT EXISTS Activity_Start_Distribution As
SELECT 
	cast(start_time/60 as int),
        sum(CASE WHEN type= 'EAT OUT' THEN 1 END) as EAT_OUT,
	sum(CASE WHEN type= 'ERRANDS' THEN 1 END) as ERRANDS,
        sum(CASE WHEN type= 'HEALTHCARE' THEN 1 END) as HEALTHCARE,
	sum(CASE WHEN type= 'HOME' THEN 1 END) as HOME,
        sum(CASE WHEN type= 'LEISURE' THEN 1 END) as LEISURE,
	sum(CASE WHEN type= 'PERSONAL' THEN 1 END) as PERSONAL,
	sum(CASE WHEN type= 'PICKUP-DROPOFF' THEN 1 END) as PICKUP,
        sum(CASE WHEN type= 'RELIGIOUS-CIVIC' THEN 1 END) as RELIGIOUS,
	sum(CASE WHEN type= 'SCHOOL' THEN 1 END) as SCHOOL,
        sum(CASE WHEN type= 'SERVICE' THEN 1 END) as SERVICE,
	sum(CASE WHEN type= 'SHOP-MAJOR' THEN 1 END) as SHOP_MAJOR,
	sum(CASE WHEN type= 'SOCIAL' THEN 1 END) as SOCIAL,
        sum(CASE WHEN type= 'WORK' THEN 1 END) as WORK,
	sum(CASE WHEN type= 'SHOP-OTHER' THEN 1 END) as SHOP_OTHER,
	sum(1) AS total
FROM 
	Activity
GROUP BY 
	cast (start_time/60 as int);
	
DROP TABLE IF EXISTS Trip_Start_Distribution;
CREATE TABLE IF NOT EXISTS Trip_Start_Distribution As
SELECT 
	cast(start/60 as int),
    sum(1) as Total_Trips
FROM 
	Trip
WHERE
	Start > 62 and mode = 0
GROUP BY 
	cast (start/60 as int);
	
--############################################################
-- GENERATE ACTIVITY DISTRIBUTION PER PERSON
DROP TABLE IF EXISTS Activity_Distribution_By_Person;
CREATE TABLE IF NOT EXISTS Activity_Distribution_By_Person As
SELECT 
	person,
        sum(CASE WHEN type= 'EAT OUT' THEN 1 END) as EAT_OUT,
	sum(CASE WHEN type= 'ERRANDS' THEN 1 END) as ERRANDS,
        sum(CASE WHEN type= 'HEALTHCARE' THEN 1 END) as HEALTHCARE,
	sum(CASE WHEN type= 'HOME' THEN 1 END) as HOME,
        sum(CASE WHEN type= 'LEISURE' THEN 1 END) as LEISURE,
	sum(CASE WHEN type= 'PERSONAL' THEN 1 END) as PERSONAL,
        sum(CASE WHEN type= 'RELIGIOUS-CIVIC' THEN 1 END) as RELIGIOUS,
	sum(CASE WHEN type= 'SCHOOL' THEN 1 END) as SCHOOL,
        sum(CASE WHEN type= 'SERVICE' THEN 1 END) as SERVICE,
	sum(CASE WHEN type= 'SHOP-MAJOR' THEN 1 END) as SHOP_MAJOR,
	sum(CASE WHEN type= 'SOCIAL' THEN 1 END) as SOCIAL,
        sum(CASE WHEN type= 'WORK' THEN 1 END) as WORK,
	sum(CASE WHEN type= 'SHOP-OTHER' THEN 1 END) as SHOP_OTHER,
	sum(1) AS total
FROM 
	Activity
GROUP BY 
	person;
	
--############################################################
-- GENERATE ACTIVITY DISTRIBUTION
DROP TABLE IF EXISTS Activity_Distribution;
CREATE TABLE IF NOT EXISTS Activity_Distribution As
SELECT 
    sum(CASE WHEN type= 'EAT OUT' THEN 1 END) as EAT_OUT,
	sum(CASE WHEN type= 'ERRANDS' THEN 1 END) as ERRANDS,
    sum(CASE WHEN type= 'HEALTHCARE' THEN 1 END) as HEALTHCARE,
	sum(CASE WHEN type= 'HOME' THEN 1 END) as HOME,
    sum(CASE WHEN type= 'LEISURE' THEN 1 END) as LEISURE,
	sum(CASE WHEN type= 'PERSONAL' THEN 1 END) as PERSONAL,
    sum(CASE WHEN type= 'RELIGIOUS-CIVIC' THEN 1 END) as RELIGIOUS,
	sum(CASE WHEN type= 'SCHOOL' THEN 1 END) as SCHOOL,
    sum(CASE WHEN type= 'SERVICE' THEN 1 END) as SERVICE,
	sum(CASE WHEN type= 'SHOP-MAJOR' THEN 1 END) as SHOP_MAJOR,
	sum(CASE WHEN type= 'SOCIAL' THEN 1 END) as SOCIAL,
    sum(CASE WHEN type= 'WORK' THEN 1 END) as WORK,
	sum(CASE WHEN type= 'SHOP-OTHER' THEN 1 END) as SHOP_OTHER,
	sum(1) AS total
FROM 
	Activity;

	
	
--############################################################
-- GENERATE TRIP ORIGIN / DESTINATION BY TIME TABLES
DROP TABLE IF EXISTS Origin_By_Time_Distribution;
CREATE TABLE IF NOT EXISTS Origin_By_Time_Distribution As
SELECT 
	origin,
        sum(CASE WHEN cast(start/3600 as int ) = 0 THEN vehicle END) as '12AM',
        sum(CASE WHEN cast(start/3600 as int ) = 1 THEN vehicle END) as '1AM',
        sum(CASE WHEN cast(start/3600 as int ) = 2 THEN vehicle END) as '2AM',
        sum(CASE WHEN cast(start/3600 as int ) = 3 THEN vehicle END) as '3AM',
        sum(CASE WHEN cast(start/3600 as int ) = 4 THEN vehicle END) as '4AM',
        sum(CASE WHEN cast(start/3600 as int ) = 5 THEN vehicle END) as '5AM',
        sum(CASE WHEN cast(start/3600 as int ) = 6 THEN vehicle END) as '6AM',
        sum(CASE WHEN cast(start/3600 as int ) = 7 THEN vehicle END) as '7AM',
        sum(CASE WHEN cast(start/3600 as int ) = 8 THEN vehicle END) as '8AM',
        sum(CASE WHEN cast(start/3600 as int ) = 9 THEN vehicle END) as '9AM',
        sum(CASE WHEN cast(start/3600 as int ) = 10 THEN vehicle END) as '10AM',
        sum(CASE WHEN cast(start/3600 as int ) = 11 THEN vehicle END) as '11AM',
        sum(CASE WHEN cast(start/3600 as int ) = 12 THEN vehicle END) as '12PM',
        sum(CASE WHEN cast(start/3600 as int ) = 13 THEN vehicle END) as '1PM',
        sum(CASE WHEN cast(start/3600 as int ) = 14 THEN vehicle END) as '2PM',
        sum(CASE WHEN cast(start/3600 as int ) = 15 THEN vehicle END) as '3PM',
        sum(CASE WHEN cast(start/3600 as int ) = 16 THEN vehicle END) as '4PM',
        sum(CASE WHEN cast(start/3600 as int ) = 17 THEN vehicle END) as '5PM',
        sum(CASE WHEN cast(start/3600 as int ) = 18 THEN vehicle END) as '6PM',
        sum(CASE WHEN cast(start/3600 as int ) = 19 THEN vehicle END) as '7PM',
        sum(CASE WHEN cast(start/3600 as int ) = 20 THEN vehicle END) as '8PM',
        sum(CASE WHEN cast(start/3600 as int ) = 21 THEN vehicle END) as '9PM',
        sum(CASE WHEN cast(start/3600 as int ) = 22 THEN vehicle END) as '10PM',
        sum(CASE WHEN cast(start/3600 as int ) = 23 THEN vehicle END) as '11PM',
	sum(vehicle ) AS total
FROM 
	Trip
GROUP BY 
	origin;
	
DROP TABLE IF EXISTS Destinations_by_Time_Distribution;
CREATE TABLE Destinations_by_Time_Distribution As
SELECT 
	destination,
        sum(CASE WHEN cast(end/3600 as int ) = 0 THEN vehicle END) as '12AM',
        sum(CASE WHEN cast(end/3600 as int ) = 1 THEN vehicle END) as '1AM',
        sum(CASE WHEN cast(end/3600 as int ) = 2 THEN vehicle END) as '2AM',
        sum(CASE WHEN cast(end/3600 as int ) = 3 THEN vehicle END) as '3AM',
        sum(CASE WHEN cast(end/3600 as int ) = 4 THEN vehicle END) as '4AM',
        sum(CASE WHEN cast(end/3600 as int ) = 5 THEN vehicle END) as '5AM',
        sum(CASE WHEN cast(end/3600 as int ) = 6 THEN vehicle END) as '6AM',
        sum(CASE WHEN cast(end/3600 as int ) = 7 THEN vehicle END) as '7AM',
        sum(CASE WHEN cast(end/3600 as int ) = 8 THEN vehicle END) as '8AM',
        sum(CASE WHEN cast(end/3600 as int ) = 9 THEN vehicle END) as '9AM',
        sum(CASE WHEN cast(end/3600 as int ) = 10 THEN vehicle END) as '10AM',
        sum(CASE WHEN cast(end/3600 as int ) = 11 THEN vehicle END) as '11AM',
        sum(CASE WHEN cast(end/3600 as int ) = 12 THEN vehicle END) as '12PM',
        sum(CASE WHEN cast(end/3600 as int ) = 13 THEN vehicle END) as '1PM',
        sum(CASE WHEN cast(end/3600 as int ) = 14 THEN vehicle END) as '2PM',
        sum(CASE WHEN cast(end/3600 as int ) = 15 THEN vehicle END) as '3PM',
        sum(CASE WHEN cast(end/3600 as int ) = 16 THEN vehicle END) as '4PM',
        sum(CASE WHEN cast(end/3600 as int ) = 17 THEN vehicle END) as '5PM',
        sum(CASE WHEN cast(end/3600 as int ) = 18 THEN vehicle END) as '6PM',
        sum(CASE WHEN cast(end/3600 as int ) = 19 THEN vehicle END) as '7PM',
        sum(CASE WHEN cast(end/3600 as int ) = 20 THEN vehicle END) as '8PM',
        sum(CASE WHEN cast(end/3600 as int ) = 21 THEN vehicle END) as '9PM',
        sum(CASE WHEN cast(end/3600 as int ) = 22 THEN vehicle END) as '10PM',
        sum(CASE WHEN cast(end/3600 as int ) = 23 THEN vehicle END) as '11PM',
	sum(vehicle ) AS total
FROM 
	Trip
GROUP BY 
	destination;
	
	
--############################################################
-- GENERATE TRAVEL TIME BY ACTIVITY TYPE DISTRIBUTION
DROP TABLE IF EXISTS Activity_TTime_tmp;
CREATE TABLE Activity_TTime_tmp As
SELECT Activity.location_id, Activity.start_time AS Start, round((end-start)/60) AS TTime, Activity.Mode, Activity.type, id
FROM Trip
INNER JOIN Activity
ON Activity.trip=Trip.trip_id;


DROP TABLE IF EXISTS Activity_TTime_Distribution;
CREATE TABLE Activity_TTime_Distribution As
SELECT 
	TTime,
        sum(CASE WHEN type= 'EAT OUT' THEN 1 END) as EAT_OUT,
	sum(CASE WHEN type= 'ERRANDS' THEN 1 END) as ERRANDS,
        sum(CASE WHEN type= 'HEALTHCARE' THEN 1 END) as HEALTHCARE,
	sum(CASE WHEN type= 'HOME' THEN 1 END) as HOME,
        sum(CASE WHEN type= 'LEISURE' THEN 1 END) as LEISURE,
	sum(CASE WHEN type= 'PERSONAL' THEN 1 END) as PERSONAL,
        sum(CASE WHEN type= 'RELIGIOUS-CIVIC' THEN 1 END) as RELIGIOUS,
	sum(CASE WHEN type= 'SCHOOL' THEN 1 END) as SCHOOL,
        sum(CASE WHEN type= 'SERVICE' THEN 1 END) as SERVICE,
	sum(CASE WHEN type= 'SHOP-MAJOR' THEN 1 END) as SHOP_MAJOR,
	sum(CASE WHEN type= 'SOCIAL' THEN 1 END) as SOCIAL,
        sum(CASE WHEN type= 'WORK' THEN 1 END) as WORK,
	sum(CASE WHEN type= 'SHOP-OTHER' THEN 1 END) as SHOP_OTHER,
	sum(1) AS total
FROM 
	Activity_TTime_tmp
GROUP BY 
	TTime;

DROP TABLE IF EXISTS Activity_TTime_tmp;


--############################################################
-- Append O/D and zone information to the activities for later queries
DROP TABLE IF EXISTS Activity_TTime_tmp;
CREATE TABLE Activity_TTime_tmp As
SELECT Activity.*, Trip.origin as origin, Trip.destination as destination
FROM Trip
INNER JOIN Activity
ON Activity.trip=Trip.trip_id;

DROP TABLE IF EXISTS Activity_With_Zone_tmp;
CREATE TABLE Activity_With_Zone_tmp As
SELECT Activity_TTime_tmp.*, supply.Location.zone as origzone
FROM Activity_TTime_tmp
INNER JOIN supply.Location
ON Activity_TTime_tmp.origin=supply.Location.location;

DROP TABLE IF EXISTS Activity_With_Zone;
CREATE TABLE Activity_With_Zone As
SELECT Activity_With_Zone_tmp.*, supply.Location.zone as destzone
FROM Activity_With_Zone_tmp
INNER JOIN supply.Location
ON Activity_With_Zone_tmp.destination=supply.Location.location;

DROP TABLE IF EXISTS Activity_TTime_tmp;
DROP TABLE IF EXISTS Activity_With_Zone_tmp;

--############################################################
--Calculate home to work distance distribution
DROP TABLE IF EXISTS Person_Work_Distance_tmp;
CREATE TABLE Person_Work_Distance_tmp As
SELECT Person.person, Person.ID, Person.household, Person.Work_Location_id as work_loc, Household.location as house_loc
FROM Person, Household
where Person.household = household.household;

DROP TABLE IF EXISTS Person_Work_Distance_tmp2;
CREATE TABLE Person_Work_Distance_tmp2 As
SELECT Person_Work_Distance_tmp.person, Person_Work_Distance_tmp.ID, Person_Work_Distance_tmp.household, supply.location.X as work_x, supply.location.Y as work_y, Person_Work_Distance_tmp.house_loc as house_loc
FROM Person_Work_Distance_tmp, supply.location
where Person_Work_Distance_tmp.work_loc = supply.location.location;

DROP TABLE IF EXISTS Person_Work_Distance_tmp3;
CREATE TABLE Person_Work_Distance_tmp3 As
SELECT Person_Work_Distance_tmp2.person, Person_Work_Distance_tmp2.ID, Person_Work_Distance_tmp2.household, Person_Work_Distance_tmp2.work_x, Person_Work_Distance_tmp2.work_y, supply.location.X as home_x, supply.location.Y as home_y
FROM Person_Work_Distance_tmp2, supply.location
where Person_Work_Distance_tmp2.house_loc = supply.location.location;

CREATE TABLE Person_Work_Distance As
SELECT Person_Work_Distance_tmp3.*, Sqrt(Pow((home_x - work_x),2) + Pow((home_y - work_y),2)) as Dist, Round(Sqrt(Pow((home_x - work_x),2) + Pow((home_y - work_y),2))/1000,1) AS Dist_Rnd
FROM Person_Work_Distance_tmp3;

DROP TABLE IF EXISTS Person_Work_Distance_tmp;
DROP TABLE IF EXISTS Person_Work_Distance_tmp2;
DROP TABLE IF EXISTS Person_Work_Distance_tmp3;

SELECT 
	Dist_rnd, count(1)
FROM 
	Person_Work_Distance
GROUP BY 
	Dist_rnd;


--############################################################
--Calculate straight-line distance for each activity
DROP TABLE IF EXISTS Activity_With_OD;
CREATE TABLE Activity_With_OD As
SELECT Activity.*, Trip.origin as origin, Trip.destination as destination
FROM Trip
INNER JOIN Activity
ON Activity.trip=Trip.trip_id;

DROP TABLE IF EXISTS Activity_With_OD_tmp;
CREATE TABLE Activity_With_OD_tmp As
SELECT Activity_With_OD.*, supply.Location.x as Orig_X, supply.Location.y as Orig_Y
FROM Activity_With_OD
INNER JOIN supply.Location
ON Activity_With_OD.origin=supply.Location.location;

DROP TABLE IF EXISTS Activity_With_OD_tmp2;
CREATE TABLE Activity_With_OD_tmp2 As
SELECT Activity_With_OD_tmp.*, supply.Location.x as Dest_X,supply.Location.y as Dest_Y
FROM Activity_With_OD_tmp
INNER JOIN supply.Location
ON Activity_With_OD_tmp.destination=supply.Location.location;

DROP TABLE IF EXISTS Activity_With_Distance;
CREATE TABLE Activity_With_Distance As
SELECT Activity_With_OD_tmp2.*, Sqrt(Pow((Orig_X - Dest_X),2) + Pow((Orig_Y - Dest_Y),2)) as Dist, Round(Sqrt(Pow((Orig_X - Dest_X),2) + Pow((Orig_Y - Dest_Y),2))/1000,1) AS Dist_Rnd
FROM Activity_With_OD_tmp2;

DROP TABLE IF EXISTS Activity_With_OD;
DROP TABLE IF EXISTS Activity_With_OD_tmp;
DROP TABLE IF EXISTS Activity_With_OD_tmp2;

SELECT 
	Dist_rnd,
      sum(CASE WHEN type= 'EAT OUT' THEN 1 END) as EAT_OUT,
	sum(CASE WHEN type= 'ERRANDS' THEN 1 END) as ERRANDS,
      sum(CASE WHEN type= 'HEALTHCARE' THEN 1 END) as HEALTHCARE,
	sum(CASE WHEN type= 'HOME' THEN 1 END) as HOME,
      sum(CASE WHEN type= 'LEISURE' THEN 1 END) as LEISURE,
	sum(CASE WHEN type= 'PERSONAL' THEN 1 END) as PERSONAL,
      sum(CASE WHEN type= 'RELIGIOUS-CIVIC' THEN 1 END) as RELIGIOUS,
	sum(CASE WHEN type= 'SCHOOL' THEN 1 END) as SCHOOL,
      sum(CASE WHEN type= 'SERVICE' THEN 1 END) as SERVICE,
	sum(CASE WHEN type= 'SHOP-MAJOR' THEN 1 END) as SHOP_MAJOR,
	sum(CASE WHEN type= 'SOCIAL' THEN 1 END) as SOCIAL,
      sum(CASE WHEN type= 'WORK' THEN 1 END) as WORK,
	sum(CASE WHEN type= 'SHOP-OTHER' THEN 1 END) as SHOP_OTHER,
	sum(1) AS total
FROM 
	Activity_With_Distance
GROUP BY 
	Dist_rnd;



--############################################################
-- Productions and attractions
DROP TABLE IF EXISTS Productions;
CREATE TABLE Productions As
SELECT 
  origzone, count(origzone) as Prod
FROM Activity_With_Zone
GROUP BY origzone;

DROP TABLE IF EXISTS Attractions;
CREATE TABLE Attractions As
SELECT 
  destzone, count(destzone) as Prod
FROM Activity_With_Zone
GROUP BY destzone;



--############################################################
-- IF DOING PUMA-level analysis, make sure to attach zone to puma crosswalk table
DROP TABLE IF EXISTS Activity_With_Zone_tmp;
CREATE TABLE Activity_With_Zone_tmp As
SELECT Activity_With_Zone.*, Zone_To_Area_Crosswalk.PUMA as orig_puma
FROM Activity_With_Zone
INNER JOIN Zone_To_Area_Crosswalk
ON Activity_With_Zone.origzone=Zone_To_Area_Crosswalk.zone;

DROP TABLE IF EXISTS Activity_With_PUMA;
CREATE TABLE Activity_With_PUMA As
SELECT Activity_With_Zone_tmp.*, Zone_To_Area_Crosswalk.PUMA as dest_puma
FROM Activity_With_Zone_tmp
INNER JOIN Zone_To_Area_Crosswalk
ON Activity_With_Zone_tmp.destzone=Zone_To_Area_Crosswalk.zone;

DROP TABLE IF EXISTS Activity_With_Zone_tmp;


--############################################################
-- GENERATE MODE DISTRIBUTION TO EACH DESTINATION ZONE
DROP TABLE IF EXISTS Modes_by_Destination_all;
CREATE TABLE Modes_by_Destination_all As
SELECT supply.Location.zone as Zone, Activity.Mode, Activity.type, id
FROM Activity
INNER JOIN supply.Location
ON supply.Location.location =Activity.location_id;


DROP TABLE IF EXISTS Mode_By_Destination_Distribution;
CREATE TABLE IF NOT EXISTS Mode_By_Destination_Distribution As
SELECT 
	Zone,
	sum(CASE WHEN mode= 'AUTO' THEN 1 END) as Auto,
	sum(CASE WHEN mode= 'HOV' THEN 1 END) as Hov,
	sum(CASE WHEN mode= 'TRANSIT' THEN 1 END) as Transit,
	sum(1) AS total
FROM 
	Modes_by_Destination_all
GROUP BY 
	Zone;

DROP TABLE IF EXISTS Modes_by_Destination_all;


DROP TABLE IF EXISTS Modes_by_Destination_all;
CREATE TABLE Modes_by_Destination_all As
SELECT supply.Location.zone as Zone, Activity.Mode, Activity.type, id
FROM Activity
INNER JOIN supply.Location
ON supply.Location.location =Activity.location_id
WHERE Activity.type='WORK';


DROP TABLE IF EXISTS Mode_By_Work_Destination_Distribution;
CREATE TABLE IF NOT EXISTS Mode_By_Work_Destination_Distribution As
SELECT 
	Zone,
	sum(CASE WHEN mode= 'AUTO' THEN 1 END) as Auto,
	sum(CASE WHEN mode= 'TRANSIT' THEN 1 END) as Transit,
	sum(1) AS total
FROM 
	Modes_by_Destination_all
GROUP BY 
	Zone;

DROP TABLE IF EXISTS Modes_by_Destination_all;


--############################################################
-- GENERATE MODE DISTRIBUTION FROM EACH ORIGIN ZONE
DROP TABLE IF EXISTS Modes_by_Origin_Temp;
CREATE TABLE Modes_by_Origin_Temp As
SELECT Trip.origin as orig, Trip.destination as destination, Activity.Mode, Activity.type, id, Start_Time
FROM Activity
INNER JOIN Trip
ON Trip.trip_id =Activity.id;

DROP TABLE IF EXISTS Modes_by_Origin_Temp_2;
CREATE TABLE Modes_by_Origin_Temp_2 As
SELECT supply.Location.zone as OrigZone, Modes_by_Origin_Temp.destination as destination, Modes_by_Origin_Temp.Mode, Modes_by_Origin_Temp.type,Modes_by_Origin_Temp.id, Start_Time
FROM Modes_by_Origin_Temp
INNER JOIN supply.Location
ON supply.Location.location =Modes_by_Origin_Temp.orig;



DROP TABLE IF EXISTS Modes_by_Origin_Temp_3;
CREATE TABLE Modes_by_Origin_Temp_3 As
SELECT Modes_by_Origin_Temp_2.OrigZone as OrigZone, supply.Location.zone as DestZone, Modes_by_Origin_Temp_2.Mode, Modes_by_Origin_Temp_2.type,Modes_by_Origin_Temp_2.id, Start_Time
FROM Modes_by_Origin_Temp_2
INNER JOIN supply.Location
ON supply.Location.location =Modes_by_Origin_Temp_2.destination;


DROP TABLE IF EXISTS Mode_By_Origin_Distribution;
CREATE TABLE IF NOT EXISTS Mode_By_Origin_Distribution As
SELECT 
	OrigZone,
	sum(CASE WHEN mode= 'AUTO' THEN 1 END) as Auto,
	sum(CASE WHEN mode= 'TRANSIT' THEN 1 END) as Transit,
	sum(1) AS total
FROM 
	Modes_by_Origin_Temp_3
Where
	Start_Time > 62
GROUP BY 
	OrigZone;
	
DROP TABLE IF EXISTS Mode_By_Origin_Distribution_To_Work;
CREATE TABLE IF NOT EXISTS Mode_By_Origin_Distribution_To_Work As
SELECT 
	OrigZone,
	sum(CASE WHEN mode= 'AUTO' THEN 1 END) as Auto,
	sum(CASE WHEN mode= 'TRANSIT' THEN 1 END) as Transit,
	sum(1) AS total
FROM 
	Modes_by_Origin_Temp_3
WHERE
       OrigZone<>DestZone AND type = 'WORK' AND Start_Time > 62
GROUP BY 
	OrigZone;

DROP TABLE IF EXISTS Modes_by_Origin_Temp;
DROP TABLE IF EXISTS Modes_by_Origin_Temp_2;
DROP TABLE IF EXISTS Modes_by_Origin_Temp_3;



--############################################################
-- GENERATE MODE DISTRIBUTION FROM EACH DESTINATION ZONE - IGNORE INTRAZONAL TRIPS
DROP TABLE IF EXISTS Modes_by_Destination_Temp;
CREATE TABLE Modes_by_Destination_Temp As
SELECT Trip.origin as orig, Trip.destination as destination, Activity.Mode, Activity.type, id, Start_Time
FROM Activity
INNER JOIN Trip
ON Trip.trip_id =Activity.id;

DROP TABLE IF EXISTS Modes_by_Destination_Temp_2;
CREATE TABLE Modes_by_Destination_Temp_2 As
SELECT supply.Location.zone as OrigZone, Modes_by_Destination_Temp.destination as destination, Modes_by_Destination_Temp.Mode, Modes_by_Destination_Temp.type,Modes_by_Destination_Temp.id, Start_Time
FROM Modes_by_Destination_Temp
INNER JOIN supply.Location
ON supply.Location.location = Modes_by_Destination_Temp.orig;



DROP TABLE IF EXISTS Modes_by_Destination_Temp_3;
CREATE TABLE Modes_by_Destination_Temp_3 As
SELECT Modes_by_Destination_Temp_2.OrigZone as OrigZone, supply.Location.zone as DestZone, Modes_by_Destination_Temp_2.Mode, Modes_by_Destination_Temp_2.type,Modes_by_Destination_Temp_2.id, Start_Time
FROM Modes_by_Destination_Temp_2
INNER JOIN supply.Location
ON supply.Location.location =Modes_by_Destination_Temp_2.destination;


DROP TABLE IF EXISTS Mode_By_Destination_Distribution;
CREATE TABLE IF NOT EXISTS Mode_By_Destination_Distribution As
SELECT 
	DestZone,
	sum(CASE WHEN mode= 'AUTO' THEN 1 END) as Auto,
	sum(CASE WHEN mode= 'TRANSIT' THEN 1 END) as Transit,
	sum(1) AS total
FROM 
	Modes_by_Destination_Temp_3
WHERE
	Start_Time > 62
GROUP BY 
	DestZone;
	
DROP TABLE IF EXISTS Mode_By_Destination_Distribution_To_Work;
CREATE TABLE IF NOT EXISTS Mode_By_Destination_Distribution_To_Work As
SELECT 
	DestZone,
	sum(CASE WHEN mode= 'AUTO' THEN 1 END) as Auto,
	sum(CASE WHEN mode= 'TRANSIT' THEN 1 END) as Transit,
	sum(1) AS total
FROM 
	Modes_by_Destination_Temp_3
WHERE
       OrigZone<>DestZone AND type = 'WORK' AND Start_Time > 62
GROUP BY 
	DestZone;

DROP TABLE IF EXISTS Modes_by_Destination_Temp;
DROP TABLE IF EXISTS Modes_by_Destination_Temp_2;
DROP TABLE IF EXISTS Modes_by_Destination_Temp_3;



--############################################################
-- GENERATE EMPLOYMENT DISTRIBUTION
DROP TABLE IF EXISTS PersonWorkplaceTemp;
CREATE TABLE PersonWorkplaceTemp As
SELECT supply.Location.zone as workzone, person, id
FROM Person
INNER JOIN supply.Location
ON supply.Location.location =Person.work_location_id;

DROP TABLE IF EXISTS Employment_Distribution_By_Zone;
CREATE TABLE IF NOT EXISTS Employment_Distribution_By_Zone As
SELECT 
	workzone,
	sum(1) AS total
FROM 
	PersonWorkplaceTemp
GROUP BY 
	workzone;
	
DROP TABLE IF EXISTS PersonWorkplaceTemp;



--############################################################
-- CBD Employment by home taz distribution (origins of CBD workers)
DROP TABLE IF EXISTS PersonWorkplaceTemp1;
CREATE TABLE PersonWorkplaceTemp1 As
SELECT Household.location as home_location, Person.person,  Person.id,  Person.work_location_id
FROM Person
INNER JOIN Household
ON Person.household  = Household.household;

DROP TABLE IF EXISTS PersonWorkplaceTemp2;
CREATE TABLE PersonWorkplaceTemp2 As
SELECT supply.Location.zone as workzone, person, id, PersonWorkplaceTemp1.home_location as home_location
FROM PersonWorkplaceTemp1
INNER JOIN supply.Location
ON supply.Location.location =PersonWorkplaceTemp1.work_location_id;


DROP TABLE IF EXISTS PersonWorkplaceTemp3;
CREATE TABLE PersonWorkplaceTemp3 As
SELECT supply.Location.zone as homezone, person, id, workzone
FROM PersonWorkplaceTemp2
INNER JOIN supply.Location
ON supply.Location.location =PersonWorkplaceTemp2.home_location;


DROP TABLE IF EXISTS CBDEmploymentByOriginZone;
CREATE TABLE IF NOT EXISTS  CBDEmploymentByOriginZone As
SELECT 
	homezone,
	sum(1) AS total
FROM 
	PersonWorkplaceTemp3
WHERE 
	workzone <= 77
GROUP BY 
	homezone;
	
DROP TABLE IF EXISTS PersonWorkplaceTemp1;
DROP TABLE IF EXISTS PersonWorkplaceTemp2;
DROP TABLE IF EXISTS PersonWorkplaceTemp3;


--############################################################
-- PUMA Level OD table
CREATE TABLE IF NOT EXISTS Origin_Destination_by_PUMA As
SELECT 
	OriginPuma,
	sum(CASE WHEN dest_puma = 'AUTO' THEN 1 ELSE 0 END) as Auto,
	sum(CASE WHEN mode= 'TRANSIT' THEN 1 END) as Transit,
	sum(1) AS total
FROM 
	Activity_With_Puma
GROUP BY 
	OriginPuma;

	


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
select trip.*, supply.location.zone as orig_zone from trip, supply.location where origin = supply.location.location;

create table trip_temp2 as
select trip_temp1.*, supply.location.zone as dest_zone from trip_temp1, supply.location where destination = supply.location.location;


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





DROP TABLE IF EXISTS Trip_Zone_tmp;
CREATE TABLE Trip_Zone_tmp As
SELECT Trip.*, supply.Location.zone as origzone
FROM Trip
INNER JOIN supply.Location
ON trip.origin=supply.Location.location;

DROP TABLE IF EXISTS Trip_With_Zone;
CREATE TABLE Trip_With_Zone As
SELECT Trip_Zone_tmp.*, supply.Location.zone as destzone
FROM Trip_Zone_tmp
INNER JOIN supply.Location
ON Trip_Zone_tmp.destination=supply.Location.location;

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
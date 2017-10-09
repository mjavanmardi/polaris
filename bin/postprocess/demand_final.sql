--############################################################
-- CREATE INDICES TO SPEED UP QUERIES
CREATE INDEX activity_person_idx ON "Activity" (person);
CREATE INDEX activity_trip_idx ON "Activity" (trip);
CREATE INDEX activity_location_idx ON "Activity" (location_id);
CREATE INDEX person_household_idx ON "Person" (household);
CREATE INDEX person_work_idx ON "Person" (work_location_id);

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
	
DROP TABLE IF EXISTS TTime_By_Act_Distribution;
CREATE TABLE IF NOT EXISTS TTime_By_Act_Distribution As
SELECT 
	round((end-start)/60) as TTime_minutes,
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
	sum(CASE WHEN type= 'WORK AT HOME' THEN 1 END) as WORK_HOME,
	sum(CASE WHEN type= 'PART_WORK' THEN 1 END) as WORK_PART,
	sum(CASE WHEN type= 'SHOP-OTHER' THEN 1 END) as SHOP_OTHER,
	sum(1) AS total
FROM 
	Trip
WHERE
	Start > 62 and mode = 0
GROUP BY 
	TTime_minutes;


--############################################################
-- ACTIVITY GENERATION RESULTS
DROP TABLE IF EXISTS Person_types;
CREATE TABLE IF NOT EXISTS Person_types As
SELECT 
case when (employment = 4 or employment = 1) and work_hours >= 30 then 3
	when (employment = 4 or employment = 1) then 4
	when (school_enrollment = 3 or school_enrollment = 2) and age > 18 then 7
	when age >= 65 then 6
	when age < 65 and age > 18 then 5
	when age >= 16 and age <= 18 then 0
	when age < 16 and age >= 5 then 1
	when age < 5 then 2
	else 5
end as per_cat,
sum(1) AS total
from person
GROUP BY 
	per_cat;
	
DROP TABLE IF EXISTS Activity_with_person;
CREATE TABLE IF NOT EXISTS Activity_with_person As
SELECT 
case when (employment = 4 or employment = 1) and work_hours >= 30 then 3
	when (employment = 4 or employment = 1) then 4
	when (school_enrollment = 3 or school_enrollment = 2) and age > 18 then 7
	when age >= 65 then 6
	when age < 65 and age > 18 then 5
	when age >= 16 and age <= 18 then 0
	when age < 16 and age >= 5 then 1
	when age < 5 then 2
	else 5
end as per_cat,
sum(CASE WHEN type= 'EAT OUT' THEN 1 END) as EAT_OUT,
sum(CASE WHEN type= 'ERRANDS' THEN 1 END) as ERRANDS,
sum(CASE WHEN type= 'HEALTHCARE' THEN 1 END) as HEALTHCARE,
sum(CASE WHEN type= 'HOME' THEN 1 END) as HOME,
sum(CASE WHEN type= 'LEISURE' THEN 1 END) as LEISURE,
sum(CASE WHEN type= 'PERSONAL' THEN 1 END) as PERSONAL,
sum(CASE WHEN type= 'PICKUP-DROPOFF' THEN 1 END) as PICK_DROP,
sum(CASE WHEN type= 'RELIGIOUS-CIVIC' THEN 1 END) as RELIGIOUS,
sum(CASE WHEN type= 'SCHOOL' THEN 1 END) as SCHOOL,
sum(CASE WHEN type= 'SERVICE' THEN 1 END) as SERVICE,
sum(CASE WHEN type= 'SHOP-MAJOR' THEN 1 END) as SHOP_MAJOR,
sum(CASE WHEN type= 'SOCIAL' THEN 1 END) as SOCIAL,
sum(CASE WHEN type= 'WORK' THEN 1 END) as WORK,
sum(CASE WHEN type= 'WORK AT HOME' THEN 1 END) as WORK_HOME,
sum(CASE WHEN type= 'PART_WORK' THEN 1 END) as WORK_PART,
sum(CASE WHEN type= 'SHOP-OTHER' THEN 1 END) as SHOP_OTHER,
sum(1) AS total
from person, activity
where person.person = activity.person and activity.Start_Time > 62
GROUP BY 
	per_cat;

--############################################################
-- GENERATE ACTIVITY TYPE BY MODAL DISTRIBUTION

DROP TABLE IF EXISTS Mode_Distribution_ADULT;
CREATE TABLE IF NOT EXISTS Mode_Distribution_ADULT As
SELECT 
	case when activity.type = 'WORK' or activity.type = 'SCHOOL' or activity.type = 'PART_WORK' then 'HBW'
	when trip.origin = household.location then 'HBO'
	else 'NHB' end as acttype,
	sum(CASE WHEN activity.mode= 'AUTO' THEN 1 END) as Auto,
	sum(CASE WHEN activity.mode= 'HOV' THEN 1 END) as HOV,
	sum(CASE WHEN activity.mode= 'TRANSIT' THEN 1 END) as Transit,
	sum(CASE WHEN activity.mode= 'WALK' THEN 1 END) as Walk,
	sum(CASE WHEN activity.mode= 'BIKE' THEN 1 END) as Bike,
	sum(CASE WHEN activity.mode= 'TAXI' THEN 1 END) as Taxi,
	sum(CASE WHEN activity.mode= 'SCHOOLBUS' THEN 1 END) as Schoolbus,
	sum(1) AS total
FROM 
	Activity, person, household, trip
WHERE
	Start_Time > 62 and activity.trip = trip.trip_id and person.age >= 16 and activity.person = person.person and person.household=household.household
GROUP BY 
	acttype;


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
	sum(CASE WHEN type= 'WORK AT HOME' THEN 1 END) as WORK_HOME,
	sum(CASE WHEN type= 'PART_WORK' THEN 1 END) as WORK_PART,
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

DROP TABLE IF EXISTS Mode_TTime_Distribution;
CREATE TABLE Mode_TTime_Distribution As
SELECT 
	TTime,
        sum(CASE WHEN mode= 'AUTO' THEN 1 END) as AUTO,
	sum(CASE WHEN mode= 'HOV' THEN 1 END) as HOV,
        sum(CASE WHEN mode= 'TRANSIT' THEN 1 END) as TRANSIT,
	sum(CASE WHEN mode= 'WALK' THEN 1 END) as WALK,
	sum(1) AS total
FROM 
	Activity_TTime_tmp
GROUP BY 
	TTime;



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
	sum(CASE WHEN type= 'PICKUP-DROPOFF' THEN 1 END) as PICK_DRIP,
	sum(CASE WHEN type= 'SCHOOL' THEN 1 END) as SCHOOL,
    sum(CASE WHEN type= 'SERVICE' THEN 1 END) as SERVICE,
	sum(CASE WHEN type= 'SHOP-MAJOR' THEN 1 END) as SHOP_MAJOR,
	sum(CASE WHEN type= 'SOCIAL' THEN 1 END) as SOCIAL,
    sum(CASE WHEN type= 'WORK' THEN 1 END) as WORK,
	sum(CASE WHEN type= 'WORK AT HOME' THEN 1 END) as WORK_HOME,
	sum(CASE WHEN type= 'PART_WORK' THEN 1 END) as WORK_PART,
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

CREATE INDEX activity_tmp_orig_idx ON "Activity_TTime_tmp" (origin);


DROP TABLE IF EXISTS Activity_With_Zone_tmp;
CREATE TABLE Activity_With_Zone_tmp As
SELECT Activity_TTime_tmp.*, a.Location.zone as origzone
FROM Activity_TTime_tmp
INNER JOIN a.Location
ON Activity_TTime_tmp.origin=a.Location.location;

CREATE INDEX activity_tmp_dest_idx ON "Activity_With_Zone_tmp" (destination);

DROP TABLE IF EXISTS Activity_With_Zone;
CREATE TABLE Activity_With_Zone As
SELECT Activity_With_Zone_tmp.*, a.Location.zone as destzone
FROM Activity_With_Zone_tmp
INNER JOIN a.Location
ON Activity_With_Zone_tmp.destination=a.Location.location;

DROP TABLE IF EXISTS Activity_TTime_tmp;
DROP TABLE IF EXISTS Activity_With_Zone_tmp;

--############################################################
--Calculate home to work distance distribution
DROP TABLE IF EXISTS Person_Work_Distance_tmp;
CREATE TABLE Person_Work_Distance_tmp As
SELECT Person.person, Person.ID, Person.household, Person.Work_Location_id as work_loc, Household.location as house_loc
FROM Person, Household
where Person.household = household.household;

CREATE INDEX work_loc_idx ON "Person_Work_Distance_tmp" (work_loc);

DROP TABLE IF EXISTS Person_Work_Distance_tmp2;
CREATE TABLE Person_Work_Distance_tmp2 As
SELECT Person_Work_Distance_tmp.person, Person_Work_Distance_tmp.ID, Person_Work_Distance_tmp.household, a.location.X as work_x, a.location.Y as work_y, Person_Work_Distance_tmp.house_loc as house_loc
FROM Person_Work_Distance_tmp, a.location
where Person_Work_Distance_tmp.work_loc = a.location.location;

CREATE INDEX house_loc_idx ON "Person_Work_Distance_tmp2" (house_loc);

DROP TABLE IF EXISTS Person_Work_Distance_tmp3;
CREATE TABLE Person_Work_Distance_tmp3 As
SELECT Person_Work_Distance_tmp2.person, Person_Work_Distance_tmp2.ID, Person_Work_Distance_tmp2.household, Person_Work_Distance_tmp2.work_x, Person_Work_Distance_tmp2.work_y, a.location.X as home_x, a.location.Y as home_y
FROM Person_Work_Distance_tmp2, a.location
where Person_Work_Distance_tmp2.house_loc = a.location.location;

DROP TABLE IF EXISTS Person_Work_Distance;
CREATE TABLE Person_Work_Distance As
SELECT Person_Work_Distance_tmp3.*, Sqrt(Pow((home_x - work_x),2) + Pow((home_y - work_y),2)) as Dist, Round(Sqrt(Pow((home_x - work_x),2) + Pow((home_y - work_y),2))/1000,1) AS Dist_Rnd
FROM Person_Work_Distance_tmp3;

DROP TABLE IF EXISTS Person_Work_Distance_tmp;
DROP TABLE IF EXISTS Person_Work_Distance_tmp2;
DROP TABLE IF EXISTS Person_Work_Distance_tmp3;

--SELECT 
--	Dist_rnd, count(1)
--FROM 
--	Person_Work_Distance
--GROUP BY 
--	Dist_rnd;


--############################################################
--Calculate straight-line distance for each activity
DROP TABLE IF EXISTS Activity_With_OD;
CREATE TABLE Activity_With_OD As
SELECT Activity.*, Trip.origin as origin, Trip.destination as destination
FROM Trip
INNER JOIN Activity
ON Activity.trip=Trip.trip_id;

CREATE INDEX origin_idx ON "Activity_With_OD" (origin);


DROP TABLE IF EXISTS Activity_With_OD_tmp;
CREATE TABLE Activity_With_OD_tmp As
SELECT Activity_With_OD.*, a.Location.x as Orig_X, a.Location.y as Orig_Y
FROM Activity_With_OD
INNER JOIN a.Location
ON Activity_With_OD.origin=a.Location.location;

CREATE INDEX destination_idx ON "Activity_With_OD_tmp" (destination);

DROP TABLE IF EXISTS Activity_With_OD_tmp2;
CREATE TABLE Activity_With_OD_tmp2 As
SELECT Activity_With_OD_tmp.*, a.Location.x as Dest_X,a.Location.y as Dest_Y
FROM Activity_With_OD_tmp
INNER JOIN a.Location
ON Activity_With_OD_tmp.destination=a.Location.location;

DROP TABLE IF EXISTS Activity_With_Distance;
CREATE TABLE Activity_With_Distance As
SELECT Activity_With_OD_tmp2.*, Sqrt(Pow((Orig_X - Dest_X),2) + Pow((Orig_Y - Dest_Y),2)) as Dist, Round(Sqrt(Pow((Orig_X - Dest_X),2) + Pow((Orig_Y - Dest_Y),2))/1000,1) AS Dist_Rnd
FROM Activity_With_OD_tmp2;

DROP TABLE IF EXISTS Activity_With_OD;
DROP TABLE IF EXISTS Activity_With_OD_tmp;
DROP TABLE IF EXISTS Activity_With_OD_tmp2;

DROP TABLE IF EXISTS Activity_Distance_Distribution;
CREATE TABLE Activity_Distance_Distribution As
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
	  sum(CASE WHEN type= 'WORK AT HOME' THEN 1 END) as WORK_HOME,
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
--DROP TABLE IF EXISTS Activity_With_Zone_tmp;
--CREATE TABLE Activity_With_Zone_tmp As
--SELECT Activity_With_Zone.*, Zone_To_Area_Crosswalk.PUMA as orig_puma
--FROM Activity_With_Zone
--INNER JOIN Zone_To_Area_Crosswalk
--ON Activity_With_Zone.origzone=Zone_To_Area_Crosswalk.zone;
--DROP TABLE IF EXISTS Activity_With_PUMA;
--CREATE TABLE Activity_With_PUMA As
--SELECT Activity_With_Zone_tmp.*, Zone_To_Area_Crosswalk.PUMA as dest_puma
--FROM Activity_With_Zone_tmp
--INNER JOIN Zone_To_Area_Crosswalk
--ON Activity_With_Zone_tmp.destzone=Zone_To_Area_Crosswalk.zone;
--DROP TABLE IF EXISTS Activity_With_Zone_tmp;


--############################################################
-- GENERATE MODE DISTRIBUTION TO EACH DESTINATION ZONE
DROP TABLE IF EXISTS Modes_by_Destination_all;
CREATE TABLE Modes_by_Destination_all As
SELECT a.Location.zone as Zone, Activity.Mode, Activity.type, id
FROM Activity
INNER JOIN a.Location
ON a.Location.location =Activity.location_id;


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
SELECT a.Location.zone as Zone, Activity.Mode, Activity.type, id
FROM Activity
INNER JOIN a.Location
ON a.Location.location =Activity.location_id
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

CREATE INDEX orig_idx ON "Modes_by_Origin_Temp" (orig);

DROP TABLE IF EXISTS Modes_by_Origin_Temp_2;
CREATE TABLE Modes_by_Origin_Temp_2 As
SELECT a.Location.zone as OrigZone, Modes_by_Origin_Temp.destination as destination, Modes_by_Origin_Temp.Mode, Modes_by_Origin_Temp.type,Modes_by_Origin_Temp.id, Start_Time
FROM Modes_by_Origin_Temp
INNER JOIN a.Location
ON a.Location.location =Modes_by_Origin_Temp.orig;

CREATE INDEX destination_idx ON "Modes_by_Origin_Temp_2" (destination);

DROP TABLE IF EXISTS Modes_by_Origin_Temp_3;
CREATE TABLE Modes_by_Origin_Temp_3 As
SELECT Modes_by_Origin_Temp_2.OrigZone as OrigZone, a.Location.zone as DestZone, Modes_by_Origin_Temp_2.Mode, Modes_by_Origin_Temp_2.type,Modes_by_Origin_Temp_2.id, Start_Time
FROM Modes_by_Origin_Temp_2
INNER JOIN a.Location
ON a.Location.location =Modes_by_Origin_Temp_2.destination;


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

CREATE INDEX orig_idx ON "Modes_by_Destination_Temp" (orig);

DROP TABLE IF EXISTS Modes_by_Destination_Temp_2;
CREATE TABLE Modes_by_Destination_Temp_2 As
SELECT a.Location.zone as OrigZone, Modes_by_Destination_Temp.destination as destination, Modes_by_Destination_Temp.Mode, Modes_by_Destination_Temp.type,Modes_by_Destination_Temp.id, Start_Time
FROM Modes_by_Destination_Temp
INNER JOIN a.Location
ON a.Location.location = Modes_by_Destination_Temp.orig;

CREATE INDEX destination_idx ON "Modes_by_Destination_Temp_2" (destination);

DROP TABLE IF EXISTS Modes_by_Destination_Temp_3;
CREATE TABLE Modes_by_Destination_Temp_3 As
SELECT Modes_by_Destination_Temp_2.OrigZone as OrigZone, a.Location.zone as DestZone, Modes_by_Destination_Temp_2.Mode, Modes_by_Destination_Temp_2.type,Modes_by_Destination_Temp_2.id, Start_Time
FROM Modes_by_Destination_Temp_2
INNER JOIN a.Location
ON a.Location.location =Modes_by_Destination_Temp_2.destination;


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
SELECT a.Location.zone as workzone, person, id
FROM Person
INNER JOIN a.Location
ON a.Location.location =Person.work_location_id;

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
CREATE INDEX work_location_idx ON "PersonWorkplaceTemp1" (work_location_id);

DROP TABLE IF EXISTS PersonWorkplaceTemp2;
CREATE TABLE PersonWorkplaceTemp2 As
SELECT a.Location.zone as workzone, person, id, PersonWorkplaceTemp1.home_location as home_location
FROM PersonWorkplaceTemp1
INNER JOIN a.Location
ON a.Location.location =PersonWorkplaceTemp1.work_location_id;
CREATE INDEX home_location_idx ON "PersonWorkplaceTemp2" (home_location);

DROP TABLE IF EXISTS PersonWorkplaceTemp3;
CREATE TABLE PersonWorkplaceTemp3 As
SELECT a.Location.zone as homezone, person, id, workzone
FROM PersonWorkplaceTemp2
INNER JOIN a.Location
ON a.Location.location =PersonWorkplaceTemp2.home_location;


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
--CREATE TABLE IF NOT EXISTS Origin_Destination_by_PUMA As
--SELECT 
--	OriginPuma,
--	sum(CASE WHEN dest_puma = 'AUTO' THEN 1 ELSE 0 END) as Auto,
--	sum(CASE WHEN mode= 'TRANSIT' THEN 1 END) as Transit,
--	sum(1) AS total
--FROM 
--	Activity_With_Puma
--GROUP BY 
--	OriginPuma;

	


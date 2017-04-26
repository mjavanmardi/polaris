--############################################################
-- CREATE INDICES TO SPEED UP QUERIES
CREATE INDEX activity_person_idx ON "Activity" (person);
CREATE INDEX person_household_idx ON "Person" (household);
CREATE INDEX activity_trip_idx ON "Activity" (trip);


--############################################################
-- SAMPLE FROM FULL DATA TO SPEED UP QUERIES and combine all data into one table

create table hh_per as select * from household, person 
where household.household = person.household 
	and household.rowid % 20 = 0;
	
create index per_idx on "hh_per" (person);
	
create table combine as select *, round((end-start)/60) AS TTime, cast(start_time/60 as int) as Start_Time_minutes,
	case when (employment = 4 or employment = 1) and work_hours >= 30 then 3
		when (employment = 4 or employment = 1) then 4
		when (school_enrollment = 3 or school_enrollment = 2) and age > 18 then 7
		when age >= 65 then 6
		when age < 65 and age > 18 then 5
		when age >= 16 and age <= 18 then 0
		when age < 16 and age >= 5 then 1
		when age < 5 then 2
		else 5
	end as person_category
from activity, trip, hh_per 
where activity.trip = trip.trip_id and activity.person = hh_per.person and start_time > 0;

-- Drop extraneous tables
drop table activity;
drop table trip;
drop table person;
drop table household;
drop table hh_per;
drop table path;
drop table path_links;
drop table plan
drop table selection;
drop table traveler;

--############################################################
-- VEHICLE TYPE RESULTS
DROP TABLE IF EXISTS Vehicle_Type_Distribution;
create table Vehicle_Type_Distribution as 
SELECT type, 
	sum(CASE WHEN subtype=0 THEN 1 END) as No_Automation,
	sum(CASE WHEN subtype=1 THEN 1 END) as Automated,
	sum(1) as total
from vehicle
group by type;

DROP TABLE IF EXISTS Automation_WTP_Distribution;
create table Automation_WTP_Distribution as 
select case 
	when parking < 2000 then "<2000"
	when parking < 5000 then "2000-5000"
	when parking < 10000 then "5000-10000"
	when parking >= 10000 then ">10000" end as WTP,
	count(hhold) as count
from vehicle
group by wtp;

-- Drop extraneous vehicle tables
drop table vehicle;
drop table vehicle_type;
drop table automation_type;
drop table Connectivity_Type;
drop table fuel_type;
drop table vehicle_class;
drop table powertrain_type;


CREATE INDEX activity_location_idx ON "combine" (location_id);
CREATE INDEX activity_person_idx ON "combine" (person);
CREATE INDEX person_household_idx ON "combine" (household);
CREATE INDEX activity_trip_idx ON "combine" (trip);
CREATE INDEX person_work_idx ON "combine" (work_location_id);

	
--############################################################
-- ACTIVITY GENERATION RESULTS

DROP TABLE IF EXISTS Activity_with_person;
CREATE TABLE IF NOT EXISTS Activity_with_person As
SELECT 
person_category,
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
from combine
where Start_Time > 62 and trip > 0
GROUP BY 
person_category;



--############################################################
-- GENERATE ACTIVITY TYPE BY MODAL DISTRIBUTION

DROP TABLE IF EXISTS Mode_Distribution_ADULT;
CREATE TABLE Mode_Distribution_ADULT As
SELECT 
	case when type = 'WORK' or type = 'SCHOOL' or type = 'PART_WORK' then 'HBW'
	when origin = location then 'HBO'
	else 'NHB' end as acttype,
	sum(CASE WHEN mode= 'AUTO' THEN 1 END) as Auto,
	sum(CASE WHEN mode= 'HOV' THEN 1 END) as HOV,
	sum(CASE WHEN mode= 'TRANSIT' THEN 1 END) as Transit,
	sum(CASE WHEN mode= 'WALK' THEN 1 END) as Walk,
	sum(CASE WHEN mode= 'BIKE' THEN 1 END) as Bike,
	sum(CASE WHEN mode= 'TAXI' THEN 1 END) as Taxi,
	sum(CASE WHEN mode= 'SCHOOLBUS' THEN 1 END) as Schoolbus,
	sum(1) AS total
FROM 
	combine
WHERE
	Start_Time > 62 and age >= 16
GROUP BY 
	acttype;



--############################################################
-- GENERATE ACTIVITY START TIME DISTRIBUTION
DROP TABLE IF EXISTS Activity_Start_Distribution;
CREATE TABLE IF NOT EXISTS Activity_Start_Distribution As
SELECT 
	cast(start_time/60 as int) as Start_Time_minutes,
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
	combine
WHERE
	trip > 0 and start_time > 62
GROUP BY 
	cast (start_time/60 as int);
	
	
DROP TABLE IF EXISTS Auto_Trip_Start_Distribution;
CREATE TABLE IF NOT EXISTS Auto_Trip_Start_Distribution As
SELECT 
	cast(start/60 as int) as Trip_Start_minutes,
    sum(1) as Total_Trips
FROM 
	combine
WHERE
	Start > 62 and "mode:1" = 0
GROUP BY 
	cast (start/60 as int);
	


	
--############################################################
-- GENERATE TRAVEL TIME BY ACTIVITY TYPE DISTRIBUTION
DROP TABLE IF EXISTS Mode_TTime_Distribution;
CREATE TABLE Mode_TTime_Distribution As
SELECT 
	TTime,
    sum(CASE WHEN mode= 'AUTO' THEN 1 END) as AUTO,
	sum(CASE WHEN mode= 'HOV' THEN 1 END) as HOV,
    sum(CASE WHEN mode= 'TRANSIT' THEN 1 END) as TRANSIT,
	sum(CASE WHEN mode= 'WALK' THEN 1 END) as WALK,
	sum(CASE WHEN mode= 'TAXI' THEN 1 END) as TAXI,
	sum(CASE WHEN mode= 'BIKE' THEN 1 END) as BIKE,
	sum(CASE WHEN mode= 'SCHOOLBUS' THEN 1 END) as SCHOOLBUS,
	sum(1) AS total
FROM 
	combine
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
	combine
GROUP BY 
	TTime;


--############################################################
--Calculate straight-line distance for each activity
CREATE INDEX origin_idx ON "combine" (origin);


DROP TABLE IF EXISTS Activity_With_OD_tmp;
CREATE TABLE Activity_With_OD_tmp As
SELECT combine.*, a.Location.x as Orig_X, a.Location.y as Orig_Y
FROM combine
INNER JOIN a.Location
ON combine_with_od.origin=a.Location.location;

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
	Activity_With_Distance
GROUP BY 
	Dist_rnd;


DROP TABLE IF EXISTS Activity_With_OD_tmp;
DROP TABLE IF EXISTS Activity_With_OD_tmp2;
--DROP TABLE IF EXISTS Activity_With_Distance;


CREATE TABLE Combine_With_OD_tmp As
SELECT combine.*, a.Location.zone as orig_zone, taz_to_county.county as orig_county, a.Location.x as Orig_X, a.Location.y as Orig_Y
FROM combine, a.location, taz_to_county
where combine.origin=a.Location.location and a.location.zone = taz_to_county.taz;


CREATE TABLE Combine_With_OD As
SELECT Combine_With_OD_tmp.*, a.Location.zone as dest_zone, taz_to_county.county as dest_county, a.Location.x as Dest_X,a.Location.y as Dest_Y
FROM Combine_With_OD_tmp, a.location, taz_to_county
where Combine_With_OD_tmp.destination=a.Location.location and a.location.zone = taz_to_county.taz;

CREATE TABLE Activity_TTime_By_County As
SELECT 
	Orig_COunty,
    avg(CASE WHEN type= 'EAT OUT' THEN Dist END) as EAT_OUT,
	avg(CASE WHEN type= 'ERRANDS' THEN Dist END) as ERRANDS,
    avg(CASE WHEN type= 'HEALTHCARE' THEN Dist END) as HEALTHCARE,
	avg(CASE WHEN type= 'HOME' THEN Dist END) as HOME,
    avg(CASE WHEN type= 'LEISURE' THEN Dist END) as LEISURE,
	avg(CASE WHEN type= 'PERSONAL' THEN Dist END) as PERSONAL,
    avg(CASE WHEN type= 'RELIGIOUS-CIVIC' THEN Dist END) as RELIGIOUS,
	avg(CASE WHEN type= 'PICKUP-DROPOFF' THEN Dist END) as PICK_DRIP,
	avg(CASE WHEN type= 'SCHOOL' THEN Dist END) as SCHOOL,
    avg(CASE WHEN type= 'SERVICE' THEN Dist END) as SERVICE,
	avg(CASE WHEN type= 'SHOP-MAJOR' THEN Dist END) as SHOP_MAJOR,
	avg(CASE WHEN type= 'SOCIAL' THEN Dist END) as SOCIAL,
    avg(CASE WHEN type= 'WORK' THEN Dist END) as WORK,
	avg(CASE WHEN type= 'WORK AT HOME' THEN Dist END) as WORK_HOME,
	avg(CASE WHEN type= 'PART_WORK' THEN Dist END) as WORK_PART,
	avg(CASE WHEN type= 'SHOP-OTHER' THEN Dist END) as SHOP_OTHER,
	sum(1) AS total
FROM 
	combine
GROUP BY 
	orig_county;

CREATE TABLE Activity_Count_By_County As
SELECT 
	Orig_COunty,
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
	combine
GROUP BY 
	orig_county;

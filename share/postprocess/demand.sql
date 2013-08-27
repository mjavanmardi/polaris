--############################################################
-- GENERATE TRAVEL TIME DISTRIBUTION

DROP TABLE IF EXISTS TTime_Distribution_tmp;
CREATE TABLE IF NOT EXISTS TTime_Distribution_tmp As
SELECT 
	round((end-start)/60),
	sum(vehicle) AS total
FROM 
	Trip
GROUP BY 
	round((end-start)/60);

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
	sum(CASE WHEN mode= 'AUTO' THEN id/id END) as Auto,
	sum(CASE WHEN mode= 'TRANSIT' THEN id/id END) as Transit,
	sum(id/id) AS total
FROM 
	Activity
GROUP BY 
	type;
	

--############################################################
-- GENERATE ACTIVITY START TIME DISTRIBUTION
DROP TABLE IF EXISTS Activity_Start_Distribution;
CREATE TABLE IF NOT EXISTS Activity_Start_Distribution As
SELECT 
	cast(start_time/60 as int),
        sum(CASE WHEN type= 'EAT OUT' THEN id/id END) as EAT_OUT,
	sum(CASE WHEN type= 'ERRANDS' THEN id/id END) as ERRANDS,
        sum(CASE WHEN type= 'HEALTHCARE' THEN id/id END) as HEALTHCARE,
	sum(CASE WHEN type= 'HOME' THEN id/id END) as HOME,
        sum(CASE WHEN type= 'LEISURE' THEN id/id END) as LEISURE,
	sum(CASE WHEN type= 'PERSONAL' THEN id/id END) as PERSONAL,
        sum(CASE WHEN type= 'RELIGIOUS-CIVIC' THEN id/id END) as RELIGIOUS,
	sum(CASE WHEN type= 'SCHOOL' THEN id/id END) as SCHOOL,
        sum(CASE WHEN type= 'SERVICE' THEN id/id END) as SERVICE,
	sum(CASE WHEN type= 'SHOP-MAJOR' THEN id/id END) as SHOP_MAJOR,
	sum(CASE WHEN type= 'SOCIAL' THEN id/id END) as SOCIAL,
        sum(CASE WHEN type= 'WORK' THEN id/id END) as WORK,
	sum(CASE WHEN type= 'SHOP-OTHER' THEN id/id END) as SHOP_OTHER,
	sum(id/id) AS total
FROM 
	Activity
GROUP BY 
	cast (start_time/60 as int);
	
	
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

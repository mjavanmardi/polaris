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
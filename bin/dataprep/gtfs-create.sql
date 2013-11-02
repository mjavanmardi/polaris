-- select DiscardGeometryColumn('gtfs_stops', 'location');
drop table if exists gtfs_agency;
drop table if exists gtfs_stops;
drop table if exists gtfs_routes;
drop table if exists gtfs_route_types;
drop table if exists gtfs_directions;
drop table if exists gtfs_trips;
drop table if exists gtfs_stop_times;
drop table if exists gtfs_calendar;
drop table if exists gtfs_pickup_dropoff_types;

-- drop table if exists gtfs_calendar_dates;
-- drop table if exists fare_attributes;
-- drop table if exists fare_rules;
-- drop table if exists shapes;
-- drop table if exists frequencies;
-- drop table if exists transfer_types;
-- drop table if exists transfers;
-- drop table if exists feed_info;
-- drop table if exists payment_methods;



PRAGMA foreign_keys = OFF;

begin;

create table gtfs_agency (
  agency_id    text PRIMARY KEY,
  agency_name  text NOT NULL,
  agency_url   text NOT NULL,
  agency_timezone    text NOT NULL,
  agency_lang  text,
  agency_phone text,
  agency_fare_url text
);
create table gtfs_stops (
  stop_id    text PRIMARY KEY,
  stop_code  text, 
  stop_name  text NOT NULL, 
  stop_desc  text,
  stop_lat   double precision,
  stop_lon   double precision,
  zone_id    int,
  stop_url   text,
  location_type int,
  parent_station text,
  stop_timezone text,
  wheelchair_boarding integer,
  FOREIGN KEY (parent_station) REFERENCES gtfs_stops(stop_id)
);

create table gtfs_route_types (
  route_type int PRIMARY KEY,
  description text
);
insert into gtfs_route_types (route_type, description) values (0, 'Street Level Rail');
insert into gtfs_route_types (route_type, description) values (1, 'Underground Rail');
insert into gtfs_route_types (route_type, description) values (2, 'Intercity Rail');
insert into gtfs_route_types (route_type, description) values (3, 'Bus');
insert into gtfs_route_types (route_type, description) values (4, 'Ferry');
insert into gtfs_route_types (route_type, description) values (5, 'Cable Car');
insert into gtfs_route_types (route_type, description) values (6, 'Suspended Car');
insert into gtfs_route_types (route_type, description) values (7, 'Steep Incline Mode');
create table gtfs_routes (
  route_id    text PRIMARY KEY,
  gtfs_agency_id   text , --REFERENCES gtfs_agency(gtfs_agency_id),
  route_short_name  text DEFAULT '',
  route_long_name   text DEFAULT '',
  route_desc  text,
  route_type  int , --REFERENCES gtfs_route_types(route_type),
  route_url   text,
  route_color text,
  route_text_color  text,
  FOREIGN KEY (gtfs_agency_id) REFERENCES gtfs_agency(agency_id),
  FOREIGN KEY (route_type) REFERENCES gtfs_route_types(route_type)
);
create table gtfs_directions (
  direction_id int PRIMARY KEY,
  description text
);
insert into gtfs_directions (direction_id, description) values (0,'This way');
insert into gtfs_directions (direction_id, description) values (1,'That way');
create table gtfs_trips (
  route_id text , --REFERENCES gtfs_routes(route_id),
  service_id    text , --REFERENCES gtfs_calendar(service_id),
  trip_id text PRIMARY KEY,
  trip_headsign text,
  direction_id  int, --REFERENCES gtfs_directions(direction_id),
  block_id text,
  shape_id text,
  wheelchair_accessible int,
  FOREIGN KEY (route_id) REFERENCES gtfs_routes(route_id),
  FOREIGN KEY (service_id) REFERENCES gtfs_calendar(service_id),
  FOREIGN KEY (direction_id) REFERENCES gtfs_directions(direction_id)
);
create table gtfs_pickup_dropoff_types (
  type_id int PRIMARY KEY,
  description text
);
insert into gtfs_pickup_dropoff_types (type_id, description) values (0,'Regularly Scheduled');
insert into gtfs_pickup_dropoff_types (type_id, description) values (1,'Not available');
insert into gtfs_pickup_dropoff_types (type_id, description) values (2,'Phone arrangement only');
insert into gtfs_pickup_dropoff_types (type_id, description) values (3,'Driver arrangement only');

create table gtfs_stop_times (
  trip_id text , --REFERENCES gtfs_trips(trip_id),
  arrival_time text, -- CHECK (arrival_time LIKE '__:__:__'),
  departure_time text, -- CHECK (departure_time LIKE '__:__:__'),
  stop_id text , --REFERENCES gtfs_stops(stop_id),
  stop_sequence int NOT NULL, 
  stop_headsign text,
  pickup_type   int , --REFERENCES gtfs_pickup_dropoff_types(type_id),
  drop_off_type int , --REFERENCES gtfs_pickup_dropoff_types(type_id),
  shape_dist_traveled double precision,
  --convinience columns, 
  arrival_time_seconds int, 
  departure_time_seconds int,
  FOREIGN KEY (trip_id) REFERENCES gtfs_trips(trip_id),
  FOREIGN KEY (stop_id) REFERENCES gtfs_stops(stop_id),
  FOREIGN KEY (pickup_type) REFERENCES gtfs_pickup_dropoff_types(type_id),
  FOREIGN KEY (drop_off_type) REFERENCES gtfs_pickup_dropoff_types(type_id),
  CHECK (arrival_time LIKE '__:__:__'),
  CHECK (departure_time LIKE '__:__:__')
);
create table gtfs_calendar (
  service_id   text PRIMARY KEY,
  monday int NOT NULL, 
  tuesday int NOT NULL, 
  wednesday    int NOT NULL, 
  thursday     int NOT NULL, 
  friday int NOT NULL, 
  saturday     int NOT NULL, 
  sunday int NOT NULL, 
  start_date   date NOT NULL, 
  end_date     date NOT NULL
);

-- create table gtfs_calendar_dates (
  -- service_id     text NOT NULL, --REFERENCES gtfs_calendar(service_id),
  -- date     date NOT NULL, 
  -- exception_type int  NOT NULL,
  -- FOREIGN KEY (service_id) REFERENCES gtfs_calendar(service_id)
-- );
-- create table payment_methods (
  -- payment_method int PRIMARY KEY,
  -- description text
-- );
-- insert into payment_methods (payment_method, description) values (0,'On Board');
-- insert into payment_methods (payment_method, description) values (1,'Prepay');
-- create table fare_attributes (
  -- fare_id     text PRIMARY KEY,
  -- price double precision NOT NULL, 
  -- currency_type     text NOT NULL, 
  -- payment_method    int , --REFERENCES payment_methods,
  -- transfers   int,
  -- transfer_duration int,
  -- gtfs_agency_id text,  --REFERENCES gtfs_agency(gtfs_agency_id),
  -- FOREIGN KEY (payment_method) REFERENCES payment_methods(payment_method),
  -- FOREIGN KEY (gtfs_agency_id) REFERENCES gtfs_agency(gtfs_agency_id)
-- );
-- create table fare_rules (
  -- fare_id     text , --REFERENCES fare_attributes(fare_id),
  -- route_id    text , --REFERENCES gtfs_routes(route_id),
  -- origin_id   int ,
  -- destination_id int ,
  -- contains_id int, 
  -- FOREIGN KEY (fare_id) REFERENCES fare_attributes(fare_id),
  -- FOREIGN KEY (route_id) REFERENCES gtfs_routes(route_id)
-- );
-- create table shapes (
  -- shape_id    text NOT NULL, 
  -- shape_pt_lat double precision NOT NULL, 
  -- shape_pt_lon double precision NOT NULL, 
  -- shape_pt_sequence int NOT NULL, 
  -- shape_dist_traveled double precision
-- );


-- create index arr_time_index on gtfs_stop_times(arrival_time_seconds);
-- create index dep_time_index on gtfs_stop_times(departure_time_seconds);
-- create index stop_seq_index on gtfs_stop_times(trip_id,stop_sequence);
-- -- select AddGeometryColumn( 'shapes', 'shape', #{WGS84_LATLONG_EPSG}, 'LINESTRING', 2 );
-- create table frequencies (
  -- trip_id     text , --REFERENCES gtfs_trips(trip_id),
  -- start_time  text NOT NULL, 
  -- end_time    text NOT NULL, 
  -- headway_secs int NOT NULL, --NOT NULL
  -- start_time_seconds int,
  -- end_time_seconds int,
  -- FOREIGN KEY (trip_id) REFERENCES gtfs_trips(trip_id)
-- );
-- create table transfer_types (
  -- transfer_type int PRIMARY KEY,
  -- description text
-- );
-- insert into transfer_types (transfer_type, description) 
       -- values (0,'Preferred transfer point');
-- insert into transfer_types (transfer_type, description) 
       -- values (1,'Designated transfer point');
-- insert into transfer_types (transfer_type, description) 
       -- values (2,'Transfer possible with min_transfer_time window');
-- insert into transfer_types (transfer_type, description) 
       -- values (3,'Transfers forbidden');
-- create table transfers (
  -- from_stop_id text, --REFERENCES gtfs_stops(stop_id)
  -- to_stop_id text, --REFERENCES gtfs_stops(stop_id)
  -- transfer_type int, --REFERENCES transfer_types(transfer_type)
  -- min_transfer_time int,
  -- from_route_id text, --REFERENCES gtfs_routes(route_id)
  -- to_route_id text, --REFERENCES gtfs_routes(route_id)
  -- service_id text, --REFERENCES gtfs_calendar(service_id) ?
  -- FOREIGN KEY (from_stop_id) REFERENCES gtfs_stops(stop_id),

  -- FOREIGN KEY (to_stop_id) REFERENCES gtfs_stops(stop_id),
  -- FOREIGN KEY (transfer_type) REFERENCES transfer_types(transfer_type),
  -- FOREIGN KEY (from_route_id) REFERENCES gtfs_routes(route_id),
  -- FOREIGN KEY (to_route_id) REFERENCES gtfs_routes(route_id)
-- );
-- create table feed_info (
  -- feed_publisher_name text,
  -- feed_publisher_url text,
  -- feed_timezone text,
  -- feed_lang text,
  -- feed_version text
-- );



commit;
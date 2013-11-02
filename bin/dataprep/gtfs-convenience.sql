--convenience updates
PRAGMA foreign_keys = OFF;
update gtfs_stops set parent_station=NULL;
select AddGeometryColumn( 'gtfs_stops', 'location', 4326, 'POINT', 'XY' );
update gtfs_stops set location=MakePoint(stop_lon, stop_lat, 4326);
update gtfs_stop_times set arrival_time_seconds=60*strftime('%H',arrival_time) + strftime('%M',arrival_time);
update gtfs_stop_times set departure_time_seconds=60*strftime('%H',departure_time) + strftime('%M',departure_time);
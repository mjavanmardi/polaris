PRAGMA synchronous = OFF;
PRAGMA journal_mode = MEMORY;
PRAGMA foreign_keys=0;

delete from ZoneLandUse;
.separator ,
.import z:/polaris/interchange/Subarea/ZoneLandUse_CBD.csv ZoneLandUse
delete from Location where zone not in (select zone from ZoneLandUse);
delete from Zone where zone not in (select zone from ZoneLandUse);
delete from LocationData where location not in (select location from location);


delete from link where link in (select link  from DisjointLink);

delete from Location where zone not in (select zone from ZoneLandUse);
delete from Zone where zone not in (select zone from ZoneLandUse);
delete from LocationData where location not in (select location from location);

delete from Location where link in (select link from DisjointLink);

delete from Node where Node not in (select node_a from  Link) and Node not in (select node_b from  Link);

delete from Access where Link not in (select link from  Link);
delete from Lane_Use where link not in (select link from  Link);

delete from Pocket where link not in (select link from  Link);
delete from Parking where link not in (select link from  Link);


delete from Connection where link in (select link from  DisjointLink);
delete from Connection where to_link in (select link from  DisjointLink);


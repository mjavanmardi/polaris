PRAGMA synchronous = OFF;
PRAGMA journal_mode = MEMORY;
PRAGMA foreign_keys = ON;

drop table if exists Subarea;
drop table if exists DisjointLink;
drop table if exists DisjointZone;

.loadshp "Z:/POLARIS/interchange/subarea/Zone07_CMAP_Sub_Area_Project" Subarea cp1252 26916;
create table DisjointLink as 
select Link, node_a, node_b, GEO  from Subarea, Link where Disjoint(Link.GEO, Subarea.Geometry) 
union 
select Link, node_a, node_b, GEO  from Link where use like "rail";
/* create table DisjointZone as select Zone, GEO  from Subarea, Zone where Disjoint(Zone.GEO, Subarea.Geometry); */


create index if not exists dl_ind on DisjointLink(Link);
/* create index if not exists link_ind on Link(Link);
create index if not exists node_ind on DisjointLink(Link); */

select DisableSpatialIndex("Node", "GEO"); 
select DisableSpatialIndex("Link", "GEO"); 
select DisableSpatialIndex("Zone", "GEO"); 




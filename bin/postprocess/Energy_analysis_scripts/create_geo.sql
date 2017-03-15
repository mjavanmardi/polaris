select InitSpatialMetadata();

create table result_points as select * from results;
select addgeometrycolumn('results','geo',(SELECT srid from a.geometry_columns where f_table_name = 'link'),'linestring',2);
update results set geo = (select geo from a.link where a.link.link = results.link)
where exists(select * from a.link where a.link.link = results.link);

CREATE UNIQUE INDEX link_idx5 ON "Result_points" (link);
	
select AddGeometryColumn('result_points', 'geo', (SELECT srid from a.geometry_columns where f_table_name = 'link'), 'MULTIPOINTM', 'XYM');

update result_points set geo = (select 
	case
		when results.length > 1000 then (Line_Interpolate_Equidistant_Points(results.geo, 1000.0))
		else (Line_Interpolate_Equidistant_Points(results.geo, results.length/2.0+1.0))
	end
from results where results.link = result_points.link)
where exists (select * from results where results.link = result_points.link);
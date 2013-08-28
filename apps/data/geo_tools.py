import sqlite3
import sys 
import os
import struct

# conn is an sqlite3 object with spatialite extensiotn loaded
def match(conn, rt, pt, srid=26916, delta= 300):
    RefTable, RefTableGeo, RefTableKey = rt
    PointTable, PointTableGeo, PointTableKey = pt
    c = conn.cursor()
    #so that row elements can be fetched as dictionary
    conn.row_factory = sqlite3.Row
    #geometry column of the the point table
    rows = c.execute("select "+PointTableKey+", AsBinary(Transform("+PointTableGeo+", ?)) from "+PointTable,(srid,)).fetchall()
    #rows = c.execute("select "+PointTableKey+", AsBinary(Transform(\'Geo\', ?)) from "+PointTable,(srid,))
    MbrCacheName = 'CACHE_' + RefTable.upper () + '_' + RefTableGeo.upper ()
    res = []
    for point in rows:
        id, geo = point
        if geo==None:
            continue
        EndianType, GeoType, x, y = struct.unpack ( '=B1I2d', geo )
        #select features from the reference table that are within delta of (x,y)
        features_within = c.execute (
                        "select "+RefTableKey+" from "+RefTable+" where Intersects (BuildMbr( ?, ?, ?, ? ), "+RefTableGeo+") and ROWID in (select ROWID from "+MbrCacheName+" where MBR = FilterMbrIntersects ( ?, ?, ?, ? ));",
                        (x+delta, y+delta, x-delta, y-delta, x+delta, y+delta, x-delta, y-delta, ) ).fetchall()
        #nothing has been matched
        if len(features_within)==0:
            res.append((id, None))
            continue
        dist = 99999;
        match_feature = -1;
        for feature in features_within: 
            d = c.execute("select Distance("+RefTable+"."+RefTableGeo+", Transform("+PointTable+"."+PointTableGeo+", ?)) from "+RefTable+", "+PointTable+" where "+RefTable+"."+RefTableKey+"=? and "+PointTable+"."+PointTableKey+"=?;",(srid,feature[0], id)).fetchone()[0]
            if d < dist:
                dist = d
                match_feature = feature[0]
        res.append((id, match_feature))
    return res
def delete_table_layer(conn, table_name, geo_field):
    #conn.execute("select DiscardGeometryColumn(%s, %s);"%(table_name, geo_field))
    try:
        conn.execute("select DiscardGeometryColumn(\'%s\', \'%s\');"%(table_name, geo_field))
    except sqlite3.OperationalError:
        pass
    conn.execute("drop table if exists %s;"%(table_name))


                
            
            
        
        
    
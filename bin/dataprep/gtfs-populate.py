import sqlite3
import sys
import csv

allowed_columns = {}
allowed_columns['gtfs_agency'] = ['agency_id','agency_name','agency_url','agency_timezone','agency_lang','agency_phone','agency_phone','agency_fare_url']
allowed_columns['gtfs_stops'] = ['stop_id','stop_name','stop_desc','stop_lat','stop_lon','zone_id','stop_url','stop_code','location_type','parent_station','stop_timezone','wheelchair_boarding']
allowed_columns['gtfs_routes'] = ['route_id','agency_id','route_short_name','route_long_name','route_desc','route_type','route_url','route_color','route_text_color']
allowed_columns['gtfs_trips'] = ['route_id','service_id','trip_id','trip_headsign','direction_id','block_id','shape_id','wheelchair_accessible']
allowed_columns['gtfs_stop_times'] = ['trip_id','arrival_time','departure_time','stop_id','stop_sequence','stop_headsign','pickup_type', 'drop_off_type','shape_dist_traveled']
allowed_columns['gtfs_calendar'] = ['service_id','monday','tuesday','wednesday','thursday','friday','saturday','sunday','start_date','end_date']
# allowed_columns[''] = ['','','','','','']
# allowed_columns[''] = ['','','','','','']
# allowed_columns[''] = ['','','','','','']
def import_file(fname, tablename,conn):
    try:
        f = open(fname,'r');
    except:
        print "file %s doesn't exist" % fname
        return

    print "Importing %s"%tablename
    reader = csv.reader(f, dialect=csv.excel)
    header = reader.next()    
    
    n = len(header)
    good_indices = []
    for i in range(n):
        if header[i] in allowed_columns[tablename]:
            good_indices.append(i)
        else:
            print "Warning column %s in table %s is not supported"%(header[i] , tablename)
    
    n_good = len(good_indices)
    header_good = [header[i] for i in good_indices]
    cols = ",".join(header_good);
    func = lambda s: str(s).replace(str(s),"'"+str(s)+"'") 
    count = 0
    for row in reader:     
        count+=1
        if (count % 100000 == 0):
            print count
        row_good = [row[i] for i in good_indices] 
        row = map(func,row_good)
        try:
            conn.execute("insert into %s (%s) values (%s)"%(tablename,cols,','.join(['?']*n_good)),row_good )
        except sqlite3.Error as e:
            print "An error occurred:", e.args[0]
            print "insert into %s (%s) values (%s)"%(tablename,cols,','.join(['?']*n_good))
            sys.exit()
    conn.commit()
    
if __name__ == "__main__":
    if len(sys.argv) < 3:
        print "Usage: %s db_path gtfs_path"%sys.argv[0]
        sys.exit()
    db_path = sys.argv[1]
    gtfs_path = sys.argv[2]
    conn = sqlite3.connect(db_path)
    #enable spatial extension
    # conn.enable_load_extension(True)
    # conn.execute("SELECT load_extension('libspatialite-4.dll')");    
    conn.execute("PRAGMA synchronous = OFF");
    conn.execute("PRAGMA journal_mode = MEMORY");
    conn.execute("PRAGMA foreign_keys = OFF;");
    fnames = [
      "gtfs_agency" ,
      "gtfs_stops" ,
      "gtfs_routes" ,
      "gtfs_trips",
      "gtfs_stop_times",
      "gtfs_calendar" ,
      # "calendar_dates" ,
      # "fare_attributes" ,
      # "fare_rules" ,
      # "shapes" ,
      # "stop_times" ,
      # "frequencies" ,
      # "transfers" ,
      # "feed_info" ,
      ];
    for item in fnames:
        file_name = item.replace("gtfs_","")
        import_file(gtfs_path+'/'+file_name+'.txt', item, conn)
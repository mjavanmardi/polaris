import sys,sqlite3, os


def PopulateSensors(supply_db):
    count = 0
    conn2 = sqlite3.connect(supply_db)
    conn2.execute('delete from Fixed_Sensor')
    conn2.commit()
    rows = conn2.execute('select link, length, lanes_ab, lanes_ba, speed_ab, speed_ba from Link where type not in (\"OTHER\", \"WALKWAY\", \"RAMP\", \"LIGHTRAIL\")')
    for row in rows:        
        link, length, lanes_ab, lanes_ba, speed_ab, speed_ba = row
        if lanes_ab>0 and 12*speed_ab < length:
            temp = conn2.execute("INSERT into Fixed_Sensor values (?,?,?,?,?,?,?)", (count, link, 0, 0, 0.1, 300, ""))
            count +=1
        if lanes_ba>0  and 12*speed_ba < length:
            temp = conn2.execute("INSERT into Fixed_Sensor values (?,?,?,?,?,?,?)", (count, link, 1, 0, 0.1, 300, ""))            
            count +=1
        print "Inserted ", link
    conn2.commit()

    


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print "Usage: %s %s %s"%( sys.argv[0], "<db_prefix>")
        sys.exit()
    db_prefix = sys.argv[1]
    PopulateSensors(db_prefix+"-Supply.sqlite")


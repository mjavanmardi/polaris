import sys,sqlite3, os


def PopulateSensors(supply_db, gcm_folder):
    count = 0
    conn1 = sqlite3.connect(gcm_folder + "/DeviceLink.sqlite")
    conn2 = sqlite3.connect(supply_db)
    conn2.execute('delete from Fixed_Sensor')
    conn2.commit()
    rows = conn1.execute('select device_id, link_id, link_dir from DeviceLink')
    for row in rows:
        count +=1
        device_id, link_id, link_dir = row
        if link_id == -1:
            continue
        temp = conn2.execute("INSERT into Fixed_Sensor values (?,?,?,?,?,?,?)", (count, link_id, link_dir, 0, 0.1, 300, device_id))
        print "Inserted ", device_id, "on link ", link_id
    conn2.commit()

    


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print "Usage: %s %s %s"%( sys.argv[0], "<db_prefix>", "<gcm_folder>")
        sys.exit()
    db_prefix = sys.argv[1]
    gcm_folder = sys.argv[2]
    PopulateSensors(db_prefix+"-Supply.sqlite", gcm_folder)


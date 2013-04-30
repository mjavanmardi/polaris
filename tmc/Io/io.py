import sqlite3
import os
from collections import namedtuple
class InputContext:
    def __init__(self, _db_path):
        self.db_path = _db_path
        path = self.makePath("Supply")
        if not os.path.exists(path):
            raise ValueError("The database %s does not exist"%(path))
        self.conn = sqlite3.connect(path)
        self.c = self.conn.cursor()
        self.c.execute("attach database ? as Network_Event", (self.makePath("Network_Event"),))
        self.event = namedtuple('event', 'id event_id location_x location_y reporter confidence_level time_detected expected_duraition expired start_time end_time note links values')
        self.__event_id = None
        self.event_links = None        
    def makePath(self, schema):
        return self.db_path + "-" + schema + ".sqlite"
    # Initializes an instance of event
    def initEvent(self, event_type):
        id = self.c.execute("select id from Network_Event where name = ?", (event_type,)).fetchone()[0] 
        inst_id = self.c.execute("select seq from Network_Event.sqlite_sequence where name = ?", ("Event_Instance",)).fetchone()[0] +1
        self.event = self.event(inst_id, id, 0,0, "python_script", -1, -1, -1, -1, -1, -1, "Added by the io.py", list(), dict())
        res = self.c.execute("select value from Network_Event_keys where object_id = ?", (self.event.event_id,)).fetchall()
        for item in res:
            self.event.values[item[0]] = 0
    def newType(self, id, type):
        self.c.execute("insert into Network_Event values (?,?,?)", (id, type, ""))
        self.conn.commit()
    def replaceType(self, id, type):
        self.c.execute("delete from Network_Event where id=?",(id,))
        self.newType(id, type)
    # Writes the event instance attributes to the corresponding dab tables
    def persist_event(self):        
        self.c.execute("insert into Event_Instance values (?,?,?,?,?,?,?,?,?,?,?,?)", tuple(list(self.event)[:len(self.event)-2]))
        index = 0
        for key in self.event.values:
            value_id = self.c.execute("select seq from Network_Event.sqlite_sequence where name = ?", ("Event_Instance_Value",)).fetchone()[0] +1
            self.c.execute("insert into Event_Instance_Value values (?,?,?)", (value_id, key, self.event.values[key]))
            self.c.execute("insert into Event_Instance_values values (?,?,?)", (self.event.id, index, value_id))
            index += 1
        index = 0
        for item in self.event.links:
            self.c.execute("insert into Event_Instance_links values (?,?,?)", (self.event.id, index, item))
        self.conn.commit()
            
        
        
        
        
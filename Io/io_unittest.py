from io import *
import unittest

class TestInputContext(unittest.TestCase):
    def test_constactor(self):
        with self.assertRaises(ValueError):
            InputContext("Z:\POLARIS\interchange\with_its\lalal")
    def test_constactor1(self):
        a = InputContext("Z:\POLARIS\interchange\with_its\chicago")
        self.assertIsNotNone(a.conn)
        self.assertIsNotNone(a.c)
    def test_constactor2(self):
        a = InputContext("Z:\POLARIS\interchange\with_its\chicago")
        temp = a.c.execute("select * from Network_Event")
        self.assertIsNotNone(temp, 0)
    def test_initEvent(self):
        context = InputContext("Z:\POLARIS\interchange\with_its\chicago")
        context.initEvent("Weather")
        self.assertEqual(len(context.event.values),9)       
    def test_newType(self):
        context = InputContext("Z:\POLARIS\ITS\empty")
        context.replaceType(10, "ney type")
        res = context.c.execute("select * from Network_Event where id=?",(10,))
        self.assertIsNotNone(res)
        
    def test_persist(self):
        context = InputContext("Z:\POLARIS\interchange\with_its\chicago")
        context.initEvent("Weather")
        context.event = context.event._replace(reporter = "Some reporter")
        context.event = context.event._replace(confidence_level = 5)
        context.event = context.event._replace(time_detected = -1) #in seconds
        context.event = context.event._replace(expected_duraition = 60*60*2) #in seconds
        context.event = context.event._replace(start_time = 60*60*6) #in seconds
        context.event = context.event._replace(end_time = 60*60*8) #in seconds
        context.event = context.event._replace(expired = 0) #0 = False, 1 = True
        context.event.links.append(1)       
        context.event.values["type"] = "rain"
        context.event.values["precipm"] = 10
        context.event.values["vism"] = 20
        context.event.values["wind"] = 10
        context.event.values["asphalt"] = 2
        #
        context.persist_event()
        
        res = context.c.execute("select * from Event_Instance where id=?",(context.event.id,))
        self.assertIsNotNone(res)        
        res = context.c.execute("select * from Event_Instance_values where object_id=?",(context.event.id,))
        self.assertIsNotNone(res)        
        res = context.c.execute("select * from Event_Instance_links where object_id=?",(context.event.id,))
        self.assertIsNotNone(res)        
        
        
        
        
        
        
        
if __name__ == '__main__':
    unittest.main()
#include <Io\Io.h>
#include <vector>

void PopulateEvent(std::string db_path)
{
	using namespace polaris::io;
	using namespace std;
	auto_ptr<odb::database> db = open_sqlite_database(db_path);
	odb::transaction t(db->begin());
	db->execute("delete from Network_Event;");
	db->execute("delete from Event_Instance;");
	db->execute("delete from Event_Instance_Value;");
	db->execute("delete from Event_Instance_Values;");
	db->execute("delete from Event_Key;");
	db->execute("delete from Network_Event_Keys;");
	std::map<int, shape_geometry> point_map;
	vector<int> links;
	shape_geometry point;

	shared_ptr<Network_Event> evnt(nullptr);
	shared_ptr<Event_Key> evnt_key(nullptr);
	shared_ptr<Event_Instance> instance(nullptr);
	shared_ptr<Event_Instance_Value> value(nullptr);
	try
	{
		evnt.reset(new Network_Event(1, "Weather", "")); 

		point = GetCountyPoint(db_path, "Lake").begin()->second;
		links = GetLinksInsideCounty(db_path, "Lake");
		instance.reset(new Event_Instance(0, links, (float)point.x, (float)point.y, "Database", "5",-1, 2*3600, false, 9*3600, 11*3600, "Snow in the lake county"));  evnt->setInstanc(instance); instance->setEvent(evnt); 
		

		evnt_key.reset(new Event_Key("type", "string", "rain|snow|fog|high_temperature", true, ""));instance->setEvent(evnt);  db->persist(evnt_key); evnt->setKey(evnt_key);
		value.reset(new Event_Instance_Value(0, "snow"));   value->setKey(evnt_key); instance->setValu(value); db->persist(value);

		evnt_key.reset(new Event_Key("precipm", "int", "0..1000", false, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		evnt_key.reset(new Event_Key("snowdepthm", "int", "0..1000", false, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		value.reset(new Event_Instance_Value(0, "0.2")); value->setKey(evnt_key); instance->setValu(value); instance->setValu(value);  db->persist(value); 

		evnt_key.reset(new Event_Key("vism", "int", "0..1000", false, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		value.reset(new Event_Instance_Value(0, "3"));   value->setKey(evnt_key); instance->setValu(value); instance->setValu(value); db->persist(value);

		evnt_key.reset(new Event_Key("temperature", "int", "0..1000", false, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		evnt_key.reset(new Event_Key("zip", "int", "0..99999", false, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		evnt_key.reset(new Event_Key("county", "string", "", false, "Name of the county")); db->persist(evnt_key); evnt->setKey(evnt_key);
		value.reset(new Event_Instance_Value(0, "Lake"));   value->setKey(evnt_key); instance->setValu(value); instance->setValu(value); db->persist(value);
		db->persist(instance);
		db->persist(evnt);
		
		
		evnt.reset(new Network_Event(2, "Accident", "")); 
		point_map = GetLinkPoint(db_path, 15730);
		point = point_map.begin()->second;
		links.clear();
		links.push_back(15730);
		instance.reset(new Event_Instance(0, links, (float)point.x, (float)point.y, "Database", "5",-1, -1, false, 10*3600, 11*3600, "Accident on I-55"));  evnt->setInstanc(instance); instance->setEvent(evnt);

		evnt_key.reset(new Event_Key("lanes", "int", "0..20", true, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		value.reset(new Event_Instance_Value(0, "2"));   value->setKey(evnt_key); instance->setValu(value); db->persist(value);
		evnt_key.reset(new Event_Key("severity", "int", "0..5", true, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		value.reset(new Event_Instance_Value(0, "3"));   value->setKey(evnt_key); instance->setValu(value); db->persist(value);
		evnt_key.reset(new Event_Key("vehicles", "int", "0..20", true, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		value.reset(new Event_Instance_Value(0, "3"));   value->setKey(evnt_key); instance->setValu(value); db->persist(value);
		evnt_key.reset(new Event_Key("accident_type", "string", "collision|non-collision", true, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		value.reset(new Event_Instance_Value(0, "collision"));   value->setKey(evnt_key); instance->setValu(value); db->persist(value);
		evnt_key.reset(new Event_Key("vehicle_type", "string", "passanger|truck|bus", false, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		value.reset(new Event_Instance_Value(0, "passanger"));   value->setKey(evnt_key); instance->setValu(value); db->persist(value);
		evnt_key.reset(new Event_Key("injury", "string", "disabling|evident|fatality", false, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		value.reset(new Event_Instance_Value(0, "fatality"));   value->setKey(evnt_key); instance->setValu(value); db->persist(value);
		db->persist(instance);
		db->persist(evnt);

		evnt.reset(new Network_Event(3, "Congestion", "")); 
		links.clear();
		links.push_back(15730);
		links.push_back(15693);
		links.push_back(14392);
		instance.reset(new Event_Instance(0, links, (float)point.x, (float)point.y, "Database", "5",-1, -1, false, 10.5*3600, 11.5*3600, "Conjestion caused by an accident on I-55"));  evnt->setInstanc(instance); instance->setEvent(evnt); 
		evnt_key.reset(new Event_Key("travel_time", "int", "0..3600", true, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		value.reset(new Event_Instance_Value(0, "1200"));   value->setKey(evnt_key); instance->setValu(value); db->persist(value);
		evnt_key.reset(new Event_Key("slowdown_index", "int", "0..1", false, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		value.reset(new Event_Instance_Value(0, "0.7"));   value->setKey(evnt_key); instance->setValu(value); db->persist(value);
		db->persist(instance);
		db->persist(evnt);
		
		evnt.reset(new Network_Event(4, "Lane Closure", "")); 
		links.clear();
		links.push_back(14936);
		point = GetLinkPoint(db_path, 14936).begin()->second;
		instance.reset(new Event_Instance(0, links, (float)point.x, (float)point.y, "Database", "5",-1, -1, false, 0*3600, 14*3600, "Lame closure due to construction on I-55"));  evnt->setInstanc(instance); instance->setEvent(evnt); 
		evnt_key.reset(new Event_Key("lanes_closed", "int", "0..20", true, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		value.reset(new Event_Instance_Value(0, "2"));   value->setKey(evnt_key); instance->setValu(value); db->persist(value);
		db->persist(instance);
		db->persist(evnt);

		evnt.reset(new Network_Event(5, "Travel Time", "")); 
		evnt_key.reset(new Event_Key("travel_time_to_landmark", "int", "0..3600", true, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		db->persist(evnt);
	}
	catch (const odb::exception& e)
	{
		cout << "Exception while populating Events in PopulateEvent function \n";
		cout << e.what() << "\n";
	}
	try
	{
		t.commit();
	}
	catch (const odb::exception& e)
	{
		cout << "Exception while comitting Event in PopulateEvent function \n";
		cout << e.what() << "\n";
	}

}

#include <Io\Io.h>

void PopulateEventInstance(std::string db_path)
{
	using namespace polaris::io;
	using namespace std;
	auto_ptr<odb::database> db = open_sqlite_database(db_path);
	odb::transaction t(db->begin());
	db->execute("delete from Event_Instance;");
	db->execute("delete from Event_Instance_Value;");

	shared_ptr<Event_Instance> inst(nullptr);
	shared_ptr<Event_Instance_Value> inst_value(nullptr);
	try
	{
		evnt.reset(new Network_Event(1, "Weather", "")); db->persist(evnt);
		evnt_key.reset(new Event_Key("type", "string", "rain|snow|fog|high_temperature", true, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		evnt_key.reset(new Event_Key("precipm", "int", "0..1000", false, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		evnt_key.reset(new Event_Key("snowdepthm", "int", "0..1000", false, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		evnt_key.reset(new Event_Key("vism", "int", "0..1000", false, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		evnt_key.reset(new Event_Key("temperature", "int", "0..1000", false, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		
		
		
		evnt.reset(new Network_Event(2, "Accident", "")); db->persist(evnt);
		evnt_key.reset(new Event_Key("lanes", "int", "0..20", true, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		evnt_key.reset(new Event_Key("severity", "int", "0..5", true, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		evnt_key.reset(new Event_Key("vehicles", "int", "0..20", true, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		evnt_key.reset(new Event_Key("accident_type", "string", "collision|non-collision", true, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		evnt_key.reset(new Event_Key("vehicle_type", "string", "passanger|truck|bus", false, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		evnt_key.reset(new Event_Key("injury", "string", "disabling|evident|fatality", false, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		evnt.reset(new Network_Event(3, "Congestion", "")); db->persist(evnt);
		evnt_key.reset(new Event_Key("travel_time", "int", "0..3600", true, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		evnt_key.reset(new Event_Key("slowdown_index", "int", "0..1", false, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		evnt.reset(new Network_Event(4, "Lane Closure", "")); db->persist(evnt);
		evnt_key.reset(new Event_Key("lanes_closed", "int", "0..20", true, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
		evnt.reset(new Network_Event(5, "Travel Time", "")); db->persist(evnt);
		evnt_key.reset(new Event_Key("travel_time_to_landmark", "int", "0..3600", true, "")); db->persist(evnt_key); evnt->setKey(evnt_key);
	}
	catch (const odb::exception& e)
	{
		cout << "Exception while populating Events in PopulateEventInstance function \n";
		cout << e.what() << "\n";
	}
	try
	{
		t.commit();
	}
	catch (const odb::exception& e)
	{
		cout << "Exception while comitting Event in PopulateEventInstance function \n";
		cout << e.what() << "\n";
	}

}

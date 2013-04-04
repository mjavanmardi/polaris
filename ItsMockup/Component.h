#include <Io\Io.h>

void PopulateComponent(std::string db_path)
{
	using namespace polaris::io;
	auto_ptr<odb::database> db = open_sqlite_database(db_path);
	
	shared_ptr<Component>     co (nullptr);
	shared_ptr<Component>     co1 (nullptr);
	shared_ptr<Action>        ac (nullptr);
	shared_ptr<Action_Key>    ak (nullptr);
	shared_ptr<Component_Key> ck (nullptr);

	//odb::connection_ptr c (db->connection ());
	//odb::transaction del_t (db->begin());
	//try 
	//{
	//	c->execute("delete from Component_Key;");
	//	c->execute("delete from Action_Key;");
	//	c->execute("delete from Action;");
	//	c->execute("delete from Component;");
	//}
	//catch (const odb::exception& e)
	//{
	//	std::cout << "Exception while deleting Its tables\n";
	//	std::cout << e.what() << "\n";
	//}
	//del_t.commit();

	odb::transaction t(db->begin());
	db->execute("delete from Component_Key;");
	db->execute("delete from Action_Key;");
	db->execute("delete from Action;");
	db->execute("delete from Component;");
	try
	{
		//Component
		co.reset(new Component(1, "Variable Speed Sign", "")); db->persist(co);
		///Add component keys
		ck.reset(new Component_Key("CurrentSpeed", "int", "25..75", true, true, "The speed currenlty shown on the sign")); co->setKey(ck);	db->persist(ck);
		ck.reset(new Component_Key("DefaultSpeed", "int", "25..75", true, true, "The speed currenlty shown on the sign")); co->setKey(ck);	db->persist(ck);
		///Add component actions
		ac.reset(new Action(1, "ChangeSpeed", "")); ac->setComponent(co); db->persist(ac);
		ak.reset(new Action_Key("NextSpeed", "int", "", true, "")); ac->setKey(ak); db->persist(ak); 
	

		//Component
		co.reset(new Component(2, "Variable Message Sign", "")); db->persist(co);
		co1.reset(new Component(4, "HAR", "")); db->persist(co1); 
		ck.reset(new Component_Key("CurrentMessage", "string", "", true, true, "Human Readable message currently shown")); co->setKey(ck);	co1->setKey(ck); db->persist(ck);
		ck.reset(new Component_Key("CurrentEvent", "int", "", true, false, "Id of the event associated with the message")); co->setKey(ck);	co1->setKey(ck); db->persist(ck);
		ck.reset(new Component_Key("CandidateEvent", "int", "", true, false, "Id of the event that might be associated with the message")); co->setKey(ck);	co1->setKey(ck); db->persist(ck);


		ac.reset(new Action(2, "Change Message", "")); ac->setComponent(co); db->persist(ac); 
		ak.reset(new Action_Key("NextMessage", "string", "", true, ""));  ac->setKey(ak);  db->persist(ak);
		ak.reset(new Action_Key("NextEvent", "int", "", true, "")); ac->setComponent(co); ac->setKey(ak);  db->persist(ak); 
	

		ac.reset(new Action(1, "Change Message", "")); ac->setComponent(co1); db->persist(ac); 
		ak =db->load<Action_Key>("NextMessage"); ac->setKey(ak);
		ak =db->load<Action_Key>("NextEvent"); ac->setComponent(co1); ac->setKey(ak);
	

	
	

		//Component
		co.reset(new Component(3, "Tow Truck Depot", "")); db->persist(co);
		ck.reset(new Component_Key("FleetSize", "int", "", true, false, "Total number of trucks")); co->setKey(ck);	co1->setKey(ck); db->persist(ck);
		ck.reset(new Component_Key("Available", "int", "", true, false, "Number currenlty available")); co->setKey(ck);	co1->setKey(ck); db->persist(ck);
		ck.reset(new Component_Key("Link", "map: link -> time", "comma separated string", true, false, "Travel time to each link surviced by the depot cs string")); co->setKey(ck); db->persist(ck);
		ac.reset(new Action(3, "Dispatch Truck", "")); ac->setComponent(co); db->persist(ac);
		ak.reset(new Action_Key("Event", "int", "", true, ""));  ac->setKey(ak);  db->persist(ak);
	
	

		//Component
		co.reset(new Component(6, "Open Shoulder", "")); db->persist(co);  
		ck.reset(new Component_Key("Open", "bool", "", true, true, "Is the shoulder currenlty open"));  co->setKey(ck);	co1->setKey(ck); db->persist(ck);
		ac.reset(new Action(4, "Open", "")); ac->setComponent(co); db->persist(ac);
	
	

		///Other Components
		co.reset(new Component(5, "Traffic Signal", "")); db->persist(co);
		co.reset(new Component(7, "Toll Station", "")); db->persist(co);
		co.reset(new Component(8, "Fixed Sensor", "")); db->persist(co);
		co.reset(new Component(9, "Vehicle Identification Sensor", "")); db->persist(co);
		co.reset(new Component(10, "Probe Vehicle", "")); db->persist(co);
	}
	catch (const odb::exception& e)
	{
		std::cout << "Exception happened in Component.h while persisting.\n";
		std::cout << e.what() << "\n";
		return;
	}	
	try 
	{
		t.commit();
	}
	catch (const odb::exception& e)
	{
		std::cout << "Exception happened in Component.h while commiting\n";
		std::cout << e.what() << "\n Exiting...\n";
		return;
	}	
	cout << "Added Component, Component_Key, Action and Action_Key\n";
}
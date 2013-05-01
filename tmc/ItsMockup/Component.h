#include <Io\Io.h>

void PopulateComponent(std::string db_path)
{
	using namespace polaris::io;
	unique_ptr<odb::database> db = open_sqlite_database(db_path);
	
	shared_ptr<Component>     co (nullptr);
	shared_ptr<Component>     co1 (nullptr);
	shared_ptr<Action>        ac (nullptr);
	shared_ptr<Action_Key>    ak (nullptr);

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
	db->execute("delete from Action_Key;");
	db->execute("delete from Action_Keys;");
	db->execute("delete from Action;");
	db->execute("delete from Component;");
	try
	{
		//Component
		co.reset(new Component(1, "Variable Speed Sign", "")); 
		///Add component actions
		ac.reset(new Action(1, "ChangeSpeed", "")); ac->setComponent(co); 
		ak.reset(new Action_Key("NextSpeed", "int", "", true, "")); ac->setKey(ak); db->persist(ak); 
		db->persist(ac);
		db->persist(co);
	

		//Component
		co.reset(new Component(2, "Variable Message Sign", "")); 
		co1.reset(new Component(4, "HAR", "")); db->persist(co1); 
		db->persist(co);


		ac.reset(new Action(2, "Change Message", "")); ac->setComponent(co); 
		ak.reset(new Action_Key("NextMessage", "string", "", true, ""));  ac->setKey(ak);  db->persist(ak);
		ak.reset(new Action_Key("NextEvent", "int", "", true, "")); ac->setComponent(co); ac->setKey(ak);  db->persist(ak); 
		db->persist(ac); 
	

		ac.reset(new Action(1, "Change Message", "")); ac->setComponent(co1);  
		ak =db->load<Action_Key>("NextMessage"); ac->setKey(ak);
		ak =db->load<Action_Key>("NextEvent"); ac->setComponent(co1); ac->setKey(ak);
		db->persist(ac);
	

	
	

		//Component
		co.reset(new Component(3, "Tow Truck Depot", "")); 
		ac.reset(new Action(3, "Dispatch Truck", "")); ac->setComponent(co); 
		ak.reset(new Action_Key("Event", "int", "", true, ""));  ac->setKey(ak);  db->persist(ak);
		db->persist(ac);
		db->persist(co);
	

		//Component
		co.reset(new Component(6, "Open Shoulder", ""));  
		ac.reset(new Action(4, "Open", "")); ac->setComponent(co);
		db->persist(ac);
		db->persist(co); 
	
	

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
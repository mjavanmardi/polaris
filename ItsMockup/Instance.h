#include <Io\Io.h>
#include <Io\Geometry.h>
void PopulateInstance(std::string db_path)
{
	using namespace polaris::io;
	auto_ptr<odb::database> db = open_sqlite_database(db_path);
	
	//get link_geometry
	map<int, shape_geometry> points; 
	map<int, shape_geometry>::iterator points_it;
	std::vector<shape_geometry> shape;
	points = GetLinkPoints(db_path);
	int link_count = 0;
	//iterate over the links
	typedef odb::query<Link> ql; typedef odb::result<Link> rl;
	typedef odb::query<Component> qc; typedef odb::result<Component> rc;
	odb::transaction t (db->begin());
	odb::session s;
	db->execute("delete from Instance;");
	db->execute("delete from Instance_values;");
	db->execute("delete from Instance_Value;");
	//try
	//{
		shared_ptr<Component> comp_vss = db->load<Component>(1);
		shared_ptr<Component> comp_vms = db->load<Component>(2);
		shared_ptr<Component> comp_ttd = db->load<Component>(3);
		shared_ptr<Component> comp_har = db->load<Component>(4);
		shared_ptr<Component> comp_os =  db->load<Component>(6);
	//}
	//catch( const odb::exception& e)
	//{
	//	std::cout << "Not able to load one of the components in Instance.h\n";
	//	std::cout << e.what() << "\n. Exiting...";
	//	exit(1);
	//}
	string type;
	int p;
	shared_ptr<polaris::io::Instance> inst(nullptr);
	polaris::io::Instance inst_temp;
	shared_ptr<polaris::io::Instance_Value> inst_val (nullptr);
	shared_ptr<polaris::io::Component_Key> comp_key (nullptr);

	rl r (db->query<Link> (ql::true_expr));
	db->execute("DELETE from Instance;");
	comp_key = db->load<Component_Key>("CurrentEvent");
	for (rl::iterator i (r.begin()); i!=r.end(); ++i)
	{
		points_it = points.find(i->getLink());
		if (points_it == points.end())
			continue;
		type = i->getType()->getLink_Type();
		p = rand() % 100;
		//put a VMS with probability 10%
		if ( (type == "FREEWAY" || type == "EXPRESSWAY" ) && (p < 10) )
		{
			inst.reset(new Instance());
			inst->setComponent(comp_vms);
			inst->setLocation_X(points_it->second.x);
			inst->setLocation_Y(points_it->second.y);
			inst_val.reset(new Instance_Value());			
			inst_val->setKey(comp_key);
			inst_val->setValue("-1");	
			try 
			{
				db->persist(inst_val);
			}
			catch (odb::object_already_persistent e) {
				std::cout << "VMS Instance Values object already exists. " << e.what() << "\n";
			}
			comp_vms->setInstanc(inst);
			inst->setValu(inst_val);
			try 
			{
				db->persist(inst);
			}
			catch (odb::object_already_persistent e) 
			{
				std::cout << "VMS Instance object already exists. " << e.what() << "\n";
			}
		}
		//put a VSS with probability 5%
		comp_key = db->load<Component_Key>("CurrentSpeed");
		if ( (type == "FREEWAY" || type == "EXPRESSWAY" ) && (p > 95) )
		{
			inst.reset(new Instance());
			inst->setComponent(comp_vss);
			inst->setLocation_X(points_it->second.x);
			inst->setLocation_Y(points_it->second.y);
			try {
				db->persist(inst);
			}
			catch (odb::object_already_persistent e) {
				std::cout << "VSS Instance object already exists. " << e.what() << "\n";
			}			
			inst_val.reset(new Instance_Value());			
			inst_val->setKey(comp_key);
			inst_val->setValue("55");	
			try {
				db->persist(inst_val);
			}
			catch (odb::object_already_persistent e) {
				std::cout << "VSS Instance Values object already exists. " << e.what() << "\n";
			}	
			comp_vss->setInstanc(inst);
			inst->setValu(inst_val);
			try {
				db->persist(inst);
			}
			catch (odb::object_already_persistent e) {
				std::cout << "VSS Instance object already exists. " << e.what() << "\n";
			}

			
		}
		//put a OpenShoulder with probability 20%
		comp_key = db->load<Component_Key>("Open");
		if ( (type == "FREEWAY") && (p < 20) )
		{
			inst.reset(new Instance());
			inst->setComponent(comp_os);
			inst->setLocation_X(points_it->second.x);
			inst->setLocation_Y(points_it->second.y);
			
			inst_val.reset(new Instance_Value());			
			inst_val->setKey(comp_key);
			inst_val->setValue("0");	
			try {
				db->persist(inst_val);
			}
			catch (odb::object_already_persistent e) {
				std::cout << "OS Instance Values object already exists. " << e.what() << "\n";
			}	
			comp_os->setInstanc(inst);
			inst->setValu(inst_val);
			try {
				db->persist(inst);
			}
			catch (odb::object_already_persistent e) {
				std::cout << "OS Instance object already exists. " << e.what() << "\n";

			}
			
			
		}
	}

	t.commit();
}
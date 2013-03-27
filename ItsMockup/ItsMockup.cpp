#include <Io\Io.h>
#include <iostream>
#include <Io\Geometry.h>

//#ifdef _DEBUG
//#pragma comment(lib, "odb-sqlite-d-mt.lib")
//#pragma comment(lib, "sqlite3-d-mt.lib")
//#pragma comment(lib, "odb-d-mt.lib")
//#else
//#pragma comment(lib, "odb-sqlite-mt.lib")
//#pragma comment(lib, "sqlite3-mt.lib")
//#pragma comment(lib, "odb-mt.lib")
//#endif


int main(int argc, char* argv[])
{
	using namespace polaris::io;
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " path_to_db\n";
		exit(1);
	}
	std::string db_name = argv[1];
	//check if file exists
	std::fstream test_fh(make_name(db_name, "Supply"));
	if (!test_fh.good()) {
		std::cout << "File " << make_name(db_name, "Supply") << " does not exist\n";
		exit(1);
	}
	else test_fh.close();
	//get link_geometry
	map<int, shape_geometry> points; 
	map<int, shape_geometry>::iterator points_it;
	std::vector<shape_geometry> shape;
	points = GetLinkPoints(argv[1]);
	int link_count = 0;
	auto_ptr<odb::database> db = ::open_sqlite_database(argv[1]);
	//iterate over the links
	typedef odb::query<Link> ql; typedef odb::result<Link> rl;
	typedef odb::query<Component> qc; typedef odb::result<Component> rc;
	odb::transaction t (db->begin());
	shared_ptr<Component> comp_vss = db->load<Component>(1);
	shared_ptr<Component> comp_vms = db->load<Component>(2);
	shared_ptr<Component> comp_ttd = db->load<Component>(3);
	shared_ptr<Component> comp_har = db->load<Component>(4);
	shared_ptr<Component> comp_os =  db->load<Component>(6);
	string type;
	int p;
	shared_ptr<polaris::io::Instance> inst(nullptr);
	polaris::io::Instance inst_temp;
	shared_ptr<polaris::io::Instance_Value> inst_vals (nullptr);
	shared_ptr<polaris::io::Component_Key> comp_props (nullptr);

	rl r (db->query<Link> (ql::true_expr));
	db->execute("DELETE from Instance;");
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
			try {
				db->persist(inst);
			}
			catch (odb::object_already_persistent e) {
				std::cout << "VMS Instance object already exists. " << e.what() << "\n";
			}
		}
		//put a VSS with probability 5%
		comp_props = db->load<Component_Key>(1);
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
			inst_vals.reset(new Instance_Value());			
			inst_vals->setInstatce(inst);
			inst_vals->setValue("55");	
			try {
				db->persist(inst_vals);
			}
			catch (odb::object_already_persistent e) {
				std::cout << "VSS Instance Values object already exists. " << e.what() << "\n";
			}	
			
		}
		//put a OpenShoulder with probability 20%
		comp_props = db->load<Component_Key>(29);
		if ( (type == "FREEWAY") && (p < 20) )
		{
			inst.reset(new Instance());
			inst->setComponent(comp_os);
			inst->setLocation_X(points_it->second.x);
			inst->setLocation_Y(points_it->second.y);
			try {
				db->persist(inst);
			}
			catch (odb::object_already_persistent e) {
				std::cout << "OS Instance object already exists. " << e.what() << "\n";
			}			
			inst_vals.reset(new Instance_Value());			
			inst_vals->setInstatce(inst);
			inst_vals->setValue("0");	
			try {
				db->persist(inst_vals);
			}
			catch (odb::object_already_persistent e) {
				std::cout << "OS Instance Values object already exists. " << e.what() << "\n";
			}	
			
		}
	}
	t.commit();


}
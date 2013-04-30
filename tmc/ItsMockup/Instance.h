#include <Io\Io.h>
#include <Io\Geometry.h>
void PopulateInstance(std::string db_path)
{
	using namespace polaris::io;
	unique_ptr<odb::database> db = open_sqlite_database(db_path);
	
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

	rl r (db->query<Link> (ql::true_expr));
	db->execute("DELETE from VSS;");
	db->execute("DELETE from VMS;");
	db->execute("DELETE from HAR;");
	db->execute("DELETE from Depot;");
	db->execute("DELETE from OpenShoulder;");

	shared_ptr<VMS>				vms (nullptr);
	shared_ptr<VSS>				vss (nullptr);
	shared_ptr<HAR>				har (nullptr);
	shared_ptr<Depot>			depot (nullptr);
	shared_ptr<OpenShoulder>    os (nullptr);
	shared_ptr<LinkList>		link_set (nullptr);

	har.reset(new HAR(1,5817, 0,1000, 20, 0));
	har->setComponent(comp_har);
	link_set.reset(new LinkList());
	link_set->setLinks(GetLinksInsideCounty(db_path, "Lake"));
	db->persist(link_set);
	har->setLinks(link_set);
	db->persist(har);

	depot.reset(new Depot(0, 16277, 0, 100, 10, 500, 500));
	depot->setComponent(comp_ttd);
	link_set.reset(new LinkList());
	link_set->setLinks(GetLinksInsideDepotPolygon(db_path));
	db->persist(link_set);
	depot->setLinks(link_set);
	db->persist(depot);

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
			vms.reset(new VMS());
			vms->setComponent(comp_vms);
			if (i->getLanes_Ab() == 0 && i->getLanes_Ba() == 0)
				continue;
			if (i->getLanes_Ab() > 0) 
				vms->setDir(0);
			else
				vms->setDir(1);
			vms->setLink(i->getLink());
			vms->setOffset(i->getLength()/2);
			vms->setSetback(20);
			vms->setInitial_Event(0);
			try 
			{
				db->persist(vms);
			}
			catch (odb::object_already_persistent e) 
			{
				std::cout << "VMS Instance object already exists. " << e.what() << "\n";
			}
		}
		//put a VSS with probability 5%
		if ( (type == "FREEWAY" || type == "EXPRESSWAY" ) && (p > 95) )
		{
			vss.reset(new VSS());
			vss->setComponent(comp_vss);
			if (i->getLanes_Ab() == 0 && i->getLanes_Ba() == 0)
				continue;
			if (i->getLanes_Ab() > 0) 
				vss->setDir(0);
			else
				vss->setDir(1);
			vss->setLink(i->getLink());
			vss->setOffset(i->getLength()/2);
			vss->setSetback(20);
			vss->setInitial_Speed(55);
			try {
				db->persist(vss);
			}
			catch (odb::object_already_persistent e) {
				std::cout << "VSS Instance object already exists. " << e.what() << "\n";
			}

			
		}
		//put a OpenShoulder with probability 20%
		if ( (type == "FREEWAY") && (p < 20) )
		{
			os.reset(new OpenShoulder());
			os->setComponent(comp_os);
			link_set.reset(new LinkList());
			link_set->setLink(i->getLink());
			db->persist(link_set);
			os->setLinks(link_set);
			try {
				db->persist(os);
			}
			catch (odb::object_already_persistent e) {
				std::cout << "OS Instance Values object already exists. " << e.what() << "\n";
			}	
			
			
		}
	}
	try {
		t.commit();
	}
	catch (odb::database_exception& e) {
		std::cout << "Commit failed. " << e.what() << "\n";
	}
	
}
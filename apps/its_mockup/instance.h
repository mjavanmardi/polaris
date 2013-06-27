#include <Io/Io.h>
#include <Io/Geometry.h>
#include <iostream>
#include <fstream>
#include <string>
using std::map;

std::vector<int > ReadLinksFromFile(const char* filename)
{
	std::string line;
	int link, n_links;
	std::vector<int > result;
	std::ifstream fh(filename);
	if (fh.is_open())
	{
		getline(fh, line);
		n_links = atoi(line.c_str());
		for (int i = 0; i < n_links; i++)
		{
			getline(fh, line);
			link = atoi(line.c_str());
			result.push_back(link);
		}
	}
	return result;
}


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
	std::cout << "Delete from VSS, HAR, VMS, Depot, OpenShoulder, Fixed_Sensor\n";
	db->execute("DELETE from VSS;");
	db->execute("DELETE from VMS;");
	db->execute("DELETE from HAR;");
	db->execute("DELETE from Depot;");
	db->execute("DELETE from OpenShoulder;");
	db->execute("DELETE from Fixed_Sensor;");

	shared_ptr<VMS>				vms (nullptr);
	shared_ptr<VSS>				vss (nullptr);
	shared_ptr<HAR>				har (nullptr);
	shared_ptr<Depot>			depot (nullptr);
	shared_ptr<OpenShoulder>    os (nullptr);
	shared_ptr<LinkList>		link_set (nullptr);
	shared_ptr<Fixed_Sensor>	fixed_sensor (nullptr);


	
	har.reset(new HAR(1,17436, 0,60, 20, 0));
	har->setComponent(comp_har);
	link_set.reset(new LinkList());
	link_set->setLinks(GetLinksInsideCounty(db_path, "Cook"));
	db->persist(link_set);
	har->setLinks(link_set);
	db->persist(har);
	std::cout << "Added HAR to link " << 17436 << "and covering Cook county\n";

	depot.reset(new Depot(0, 16277, 0, 100, 10, 500, 500));
	depot->setComponent(comp_ttd);
	link_set.reset(new LinkList());
	link_set->setLinks(GetLinksInsideDepotPolygon(db_path));
	db->persist(link_set);
	depot->setLinks(link_set);
	db->persist(depot);
	std::cout << "Added HAR to link " << 16277 << "and covering depot polygon\n";


	std::cout << "Randomly locate and add VMS, Fixerd_Sensor and VSS\n";
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
			vms->setOffset((float)i->getLength()/2);
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
		if ( (type == "FREEWAY" || type == "EXPRESSWAY" ) && (p < 60) )
		{
			fixed_sensor.reset(new Fixed_Sensor());
			if (i->getLanes_Ab() == 0 && i->getLanes_Ba() == 0)
				continue;
			if (i->getLanes_Ab() > 0) 
				fixed_sensor->setDir(0);
			else
				fixed_sensor->setDir(1);
			fixed_sensor->setLink(i->getLink());
			fixed_sensor->setOffset(0);
			fixed_sensor->setAggregation_Period_Sec(60*5);
			fixed_sensor->setSigma(0.1);
			try 
			{
				db->persist(fixed_sensor);
			}
			catch (odb::object_already_persistent e) 
			{
				std::cout << "fixed_sensor Instance object already exists. " << e.what() << "\n";
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
			vss->setOffset((float)i->getLength()/2);
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
		//if ( (type == "FREEWAY") && (p < 20) )
		//{
		//	std::string line;
		//	int link, n_links;

		//	os.reset(new OpenShoulder());
		//	os->setComponent(comp_os);
		//	link_set.reset(new LinkList());
		//	std::ifstream fh("C:/Users/vsokolov/usr/polaris/Subarea/LSDLinks.txt");
		//	if (fh.is_open())
		//	{
		//		getline(fh, line);
		//		n_links = atoi(line.c_str());
		//		for (int i = 0; i < n_links; i++)
		//		{
		//			getline(fh, line);
		//			link = atoi(line.c_str());
		//			link_set->setLink(link);

		//		}
		//	}
		//	//link_set->setLink(i->getLink());
		//	db->persist(link_set);
		//	os->setLinks(link_set);
		//	try {
		//		db->persist(os);
		//	}
		//	catch (odb::object_already_persistent e) {
		//		std::cout << "OS Instance Values object already exists. " << e.what() << "\n";
		//	}	
		//	
		//	
		//}
	}

	os.reset(new OpenShoulder());
	os->setComponent(comp_os);
	link_set.reset(new LinkList());
	link_set->setLinks(ReadLinksFromFile("C:/Users/vsokolov/usr/polaris/Subarea/LSDLinks.txt"));
	db->persist(link_set);
	os->setLinks(link_set);
	try {
		db->persist(os);
	}
	catch (odb::object_already_persistent e) {
		std::cout << "OS Instance Values object already exists. " << e.what() << "\n";
	}
	std::cout << "Added OpenShoulder to links defined in C:/Users/vsokolov/usr/polaris/Subarea/LSDLinks.txt\n";

	os.reset(new OpenShoulder());
	os->setComponent(comp_os);
	link_set.reset(new LinkList());
	link_set->setLinks(ReadLinksFromFile("C:/Users/vsokolov/usr/polaris/Subarea/LinkGroup1.txt"));
	db->persist(link_set);
	os->setLinks(link_set);
	try {
		db->persist(os);
	}
	catch (odb::object_already_persistent e) {
		std::cout << "OS Instance Values object already exists. " << e.what() << "\n";
	}
	std::cout << "Added OpenShoulder to links defined in C:/Users/vsokolov/usr/polaris/Subarea/LinkGroup1.txt\n";
	os.reset(new OpenShoulder());
	os->setComponent(comp_os);
	link_set.reset(new LinkList());
	link_set->setLinks(ReadLinksFromFile("C:/Users/vsokolov/usr/polaris/Subarea/LinkGroup1.txt"));
	db->persist(link_set);
	os->setLinks(link_set);
	try {
		db->persist(os);
	}
	catch (odb::object_already_persistent e) {
		std::cout << "OS Instance Values object already exists. " << e.what() << "\n";
	}

	os.reset(new OpenShoulder());
	os->setComponent(comp_os);
	link_set.reset(new LinkList());
	link_set->setLinks(ReadLinksFromFile("C:/Users/vsokolov/usr/polaris/Subarea/LinkGroup2.txt"));
	db->persist(link_set);
	os->setLinks(link_set);
	try {
		db->persist(os);
	}
	catch (odb::object_already_persistent e) {
		std::cout << "OS Instance Values object already exists. " << e.what() << "\n";
	}
	std::cout << "Added OpenShoulder to links defined in C:/Users/vsokolov/usr/polaris/Subarea/LinkGroup2.txt\n";

	os.reset(new OpenShoulder());
	os->setComponent(comp_os);
	link_set.reset(new LinkList());
	link_set->setLinks(ReadLinksFromFile("C:/Users/vsokolov/usr/polaris/Subarea/LinkGroup3.txt"));
	db->persist(link_set);
	os->setLinks(link_set);
	try {
		db->persist(os);
	}
	catch (odb::object_already_persistent e) {
		std::cout << "OS Instance Values object already exists. " << e.what() << "\n";
	}
	std::cout << "Added OpenShoulder to links defined in C:/Users/vsokolov/usr/polaris/Subarea/LinkGroup3.txt\n";


	os.reset(new OpenShoulder());
	os->setComponent(comp_os);
	link_set.reset(new LinkList());
	link_set->setLinks(ReadLinksFromFile("C:/Users/vsokolov/usr/polaris/Subarea/DwntwnLinks.txt"));
	db->persist(link_set);
	os->setLinks(link_set);
	try {
		db->persist(os);
	}
	catch (odb::object_already_persistent e) {
		std::cout << "OS Instance Values object already exists. " << e.what() << "\n";
	}
	std::cout << "Added OpenShoulder to links defined in C:/Users/vsokolov/usr/polaris/Subarea/DwntwnLinks.txt\n";



	try {
		t.commit();
	}
	catch (odb::database_exception& e) {
		std::cout << "Commit failed. " << e.what() << "\n";
	}
	
}
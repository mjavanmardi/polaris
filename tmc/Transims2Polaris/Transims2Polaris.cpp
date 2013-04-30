#include "convert_methods.h"
#include "Io\Io.h"

#include "PopulateAdditional.h"

#include "transims_network.h"
#include "Geometry.h"
#include "Db_File.hpp"
#include <time.h>

#include "Dummy.h"
using namespace polaris::io;
using namespace std;

void test_create(const string& name)
{
	InputContainer container;
	shared_ptr<Node> n1 (new Node(1,1,1,0,0,0));
	shared_ptr<Node> n2 (new Node(2,2,1,0,0,0));
	shared_ptr<Node> n3 (new Node(3,1,1,0,0,0));
	shared_ptr<Node> n4 (new Node(4,1,1,0,0,0));
	container.Nodes[1] = n1;
	container.Nodes[2] = n2;
	container.Nodes[3] = n3;
	container.Nodes[4] = n4;
	//unique_ptr<odb::database> db (create_database (argc, argv));
	unique_ptr<odb::database> db (create_sqlite_database (name));
	cout << "New database "<< ((odb::sqlite::database*)&(*db))->name() <<" was created\n";
	//shared_ptr<Link> l1 (new Link(1, "lalal", container.Nodes[1], container.Nodes[2], 1.0, 2.0, 3.0, 10, 20, 2, 0, 8, 12, 0.3, 2,22,22,122,1,33,333,2,3,2,1,2));
	//shared_ptr<Link> l2 (new Link(2, "lalal", container.Nodes[3], container.Nodes[4], 1.0, 2.0, 3.0, 10, 20, 2, 0, 8, 12, 0.3, 2,22,22,122,1,33,333,2,3,2,1, 3));
	//container.Links[1] = l1;
	//container.Links[2] = l2;

	odb::transaction t (db->begin());
	db->persist(n1);
	db->persist(n2);
	db->persist(n3);
	db->persist(n4);
	//db->persist(l1);
	//db->persist(l2);
	t.commit();
	cout << "Press any key...\n";
	getchar();
}
void test_read(const string& name)
{
	typedef odb::query<Link> query;
	typedef odb::result<Link> result;
	vector<char>::iterator it;
	unique_ptr<odb::database> db (open_sqlite_database (name));
	cout << "Database "<< ((odb::sqlite::database*)&(*db))->name() <<" was opened\n";
	odb::transaction t (db->begin ());
	result r (db->query<Link> ( (query::node_a >120)));
    for (result::iterator i (r.begin ()); i != r.end (); ++i)
    {
		cout << i->getLink() << " " << i->getNode_A()->getNode() << " " << i->getNode_B()->getNode() << endl;
		
    }
	cout << "Press any key...\n";
	getchar();
}

int main(int argc, char* argv[])
{

	//test_create("C:\\Users\\vsokolov\\usr\\polaris_io\\Transims2Polaris\\test.sqlite");
	//test_read("C:/Users/vsokolov/usr/polaris_io/Transims2Polaris/TestNet50");
	//return 0;

	TransimsNetwork* net = new TransimsNetwork();
	InputContainer container;
	if (argc==1)
		net->Init();
	else
		net->Init(argc, argv);
	//unique_ptr<odb::database> db = create_sqlite_database  (net->path_to_database);
	shared_ptr<odb::database> db = create_sqlite_database  (net->path_to_database);
	Db_File control_file;
	char* control_record;
	string control_content;
	
	//shared_ptr<Link_Delay> ld;
	//ld->setDir(1);
	//ld->setLink(1);
	//transaction t1 (db->begin());
	//db->persist(ld);
	//t1.commit();


	/************************************************/
	/*****************Conversion*********************/
	/************************************************/	
	PopulateLinkType(net->path_to_database, &container.Link_Types);
	PopulateAreaType(net->path_to_database, &container.Area_Types);
	PopulateUseCode(net->path_to_database);
	PopulateDimensionQuantity(net->path_to_database);
	Convert<Node_File,Node, int>(net,container, NODE, "NODE", &container.Nodes);
	Convert<Link_File,Link, int>(net,container, LINK, "LINK", &container.Links);
	ConvertNested<Shape_File,Shape, int, shape_geometry>(net,container, SHAPE, "SHAPE");
	Convert<Zone_File,Zone, int>(net,container, ZONE, "ZONE", &container.Zones);
	Convert<Lane_Use_File,Lane_Use, int>(net,container, LANE_USE, "LANE_USE");
	Convert<Detector_File,Detector, int>(net,container, DETECTOR, "DETECTOR");

	ConvertNested<Signal_File,Signal, int, signal_time>(net,container, SIGNAL, "SIGNAL", &container.Signals);
	Convert<Location_File,Location, int>(net,container, LOCATION, "LOCATION", &container.Locations);
	Convert<Parking_File,Parking, int>(net,container, PARKING, "PARKING", &container.Parkings);
	Convert<Veh_Type_File,Veh_Type, int>(net,container, VEHICLE_TYPE, "VEHICLE_TYPE", &container.Veh_Types);

	Convert<Pocket_File,Pocket, int>(net,container, POCKET, "POCKET");
	Convert<Shape_File,Shape, int>(net,container, SHAPE, "SHAPE");
	Convert<Sign_File,Sign, int>(net,container, SIGN, "SIGN");
	ConvertNested<Timing_File,Timing, int, timing_phase>(net,container, TIMING_PLAN, "TIMING_PLAN");
	ConvertNested<Phasing_File,Phasing, int, phase_movement>(net,container, PHASING_PLAN, "PHASING_PLAN");
	Convert<Connect_File,Connect, int>(net,container, CONNECTION, "CONNECTION");
	Convert<Vehicle_File,Vehicle, int>(net,container, VEHICLE, "VEHICLE");
	Convert<Trip_File,Trip, int>(net,container, TRIP, "TRIP");
	if (net->add_geo_columns)
	{
		cout << "Adding necessary geo columns\n";
		AddSpatialiteGeometry(net);
		cout << "Finished adding geo columns\n";

	}
	bool cf_flag = control_file.Open(net->Control_File());
	control_content = "";
	if (cf_flag)
	{
		while (control_file.Read ()) 
		{
			control_record = (char *) control_file.Record_String ();
			control_content += control_record;
			control_content += "\n";
		}
		odb::transaction t (db->begin());
		shared_ptr<MetaData> meta_data (new MetaData("ControlFile", control_content));
		db->persist(meta_data);
		t.commit();
	}
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	shared_ptr<MetaData> meta_data (new MetaData("TimeStamp", asctime(timeinfo)));
	odb::transaction t (db->begin());
	db->persist(meta_data);
	t.commit();
	Dummy(net->path_to_database);

	return 0;
}


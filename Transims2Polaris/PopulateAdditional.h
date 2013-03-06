#include "Io\database.h"

void PopulateUseCode(string db_path)
{
	auto_ptr<database> db (open_sqlite_database (db_path)); 
	transaction t (db->begin());
	shared_ptr<polaris::io::Use_Code> uc (nullptr);
	uc = make_shared<Use_Code> ( "NONE",		1, 0, "SPECIAL", "", "", "" ); db->persist(uc);
	uc = make_shared<Use_Code> ( "RESTRICTED",	2, 0, "SPECIAL", "", "", "" ); db->persist(uc);
	uc = make_shared<Use_Code> ( "ANY",		3, 0, "", "SOV|HOV2|HOV3|HOV4|TAXI|LIGHTTRUCK|HEAVYTRUCK|BUS|RAIL|WALK|BICYCLE", "", "" ); db->persist(uc);
	uc = make_shared<Use_Code> ( "AUTO",		4, 0, "ANY", "SOV|HOV2|HOV3|HOV4|TAXI", "CAR", "" ); db->persist(uc);
	uc = make_shared<Use_Code> ( "SOV",		5, 1, "ANY|AUTO", "HOV2|HOV3|HOV4", "", "" ); db->persist(uc);
	uc = make_shared<Use_Code> ( "HOV2",		7, 1, "ANY|AUTO|SOV", "HOV3", "HOV4", "" ); db->persist(uc);
	uc = make_shared<Use_Code> ( "HOV3",		8, 1, "ANY|AUTO|SOV|HOV2", "HOV4", "", "" ); db->persist(uc);
	uc = make_shared<Use_Code> ( "HOV4",		9, 1, "ANY|AUTO|SOV|HOV2|HOV3", "", "", "" ); db->persist(uc);
	uc = make_shared<Use_Code> ( "TAXI",		10, 1, "ANY|AUTO|SOV", "", "", "" ); db->persist(uc);
	uc = make_shared<Use_Code> ( "TRUCK",		11, 0, "ANY", "LIGHTTRUCK|HEAVYTRUCK", "", "" ); db->persist(uc);
	uc = make_shared<Use_Code> ( "LIGHTTRUCK",	12, 1, "ANY|TRUCK", "", "", "" ); db->persist(uc);
	uc = make_shared<Use_Code> ( "HEAVYTRUCK",	13, 1, "ANY|TRUCK", "", "", "" ); db->persist(uc);
	uc = make_shared<Use_Code> ( "BUS",		14, 1, "ANY", "", "", "" ); db->persist(uc);
	uc = make_shared<Use_Code> ( "RAIL",		15, 1, "ANY", "", "TROLLEY|STREETCAR|LIGHTRAIL|RAPIDRAIL|REGIONRAIL", "" ); db->persist(uc);
	uc = make_shared<Use_Code> ( "WALK",		16, 1, "ANY", "", "", "" ); db->persist(uc);
	uc = make_shared<Use_Code> ( "BICYCLE",	17, 1, "ANY", "", "BIKE", "" ); db->persist(uc);
	t.commit();

}
void PopulateLinkType(string db_path, std::map<string,shared_ptr<Link_Type>> *container)
{
	auto_ptr<database> db (open_sqlite_database (db_path)); 
	transaction t (db->begin());
	shared_ptr<Link_Type> lt (nullptr);
	lt = make_shared<Link_Type>("FREEWAY",		10, "ANY|AUTO|TRUCK|BUS|SOV|HOV2|HOV3|HOV4|LIGHTTRUCK|HEAVYTRUCK|NONE|TAXI|RESTRICTED|NONE",				"", "" ); (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	lt = make_shared<Link_Type>("EXPRESSWAY",	20, "ANY|AUTO|TRUCK|BUS|SOV|HOV2|HOV3|HOV4|LIGHTTRUCK|HEAVYTRUCK|NONE|TAXI|RESTRICTED|NONE",				"XPRESSWAY", "" );  (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	lt = make_shared<Link_Type>("PRINCIPAL",		30, "ANY|WALK|BICYCLE|AUTO|TRUCK|BUS|SOV|HOV2|HOV3|HOV4|LIGHTTRUCK|HEAVYTRUCK|TAXI|RESTRICTED|NONE",		"", "" );  (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	lt = make_shared<Link_Type>("MAJOR",			40, "ANY|WALK|BICYCLE|AUTO|TRUCK|BUS|SOV|HOV2|HOV3|HOV4|LIGHTTRUCK|HEAVYTRUCK|TAXI|RESTRICTED|NONE",		"PRIARTER", "" );  (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	lt = make_shared<Link_Type>("MINOR",			50, "ANY|WALK|BICYCLE|AUTO|TRUCK|BUS|SOV|HOV2|HOV3|HOV4|LIGHTTRUCK|HEAVYTRUCK|TAXI|RESTRICTED|NONE",		"SECARTER", "" );  (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	lt = make_shared<Link_Type>("COLLECTOR",		60, "ANY|WALK|BICYCLE|AUTO|TRUCK|BUS|SOV|HOV2|HOV3|HOV4|LIGHTTRUCK|HEAVYTRUCK|TAXI|RESTRICTED|NONE",		"", "" );  (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	lt = make_shared<Link_Type>("LOCAL_THRU",	70, "ANY|WALK|BICYCLE|AUTO|TRUCK|BUS|SOV|HOV2|HOV3|HOV4|LIGHTTRUCK|HEAVYTRUCK|TAXI|RESTRICTED|NONE",		"THRU|LOCAL_ACCESS", "" );  (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	lt = make_shared<Link_Type>("LOCAL",			80, "ANY|WALK|BICYCLE|AUTO|TRUCK|BUS|SOV|HOV2|HOV3|HOV4|LIGHTTRUCK|HEAVYTRUCK|TAXI|RESTRICTED|NONE",		"", "" );  (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	lt = make_shared<Link_Type>("FRONTAGE",		90, "ANY|WALK|BICYCLE|AUTO|TRUCK|BUS|SOV|HOV2|HOV3|HOV4|LIGHTTRUCK|HEAVYTRUCK|TAXI|RESTRICTED|NONE",		"", "" );  (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	lt = make_shared<Link_Type>("RAMP",			100, "ANY|AUTO|TRUCK|BUS|SOV|HOV2|HOV3|HOV4|LIGHTTRUCK|HEAVYTRUCK|TAXI|RESTRICTED|NONE",					"", "" );  (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	lt = make_shared<Link_Type>("BRIDGE",		110, "ANY|WALK|BICYCLE|AUTO|TRUCK|BUS|SOV|HOV2|HOV3|HOV4|LIGHTTRUCK|HEAVYTRUCK|TAXI|RESTRICTED|NONE",		"", "" );  (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	lt = make_shared<Link_Type>("TUNNEL",		120, "ANY|WALK|BICYCLE|AUTO|TRUCK|BUS|SOV|HOV2|HOV3|HOV4|LIGHTTRUCK|HEAVYTRUCK|TAXI|RESTRICTED|NONE",		"", "" );  (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	lt = make_shared<Link_Type>("EXTERNAL",		130, "ANY|WALK|BICYCLE|AUTO|TRUCK|BUS|RAIL|SOV|HOV2|HOV3|HOV4|LIGHTTRUCK|HEAVYTRUCK|RESTRICTED|NONE|TAXI",	"ZONECONN", "" );  (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	lt = make_shared<Link_Type>("WALKWAY",		200, "ANY|WALK|RESTRICTED|NONE",																			"", "" );  (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	lt = make_shared<Link_Type>("BIKEWAY",		210, "ANY|BICYCLE|RESTRICTED|NONE",																			"", "" );  (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	lt = make_shared<Link_Type>("BUSWAY",		220, "ANY|BUS|RESTRICTED|NONE",																				"", "" );  (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	lt = make_shared<Link_Type>("LIGHTRAIL",		300, "ANY|WALK|RAIL|RESTRICTED|NONE",																			"", "" );  (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	lt = make_shared<Link_Type>("HEAVYRAIL",		310, "ANY|WALK|RAIL|RESTRICTED|NONE",																			"", "" );  (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	lt = make_shared<Link_Type>("FERRY", 		400, "ANY|RESTRICTED|NONE",																					"", "" );  (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	lt = make_shared<Link_Type>("OTHER", 		500, "ANY|WALK|BICYCLE|AUTO|TRUCK|BUS|RAIL|SOV|HOV2|HOV3|HOV4|LIGHTTRUCK|HEAVYTRUCK|RESTRICTED|NONE|TAXI",	"", "" );  (*container)[lt->getLink_Type()] = lt; db->persist(lt);
	t.commit();

}

void PopulateAreaType (string db_path, std::map<int,shared_ptr<Area_Type>> *container)
{
	
	auto_ptr<database> db (open_sqlite_database (db_path)); 
	//connection_ptr c (db->connection ());
	transaction t (db->begin ());
	shared_ptr<Area_Type> at (nullptr);
	at = make_shared<Area_Type>(1, "AREATYPE1", "DEFAULT");
	at = make_shared<Area_Type>(1, "AREATYPE1", "DEFAULT"); (*container)[at->getPrimaryKey()] = at; db->persist(at);
	at = make_shared<Area_Type>(2, "AREATYPE2", "DEFAULT"); (*container)[at->getPrimaryKey()] = at; db->persist(at);
	at = make_shared<Area_Type>(3, "AREATYPE3", "DEFAULT"); (*container)[at->getPrimaryKey()] = at; db->persist(at);
	at = make_shared<Area_Type>(4, "AREATYPE4", "DEFAULT"); (*container)[at->getPrimaryKey()] = at; db->persist(at);
	at = make_shared<Area_Type>(5, "AREATYPE5", "DEFAULT"); (*container)[at->getPrimaryKey()] = at; db->persist(at);
	at = make_shared<Area_Type>(6, "AREATYPE6", "DEFAULT"); (*container)[at->getPrimaryKey()] = at; db->persist(at);
	at = make_shared<Area_Type>(7, "AREATYPE7", "DEFAULT"); (*container)[at->getPrimaryKey()] = at; db->persist(at);
	at = make_shared<Area_Type>(8, "AREATYPE8", "DEFAULT"); (*container)[at->getPrimaryKey()] = at; db->persist(at);
	t.commit();
}

void PopulateDimensionQuantity (string db_path)
{
	auto_ptr<database> db (open_sqlite_database (db_path)); 
	transaction t (db->begin ());
	shared_ptr<Dimension> d (nullptr);
	shared_ptr<Quantity> q (nullptr);
	table_field tf;
	d = make_shared<Dimension>("speed", 0, 1, -1, 0, 0); db->persist(d);	
	tf.table = "Link"; tf.field = "fspd_ab"; q = make_shared<Quantity>(tf, d); db->persist(q);
	tf.table = "Link"; tf.field = "fspd_ba"; q = make_shared<Quantity>(tf, d); db->persist(q);
	tf.table = "Link"; tf.field = "speed_ab"; q = make_shared<Quantity>(tf, d); db->persist(q);
	tf.table = "Link"; tf.field = "speed_ba"; q = make_shared<Quantity>(tf, d); db->persist(q);
	d = make_shared<Dimension>("distance", 0, 1, 0, 0, 0); db->persist(d);
	tf.table = "Link"; tf.field = "length"; q = make_shared<Quantity>(tf, d); db->persist(q);
	d = make_shared<Dimension>("area", 0, 2, 0, 0, 0); db->persist(d);
	d = make_shared<Dimension>("time", 0, 0, 1, 0, 0); db->persist(d);	
	d = make_shared<Dimension>("angle_degree", 0, 0, 0, 0, 0); db->persist(d);	
	tf.table = "Link"; tf.field = "grade"; q = make_shared<Quantity>(tf, d); db->persist(q);
	tf.table = "Link"; tf.field = "bearing_a"; q = make_shared<Quantity>(tf, d); db->persist(q);
	tf.table = "Link"; tf.field = "bearing_b"; q = make_shared<Quantity>(tf, d); db->persist(q);
	d = make_shared<Dimension>("lane", 0, 0, 0, 1, 0); db->persist(d);	
	tf.table = "Link"; tf.field = "lanes_ab"; q = make_shared<Quantity>(tf, d); db->persist(q);
	tf.table = "Link"; tf.field = "lanes_ba"; q = make_shared<Quantity>(tf, d); db->persist(q);
	d = make_shared<Dimension>("capacity", 0, 0, 0, 0, 0); db->persist(d);
	tf.table = "Link"; tf.field = "cap_ab"; q = make_shared<Quantity>(tf, d); db->persist(q);
	tf.table = "Link"; tf.field = "cap_ba"; q = make_shared<Quantity>(tf, d); db->persist(q);
	t.commit();
}


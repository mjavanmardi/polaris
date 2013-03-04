#pragma once
#include "adopter_methods.h"
#include "Io\Io.h"
#include <memory>   // std::auto_ptr
#include "transims_network.h"
#include <time.h>
#include <string>
#include "Geometry.h"
#include <map>
using odb::database;
using odb::transaction;
using namespace polaris::io;
using namespace std;
using namespace odb::core;


static void trace_callback(void *log_file, const char* content)
{
	ofstream *fh = new ofstream();
	fh = reinterpret_cast<ofstream*>(log_file);
	*fh << content << "\n";
}

template<class FileType, class ODBType, class KeyType>
void Convert(TransimsNetwork *net, InputContainer &container, System_File_Type file_type, string odb_type_name, std::map<KeyType,shared_ptr<ODBType>> *container_map = NULL)
{
	if (!net->System_File_Flag(file_type))
		return;
	cout << "Converting " << odb_type_name << "\n";
	shared_ptr<ODBType> record;
	FileType *file = (FileType *) net->System_File_Handle (file_type);
	try
	{
		auto_ptr<database> db (open_sqlite_database (net->path_to_database));
		transaction t (db->begin());
		while (file->Read(false))
		{		
			net->Show_Progress();
			record = Adapter(*file, container);
			if (container_map!=NULL)
				(*container_map)[record->getPrimaryKey()] = record;
			try 
			{
				db->persist(record);
			}
			catch (odb::object_already_persistent e)
			{
				cout << "Persist for " << odb_type_name << " failed.\n";
				cout << "Primary key value: " << record->getPrimaryKey() << ". This object will not be converted\n";
			}
			//the nested records are flattened out
			int num = file->Num_Nest ();
			for (int i=1; i <= num; i++) 
			{
				file->Read (true);
				record = Adapter(*file, container);
				try 
				{
					db->persist(record);
				}
				catch (odb::object_already_persistent e)
				{
					cout << "Persist for " << odb_type_name << " failed.\n";
					cout << "Primary key value: " << record->getPrimaryKey() << ". This object will not be converted\n";
				}
			}
		}
		t.commit();
	}
	catch (odb::sqlite::database_exception e)
	{
		cout << "Convert" << odb_type_name <<"s failed. " <<e.message() << "\n";
		exit(0);
	}
}
template<class FileType, class ODBType, class KeyType>
void ConvertNoRef(TransimsNetwork *net, InputContainer &container, System_File_Type file_type, string odb_type_name, std::map<KeyType,shared_ptr<ODBType>> *container_map = NULL)
{
	if (!net->System_File_Flag(file_type))
		return;
	cout << "Converting " << odb_type_name << "\n";
	shared_ptr<ODBType> record;
	FileType *file = (FileType *) net->System_File_Handle (file_type);
	file->Rewind();
	try
	{
		auto_ptr<database> db (open_sqlite_database (net->path_to_database));
		transaction t (db->begin());
		while (file->Read(false))
		{		
			net->Show_Progress();
			record = AdapterNoRef(*file, container);
			if (container_map!=NULL)
				(*container_map)[record->getPrimaryKey()] = record;
			try 
			{
				db->persist(record);
			}
			catch (odb::object_already_persistent e)
			{
				cout << "Persist for " << odb_type_name << " failed.\n";
				cout << "Primary key value: " << record->getPrimaryKey() << ". This object will not be converted\n";
			}
			//the nested records are flattened out
			int num = file->Num_Nest ();
			for (int i=1; i <= num; i++) 
			{
				file->Read (true);
				record = AdapterNoRef(*file, container);
				try 
				{
					db->persist(record);
				}
				catch (odb::object_already_persistent e)
				{
					cout << "Persist for " << odb_type_name << " failed.\n";
					cout << "Primary key value: " << record->getPrimaryKey() << ". This object will not be converted\n";
				}
			}
		}
		t.commit();
	}
	catch (odb::sqlite::database_exception e)
	{
		cout << "Convert" << odb_type_name <<"s failed. " <<e.message() << "\n";
		exit(0);
	}
}

//This function is to be used when a nested record is defined as a separate classs. As a result the nested records will be stored in a separate table
template<class FileType, class ODBType, class KeyType, class NestedRecordType>
void ConvertNested(TransimsNetwork *net, InputContainer &container, System_File_Type file_type, string odb_type_name, std::map<KeyType,shared_ptr<ODBType>> *container_map = NULL)
{
	if (!net->System_File_Flag(file_type))
		return;
	cout << "Converting " << odb_type_name << " with nested records\n";
	shared_ptr<ODBType> record;
	NestedRecordType nested_record;
	FileType *file = (FileType *) net->System_File_Handle (file_type);
	try
	{
		auto_ptr<database> db (open_sqlite_database (net->path_to_database));
		transaction t (db->begin());
		while (file->Read(false))
		{		
			net->Show_Progress();
			record = Adapter(*file, container);
			int num = file->Num_Nest ();
			for (int i=1; i <= num; i++) 
			{
				file->Read (true);
				nested_record = AdapterNested(*file, container);
				record->nested_records.push_back(nested_record);
			}
			if (container_map!=NULL)
				(*container_map)[record->getPrimaryKey()] = record;
			try 
			{
				db->persist(record);
			}
			catch (odb::object_already_persistent e)
			{
				cout << "Persist for " << odb_type_name << " failed.\n";
				cout << "Primary key value: " << record->getPrimaryKey() << ". This object will not be converted\n";
			}

		}
		t.commit();
	}
	catch (odb::sqlite::database_exception e)
	{
		cout << "Convert" << odb_type_name <<"s failed. " <<e.message() << "\n";
		exit(0);
	}
}

void AddIndeces(TransimsNetwork *net)
{
	auto_ptr<database> db (open_sqlite_database (net->path_to_database));
	connection_ptr c (db->connection ());
	c->execute("CREATE UNIQUE INDEX IF NOT EXISTS IDX_CONNECTION ON CONNECTION (LINK, T0_LINK, DIR);");
	c->execute("CREATE UNIQUE INDEX IF NOT EXISTS IDX_SIGN ON SIGN (LINK, DIR);");
}

void AddSpatialiteGeometry(TransimsNetwork *net)
{
	ofstream fh, fh1;
	fh.open("sql_trace.txt");
	fh1.open("sql_profile.txt");
	char sql[2048];
	char buff[1024];
	string geometry;
    sqlite3 *db_handle;
    sqlite3_stmt *stmt;
    int ret;
	size_t pos;
    char *err_msg = NULL;
	int link_id, zone_id, node_id;
	double x,y;
	shape_geometry p;
	vector<shape_geometry> points;
	vector<shape_geometry>::iterator points_it;
	// link_id --> shape points 
	map<int, vector<shape_geometry>> shapes; map<int, vector<shape_geometry>>::iterator it;	
	// zone_id --> ceneter coordinates
	map<int, shape_geometry> zone_locations; map<int, shape_geometry>::iterator zone_it;
	// node_id -> node coordinates
	map<int, shape_geometry> node_locations; map<int, shape_geometry>::iterator node_it;
	vector<shape_geometry>::iterator pt_it;
	typedef odb::query<Shape> query_shape;
	typedef odb::result<Shape> result_shape;
	typedef odb::query<Zone> query_zone;
	typedef odb::result<Zone> result_zone;
	typedef odb::query<Node> query_node;
	typedef odb::result<Node> result_node;

	typedef odb::query<Link> query_link;
	typedef odb::result<Link> result_link;

	auto_ptr<database> db (open_sqlite_database (net->path_to_database));
	transaction t(db->begin());

	result_link rl(db->query<Link> (query_link::true_expr));
	int count = 0;
	cout << "Adding geo column to LINK table\n";
	//add anode
	for (result_link::iterator i (rl.begin()); i!=rl.end(); ++i)
	{
		points.clear();
		shared_ptr<Node> anode = i->getNode_A();
		p.x = anode->getX(); p.y = anode->getY(); p.z = anode->getZ();
		points.push_back(p);
		shapes[i->getLink()] = points;
	}
	//add shapes
	result_shape rs(db->query<Shape> (query_shape::true_expr));
	for (result_shape::iterator i (rs.begin()); i!=rs.end(); ++i)
	{
		points.clear();
		link_id = i->getLink()->getLink();
		shapes[link_id].insert(shapes[link_id].end(), i->nested_records.begin(), i->nested_records.end());
	}
	//add bnode
	result_link rl1(db->query<Link> (query_link::true_expr));
	for (result_link::iterator i (rl1.begin()); i!=rl1.end(); ++i)
	{
		shared_ptr<Node> bnode = i->getNode_B();
		p.x = bnode->getX(); p.y = bnode->getY(); p.z = bnode->getZ();
		shapes[i->getLink()].push_back(p);
		//cout << "Shapes size for link " << i->getLink() << " is " << shapes[i->getLink()].size() << "\n";
	}

	//get all the records
	result_zone rz(db->query<Zone> (query_zone::true_expr));
	for (result_zone::iterator i (rz.begin()); i!=rz.end(); ++i)
	{
		shape_geometry p;
		zone_id = i->getZone();
		p.x = i->getX();
		p.y = i->getY();
		p.z = i->getZ();
		zone_locations[zone_id] = p;
	}

	//get all the records
	result_node rn(db->query<Node> (query_node::true_expr));
	for (result_node::iterator i (rn.begin()); i!=rn.end(); ++i)
	{
		shape_geometry p;
		node_id = i->getNode();
		p.x = i->getX();
		p.y = i->getY();
		p.z = i->getZ();
		node_locations[node_id] = p;
	}
	t.commit();
	if (zone_locations.size() == 0 && shapes.size() == 0 && node_locations.size() == 0)
		return;

	ret = sqlite3_open_v2(make_name(net->path_to_database,"Supply").c_str(), &db_handle, SQLITE_OPEN_READWRITE , NULL);
	ret = AddGeometryTables(db_handle, net->srid);
		
	if (zone_locations.size() != 0)
	{		
		cout << "Adding zone coordinates as geometry column" << "\n";
		ret = sqlite3_exec (db_handle, "BEGIN", NULL, NULL, &err_msg);
		if (ret != SQLITE_OK)
		{
			fprintf (stderr, "Error: %s\n", err_msg);
			sqlite3_free (err_msg);
			goto stop;
		}
		for (zone_it = zone_locations.begin(); zone_it!=zone_locations.end(); zone_it++)
		{
			zone_id = zone_it->first;
			shape_geometry pos = zone_it->second;
			sprintf (buff, "UPDATE Zone SET GEO=GeomFromText('POINT (%1.6f %1.6f)', %d) WHERE zone=%d",pos.x, pos.y, net->srid, zone_id);
			strcpy (sql, buff);
			ret = sqlite3_exec(db_handle, sql, NULL, NULL, &err_msg);
			if (ret != SQLITE_OK)
			{
				fprintf (stderr, "Error: %s\n", err_msg);
				sqlite3_free (err_msg);
				goto stop;
			}
		}
		ret = sqlite3_exec (db_handle, "COMMIT", NULL, NULL, &err_msg);
		if (ret != SQLITE_OK)
		{
			fprintf (stderr, "Error: %s\n", err_msg);
			sqlite3_free (err_msg);
		}
	}

	if (node_locations.size() != 0)
	{		
		cout << "Adding node coordinates as geometry column" << "\n";
		ret = sqlite3_exec (db_handle, "BEGIN", NULL, NULL, &err_msg);
		if (ret != SQLITE_OK)
		{
			fprintf (stderr, "Error: %s\n", err_msg);
			sqlite3_free (err_msg);
			goto stop;
		}
		for (node_it = node_locations.begin(); node_it!=node_locations.end(); node_it++)
		{
			node_id = node_it->first;
			shape_geometry pos = node_it->second;
			sprintf (buff, "UPDATE Node SET GEO=GeomFromText('POINT (%1.6f %1.6f)', %d) WHERE node=%d",pos.x, pos.y, net->srid, node_id);
			strcpy (sql, buff);
			ret = sqlite3_exec(db_handle, sql, NULL, NULL, &err_msg);
			if (ret != SQLITE_OK)
			{
				fprintf (stderr, "Error: %s\n", err_msg);
				sqlite3_free (err_msg);
				goto stop;
			}
		}
		ret = sqlite3_exec (db_handle, "COMMIT", NULL, NULL, &err_msg);
		if (ret != SQLITE_OK)
		{
			fprintf (stderr, "Error: %s\n", err_msg);
			sqlite3_free (err_msg);
		}
	}

	if (shapes.size() != 0)
	{
		cout << "Adding shapes as geometry column" << "\n";
		
		//add geometry column to links table
		
		#ifdef _DEBUG
			sqlite3_trace(db_handle,trace_callback, fh);
		#endif
			sprintf (buff, "UPDATE Link SET GEO=GeomFromText(?, %d) WHERE link=?", net->srid);
		strcpy (sql, buff);
		ret = sqlite3_prepare_v2 (db_handle, sql, strlen (sql), &stmt, NULL);
		if (ret != SQLITE_OK)
		{
			fprintf (stderr, "SQL error: %s\n%s\n", sql,
			sqlite3_errmsg (db_handle));
			goto stop;
		}
		ret = sqlite3_exec (db_handle, "BEGIN", NULL, NULL, &err_msg);
		if (ret != SQLITE_OK)
		{
			fprintf (stderr, "Error: %s\n", err_msg);
			sqlite3_free (err_msg);
			goto stop;
		}
		for (it=shapes.begin(); it!=shapes.end(); it++)
		{	
			link_id = it->first;
			//cout << link_id << "\n";
			geometry = "LINESTRING(";
			for (pt_it=it->second.begin(); pt_it!=it->second.end(); pt_it++) 
			{
				x = pt_it->x;
				y = pt_it->y;			
				sprintf (buff, "%1.6f %1.6f, ", x, y);
				geometry += buff;
			}
			pos = geometry.find_last_of(",");
			geometry = geometry.substr(0, pos);
			geometry += ")";
			sqlite3_reset (stmt);
			sqlite3_clear_bindings (stmt);
			sqlite3_bind_int (stmt, 2, link_id);
			sqlite3_bind_text (stmt, 1, geometry.c_str(), strlen (geometry.c_str()), SQLITE_STATIC);
			ret = sqlite3_step (stmt);
			if (ret == SQLITE_DONE || ret == SQLITE_ROW)
				continue;
			else
			{
				fprintf (stderr, "Error: %s\n", err_msg);
				sqlite3_free (err_msg);
				goto stop;
			}
		}
		sqlite3_finalize (stmt);
		ret = sqlite3_exec (db_handle, "COMMIT", NULL, NULL, &err_msg);
		if (ret != SQLITE_OK)
		{
			fprintf (stderr, "Error: %s\n", err_msg);
			sqlite3_free (err_msg);
		}
	}
	stop:
		sqlite3_close (db_handle);
}
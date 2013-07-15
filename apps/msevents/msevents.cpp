#include <string>
#include "Io/Io.h"

int main(int argc, char** argv)
{
	using namespace std;
	using namespace polaris;
	if (argc < 3) 
	{
		cout << "Usage: " << argv[0] << "<db_prefix> [target_db_prefix] <incident_file_path>\n";
		return 1;
	}
	string db_prefix, incident_file_path, target_db_prefix;
	db_prefix = argv[1];
	if (argc == 3)
	{
		incident_file_path = argv[2];
		target_db_prefix =  argv[1];
	}
	else
	{
		incident_file_path = argv[3];
		target_db_prefix =  argv[2];
	}
	string line;
	vector<int> incident;
	int id;
	ifstream  fh (incident_file_path);
	if (fh.is_open() && fh != NULL)
	{
		while (fh.good())
		{
			getline(fh,line);
			id = atoi(line.c_str());
			incident.push_back(id);
		}
	}
	cout << "Number of incidents to be converted: " << incident.size() << "\n";

	typedef odb::query<polaris::io::Microsoft_Event> query;
	typedef odb::result<polaris::io::Microsoft_Event> result;
	unique_ptr<odb::database> db = open_sqlite_database(db_prefix);
	unique_ptr<odb::database> db_to = open_sqlite_database(target_db_prefix);
	odb::transaction t(db->begin());
	odb::transaction t_to(db_to->begin());
	result r;
	result::iterator r_it;
	int count = 0;
	for (vector<int>::iterator i (incident.begin()); i!=incident.end(); ++i)
	{
		r = db->query<polaris::io::Microsoft_Event>(query::incidentId == *i);
		r_it = r.begin();


	}

	return 0;

}
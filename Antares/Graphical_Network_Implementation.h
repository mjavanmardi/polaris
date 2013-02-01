#pragma once
#include "User_Space\Network_Prototype.h"

namespace Network_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Graphical_Network_Implementation:public Polaris_Component_Class<Graphical_Network_Implementation,MasterType,Data_Object,ParentType>
		{
			feature_implementation void read_network_data(string& db_file)
			{
				using namespace odb;
				using namespace pio;

				auto_ptr<database> db (open_sqlite_database (db_file));
				transaction t(db->begin());

				result<Link> link_result=db->query<Link>(query<Link>::true_expr);
				
				int counter=0;

				cout << "Reading Links" << endl;
				
				int link_counter=-1;

				for(result<Link>::iterator db_itr = link_result.begin (); db_itr != link_result.end (); ++db_itr)
				{
					counter++;
					if(counter%10000==0) cout << "\t" << counter << endl;
				}


			}
		};
	}
}

using namespace Network_Components::Implementations;
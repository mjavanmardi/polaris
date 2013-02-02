//*********************************************************
//	Graphical_Network_Implementation.h - Graphical Network Variant
//*********************************************************

#pragma once
#include "User_Space\Network_Prototype.h"
#include "Geometry_Implementation.h"
#include "Graphical_Link_Implementation.h"

namespace Network_Components
{
	namespace Implementations
	{
		//---------------------------------------------------------
		//	Graphical_Network_Implementation - network class definition
		//---------------------------------------------------------

		implementation struct Graphical_Network_Implementation:public Polaris_Component_Class<Graphical_Network_Implementation,MasterType,Data_Object,ParentType>
		{
			feature_implementation void read_network_data(string& db_file)
			{
				using namespace odb;
				using namespace pio;

				auto_ptr<database> db (open_sqlite_database (db_file));
				transaction t(db->begin());

				dense_hash_map<int,Point_2D<MasterType>> nodes;
				nodes.set_deleted_key(-1);
				nodes.set_empty_key(-2);

				// reset bounds, they will be set in the node reading function
				_network_bounds.reset<type_of(network_bounds),Graphical_Network_Implementation,NULLTYPE>();
				
				// read nodes
				read_nodes(db,nodes);

				// set up input_offset and shift network bounds
				_input_offset._x = -( _network_bounds._xmax + _network_bounds._xmin )/2.0f;

				_network_bounds._xmax += _input_offset._x;
				_network_bounds._xmin += _input_offset._x;

				_input_offset._y = -( _network_bounds._ymax + _network_bounds._ymin )/2.0f;

				_network_bounds._ymax += _input_offset._y;
				_network_bounds._ymin += _input_offset._y;
				
				// read links
				read_links(db,nodes);
			}
			
			void read_nodes(auto_ptr<database>& db,dense_hash_map<int,Point_2D<MasterType>>& nodes)
			{
				using namespace odb;
				using namespace pio;

				result<Node> node_result=db->query<Node>(query<Node>::true_expr);
				
				int counter=0;

				cout << "Reading Graphical Nodes" << endl;
				
				Point_2D<MasterType> current_node;

				for(result<Node>::iterator db_itr = node_result.begin(); db_itr != node_result.end(); ++db_itr)
				{
					current_node._x=db_itr->getX();
					current_node._y=db_itr->getY();

					if(current_node._x < _network_bounds._xmin)
					{
						_network_bounds._xmin = current_node._x;
					}
					else if(current_node._x > _network_bounds._xmax)
					{
						_network_bounds._xmax = current_node._x;
					}
					
					if(current_node._y < _network_bounds._ymin)
					{
						_network_bounds._ymin = current_node._y;
					}
					else if(current_node._y > _network_bounds._ymax)
					{
						_network_bounds._ymax = current_node._y;
					}

					nodes[db_itr->getNode()]=current_node;
				}
			}
			
			void read_links(auto_ptr<database>& db,dense_hash_map<int,Point_2D<MasterType>>& nodes)
			{
				using namespace odb;
				using namespace pio;

				result<Link> link_result=db->query<Link>(query<Link>::true_expr);

				cout << "Reading Graphical Links" << endl;

				Point_2D<MasterType> current_point;

				for(result<Link>::iterator db_itr = link_result.begin(); db_itr != link_result.end(); ++db_itr)
				{
					int anode=db_itr->getNode_A()->getNode();

					current_point._x = nodes[anode]._x + _input_offset._x;
					current_point._y = nodes[anode]._y + _input_offset._y;

					_link_lines.push_back(current_point);

					int bnode=db_itr->getNode_B()->getNode();

					current_point._x = nodes[bnode]._x + _input_offset._x;
					current_point._y = nodes[bnode]._y + _input_offset._y;

					_link_lines.push_back(current_point);
				}
			}
			
			member_data(Point_2D<MasterType>, input_offset,none,none);
			member_data(Rectangle_XY<MasterType>, network_bounds,none,none);
			member_data(vector< Point_2D<MasterType> >, link_lines,none,none);
		};
	}
}

using namespace Network_Components::Implementations;
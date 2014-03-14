#pragma once
#include "Network_Implementation.h"
#include "Routing/Routing.h"

namespace Routing_Components
{
	namespace Types
	{
		template<typename MasterType>
		struct static_attributes : public Base_Edge_A_Star<MasterType>
		{

		};

		struct static_to_static
		{
			t_data(float,cost);
			t_data(float,time_cost);
		};
	}

	namespace Implementations
	{


		implementation struct Routable_Network_Implementation:public Polaris_Component<MasterType,INHERIT(Routable_Network_Implementation),Data_Object>
		{
			typedef typename Polaris_Component<MasterType,INHERIT(Routable_Network_Implementation),Data_Object>::Component_Type ComponentType;

			m_data(unsigned int,static_network_graph_id,NONE,NONE);

			m_data(Graph_Pool<typename MT::graph_pool_type>*,routable_graph_pool,NONE,NONE);

			Routable_Network<ComponentType>* create_copy()
			{
				Routable_Network_Implementation* copy = Allocate<ComponentType>();

				Graph_Pool<MT::graph_pool_type>* graph_copy = _routable_graph_pool->Create_Copy();

				copy->_routable_graph_pool = graph_copy;
				copy->_static_network_graph_id = _static_network_graph_id;

				return (Routable_Network<ComponentType>*)copy;
			}

			void construct_routable_network(Network<typename MasterType::network_type>* source_network)
			{
				typedef Network<typename MasterType::network_type> Network_Interface;

				typedef Link<remove_pointer<Network_Interface::get_type_of(links_container)::value_type>::type> Link_Interface;
				typedef Random_Access_Sequence<Network_Interface::get_type_of(links_container),Link_Interface*> Link_Container_Interface;
				typedef Intersection<remove_pointer<Network_Interface::get_type_of(intersections_container)::value_type>::type> Intersection_Interface;

				typedef Movement<remove_pointer<Link_Interface::get_type_of(outbound_turn_movements)::value_type>::type> Turn_Movement_Interface;
				typedef Random_Access_Sequence<Link_Interface::get_type_of(outbound_turn_movements),Turn_Movement_Interface*> Turn_Movement_Container_Interface;




				Graph_Pool<MT::graph_pool_type>* graph_pool = (Graph_Pool<MT::graph_pool_type>*) new MT::graph_pool_type();

				_routable_graph_pool = graph_pool;

				Graph_Assembler_Connected_Edge<MT::static_graph_type>* static_graph = graph_pool->Create_New_Graph<MT::static_graph_type>();

				_static_network_graph_id = static_graph->graph_id();

				Input_Edge<Types::static_attributes<MT>> input_static_edge;

				Input_Connection_Group_Implementation<typename MT::static_to_static_connection_type>::_neighbor_graph_id = static_graph->graph_id();

				Input_Connection_Group_Implementation<typename MT::static_to_static_connection_type>* static_to_static_connection_group = new Input_Connection_Group_Implementation<typename MT::static_to_static_connection_type>();
	
				Types::static_to_static connection_attributes;


				Network_Interface* network = source_network;

				Link_Container_Interface* links = network->links_container<Link_Container_Interface*>();

				for(Link_Container_Interface::iterator links_itr=links->begin();links_itr!=links->end();links_itr++)
				{
					Link_Interface* current_link = (Link_Interface*)(*links_itr);
		
					Intersection_Interface* downstream_intersection = current_link->downstream_intersection<Intersection_Interface*>();

					input_static_edge._x = downstream_intersection->x_position<float>();
					input_static_edge._y = downstream_intersection->y_position<float>();
					input_static_edge._edge_id = current_link->uuid<unsigned int>();

					input_static_edge._cost = current_link->travel_time<float>();
					input_static_edge._time_cost = current_link->travel_time<float>();

					Turn_Movement_Container_Interface* outbound_turn_movements = current_link->outbound_turn_movements<Turn_Movement_Container_Interface*>();

					for(Turn_Movement_Container_Interface::iterator movements_itr=outbound_turn_movements->begin();movements_itr!=outbound_turn_movements->end();movements_itr++)
					{
						Turn_Movement_Interface* current_movement = (Turn_Movement_Interface*)(*movements_itr);

						long long neighbor_id = current_movement->outbound_link<Link_Interface*>()->uuid<int>();

						static_to_static_connection_group->_neighbors.push_back(neighbor_id);

						connection_attributes._cost = 0.0f;
						connection_attributes._time_cost = 0.0f;

						static_to_static_connection_group->_neighbor_attributes.push_back(connection_attributes);
					}

					input_static_edge._connection_groups.push_back(static_to_static_connection_group);

					static_graph->Add_Edge<Types::static_attributes<MT>>( &input_static_edge );

					// Clean up connection group

					static_to_static_connection_group->_neighbors.clear();
					static_to_static_connection_group->_neighbor_attributes.clear();

					// Clean up input edge

					input_static_edge._connection_groups.clear();
				}

				Interactive_Graph<MT::static_graph_type>* routable_network_graph = static_graph->Compile_Graph<Types::static_attributes<MT>>();
	
				graph_pool->Link_Graphs();
			}

			void test_routable_network()
			{
				Routable_Agent<typename MT::routable_agent_type> proxy_agent;

				boost::container::deque< global_edge_id > path;

				global_edge_id start;

				start.edge_id = 8751*2+0;
				start.graph_id = 0;
				
				global_edge_id end;

				end.edge_id = 9598*2+0;
				end.graph_id = 0;

				A_Star<MT,typename MT::routable_agent_type,typename MT::graph_pool_type>(&proxy_agent,_routable_graph_pool,start,end,0,path);

				for(boost::container::deque< global_edge_id >::iterator itr = path.begin();itr!=path.end();itr++)
				{
					cout << itr->edge_id /2 << endl;
				}
			}

			float compute_static_network_path(unsigned int origin, unsigned int destination, boost::container::deque<global_edge_id>& path_container, boost::container::deque<float>& cost_container)
			{
				Routable_Agent<typename MT::routable_agent_type> proxy_agent;

				global_edge_id start;

				start.edge_id = origin;
				start.graph_id = _static_network_graph_id;
				
				global_edge_id end;

				end.edge_id = destination;
				end.graph_id = _static_network_graph_id;

				float routed_time = A_Star<MT,typename MT::routable_agent_type,typename MT::graph_pool_type>(&proxy_agent,_routable_graph_pool,start,end,0,path_container,cost_container);

				return routed_time;
			}

			float compute_static_network_tree(unsigned int origin, boost::container::vector<float>& cost_container)
			{
				Tree_Agent<typename MT::tree_agent_type> proxy_agent;

				global_edge_id start;

				start.edge_id = origin;
				start.graph_id = _static_network_graph_id;
				
				//global_edge_id end;

				//end.edge_id = destination;
				//end.graph_id = _static_network_graph_id;

				float routed_time = A_Star_Tree<MT,typename MT::tree_agent_type,typename MT::graph_pool_type>(&proxy_agent,_routable_graph_pool,start,0,cost_container);

				return routed_time;
			}

			void update_edge_turn_cost(unsigned int edge_id,float edge_cost,unsigned int outbound_turn_index,float turn_cost)
			{
				global_edge_id edge_lookup;

				edge_lookup.edge_id = edge_id;
				edge_lookup.graph_id = _static_network_graph_id;

				A_Star_Edge<typename MasterType::static_edge_type>* edge = (A_Star_Edge<typename MasterType::static_edge_type>*)_routable_graph_pool->Get_Edge<typename MasterType::static_graph_type>(edge_lookup);

				edge->time_cost(edge_cost);
				edge->cost(edge_cost);

				Anonymous_Connection_Group<MasterType,Base_Edge_A_Star<MT>>* connection_group = edge->begin_connection_groups();

				while(connection_group != edge->end_connection_groups())
				{
					Connection_Group<typename MasterType::static_to_static_connection_type>* current_connection_group = (Connection_Group<typename MasterType::static_to_static_connection_type>*)connection_group;
					
					Connection<typename MasterType::static_to_static_connection_type::connection_type>* current_neighbor = current_connection_group->forward_edges();

					int i=0;

					while(current_neighbor != current_connection_group->end_forward_edges())
					{
						if(i == outbound_turn_index)
						{
							Types::static_to_static* current_connection_attributes = (Types::static_to_static*)current_neighbor->connection_attributes();

							current_connection_attributes->_cost = turn_cost;
							current_connection_attributes->_time_cost = turn_cost;

							break;
						}


						current_neighbor = current_neighbor->next_connection();
						i++;
					}

					connection_group = connection_group->Next_Connection_Group();
				}
			}

		};
	}
}
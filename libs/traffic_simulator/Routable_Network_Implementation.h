#pragma once
#include "Network_Implementation.h"
#include "Routing/Routing.h"

namespace Routing_Components
{
	template<typename DataRecord>
	struct Layered_Data_Array
	{
		Layered_Data_Array():_first_layer_id(-1),_layer_step(-1),_layer_tracker(-1),_layer_element_tracker(0){}

		//DataRecord* add_layer_element(DataRecord& record,int layer_id)
		unsigned int add_layer_element(DataRecord& record,int layer_id)
		{
			if(_first_layer_id == -1)
			{
				_first_layer_id = layer_id;
				_layer_tracker = layer_id;
			}
			else if(layer_id != _first_layer_id)
			{
				if(_layer_step == -1)
				{
					_layer_step = (layer_id - _first_layer_id);
					_layer_size = (int)_layer_data.size();
				}
			}
			
			if(layer_id != _layer_tracker)
			{
				if(_layer_element_tracker != _layer_size)
				{
					cout << "layer size not maintained! " << _layer_element_tracker << "," << _layer_size << endl;
					exit(0);
				}

				if((layer_id - _layer_tracker) != _layer_step)
				{
					cout << "layer step not maintained! " << _layer_tracker << "," << layer_id << endl;
					exit(0);
				}

				_layer_element_tracker = 0;
				_layer_tracker = layer_id;
			}
			
			++_layer_element_tracker;

			_layer_data.push_back(record);

			//return &_layer_data.back();
			return ((int)_layer_data.size() - 1);
		}
		
		DataRecord& get_closest_element(DataRecord* base_pointer,int layer_id)
		{
			layer_id = (layer_id - _first_layer_id)/_layer_step;
			layer_id *= _layer_step;
			layer_id += _first_layer_id;

			if(layer_id < _first_layer_id || layer_id > ((_layer_data.size()/_layer_size)*_layer_step+_first_layer_id))
			{
				int max_id = (int)((_layer_data.size()/_layer_size)*_layer_step+_first_layer_id);

				// wrap around to future days
				int days = layer_id/max_id;

				layer_id = layer_id - days*max_id;

				//cout << " closest layer is not available: " << layer_id << endl;
				//layer_id = layer_id - ((_layer_data.size()/_layer_size)*_layer_step+_first_layer_id);
				//exit(0);
			}

			return *(base_pointer+_layer_size*((layer_id-_first_layer_id)/_layer_step));
		}

		DataRecord& get_element(DataRecord* base_pointer,int layer_id)
		{
			if(layer_id < _first_layer_id || layer_id > ((_layer_data.size()/_layer_size)*_layer_step+_first_layer_id))
			{
				cout << "layer is not available: " << layer_id << endl;
				exit(0);
			}

			return *(base_pointer+_layer_size*((layer_id-_first_layer_id)/_layer_step));
		}
		
		DataRecord* get_element(unsigned int element_id)
		{
			return &(_layer_data[element_id]);
		}

		m_data(int,layer_tracker,NONE,NONE);
		m_data(int,layer_element_tracker,NONE,NONE);
		m_data(int,layer_size,NONE,NONE);
		m_data(int,first_layer_id,NONE,NONE);
		m_data(int,layer_step,NONE,NONE);

		m_data(boost::container::vector<DataRecord>,layer_data,NONE,NONE);
	};


	namespace Types
	{
		template<typename MasterType>
		struct static_attributes : public Base_Edge_A_Star<MasterType>
		{
			t_data(bool,is_highway);
		};

		struct static_to_static
		{
			t_data(float,cost);
			t_data(float,time_cost);
		};

		template<typename MasterType>
		struct time_dependent_attributes : public Base_Edge_A_Star<MasterType>
		{
			t_data(bool,is_highway);
			t_data(float*,moe_ptr);

			static t_data(Layered_Data_Array<float>*,moe_data);
		};
		
		template<typename MasterType>
		Layered_Data_Array<float>* time_dependent_attributes<MasterType>::_moe_data;

		struct time_dependent_to_time_dependent
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
			m_data(unsigned int,time_dependent_network_graph_id,NONE,NONE);

			m_data(Graph_Pool<typename MT::graph_pool_type>*,routable_graph_pool,NONE,NONE);

			static m_data(Layered_Data_Array<float>,moe_data,NONE,NONE);

			static m_data(concat(boost::unordered::unordered_map<int,int>),link_id_to_moe_data,NONE,NONE);

			static void initialize_moe_data()
			{
				cout << "reading historical moes" << endl;

				using namespace odb;
				using namespace polaris::io;
				typedef Scenario_Components::Prototypes::Scenario< typename MasterType::scenario_type> _Scenario_Interface;

				string name(polaris::io::make_name(((_Scenario_Interface*)_global_scenario)->template historical_results_database_name<string&>(), polaris::io::db_inventory[1]));
				if (name == "")
				{
					cout << "Error: historical results database name is required for time-dependent routing.  Use the 'historical_results_database_name' key in the scenario file.";
					assert(false);
				}
				//string name(((_Scenario_Interface*)_global_scenario)->template database_name<string&>());
				shared_ptr<database> db (open_sqlite_database_single<shared_ptr<database>>(name));
				transaction t(db->begin());
				result<LinkMOE> moe_result=db->template query<LinkMOE>(query<LinkMOE>::true_expr);
				

				int link_id;
				float travel_delay;
				int start_time = -1;
				int counter = -1;

				bool time_advance = false;

				for(typename result<LinkMOE>::iterator db_itr = moe_result.begin (); db_itr != moe_result.end (); ++db_itr)
				{
					if(++counter%100000 == 0) cout << counter << endl;

					if(start_time == -1) start_time= db_itr->getStart_Time();

					if(start_time != db_itr->getStart_Time())
					{
						time_advance = true;
					}

					start_time = db_itr->getStart_Time();
					link_id = db_itr->getLink_Uid();
					travel_delay = db_itr->getLink_Travel_Time()*60.0f;

					//if(travel_delay < 0.01f) cout << travel_delay << endl;

					unsigned int ptr = _moe_data.add_layer_element(travel_delay,start_time);

					if(!time_advance)
					{
						_link_id_to_moe_data[link_id] = ptr;
						time_advance = false;
					}
				}

			}

			Routable_Network<ComponentType>* create_copy()
			{
				Routable_Network_Implementation* copy = Allocate<ComponentType>();

				Graph_Pool<MT::graph_pool_type>* graph_copy = _routable_graph_pool->Create_Copy();

				copy->_routable_graph_pool = graph_copy;
				copy->_static_network_graph_id = _static_network_graph_id;

				return (Routable_Network<ComponentType>*)copy;
			}

			void initialize()
			{
				_routable_graph_pool = (Graph_Pool<MT::graph_pool_type>*) new MT::graph_pool_type();
			}

			void construct_time_dependent_routable_network(Network<typename MasterType::network_type>* source_network)
			{

				Types::time_dependent_attributes<MT>::_moe_data = &_moe_data;

				typedef Network<typename MasterType::network_type> Network_Interface;

				typedef Link_Components::Prototypes::Link<remove_pointer<Network_Interface::get_type_of(links_container)::value_type>::type> Link_Interface;
				typedef Random_Access_Sequence<Network_Interface::get_type_of(links_container),Link_Interface*> Link_Container_Interface;
				typedef Intersection<remove_pointer<Network_Interface::get_type_of(intersections_container)::value_type>::type> Intersection_Interface;

				typedef Movement<remove_pointer<Link_Interface::get_type_of(outbound_turn_movements)::value_type>::type> Turn_Movement_Interface;
				typedef Random_Access_Sequence<Link_Interface::get_type_of(outbound_turn_movements),Turn_Movement_Interface*> Turn_Movement_Container_Interface;




				//Graph_Pool<MT::graph_pool_type>* graph_pool = (Graph_Pool<MT::graph_pool_type>*) new MT::graph_pool_type();

				//_routable_graph_pool = graph_pool;

				Graph_Pool<MT::graph_pool_type>* graph_pool = _routable_graph_pool;

				
				
				Graph_Assembler_Connected_Edge<MT::time_dependent_graph_type>* time_dependent_graph = graph_pool->Create_New_Graph<MT::time_dependent_graph_type>();

				_time_dependent_network_graph_id = time_dependent_graph->graph_id();

				Input_Edge<Types::time_dependent_attributes<MT>> input_time_dependent_edge;

				Input_Connection_Group_Implementation<typename MT::time_dependent_to_time_dependent_connection_type>::_neighbor_graph_id = time_dependent_graph->graph_id();

				Input_Connection_Group_Implementation<typename MT::time_dependent_to_time_dependent_connection_type>* time_dependent_to_time_dependent_connection_group = new Input_Connection_Group_Implementation<typename MT::time_dependent_to_time_dependent_connection_type>();
	
				Types::time_dependent_to_time_dependent connection_attributes;


				Network_Interface* network = source_network;

				Link_Container_Interface* links = network->links_container<Link_Container_Interface*>();

				for(Link_Container_Interface::iterator links_itr=links->begin();links_itr!=links->end();links_itr++)
				{
					Link_Interface* current_link = (Link_Interface*)(*links_itr);
		
					Intersection_Interface* downstream_intersection = current_link->downstream_intersection<Intersection_Interface*>();

					input_time_dependent_edge._x = downstream_intersection->template x_position<float>();
					input_time_dependent_edge._y = downstream_intersection->template y_position<float>();
					input_time_dependent_edge._edge_id = current_link->uuid<unsigned int>();

					input_time_dependent_edge._cost = current_link->template travel_time<float>();
					input_time_dependent_edge._time_cost = current_link->template travel_time<float>();
					
					Link_Components::Types::Link_Type_Keys link_type = current_link->link_type<Link_Components::Types::Link_Type_Keys>();

					cout <<endl<<"Trying to find "<<current_link->template uuid<int>()<<" in list:"<<endl;
					for (boost::unordered::unordered_map<int,int>::iterator itr = _link_id_to_moe_data.begin(); itr != _link_id_to_moe_data.end(); ++itr)
					{
						cout <<itr->first<<","<<itr->second<<endl;
					}
					
					if(_link_id_to_moe_data.count(current_link->template uuid<int>()))
					{
						input_time_dependent_edge._moe_ptr = _moe_data.get_element(_link_id_to_moe_data[current_link->template uuid<int>()]);
					}
					else
					{
						input_time_dependent_edge._moe_ptr = nullptr;
						//cout << "unable to find a corresponding moe for link: " << current_link->template dbid<int>() << endl;
						//exit(0);
					}

					if(link_type == Link_Components::Types::Link_Type_Keys::ARTERIAL || link_type == Link_Components::Types::Link_Type_Keys::LOCAL)
					{
						input_time_dependent_edge._is_highway = false;
					}
					else
					{
						input_time_dependent_edge._is_highway = true;
					}
					

					Turn_Movement_Container_Interface* outbound_turn_movements = current_link->outbound_turn_movements<Turn_Movement_Container_Interface*>();

					for(Turn_Movement_Container_Interface::iterator movements_itr=outbound_turn_movements->begin();movements_itr!=outbound_turn_movements->end();movements_itr++)
					{
						Turn_Movement_Interface* current_movement = (Turn_Movement_Interface*)(*movements_itr);

						long long neighbor_id = current_movement->outbound_link<Link_Interface*>()->template uuid<int>();

						time_dependent_to_time_dependent_connection_group->_neighbors.push_back(neighbor_id);

						connection_attributes._cost = 0.0f;
						connection_attributes._time_cost = 0.0f;

						time_dependent_to_time_dependent_connection_group->_neighbor_attributes.push_back(connection_attributes);
					}

					input_time_dependent_edge._connection_groups.push_back(time_dependent_to_time_dependent_connection_group);

					time_dependent_graph->Add_Edge<Types::time_dependent_attributes<MT>>( &input_time_dependent_edge );

					// Clean up connection group

					time_dependent_to_time_dependent_connection_group->_neighbors.clear();
					time_dependent_to_time_dependent_connection_group->_neighbor_attributes.clear();

					// Clean up input edge

					input_time_dependent_edge._connection_groups.clear();
				}

				Interactive_Graph<MT::time_dependent_graph_type>* routable_network_graph = time_dependent_graph->Compile_Graph<Types::time_dependent_attributes<MT>>();
	
				//graph_pool->Link_Graphs();
			}

			void finalize()
			{
				_routable_graph_pool->Link_Graphs();
			}

			void construct_routable_network(Network<typename MasterType::network_type>* source_network)
			{
				typedef Network<typename MasterType::network_type> Network_Interface;

				typedef Link_Components::Prototypes::Link<remove_pointer<Network_Interface::get_type_of(links_container)::value_type>::type> Link_Interface;
				typedef Random_Access_Sequence<Network_Interface::get_type_of(links_container),Link_Interface*> Link_Container_Interface;
				typedef Intersection<remove_pointer<Network_Interface::get_type_of(intersections_container)::value_type>::type> Intersection_Interface;

				typedef Movement<remove_pointer<Link_Interface::get_type_of(outbound_turn_movements)::value_type>::type> Turn_Movement_Interface;
				typedef Random_Access_Sequence<Link_Interface::get_type_of(outbound_turn_movements),Turn_Movement_Interface*> Turn_Movement_Container_Interface;



				Graph_Pool<MT::graph_pool_type>* graph_pool = _routable_graph_pool;


				Graph_Assembler_Connected_Edge<MT::static_graph_type>* static_graph = graph_pool->Create_New_Graph<MT::static_graph_type>();

				_static_network_graph_id = static_graph->graph_id();
				//contains link properties (link properties). Its is a class to be used during graph construction to do things like creating a copy, it also contains a queue of connection groups
				//static_attribues inherit from A* and hold attributes necessary for running A*
				Input_Edge<Types::static_attributes<MT>> input_static_edge;
				//information between which links and graphs connection happens
				Input_Connection_Group_Implementation<typename MT::static_to_static_connection_type>::_neighbor_graph_id = static_graph->graph_id();

				Input_Connection_Group_Implementation<typename MT::static_to_static_connection_type>* static_to_static_connection_group = new Input_Connection_Group_Implementation<typename MT::static_to_static_connection_type>();
				//additional cost for turn movements
				Types::static_to_static connection_attributes;


				Network_Interface* network = source_network;

				Link_Container_Interface* links = network->links_container<Link_Container_Interface*>();

				for(Link_Container_Interface::iterator links_itr=links->begin();links_itr!=links->end();links_itr++)
				{
					Link_Interface* current_link = (Link_Interface*)(*links_itr);
		
					Intersection_Interface* downstream_intersection = current_link->downstream_intersection<Intersection_Interface*>();

					input_static_edge._x = downstream_intersection->template x_position<float>();
					input_static_edge._y = downstream_intersection->template y_position<float>();
					input_static_edge._edge_id = current_link->uuid<unsigned int>();

					input_static_edge._cost = current_link->template travel_time<float>();
					input_static_edge._time_cost = current_link->template travel_time<float>();
					
					Link_Components::Types::Link_Type_Keys link_type = current_link->link_type<Link_Components::Types::Link_Type_Keys>();
					
					if(link_type == Link_Components::Types::Link_Type_Keys::ARTERIAL || link_type == Link_Components::Types::Link_Type_Keys::LOCAL)
					{
						input_static_edge._is_highway = false;
					}
					else
					{
						input_static_edge._is_highway = true;
					}
					

					Turn_Movement_Container_Interface* outbound_turn_movements = current_link->outbound_turn_movements<Turn_Movement_Container_Interface*>();

					for(Turn_Movement_Container_Interface::iterator movements_itr=outbound_turn_movements->begin();movements_itr!=outbound_turn_movements->end();movements_itr++)
					{
						Turn_Movement_Interface* current_movement = (Turn_Movement_Interface*)(*movements_itr);

						long long neighbor_id = current_movement->outbound_link<Link_Interface*>()->template uuid<int>();

						static_to_static_connection_group->_neighbors.push_back(neighbor_id);

						connection_attributes._cost = 0.0f;
						connection_attributes._time_cost = 0.0f;

						static_to_static_connection_group->_neighbor_attributes.push_back(connection_attributes);
					}

					//each edge can have multiple connection groups, like bus->walk or bus->walk. The use pattern is to put connections of the same type into separate group, each group will have a different types of elements
					input_static_edge._connection_groups.push_back(static_to_static_connection_group);

					static_graph->Add_Edge<Types::static_attributes<MT>>( &input_static_edge );

					// Clean up connection group

					static_to_static_connection_group->_neighbors.clear();
					static_to_static_connection_group->_neighbor_attributes.clear();

					// Clean up input edge

					input_static_edge._connection_groups.clear();
				}
				//reorganizes data that holds information for a graph structure
				Interactive_Graph<MT::static_graph_type>* routable_network_graph = static_graph->Compile_Graph<Types::static_attributes<MT>>();
	
				//graph_pool->Link_Graphs();
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
			//currently calls A* algorithm
			float compute_static_network_path(unsigned int origin, unsigned int destination, boost::container::deque<global_edge_id>& path_container, boost::container::deque<float>& cost_container)
			{
				//use hamogeneous agent for now
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

			float compute_time_dependent_network_path(unsigned int origin, unsigned int destination, unsigned int start_time, boost::container::deque<global_edge_id>& path_container, boost::container::deque<float>& cost_container)
			{
				//Routable_Agent<typename MT::time_dependent_agent_type> proxy_agent;
				Routable_Agent<typename MT::routable_agent_type> proxy_agent;

				global_edge_id start;

				start.edge_id = origin;
				start.graph_id = _time_dependent_network_graph_id;
				
				global_edge_id end;

				end.edge_id = destination;
				end.graph_id = _time_dependent_network_graph_id;

				//float routed_time = Time_Dependent_A_Star<MT,typename MT::time_dependent_agent_type,typename MT::graph_pool_type>(&proxy_agent,_routable_graph_pool,start,end,start_time,path_container,cost_container);
				float routed_time = Time_Dependent_A_Star<MT,typename MT::routable_agent_type,typename MT::graph_pool_type>(&proxy_agent,_routable_graph_pool,start,end,start_time,path_container,cost_container);

				return routed_time;
			}

			float compute_static_network_tree(unsigned int origin, boost::container::vector<float>& cost_container)
			{
				Routable_Agent<typename MT::tree_agent_type> proxy_agent;

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


				typedef Scenario_Components::Prototypes::Scenario< typename MasterType::scenario_type> _Scenario_Interface;


				if(((_Scenario_Interface*)_global_scenario)->template time_dependent_routing<bool>())
				{

					global_edge_id edge_lookup;

					edge_lookup.edge_id = edge_id;
					edge_lookup.graph_id = _time_dependent_network_graph_id;

					A_Star_Edge<typename MasterType::time_dependent_edge_type>* edge = (A_Star_Edge<typename MasterType::time_dependent_edge_type>*)_routable_graph_pool->Get_Edge<typename MasterType::time_dependent_graph_type>(edge_lookup);

					edge->time_cost(edge_cost);
					edge->cost(edge_cost);

					Anonymous_Connection_Group<MasterType,Base_Edge_A_Star<MT>>* connection_group = edge->begin_connection_groups();

					while(connection_group != edge->end_connection_groups())
					{
						Connection_Group<typename MasterType::time_dependent_to_time_dependent_connection_type>* current_connection_group = (Connection_Group<typename MasterType::time_dependent_to_time_dependent_connection_type>*)connection_group;
					
						Connection<typename MasterType::time_dependent_to_time_dependent_connection_type::connection_type>* current_neighbor = current_connection_group->forward_edges();

						int i=0;

						while(current_neighbor != current_connection_group->end_forward_edges())
						{
							if(i == outbound_turn_index)
							{
								Types::time_dependent_to_time_dependent* current_connection_attributes = (Types::time_dependent_to_time_dependent*)current_neighbor->connection_attributes();

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
			}
		};

		template<typename MasterType, typename InheritanceList>
		Layered_Data_Array<float> Routable_Network_Implementation<MasterType,InheritanceList>::_moe_data;

		template<typename MasterType, typename InheritanceList>
		boost::unordered::unordered_map<int,int> Routable_Network_Implementation<MasterType,InheritanceList>::_link_id_to_moe_data;
	}
}
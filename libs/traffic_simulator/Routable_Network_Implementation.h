#pragma once
#include "Network_Implementation.h"
#include "routing/Routing.h"

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
					THROW_EXCEPTION("layer size not maintained! " << _layer_element_tracker << "," << _layer_size);
				}

				if((layer_id - _layer_tracker) != _layer_step)
				{
					THROW_EXCEPTION("layer step not maintained! " << _layer_tracker << "," << layer_id);
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

			//TODO: changed this to >= since it seemed to be running past the end of the data - check with michael to verify
			if (layer_id < _first_layer_id)
			{
				layer_id = _first_layer_id;
			}
			else if(layer_id >= ((_layer_data.size()/_layer_size)*_layer_step+_first_layer_id))
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

		m_data(std::vector<DataRecord>,layer_data,NONE,NONE);

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

			t_static_data(Layered_Data_Array<float>*,moe_data);
			t_static_data(float, ttime_weight_shape);
			t_static_data(float, ttime_weight_scale);
			t_static_data(float, ttime_weight_factor);
		};
		
		template<typename MasterType>
		Layered_Data_Array<float>* time_dependent_attributes<MasterType>::_moe_data;
		template<typename MasterType>
		float time_dependent_attributes<MasterType>::_ttime_weight_shape;
		template<typename MasterType>
		float time_dependent_attributes<MasterType>::_ttime_weight_scale;
		template<typename MasterType>
		float time_dependent_attributes<MasterType>::_ttime_weight_factor;


		struct time_dependent_to_time_dependent
		{
			t_data(float,cost);
			t_data(float,time_cost);
			t_data(float*, turn_moe_ptr);

			t_static_data(Layered_Data_Array<float>*, turn_moe_data);
		};

		Layered_Data_Array<float>* time_dependent_to_time_dependent::_turn_moe_data;

		template<typename MasterType>
		struct multimodal_attributes : public Base_Edge_A_Star<MasterType>
		{
			t_data(bool, is_highway);
			t_data(float*, moe_ptr);

			t_data(std::vector<typename MasterType::transit_vehicle_trip_type*>, trips_by_dep_time);
			t_data(std::vector<int>, index_along_trip_at_upstream_node);
			t_data(std::vector<int>, trip_to_unique_pattern_index);
			t_data(std::vector<typename MasterType::transit_pattern_type*>, unique_patterns);
			t_data(std::vector<int>, index_along_pattern_at_upstream_node);
			
			//t_data(std::vector<bool>, unique_pattern_scanned);

			t_data(Link_Components::Types::Link_Type_Keys, edge_type);

			t_data(float, ivt_time_from_origin);
			t_data(float, car_time_from_origin);
			t_data(float, walk_time_from_origin);
			t_data(float, bike_time_from_origin);
			t_data(float, wait_time_from_origin);
			t_data(float, transfer_pen_from_origin);
			t_data(int, wait_count_from_origin);
			t_data(typename MasterType::transit_vehicle_trip_type*, came_on_trip);
			t_data(int, came_on_seq_index);

			t_static_data(Layered_Data_Array<float>*, moe_data);
			t_static_data(float, ttime_weight_shape);
			t_static_data(float, ttime_weight_scale);
			t_static_data(float, ttime_weight_factor);
		};

		template<typename MasterType>
		Layered_Data_Array<float>* multimodal_attributes<MasterType>::_moe_data;
		template<typename MasterType>
		float multimodal_attributes<MasterType>::_ttime_weight_shape;
		template<typename MasterType>
		float multimodal_attributes<MasterType>::_ttime_weight_scale;
		template<typename MasterType>
		float multimodal_attributes<MasterType>::_ttime_weight_factor;

		struct multimodal_to_multimodal
		{
			t_data(float, cost);
			t_data(float, time_cost);
			t_data(float*, turn_moe_ptr);

			t_data(float, trip_wait_time);

			t_static_data(Layered_Data_Array<float>*, turn_moe_data);
		};

		Layered_Data_Array<float>* multimodal_to_multimodal::_turn_moe_data;
	}


	namespace Implementations
	{


		implementation struct Routable_Network_Implementation:public Polaris_Component<MasterType,INHERIT(Routable_Network_Implementation),Data_Object>
		{
			typedef typename Polaris_Component<MasterType,INHERIT(Routable_Network_Implementation),Data_Object>::Component_Type ComponentType;
			typedef Scenario_Components::Prototypes::Scenario< typename MasterType::scenario_type> _Scenario_Interface;

			//m_prototype(Routing_Components::Prototypes::Routable_Network, typename MasterType::multimodal_routing_option_type, Routable_Network, NONE, NONE);

			m_data(unsigned int,static_network_graph_id,NONE,NONE);
			m_data(unsigned int,multimodal_network_graph_id, NONE, NONE);
			m_data(unsigned int,time_dependent_network_graph_id,NONE,NONE);

			m_data(Graph_Pool<typename MT::graph_pool_type>*,routable_graph_pool,NONE,NONE);

			m_static_data(Layered_Data_Array<float>,moe_data,NONE,NONE);
			m_static_data(concat(std::unordered_map<int,int>),link_id_to_moe_data,NONE,NONE);

			m_static_data(Layered_Data_Array<float>, turn_moe_data, NONE, NONE);
			m_static_data(concat(std::unordered_map<int, int>), turn_id_to_moe_data, NONE, NONE);
			
			static bool static_initialize(const string& option_file)
			{
				// set the base values
				default_static_initializer();

				// now see if there are config file changes
				Scenario_Components::Types::ScenarioData document;
				if (option_file.length() < 1)
				{
					cout << "Warning: option file for multimodal_routing was not specified" << endl;
					return true;
				}

				_Scenario_Interface* scenario = static_cast<_Scenario_Interface*>(_global_scenario);
				if (!scenario->parse_option_file(document, option_file))
					return false;

				scenario->set_parameter(document, "transferPenalty", _transferPenalty);
				scenario->set_parameter(document, "waitWeight", _waitWeight);
				scenario->set_parameter(document, "walkWeight", _walkWeight);
				scenario->set_parameter(document, "bikeWeight", _bikeWeight);
				scenario->set_parameter(document, "ivtWeight", _ivtWeight);
				scenario->set_parameter(document, "carWeight", _carWeight);
				scenario->set_parameter(document, "scanThreshold", _scanThreshold);
				scenario->set_parameter(document, "costThreshold", _costThreshold);
				scenario->set_parameter(document, "waitThreshold", _waitThreshold);
				scenario->set_parameter(document, "walkThreshold", _walkThreshold);
				scenario->set_parameter(document, "walkSpeed", _walkSpeed);
				scenario->set_parameter(document, "bikeThreshold", _bikeThreshold);
				scenario->set_parameter(document, "bikeSpeed", _bikeSpeed);
				scenario->set_parameter(document, "debug_route", _debug_route);
				scenario->set_parameter(document, "multimodal_dijkstra", _multimodal_dijkstra);
								
				return true;
			}

			static void print_parameters()
			{				
				cout << "Multimodal Routing parameters" << endl;
				cout << "\ttransferPenalty = " << transferPenalty	<float>() << endl;
				cout << "\twaitWeight = " << waitWeight		<float>() << endl;
				cout << "\twalkWeight = " << walkWeight	<float>() << endl;
				cout << "\tbikeWeight = " << bikeWeight	<float>() << endl;
				cout << "\tivtWeight = " << ivtWeight		<float>() << endl;
				cout << "\tcarWeight = " << carWeight	<float>() << endl;
				cout << "\tscanThreshold = " << scanThreshold	<float>() << endl;
				cout << "\tcostThreshold = " << costThreshold	<float>() << endl;
				cout << "\twaitThreshold = " << waitThreshold	<float>() << endl;
				cout << "\twalkThreshold = " << walkThreshold	<Meters>() << endl;
				cout << "\twalkSpeed = " << walkSpeed	<float>() << endl;
				cout << "\tbikeThreshold = " << bikeThreshold	<Meters>() << endl;
				cout << "\tbikeSpeed = " << bikeSpeed	<float>() << endl;
				cout << "\tdebug_route = " << debug_route	<bool>() << endl;
				cout << "\tmultimodal_dijkstra = " << multimodal_dijkstra	<bool>() << endl;
			}
			
			static void default_static_initializer()
			{
				_transferPenalty = 300.0;
				_waitWeight = 2.0;
				_walkWeight = 2.0;
				_bikeWeight = 2.0;
				_ivtWeight = 1.0;
				_carWeight = 1.5;
				_scanThreshold = 200000.0;
				_costThreshold = 28800.0;
				_waitThreshold = 3600.0;
				_walkThreshold = 5000.0;
				_walkSpeed = 5.0;
				_bikeThreshold = 10000.0;
				_bikeSpeed = 10.0;
				_debug_route = false;
				_multimodal_dijkstra = true;
			}

			#pragma region static parameters declaration		
			m_static_data(float, transferPenalty, NONE, NONE);
			m_static_data(float, waitWeight, NONE, NONE);
			m_static_data(float, walkWeight, NONE, NONE);
			m_static_data(float, bikeWeight, NONE, NONE);
			m_static_data(float, ivtWeight, NONE, NONE);
			m_static_data(float, carWeight, NONE, NONE);
			m_static_data(float, scanThreshold, NONE, NONE);
			m_static_data(float, costThreshold, NONE, NONE);
			m_static_data(float, waitThreshold, NONE, NONE);
			m_static_data(float, walkThreshold, NONE, NONE);
			m_static_data(float, walkSpeed, NONE, NONE);
			m_static_data(float, bikeThreshold, NONE, NONE);
			m_static_data(float, bikeSpeed, NONE, NONE);
			m_static_data(bool, debug_route, NONE, NONE);
			m_static_data(bool, multimodal_dijkstra, NONE, NONE);
			#pragma endregion

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

				//==============================================================================
				// ADD LINK MOEs
				//------------------------------------------------------------------------------
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
					travel_delay = db_itr->getLink_Travel_Time();

					//if(travel_delay < 0.01f) cout << travel_delay << endl;

					unsigned int ptr = _moe_data.add_layer_element(travel_delay,start_time);

					if(!time_advance)
					{
						_link_id_to_moe_data[link_id] = ptr;
						time_advance = false;
					}
				}

				//==============================================================================
				// ADD TURN MOEs
				//------------------------------------------------------------------------------
				result<TurnMOE> turn_moe_result = db->template query<TurnMOE>(query<TurnMOE>::true_expr);

				int turn_id;
				float turn_delay;
				start_time = -1;
				counter = -1;

				time_advance = false;

				for (typename result<TurnMOE>::iterator db_itr = turn_moe_result.begin(); db_itr != turn_moe_result.end(); ++db_itr)
				{
					if (++counter % 100000 == 0) cout << counter << endl;

					if (start_time == -1) start_time = db_itr->getStart_Time();

					if (start_time != db_itr->getStart_Time())
					{
						time_advance = true;
					}

					start_time = db_itr->getStart_Time();
					turn_id = db_itr->getTurn_Uid();
					turn_delay = db_itr->getTurn_Penalty();

					//if(travel_delay < 0.01f) cout << travel_delay << endl;

					unsigned int ptr = _turn_moe_data.add_layer_element(turn_delay, start_time);

					if (!time_advance)
					{
						_turn_id_to_moe_data[turn_id] = ptr;
						time_advance = false;
					}
				}

			}

			Routable_Network<ComponentType>* create_copy()
			{
				Routable_Network_Implementation* copy = Allocate<ComponentType>();

				Graph_Pool<typename MT::graph_pool_type>* graph_copy = _routable_graph_pool->Create_Copy();

				copy->_routable_graph_pool = graph_copy;
				copy->_static_network_graph_id = _static_network_graph_id;
				copy->_multimodal_network_graph_id = _multimodal_network_graph_id;
				copy->_time_dependent_network_graph_id = _time_dependent_network_graph_id;

				return (Routable_Network<ComponentType>*)copy;
			}

			void initialize()
			{
				_routable_graph_pool = (Graph_Pool<typename MT::graph_pool_type>*) new typename MT::graph_pool_type();
			}

			void construct_time_dependent_routable_network(Network<typename MasterType::network_type>* source_network)
			{
				typedef Scenario<typename MasterType::scenario_type> Scenario_Interface;

				Types::time_dependent_attributes<MT>::_moe_data = &_moe_data;
				Types::time_dependent_attributes<MT>::_ttime_weight_shape = ((Scenario_Interface*)_global_scenario)->time_dependent_routing_weight_shape<float>();
				Types::time_dependent_attributes<MT>::_ttime_weight_scale = ((Scenario_Interface*)_global_scenario)->time_dependent_routing_weight_scale<float>();
				Types::time_dependent_attributes<MT>::_ttime_weight_factor = ((Scenario_Interface*)_global_scenario)->time_dependent_routing_weight_factor<float>();

				Types::time_dependent_to_time_dependent::_turn_moe_data = &_turn_moe_data;

				typedef Network<typename MasterType::network_type> Network_Interface;

				typedef Link_Components::Prototypes::Link<typename remove_pointer<typename Network_Interface::get_type_of(links_container)::value_type>::type> Link_Interface;
				typedef Random_Access_Sequence<typename Network_Interface::get_type_of(links_container),Link_Interface*> Link_Container_Interface;
				typedef Intersection<typename remove_pointer<typename Network_Interface::get_type_of(intersections_container)::value_type>::type> Intersection_Interface;

				typedef Movement<typename remove_pointer<typename Link_Interface::get_type_of(outbound_turn_movements)::value_type>::type> Turn_Movement_Interface;
				typedef Random_Access_Sequence<typename Link_Interface::get_type_of(outbound_turn_movements),Turn_Movement_Interface*> Turn_Movement_Container_Interface;




				//Graph_Pool<typename MT::graph_pool_type>* graph_pool = (Graph_Pool<typename MT::graph_pool_type>*) new typename MT::graph_pool_type();

				//_routable_graph_pool = graph_pool;

				Graph_Pool<typename MT::graph_pool_type>* graph_pool = _routable_graph_pool;

				
				
				Graph_Assembler_Connected_Edge<typename MT::time_dependent_graph_type>* time_dependent_graph = graph_pool->template Create_New_Graph<typename MT::time_dependent_graph_type>();

				_time_dependent_network_graph_id = time_dependent_graph->graph_id();

				Input_Edge<Types::time_dependent_attributes<MT>> input_time_dependent_edge;

				Input_Connection_Group_Implementation<typename MT::time_dependent_to_time_dependent_connection_type>::_neighbor_graph_id = time_dependent_graph->graph_id();

				Input_Connection_Group_Implementation<typename MT::time_dependent_to_time_dependent_connection_type>* time_dependent_to_time_dependent_connection_group = new Input_Connection_Group_Implementation<typename MT::time_dependent_to_time_dependent_connection_type>();
	
				Types::time_dependent_to_time_dependent connection_attributes;


				Network_Interface* network = source_network;

				Link_Container_Interface* links = network->template links_container<Link_Container_Interface*>();

				for(auto links_itr=links->begin();links_itr!=links->end();links_itr++)
				{
					Link_Interface* current_link = (Link_Interface*)(*links_itr);
					Link_Components::Types::Link_Type_Keys link_type = current_link->template link_type<Link_Components::Types::Link_Type_Keys>();

					if (link_type != Link_Components::Types::Link_Type_Keys::WALK && link_type != Link_Components::Types::Link_Type_Keys::TRANSIT)
					{

						Intersection_Interface* downstream_intersection = current_link->template downstream_intersection<Intersection_Interface*>();

						input_time_dependent_edge._x = downstream_intersection->template x_position<float>();
						input_time_dependent_edge._y = downstream_intersection->template y_position<float>();
						input_time_dependent_edge._edge_id = current_link->template uuid<unsigned int>();

						input_time_dependent_edge._cost = current_link->template travel_time<float>();
						input_time_dependent_edge._cost_backup = current_link->template travel_time<float>();
						input_time_dependent_edge._time_cost = current_link->template travel_time<float>();
						input_time_dependent_edge._time_cost_backup = current_link->template travel_time<float>();

												
						if (_link_id_to_moe_data.count(current_link->template uuid<int>()))
						{
							input_time_dependent_edge._moe_ptr = _moe_data.get_element(_link_id_to_moe_data[current_link->template uuid<int>()]);
						}
						else
						{
							input_time_dependent_edge._moe_ptr = nullptr;
							//cout << "unable to find a corresponding moe for link: " << current_link->template dbid<int>() << endl;
							//exit(0);
						}

						if (link_type == Link_Components::Types::Link_Type_Keys::ARTERIAL || link_type == Link_Components::Types::Link_Type_Keys::LOCAL)
						{
							input_time_dependent_edge._is_highway = false;
						}
						else
						{
							input_time_dependent_edge._is_highway = true;
						}


						Turn_Movement_Container_Interface* outbound_turn_movements = current_link->template outbound_turn_movements<Turn_Movement_Container_Interface*>();

						for (auto movements_itr = outbound_turn_movements->begin(); movements_itr != outbound_turn_movements->end(); movements_itr++)
						{
							Turn_Movement_Interface* current_movement = (Turn_Movement_Interface*)(*movements_itr);

							Link_Interface* out_link = current_movement->template outbound_link<Link_Interface*>();
							Link_Components::Types::Link_Type_Keys out_link_type = out_link->template link_type<Link_Components::Types::Link_Type_Keys>();

							if (out_link_type != Link_Components::Types::Link_Type_Keys::WALK && out_link_type != Link_Components::Types::Link_Type_Keys::TRANSIT)
							{

								if (_turn_id_to_moe_data.count(current_movement->template uuid<int>()))
								{
									connection_attributes._turn_moe_ptr = _turn_moe_data.get_element(_turn_id_to_moe_data[current_movement->template uuid<int>()]);
								}
								else
								{
									connection_attributes._turn_moe_ptr = nullptr;
								}

								long long neighbor_id = current_movement->template outbound_link<Link_Interface*>()->template uuid<int>();

								time_dependent_to_time_dependent_connection_group->_neighbors.push_back(neighbor_id);

								connection_attributes._cost = 0.0f;
								connection_attributes._time_cost = 0.0f;

								time_dependent_to_time_dependent_connection_group->_neighbor_attributes.push_back(connection_attributes);
							}

						}

						input_time_dependent_edge._connection_groups.push_back(time_dependent_to_time_dependent_connection_group);

						time_dependent_graph->template Add_Edge<Types::time_dependent_attributes<MT>>(&input_time_dependent_edge);

						// Clean up connection group

						time_dependent_to_time_dependent_connection_group->_neighbors.clear();
						time_dependent_to_time_dependent_connection_group->_neighbor_attributes.clear();

						// Clean up input edge

						input_time_dependent_edge._connection_groups.clear();
					}
				}

				Interactive_Graph<typename MT::time_dependent_graph_type>* routable_network_graph = time_dependent_graph->template Compile_Graph<Types::time_dependent_attributes<MT>>();
	
				//graph_pool->Link_Graphs();
			}

			void finalize()
			{
				_routable_graph_pool->Link_Graphs();
			}

			void construct_routable_network(Network<typename MasterType::network_type>* source_network)
			{
				typedef Network<typename MasterType::network_type> Network_Interface;

				typedef Link_Components::Prototypes::Link<typename remove_pointer<typename Network_Interface::get_type_of(links_container)::value_type>::type> Link_Interface;
				typedef Random_Access_Sequence<typename Network_Interface::get_type_of(links_container),Link_Interface*> Link_Container_Interface;
				typedef Intersection<typename remove_pointer<typename Network_Interface::get_type_of(intersections_container)::value_type>::type> Intersection_Interface;

				typedef Movement<typename remove_pointer<typename Link_Interface::get_type_of(outbound_turn_movements)::value_type>::type> Turn_Movement_Interface;
				typedef Random_Access_Sequence<typename Link_Interface::get_type_of(outbound_turn_movements),Turn_Movement_Interface*> Turn_Movement_Container_Interface;



				Graph_Pool<typename MT::graph_pool_type>* graph_pool = _routable_graph_pool;


				Graph_Assembler_Connected_Edge<typename MT::static_graph_type>* static_graph = graph_pool->template Create_New_Graph<typename MT::static_graph_type>();

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

				Link_Container_Interface* links = network->template links_container<Link_Container_Interface*>();

				for(auto links_itr=links->begin();links_itr!=links->end();links_itr++)
				{
					Link_Interface* current_link = (Link_Interface*)(*links_itr);
					Link_Components::Types::Link_Type_Keys link_type = current_link->template link_type<Link_Components::Types::Link_Type_Keys>();

					if (link_type != Link_Components::Types::Link_Type_Keys::WALK && link_type != Link_Components::Types::Link_Type_Keys::TRANSIT)
					{
						Intersection_Interface* downstream_intersection = current_link->template downstream_intersection<Intersection_Interface*>();

						input_static_edge._x = downstream_intersection->template x_position<float>();
						input_static_edge._y = downstream_intersection->template y_position<float>();
						input_static_edge._edge_id = current_link->template uuid<unsigned int>();

						input_static_edge._cost = current_link->template travel_time<float>();
						input_static_edge._cost_backup = current_link->template travel_time<float>();
						input_static_edge._time_cost = current_link->template travel_time<float>();
						input_static_edge._time_cost_backup = current_link->template travel_time<float>();

						if (link_type == Link_Components::Types::Link_Type_Keys::ARTERIAL || link_type == Link_Components::Types::Link_Type_Keys::LOCAL)
						{
							input_static_edge._is_highway = false;
						}
						else
						{
							input_static_edge._is_highway = true;
						}


						Turn_Movement_Container_Interface* outbound_turn_movements = current_link->template outbound_turn_movements<Turn_Movement_Container_Interface*>();

						for (auto movements_itr = outbound_turn_movements->begin(); movements_itr != outbound_turn_movements->end(); movements_itr++)
						{
							Turn_Movement_Interface* current_movement = (Turn_Movement_Interface*)(*movements_itr);

							Link_Interface* out_link = current_movement->template outbound_link<Link_Interface*>();
							Link_Components::Types::Link_Type_Keys out_link_type = out_link->template link_type<Link_Components::Types::Link_Type_Keys>();

							if (out_link_type != Link_Components::Types::Link_Type_Keys::WALK && out_link_type != Link_Components::Types::Link_Type_Keys::TRANSIT)
							{
								long long neighbor_id = current_movement->template outbound_link<Link_Interface*>()->template uuid<int>();

								static_to_static_connection_group->_neighbors.push_back(neighbor_id);

								connection_attributes._cost = 0.0f;
								connection_attributes._time_cost = 0.0f;

								static_to_static_connection_group->_neighbor_attributes.push_back(connection_attributes);
							}
						}

						//each edge can have multiple connection groups, like bus->walk or bus->walk. The use pattern is to put connections of the same type into separate group, each group will have a different types of elements
						input_static_edge._connection_groups.push_back(static_to_static_connection_group);

						static_graph->template Add_Edge<Types::static_attributes<MT>>(&input_static_edge);

						// Clean up connection group

						static_to_static_connection_group->_neighbors.clear();
						static_to_static_connection_group->_neighbor_attributes.clear();

						// Clean up input edge

						input_static_edge._connection_groups.clear();
					}
				}
				//reorganizes data that holds information for a graph structure
				Interactive_Graph<typename MT::static_graph_type>* routable_network_graph = static_graph->template Compile_Graph<Types::static_attributes<MT>>();
	
				//graph_pool->Link_Graphs();
			}

			void construct_multimodal_routable_network(Network<typename MasterType::network_type>* source_network)
			{
				typedef Scenario<typename MasterType::scenario_type> Scenario_Interface;

				Types::multimodal_attributes<MT>::_moe_data = &_moe_data;
				Types::multimodal_attributes<MT>::_ttime_weight_shape = ((Scenario_Interface*)_global_scenario)->time_dependent_routing_weight_shape<float>();
				Types::multimodal_attributes<MT>::_ttime_weight_scale = ((Scenario_Interface*)_global_scenario)->time_dependent_routing_weight_scale<float>();
				Types::multimodal_attributes<MT>::_ttime_weight_factor = ((Scenario_Interface*)_global_scenario)->time_dependent_routing_weight_factor<float>();

				Types::multimodal_to_multimodal::_turn_moe_data = &_turn_moe_data;

				typedef Network<typename MasterType::network_type> Network_Interface;

				typedef Link_Components::Prototypes::Link<typename remove_pointer<typename Network_Interface::get_type_of(links_container)::value_type>::type> Link_Interface;
				typedef Random_Access_Sequence<typename Network_Interface::get_type_of(links_container), Link_Interface*> Link_Container_Interface;

				typedef Random_Access_Sequence<typename Link_Interface::get_type_of(index_along_pattern_at_upstream_node)> Index_Along_Pattern_Container_Interface;
				typedef Random_Access_Sequence<typename Link_Interface::get_type_of(unique_patterns)> Unique_Patterns_Container_Interface;

				typedef Intersection<typename remove_pointer<typename Network_Interface::get_type_of(intersections_container)::value_type>::type> Intersection_Interface;

				typedef Movement<typename remove_pointer<typename Link_Interface::get_type_of(outbound_turn_movements)::value_type>::type> Turn_Movement_Interface;
				typedef Random_Access_Sequence<typename Link_Interface::get_type_of(outbound_turn_movements), Turn_Movement_Interface*> Turn_Movement_Container_Interface;

				typedef  Transit_Pattern_Components::Prototypes::Transit_Pattern<typename remove_pointer< typename Network_Interface::get_type_of(transit_patterns_container)::value_type>::type>  _Transit_Pattern_Interface;
				typedef  Random_Access_Sequence< typename Network_Interface::get_type_of(transit_patterns_container), _Transit_Pattern_Interface*> _Transit_Patterns_Container_Interface;
				typedef  Random_Access_Sequence< typename _Transit_Pattern_Interface::get_type_of(pattern_trips)> _Patterns_Trips_Container_Interface;
				typedef  Random_Access_Sequence< typename _Transit_Pattern_Interface::get_type_of(pattern_edge_ids)> _Patterns_Edge_Ids_Container_Interface;

				typedef  Transit_Vehicle_Trip_Components::Prototypes::Transit_Vehicle_Trip<typename remove_pointer< typename Network_Interface::get_type_of(transit_vehicle_trips_container)::value_type>::type>  _Transit_Vehicle_Trip_Interface;
				typedef  Random_Access_Sequence< typename Network_Interface::get_type_of(transit_vehicle_trips_container), _Transit_Vehicle_Trip_Interface*> _Transit_Vehicle_Trips_Container_Interface;
				typedef  Random_Access_Sequence< typename _Transit_Vehicle_Trip_Interface::get_type_of(arrival_seconds)> _Arrival_Seconds_Container_Interface;

				typedef Pair_Associative_Container< typename Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
				typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_Zones_Container_Interface)> _Zone_Interface;


				cout << "Constructing multi-modal routable network..." << endl;

				Graph_Pool<typename MT::graph_pool_type>* graph_pool = _routable_graph_pool;

				typedef typename Graph_Pool<typename MT::graph_pool_type>::base_edge_type base_edge_type;


				Graph_Assembler_Connected_Edge<typename MT::multimodal_graph_type>* multimodal_graph = graph_pool->template Create_New_Graph<typename MT::multimodal_graph_type>();

				_multimodal_network_graph_id = multimodal_graph->graph_id();

				Input_Edge<Types::multimodal_attributes<MT>> input_multimodal_edge;

				Input_Connection_Group_Implementation<typename MT::multimodal_to_multimodal_connection_type>::_neighbor_graph_id = multimodal_graph->graph_id();

				Input_Connection_Group_Implementation<typename MT::multimodal_to_multimodal_connection_type>* multimodal_to_multimodal_connection_group = new Input_Connection_Group_Implementation<typename MT::multimodal_to_multimodal_connection_type>();

				Types::multimodal_to_multimodal connection_attributes;


				Network_Interface* network = source_network;

				Link_Container_Interface* links = network->template links_container<Link_Container_Interface*>();


				for (auto links_itr = links->begin(); links_itr != links->end(); links_itr++)
				{
					Link_Interface* current_link = (Link_Interface*)(*links_itr);
					Link_Components::Types::Link_Type_Keys link_type = current_link->template link_type<Link_Components::Types::Link_Type_Keys>();
					float walkWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::walkWeight<float>();
					float bikeWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::walkWeight<float>();
					float ivtWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::ivtWeight<float>();
					float carWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::carWeight<float>();

					/*if (link_type == Link_Components::Types::Link_Type_Keys::WALK || link_type == Link_Components::Types::Link_Type_Keys::TRANSIT)
					{*/

						Intersection_Interface* downstream_intersection = current_link->template downstream_intersection<Intersection_Interface*>();

						input_multimodal_edge._x = downstream_intersection->template x_position<float>();
						input_multimodal_edge._y = downstream_intersection->template y_position<float>();
						input_multimodal_edge._edge_id = current_link->template uuid<unsigned int>();
						input_multimodal_edge._edge_type = current_link->link_type<Link_Components::Types::Link_Type_Keys>();

						input_multimodal_edge._cost = current_link->template travel_time<float>();
						input_multimodal_edge._cost_backup = current_link->template travel_time<float>();
						input_multimodal_edge._time_cost = current_link->template travel_time<float>();
						input_multimodal_edge._time_cost_backup = current_link->template travel_time<float>();
						input_multimodal_edge._source_link = current_link;
						
						if (link_type == Link_Components::Types::Link_Type_Keys::TRANSIT)
						{
							int my_itr = 0;
							float min_travel_time = FLT_MAX / 2.0f;

							for (auto patterns_itr = current_link->unique_patterns<Unique_Patterns_Container_Interface&>().begin(); patterns_itr != current_link->unique_patterns<Unique_Patterns_Container_Interface&>().end(); ++patterns_itr)
							{
								_Transit_Pattern_Interface* current_pattern = (_Transit_Pattern_Interface*)(*patterns_itr);
								input_multimodal_edge._unique_patterns.push_back(current_pattern);
								int mySeq = current_link->index_along_pattern_at_upstream_node<Index_Along_Pattern_Container_Interface&>()[my_itr];
								input_multimodal_edge._index_along_pattern_at_upstream_node.push_back(mySeq);
								for (auto trips_itr = current_pattern->pattern_trips<_Patterns_Trips_Container_Interface&>().begin(); trips_itr != current_pattern->pattern_trips<_Patterns_Trips_Container_Interface&>().end(); ++trips_itr)
								{
									_Transit_Vehicle_Trip_Interface* current_trip = (_Transit_Vehicle_Trip_Interface*)(*trips_itr);
									float temp_travel_time = current_trip->arrival_seconds<_Arrival_Seconds_Container_Interface&>().at(mySeq + 1) - current_trip->arrival_seconds<_Arrival_Seconds_Container_Interface&>().at(mySeq);
									if (temp_travel_time < min_travel_time)
									{
										min_travel_time = temp_travel_time;
									}
								}
								my_itr++;
							}

							current_link->template min_multi_modal_cost<float>(ivtWeight * min_travel_time);
							current_link->template walk_length<float>(FLT_MAX / 2.0f);
							current_link->template drive_time<float>(FLT_MAX / 2.0f);
							current_link->template walk_distance_to_transit<float>(FLT_MAX / 2.0f);
							current_link->template drive_fft_to_transit<float>(FLT_MAX / 2.0f);
						}
						else if (link_type == Link_Components::Types::Link_Type_Keys::WALK)
						{
							current_link->template min_multi_modal_cost<float>(walkWeight*current_link->template travel_time<float>() );
							
							Feet walk_length_feet = current_link->template length<float>();
							Meters walk_length_meters = GLOBALS::Length_Converter.Convert_Value<Feet, Meters>(walk_length_feet);							
							current_link->template walk_length<float>(walk_length_meters);

							current_link->template drive_time<float>(FLT_MAX / 2.0f);
							current_link->template walk_distance_to_transit<float>(FLT_MAX / 2.0f);
							current_link->template drive_fft_to_transit<float>(FLT_MAX / 2.0f);
						}
						else
						{
							current_link->template min_multi_modal_cost<float>(carWeight*current_link->template travel_time<float>() );
							current_link->template walk_length<float>(FLT_MAX / 2.0f);
							current_link->template drive_time<float>(current_link->template travel_time<float>() );
							current_link->template walk_distance_to_transit<float>(FLT_MAX / 2.0f);
							current_link->template drive_fft_to_transit<float>(FLT_MAX / 2.0f);
						}								
						
						if (_link_id_to_moe_data.count(current_link->template uuid<int>()))
						{
							input_multimodal_edge._moe_ptr = _moe_data.get_element(_link_id_to_moe_data[current_link->template uuid<int>()]);
						}
						else
						{
							input_multimodal_edge._moe_ptr = nullptr;
							//cout << "unable to find a corresponding moe for link: " << current_link->template dbid<int>() << endl;
							//exit(0);
						}

						if (link_type == Link_Components::Types::Link_Type_Keys::ARTERIAL || link_type == Link_Components::Types::Link_Type_Keys::LOCAL ||
							link_type == Link_Components::Types::Link_Type_Keys::WALK || link_type == Link_Components::Types::Link_Type_Keys::TRANSIT)
						{
							input_multimodal_edge._is_highway = false;
						}
						else
						{
							input_multimodal_edge._is_highway = true;
						}

						Turn_Movement_Container_Interface* outbound_turn_movements = current_link->template outbound_turn_movements<Turn_Movement_Container_Interface*>();

						for (auto movements_itr = outbound_turn_movements->begin(); movements_itr != outbound_turn_movements->end(); movements_itr++)
						{
							Turn_Movement_Interface* current_movement = (Turn_Movement_Interface*)(*movements_itr);

							Link_Interface* out_link = current_movement->template outbound_link<Link_Interface*>();
							Link_Components::Types::Link_Type_Keys out_link_type = out_link->template link_type<Link_Components::Types::Link_Type_Keys>();

							/*if (out_link_type == Link_Components::Types::Link_Type_Keys::WALK || out_link_type == Link_Components::Types::Link_Type_Keys::TRANSIT)
							{*/
								if (_turn_id_to_moe_data.count(current_movement->template uuid<int>()))
								{
									connection_attributes._turn_moe_ptr = _turn_moe_data.get_element(_turn_id_to_moe_data[current_movement->template uuid<int>()]);
								}
								else
								{
									connection_attributes._turn_moe_ptr = nullptr;
								}

								long long neighbor_id = current_movement->template outbound_link<Link_Interface*>()->template uuid<int>();

								multimodal_to_multimodal_connection_group->_neighbors.push_back(neighbor_id);

								connection_attributes._cost = 0.0f;
								connection_attributes._time_cost = 0.0f;

								multimodal_to_multimodal_connection_group->_neighbor_attributes.push_back(connection_attributes);
							//}
						}

						input_multimodal_edge._connection_groups.push_back(multimodal_to_multimodal_connection_group);

						multimodal_graph->template Add_Edge<Types::multimodal_attributes<MT>>(&input_multimodal_edge);

						// Clean up connection group

						multimodal_to_multimodal_connection_group->_neighbors.clear();
						multimodal_to_multimodal_connection_group->_neighbor_attributes.clear();

						// Clean up input edge

						input_multimodal_edge._connection_groups.clear();
						input_multimodal_edge._trips_by_dep_time.clear();
						input_multimodal_edge._index_along_trip_at_upstream_node.clear();
						input_multimodal_edge._trip_to_unique_pattern_index.clear();
						input_multimodal_edge._unique_patterns.clear();
						input_multimodal_edge._index_along_pattern_at_upstream_node.clear();
						//input_multimodal_edge._unique_pattern_scanned.clear();
					//}
				}								

				Interactive_Graph<typename MT::multimodal_graph_type>* routable_network_graph = multimodal_graph->template Compile_Graph<Types::multimodal_attributes<MT>>();

				_Transit_Patterns_Container_Interface* patterns = network->template transit_patterns_container<_Transit_Patterns_Container_Interface*>();
				for (auto patterns_itr = patterns->begin(); patterns_itr != patterns->end(); ++patterns_itr)
				{
					_Transit_Pattern_Interface* pattern = (_Transit_Pattern_Interface*)(*patterns_itr);
					Link_Container_Interface* pattern_links = pattern->pattern_links<Link_Container_Interface*>();

					for (auto links_itr = pattern_links->begin(); links_itr != pattern_links->end(); links_itr++)
					{
						Link_Interface* current_link = (Link_Interface*)(*links_itr);

						int edge_id = current_link->uuid<int>();

						pattern->pattern_edge_ids<_Patterns_Edge_Ids_Container_Interface&>().push_back(edge_id);
					}
				}
				//graph_pool->Link_Graphs();
			}

			void test_routable_network()
			{
				Routable_Agent<typename MT::routable_agent_type> proxy_agent;

				std::deque< global_edge_id > path;

				global_edge_id start;

				start.edge_id = 8751*2+0;
				start.graph_id = 0;
				
				global_edge_id end;

				end.edge_id = 9598*2+0;
				end.graph_id = 0;

				A_Star<MT,typename MT::routable_agent_type,typename MT::graph_pool_type>(&proxy_agent,_routable_graph_pool,start,end,0,path);

				for(auto itr = path.begin();itr!=path.end();itr++)
				{
					cout << itr->edge_id /2 << endl;
				}
			}
			//currently calls A* algorithm
			float compute_static_network_path(std::vector<unsigned int>& origins, std::vector<unsigned int>& destinations, unsigned int start_time, std::deque<global_edge_id>& path_container, std::deque<float>& cost_container, unsigned int origin_loc_id,	unsigned int destination_loc_id, bool debug_route, std::string& summary_paragraph)
			{
				//use hamogeneous agent for now
				Routable_Agent<typename MT::routable_agent_type> proxy_agent;

				// get start id list from link id list
				std::vector<global_edge_id> starts;
				for (auto itr = origins.begin(); itr != origins.end(); ++itr)
				{
					global_edge_id start;
					start.edge_id = *itr;
					start.graph_id = _static_network_graph_id;
					starts.push_back(start);
				}

				// get edge id list from link id list
				std::vector<global_edge_id> ends;
				for (auto itr = destinations.begin(); itr != destinations.end(); ++itr)
				{
					global_edge_id end;
					end.edge_id = *itr;
					end.graph_id = _static_network_graph_id;
					ends.push_back(end);
				}

				float routed_time = A_Star<MT, typename MT::routable_agent_type, typename MT::graph_pool_type>(&proxy_agent, _routable_graph_pool, starts, ends, 0, path_container, cost_container, origin_loc_id, destination_loc_id, debug_route, summary_paragraph);

				// update origins/destinations lists in from A_Star results
				origins.clear();
				origins.push_back(starts.front().edge_id);
				destinations.clear();
				destinations.push_back(ends.front().edge_id);

				return routed_time;
			}

			//TODO: Remove when done testing routing execution time
			float compute_multimodal_network_path(
				std::vector<unsigned int>& origins, 
				std::vector<unsigned int>& destinations, 
				/*std::vector<unsigned int>& tr_destinations,*/ 
				unsigned int start_time, 
				std::deque<global_edge_id>& path_container, 
				std::deque<float>& cost_container,
				std::deque<Link_Components::Types::Link_Type_Keys>& out_type,
				std::deque<int>& out_trip,
				std::deque<int>& out_seq,
				std::deque<float>& out_time,
				std::deque<float>& out_arr_time,
				std::deque<float>& out_wait_time,
				std::deque<float>& out_walk_time,
				std::deque<float>& out_ivt_time,
				std::deque<float>& out_car_time,
				std::deque<int>& out_wait_count,
				std::deque<float>& out_transfer_pen,
				std::deque<float>& out_heur_cost,
				__int64& astar_time, 
				unsigned int origin_loc_id, 
				unsigned int destination_loc_id, 
				bool debug_route,
				std::string& summary_paragraph,
				std::string& detail_paragraph,
				Vehicle_Components::Types::Vehicle_Type_Keys sub_mode)
			{
				
				//Routable_Agent<typename MT::time_dependent_agent_type> proxy_agent;
				Routable_Agent<typename MT::routable_agent_type> proxy_agent;

				// get start id list from link id list
				std::vector<global_edge_id> starts;
				for (auto itr = origins.begin(); itr != origins.end(); ++itr)
				{
					global_edge_id start;
					start.edge_id = *itr;
					start.graph_id = _multimodal_network_graph_id;
					starts.push_back(start);
				}

				// get end id list from link id list
				std::vector<global_edge_id> ends;
				for (auto itr = destinations.begin(); itr != destinations.end(); ++itr)
				{
					global_edge_id end;
					end.edge_id = *itr;
					end.graph_id = _multimodal_network_graph_id;
					ends.push_back(end);
				}

				//// get end_tr id list from link id list
				//std::vector<global_edge_id> tr_ends;
				//for (auto itr = tr_destinations.begin(); itr != tr_destinations.end(); ++itr)
				//{
				//	global_edge_id tr_end;
				//	tr_end.edge_id = *itr;
				//	tr_end.graph_id = _multimodal_network_graph_id;
				//	tr_ends.push_back(tr_end);
				//}

				
				float routed_time = Multimodal_A_Star<MT, typename MT::routable_agent_type, typename MT::graph_pool_type>(&proxy_agent, _routable_graph_pool, starts, ends, /*tr_ends,*/ start_time, path_container, cost_container, out_type, out_trip, out_seq, out_time, out_arr_time, out_wait_time, out_walk_time, out_ivt_time, out_car_time, out_wait_count, out_transfer_pen, out_heur_cost, astar_time, origin_loc_id, destination_loc_id, debug_route, summary_paragraph, detail_paragraph, sub_mode);

				
				// update origins/destinations lists in from A_Star results
				origins.clear();
				origins.push_back(starts.front().edge_id);
				destinations.clear();
				destinations.push_back(ends.front().edge_id);
				//tr_destinations.clear();
				//tr_destinations.push_back(tr_ends.front().edge_id);
				return routed_time;
			}

			float compute_time_dependent_network_path(std::vector<unsigned int>& origins, std::vector<unsigned int>& destinations, unsigned int start_time, std::deque<global_edge_id>& path_container, std::deque<float>& cost_container, unsigned int origin_loc_id, unsigned int destination_loc_id, bool debug_route, std::string& summary_paragraph)
			{
				//Routable_Agent<typename MT::time_dependent_agent_type> proxy_agent;
				Routable_Agent<typename MT::routable_agent_type> proxy_agent;
			
				// get start id list from link id list
				std::vector<global_edge_id> starts;
				for (auto itr = origins.begin(); itr != origins.end(); ++itr)
				{
					global_edge_id start;
					start.edge_id = *itr;
					start.graph_id = _time_dependent_network_graph_id;
					starts.push_back(start);
				}

				// get edge id list from link id list
				std::vector<global_edge_id> ends;
				for (auto itr = destinations.begin(); itr != destinations.end(); ++itr)
				{
					global_edge_id end;
					end.edge_id = *itr;
					end.graph_id = _time_dependent_network_graph_id;
					ends.push_back(end);
				}

				//float routed_time = Time_Dependent_A_Star<MT,typename MT::time_dependent_agent_type,typename MT::graph_pool_type>(&proxy_agent,_routable_graph_pool,start,end,start_time,path_container,cost_container);
				
				//float routed_time = Time_Dependent_A_Star<MT, typename MT::routable_agent_type, typename MT::graph_pool_type>(&proxy_agent, _routable_graph_pool, starts, ends, start_time, path_container, cost_container, origin_loc_id, destination_loc_id, debug_route, summary_paragraph);
				float routed_time = A_Star<MT, typename MT::routable_agent_type, typename MT::graph_pool_type>(&proxy_agent, _routable_graph_pool, starts, ends, start_time, path_container, cost_container, origin_loc_id, destination_loc_id, debug_route, summary_paragraph);
				
				// update origins/destinations lists in from A_Star results
				origins.clear();
				origins.push_back(starts.front().edge_id);
				destinations.clear();
				destinations.push_back(ends.front().edge_id);

				return routed_time;
			}

			float compute_static_network_tree(unsigned int origin, std::vector<float>& cost_container)
			{
				Routable_Agent<typename MT::tree_agent_type> proxy_agent;

				global_edge_id start;

				start.edge_id = origin;
				start.graph_id = _static_network_graph_id;
				
				float routed_time = A_Star_Tree<MT,typename MT::tree_agent_type,typename MT::graph_pool_type>(&proxy_agent,_routable_graph_pool,start,0,cost_container);

				return routed_time;
			}

			float compute_time_dependent_network_tree(unsigned int origin, std::vector<float>& cost_container)
			{
				Routable_Agent<typename MT::tree_agent_type> proxy_agent;

				global_edge_id start;

				start.edge_id = origin;
				start.graph_id = _time_dependent_network_graph_id;

				float routed_time = A_Star_Tree<MT, typename MT::tree_agent_type, typename MT::graph_pool_type>(&proxy_agent, _routable_graph_pool, start, 0, cost_container);

				return routed_time;
			}				
			
			void schedule_dijkstra_network_tree(Network<typename MasterType::network_type>* source_network)
			{				
				typedef Network<typename MasterType::network_type> _Network_Interface;
				typedef Routing_Components::Prototypes::Routing<typename MasterType::dijkstra_heuristics_routing_type> _Routing_Interface;

				typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
				typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_Zones_Container_Interface)> _Zone_Interface;

				bool debug_route = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::debug_route<bool>();

				_Network_Interface* network = source_network;
				

				_Zones_Container_Interface::iterator orig_zone_itr;
				_Zones_Container_Interface* origin_zones = network->zones_container<_Zones_Container_Interface*>();

				for (auto orig_zone_itr = origin_zones->begin(); orig_zone_itr != origin_zones->end(); orig_zone_itr++)
				{
					_Routing_Interface* dijkstra_router = (_Routing_Interface*)Allocate<typename _Routing_Interface::Component_Type>();
					_Zone_Interface* origin_zone = (_Zone_Interface*)(orig_zone_itr->second);
					dijkstra_router->origin_zone(origin_zone);
					dijkstra_router->template network<_Network_Interface*>(network);
					dijkstra_router->Schedule_Route_Computation(iteration());
				}
			}

			void compute_dijkstra_network_tree(std::vector<unsigned int>& origins, int zone_index, bool debug_route, std::string& summary_paragraph)
			{		
				Routable_Agent<typename MT::multi_modal_tree_agent_type> proxy_agent;

				typedef Network<typename MasterType::network_type> _Network_Interface;

				//typedef Zone_Components::Prototypes::Zone<typename remove_pointer<typename Network_Interface::get_type_of(zones_container)::value_type>::type> _Zone_Interface;
				typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
				typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_Zones_Container_Interface)> _Zone_Interface;
								
				// get start id list from link id list
				std::vector<global_edge_id> starts;
				for (auto itr = origins.begin(); itr != origins.end(); ++itr)
				{
					global_edge_id start;
					start.edge_id = *itr;
					start.graph_id = _multimodal_network_graph_id;
					starts.push_back(start);
				}

				if (!starts.empty())
				{
					float routed_time = Dijkstra_Tree<MT, typename MT::multi_modal_tree_agent_type, typename MT::graph_pool_type>(&proxy_agent, _routable_graph_pool, starts, zone_index, debug_route, summary_paragraph);
				}


			}

			void compute_walk_distance_to_transit(Network<typename MasterType::network_type>* source_network)
			{
				cout << "Constructing the walking distance tree to closest transit for better A* filtering..." << endl;

				Routable_Agent<typename MT::walk_to_transit_tree_agent_type> proxy_agent;				

				typedef typename Graph_Pool<typename MT::graph_pool_type>::base_edge_type base_edge_type;
				Graph_Pool<typename MT::graph_pool_type>* graph_pool = _routable_graph_pool;

				typedef Network<typename MasterType::network_type> Network_Interface;
				
				typedef Intersection_Components::Prototypes::Intersection<typename remove_pointer<typename Network_Interface::get_type_of(intersections_container)::value_type>::type> Intersection_Interface;

				typedef Link_Components::Prototypes::Link<typename remove_pointer<typename Network_Interface::get_type_of(links_container)::value_type>::type> Link_Interface;
				typedef Random_Access_Sequence<typename Network_Interface::get_type_of(links_container), Link_Interface*> Link_Container_Interface;
				
				bool debug_route = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::debug_route<bool>();

				Network_Interface* network = source_network;

				Link_Container_Interface::iterator link_itr;
				Link_Container_Interface* links = network->links_container<Link_Container_Interface*>();

				std::vector<global_edge_id> starts;

				for (link_itr = links->begin(); link_itr != links->end(); link_itr++)
				{
					Link_Interface* link = (Link_Interface*)(*link_itr);

					Intersection_Interface* down_node = link->downstream_intersection<Intersection_Interface*>();
					Link_Container_Interface& outbound_links = down_node->template outbound_links<Link_Container_Interface&>();

					Link_Components::Types::Link_Type_Keys facility_type = link->template link_type<Link_Components::Types::Link_Type_Keys>();

					if (facility_type == Link_Components::Types::Link_Type_Keys::WALK)
					{						
						global_edge_id in_edge_g;
						in_edge_g.edge_id = link->uuid<int>();
						in_edge_g.graph_id = _multimodal_network_graph_id;
						starts.push_back(in_edge_g);

						Link_Container_Interface::iterator out_links_itr;
						for (out_links_itr = outbound_links.begin(); out_links_itr != outbound_links.end(); out_links_itr++)
						{
							Link_Interface* outbound_link = (Link_Interface*)(*out_links_itr);
							Link_Components::Types::Link_Type_Keys out_facility_type = outbound_link->template link_type<Link_Components::Types::Link_Type_Keys>();
							if (out_facility_type == Link_Components::Types::Link_Type_Keys::TRANSIT)
							{							
								link->template touch_transit<bool>(true);
								break;
							}
						}
					}
				}

				int my_itr = 0;
				for (auto orig_itr = starts.begin(); orig_itr != starts.end(); orig_itr++)
				{
					global_edge_id start = global_edge_id (*orig_itr);
					
					float routed_time = Dijkstra_Walk<MT, typename MT::walk_to_transit_tree_agent_type, typename MT::graph_pool_type>(&proxy_agent, _routable_graph_pool, start, debug_route);
					

					if (my_itr % 10000 == 0) cout << "\tLink Counter:\t" << my_itr << endl;
					++my_itr;
				}


			}

			void compute_drive_fft_to_transit(Network<typename MasterType::network_type>* source_network)
			{
				cout << "Constructing the free flow drive time tree to closest transit for better A* filtering..." << endl;

				Routable_Agent<typename MT::drive_to_transit_tree_agent_type> proxy_agent;

				typedef typename Graph_Pool<typename MT::graph_pool_type>::base_edge_type base_edge_type;
				Graph_Pool<typename MT::graph_pool_type>* graph_pool = _routable_graph_pool;

				typedef Network<typename MasterType::network_type> Network_Interface;

				typedef Intersection_Components::Prototypes::Intersection<typename remove_pointer<typename Network_Interface::get_type_of(intersections_container)::value_type>::type> Intersection_Interface;

				typedef Link_Components::Prototypes::Link<typename remove_pointer<typename Network_Interface::get_type_of(links_container)::value_type>::type> Link_Interface;
				typedef Random_Access_Sequence<typename Network_Interface::get_type_of(links_container), Link_Interface*> Link_Container_Interface;

				bool debug_route = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::debug_route<bool>();

				Network_Interface* network = source_network;

				Link_Container_Interface::iterator link_itr;
				Link_Container_Interface* links = network->links_container<Link_Container_Interface*>();

				std::vector<global_edge_id> starts;

				for (link_itr = links->begin(); link_itr != links->end(); link_itr++)
				{
					Link_Interface* link = (Link_Interface*)(*link_itr);

					Intersection_Interface* down_node = link->downstream_intersection<Intersection_Interface*>();
					Link_Container_Interface& outbound_links = down_node->template outbound_links<Link_Container_Interface&>();

					Link_Components::Types::Link_Type_Keys facility_type = link->template link_type<Link_Components::Types::Link_Type_Keys>();

					if (facility_type == Link_Components::Types::Link_Type_Keys::ARTERIAL || facility_type == Link_Components::Types::Link_Type_Keys::LOCAL)
					{
						global_edge_id in_edge_g;
						in_edge_g.edge_id = link->uuid<int>();
						in_edge_g.graph_id = _multimodal_network_graph_id;
						A_Star_Edge<base_edge_type>* in_edge = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(in_edge_g);
						
						starts.push_back(in_edge_g);

						Link_Container_Interface::iterator out_links_itr;
						for (out_links_itr = outbound_links.begin(); out_links_itr != outbound_links.end(); out_links_itr++)
						{
							Link_Interface* outbound_link = (Link_Interface*)(*out_links_itr);
							Link_Components::Types::Link_Type_Keys out_facility_type = outbound_link->template link_type<Link_Components::Types::Link_Type_Keys>();
													
							if (out_facility_type == Link_Components::Types::Link_Type_Keys::WALK && outbound_link->template touch_transit<bool>() )
							{								
								link->template touch_transit<bool>(true);
								break;
							}
						}
					}
				}

				int my_itr = 0;
				for (auto orig_itr = starts.begin(); orig_itr != starts.end(); orig_itr++)
				{
					global_edge_id start = global_edge_id(*orig_itr);

					float routed_time = Dijkstra_Drive<MT, typename MT::drive_to_transit_tree_agent_type, typename MT::graph_pool_type>(&proxy_agent, _routable_graph_pool, start, debug_route);


					if (my_itr % 10000 == 0) cout << "\tLink Counter:\t" << my_itr << endl;
					++my_itr;
				}


			}

			void update_edge_turn_cost(unsigned int edge_id,float edge_cost,unsigned int outbound_turn_index,float turn_cost)
			{
				typedef Scenario_Components::Prototypes::Scenario< typename MasterType::scenario_type> _Scenario_Interface;

				if (!((_Scenario_Interface*)_global_scenario)->template time_dependent_routing<bool>()) // static network
				{
					global_edge_id edge_lookup;

					edge_lookup.edge_id = edge_id;
					edge_lookup.graph_id = _static_network_graph_id;

					A_Star_Edge<typename MasterType::static_edge_type>* edge = (A_Star_Edge<typename MasterType::static_edge_type>*)_routable_graph_pool->template Get_Edge<typename MasterType::static_graph_type>(edge_lookup);

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

				if(((_Scenario_Interface*)_global_scenario)->template time_dependent_routing<bool>())
				{

					global_edge_id edge_lookup;

					edge_lookup.edge_id = edge_id;
					edge_lookup.graph_id = _time_dependent_network_graph_id;

					A_Star_Edge<typename MasterType::time_dependent_edge_type>* edge = (A_Star_Edge<typename MasterType::time_dependent_edge_type>*)_routable_graph_pool->template Get_Edge<typename MasterType::time_dependent_graph_type>(edge_lookup);

					if (edge == nullptr)
					{
						THROW_EXCEPTION("ERROR in update_edge_turn_cost: edge not found, id="<<edge_lookup.edge_id<<", edge_cost="<<edge_cost<<", outbound_turn_index="<<outbound_turn_index<<", turn_cost'"<<turn_cost<<endl);
					}

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
		#pragma region static choice option parameter definitions
		define_static_member_variable(Routable_Network_Implementation, transferPenalty);
		define_static_member_variable(Routable_Network_Implementation, waitWeight);
		define_static_member_variable(Routable_Network_Implementation, walkWeight);
		define_static_member_variable(Routable_Network_Implementation, bikeWeight);
		define_static_member_variable(Routable_Network_Implementation, ivtWeight);
		define_static_member_variable(Routable_Network_Implementation, carWeight);
		define_static_member_variable(Routable_Network_Implementation, scanThreshold);
		define_static_member_variable(Routable_Network_Implementation, costThreshold);
		define_static_member_variable(Routable_Network_Implementation, waitThreshold);
		define_static_member_variable(Routable_Network_Implementation, walkThreshold);
		define_static_member_variable(Routable_Network_Implementation, walkSpeed);
		define_static_member_variable(Routable_Network_Implementation, bikeThreshold);
		define_static_member_variable(Routable_Network_Implementation, bikeSpeed);
		define_static_member_variable(Routable_Network_Implementation, debug_route);
		define_static_member_variable(Routable_Network_Implementation, multimodal_dijkstra);
		#pragma endregion

		template<typename MasterType, typename InheritanceList>
		Layered_Data_Array<float> Routable_Network_Implementation<MasterType,InheritanceList>::_moe_data;

		template<typename MasterType, typename InheritanceList>
		std::unordered_map<int,int> Routable_Network_Implementation<MasterType,InheritanceList>::_link_id_to_moe_data;

		template<typename MasterType, typename InheritanceList>
		Layered_Data_Array<float> Routable_Network_Implementation<MasterType, InheritanceList>::_turn_moe_data;

		template<typename MasterType, typename InheritanceList>
		std::unordered_map<int, int> Routable_Network_Implementation<MasterType, InheritanceList>::_turn_id_to_moe_data;

	}
}
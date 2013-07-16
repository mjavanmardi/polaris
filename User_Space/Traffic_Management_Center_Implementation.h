#pragma once
#include "Traffic_Management_Center_Prototype.h"
//#include "Io\Geometry.h"
#include "Polaris_Sensor_Implementation.h"
#include "Geometry_Implementation.h"


namespace Traffic_Management_Center_Components
{
	namespace Types
	{
		struct TmcNode
		{
		public:
			TmcNode(int id_) : id(id_){}
			int id;
		};
		/// Contains Link information
		struct AdjNode
		{
			int downstream_node; ///< The end node of the link
			double cost; ///< Cost of traversing the link
			int dir_link_uid; ///< id of the link. This attribute is not required by algorithms but needed to be integrated with other polaris components
		};

		/// This class defines a directed weighted graph. 
		/// In TMC graph is used to find directed components and weights are not required, however in case those are needed in the future, I included them
		class Digraph
		{
		public:
			typedef std::vector<AdjNode> AdjListRow;
			typedef std::unordered_map<int, AdjListRow> AdjList;
			typedef AdjList::const_iterator AdjListIt;
			typedef AdjListRow::const_iterator AdjListRowIt;
			typedef std::set<int> NodeContainer;

			Digraph() {this->v = 0; this->e = 0;}
			/// Returns number of vertices in the graph. The vertices counting is not cleanly implemented, so the result is not to be trusted
			const int V() {return (int)this->all_nodes.size();}
			/// Returns number of edges 
			const int E() {return this->e;}
			/// Use this function to populate the graph
			void addEdge(int v, int w, float cost, int dir_link_uid = -1)
			{
				adj_node.cost = (double) cost; 
				adj_node.dir_link_uid = dir_link_uid;
				adj_node.downstream_node = w;
				adj[v].push_back(adj_node);
				adj[w]; // create empty adjacency list to keep the Adj consistent
				e++;
				all_nodes.insert(v);
				all_nodes.insert(w);
			}
			/// Returns a set of all of the nodes defined in the graph
			const NodeContainer AllNodes() const
			{
				return all_nodes;
			}
			/// Returns the outgoing edges that are adjacent to the node v
			const AdjListRow& Adj(int v) const
			{
				const AdjListIt it = adj.find(v);
				if (it != adj.end()) return adj.at(v);
				else exit(0);
			}
			/// Builds a new graph with directions of all of the edges reversed and returns a pointer to it
			Digraph* reverse() const
			{
				Digraph* result = new Digraph();
				for (auto adj_list_it = adj.cbegin(); adj_list_it != adj.cend(); ++adj_list_it)
				{
					int dest = adj_list_it->first;
					for (auto adj_row_it = adj_list_it->second.cbegin(); adj_row_it != adj_list_it->second.cend(); ++adj_row_it)
					{
						result->addEdge(adj_row_it->downstream_node, dest, (float)adj_row_it->cost, adj_row_it->dir_link_uid);
					}
				}
				return result;
			}
			void print() const
			{
				for (auto adj_list_it = adj.cbegin(); adj_list_it != adj.cend(); ++adj_list_it)
				{
					int dest = adj_list_it->first;
					for (auto adj_row_it = adj_list_it->second.cbegin(); adj_row_it != adj_list_it->second.cend(); ++adj_row_it)
					{
						cout << dest << ',' << adj_row_it->downstream_node << '\n'; 
					}
				}
			}
			void Clear()
			{
				v = 0;
				e = 0;
				// maps a node to a vector of 2-tuples. Each tuple corresponds to an adjacent node
				// first element of the tuple contains the node id and the second constrains link cost
				adj.clear();
				all_nodes.clear();
			}

		private:
			int v;
			int e;
			// maps a node to a vector of 2-tuples. Each tuple corresponds to an adjacent node
			// first element of the tuple contains the node id and the second constrains link cost
			AdjList adj;
			NodeContainer all_nodes;
			AdjNode adj_node;
		};

		class WeakCC
		{
		private:
			std::unordered_map<int, bool> marked;
			std::unordered_map<int, int> component_id;
			std::unordered_map<int, std::vector<int>> link_component;
			int count;
			Digraph* g_reverse;
			void dfs(const Digraph& g, int v)
			{
				marked[v] = true;
				component_id[v] = count;
				Digraph::AdjListRow l = g.Adj(v);
				Digraph::AdjListRow l_reverse = g_reverse->Adj(v);
				for (Digraph::AdjListRowIt it = l.cbegin();  it!=l.cend(); ++it)
				{
					if ( !Marked(it->downstream_node) ) 
					{
						link_component[count].push_back(it->dir_link_uid);
						dfs(g, it->downstream_node);
					}
				}
				for (Digraph::AdjListRowIt it = l_reverse.cbegin();  it!=l_reverse.cend(); ++it)
				{
					if ( !Marked(it->downstream_node) ) 
					{	
						link_component[count].push_back(it->dir_link_uid);
						dfs(g, it->downstream_node);
					}
				}
			}
			bool const Marked(const int& v)
			{
				std::unordered_map<int, bool>::iterator res = marked.find(v);
				if (res == marked.end())
				{
					return false;
				}
				else
					return res->second;
			}
		public:
			void Clear()
			{
				marked.clear();
				component_id.clear();
				link_component.clear();
				count = 0;
				g_reverse = NULL;
			}

			//Runs the algorithm on the graph g
			void Run(const Digraph& g)
			{
				count = 0;
				Digraph::NodeContainer nodes = g.AllNodes();
				g_reverse = g.reverse();
				for (Digraph::NodeContainer::iterator it = nodes.begin(); it!=nodes.end();++it)
				{
					if (!Marked(*it))
					{
						dfs(g, *it);
						count++;

					}
				}
			}
			/// Returns the total number of components
			int Count()
			{
				return count;
			}	
			/// Returns the id of the component to which node v belongs
			int Id(int v) const
			{
				return component_id.at(v);
			}
			/// This is the result of the algorithm, a map: component_id -> component_links
			const std::unordered_map<int, std::vector<int> >& LinkComponents() const
			{
				return link_component;
			}
	
		};
	}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Simple_TMC:public Polaris_Component<APPEND_CHILD(Simple_TMC),MasterType,Execution_Object>
		{
			
			typedef typename Polaris_Component<APPEND_CHILD(Simple_TMC),MasterType,Execution_Object>::Component_Type ComponentType;
			
			typedef MasterType Reference_Type;

			// added for convenience
			typedef Network_Event<typename MasterType::type_of(base_network_event),ComponentType> Base_Network_Event_Interface;

			typedef Network_Event<typename MasterType::congestion_network_event_type> Congestion_Network_Event_Interface;

			typedef Link_Prototype<typename type_of(MasterType::link),ComponentType> Link_Interface;

			typedef Advisory_ITS<typename MasterType::type_of(variable_speed_sign),Simple_TMC> Variable_Speed_Sign_Interface;
			typedef Advisory_ITS<typename MasterType::type_of(variable_word_sign),Simple_TMC> Variable_Word_Sign_Interface;
			typedef Advisory_ITS<typename MasterType::type_of(advisory_radio),Simple_TMC> Advisory_Radio_Interface;

			typedef Depot<typename MasterType::type_of(depot),Simple_TMC> Depot_Interface;
			typedef Link_Control<typename MasterType::type_of(link_control),Simple_TMC> Link_Control_Interface;
			typedef Sensor<typename MasterType::type_of(link_sensor),Simple_TMC> Sensor_Interface;
			
			typedef Zone_Components::Prototypes::Zone_Prototype<typename MasterType::zone_type,ComponentType> Zone_Interface;
			define_container_and_value_interface(Location_Container_Interface,Location_Interface,typename Link_Interface::get_type_of(activity_locations),Containers::Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);


			// that places a pointer to network_event_manager_type
			//			  name of the prototype class		variable name					underlying type
			member_prototype(Network_Event_Manager, network_event_manager, typename MasterType::network_event_manager_type, none, none);

			member_data(vector<Variable_Word_Sign_Interface*>,variable_word_signs,none,none);
			member_data(vector<Variable_Speed_Sign_Interface*>,variable_speed_signs,none,none);
			member_data(vector<Advisory_Radio_Interface*>,advisory_radios,none,none);
			
			member_data(vector<Depot_Interface*>,depots,none,none);
			member_data(vector<Link_Control_Interface*>,link_controls,none,none);

			member_data(vector<Sensor_Interface*>,traffic_sensors,none,none);

			member_data(vector<Base_Network_Event_Interface*>,tracked_events,none,none);

			//                                          name of the function
			declare_feature_conditional_implementation(TMC_Conditional)
			{
				response.next._iteration = _iteration + 5*60;
				response.next._sub_iteration = 0;

				//this variable specifies whether the the Event function will be called
				response.result = true;
			}

			declare_feature_event_implementation(TMC_Event)
			{
				ComponentType* pthis = (ComponentType*)_this;

				pthis->template Load_New_Events<ComponentType,ComponentType,NT>();

				pthis->Analyze_Sensors();

				pthis->template DecideOnEventsToBeDisplayed<ComponentType,ComponentType,NT>();
			}

			void Analyze_Sensors()
			{
				Types::Digraph my_digraph;

				for(typename vector<Sensor_Interface*>::iterator itr = _traffic_sensors.begin();itr!=_traffic_sensors.end();itr++)
				{
					Sensor_Interface* sensor = *itr;

					if( sensor->template Check_Outlier<NT>() )
					{
						Link_Interface* covered_link = sensor->template covered_link<Link_Interface*>();

						Intersection_Prototype<typename MasterType::intersection_type>* intersection;
						
						intersection = covered_link->template upstream_intersection<Intersection_Prototype<typename MasterType::intersection_type>*>();

						int v = intersection->template uuid<int>();

						intersection = covered_link->template downstream_intersection<Intersection_Prototype<typename MasterType::intersection_type>*>();

						int w = intersection->template uuid<int>();

						int id = covered_link->template dbid<int>();

						my_digraph.addEdge(v,w,id);
					}
				}

				Types::WeakCC my_weakcc;

				my_weakcc.Run(my_digraph);

				const unordered_map<int, std::vector<int> >& link_components = my_weakcc.LinkComponents();
				vector<typename MasterType::link_type*> affected_links;
				
				//cout << "Done analyzing, number of congestion events: " << link_components.size() << endl;

				//for(unordered_map<int, std::vector<int> >::const_iterator map_itr=link_components.begin();map_itr!=link_components.end();map_itr++)
				//{
				//	Congestion_Network_Event_Interface* new_event = (Congestion_Network_Event_Interface*)Allocate<Congestion_Network_Event_Interface::ComponentType>();

				//	_tracked_events.push_back((Base_Network_Event_Interface*)new_event);

				//	

				//	unordered_map<int,vector<typename MasterType::link_type*>>& db_map=((Network_Prototype<typename type_of(MasterType::network),ComponentType>*)_global_network)->template db_id_to_links_map<unordered_map<int,vector<typename MasterType::link_type*>>&>();

				//	for(typename vector<int>::const_iterator itr=map_itr->second.begin();itr!=map_itr->second.end();itr++)
				//	{
				//		int link = *itr;

				//		if(db_map.count(link))
				//		{
				//			vector<typename MasterType::link_type*>& links=db_map[link];

				//			typename vector<typename type_of(MasterType::link)*>::iterator vitr;

				//			for(vitr=links.begin();vitr!=links.end();vitr++)
				//			{
				//				Link_Interface* link = (Link_Interface*)(*vitr);
				//				affected_links.push_back( *vitr );
				//				//Location_Container_Interface* locations = link->template activity_locations<Location_Container_Interface*>();

				//				//// push locations from link to affected locations container
				//				//for (typename Location_Container_Interface::iterator litr = locations->begin(); litr != locations->end(); ++litr)
				//				//{
				//				//	Location_Interface* loc = (*litr);
				//				//	this->_affected_locations.push_back(loc);
				//				//	zone_set.insert(loc->template zone<Zone_Interface*>());
				//				//}

				//			}
				//		}
				//	}

				//	new_event->Initialize<MasterType>(_iteration, _iteration + 5*60, affected_links);

				//	new_event->Start<NT>();

				//	affected_links.clear();
				//}
			}

			feature_implementation void Load_New_Events()
			{
				vector<Base_Network_Event_Interface*> current_events;
				_network_event_manager->template Get_Network_Events<typename type_of(MasterType::base_network_event)>(current_events);

				for(typename vector<Advisory_Radio_Interface*>::iterator itr=_advisory_radios.begin();itr!=_advisory_radios.end();itr++)
				{
					(*itr)->template Push_Network_Events<typename type_of(MasterType::base_network_event)>((vector<Network_Event<typename type_of(MasterType::base_network_event)>*>&)current_events);
				}

				for(typename vector<Variable_Word_Sign_Interface*>::iterator itr=_variable_word_signs.begin();itr!=_variable_word_signs.end();itr++)
				{
					(*itr)->template Push_Network_Events<typename type_of(MasterType::base_network_event)>((vector<Network_Event<typename type_of(MasterType::base_network_event)>*>&)current_events);
				}

				for(typename vector<Variable_Speed_Sign_Interface*>::iterator itr=_variable_speed_signs.begin();itr!=_variable_speed_signs.end();itr++)
				{
					(*itr)->template Push_Network_Events<typename type_of(MasterType::base_network_event)>((vector<Network_Event<typename type_of(MasterType::base_network_event)>*>&)current_events);
				}

				for(typename vector<Depot_Interface*>::iterator itr=_depots.begin();itr!=_depots.end();itr++)
				{
					(*itr)->template Push_Network_Events<typename type_of(MasterType::base_network_event)>((vector<Network_Event<typename type_of(MasterType::base_network_event)>*>&)current_events);
				}
			}

			feature_implementation void DecideOnEventsToBeDisplayed()
			{
				vector<Base_Network_Event_Interface*> current_events;
				_network_event_manager->template Get_Network_Events<typename type_of(MasterType::base_network_event)>(current_events);
				for(typename vector<Advisory_Radio_Interface*>::iterator itr=_advisory_radios.begin();itr!=_advisory_radios.end();itr++)
				{
					vector<Base_Network_Event_Interface*> events_to_display;
					//some calculations here
					(*itr)->template Push_Displayed_Network_Events<typename type_of(MasterType::base_network_event)>((vector<Network_Event<typename type_of(MasterType::base_network_event)>*>&)events_to_display);
				}
				for(typename vector<Variable_Word_Sign_Interface*>::iterator itr=_variable_word_signs.begin();itr!=_variable_word_signs.end();itr++)
				{
					vector<Base_Network_Event_Interface*> events_to_display;
					//some calculations here
					(*itr)->template Push_Displayed_Network_Events<typename type_of(MasterType::base_network_event)>((vector<Network_Event<typename type_of(MasterType::base_network_event)>*>&)events_to_display);
				}
				for(typename vector<Variable_Speed_Sign_Interface*>::iterator itr=_variable_speed_signs.begin();itr!=_variable_speed_signs.end();itr++)
				{
					vector<Base_Network_Event_Interface*> events_to_display;
					//some calculations here
					(*itr)->template Push_Displayed_Network_Events<typename type_of(MasterType::base_network_event)>((vector<Network_Event<typename type_of(MasterType::base_network_event)>*>&)events_to_display);
				}
			}

			feature_implementation void Initialize()
			{
				this_component()->template Read_Database<ComponentType,CallerType,TargetType>();

				((ComponentType*)this)->template Load_Event<ComponentType>(&ComponentType::template TMC_Conditional<ComponentType,NT,NT>,&ComponentType::template TMC_Event<ComponentType,NT,NT>, 0, 0);
			}

			/// This function reads the speed data from LinkMOE data and uses is to train the outliers detector
			void Train_Detectors(unique_ptr<odb::database>& db, unordered_map<int, Detector1DU<double> >& out_link_detectors)
			{
				using namespace odb;
				using namespace polaris::io;

				int id;
				Detector1DU<double> detector;
				vector<double> training_data;

				result<LinkMOE> rmoe(db->query<LinkMOE>(query<LinkMOE>::true_expr));

				
				for (auto it = (rmoe.begin()); it!= rmoe.end(); ++it  )
				{
					id = it->getLink_Uid();
					auto it_ld = out_link_detectors.find(id);


					if (it_ld == out_link_detectors.end())
					{
						detector.Clear();
						detector.Update(it->getLink_Density());
						out_link_detectors[id] = detector;
					}
					else
					{
						it_ld->second.Update(it->getLink_Density());
					}
				}
			}

			feature_implementation void Read_Database()
			{
				using namespace odb;
				using namespace polaris::io;
				
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type,ComponentType> _Scenario_Interface;
				string db_name(((_Scenario_Interface*)_global_scenario)->template database_name<string&>());

				unique_ptr<database> db (open_sqlite_database (db_name));
				
				session s;

				transaction t(db->begin());

				cout << "Reading Components" << endl;

				
				cout << "\tVSS" << endl;

				result<VSS> vss_component_result=db->template query<VSS>(query<VSS>::true_expr);
				
				Variable_Speed_Sign_Interface::template Initialize_Type<NT>();

				for(typename result<VSS>::iterator db_itr = vss_component_result.begin (); db_itr != vss_component_result.end (); ++db_itr)
				{
					Variable_Speed_Sign_Interface* its_component = (Variable_Speed_Sign_Interface*)Allocate<typename Variable_Speed_Sign_Interface::ComponentType>();
					its_component->template Initialize< VSS& >( *db_itr );
					_variable_speed_signs.push_back(its_component);				
				}
				

				cout << "\tVWS" << endl;

				result<VMS> vws_component_result=db->template query<VMS>(query<VMS>::true_expr);

				Variable_Word_Sign_Interface::template Initialize_Type<NT>();

				for(typename result<VMS>::iterator db_itr = vws_component_result.begin (); db_itr != vws_component_result.end (); ++db_itr)
				{
					Variable_Word_Sign_Interface* its_component = (Variable_Word_Sign_Interface*)Allocate<typename Variable_Word_Sign_Interface::ComponentType>();
					its_component->template Initialize< VMS& >( *db_itr );
					_variable_word_signs.push_back(its_component);				
				}


				cout << "\tHAR" << endl;

				result<HAR> har_component_result=db->template query<HAR>(query<HAR>::true_expr);

				Advisory_Radio_Interface::template Initialize_Type<NT>();

				for(typename result<HAR>::iterator db_itr = har_component_result.begin (); db_itr != har_component_result.end (); ++db_itr)
				{
					Advisory_Radio_Interface* its_component = (Advisory_Radio_Interface*)Allocate<typename Advisory_Radio_Interface::ComponentType>();
					its_component->Initialize< HAR& >( *db_itr );
					_advisory_radios.push_back(its_component);				
				}


				cout << "\tDepot" << endl;

				result<polaris::io::Depot> depot_component_result=db->template query<polaris::io::Depot>(query<polaris::io::Depot>::true_expr);

				Depot_Interface::template Initialize_Type<NT>();

				for(typename result<polaris::io::Depot>::iterator db_itr = depot_component_result.begin (); db_itr != depot_component_result.end (); ++db_itr)
				{
					Depot_Interface* its_component = (Depot_Interface*)Allocate<typename Depot_Interface::ComponentType>();
					its_component->template Initialize< polaris::io::Depot& >( *db_itr );
					_depots.push_back(its_component);				
				}


				cout << "\tLink Control" << endl;

				result<OpenShoulder> link_control_component_result=db->template query<OpenShoulder>(query<OpenShoulder>::true_expr);

				Link_Control_Interface::template Initialize_Type<NT>();

				for(typename result<OpenShoulder>::iterator db_itr = link_control_component_result.begin (); db_itr != link_control_component_result.end (); ++db_itr)
				{
					Link_Control_Interface* its_component = (Link_Control_Interface*)Allocate<typename Link_Control_Interface::ComponentType>();
					its_component->template Initialize< OpenShoulder& >( *db_itr );
					_link_controls.push_back(its_component);				
				}

				cout << "\tDetectors" << endl;

				unordered_map<int, Detector1DU<double> > link_detectors;

				Train_Detectors(db, link_detectors);

				cout << "\tSensor" << endl;

				result<Fixed_Sensor> sensor_component_result=db->template query<Fixed_Sensor>(query<Fixed_Sensor>::true_expr);

				Sensor_Interface::template Initialize_Type<NT>();

				for(typename result<Fixed_Sensor>::iterator db_itr = sensor_component_result.begin (); db_itr != sensor_component_result.end (); ++db_itr)
				{
					Sensor_Interface* its_component = (Sensor_Interface*)Allocate<typename Sensor_Interface::ComponentType>();
					
					its_component->template Initialize< Fixed_Sensor& >( *db_itr );

					its_component->template Attach_Detector< unordered_map<int, Detector1DU<double> >& >( link_detectors );

					_traffic_sensors.push_back(its_component);				
				}

				cout << "Done Reading" << endl;
			}

		};
	}
}

using namespace Traffic_Management_Center_Components::Implementations;
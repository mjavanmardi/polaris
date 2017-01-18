#pragma once

#include "Vehicle_Chooser_Prototype.h"
#include "Vehicle_Prototype.h"
#include "Household_Prototype.h"
#include "Demand.h"

namespace Household_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Planning classes
		//----------------------------------------------------------------------------------
		/*
		implementation struct Vehicle_Choice_Option : public Choice_Model_Components::Implementations::Choice_Option_Base<MasterType,INHERIT(Vehicle_Choice_Option)>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Vehicle_Choice_Option),Data_Object>::Component_Type ComponentType;

			static m_data(float, BETA, NONE, NONE);
			
			//------------------------------------------------------------------------------------------------------------------------------------

			// Feature called from prototype and by Choice_Model
			virtual double Calculate_Utility()
			{
				double utility = -FLT_MAX;

				return utility;				
			}

			virtual void Print_Utility()
			{
				
			}

			// Local features

		};
		

		//INITIALIZE Vehicle Choice STATIC PARAMETERS
		#pragma region Choice option parameters	
		template<typename MasterType, typename InheritanceList> typename Vehicle_Choice_Option<MasterType, InheritanceList>::type_of(BETA) Vehicle_Choice_Option<MasterType,InheritanceList>::_BETA = 0.0;
		#pragma endregion
		*/

		implementation struct Vehicle_Chooser_Implementation : public Polaris_Component<MasterType,INHERIT(Vehicle_Chooser_Implementation),Data_Object>
		{
			// Tag as Implementation
			//typedef typename Polaris_Component<MasterType,INHERIT(Vehicle_Chooser_Implementation),Data_Object>::Component_Type ComponentType;

			// Pointer to the Parent class
			m_prototype(Household_Components::Prototypes::Household, typename MasterType::household_type, Parent_Household, NONE, NONE);
			//m_prototype(Choice_Model_Components::Prototypes::Choice_Model, typename MasterType::mnl_model_type, Choice_Model, NONE, NONE);
			

			// Interface definitions	
			typedef Demand_Components::Prototypes::Demand<typename MasterType::demand_type> _Demand_Interface;
			//typedef Choice_Model_Components::Prototypes::Choice_Model<typename MasterType::mnl_model_type > _Choice_Model_Interface;
			//typedef Prototypes::Vehicle_Choice_Option<typename MasterType::mode_choice_option_type> _Mode_Choice_Option_Interface;
			//typedef Choice_Model_Components::Prototypes::Choice_Option<typename MasterType::mode_choice_option_type> _Choice_Option_Interface;

			typedef Household_Components::Prototypes::Household<typename type_of(Parent_Household)> household_itf;
			typedef Household_Components::Prototypes::Household_Properties<typename household_itf::get_type_of(Static_Properties)> household_static_properties_itf;
		
			typedef Network_Components::Prototypes::Network< typename household_itf::get_type_of(network_reference)> _Network_Interface;
			
			//typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
			//typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(_Activity_Locations_Container_Interface)>  _Activity_Location_Interface;	
				
			//typedef Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links)> _Links_Container_Interface;
			//typedef Link_Components::Prototypes::Link<typename get_component_type(_Links_Container_Interface)>  _Link_Interface;
	
			//typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
			//typedef Zone_Components::Prototypes::Zone<typename get_mapped_component_type(_Zones_Container_Interface)>  _Zone_Interface;

			//typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(zone_ids_container),int> _Zone_Ids_Interface;

			typedef Random_Access_Sequence< typename household_itf::get_type_of(Vehicles_Container)> _Vehicles_Container_Interface;
			typedef Vehicle_Components::Prototypes::Vehicle<typename get_component_type(_Vehicles_Container_Interface)> _Vehicle_Interface;
			typedef typename get_component_type(_Vehicles_Container_Interface) _Vehicle_type;
			//typedef Vehicle_Components::Prototypes::Vehicle_Characteristics<typename _Vehicle_Interface::get_type_of(vehicle_characteristics)> _Vehicle_Characteristics_Interface;

			typedef Random_Access_Sequence< typename _Demand_Interface::get_type_of(vehicle_types_container)> _Vehicle_Types_Interface;
			
			

			// static container for vehicle distribution by type and powertrain for each census tract
			//static m_container(concat(std::unordered_map<int, map_type >), vehicle_distribution_container, NONE, NONE);
			//static m_container(std::unordered_map<SynthesisZoneType, std::map<Vehicle_Components::Types::EPA_Vehicle_Class_Keys, std::map< Vehicle_Components::Types::Powertrain_Type_Keys, float>>>, vehicle_distribution_container, NONE, NONE);			
			//static m_container(std::unordered_map<SynthesisZoneType, Veh_Type>, vehicle_distribution_container, NONE, NONE);
			typedef Vehicle_Components::Prototypes::Vehicle_Characteristics<typename MasterType::vehicle_characteristics_type> vehicle_characteristics_interface;
			typedef pair<vehicle_characteristics_interface*, float> vehicle_probability_pair_type;
			typedef pair<double, vector<vehicle_probability_pair_type>> vehicle_distribution_pair_type;
			typedef std::unordered_map<double, vector<vehicle_probability_pair_type>> vehicle_distribution_container_type;
			static m_container(vehicle_distribution_container_type, vehicle_distribution_container, NONE, NONE);


			//template<typename TargetType> void Initialize()	{}

			template<typename T> void Select_Vehicles(T census_zone)
			{
				_Demand_Interface* demand = (_Demand_Interface*)_global_demand;
				_Vehicle_Types_Interface* veh_types = demand->vehicle_types_container<_Vehicle_Types_Interface*>();
				
				_Network_Interface* network = this->_Parent_Household->network_reference<_Network_Interface*>();
				_Vehicles_Container_Interface* vehicles = _Parent_Household->Vehicles_Container<_Vehicles_Container_Interface*>();
				
				this->_Parent_Household->network_reference<_Network_Interface*>();
				household_static_properties_itf* household_properties = _Parent_Household->Static_Properties<household_static_properties_itf*>();

				int lookup_id = census_zone->ID<int>();

				// create a vehicle for num vehicles in the household
				for (int i = 0; i < household_properties->Number_of_vehicles<int>(); ++i)
				{
					float rand = GLOBALS::Uniform_RNG.template Next_Rand<float>();					
					vehicle_distribution_container_type::iterator itr = _vehicle_distribution_container.find(lookup_id);

					// home census tract could not be found in vehicle type distribution file!
					if (itr == _vehicle_distribution_container.end())
					{
						THROW_EXCEPTION("ERROR: home census tract could not be found in vehicle type distribution file!");
					}
					else
					{												
						//_Vehicle_Types_Interface veh_types = (*itr)->second;  //.push_back(vehicle_probability_pair_type(veh_char, prob));
						double sumProb = 0;
						for (auto x = (*itr).second.begin(); x != (*itr).second.end(); x++)						
						{
							sumProb += x->second;
							if (sumProb >= rand)
							{
								// Allocate a new vehicle
								_Vehicle_Interface* veh = (_Vehicle_Interface*)Allocate<_Vehicle_type>();
								veh->initialize(x->first, _Parent_Household->uuid<int>());
								veh->is_integrated(true);
								// Push to household vehicle container
								vehicles->push_back(veh);
								break;
							}
						}							
					}

					/*
					// 1. Draw a random set of vehicle properties from your input file for the current zone
					Vehicle_Components::Types::Fuel_Type_Keys fuel_type;
					Vehicle_Components::Types::EPA_Vehicle_Class_Keys veh_class;
					Vehicle_Components::Types::Powertrain_Type_Keys pt_type;
					
					//.........

					// 2. search the demand->vehicle_char.. container until you match the three components above, and pull pointer from it
					for (_Vehicle_Types_Interface::iterator vitr = veh_types->begin(); vitr != veh_types->end(); ++vitr)
					{
						_Vehicle_Characteristics_Interface* vc = (_Vehicle_Characteristics_Interface*)(*vitr);
						if (vc->fuel_type<Vehicle_Components::Types::Fuel_Type_Keys>() == fuel_type && vc->vehicle_class<Vehicle_Components::Types::EPA_Vehicle_Class_Keys>() == veh_class && vc->powertrain_type<Vehicle_Components::Types::Powertrain_Type_Keys>() == pt_type) veh->vehicle_characteristics(vc);
					}
					*/
					
					// Done.
				}				
			}



			// Static initialization
			static m_data(bool, is_initialized, NONE, NONE);
			template<typename T> static void static_initializer(string filename, T demand)
			{
				// make sure this is only run once				
				if(_is_initialized ) return; else _is_initialized = true;

				typedef Random_Access_Sequence<typename strip_modifiers(T)::get_type_of(vehicle_types_container)> vehicle_types_container_type;

				vehicle_types_container_type* all_vehicle_types = demand->vehicle_types_container<vehicle_types_container_type*>();
				// add items
				ifstream data_file;
				data_file.open(filename.c_str(), ios::in);
				if (!data_file.is_open()) cout << endl << "Could not open 'vehicle_distribution.txt' in the working directory.  Check to make sure the vehicle_distribution.txt file exists.";

				string line;
				double census_tract;
				float prob ;
				string veh_class_txt, pt_type_txt;
				string strCensusTract;
				string type;
				Vehicle_Components::Types::EPA_Vehicle_Class_Keys veh_class;
				Vehicle_Components::Types::Powertrain_Type_Keys pt_type;
				int veh_class_i, pt_type_i;
								
				getline(data_file, line); //throw out header
				while (getline(data_file, line))
				{
					std::stringstream   linestream(line);
					//linestream >> strCensusTract;
					std::getline(linestream, strCensusTract, '\t');
					census_tract = stod(strCensusTract);
					std::getline(linestream, veh_class_txt, '\t');
					std::getline(linestream, pt_type_txt, '\t');
					linestream >> prob;

					if(veh_class_txt == "Compact")			veh_class = Vehicle_Components::Types::EPA_Vehicle_Class_Keys::CAR_COMPACT;			
					else if ( veh_class_txt == "Midsize")		veh_class = Vehicle_Components::Types::EPA_Vehicle_Class_Keys::CAR_MID_SIZE;
					else if ( veh_class_txt == "Midsize SUV")	veh_class = Vehicle_Components::Types::EPA_Vehicle_Class_Keys::SUV_MID_SIZE;
					else if ( veh_class_txt == "Small SUV")		veh_class = Vehicle_Components::Types::EPA_Vehicle_Class_Keys::WAGON_COMPACT;
					else if ( veh_class_txt == "Pickup")		veh_class = Vehicle_Components::Types::EPA_Vehicle_Class_Keys::TRUCK_MID_SIZE;
					else    								veh_class = Vehicle_Components::Types::EPA_Vehicle_Class_Keys::CAR_COMPACT;
					
					if(pt_type_txt ==  "PHEV")				pt_type = Vehicle_Components::Types::Powertrain_Type_Keys::PHEV;
					else if (pt_type_txt == "HEV")			pt_type = Vehicle_Components::Types::Powertrain_Type_Keys::HEV;				
					else if (pt_type_txt == "ICE")			pt_type = Vehicle_Components::Types::Powertrain_Type_Keys::CONVENTIONAL;
					else if (pt_type_txt == "LSEV")			pt_type = Vehicle_Components::Types::Powertrain_Type_Keys::BEV;				
					else if (pt_type_txt == "EV")			pt_type = Vehicle_Components::Types::Powertrain_Type_Keys::BEV;				
					else if (pt_type_txt == "H2")			pt_type = Vehicle_Components::Types::Powertrain_Type_Keys::FCEV;				
					else 									pt_type = Vehicle_Components::Types::Powertrain_Type_Keys::CONVENTIONAL;	

					// find your vehicle characteristics pointer given the above information...
					vehicle_characteristics_interface* veh_char = nullptr;

					// find the type in demand->vehicle types list that matches
					for (vehicle_types_container_type::iterator t_itr = all_vehicle_types->begin(); t_itr != all_vehicle_types->end(); ++t_itr)
					{
						vehicle_characteristics_interface* veh = (vehicle_characteristics_interface*)(*t_itr);
						if (veh->vehicle_class<Vehicle_Components::Types::EPA_Vehicle_Class_Keys>() == veh_class && veh->powertrain_type<Vehicle_Components::Types::Powertrain_Type_Keys>() == pt_type)
						{
							if ((veh->powertrain_type<Vehicle_Components::Types::Powertrain_Type_Keys>() == Vehicle_Components::Types::CONVENTIONAL && veh->fuel_type<Vehicle_Components::Types::Fuel_Type_Keys>() == Vehicle_Components::Types::GASOLINE) || veh->powertrain_type<Vehicle_Components::Types::Powertrain_Type_Keys>() != Vehicle_Components::Types::CONVENTIONAL)
							{
								veh_char = (vehicle_characteristics_interface*)*t_itr;
								break;
							}
						}
					}
					if (veh_char == nullptr) THROW_EXCEPTION("ERROR: vehicle class '"<<veh_class<<"' and powertrain type '"<<pt_type<<"' combination not found in the input demand database.");


					vehicle_distribution_container_type::iterator itr = _vehicle_distribution_container.find(census_tract);

					// this zone has not been created yet
					if (itr == _vehicle_distribution_container.end())
					{
						vector<vehicle_probability_pair_type> new_vec;
						new_vec.push_back(vehicle_probability_pair_type(veh_char, prob));
						_vehicle_distribution_container.insert(vehicle_distribution_pair_type(census_tract, new_vec));
					}
					else
					{
						itr->second.push_back(vehicle_probability_pair_type(veh_char, prob));
					}
				}								
			}
		};
				
		template<typename MasterType, typename InheritanceList> typename Vehicle_Chooser_Implementation <MasterType, InheritanceList>::type_of(is_initialized) Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_is_initialized = false;		
		template<typename MasterType, typename InheritanceList> std::unordered_map<double, vector<pair<Vehicle_Components::Prototypes::Vehicle_Characteristics<typename MasterType::vehicle_characteristics_type>*,float>>> Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_vehicle_distribution_container;		
	}
}


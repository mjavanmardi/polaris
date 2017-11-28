#pragma once

#include "Vehicle_Chooser_Prototype.h"
#include "Vehicle_Prototype.h"
#include "Household_Prototype.h"
#include "Demand.h"

namespace Household_Components
{
	namespace Implementations
	{
		implementation struct Vehicle_Chooser_Implementation : public Polaris_Component<MasterType,INHERIT(Vehicle_Chooser_Implementation),Data_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Vehicle_Chooser_Implementation),Data_Object>::Component_Type ComponentType;

			// Pointer to the Parent class
			m_prototype(Household_Components::Prototypes::Household, typename MasterType::household_type, Parent_Household, NONE, NONE);
			m_prototype(Household_Components::Prototypes::Vehicle_Technology_Chooser, typename MasterType::vehicle_technology_chooser_type, Vehicle_Technology_Chooser, NONE, NONE);
					
			// Interface definitions	
			typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
			typedef Demand_Components::Prototypes::Demand<typename MasterType::demand_type> _Demand_Interface;
			typedef Household_Components::Prototypes::Household<typename type_of(Parent_Household)> household_itf;
			typedef Household_Components::Prototypes::Household_Properties<typename household_itf::get_type_of(Static_Properties)> household_static_properties_itf;	
			typedef Network_Components::Prototypes::Network< typename household_itf::get_type_of(network_reference)> _Network_Interface;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _Network_Interface::get_type_of(skimming_faculty)> _Skim_Interface;
			typedef Network_Skimming_Components::Prototypes::LOS<typename MasterType::los_value_type> los_itf;
			typedef Network_Skimming_Components::Prototypes::LOS<typename MasterType::los_invariant_value_type> los_invariant_itf;
			typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
			typedef Zone_Components::Prototypes::Zone<typename get_mapped_component_type(_Zones_Container_Interface)>  _Zone_Interface;
			typedef Random_Access_Sequence< typename household_itf::get_type_of(Vehicles_Container)> _Vehicles_Container_Interface;
			typedef Vehicle_Components::Prototypes::Vehicle<typename get_component_type(_Vehicles_Container_Interface)> _Vehicle_Interface;
			typedef Random_Access_Sequence< typename household_itf::get_type_of(Persons_Container)> _Persons_Container_Interface;
			typedef Person_Components::Prototypes::Person<typename get_component_type(_Persons_Container_Interface)> _Person_Interface;
			typedef Person_Components::Prototypes::Person_Properties< typename _Person_Interface::get_type_of(Properties)> _Properties_Itf;
			typedef Person_Components::Prototypes::Person_Properties< typename _Person_Interface::get_type_of(Static_Properties)> _Static_Properties_Itf;
			typedef typename get_component_type(_Vehicles_Container_Interface) _Vehicle_type;
			typedef Random_Access_Sequence< typename _Demand_Interface::get_type_of(vehicle_types_container)> _Vehicle_Types_Interface;
			typedef Vehicle_Components::Prototypes::Vehicle_Characteristics<typename MasterType::vehicle_characteristics_type> vehicle_characteristics_interface;
			typedef pair<vehicle_characteristics_interface*, float> vehicle_probability_pair_type;
			typedef pair<long long, vector<vehicle_probability_pair_type>> vehicle_distribution_pair_type;
			typedef std::unordered_map<long long, vector<vehicle_probability_pair_type>> vehicle_distribution_container_type;
			m_static_container(vehicle_distribution_container_type, vehicle_distribution_container, NONE, NONE);

			template<typename T> void Initialize(T household)
			{
				this->Parent_Household(household);

				_Vehicle_Technology_Chooser = (Vehicle_Technology_Chooser_type)Allocate<type_of(Vehicle_Technology_Chooser)>();
				_Vehicle_Technology_Chooser->Initialize(household);
			}

			template<typename T> void Select_Vehicles(T census_zone)
			{
				_Demand_Interface* demand = (_Demand_Interface*)_global_demand;
				_Vehicle_Types_Interface* veh_types = demand->vehicle_types_container<_Vehicle_Types_Interface*>();
				
				_Network_Interface* network = this->_Parent_Household->network_reference<_Network_Interface*>();
				_Vehicles_Container_Interface* vehicles = _Parent_Household->Vehicles_Container<_Vehicles_Container_Interface*>();
				
				this->_Parent_Household->network_reference<_Network_Interface*>();
				household_static_properties_itf* household_properties = _Parent_Household->Static_Properties<household_static_properties_itf*>();

				long long lookup_id = census_zone->ID<long long>();

				// create a vehicle for num vehicles in the household
				for (int i = 0; i < household_properties->Number_of_vehicles<int>(); ++i)
				{
					float rand = GLOBALS::Uniform_RNG.template Next_Rand<float>();					
					vehicle_distribution_container_type::iterator itr = _vehicle_distribution_container.find(lookup_id);

					// home census tract could not be found in vehicle type distribution file!
					if (itr == _vehicle_distribution_container.end())
					{
						THROW_WARNING("WARNING: home census tract '"<< lookup_id<< "' could not be found in vehicle type distribution file!"<<endl);
						itr = _vehicle_distribution_container.begin();
					}
											
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

							// determine additional technology for vehicle
							_Vehicle_Technology_Chooser->Select_Vehicle_Technology(veh, _Parent_Household->Get_Primary_Driver<_Person_Interface*>());

							// Push to household vehicle container
							vehicles->push_back(veh);
							break;
						}
					}							
				}				
			}

			// Static initialization
			template<typename T> static void distribution_static_initializer(string filename, T demand)
			{
				// make sure this is only run once				
				if(_is_initialized ) return; else _is_initialized = true;

				// set the default values
				default_static_initializer();


				//==================================================================================================================================
				// Vehicle Type distribution setup
				//----------------------------------------------------------------------------------------------------------------------------------
				typedef Random_Access_Sequence<typename strip_modifiers(T)::get_type_of(vehicle_types_container)> vehicle_types_container_type;
				vehicle_types_container_type* all_vehicle_types = demand->vehicle_types_container<vehicle_types_container_type*>();
				// add items
				ifstream data_file;
				data_file.open(filename.c_str(), ios::in);
				if (!data_file.is_open()) cout << endl << "Could not open 'vehicle_distribution.txt' in the working directory.  Check to make sure the vehicle_distribution.txt file exists.";

				string line;
				long long census_tract;
				float prob ;
				string veh_class_txt, pt_type_txt;
				string strCensusTract;
				string type;
				Vehicle_Components::Types::EPA_Vehicle_Class_Keys veh_class;
				Vehicle_Components::Types::Powertrain_Type_Keys pt_type;
				//int veh_class_i, pt_type_i;
								
				getline(data_file, line); //throw out header
				while (getline(data_file, line))
				{
					std::stringstream   linestream(line);
					//linestream >> strCensusTract;
					std::getline(linestream, strCensusTract, '\t');
					census_tract = stoll(strCensusTract);
					std::getline(linestream, veh_class_txt, '\t');
					std::getline(linestream, pt_type_txt, '\t');
					linestream >> prob;

					if (veh_class_txt == "Compact" || veh_class_txt == "COMPACT" || veh_class_txt == "compact")						veh_class = Vehicle_Components::Types::EPA_Vehicle_Class_Keys::CAR_COMPACT;
					else if (veh_class_txt == "Midsize" || veh_class_txt == "MIDSIZE" || veh_class_txt == "midsize")				veh_class = Vehicle_Components::Types::EPA_Vehicle_Class_Keys::CAR_MID_SIZE;
					else if ( veh_class_txt == "Midsize SUV" || veh_class_txt == "MIDSIZE SUV" || veh_class_txt == "midsize SUV")	veh_class = Vehicle_Components::Types::EPA_Vehicle_Class_Keys::SUV_MID_SIZE;
					else if ( veh_class_txt == "Small SUV" || veh_class_txt == "SMALL SUV" || veh_class_txt == "small SUV")			veh_class = Vehicle_Components::Types::EPA_Vehicle_Class_Keys::WAGON_COMPACT;
					else if ( veh_class_txt == "Pickup" || veh_class_txt == "PICKUP" || veh_class_txt == "pickup")					veh_class = Vehicle_Components::Types::EPA_Vehicle_Class_Keys::TRUCK_MID_SIZE;
					else    																										veh_class = Vehicle_Components::Types::EPA_Vehicle_Class_Keys::CAR_COMPACT;
					
					if(pt_type_txt ==  "PHEV" || pt_type_txt == "phev")				pt_type = Vehicle_Components::Types::Powertrain_Type_Keys::PHEV;
					else if (pt_type_txt == "HEV" || pt_type_txt == "hev")			pt_type = Vehicle_Components::Types::Powertrain_Type_Keys::HEV;
					else if (pt_type_txt == "ICE" || pt_type_txt == "ice")			pt_type = Vehicle_Components::Types::Powertrain_Type_Keys::CONVENTIONAL;
					else if (pt_type_txt == "LSEV" || pt_type_txt == "lsev")		pt_type = Vehicle_Components::Types::Powertrain_Type_Keys::BEV;
					else if (pt_type_txt == "EV" || pt_type_txt == "ev")			pt_type = Vehicle_Components::Types::Powertrain_Type_Keys::BEV;
					else if (pt_type_txt == "H2" || pt_type_txt == "h2")			pt_type = Vehicle_Components::Types::Powertrain_Type_Keys::FCEV;
					else 															pt_type = Vehicle_Components::Types::Powertrain_Type_Keys::CONVENTIONAL;	

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

			// Initializer for the CAV tech choice model
			static bool static_initialize(const string& option_file)
			{
				// set the default values
				default_static_initializer();
				return true;
			}
			static void default_static_initializer()
			{
			}


			// PARAMETER DECLARATIONS - initialize in the static initializer function using scenario parameters
			m_static_data(bool, is_initialized, NONE, NONE);
			#pragma region static parameters		

			#pragma endregion
		};
				
		template<typename MasterType, typename InheritanceList> typename Vehicle_Chooser_Implementation <MasterType, InheritanceList>::type_of(is_initialized) Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_is_initialized = false;		
		template<typename MasterType, typename InheritanceList> std::unordered_map<long long, vector<pair<Vehicle_Components::Prototypes::Vehicle_Characteristics<typename MasterType::vehicle_characteristics_type>*,float>>> Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_vehicle_distribution_container;		
	}
}


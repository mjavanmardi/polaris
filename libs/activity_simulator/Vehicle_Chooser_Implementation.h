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

			typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;

			// Pointer to the Parent class
			m_prototype(Household_Components::Prototypes::Household, typename MasterType::household_type, Parent_Household, NONE, NONE);
					

			// Interface definitions	
			typedef Demand_Components::Prototypes::Demand<typename MasterType::demand_type> _Demand_Interface;

			typedef Household_Components::Prototypes::Household<typename type_of(Parent_Household)> household_itf;
			typedef Household_Components::Prototypes::Household_Properties<typename household_itf::get_type_of(Static_Properties)> household_static_properties_itf;
		
			typedef Network_Components::Prototypes::Network< typename household_itf::get_type_of(network_reference)> _Network_Interface;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _Network_Interface::get_type_of(skimming_faculty)> _Skim_Interface;
			typedef Network_Skimming_Components::Prototypes::LOS<typename MasterType::los_value_type> los_itf;
			typedef Network_Skimming_Components::Prototypes::LOS<typename MasterType::los_invariant_value_type> los_invariant_itf;
			
			//typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
			//typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(_Activity_Locations_Container_Interface)>  _Activity_Location_Interface;	
				
			//typedef Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links)> _Links_Container_Interface;
			//typedef Link_Components::Prototypes::Link<typename get_component_type(_Links_Container_Interface)>  _Link_Interface;
	
			typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
			typedef Zone_Components::Prototypes::Zone<typename get_mapped_component_type(_Zones_Container_Interface)>  _Zone_Interface;

			//typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(zone_ids_container),int> _Zone_Ids_Interface;

			typedef Random_Access_Sequence< typename household_itf::get_type_of(Vehicles_Container)> _Vehicles_Container_Interface;
			typedef Vehicle_Components::Prototypes::Vehicle<typename get_component_type(_Vehicles_Container_Interface)> _Vehicle_Interface;
			typedef Random_Access_Sequence< typename household_itf::get_type_of(Persons_Container)> _Persons_Container_Interface;
			typedef Person_Components::Prototypes::Person<typename get_component_type(_Persons_Container_Interface)> _Person_Interface;
			typedef Person_Components::Prototypes::Person_Properties< typename _Person_Interface::get_type_of(Properties)> _Properties_Itf;
			typedef Person_Components::Prototypes::Person_Properties< typename _Person_Interface::get_type_of(Static_Properties)> _Static_Properties_Itf;
			typedef typename get_component_type(_Vehicles_Container_Interface) _Vehicle_type;
			//typedef Vehicle_Components::Prototypes::Vehicle_Characteristics<typename _Vehicle_Interface::get_type_of(vehicle_characteristics)> _Vehicle_Characteristics_Interface;

			typedef Random_Access_Sequence< typename _Demand_Interface::get_type_of(vehicle_types_container)> _Vehicle_Types_Interface;
			
			

			// static container for vehicle distribution by type and powertrain for each census tract
			//static m_container(concat(std::unordered_map<int, map_type >), vehicle_distribution_container, NONE, NONE);
			//static m_container(std::unordered_map<SynthesisZoneType, std::map<Vehicle_Components::Types::EPA_Vehicle_Class_Keys, std::map< Vehicle_Components::Types::Powertrain_Type_Keys, float>>>, vehicle_distribution_container, NONE, NONE);			
			//static m_container(std::unordered_map<SynthesisZoneType, Veh_Type>, vehicle_distribution_container, NONE, NONE);
			typedef Vehicle_Components::Prototypes::Vehicle_Characteristics<typename MasterType::vehicle_characteristics_type> vehicle_characteristics_interface;
			typedef pair<vehicle_characteristics_interface*, float> vehicle_probability_pair_type;
			typedef pair<long long, vector<vehicle_probability_pair_type>> vehicle_distribution_pair_type;
			typedef std::unordered_map<long long, vector<vehicle_probability_pair_type>> vehicle_distribution_container_type;
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
							Evaluate_Vehicle_Technology_Purchase(veh, _Parent_Household->Get_Primary_Driver<_Person_Interface*>());

							// Push to household vehicle container
							vehicles->push_back(veh);
							break;
						}
					}							
				}				
			}

			// Implementation of Bansal et. al. (2016) - Transportation Research part C (67) - add automation technology based on calculated willingness to pay
			template<typename VehicleItfType, typename PersonItfType> void Evaluate_Vehicle_Technology_Purchase(VehicleItfType* veh, PersonItfType* person)
			{
				_Person_Interface* p = static_cast<_Person_Interface*>(person);
				_Static_Properties_Itf* properties = p->Static_Properties<_Static_Properties_Itf*>();
				household_static_properties_itf* hh_properties = _Parent_Household->Static_Properties<household_static_properties_itf*>();
				_Network_Interface* network = this->_Parent_Household->network_reference<_Network_Interface*>();
				_Zone_Interface* home_zone = _Parent_Household->Home_Location<_Zone_Interface*>();
				_Skim_Interface* skimmer = network->skimming_faculty<_Skim_Interface*>();

				//------------------------------------------------
				// Get variables for input to OP choice model
				//------------------------------------------------
				float age = properties->Age<float>();
				float male = properties->Gender<Person_Components::Types::GENDER>() == Person_Components::Types::GENDER::MALE ? 1.0 : 0.0;
				float lic = 1.0;
				float children = (float)_Parent_Household->Number_of_Children();
				Dollars inc = hh_properties->Income<Dollars>();

				float density = 7435.0f; // fix density at the average for model estimation from the paper to remove its effects from the model

				float num_crash = float(age) / 25.0;
				float smartphone_user = GLOBALS::Normal_RNG.Next_Rand(0.92, 0.27) < 0.5 ? 0.0 : 1.0;
				float carshare_user = GLOBALS::Normal_RNG.Next_Rand(0.95, 0.21) < 0.5 ? 0.0 : 1.0;
				float rideshare_user = GLOBALS::Normal_RNG.Next_Rand(0.88, 0.32) < 0.5 ? 0.0 : 1.0;
				float alone_work = properties->Journey_To_Work_Vehicle_Occupancy<int>() == 1 ? 1.0 : 0.0;
				float alone_other = GLOBALS::Normal_RNG.Next_Rand(0.29, 0.45) < 0.5 ? 0.0 : 1.0;

				// estimate work distance
				float work_dist = 0.0;
				if (p->Work_Location<_Zone_Interface*>() != nullptr)
				{
					los_itf* los = skimmer->template Get_LOS<_Zone_Interface*, Time_Seconds, los_itf*>(home_zone, p->Work_Location<_Zone_Interface*>(), iteration());
					work_dist = los->auto_distance<Miles>();
				}
			
				// estimate annual vmt
				float age_u35 = age < 35.0 ? 1.0 : 0.0;
				float age_o65 = age >= 65.0 ? 1.0 : 0.0;
				float hh_taz_density = home_zone->area<Basic_Units::Area_Variables::Square_Miles>() > 0.0 ? home_zone->employment_total<float>() / home_zone->area<Basic_Units::Area_Variables::Square_Miles>() : 0.0;
				float veh_per_person = hh_properties->Number_of_vehicles<float>() / hh_properties->Household_size<float>();
				float inc_hi = inc >= 100000 ? 1.0 : 0.0;
				float inc_med = inc >= 50000 && inc < 100000 ? 1.0 : 0.0;
				float full_time_dist = properties->Is_Fulltime_Worker() ? work_dist : 0.0;
				float part_time_dist = properties->Is_Parttime_Worker() ? work_dist : 0.0;
				float ln_vmt = std::log(std::max<float>(4826.85 + 436.52 * male - 175.914 * age_u35 + 224.492 * age_o65 + 714.5569 * lic + 602.311 * full_time_dist + 533.078 * part_time_dist - 0.02384 * hh_taz_density + 915.7546 * veh_per_person + 1493.534 * inc_hi + 1152.284 * inc_med, 1000.0f));

				//------------------------------------------------------
				// Run Ordered Probit WTP model
				float U = _B_PAST_CRASHES * num_crash +
					_B_SMARTPHONE * smartphone_user +
					_B_CARSHARE * carshare_user +
					_B_RIDESHARE * rideshare_user +
					_B_DRIVE_ALONE_WORK * alone_work +
					_B_DRIVE_ALONE_OTHER * alone_other +
					_B_LN_VMT * ln_vmt +
					_B_WORK_DIST * work_dist +
					_B_GENDER * male +
					_B_LICENSE * lic +
					_B_NUM_CHILDREN * children +
					_B_AGE * age +
					_B_EMPL_DENSITY * density +
					_B_HHINCOME * inc +
					_C_CALIBRATION;

				float p1 = GLOBALS::Normal_Distribution->Cumulative_Distribution_Value<float>(_MU_1 - U);
				float p2 = GLOBALS::Normal_Distribution->Cumulative_Distribution_Value<float>(_MU_2 - U);
				float p3 = GLOBALS::Normal_Distribution->Cumulative_Distribution_Value<float>(_MU_3 - U);
				float r = GLOBALS::Uniform_RNG.Next_Rand<float>();
				float r2 = GLOBALS::Uniform_RNG.Next_Rand<float>();

				float WTP = 0.0;
				if (r < p1) WTP = r2 * 2000;					// WTP < $2000
				else if (r < p2) WTP = r2 * 3000 + 2000;		// WTP $2000 - $5000
				else if (r < p3) WTP = r2 * 5000 + 5000;		// WTP $5000 - $10000
				else WTP = r2 * 10000.0 + 10000.0;				// WTP > $10,000

				//if (work_dist > 0.0) cout << "WTP,U,VMT,WORK_DIST: " << WTP << ", " << U << ", " << exp(ln_vmt) << ", " << work_dist<<", From-to: "<< home_zone->uuid<int>()<<"-"<< p->Work_Location<_Zone_Interface*>()->uuid<int>()<<endl;

				// Finally, compare cost against willingness to pay and set the automation flag
				_Scenario_Interface* scenario = static_cast<_Scenario_Interface*>(_global_scenario);
				if (WTP > scenario->automation_cost<float>()) veh->is_autonomous(true);
				else veh->is_autonomous(false);
				veh->willingness_to_pay(WTP);
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

				//=============================================================
				// INITIALIZE THE VEHICLE TECHNOLOGY CHOICE MODEL PARAMETERS
				_Scenario_Interface* scenario = static_cast<_Scenario_Interface*>(_global_scenario);
				_B_PAST_CRASHES = scenario->vehicle_techchoice_beta_past_crashes<float>();
				_B_SMARTPHONE = scenario->vehicle_techchoice_beta_smartphone<float>();
				_B_CARSHARE = scenario->vehicle_techchoice_beta_carshare<float>();
				_B_RIDESHARE = scenario->vehicle_techchoice_beta_rideshare<float>();
				_B_DRIVE_ALONE_WORK = scenario->vehicle_techchoice_beta_drive_alone_work<float>();
				_B_DRIVE_ALONE_OTHER = scenario->vehicle_techchoice_beta_drive_alone_other<float>();
				_B_LN_VMT = scenario->vehicle_techchoice_beta_ln_vmt<float>();
				_B_WORK_DIST = scenario->vehicle_techchoice_beta_work_dist<float>();
				_B_GENDER = scenario->vehicle_techchoice_beta_gender<float>();
				_B_LICENSE = scenario->vehicle_techchoice_beta_license<float>();
				_B_NUM_CHILDREN = scenario->vehicle_techchoice_beta_num_children<float>();
				_B_AGE = scenario->vehicle_techchoice_beta_age<float>();
				_B_EMPL_DENSITY = scenario->vehicle_techchoice_beta_empl_density<float>();
				_B_HHINCOME = scenario->vehicle_techchoice_beta_hhincome<float>();
				_C_CALIBRATION = scenario->vehicle_techchoice_calibration<float>();
				_MU_1 = scenario->vehicle_techchoice_mu_1<float>();
				_MU_2 = scenario->vehicle_techchoice_mu_2<float>();
				_MU_3 = scenario->vehicle_techchoice_mu_3<float>();
			}

			// PARAMETER DECLARATIONS - initialize in the static initializer function using scenario parameters
			#pragma region static parameters
			static m_data(float, B_PAST_CRASHES, NONE, NONE);
			static m_data(float, B_SMARTPHONE, NONE, NONE);	
			static m_data(float, B_CARSHARE, NONE, NONE);
			static m_data(float, B_RIDESHARE, NONE, NONE);
			static m_data(float, B_DRIVE_ALONE_WORK, NONE, NONE);
			static m_data(float, B_DRIVE_ALONE_OTHER, NONE, NONE);
			static m_data(float, B_LN_VMT, NONE, NONE);
			static m_data(float, B_WORK_DIST, NONE, NONE);
			static m_data(float, B_GENDER, NONE, NONE);
			static m_data(float, B_LICENSE, NONE, NONE);
			static m_data(float, B_NUM_CHILDREN, NONE, NONE);
			static m_data(float, B_AGE, NONE, NONE);
			static m_data(float, B_EMPL_DENSITY, NONE, NONE);
			static m_data(float, B_HHINCOME, NONE, NONE);
			static m_data(float, C_CALIBRATION, NONE, NONE);
			static m_data(float, MU_1, NONE, NONE);
			static m_data(float, MU_2, NONE, NONE);
			static m_data(float, MU_3, NONE, NONE);
			#pragma endregion
		};
				
		template<typename MasterType, typename InheritanceList> typename Vehicle_Chooser_Implementation <MasterType, InheritanceList>::type_of(is_initialized) Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_is_initialized = false;		
		template<typename MasterType, typename InheritanceList> std::unordered_map<long long, vector<pair<Vehicle_Components::Prototypes::Vehicle_Characteristics<typename MasterType::vehicle_characteristics_type>*,float>>> Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_vehicle_distribution_container;		

		#pragma region Choice option parameters	
		template<typename MasterType, typename InheritanceList> float Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_B_PAST_CRASHES;
		template<typename MasterType, typename InheritanceList> float Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_B_SMARTPHONE;
		template<typename MasterType, typename InheritanceList> float Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_B_CARSHARE;
		template<typename MasterType, typename InheritanceList> float Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_B_RIDESHARE;
		template<typename MasterType, typename InheritanceList> float Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_B_DRIVE_ALONE_WORK;
		template<typename MasterType, typename InheritanceList> float Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_B_DRIVE_ALONE_OTHER;
		template<typename MasterType, typename InheritanceList> float Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_B_LN_VMT;
		template<typename MasterType, typename InheritanceList> float Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_B_WORK_DIST;
		template<typename MasterType, typename InheritanceList> float Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_B_GENDER;
		template<typename MasterType, typename InheritanceList> float Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_B_LICENSE;
		template<typename MasterType, typename InheritanceList> float Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_B_NUM_CHILDREN;
		template<typename MasterType, typename InheritanceList> float Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_B_AGE;
		template<typename MasterType, typename InheritanceList> float Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_B_EMPL_DENSITY;
		template<typename MasterType, typename InheritanceList> float Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_B_HHINCOME;
		template<typename MasterType, typename InheritanceList> float Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_C_CALIBRATION;
		template<typename MasterType, typename InheritanceList> float Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_MU_1;
		template<typename MasterType, typename InheritanceList> float Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_MU_2;
		template<typename MasterType, typename InheritanceList> float Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_MU_3;

	}
}


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
			typedef typename Polaris_Component<MasterType,INHERIT(Vehicle_Chooser_Implementation),Data_Object>::Component_Type ComponentType;

			// Pointer to the Parent class
			m_prototype(Household_Components::Prototypes::Household, typename MasterType::household_type, Parent_Household, NONE, NONE);
					
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
			static m_container(vehicle_distribution_container_type, vehicle_distribution_container, NONE, NONE);


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

			// Initializer for the CAV tech choice model
			static bool static_initialize(const string& option_file)
			{
				// set the default values
				default_static_initializer();

				//==================================================================================================================================
				// INITIALIZE THE VEHICLE TECHNOLOGY CHOICE MODEL PARAMETERS
				//----------------------------------------------------------------------------------------------------------------------------------

				// now see if there are config file changes
				rapidjson::Document document;
				if (option_file.length() < 1)
				{
					cout << "Warning: option file for Vehicle_Chooser_Implementation was not specified" << endl;
					return true;
				}

				_Scenario_Interface* scenario = static_cast<_Scenario_Interface*>(_global_scenario);
				if (!scenario->parse_option_file(document, option_file)) THROW_EXCEPTION("ERROR: unable to parse model file '" << option_file << "'.");

				// check that model is defined if it is requested through scenario
				if (!document.HasMember("CAV_WTP_Model")) THROW_EXCEPTION("ERROR: CAV_WTP_Model parameter not found in '" << option_file << "', but specified in scenarion.json.");

				string section = "CAV_WTP_Model";

				scenario->set_parameter(document, section, "B_PAST_CRASHES", _B_PAST_CRASHES);
				scenario->set_parameter(document, section, "B_SMARTPHONE", _B_SMARTPHONE);
				scenario->set_parameter(document, section, "B_CARSHARE", _B_CARSHARE);
				scenario->set_parameter(document, section, "B_RIDESHARE", _B_RIDESHARE);
				scenario->set_parameter(document, section, "B_DRIVE_ALONE_WORK", _B_DRIVE_ALONE_WORK);
				scenario->set_parameter(document, section, "B_DRIVE_ALONE_OTHER", _B_DRIVE_ALONE_OTHER);
				scenario->set_parameter(document, section, "B_LN_VMT", _B_LN_VMT);
				scenario->set_parameter(document, section, "B_WORK_DIST", _B_WORK_DIST);
				scenario->set_parameter(document, section, "B_GENDER", _B_GENDER);
				scenario->set_parameter(document, section, "B_LICENSE", _B_LICENSE);
				scenario->set_parameter(document, section, "B_NUM_CHILDREN", _B_NUM_CHILDREN);
				scenario->set_parameter(document, section, "B_AGE", _B_AGE);
				scenario->set_parameter(document, section, "B_EMPL_DENSITY", _B_EMPL_DENSITY);
				scenario->set_parameter(document, section, "B_HHINCOME", _B_HHINCOME);
				scenario->set_parameter(document, section, "C_CALIBRATION", _C_CALIBRATION);
				scenario->set_parameter(document, section, "MU_1", _MU_1);
				scenario->set_parameter(document, section, "MU_2", _MU_2);
				scenario->set_parameter(document, section, "MU_3", _MU_3);

				return true;
			}
			static void default_static_initializer()
			{
				_B_PAST_CRASHES= 0.309;
				_B_SMARTPHONE= 0.0;
				_B_CARSHARE= -1.149;
				_B_RIDESHARE= -1.4;
				_B_DRIVE_ALONE_WORK= 0.616;
				_B_DRIVE_ALONE_OTHER= 0.833;
				_B_LN_VMT= 0.329;
				_B_WORK_DIST= 0.087;
				_B_GENDER= 0.442;
				_B_LICENSE= -1.159;
				_B_NUM_CHILDREN= 0.341;
				_B_AGE= -0.039;
				_B_EMPL_DENSITY= -0.000337;
				_B_HHINCOME= 0.00000729;
				_C_CALIBRATION= -5.0;
				_MU_1= -7.401;
				_MU_2= -6.514;
				_MU_3= -5.503;
			}
			static void print_parameters()
			{
				cout << "Vehicle_Chooser_Implementation parameters" << endl;
				cout << "\tB_PAST_CRASHES = " << B_PAST_CRASHES<float>() << endl;
				cout << "\tB_SMARTPHONE = " << B_SMARTPHONE<float>() << endl;
				cout << "\tB_CARSHARE = " << B_CARSHARE<float>() << endl;
				cout << "\tB_RIDESHARE = " << B_RIDESHARE<float>() << endl;
				cout << "\tB_DRIVE_ALONE_WORK = " << B_DRIVE_ALONE_WORK<float>() << endl;
				cout << "\tB_DRIVE_ALONE_OTHER = " << B_DRIVE_ALONE_OTHER<float>() << endl;
				cout << "\tB_LN_VMT = " << B_LN_VMT<float>() << endl;
				cout << "\tB_WORK_DIST = " << B_WORK_DIST<float>() << endl;
				cout << "\tB_GENDER = " << B_GENDER<float>() << endl;
				cout << "\tB_LICENSE = " << B_LICENSE<float>() << endl;
				cout << "\tB_NUM_CHILDREN = " << B_NUM_CHILDREN<float>() << endl;
				cout << "\tB_AGE = " << B_AGE<float>() << endl;
				cout << "\tB_EMPL_DENSITY = " << B_EMPL_DENSITY<float>() << endl;
				cout << "\tB_HHINCOME = " << B_HHINCOME<float>() << endl;
				cout << "\tC_CALIBRATION = " << C_CALIBRATION<float>() << endl;
				cout << "\tMU_1 = " << MU_1<float>() << endl;
				cout << "\tMU_2 = " << MU_2<float>() << endl;
				cout << "\tMU_3 = " << MU_3<float>() << endl;
			}

			// PARAMETER DECLARATIONS - initialize in the static initializer function using scenario parameters
			static m_data(bool, is_initialized, NONE, NONE);
			#pragma region static parameters		
			m_static_data(float, B_PAST_CRASHES, NONE, NONE);
			m_static_data(float, B_SMARTPHONE, NONE, NONE);	
			m_static_data(float, B_CARSHARE, NONE, NONE);
			m_static_data(float, B_RIDESHARE, NONE, NONE);
			m_static_data(float, B_DRIVE_ALONE_WORK, NONE, NONE);
			m_static_data(float, B_DRIVE_ALONE_OTHER, NONE, NONE);
			m_static_data(float, B_LN_VMT, NONE, NONE);
			m_static_data(float, B_WORK_DIST, NONE, NONE);
			m_static_data(float, B_GENDER, NONE, NONE);
			m_static_data(float, B_LICENSE, NONE, NONE);
			m_static_data(float, B_NUM_CHILDREN, NONE, NONE);
			m_static_data(float, B_AGE, NONE, NONE);
			m_static_data(float, B_EMPL_DENSITY, NONE, NONE);
			m_static_data(float, B_HHINCOME, NONE, NONE);
			m_static_data(float, C_CALIBRATION, NONE, NONE);
			m_static_data(float, MU_1, NONE, NONE);
			m_static_data(float, MU_2, NONE, NONE);
			m_static_data(float, MU_3, NONE, NONE);
			#pragma endregion
		};
				
		template<typename MasterType, typename InheritanceList> typename Vehicle_Chooser_Implementation <MasterType, InheritanceList>::type_of(is_initialized) Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_is_initialized = false;		
		template<typename MasterType, typename InheritanceList> std::unordered_map<long long, vector<pair<Vehicle_Components::Prototypes::Vehicle_Characteristics<typename MasterType::vehicle_characteristics_type>*,float>>> Vehicle_Chooser_Implementation<MasterType, InheritanceList>::_vehicle_distribution_container;		

		#pragma region Choice option parameters	
		define_static_member_variable(Vehicle_Chooser_Implementation,B_PAST_CRASHES);
		define_static_member_variable(Vehicle_Chooser_Implementation,B_SMARTPHONE);
		define_static_member_variable(Vehicle_Chooser_Implementation,B_CARSHARE);
		define_static_member_variable(Vehicle_Chooser_Implementation,B_RIDESHARE);
		define_static_member_variable(Vehicle_Chooser_Implementation,B_DRIVE_ALONE_WORK);
		define_static_member_variable(Vehicle_Chooser_Implementation,B_DRIVE_ALONE_OTHER);
		define_static_member_variable(Vehicle_Chooser_Implementation,B_LN_VMT);
		define_static_member_variable(Vehicle_Chooser_Implementation,B_WORK_DIST);
		define_static_member_variable(Vehicle_Chooser_Implementation,B_GENDER);
		define_static_member_variable(Vehicle_Chooser_Implementation,B_LICENSE);
		define_static_member_variable(Vehicle_Chooser_Implementation,B_NUM_CHILDREN);
		define_static_member_variable(Vehicle_Chooser_Implementation,B_AGE);
		define_static_member_variable(Vehicle_Chooser_Implementation,B_EMPL_DENSITY);
		define_static_member_variable(Vehicle_Chooser_Implementation,B_HHINCOME);
		define_static_member_variable(Vehicle_Chooser_Implementation,C_CALIBRATION);
		define_static_member_variable(Vehicle_Chooser_Implementation,MU_1);
		define_static_member_variable(Vehicle_Chooser_Implementation,MU_2);
		define_static_member_variable(Vehicle_Chooser_Implementation,MU_3);
		#pragma endregion
	}
}


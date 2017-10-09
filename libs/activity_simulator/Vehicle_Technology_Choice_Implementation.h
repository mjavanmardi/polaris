#pragma once

#include "Vehicle_Technology_Choice_Prototype.h"

namespace Household_Components
{
	namespace Implementations
	{
		//==================================================================================
		// Selects the technologies assigned to the vehicle
		//
		// Implementation of Bansal et. al. (2016) - Transportation Research part C (67) - add automation technology based on calculated willingness to pay
		//----------------------------------------------------------------------------------
		implementation struct Vehicle_Technology_Chooser_Implementation : public Polaris_Component<MasterType, INHERIT(Vehicle_Technology_Chooser_Implementation), Data_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType, INHERIT(Vehicle_Technology_Chooser_Implementation), Data_Object>::Component_Type ComponentType;

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

			template <typename T> void Initialize(T household)
			{
				this->Parent_Household(household);
			}

			template<typename VehicleItfType, typename PersonItfType> void Select_Vehicle_Technology(VehicleItfType* veh, PersonItfType* person)
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

			// Static Initializer for the CAV tech choice model
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
				_B_PAST_CRASHES = 0.309;
				_B_SMARTPHONE = 0.0;
				_B_CARSHARE = -1.149;
				_B_RIDESHARE = -1.4;
				_B_DRIVE_ALONE_WORK = 0.616;
				_B_DRIVE_ALONE_OTHER = 0.833;
				_B_LN_VMT = 0.329;
				_B_WORK_DIST = 0.087;
				_B_GENDER = 0.442;
				_B_LICENSE = -1.159;
				_B_NUM_CHILDREN = 0.341;
				_B_AGE = -0.039;
				_B_EMPL_DENSITY = -0.000337;
				_B_HHINCOME = 0.00000729;
				_C_CALIBRATION = -5.0;
				_MU_1 = -7.401;
				_MU_2 = -6.514;
				_MU_3 = -5.503;
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
			#pragma region static parameters declaration		
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
		#pragma region static choice option parameter definitions
		define_static_member_variable(Vehicle_Technology_Chooser_Implementation, B_PAST_CRASHES);
		define_static_member_variable(Vehicle_Technology_Chooser_Implementation, B_SMARTPHONE);
		define_static_member_variable(Vehicle_Technology_Chooser_Implementation, B_CARSHARE);
		define_static_member_variable(Vehicle_Technology_Chooser_Implementation, B_RIDESHARE);
		define_static_member_variable(Vehicle_Technology_Chooser_Implementation, B_DRIVE_ALONE_WORK);
		define_static_member_variable(Vehicle_Technology_Chooser_Implementation, B_DRIVE_ALONE_OTHER);
		define_static_member_variable(Vehicle_Technology_Chooser_Implementation, B_LN_VMT);
		define_static_member_variable(Vehicle_Technology_Chooser_Implementation, B_WORK_DIST);
		define_static_member_variable(Vehicle_Technology_Chooser_Implementation, B_GENDER);
		define_static_member_variable(Vehicle_Technology_Chooser_Implementation, B_LICENSE);
		define_static_member_variable(Vehicle_Technology_Chooser_Implementation, B_NUM_CHILDREN);
		define_static_member_variable(Vehicle_Technology_Chooser_Implementation, B_AGE);
		define_static_member_variable(Vehicle_Technology_Chooser_Implementation, B_EMPL_DENSITY);
		define_static_member_variable(Vehicle_Technology_Chooser_Implementation, B_HHINCOME);
		define_static_member_variable(Vehicle_Technology_Chooser_Implementation, C_CALIBRATION);
		define_static_member_variable(Vehicle_Technology_Chooser_Implementation, MU_1);
		define_static_member_variable(Vehicle_Technology_Chooser_Implementation, MU_2);
		define_static_member_variable(Vehicle_Technology_Chooser_Implementation, MU_3);
		#pragma endregion


		//==================================================================================
		// Selects the technologies assigned to the vehicle
		//
		// Implementation of Bansal et. al. (2016) - Transportation Research part C (67) - add automation technology based on calculated willingness to pay
		//----------------------------------------------------------------------------------
		implementation struct UIC_Vehicle_Technology_Chooser_Implementation : public Polaris_Component<MasterType, INHERIT(UIC_Vehicle_Technology_Chooser_Implementation), Data_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType, INHERIT(UIC_Vehicle_Technology_Chooser_Implementation), Data_Object>::Component_Type ComponentType;


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

			template <typename T> void Initialize(T household)
			{
				this->Parent_Household(household);
			}

			template<typename VehicleItfType, typename PersonItfType> void Select_Vehicle_Technology(VehicleItfType* veh, PersonItfType* person)
			{
				using namespace Person_Components::Types;

				_Person_Interface* p = static_cast<_Person_Interface*>(person);
				_Static_Properties_Itf* properties = p->Static_Properties<_Static_Properties_Itf*>();
				household_static_properties_itf* hh_properties = _Parent_Household->Static_Properties<household_static_properties_itf*>();
				_Network_Interface* network = this->_Parent_Household->network_reference<_Network_Interface*>();
				_Zone_Interface* home_zone = _Parent_Household->Home_Location<_Zone_Interface*>();
				_Zone_Interface* work_zone = properties->Is_Employed<NT>() ? p->Work_Location<_Zone_Interface*>() : nullptr;
				_Skim_Interface* skimmer = network->skimming_faculty<_Skim_Interface*>();

				float work_dist = 0.0;
				if (work_zone != nullptr)
				{
					los_itf* los = skimmer->template Get_LOS<_Zone_Interface*, Time_Seconds, los_itf*>(home_zone, work_zone, iteration());
					work_dist = los->auto_distance<Miles>();
				}

				//------------------------------------------------
				// Get variables for input to OP choice model
				//------------------------------------------------
				// demographic and built environment
				float occ_admin = properties->Employment_Industry_Base<EMPLOYMENT_INDUSTRY_BASE>() == EMPLOYMENT_INDUSTRY_BASE::IND_ADMINISTRATION ? 1.0 : 0.0;
				float inc_hi = hh_properties->Income<Dollars>() >= 100000 ? 1.0 : 0.0;
				float no_telecommute = p->Properties<_Properties_Itf*>()->Telecommute_Frequency<TELECOMMUTE_FREQUENCY>() == TELECOMMUTE_FREQUENCY::TC_NEVER ? 1.0 : 0.0;
				float work_dist_hi = work_dist > 15.0 ? 1.0 : 0.0;
				float veh_avail = 0.0;
				if (hh_properties->Number_of_workers<int>() > 0) veh_avail = std::max<float>(hh_properties->Number_of_vehicles<float>() / hh_properties->Number_of_workers<int>(), 1.0);
				else veh_avail = std::max<float>(hh_properties->Number_of_vehicles<float>() / (hh_properties->Household_size<float>() - (float)_Parent_Household->Number_of_Children()), 1.0);
				float veh_access = GLOBALS::Uniform_RNG.Next_Rand<float>() < veh_avail ? 1.0 : 0.0;
				float long_distance = GLOBALS::Uniform_RNG.Next_Rand<float>() < 0.45 ? 1.0 : 0.0;
				float age_over36 = properties->Age<int>() > 36.0 ? 1.0 : 0.0;	
				float maj_accident = GLOBALS::Uniform_RNG.Next_Rand<float>() < 0.0893 * log(properties->Age<float>()) - 0.1112 ? 1.0 : 0.0;
				float emp_density_hi = 0.0;
				if (work_zone != nullptr) { if (work_zone->employment_total<float>() / work_zone->area<Square_Miles>() > 5000.0) { emp_density_hi = 1.0; } }
				float transit_access = home_zone->min_ovtt_transit<Time_Minutes>() < 10.0 ? 1.0 : 0.0;
				// estimate annual vmt
				float age_u35 = properties->Age<float>() < 35.0 ? 1.0 : 0.0;
				float age_o65 = properties->Age<float>() >= 65.0 ? 1.0 : 0.0;
				float hh_taz_density = home_zone->area<Basic_Units::Area_Variables::Square_Miles>() > 0.0 ? home_zone->employment_total<float>() / home_zone->area<Basic_Units::Area_Variables::Square_Miles>() : 0.0;
				float veh_per_person = hh_properties->Number_of_vehicles<float>() / hh_properties->Household_size<float>();			
				float inc_med = hh_properties->Income<Dollars>() >= 50000 && hh_properties->Income<Dollars>() < 100000 ? 1.0 : 0.0;
				float full_time_dist = properties->Is_Fulltime_Worker() ? work_dist : 0.0;
				float part_time_dist = properties->Is_Parttime_Worker() ? work_dist : 0.0;
				float ln_vmt = std::log(std::max<float>(4826.85 + 436.52 * float(properties->Gender<GENDER>()==MALE) - 175.914 * age_u35 + 224.492 * age_o65 + 714.5569 * 1.0 + 602.311 * full_time_dist + 533.078 * part_time_dist - 0.02384 * hh_taz_density + 915.7546 * veh_per_person + 1493.534 * inc_hi + 1152.284 * inc_med, 1000.0f));
				float av_to_work = GLOBALS::Uniform_RNG.Next_Rand<float>() < 0.3 ? 1.0 : 0.0;

				// Expectations about AV
				float fuel_efficiency = GLOBALS::Uniform_RNG.Next_Rand<float>() < _EXPECT_FUEL_EFFICIENCY_AVG ? 1.0 : 0.0;
				float less_stressful = GLOBALS::Uniform_RNG.Next_Rand<float>() < _EXPECT_LESS_STRESSFUL_AVG ? 1.0 : 0.0;
				float increase_safety = GLOBALS::Uniform_RNG.Next_Rand<float>() < _EXPECT_INCREASED_SAFETY_AVG ? 1.0 : 0.0;
				float imperfect_performance = GLOBALS::Uniform_RNG.Next_Rand<float>() < _EXPECT_IMPERFECT_PERFORMANCE_AVG ? 1.0 : 0.0;
				float control_failure = GLOBALS::Uniform_RNG.Next_Rand<float>() < _EXPECT_CONTROL_FAILURE_AVG ? 1.0 : 0.0;
				float high_price = GLOBALS::Uniform_RNG.Next_Rand<float>() < _EXPECT_HIGH_PRICE_AVG ? 1.0 : 0.0;
				float privacy_breach = GLOBALS::Uniform_RNG.Next_Rand<float>() < _EXPECT_PRIVACY_BREACH ? 1.0 : 0.0;

				// factors
				float factor_advanced_mobility = GLOBALS::Normal_RNG.Next_Rand(_FACTOR_ADVANCED_MOBILITY_AVG, _FACTOR_ADVANCED_MOBILITY_STD);
				float factor_drive_thrill = GLOBALS::Normal_RNG.Next_Rand(_FACTOR_DRIVE_THRILL_AVG, _FACTOR_DRIVE_THRILL_STD);
				float factor_transit = GLOBALS::Normal_RNG.Next_Rand(_FACTOR_TRANSIT_AVG, _FACTOR_TRANSIT_STD);
				float factor_environment = GLOBALS::Normal_RNG.Next_Rand(_FACTOR_ENVIRONMENT_AVG, _FACTOR_ENVIRONMENT_STD);


				//------------------------------------------------------
				// Run Ordered Probit WTP model
				
				float U=GLOBALS::Normal_RNG.Next_Rand(_CONSTANT, _STD_CONSTANT) +
						GLOBALS::Normal_RNG.Next_Rand(_B_JOB_ADMIN, _STD_JOB_ADMIN) * occ_admin +
						GLOBALS::Normal_RNG.Next_Rand(_B_HAS_VEHICLE, _STD_HAS_VEHICLE) * veh_access +
						GLOBALS::Normal_RNG.Next_Rand(_B_LONG_DISTANCE_TRIPS, _STD_LONG_DISTANCE_TRIPS) * long_distance +
						GLOBALS::Normal_RNG.Next_Rand(_B_HAD_MAJOR_ACCIDENT, _STD_MAJOR_ACCIDENT) * maj_accident +
						GLOBALS::Normal_RNG.Next_Rand(_B_NO_TELECOMMUTE, _STD_NO_TELECOMMUTE) * no_telecommute +
						GLOBALS::Normal_RNG.Next_Rand(_B_FUEL_EFFICIENCY_EXP_IMP, _STD_FUEL_EFFICIENCY_EXP_IMP) * fuel_efficiency +
						GLOBALS::Normal_RNG.Next_Rand(_B_LESS_STRESSFUL_EXP, _STD_LESS_STRESSFUL_EXP) * less_stressful +
						_B_IMPERFECT_PERFORMANCE_EXP_IMP * imperfect_performance + 
						GLOBALS::Normal_RNG.Next_Rand(_B_HIGH_PRICE_EXP_IMP, _STD_HIGH_PRICE_EXP_IMP) * high_price +
						_B_TRANSIT_ACCESSIBLE * transit_access +
						GLOBALS::Normal_RNG.Next_Rand(_B_LN_VMT, _STD_LN_VMT) * ln_vmt +
						GLOBALS::Normal_RNG.Next_Rand(_B_WANTS_AV_TO_WORK, _STD_WANTS_AV_TO_WORK) * av_to_work +
						GLOBALS::Normal_RNG.Next_Rand(_B_FACTOR_ADVANCED_MOBILITY, _STD_FACTOR_ADVANCED_MOBILITY) * factor_advanced_mobility +
						GLOBALS::Normal_RNG.Next_Rand(_B_FACTOR_ENJOY_DRIVE, _STD_FACTOR_ENJOY_DRIVE) * factor_drive_thrill +
						GLOBALS::Normal_RNG.Next_Rand(_B_FACTOR_PUBLIC_TRANSIT, _STD_FACTOR_PUBLIC_TRANSIT) * factor_transit +
						GLOBALS::Normal_RNG.Next_Rand(_B_FACTOR_ENVIRONMENTAL, _STD_FACTOR_ENVIRONMENT) * factor_environment
					;

				float t0 = 0.0;
				float t1 = t0 + exp(GLOBALS::Normal_RNG.Next_Rand(_MU_1, _MU_1_STD) + _T_INCOME_HIGH * inc_hi + _T_INCREASE_SAFETY_EXP_IMP * increase_safety);
				float t2 = t1 + exp(GLOBALS::Normal_RNG.Next_Rand(_MU_2, _MU_2_STD) + _T_INCOME_HIGH * inc_hi + _T_INCREASE_SAFETY_EXP_IMP * increase_safety);
				float t3 = t2 + exp(GLOBALS::Normal_RNG.Next_Rand(_MU_3, _MU_3_STD) + _T_INCOME_HIGH * inc_hi + _T_INCREASE_SAFETY_EXP_IMP * increase_safety);
				float t4 = t3 + exp(GLOBALS::Normal_RNG.Next_Rand(_MU_4, _MU_4_STD) + _T_INCOME_HIGH * inc_hi + _T_INCREASE_SAFETY_EXP_IMP * increase_safety);

				float p0 = GLOBALS::Normal_Distribution->Cumulative_Distribution_Value<float>(t0 - U);
				float p1 = GLOBALS::Normal_Distribution->Cumulative_Distribution_Value<float>(t1 - U);
				float p2 = GLOBALS::Normal_Distribution->Cumulative_Distribution_Value<float>(t2 - U);
				float p3 = GLOBALS::Normal_Distribution->Cumulative_Distribution_Value<float>(t3 - U);
				float p4 = GLOBALS::Normal_Distribution->Cumulative_Distribution_Value<float>(t4 - U);

				float r = GLOBALS::Uniform_RNG.Next_Rand<float>();
				float r2 = GLOBALS::Uniform_RNG.Next_Rand<float>();

				float WTP = 0.0;
				if (r < p0) WTP = 0;							// WTP = $0
				else if (r < p1) WTP = r2 * 1000;				// WTP < $1000
				else if (r < p2) WTP = r2 * 1500 + 1000;		// WTP $1000 - $2500
				else if (r < p3) WTP = r2 * 2500 + 2500;		// WTP $2500 - $5000
				else if (r < p4) WTP = r2 * 2500 + 5000;		// WTP $5000 - $75000
				else WTP = r2 * 7500.0 + 7500.0;				// WTP > $7,500

																//if (work_dist > 0.0) cout << "WTP,U,VMT,WORK_DIST: " << WTP << ", " << U << ", " << exp(ln_vmt) << ", " << work_dist<<", From-to: "<< home_zone->uuid<int>()<<"-"<< p->Work_Location<_Zone_Interface*>()->uuid<int>()<<endl;

				// Finally, compare cost against willingness to pay and set the automation flag
				_Scenario_Interface* scenario = static_cast<_Scenario_Interface*>(_global_scenario);
				if (WTP > scenario->automation_cost<float>()) veh->is_autonomous(true);
				else veh->is_autonomous(false);
				veh->willingness_to_pay(WTP);
			}

			// Static Initializer for the CAV tech choice model
			static bool static_initialize(const string& option_file)
			{
				// set the default values
				default_static_initializer();

				//==================================================================================================================================
				// INITIALIZE THE VEHICLE TECHNOLOGY CHOICE MODEL PARAMETERS
				//----------------------------------------------------------------------------------------------------------------------------------

				// now see if there are config file changes
				Scenario_Components::Types::ScenarioData document;
				if (option_file.length() < 1)
				{
					cout << "Warning: option file for Vehicle_Technology_Chooser_Implementation was not specified" << endl;
					return true;
				}

				_Scenario_Interface* scenario = static_cast<_Scenario_Interface*>(_global_scenario);
				if (!scenario->parse_option_file(document, option_file)) THROW_EXCEPTION("ERROR: unable to parse model file '" << option_file << "'.");

				scenario->set_parameter<float>(document, "CONSTANT", _CONSTANT);
				scenario->set_parameter<float>(document, "STD_CONSTANT", _STD_CONSTANT);
				scenario->set_parameter<float>(document, "B_JOB_ADMIN", _B_JOB_ADMIN);
				scenario->set_parameter<float>(document, "STD_JOB_ADMIN", _STD_JOB_ADMIN);
				scenario->set_parameter<float>(document, "T_INCOME_HIGH", _T_INCOME_HIGH);
				scenario->set_parameter<float>(document, "B_HAS_VEHICLE", _B_HAS_VEHICLE);
				scenario->set_parameter<float>(document, "STD_HAS_VEHICLE", _STD_HAS_VEHICLE);
				scenario->set_parameter<float>(document, "B_LONG_DISTANCE_TRIPS", _B_LONG_DISTANCE_TRIPS);
				scenario->set_parameter<float>(document, "STD_LONG_DISTANCE_TRIPS", _STD_LONG_DISTANCE_TRIPS);
				scenario->set_parameter<float>(document, "B_HAD_MAJOR_ACCIDENT", _B_HAD_MAJOR_ACCIDENT);
				scenario->set_parameter<float>(document, "STD_MAJOR_ACCIDENT", _STD_MAJOR_ACCIDENT);
				scenario->set_parameter<float>(document, "B_NO_TELECOMMUTE", _B_NO_TELECOMMUTE);
				scenario->set_parameter<float>(document, "STD_NO_TELECOMMUTE", _STD_NO_TELECOMMUTE);
				scenario->set_parameter<float>(document, "B_FUEL_EFFICIENCY_EXP_IMP", _B_FUEL_EFFICIENCY_EXP_IMP);
				scenario->set_parameter<float>(document, "STD_FUEL_EFFICIENCY_EXP_IMP", _STD_FUEL_EFFICIENCY_EXP_IMP);
				scenario->set_parameter<float>(document, "B_LESS_STRESSFUL_EXP", _B_LESS_STRESSFUL_EXP);
				scenario->set_parameter<float>(document, "STD_LESS_STRESSFUL_EXP", _STD_LESS_STRESSFUL_EXP);
				scenario->set_parameter<float>(document, "B_IMPERFECT_PERFORMANCE_EXP_IMP", _B_IMPERFECT_PERFORMANCE_EXP_IMP);
				scenario->set_parameter<float>(document, "T_INCREASE_SAFETY_EXP_IMP", _T_INCREASE_SAFETY_EXP_IMP);
				scenario->set_parameter<float>(document, "B_HIGH_PRICE_EXP_IMP", _B_HIGH_PRICE_EXP_IMP);
				scenario->set_parameter<float>(document, "STD_HIGH_PRICE_EXP_IMP", _STD_HIGH_PRICE_EXP_IMP);
				scenario->set_parameter<float>(document, "B_TRANSIT_ACCESSIBLE", _B_TRANSIT_ACCESSIBLE);
				scenario->set_parameter<float>(document, "B_LN_VMT", _B_LN_VMT);
				scenario->set_parameter<float>(document, "STD_LN_VMT", _STD_LN_VMT);
				scenario->set_parameter<float>(document, "B_WANTS_AV_TO_WORK", _B_WANTS_AV_TO_WORK);
				scenario->set_parameter<float>(document, "STD_WANTS_AV_TO_WORK", _STD_WANTS_AV_TO_WORK);
				scenario->set_parameter<float>(document, "B_FACTOR_ADVANCED_MOBILITY", _B_FACTOR_ADVANCED_MOBILITY);
				scenario->set_parameter<float>(document, "STD_FACTOR_ADVANCED_MOBILITY", _STD_FACTOR_ADVANCED_MOBILITY);
				scenario->set_parameter<float>(document, "B_FACTOR_ENJOY_DRIVE", _B_FACTOR_ENJOY_DRIVE);
				scenario->set_parameter<float>(document, "STD_FACTOR_ENJOY_DRIVE", _STD_FACTOR_ENJOY_DRIVE);
				scenario->set_parameter<float>(document, "B_FACTOR_PUBLIC_TRANSIT", _B_FACTOR_PUBLIC_TRANSIT);
				scenario->set_parameter<float>(document, "STD_FACTOR_PUBLIC_TRANSIT", _STD_FACTOR_PUBLIC_TRANSIT);
				scenario->set_parameter<float>(document, "B_FACTOR_ENVIRONMENTAL", _B_FACTOR_ENVIRONMENTAL);
				scenario->set_parameter<float>(document, "STD_FACTOR_ENVIRONMENT", _STD_FACTOR_ENVIRONMENT);
				scenario->set_parameter<float>(document, "MU_1", _MU_1);
				scenario->set_parameter<float>(document, "MU_1_STD", _MU_1_STD);
				scenario->set_parameter<float>(document, "MU_2", _MU_2);
				scenario->set_parameter<float>(document, "MU_2_STD", _MU_2_STD);
				scenario->set_parameter<float>(document, "MU_3", _MU_3);
				scenario->set_parameter<float>(document, "MU_3_STD", _MU_3_STD);
				scenario->set_parameter<float>(document, "MU_4", _MU_4);
				scenario->set_parameter<float>(document, "MU_4_STD", _MU_4_STD);
				scenario->set_parameter<float>(document, "EXPECT_FUEL_EFFICIENCY_AVG", _EXPECT_FUEL_EFFICIENCY_AVG);
				scenario->set_parameter<float>(document, "EXPECT_LESS_STRESSFUL_AVG", _EXPECT_LESS_STRESSFUL_AVG);
				scenario->set_parameter<float>(document, "EXPECT_INCREASED_SAFETY_AVG", _EXPECT_INCREASED_SAFETY_AVG);
				scenario->set_parameter<float>(document, "EXPECT_IMPERFECT_PERFORMANCE_AVG", _EXPECT_IMPERFECT_PERFORMANCE_AVG);
				scenario->set_parameter<float>(document, "EXPECT_CONTROL_FAILURE_AVG", _EXPECT_CONTROL_FAILURE_AVG);
				scenario->set_parameter<float>(document, "EXPECT_HIGH_PRICE_AVG", _EXPECT_HIGH_PRICE_AVG);
				scenario->set_parameter<float>(document, "EXPECT_PRIVACY_BREACH", _EXPECT_PRIVACY_BREACH);
				scenario->set_parameter<float>(document, "FACTOR_ADVANCED_MOBILITY_AVG", _FACTOR_ADVANCED_MOBILITY_AVG);
				scenario->set_parameter<float>(document, "FACTOR_DRIVE_THRILL_AVG", _FACTOR_DRIVE_THRILL_AVG);
				scenario->set_parameter<float>(document, "FACTOR_TRANSIT_AVG", _FACTOR_TRANSIT_AVG);
				scenario->set_parameter<float>(document, "FACTOR_ENVIRONMENT_AVG", _FACTOR_ENVIRONMENT_AVG);
				scenario->set_parameter<float>(document, "FACTOR_ADVANCED_MOBILITY_STD", _FACTOR_ADVANCED_MOBILITY_STD);
				scenario->set_parameter<float>(document, "FACTOR_DRIVE_THRILL_STD", _FACTOR_DRIVE_THRILL_STD);
				scenario->set_parameter<float>(document, "FACTOR_TRANSIT_STD", _FACTOR_TRANSIT_STD);
				scenario->set_parameter<float>(document, "FACTOR_ENVIRONMENT_STD", _FACTOR_ENVIRONMENT_STD);


				return true;
			}
			static void default_static_initializer()
			{
				_CONSTANT = 2.3;
				_STD_CONSTANT = 1.5;
				_B_JOB_ADMIN = -0.64;
				_STD_JOB_ADMIN = 0.8;		
				_B_HAS_VEHICLE = -1.86;
				_STD_HAS_VEHICLE = 3.75;
				_B_LONG_DISTANCE_TRIPS = 0.43;
				_STD_LONG_DISTANCE_TRIPS = 1.4;
				_B_HAD_MAJOR_ACCIDENT = 1.2;
				_STD_MAJOR_ACCIDENT = 1.9;
				_B_NO_TELECOMMUTE = -0.96;
				_STD_NO_TELECOMMUTE = 0.47;
				_B_FUEL_EFFICIENCY_EXP_IMP = -1.78;
				_STD_FUEL_EFFICIENCY_EXP_IMP = 1.45;
				_B_LESS_STRESSFUL_EXP = 0.85;
				_STD_LESS_STRESSFUL_EXP = 0.9;
				_B_IMPERFECT_PERFORMANCE_EXP_IMP = 1.97;		
				_B_HIGH_PRICE_EXP_IMP = 1.51;
				_STD_HIGH_PRICE_EXP_IMP = 2.33;
				_B_TRANSIT_ACCESSIBLE = -0.18;
				_B_LN_VMT = 0.19;
				_STD_LN_VMT = 0.06;
				_B_WANTS_AV_TO_WORK = 1.65;
				_STD_WANTS_AV_TO_WORK = 1.25;
				_B_FACTOR_ADVANCED_MOBILITY = 1.78;
				_STD_FACTOR_ADVANCED_MOBILITY = 1.31;
				_B_FACTOR_ENJOY_DRIVE = -0.21;
				_STD_FACTOR_ENJOY_DRIVE = 0.43;
				_B_FACTOR_PUBLIC_TRANSIT = 0.11;
				_STD_FACTOR_PUBLIC_TRANSIT = 0.43;
				_B_FACTOR_ENVIRONMENTAL = 0.28;
				_STD_FACTOR_ENVIRONMENT = 0.41;

				_MU_1 = 0.37;
				_MU_1_STD = 1.16;
				_MU_2 = 0.54;
				_MU_2_STD = 1.55;
				_MU_3 = 0.19;
				_MU_3_STD = 0.74;
				_MU_4 = 1.36;
				_MU_4_STD = 0.31;
				_T_INCOME_HIGH = -0.45;
				_T_INCREASE_SAFETY_EXP_IMP = -0.15;

				_EXPECT_FUEL_EFFICIENCY_AVG = 0.35;
				_EXPECT_LESS_STRESSFUL_AVG = 0.45;
				_EXPECT_INCREASED_SAFETY_AVG = 0.09;
				_EXPECT_IMPERFECT_PERFORMANCE_AVG = 0.07;
				_EXPECT_CONTROL_FAILURE_AVG = 0.07;
				_EXPECT_HIGH_PRICE_AVG = 0.2;
				_EXPECT_PRIVACY_BREACH = 0.63;

				_FACTOR_ADVANCED_MOBILITY_AVG = 0.03;
				_FACTOR_DRIVE_THRILL_AVG = -0.1;
				_FACTOR_TRANSIT_AVG = 0.29;
				_FACTOR_ENVIRONMENT_AVG = -0.2;
				_FACTOR_ADVANCED_MOBILITY_STD = 1.7;
				_FACTOR_DRIVE_THRILL_STD = 0.86;
				_FACTOR_TRANSIT_STD = 1.65;
				_FACTOR_ENVIRONMENT_STD = 2.23;


			}
			static void print_parameters()
			{
				cout << "\tCONSTANT = " << CONSTANT<float>() << endl;
				cout << "\tSTD_CONSTANT = " << STD_CONSTANT<float>() << endl;
				cout << "\tB_JOB_ADMIN = " << B_JOB_ADMIN<float>() << endl;
				cout << "\tSTD_JOB_ADMIN = " << STD_JOB_ADMIN<float>() << endl;		
				cout << "\tB_HAS_VEHICLE = " << B_HAS_VEHICLE<float>() << endl;
				cout << "\tSTD_HAS_VEHICLE = " << STD_HAS_VEHICLE<float>() << endl;
				cout << "\tB_LONG_DISTANCE_TRIPS = " << B_LONG_DISTANCE_TRIPS<float>() << endl;
				cout << "\tSTD_LONG_DISTANCE_TRIPS = " << STD_LONG_DISTANCE_TRIPS<float>() << endl;
				cout << "\tB_HAD_MAJOR_ACCIDENT = " << B_HAD_MAJOR_ACCIDENT<float>() << endl;
				cout << "\tSTD_MAJOR_ACCIDENT = " << STD_MAJOR_ACCIDENT<float>() << endl;
				cout << "\tB_NO_TELECOMMUTE = " << B_NO_TELECOMMUTE<float>() << endl;
				cout << "\tSTD_NO_TELECOMMUTE = " << STD_NO_TELECOMMUTE<float>() << endl;
				cout << "\tB_FUEL_EFFICIENCY_EXP_IMP = " << B_FUEL_EFFICIENCY_EXP_IMP<float>() << endl;
				cout << "\tSTD_FUEL_EFFICIENCY_EXP_IMP = " << STD_FUEL_EFFICIENCY_EXP_IMP<float>() << endl;
				cout << "\tB_LESS_STRESSFUL_EXP = " << B_LESS_STRESSFUL_EXP<float>() << endl;
				cout << "\tSTD_LESS_STRESSFUL_EXP = " << STD_LESS_STRESSFUL_EXP<float>() << endl;
				cout << "\tB_IMPERFECT_PERFORMANCE_EXP_IMP = " << B_IMPERFECT_PERFORMANCE_EXP_IMP<float>() << endl;			
				cout << "\tB_HIGH_PRICE_EXP_IMP = " << B_HIGH_PRICE_EXP_IMP<float>() << endl;
				cout << "\tSTD_HIGH_PRICE_EXP_IMP = " << STD_HIGH_PRICE_EXP_IMP<float>() << endl;
				cout << "\tB_TRANSIT_ACCESSIBLE = " << B_TRANSIT_ACCESSIBLE<float>() << endl;
				cout << "\tB_LN_VMT = " << B_LN_VMT<float>() << endl;
				cout << "\tSTD_LN_VMT = " << STD_LN_VMT<float>() << endl;
				cout << "\tB_WANTS_AV_TO_WORK = " << B_WANTS_AV_TO_WORK<float>() << endl;
				cout << "\tSTD_WANTS_AV_TO_WORK = " << STD_WANTS_AV_TO_WORK<float>() << endl;
				cout << "\tB_FACTOR_ADVANCED_MOBILITY = " << B_FACTOR_ADVANCED_MOBILITY<float>() << endl;
				cout << "\tSTD_FACTOR_ADVANCED_MOBILITY = " << STD_FACTOR_ADVANCED_MOBILITY<float>() << endl;
				cout << "\tB_FACTOR_ENJOY_DRIVE = " << B_FACTOR_ENJOY_DRIVE<float>() << endl;
				cout << "\tSTD_FACTOR_ENJOY_DRIVE = " << STD_FACTOR_ENJOY_DRIVE<float>() << endl;
				cout << "\tB_FACTOR_PUBLIC_TRANSIT = " << B_FACTOR_PUBLIC_TRANSIT<float>() << endl;
				cout << "\tSTD_FACTOR_PUBLIC_TRANSIT = " << STD_FACTOR_PUBLIC_TRANSIT<float>() << endl;
				cout << "\tB_FACTOR_ENVIRONMENTAL = " << B_FACTOR_ENVIRONMENTAL<float>() << endl;
				cout << "\tSTD_FACTOR_ENVIRONMENT = " << STD_FACTOR_ENVIRONMENT<float>() << endl;
				cout << "\tT_INCOME_HIGH = " << T_INCOME_HIGH<float>() << endl;
				cout << "\tT_INCREASE_SAFETY_EXP_IMP = " << T_INCREASE_SAFETY_EXP_IMP<float>() << endl;
				cout << "\tMU_1 = " << MU_1<float>() << endl;
				cout << "\tMU_1_STD = " << MU_1_STD<float>() << endl;
				cout << "\tMU_2 = " << MU_2<float>() << endl;
				cout << "\tMU_2_STD = " << MU_2_STD<float>() << endl;
				cout << "\tMU_3 = " << MU_3<float>() << endl;
				cout << "\tMU_3_STD = " << MU_3_STD<float>() << endl;
				cout << "\tMU_4 = " << MU_4<float>() << endl;
				cout << "\tMU_4_STD = " << MU_4_STD<float>() << endl;

			}

			// PARAMETER DECLARATIONS - initialize in the static initializer function using scenario parameters
			#pragma region static parameters declaration		
			// Ordered probit parameters
			m_static_data(float, CONSTANT, NONE, NONE);
			m_static_data(float, STD_CONSTANT, NONE, NONE);
			m_static_data(float, B_JOB_ADMIN, NONE, NONE);
			m_static_data(float, STD_JOB_ADMIN, NONE, NONE);
			m_static_data(float, B_HAS_VEHICLE, NONE, NONE);
			m_static_data(float, STD_HAS_VEHICLE, NONE, NONE);
			m_static_data(float, B_LONG_DISTANCE_TRIPS, NONE, NONE);
			m_static_data(float, STD_LONG_DISTANCE_TRIPS, NONE, NONE);
			m_static_data(float, B_HAD_MAJOR_ACCIDENT, NONE, NONE);
			m_static_data(float, STD_MAJOR_ACCIDENT, NONE, NONE);
			m_static_data(float, B_NO_TELECOMMUTE, NONE, NONE);
			m_static_data(float, STD_NO_TELECOMMUTE, NONE, NONE);
			m_static_data(float, B_FUEL_EFFICIENCY_EXP_IMP, NONE, NONE);
			m_static_data(float, STD_FUEL_EFFICIENCY_EXP_IMP, NONE, NONE);
			m_static_data(float, B_LESS_STRESSFUL_EXP, NONE, NONE);
			m_static_data(float, STD_LESS_STRESSFUL_EXP, NONE, NONE);
			m_static_data(float, B_IMPERFECT_PERFORMANCE_EXP_IMP, NONE, NONE);
			m_static_data(float, B_HIGH_PRICE_EXP_IMP, NONE, NONE);
			m_static_data(float, STD_HIGH_PRICE_EXP_IMP, NONE, NONE);
			m_static_data(float, B_TRANSIT_ACCESSIBLE, NONE, NONE);
			m_static_data(float, B_LN_VMT, NONE, NONE);
			m_static_data(float, STD_LN_VMT, NONE, NONE);
			m_static_data(float, B_WANTS_AV_TO_WORK, NONE, NONE);
			m_static_data(float, STD_WANTS_AV_TO_WORK, NONE, NONE);
			m_static_data(float, B_FACTOR_ADVANCED_MOBILITY, NONE, NONE);
			m_static_data(float, STD_FACTOR_ADVANCED_MOBILITY, NONE, NONE);
			m_static_data(float, B_FACTOR_ENJOY_DRIVE, NONE, NONE);
			m_static_data(float, STD_FACTOR_ENJOY_DRIVE, NONE, NONE);
			m_static_data(float, B_FACTOR_PUBLIC_TRANSIT, NONE, NONE);
			m_static_data(float, STD_FACTOR_PUBLIC_TRANSIT, NONE, NONE);
			m_static_data(float, B_FACTOR_ENVIRONMENTAL, NONE, NONE);
			m_static_data(float, STD_FACTOR_ENVIRONMENT, NONE, NONE);

			// Threshold parameters
			m_static_data(float, MU_1, NONE, NONE);
			m_static_data(float, MU_1_STD, NONE, NONE);
			m_static_data(float, MU_2, NONE, NONE);
			m_static_data(float, MU_2_STD, NONE, NONE);
			m_static_data(float, MU_3, NONE, NONE);
			m_static_data(float, MU_3_STD, NONE, NONE);
			m_static_data(float, MU_4, NONE, NONE);
			m_static_data(float, MU_4_STD, NONE, NONE);
			m_static_data(float, T_INCOME_HIGH, NONE, NONE);
			m_static_data(float, T_INCREASE_SAFETY_EXP_IMP, NONE, NONE);

			// Population variable averages
			m_static_data(float, EXPECT_FUEL_EFFICIENCY_AVG, NONE, NONE);
			m_static_data(float, EXPECT_LESS_STRESSFUL_AVG, NONE, NONE);
			m_static_data(float, EXPECT_INCREASED_SAFETY_AVG, NONE, NONE);
			m_static_data(float, EXPECT_IMPERFECT_PERFORMANCE_AVG, NONE, NONE);
			m_static_data(float, EXPECT_CONTROL_FAILURE_AVG, NONE, NONE);
			m_static_data(float, EXPECT_HIGH_PRICE_AVG, NONE, NONE);
			m_static_data(float, EXPECT_PRIVACY_BREACH, NONE, NONE);

			m_static_data(float, FACTOR_ADVANCED_MOBILITY_AVG, NONE, NONE);
			m_static_data(float, FACTOR_DRIVE_THRILL_AVG, NONE, NONE);
			m_static_data(float, FACTOR_TRANSIT_AVG, NONE, NONE);
			m_static_data(float, FACTOR_ENVIRONMENT_AVG, NONE, NONE);
			m_static_data(float, FACTOR_ADVANCED_MOBILITY_STD, NONE, NONE);
			m_static_data(float, FACTOR_DRIVE_THRILL_STD, NONE, NONE);
			m_static_data(float, FACTOR_TRANSIT_STD, NONE, NONE);
			m_static_data(float, FACTOR_ENVIRONMENT_STD, NONE, NONE);
			#pragma endregion
		};
		#pragma region static choice option parameter definitions
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, CONSTANT);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, STD_CONSTANT);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, B_JOB_ADMIN);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, STD_JOB_ADMIN);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, T_INCOME_HIGH);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, B_HAS_VEHICLE);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, STD_HAS_VEHICLE);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, B_LONG_DISTANCE_TRIPS);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, STD_LONG_DISTANCE_TRIPS);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, B_HAD_MAJOR_ACCIDENT);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, STD_MAJOR_ACCIDENT);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, B_NO_TELECOMMUTE);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, STD_NO_TELECOMMUTE);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, B_FUEL_EFFICIENCY_EXP_IMP);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, STD_FUEL_EFFICIENCY_EXP_IMP);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, B_LESS_STRESSFUL_EXP);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, STD_LESS_STRESSFUL_EXP);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, B_IMPERFECT_PERFORMANCE_EXP_IMP);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, T_INCREASE_SAFETY_EXP_IMP);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, B_HIGH_PRICE_EXP_IMP);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, STD_HIGH_PRICE_EXP_IMP);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, B_TRANSIT_ACCESSIBLE);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, B_LN_VMT);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, STD_LN_VMT);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, B_WANTS_AV_TO_WORK);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, STD_WANTS_AV_TO_WORK);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, B_FACTOR_ADVANCED_MOBILITY);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, STD_FACTOR_ADVANCED_MOBILITY);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, B_FACTOR_ENJOY_DRIVE);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, STD_FACTOR_ENJOY_DRIVE);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, B_FACTOR_PUBLIC_TRANSIT);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, STD_FACTOR_PUBLIC_TRANSIT);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, B_FACTOR_ENVIRONMENTAL);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, STD_FACTOR_ENVIRONMENT);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, MU_1);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, MU_1_STD);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, MU_2);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, MU_2_STD);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, MU_3);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, MU_3_STD);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, MU_4);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, MU_4_STD);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, EXPECT_FUEL_EFFICIENCY_AVG);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, EXPECT_LESS_STRESSFUL_AVG);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, EXPECT_INCREASED_SAFETY_AVG);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, EXPECT_IMPERFECT_PERFORMANCE_AVG);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, EXPECT_CONTROL_FAILURE_AVG);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, EXPECT_HIGH_PRICE_AVG);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, EXPECT_PRIVACY_BREACH);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, FACTOR_ADVANCED_MOBILITY_AVG);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, FACTOR_DRIVE_THRILL_AVG);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, FACTOR_TRANSIT_AVG);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, FACTOR_ENVIRONMENT_AVG);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, FACTOR_ADVANCED_MOBILITY_STD);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, FACTOR_DRIVE_THRILL_STD);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, FACTOR_TRANSIT_STD);
		define_static_member_variable(UIC_Vehicle_Technology_Chooser_Implementation, FACTOR_ENVIRONMENT_STD);

		#pragma endregion
	}
}



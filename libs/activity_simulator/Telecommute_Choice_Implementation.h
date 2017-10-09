#pragma once

#include "Telecommute_Chooser_Prototype.h"
#include "Person_Prototype.h"

namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Mode Choice Planning classes
		//----------------------------------------------------------------------------------

		implementation struct Telecommute_Choice_Implementation : public Polaris_Component<MasterType, INHERIT(Telecommute_Choice_Implementation), Data_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType, INHERIT(Telecommute_Choice_Implementation), Data_Object>::Component_Type ComponentType;

			// Pointer to the Parent class
			m_prototype(Prototypes::Person_Planner, typename MasterType::person_planner_type, Parent_Planner, NONE, NONE);

			static bool static_initialize(const string& option_file)
			{
				// set the base values
				default_static_initializer();

				// now see if there are config file changes
				Scenario_Components::Types::ScenarioData document;
				//std::string option_file = reinterpret_cast<_Scenario_Interface*>(_global_scenario)->template telecommute_choice_implementation_param_file<string>();
				if (option_file.length() < 1)
				{
					cout << "Warning: option file for Telecommute_Choice_Implementation was not specified" << endl;
					return true;
				}

				_Scenario_Interface* scenario = static_cast<_Scenario_Interface*>(_global_scenario);
				if (!scenario->parse_option_file(document, option_file))
					return false;

				// check that model is defined if it is requested through scenario
				if (!document.document.HasMember("Telecommute_Choice_Model")) THROW_EXCEPTION("ERROR: Telecommute_Choice_Model parameter not found in '" << option_file << "', but specified in scenarion.json.");


				string section = "Telecommute_Choice_Model";

				scenario->set_parameter<float>(document, "Z_CONSTANT"					, _Z_CONSTANT				 );
				scenario->set_parameter<float>(document, "Z_MALE"						, _Z_MALE					 );
				scenario->set_parameter<float>(document, "Z_INCOME_LOW"				, _Z_INCOME_LOW				 );
				scenario->set_parameter<float>(document, "Z_EDUC_NO_COLLEGE"			, _Z_EDUC_NO_COLLEGE		 );
				scenario->set_parameter<float>(document, "Z_WORK_TRIP_DIST"			, _Z_WORK_TRIP_DIST			 );
				scenario->set_parameter<float>(document, "Z_NWORKERS"					, _Z_NWORKERS				 );
				scenario->set_parameter<float>(document, "Z_FLEX_WORK_INDICATOR"		, _Z_FLEX_WORK_INDICATOR	 );
				scenario->set_parameter<float>(document, "Z_OCC_TRANS_UTILITY"		, _Z_OCC_TRANS_UTILITY		 );
				scenario->set_parameter<float>(document, "Z_OCC_MANAGEMENT"			, _Z_OCC_MANAGEMENT			 );
				scenario->set_parameter<float>(document, "Z_OCC_SERVICE"				, _Z_OCC_SERVICE			 );
				scenario->set_parameter<float>(document, "Z_EMP_DENSITY_OVER_20000"	, _Z_EMP_DENSITY_OVER_20000	 );
				scenario->set_parameter<float>(document, "Z_POP_DENSITY"				, _Z_POP_DENSITY			 );

				scenario->set_parameter<float>(document, "O_CONSTANT"					, _O_CONSTANT				 );
				scenario->set_parameter<float>(document, "O_INCOME_MED"				, _O_INCOME_MED				 );
				scenario->set_parameter<float>(document, "O_AGE_35_55"				, _O_AGE_35_55				 );
				scenario->set_parameter<float>(document, "O_EDUC_GRAD_DEGREE"			, _O_EDUC_GRAD_DEGREE		 );
				scenario->set_parameter<float>(document, "O_WORK_TRIP_TTIME"			, _O_WORK_TRIP_TTIME		 );
				scenario->set_parameter<float>(document, "O_NVEHICLE"					, _O_NVEHICLE				 );
				scenario->set_parameter<float>(document, "O_FLEX_WORK_INDICATOR"		, _O_FLEX_WORK_INDICATOR	 );
				scenario->set_parameter<float>(document, "O_OCC_GOVERNMENT"			, _O_OCC_GOVERNMENT			 );
				scenario->set_parameter<float>(document, "O_OCC_COMMUNICATION"		, _O_OCC_COMMUNICATION		 );
				scenario->set_parameter<float>(document, "O_OCC_MANUFACTURING"		, _O_OCC_MANUFACTURING		 );
				scenario->set_parameter<float>(document, "O_EMP_DENSITY_UNDER_3000"	, _O_EMP_DENSITY_UNDER_3000	 );

				scenario->set_parameter<float>(document, "T_WORK_DURATION"			, _T_WORK_DURATION			 );
				scenario->set_parameter<float>(document, "T_VEH_AVAILABLE"			, _T_VEH_AVAILABLE			 );
				scenario->set_parameter<float>(document, "T_HH_VEH_OVER_2_INDICATOR"	, _T_HH_VEH_OVER_2_INDICATOR );
				scenario->set_parameter<float>(document, "T_C1"						, _T_C1						 );
				scenario->set_parameter<float>(document, "T_C2"						, _T_C2						 );
				scenario->set_parameter<float>(document, "T_C3"						, _T_C3						 );
				scenario->set_parameter<float>(document, "T_RHO"						, _T_RHO					 );
				
				return true;
			}
			static void print_parameters()
			{
				cout << "Telecommute_Choice_Implementation parameters" << endl;
				cout << "\tZ_CONSTANT = " << Z_CONSTANT<float>() << endl;
				cout << "\tZ_MALE = " << Z_MALE<float>() << endl;
				cout << "\tZ_INCOME_LOW = " << Z_INCOME_LOW<float>() << endl;
				cout << "\tZ_EDUC_NO_COLLEGE = " << Z_EDUC_NO_COLLEGE<float>() << endl;
				cout << "\tZ_WORK_TRIP_DIST = " << Z_WORK_TRIP_DIST<float>() << endl;
				cout << "\tZ_NWORKERS = " << Z_NWORKERS<float>() << endl;
				cout << "\tZ_FLEX_WORK_INDICATOR = " << Z_FLEX_WORK_INDICATOR<float>() << endl;
				cout << "\tZ_OCC_TRANS_UTILITY = " << Z_OCC_TRANS_UTILITY<float>() << endl;
				cout << "\tZ_OCC_MANAGEMENT = " << Z_OCC_MANAGEMENT<float>() << endl;
				cout << "\tZ_OCC_SERVICE = " << Z_OCC_SERVICE<float>() << endl;
				cout << "\tZ_EMP_DENSITY_OVER_20000 = " << Z_EMP_DENSITY_OVER_20000<float>() << endl;
				cout << "\tZ_POP_DENSITY = " << Z_POP_DENSITY<float>() << endl;

				cout << "\tO_CONSTANT = " << O_CONSTANT<float>() << endl;
				cout << "\tO_INCOME_MED = " << O_INCOME_MED<float>() << endl;
				cout << "\tO_AGE_35_55 = " << O_AGE_35_55<float>() << endl;
				cout << "\tO_EDUC_GRAD_DEGREE = " << O_EDUC_GRAD_DEGREE<float>() << endl;
				cout << "\tO_WORK_TRIP_TTIME = " << O_WORK_TRIP_TTIME<float>() << endl;
				cout << "\tO_NVEHICLE = " << O_NVEHICLE<float>() << endl;
				cout << "\tO_FLEX_WORK_INDICATOR = " << O_FLEX_WORK_INDICATOR<float>() << endl;
				cout << "\tO_OCC_GOVERNMENT = " << O_OCC_GOVERNMENT<float>() << endl;
				cout << "\tO_OCC_COMMUNICATION = " << O_OCC_COMMUNICATION<float>() << endl;
				cout << "\tO_OCC_MANUFACTURING = " << O_OCC_MANUFACTURING<float>() << endl;
				cout << "\tO_EMP_DENSITY_UNDER_3000 = " << O_EMP_DENSITY_UNDER_3000<float>() << endl;

				cout << "\tT_WORK_DURATION = " << T_WORK_DURATION<float>() << endl;
				cout << "\tT_VEH_AVAILABLE = " << T_VEH_AVAILABLE<float>() << endl;
				cout << "\tT_HH_VEH_OVER_2_INDICATOR = " << T_HH_VEH_OVER_2_INDICATOR<float>() << endl;
				cout << "\tT_C1 = " << T_C1<float>() << endl;
				cout << "\tT_C2 = " << T_C2<float>() << endl;
				cout << "\tT_C3 = " << T_C3<float>() << endl;
				cout << "\tT_RHO = " << T_RHO<float>() << endl;
			}

			//==============================================================================================================================
			#pragma region parameter declarations
			// Zero-inflation paramaters
			m_static_data(float, Z_CONSTANT, NONE, NONE);
			m_static_data(float, Z_MALE, NONE, NONE);
			m_static_data(float, Z_INCOME_LOW, NONE, NONE);
			m_static_data(float, Z_EDUC_NO_COLLEGE, NONE, NONE);
			m_static_data(float, Z_WORK_TRIP_DIST, NONE, NONE);
			m_static_data(float, Z_NWORKERS, NONE, NONE);
			m_static_data(float, Z_FLEX_WORK_INDICATOR, NONE, NONE);
			m_static_data(float, Z_OCC_TRANS_UTILITY, NONE, NONE);
			m_static_data(float, Z_OCC_MANAGEMENT, NONE, NONE);
			m_static_data(float, Z_OCC_SERVICE, NONE, NONE);
			m_static_data(float, Z_EMP_DENSITY_OVER_20000, NONE, NONE);
			m_static_data(float, Z_POP_DENSITY, NONE, NONE);


			// Ordered probit model
			m_static_data(float, O_CONSTANT, NONE, NONE);
			m_static_data(float, O_INCOME_MED, NONE, NONE);
			m_static_data(float, O_AGE_35_55, NONE, NONE);
			m_static_data(float, O_EDUC_GRAD_DEGREE, NONE, NONE);
			m_static_data(float, O_WORK_TRIP_TTIME, NONE, NONE);
			m_static_data(float, O_NVEHICLE, NONE, NONE);
			m_static_data(float, O_FLEX_WORK_INDICATOR, NONE, NONE);
			m_static_data(float, O_OCC_GOVERNMENT, NONE, NONE);
			m_static_data(float, O_OCC_COMMUNICATION, NONE, NONE);
			m_static_data(float, O_OCC_MANUFACTURING, NONE, NONE);
			m_static_data(float, O_EMP_DENSITY_UNDER_3000, NONE, NONE);


			// threshold variables
			m_static_data(float, T_WORK_DURATION, NONE, NONE);				// Average work duration in hours
			m_static_data(float, T_VEH_AVAILABLE, NONE, NONE);				// vehicle is available for work
			m_static_data(float, T_HH_VEH_OVER_2_INDICATOR, NONE, NONE);	// HH has more than 2 vehicles
			m_static_data(float, T_C1, NONE, NONE);
			m_static_data(float, T_C2, NONE, NONE);
			m_static_data(float, T_C3, NONE, NONE);
			m_static_data(float, T_RHO, NONE, NONE);
			#pragma endregion

			static void default_static_initializer()
			{
				_Z_CONSTANT = -0.98;
				_Z_MALE = 0.26;
				_Z_INCOME_LOW = -0.64;
				_Z_EDUC_NO_COLLEGE = -0.67;
				_Z_WORK_TRIP_DIST = 0.22;
				_Z_NWORKERS = 0.18;
				_Z_FLEX_WORK_INDICATOR = 0.89;
				_Z_OCC_TRANS_UTILITY = -0.24;
				_Z_OCC_MANAGEMENT = -0.61;
				_Z_OCC_SERVICE = -0.38;
				_Z_EMP_DENSITY_OVER_20000 = 0.34;
				_Z_POP_DENSITY = -0.07;

				_O_CONSTANT = -0.62;
				_O_INCOME_MED = 0.49;
				_O_AGE_35_55 = 0.2;
				_O_EDUC_GRAD_DEGREE = 0.31;
				_O_WORK_TRIP_TTIME = 0.23;
				_O_NVEHICLE = 0.14;
				_O_FLEX_WORK_INDICATOR = 0.82;
				_O_OCC_GOVERNMENT = -0.23;
				_O_OCC_COMMUNICATION = 0.31;
				_O_OCC_MANUFACTURING = -0.16;
				_O_EMP_DENSITY_UNDER_3000 = -0.22;

				_T_WORK_DURATION = -0.03;
				_T_VEH_AVAILABLE = -0.11;
				_T_HH_VEH_OVER_2_INDICATOR = 0.26;
				_T_C1 = -1.6;
				_T_C2 = -0.47;
				_T_C3 = 0.43;
				_T_RHO = 0.28;
			}
			
			//===========================================================================================================================================
			// Interface definitions	
			#pragma region object-specific type-defs...
			typedef Prototypes::Person<typename type_of(Parent_Planner)::type_of(Parent_Person)> person_itf;
			typedef Prototypes::Person_Properties<typename person_itf::get_type_of(Static_Properties)> person_static_properties_itf;
			typedef Household_Components::Prototypes::Household<typename person_itf::get_type_of(Household)> household_itf;
			typedef Household_Components::Prototypes::Household_Properties<typename household_itf::get_type_of(Static_Properties)> household_static_properties_itf;
			typedef Prototypes::Person_Scheduler<typename person_itf::get_type_of(Scheduling_Faculty)> scheduler_itf;
			typedef Scenario_Components::Prototypes::Scenario< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference)> _Scenario_Interface;
			typedef Network_Components::Prototypes::Network< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference)> _Network_Interface;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _Network_Interface::get_type_of(skimming_faculty)> _Skim_Interface;
			typedef Network_Skimming_Components::Prototypes::LOS<typename MasterType::los_value_type> los_itf;
			typedef Network_Skimming_Components::Prototypes::LOS<typename MasterType::los_invariant_value_type> los_invariant_itf;
			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(_Activity_Locations_Container_Interface)>  _Activity_Location_Interface;
			typedef Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links)> _Links_Container_Interface;
			typedef Link_Components::Prototypes::Link<get_component_type(_Links_Container_Interface)>  _Link_Interface;
			typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_Zones_Container_Interface)>  _Zone_Interface;
			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(zone_ids_container), int> _Zone_Ids_Interface;
			#pragma endregion

			Person_Components::Types::TELECOMMUTE_FREQUENCY Telecommute_Choice()
			{
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
				scheduler_itf* scheduler = _Parent_Person->template Scheduling_Faculty<scheduler_itf*>();
				household_itf* _Parent_Household = _Parent_Person->person_itf::template Household<household_itf*>();
				household_static_properties_itf* household_properties = _Parent_Household->template Static_Properties<household_static_properties_itf*>();
				person_static_properties_itf* person_properties = _Parent_Person->Static_Properties<person_static_properties_itf*>();

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Scenario_Interface* scenario = _Parent_Person->scenario_reference<_Scenario_Interface*>();
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();
				_Skim_Interface* skimmer = network->skimming_faculty<_Skim_Interface*>();

				//==================================
				// create model variables
				//----------------------------------
				// estimate flex work schedule probability
				float flex_work = 0.0;
				if (GLOBALS::Uniform_RNG.Next_Rand<float>() < scenario->flexible_work_percentage<float>()) flex_work = 1.0;

				// estimate work distance
				_Zone_Interface* work_taz = _Parent_Person->Work_Location<_Zone_Interface*>();
				_Zone_Interface* home_taz = _Parent_Household->Home_Location<_Zone_Interface*>();
				float work_dist_indicator = 0.0;
				float work_ttime = 0.0;
				float emp_20000_indicator = 0.0;
				float emp_3000_indicator = 0.0;
				if (home_taz->employment_total<int>() > 20000) emp_20000_indicator = 1.0;
				if (home_taz->employment_total<int>() < 3000) emp_3000_indicator = 1.0;
				if (work_taz != nullptr)
				{
					los_itf* los = skimmer->template Get_LOS<_Zone_Interface*, Time_Seconds, los_itf*>(home_taz, work_taz, iteration());
					work_dist_indicator = los->auto_distance<Miles>() > 20.0 ? 1.0 : 0.0;
					work_ttime = los->auto_ttime<Time_Hours>();
				}
				float pop_density = home_taz->area<Square_Miles>() > 0.0 ? home_taz->pop_persons<float>() / home_taz->area<Square_Miles>() / 10000.0f : 0.0;
				// vehicle availability
				float veh_avail = 0;
				float nworker = household_properties->Number_of_workers<int>();
				if (nworker > 0) veh_avail = std::max<float>(household_properties->Number_of_vehicles<float>() / nworker, 1.0);
				// income < 50k
				float low_income = (household_properties->Income<Dollars>() < 50000.0) ? 1.0: 0.0;
				float high_income = (household_properties->Income<Dollars>() >= 100000.0) ? 1.0 : 0.0;
				// employment categories
				Person_Components::Types::EMPLOYMENT_INDUSTRY_BASE emp_industry = person_properties->Employment_Industry_Base<Person_Components::Types::EMPLOYMENT_INDUSTRY_BASE>();
				float occ_admin = emp_industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_BASE::IND_ADMINISTRATION ? 1.0: 0.0;
				float occ_finance = emp_industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_BASE::IND_FINANCE ? 1.0 : 0.0;
				float occ_education = emp_industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_BASE::IND_EDUCATION ? 1.0 : 0.0;
				float occ_realestate = emp_industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_BASE::IND_REALESTATE ? 1.0 : 0.0;
				float occ_communication = emp_industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_BASE::IND_INFORMATION ? 1.0 : 0.0;
				float occ_professional = emp_industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_BASE::IND_PROFESSIONAL ? 1.0 : 0.0;
				float occ_healthcare = emp_industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_BASE::IND_MEDICAL ? 1.0 : 0.0;
				float occ_trans = emp_industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_BASE::IND_TRANSPORTATION ? 1.0 : 0.0;
				float occ_utility = emp_industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_BASE::IND_UTILITIES ? 1.0 : 0.0;
				float occ_socialwork = emp_industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_BASE::IND_SOCIALWORK ? 1.0 : 0.0;
				float occ_manufacturing = emp_industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_BASE::IND_MANUFACTURING ? 1.0 : 0.0;
				float occ_government = 0.0;
				// education level
				Person_Components::Types::EDUCATION_LEVEL educ = person_properties->Educational_Attainment<Person_Components::Types::EDUCATION_LEVEL>();
				float educ_assoc = educ == Person_Components::Types::EDUCATION_LEVEL::EDUC_12 ? 1.0 : 0.0;
				float educ_undergrad = educ == Person_Components::Types::EDUCATION_LEVEL::EDUC_UNDERGRADUATE ? 1.0 : 0.0;
				float educ_grad = educ == Person_Components::Types::EDUCATION_LEVEL::EDUC_GRADUATE ? 1.0 : 0.0;
				float educ_assoc_hs = 1.0 - educ_undergrad - educ_grad;
				float male = person_properties->Gender<Person_Components::Types::GENDER>() == Person_Components::Types::MALE ? 1.0 : 0.0;
				float emp_full = person_properties->Is_Fulltime_Worker();
				float emp_part = person_properties->Is_Parttime_Worker();
				// other characteristics
				//float trip_time = person_properties->Journey_To_Work_Travel_Time<Time_Minutes>();
				float work_dur = person_properties->Work_Hours<Time_Hours>() / 5.0;
				float nveh = household_properties->Number_of_vehicles<float>();
				float age_35_55 = person_properties->Age<int>() >= 35 && person_properties->Age<int>() <= 55 ? 1.0 : 0.0;

				//=====================================
				//Telecommute option model
				//-------------------------------------
				float az =	_Z_CONSTANT + _Z_MALE*male + _Z_INCOME_LOW*low_income + _Z_EDUC_NO_COLLEGE*educ_assoc_hs + _Z_WORK_TRIP_DIST*work_dist_indicator + _Z_NWORKERS*nworker +
							_Z_FLEX_WORK_INDICATOR * flex_work + _Z_OCC_TRANS_UTILITY * (occ_trans + occ_utility) + _Z_OCC_MANAGEMENT*occ_admin + _Z_OCC_SERVICE*(occ_healthcare + occ_socialwork) +
							_Z_EMP_DENSITY_OVER_20000*emp_20000_indicator + _Z_POP_DENSITY*pop_density;
				float bx =	_O_CONSTANT + _O_INCOME_MED * (1.0 - low_income - high_income) + _O_AGE_35_55*age_35_55 + _O_EDUC_GRAD_DEGREE*educ_grad + _O_WORK_TRIP_TTIME*work_ttime + _O_NVEHICLE*nveh +
							_O_FLEX_WORK_INDICATOR*flex_work + _O_OCC_GOVERNMENT*occ_government + _O_OCC_COMMUNICATION*occ_communication + 
							_O_OCC_MANUFACTURING*occ_manufacturing*occ_manufacturing + _O_EMP_DENSITY_UNDER_3000*emp_3000_indicator;
				float gs = _T_WORK_DURATION * work_dur + _T_VEH_AVAILABLE * veh_avail + _T_HH_VEH_OVER_2_INDICATOR * (nveh > 2.0);

				// set threshold values
				float threshold[5];
				threshold[0] = 0.0;
				threshold[1] = threshold[0] + exp(_T_C1 + gs);
				threshold[2] = threshold[1] + exp(_T_C2 + gs);
				threshold[3] = threshold[2] + exp(_T_C3 + gs);
				threshold[4] = FLT_MAX;


				// probability of telecommute levels
				float prob[5];
				float n1 = GLOBALS::Normal_Distribution->Cumulative_Distribution_Value<float>(az);
				float n2 = GLOBALS::Normal_Distribution->Cumulative_Distribution_Value<float>(-bx);
				float n3 = GLOBALS::Bivariate_Normal_Distribution->Cumulative_Distribution_Value<float>(az, -bx, -_T_RHO);
				prob[0] = 1.0 - GLOBALS::Normal_Distribution->Cumulative_Distribution_Value<float>(az) + GLOBALS::Bivariate_Normal_Distribution->Cumulative_Distribution_Value<float>(az, -bx, -_T_RHO);
				for (int i = 1; i <= 4; i++) prob[i] = GLOBALS::Bivariate_Normal_Distribution->Cumulative_Distribution_Value<float>(az, threshold[i] - bx, -_T_RHO) - GLOBALS::Bivariate_Normal_Distribution->Cumulative_Distribution_Value<float>(az, threshold[i - 1] - bx, -_T_RHO);
				for (int i = 1; i <= 4; i++) prob[i] = prob[i] + prob[i - 1];


				// determine if telecommuting on travel day or not
				float r = GLOBALS::Uniform_RNG.Next_Rand<float>();
				if		(r < prob[0]) return Types::TC_NEVER;
				else if (r < prob[1]) return Types::TC_YEARLY; // does not telecommute or few times a year
				else if (r < prob[2]) return Types::TC_MONTHLY;// once a month or more
				else if (r < prob[3]) return Types::TC_WEEKLY; // once a week or more
				else				  return Types::TC_DAILY; // Every day

				
			}
		};

		// INITIALIZE TELECOMMUTE CHOICE MODEL STATIC PARAMETERS
		#pragma region Choice option parameters	
		// zero-inflation parameters
		define_static_member_variable(Telecommute_Choice_Implementation, Z_CONSTANT);
		define_static_member_variable(Telecommute_Choice_Implementation, Z_MALE);
		define_static_member_variable(Telecommute_Choice_Implementation, Z_INCOME_LOW);
		define_static_member_variable(Telecommute_Choice_Implementation, Z_EDUC_NO_COLLEGE);
		define_static_member_variable(Telecommute_Choice_Implementation, Z_WORK_TRIP_DIST);
		define_static_member_variable(Telecommute_Choice_Implementation, Z_NWORKERS);
		define_static_member_variable(Telecommute_Choice_Implementation, Z_FLEX_WORK_INDICATOR);
		define_static_member_variable(Telecommute_Choice_Implementation, Z_OCC_TRANS_UTILITY);
		define_static_member_variable(Telecommute_Choice_Implementation, Z_OCC_MANAGEMENT);
		define_static_member_variable(Telecommute_Choice_Implementation, Z_OCC_SERVICE);
		define_static_member_variable(Telecommute_Choice_Implementation, Z_EMP_DENSITY_OVER_20000);
		define_static_member_variable(Telecommute_Choice_Implementation, Z_POP_DENSITY);

		// ordered probit parameters
		define_static_member_variable(Telecommute_Choice_Implementation, O_CONSTANT);
		define_static_member_variable(Telecommute_Choice_Implementation, O_INCOME_MED);
		define_static_member_variable(Telecommute_Choice_Implementation, O_AGE_35_55);
		define_static_member_variable(Telecommute_Choice_Implementation, O_EDUC_GRAD_DEGREE);
		define_static_member_variable(Telecommute_Choice_Implementation, O_WORK_TRIP_TTIME);
		define_static_member_variable(Telecommute_Choice_Implementation, O_NVEHICLE);
		define_static_member_variable(Telecommute_Choice_Implementation, O_FLEX_WORK_INDICATOR);
		define_static_member_variable(Telecommute_Choice_Implementation, O_OCC_GOVERNMENT);
		define_static_member_variable(Telecommute_Choice_Implementation, O_OCC_COMMUNICATION);
		define_static_member_variable(Telecommute_Choice_Implementation, O_OCC_MANUFACTURING);
		define_static_member_variable(Telecommute_Choice_Implementation, O_EMP_DENSITY_UNDER_3000);

		// threshold parameters
		define_static_member_variable(Telecommute_Choice_Implementation, T_WORK_DURATION);
		define_static_member_variable(Telecommute_Choice_Implementation, T_VEH_AVAILABLE);
		define_static_member_variable(Telecommute_Choice_Implementation, T_HH_VEH_OVER_2_INDICATOR);
		define_static_member_variable(Telecommute_Choice_Implementation, T_C1);
		define_static_member_variable(Telecommute_Choice_Implementation, T_C2);
		define_static_member_variable(Telecommute_Choice_Implementation, T_C3);
		define_static_member_variable(Telecommute_Choice_Implementation, T_RHO);
		#pragma endregion
	}
}


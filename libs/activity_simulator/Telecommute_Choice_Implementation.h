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

			static double GetOption_Double(rapidjson::Document& document, const std::string& key, double default_value)
			{
				return reinterpret_cast<_Scenario_Interface*>(_global_scenario)->get_parameter(document, "Telecommute_Choice_Implementation", key, default_value);
			}

			static bool static_initialize()
			{
				rapidjson::Document document;
				std::string option_file = reinterpret_cast<_Scenario_Interface*>(_global_scenario)->template telecommute_choice_implementation_param_file<string>();
				if (option_file.length() < 1)
				{
					cout << "Warning: option file for Telecommute_Choice_Implementation was not specified" << endl;
					return true;
				}

				if (!reinterpret_cast<_Scenario_Interface*>(_global_scenario)->parse_option_file(document, option_file))
					return false;

				Z_CONST					<float>(GetOption_Double(document, "Z_CONST"			, Z_CONST				<float>()));
				Z_VEH_AVAIL				<float>(GetOption_Double(document, "Z_VEH_AVAIL"		, Z_VEH_AVAIL			<float>()));
				Z_HU_DENSITY			<float>(GetOption_Double(document, "Z_HU_DENSITY"		, Z_HU_DENSITY			<float>()));
				Z_OCC_INC				<float>(GetOption_Double(document, "Z_OCC_INC"			, Z_OCC_INC				<float>()));
				Z_OCC_FINANCE			<float>(GetOption_Double(document, "Z_OCC_FINANCE"		, Z_OCC_FINANCE			<float>()));
				Z_OCC_EDUCATION 		<float>(GetOption_Double(document, "Z_OCC_EDUCATION"	, Z_OCC_EDUCATION 		<float>()));
				Z_DEGREE_ASSOC 			<float>(GetOption_Double(document, "Z_DEGREE_ASSOC"		, Z_DEGREE_ASSOC 		<float>()));

				O_CONST					<float>(GetOption_Double(document, "O_CONST"			, O_CONST				<float>()));
				O_HINC 					<float>(GetOption_Double(document, "O_HINC"				, O_HINC 				<float>()));
				O_MALE 					<float>(GetOption_Double(document, "O_MALE"				, O_MALE 				<float>()));
				O_EMPFULL 				<float>(GetOption_Double(document, "O_EMPFULL"			, O_EMPFULL 			<float>()));
				O_DEGREE_GRAD			<float>(GetOption_Double(document, "O_DEGREE_GRAD"		, O_DEGREE_GRAD			<float>()));
				O_TTIME 				<float>(GetOption_Double(document, "O_TTIME"			, O_TTIME 				<float>()));
				O_WORKDUR 				<float>(GetOption_Double(document, "O_WORKDUR"			, O_WORKDUR 			<float>()));
				O_NVEH 					<float>(GetOption_Double(document, "O_NVEH"				, O_NVEH 				<float>()));
				O_OCC_REALESTATE		<float>(GetOption_Double(document, "O_OCC_REALESTATE"	, O_OCC_REALESTATE		<float>()));
				O_OCC_COMMUNICATION 	<float>(GetOption_Double(document, "O_OCC_COMMUNICATION", O_OCC_COMMUNICATION	<float>()));
				O_OCC_PROFESSIONAL 		<float>(GetOption_Double(document, "O_OCC_PROFESSIONAL"	, O_OCC_PROFESSIONAL 	<float>()));
				O_OCC_HEALTH 			<float>(GetOption_Double(document, "O_OCC_HEALTH"		, O_OCC_HEALTH 			<float>()));

				T_EMPPART				<float>(GetOption_Double(document, "T_EMPPART"			, T_EMPPART				<float>()));
				T_DEGREE 				<float>(GetOption_Double(document, "T_DEGREE"			, T_DEGREE 				<float>()));
				T_STUDENTPART 			<float>(GetOption_Double(document, "T_STUDENTPART"		, T_STUDENTPART 		<float>()));
				T_POPDENSITY 			<float>(GetOption_Double(document, "T_POPDENSITY"		, T_POPDENSITY 			<float>()));
				T_C1					<float>(GetOption_Double(document, "T_C1"				, T_C1					<float>()));
				T_C2 					<float>(GetOption_Double(document, "T_C2"				, T_C2 					<float>()));
				T_C3 					<float>(GetOption_Double(document, "T_C3"				, T_C3 					<float>()));
				T_RHO					<float>(GetOption_Double(document, "T_RHO"				, T_RHO					<float>()));

				
				return true;
			}
			//==============================================================================================================================
			// PARAMETER DECLARATIONS
			//--------------------------------------------------
			// Zero-inflation paramaters
			/*static m_data(float, Z_CONST, NONE, NONE);
			static m_data(float, Z_VEH_AVAIL, NONE, NONE);
			static m_data(float, Z_HU_DENSITY, NONE, NONE);
			static m_data(float, Z_OCC_INC, NONE, NONE);
			static m_data(float, Z_OCC_FINANCE, NONE, NONE);
			static m_data(float, Z_OCC_EDUCATION, NONE, NONE);
			static m_data(float, Z_DEGREE_ASSOC, NONE, NONE);		// has associates or technical school degree*/
			m_static_data(float, Z_CONST, NONE, NONE);
			m_static_data(float, Z_VEH_AVAIL, NONE, NONE);
			m_static_data(float, Z_HU_DENSITY, NONE, NONE);
			m_static_data(float, Z_OCC_INC, NONE, NONE);
			m_static_data(float, Z_OCC_FINANCE, NONE, NONE);
			m_static_data(float, Z_OCC_EDUCATION, NONE, NONE);
			m_static_data(float, Z_DEGREE_ASSOC, NONE, NONE);		// has associates or technical school degree

			static m_data(float, Z_CONSTANT, NONE, NONE);
			static m_data(float, Z_MALE, NONE, NONE);
			static m_data(float, Z_INCOME_LOW, NONE, NONE);
			static m_data(float, Z_EDUC_NO_COLLEGE, NONE, NONE);
			static m_data(float, Z_WORK_TRIP_DIST, NONE, NONE);
			static m_data(float, Z_NWORKERS, NONE, NONE);
			static m_data(float, Z_FLEX_WORK_INDICATOR, NONE, NONE);
			static m_data(float, Z_OCC_TRANS_UTILITY, NONE, NONE);
			static m_data(float, Z_OCC_MANAGEMENT, NONE, NONE);
			static m_data(float, Z_OCC_SERVICE, NONE, NONE);
			static m_data(float, Z_EMP_DENSITY_OVER_20000, NONE, NONE);
			static m_data(float, Z_POP_DENSITY, NONE, NONE);


			// Ordered probit model
			/*static m_data(float, O_CONST, NONE, NONE);
			static m_data(float, O_HINC, NONE, NONE);				// inc > 100k
			static m_data(float, O_MALE, NONE, NONE);
			static m_data(float, O_EMPFULL, NONE, NONE);
			static m_data(float, O_DEGREE_GRAD, NONE, NONE);
			static m_data(float, O_TTIME, NONE, NONE);
			static m_data(float, O_WORKDUR, NONE, NONE);
			static m_data(float, O_NVEH, NONE, NONE);
			static m_data(float, O_OCC_REALESTATE, NONE, NONE);		// real estate, rental, leasing service
			static m_data(float, O_OCC_COMMUNICATION, NONE, NONE);	// communication industry
			static m_data(float, O_OCC_PROFESSIONAL, NONE, NONE);	// professional, scientific, technical services
			static m_data(float, O_OCC_HEALTH, NONE, NONE);			//health care or social assistance*/
			m_static_data(float, O_CONST, NONE, NONE);
			m_static_data(float, O_HINC, NONE, NONE);				// inc > 100k
			m_static_data(float, O_MALE, NONE, NONE);
			m_static_data(float, O_EMPFULL, NONE, NONE);
			m_static_data(float, O_DEGREE_GRAD, NONE, NONE);
			m_static_data(float, O_TTIME, NONE, NONE);
			m_static_data(float, O_WORKDUR, NONE, NONE);
			m_static_data(float, O_NVEH, NONE, NONE);
			m_static_data(float, O_OCC_REALESTATE, NONE, NONE);		// real estate, rental, leasing service
			m_static_data(float, O_OCC_COMMUNICATION, NONE, NONE);	// communication industry
			m_static_data(float, O_OCC_PROFESSIONAL, NONE, NONE);	// professional, scientific, technical services
			m_static_data(float, O_OCC_HEALTH, NONE, NONE);			//health care or social assistance

			static m_data(float, O_CONSTANT, NONE, NONE);
			static m_data(float, O_INCOME_MED, NONE, NONE);
			static m_data(float, O_AGE_35_55, NONE, NONE);
			static m_data(float, O_EDUC_GRAD_DEGREE, NONE, NONE);
			static m_data(float, O_WORK_TRIP_TTIME, NONE, NONE);
			static m_data(float, O_NVEHICLE, NONE, NONE);
			static m_data(float, O_FLEX_WORK_INDICATOR, NONE, NONE);
			static m_data(float, O_OCC_GOVERNMENT, NONE, NONE);
			static m_data(float, O_OCC_COMMUNICATION, NONE, NONE);
			static m_data(float, O_OCC_MANUFACTURING, NONE, NONE);
			static m_data(float, O_EMP_DENSITY_UNDER_3000, NONE, NONE);


			// threshold variables
			/*static m_data(float, T_EMPPART, NONE, NONE);			// 1=part-time worker, 0=other
			static m_data(float, T_DEGREE, NONE, NONE);				// has bachelor or grad degree
			static m_data(float, T_STUDENTPART, NONE, NONE);		// 1=parttime student, 0=other
			static m_data(float, T_POPDENSITY, NONE, NONE);			
			static m_data(float, T_C1, NONE, NONE);
			static m_data(float, T_C2, NONE, NONE);
			static m_data(float, T_C3, NONE, NONE);
			static m_data(float, T_RHO, NONE, NONE);*/

			static m_data(float, T_WORK_DURATION, NONE, NONE);				// Average work duration in hours
			static m_data(float, T_VEH_AVAILABLE, NONE, NONE);				// vehicle is available for work
			static m_data(float, T_HH_VEH_OVER_2_INDICATOR, NONE, NONE);	// HH has more than 2 vehicles
			static m_data(float, T_C1, NONE, NONE);
			static m_data(float, T_C2, NONE, NONE);
			static m_data(float, T_C3, NONE, NONE);
			static m_data(float, T_RHO, NONE, NONE);
			m_static_data(float, T_EMPPART, NONE, NONE);			// 1=part-time worker, 0=other
			m_static_data(float, T_DEGREE, NONE, NONE);				// has bachelor or grad degree
			m_static_data(float, T_STUDENTPART, NONE, NONE);		// 1=parttime student, 0=other
			m_static_data(float, T_POPDENSITY, NONE, NONE);			
			m_static_data(float, T_C1, NONE, NONE);
			m_static_data(float, T_C2, NONE, NONE);
			m_static_data(float, T_C3, NONE, NONE);
			m_static_data(float, T_RHO, NONE, NONE);

			static void static_initializer()
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
		/*template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(Z_CONST) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_Z_CONST = -0.98;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(Z_VEH_AVAIL) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_Z_VEH_AVAIL = -0.8;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(Z_HU_DENSITY) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_Z_HU_DENSITY = -39.99;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(Z_OCC_INC) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_Z_OCC_INC = -0.81;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(Z_OCC_FINANCE) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_Z_OCC_FINANCE = 0.55;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(Z_OCC_EDUCATION) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_Z_OCC_EDUCATION = -0.57;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(Z_DEGREE_ASSOC) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_Z_DEGREE_ASSOC = -0.53;
		*/
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
		/*template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(O_CONST) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_O_CONST = -0.86;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(O_HINC) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_O_HINC = -0.44;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(O_MALE) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_O_MALE = 0.07;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(O_EMPFULL) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_O_EMPFULL = 0.04;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(O_DEGREE_GRAD) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_O_DEGREE_GRAD = 0.18;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(O_TTIME) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_O_TTIME = 0.03;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(O_WORKDUR) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_O_WORKDUR = -0.0007;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(O_NVEH) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_O_NVEH = -0.07;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(O_OCC_REALESTATE) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_O_OCC_REALESTATE = -0.48;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(O_OCC_COMMUNICATION) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_O_OCC_COMMUNICATION = 0.38;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(O_OCC_PROFESSIONAL) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_O_OCC_PROFESSIONAL = 0.29;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(O_OCC_HEALTH) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_O_OCC_HEALTH = -0.17;
		*/
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
		/*template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(T_EMPPART) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_T_EMPPART = 0.16;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(T_DEGREE) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_T_DEGREE = 0.22;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(T_STUDENTPART) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_T_STUDENTPART = -0.28;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(T_POPDENSITY) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_T_POPDENSITY = 4.79;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(T_C1) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_T_C1 = -2.9;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(T_C2) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_T_C2 = -1.13;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(T_C3) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_T_C3 = -0.34;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(T_RHO) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_T_RHO = -0.47;
		*/
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


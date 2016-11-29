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

			//==============================================================================================================================
			// PARAMETER DECLARATIONS
			//--------------------------------------------------
			// Zero-inflation paramaters
			static m_data(float, Z_CONST, NONE, NONE);
			static m_data(float, Z_VEH_AVAIL, NONE, NONE);
			static m_data(float, Z_HU_DENSITY, NONE, NONE);
			static m_data(float, Z_OCC_INC, NONE, NONE);
			static m_data(float, Z_OCC_FINANCE, NONE, NONE);
			static m_data(float, Z_OCC_EDUCATION, NONE, NONE);
			static m_data(float, Z_DEGREE_ASSOC, NONE, NONE);		// has associates or technical school degree

			// Ordered probit model
			static m_data(float, O_CONST, NONE, NONE);
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
			static m_data(float, O_OCC_HEALTH, NONE, NONE);			//health care or social assistance

			// threshold variables
			static m_data(float, T_EMPPART, NONE, NONE);			// 1=part-time worker, 0=other
			static m_data(float, T_DEGREE, NONE, NONE);				// has bachelor or grad degree
			static m_data(float, T_STUDENTPART, NONE, NONE);		// 1=parttime student, 0=other
			static m_data(float, T_POPDENSITY, NONE, NONE);			
			static m_data(float, T_C1, NONE, NONE);
			static m_data(float, T_C2, NONE, NONE);
			static m_data(float, T_C3, NONE, NONE);
			static m_data(float, T_RHO, NONE, NONE);


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
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();

				//==================================
				// create model variables
				//----------------------------------
				float pop_density = 0.0;
				float house_density = 0.0; // these are set to zero as I don't trust the parameters
				// vehicle availability
				float veh_avail = 0;
				if (household_properties->Number_of_workers<int>() > 0) veh_avail = std::max<float>(household_properties->Number_of_vehicles<float>() / household_properties->Number_of_workers<float>(), 1.0);
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
				// education level
				Person_Components::Types::EDUCATION_LEVEL educ = person_properties->Educational_Attainment<Person_Components::Types::EDUCATION_LEVEL>();
				float educ_assoc = educ == Person_Components::Types::EDUCATION_LEVEL::EDUC_12 ? 1.0 : 0.0;
				float educ_undergrad = educ == Person_Components::Types::EDUCATION_LEVEL::EDUC_UNDERGRADUATE ? 1.0 : 0.0;
				float educ_grad = educ == Person_Components::Types::EDUCATION_LEVEL::EDUC_GRADUATE ? 1.0 : 0.0;
				float male = person_properties->Gender<Person_Components::Types::GENDER>() == Person_Components::Types::MALE ? 1.0 : 0.0;
				float emp_full = person_properties->Is_Fulltime_Worker();
				float emp_part = person_properties->Is_Parttime_Worker();
				// other characteristics
				float trip_time = person_properties->Journey_To_Work_Travel_Time<Time_Minutes>();
				float work_dur = person_properties->Work_Hours<Time_Minutes>() / 5.0;
				float nveh = household_properties->Number_of_vehicles<float>();


				//=====================================
				//Telecommute option model
				//-------------------------------------
				float az = _Z_CONST + _Z_VEH_AVAIL * veh_avail + _Z_OCC_INC * occ_admin * low_income + _Z_OCC_FINANCE * occ_finance + _Z_OCC_EDUCATION * occ_education + _Z_DEGREE_ASSOC * educ_assoc + _Z_HU_DENSITY * house_density;
				float bx = _O_CONST + _O_HINC * high_income + _O_MALE * male + _O_EMPFULL * emp_full + _O_DEGREE_GRAD * educ_grad + _O_TTIME * trip_time + _O_WORKDUR * work_dur + _O_NVEH * nveh + _O_OCC_REALESTATE * occ_realestate + _O_OCC_COMMUNICATION * occ_communication + _O_OCC_PROFESSIONAL * occ_professional + _O_OCC_HEALTH * occ_healthcare;
				float gs = _T_EMPPART * emp_part + _T_DEGREE * (educ_undergrad + educ_grad) + _T_STUDENTPART * person_properties->Is_Student<NT>() + _T_POPDENSITY * pop_density;

				// set threshold values
				float threshold[4];
				threshold[0] = 0.0;
				threshold[1] = threshold[0] + exp(_T_C1 + gs);
				threshold[2] = threshold[1] + exp(_T_C2 + gs);
				threshold[3] = threshold[2] + exp(_T_C3 + gs);


				// probability of telecommute levels
				float prob[5];
				prob[0] = 1.0 - GLOBALS::Normal_Distribution->Cumulative_Distribution_Value<float>(az) + GLOBALS::Bivariate_Normal_Distribution->Cumulative_Distribution_Value<float>(az, -bx, _T_RHO);
				for (int i=1; i<4; i++) prob[i] = GLOBALS::Bivariate_Normal_Distribution->Cumulative_Distribution_Value<float>(az, threshold[i]-bx, _T_RHO);
				prob[4] = 1.0;


				// determine if telecommuting on travel day or not
				float r = GLOBALS::Uniform_RNG.Next_Rand<float>();
				if (r < prob[0]) return Types::TC_NEVER;
				else if (r < prob[1]) return Types::TC_YEARLY; // does not telecommute or few times a year
				else if (r < prob[2]) return Types::TC_MONTHLY;// once a month or more
				else if (r < prob[3]) return Types::TC_WEEKLY; // once a week or more
				else				  return Types::TC_DAILY; // Every day
			}
		};

		// INITIALIZE TELECOMMUTE CHOICE MODEL STATIC PARAMETERS
		#pragma region Choice option parameters	
		// zero-inflation parameters
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(Z_CONST) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_Z_CONST = -0.72;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(Z_VEH_AVAIL) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_Z_VEH_AVAIL = -0.8;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(Z_HU_DENSITY) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_Z_HU_DENSITY = -39.99;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(Z_OCC_INC) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_Z_OCC_INC = -0.81;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(Z_OCC_FINANCE) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_Z_OCC_FINANCE = 0.55;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(Z_OCC_EDUCATION) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_Z_OCC_EDUCATION = -0.57;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(Z_DEGREE_ASSOC) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_Z_DEGREE_ASSOC = -0.53;
		// ordered probit parameters
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(O_CONST) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_O_CONST = -0.86;
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
		// threshold parameters
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(T_EMPPART) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_T_EMPPART = 0.16;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(T_DEGREE) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_T_DEGREE = 0.22;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(T_STUDENTPART) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_T_STUDENTPART = -0.28;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(T_POPDENSITY) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_T_POPDENSITY = 4.79;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(T_C1) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_T_C1 = -2.9;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(T_C2) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_T_C2 = -1.13;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(T_C3) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_T_C3 = -0.34;
		template<typename MasterType, typename InheritanceList> typename Telecommute_Choice_Implementation<MasterType, InheritanceList>::type_of(T_RHO) Telecommute_Choice_Implementation<MasterType, InheritanceList>::_T_RHO = -0.47;


#pragma endregion
	}
}


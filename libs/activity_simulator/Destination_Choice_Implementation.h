#pragma once

#include "Destination_Chooser_Prototype.h"
#include "Person_Prototype.h"

namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Planning classes
		//----------------------------------------------------------------------------------

		implementation struct ADAPTS_Destination_Choice_Option : public Polaris_Component<MasterType,INHERIT(ADAPTS_Destination_Choice_Option),Data_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(ADAPTS_Destination_Choice_Option),Data_Object>::Component_Type ComponentType;


			// Pointer to the Parent class
			m_prototype(Prototypes::Person_Planner< typename MasterType::person_planner_type>, Parent_Planner, NONE, NONE);

			// PARAMETER DECLARATIONS
			#pragma region DESTINATION CHOICE MODEL PARAMETERS
			static m_data(float, BTT_PICK, NONE, NONE);		static m_data(float, BTT_OTHER_WORK, NONE, NONE);			static m_data(float, BTT_SERVICE, NONE, NONE);			static m_data(float, BTT_MIN_SHOP, NONE, NONE);		static m_data(float, BTT_MAJ_SHOP, NONE, NONE);		static m_data(float, BTT_EAT_OUT, NONE, NONE);			static m_data(float, BTT_CIVIC, NONE, NONE);			static m_data(float, BTT_LEISURE, NONE, NONE);			static m_data(float, BTT_SOCIAL, NONE, NONE);			static m_data(float, BTT_OTHER, NONE, NONE);			static m_data(float, BTTAUTO_WORK, NONE, NONE); static m_data(float, BTTTRAN_WORK, NONE, NONE); static m_data(float, BTTOTHER_WORK, NONE, NONE);
			static m_data(float, BINCD_PICK, NONE, NONE);		static m_data(float, BINCD_OTHER_WORK, NONE, NONE);		static m_data(float, BINCD_SERVICE, NONE, NONE);		static m_data(float, BINCD_MIN_SHOP, NONE, NONE);		static m_data(float, BINCD_MAJ_SHOP, NONE, NONE);		static m_data(float, BINCD_EAT_OUT, NONE, NONE);		static m_data(float, BINCD_CIVIC, NONE, NONE);			static m_data(float, BINCD_LEISURE, NONE, NONE);		static m_data(float, BINCD_SOCIAL, NONE, NONE);		static m_data(float, BINCD_OTHER, NONE, NONE);			static m_data(float, BINCD_WORK, NONE, NONE); static m_data(float, BHOME_WORK, NONE, NONE);
			static m_data(float, BRACED_PICK, NONE, NONE);		static m_data(float, BRACED_OTHER_WORK, NONE, NONE);		static m_data(float, BRACED_SERVICE, NONE, NONE);		static m_data(float, BRACED_MIN_SHOP, NONE, NONE);		static m_data(float, BRACED_MAJ_SHOP, NONE, NONE);		static m_data(float, BRACED_EAT_OUT, NONE, NONE);		static m_data(float, BRACED_CIVIC, NONE, NONE);		static m_data(float, BRACED_LEISURE, NONE, NONE);		static m_data(float, BRACED_SOCIAL, NONE, NONE);		static m_data(float, BRACED_OTHER, NONE, NONE);		static m_data(float, BRACED_WORK, NONE, NONE);
			static m_data(float, BArRes_PICK, NONE, NONE);		static m_data(float, BArRes_OTHER_WORK, NONE, NONE);		static m_data(float, BArRes_SERVICE, NONE, NONE);		static m_data(float, BArRes_MIN_SHOP, NONE, NONE);		static m_data(float, BArRes_MAJ_SHOP, NONE, NONE);		static m_data(float, BArRes_EAT_OUT, NONE, NONE);		static m_data(float, BArRes_CIVIC, NONE, NONE);		static m_data(float, BArRes_LEISURE, NONE, NONE);		static m_data(float, BArRes_SOCIAL, NONE, NONE);		static m_data(float, BArRes_OTHER, NONE, NONE);		static m_data(float, BArRes_WORK, NONE, NONE);
			static m_data(float, BArRec_PICK, NONE, NONE);		static m_data(float, BArRec_OTHER_WORK, NONE, NONE);		static m_data(float, BArRec_SERVICE, NONE, NONE);		static m_data(float, BArRec_MIN_SHOP, NONE, NONE);		static m_data(float, BArRec_MAJ_SHOP, NONE, NONE);		static m_data(float, BArRec_EAT_OUT, NONE, NONE);		static m_data(float, BArRec_CIVIC, NONE, NONE);		static m_data(float, BArRec_LEISURE, NONE, NONE);		static m_data(float, BArRec_SOCIAL, NONE, NONE);		static m_data(float, BArRec_OTHER, NONE, NONE);		static m_data(float, BArRec_WORK, NONE, NONE);
			static m_data(float, BArRet_PICK, NONE, NONE);		static m_data(float, BArRet_OTHER_WORK, NONE, NONE);		static m_data(float, BArRet_SERVICE, NONE, NONE);		static m_data(float, BArRet_MIN_SHOP, NONE, NONE);		static m_data(float, BArRet_MAJ_SHOP, NONE, NONE);		static m_data(float, BArRet_EAT_OUT, NONE, NONE);		static m_data(float, BArRet_CIVIC, NONE, NONE);		static m_data(float, BArRet_LEISURE, NONE, NONE);		static m_data(float, BArRet_SOCIAL, NONE, NONE);		static m_data(float, BArRet_OTHER, NONE, NONE);		static m_data(float, BArRet_WORK, NONE, NONE);
			static m_data(float, BArEnt_PICK, NONE, NONE);		static m_data(float, BArEnt_OTHER_WORK, NONE, NONE);		static m_data(float, BArEnt_SERVICE, NONE, NONE);		static m_data(float, BArEnt_MIN_SHOP, NONE, NONE);		static m_data(float, BArEnt_MAJ_SHOP, NONE, NONE);		static m_data(float, BArEnt_EAT_OUT, NONE, NONE);		static m_data(float, BArEnt_CIVIC, NONE, NONE);		static m_data(float, BArEnt_LEISURE, NONE, NONE);		static m_data(float, BArEnt_SOCIAL, NONE, NONE);		static m_data(float, BArEnt_OTHER, NONE, NONE);		static m_data(float, BArEnt_WORK, NONE, NONE);
			static m_data(float, BArIns_PICK, NONE, NONE);		static m_data(float, BArIns_OTHER_WORK, NONE, NONE);		static m_data(float, BArIns_SERVICE, NONE, NONE);		static m_data(float, BArIns_MIN_SHOP, NONE, NONE);		static m_data(float, BArIns_MAJ_SHOP, NONE, NONE);		static m_data(float, BArIns_EAT_OUT, NONE, NONE);		static m_data(float, BArIns_CIVIC, NONE, NONE);		static m_data(float, BArIns_LEISURE, NONE, NONE);		static m_data(float, BArIns_SOCIAL, NONE, NONE);		static m_data(float, BArIns_OTHER, NONE, NONE);		static m_data(float, BArIns_WORK, NONE, NONE);
			static m_data(float, BArOff_PICK, NONE, NONE);		static m_data(float, BArOff_OTHER_WORK, NONE, NONE);		static m_data(float, BArOff_SERVICE, NONE, NONE);		static m_data(float, BArOff_MIN_SHOP, NONE, NONE);		static m_data(float, BArOff_MAJ_SHOP, NONE, NONE);		static m_data(float, BArOff_EAT_OUT, NONE, NONE);		static m_data(float, BArOff_CIVIC, NONE, NONE);		static m_data(float, BArOff_LEISURE, NONE, NONE);		static m_data(float, BArOff_SOCIAL, NONE, NONE);		static m_data(float, BArOff_OTHER, NONE, NONE);		static m_data(float, BArOff_WORK, NONE, NONE);
			static m_data(float, BArMix_PICK, NONE, NONE);		static m_data(float, BArMix_OTHER_WORK, NONE, NONE);		static m_data(float, BArMix_SERVICE, NONE, NONE);		static m_data(float, BArMix_MIN_SHOP, NONE, NONE);		static m_data(float, BArMix_MAJ_SHOP, NONE, NONE);		static m_data(float, BArMix_EAT_OUT, NONE, NONE);		static m_data(float, BArMix_CIVIC, NONE, NONE);		static m_data(float, BArMix_LEISURE, NONE, NONE);		static m_data(float, BArMix_SOCIAL, NONE, NONE);		static m_data(float, BArMix_OTHER, NONE, NONE);		static m_data(float, BArMix_WORK, NONE, NONE);
			static m_data(float, BArSch_PICK, NONE, NONE);		static m_data(float, BArSch_OTHER_WORK, NONE, NONE);		static m_data(float, BArSch_SERVICE, NONE, NONE);		static m_data(float, BArSch_MIN_SHOP, NONE, NONE);		static m_data(float, BArSch_MAJ_SHOP, NONE, NONE);		static m_data(float, BArSch_EAT_OUT, NONE, NONE);		static m_data(float, BArSch_CIVIC, NONE, NONE);		static m_data(float, BArSch_LEISURE, NONE, NONE);		static m_data(float, BArSch_SOCIAL, NONE, NONE);		static m_data(float, BArSch_OTHER, NONE, NONE);		static m_data(float, BArSch_WORK, NONE, NONE);
			static m_data(float, BEmOth_PICK, NONE, NONE);		static m_data(float, BEmOth_OTHER_WORK, NONE, NONE);		static m_data(float, BEmOth_SERVICE, NONE, NONE);		static m_data(float, BEmOth_MIN_SHOP, NONE, NONE);		static m_data(float, BEmOth_MAJ_SHOP, NONE, NONE);		static m_data(float, BEmOth_EAT_OUT, NONE, NONE);		static m_data(float, BEmOth_CIVIC, NONE, NONE);		static m_data(float, BEmOth_LEISURE, NONE, NONE);		static m_data(float, BEmOth_SOCIAL, NONE, NONE);		static m_data(float, BEmOth_OTHER, NONE, NONE);		static m_data(float, BEmOth_WORK, NONE, NONE);
			static m_data(float, BEmInd_PICK, NONE, NONE);		static m_data(float, BEmInd_OTHER_WORK, NONE, NONE);		static m_data(float, BEmInd_SERVICE, NONE, NONE);		static m_data(float, BEmInd_MIN_SHOP, NONE, NONE);		static m_data(float, BEmInd_MAJ_SHOP, NONE, NONE);		static m_data(float, BEmInd_EAT_OUT, NONE, NONE);		static m_data(float, BEmInd_CIVIC, NONE, NONE);		static m_data(float, BEmInd_LEISURE, NONE, NONE);		static m_data(float, BEmInd_SOCIAL, NONE, NONE);		static m_data(float, BEmInd_OTHER, NONE, NONE);		static m_data(float, BEmInd_WORK, NONE, NONE);
			static m_data(float, BEmGov_PICK, NONE, NONE);		static m_data(float, BEmGov_OTHER_WORK, NONE, NONE);		static m_data(float, BEmGov_SERVICE, NONE, NONE);		static m_data(float, BEmGov_MIN_SHOP, NONE, NONE);		static m_data(float, BEmGov_MAJ_SHOP, NONE, NONE);		static m_data(float, BEmGov_EAT_OUT, NONE, NONE);		static m_data(float, BEmGov_CIVIC, NONE, NONE);		static m_data(float, BEmGov_LEISURE, NONE, NONE);		static m_data(float, BEmGov_SOCIAL, NONE, NONE);		static m_data(float, BEmGov_OTHER, NONE, NONE);		static m_data(float, BEmGov_WORK, NONE, NONE);
			static m_data(float, BEmSer_PICK, NONE, NONE);		static m_data(float, BEmSer_OTHER_WORK, NONE, NONE);		static m_data(float, BEmSer_SERVICE, NONE, NONE);		static m_data(float, BEmSer_MIN_SHOP, NONE, NONE);		static m_data(float, BEmSer_MAJ_SHOP, NONE, NONE);		static m_data(float, BEmSer_EAT_OUT, NONE, NONE);		static m_data(float, BEmSer_CIVIC, NONE, NONE);		static m_data(float, BEmSer_LEISURE, NONE, NONE);		static m_data(float, BEmSer_SOCIAL, NONE, NONE);		static m_data(float, BEmSer_OTHER, NONE, NONE);		static m_data(float, BEmSer_WORK, NONE, NONE);
			static m_data(float, BEmMan_PICK, NONE, NONE);		static m_data(float, BEmMan_OTHER_WORK, NONE, NONE);		static m_data(float, BEmMan_SERVICE, NONE, NONE);		static m_data(float, BEmMan_MIN_SHOP, NONE, NONE);		static m_data(float, BEmMan_MAJ_SHOP, NONE, NONE);		static m_data(float, BEmMan_EAT_OUT, NONE, NONE);		static m_data(float, BEmMan_CIVIC, NONE, NONE);		static m_data(float, BEmMan_LEISURE, NONE, NONE);		static m_data(float, BEmMan_SOCIAL, NONE, NONE);		static m_data(float, BEmMan_OTHER, NONE, NONE);		static m_data(float, BEmMan_WORK, NONE, NONE);
			static m_data(float, BEmRet_PICK, NONE, NONE);		static m_data(float, BEmRet_OTHER_WORK, NONE, NONE);		static m_data(float, BEmRet_SERVICE, NONE, NONE);		static m_data(float, BEmRet_MIN_SHOP, NONE, NONE);		static m_data(float, BEmRet_MAJ_SHOP, NONE, NONE);		static m_data(float, BEmRet_EAT_OUT, NONE, NONE);		static m_data(float, BEmRet_CIVIC, NONE, NONE);		static m_data(float, BEmRet_LEISURE, NONE, NONE);		static m_data(float, BEmRet_SOCIAL, NONE, NONE);		static m_data(float, BEmRet_OTHER, NONE, NONE);		static m_data(float, BEmRet_WORK, NONE, NONE);
			static m_data(float, THETAG_PICK, NONE, NONE);		static m_data(float, THETAG_OTHER_WORK, NONE, NONE);		static m_data(float, THETAG_SERVICE, NONE, NONE);		static m_data(float, THETAG_MIN_SHOP, NONE, NONE);		static m_data(float, THETAG_MAJ_SHOP, NONE, NONE);		static m_data(float, THETAG_EAT_OUT, NONE, NONE);		static m_data(float, THETAG_CIVIC, NONE, NONE);		static m_data(float, THETAG_LEISURE, NONE, NONE);		static m_data(float, THETAG_SOCIAL, NONE, NONE);		static m_data(float, THETAG_OTHER, NONE, NONE);		static m_data(float, THETAG_WORK, NONE, NONE);
			static m_data(float, THETAM_PICK, NONE, NONE);		static m_data(float, THETAM_OTHER_WORK, NONE, NONE);		static m_data(float, THETAM_SERVICE, NONE, NONE);		static m_data(float, THETAM_MIN_SHOP, NONE, NONE);		static m_data(float, THETAM_MAJ_SHOP, NONE, NONE);		static m_data(float, THETAM_EAT_OUT, NONE, NONE);		static m_data(float, THETAM_CIVIC, NONE, NONE);		static m_data(float, THETAM_LEISURE, NONE, NONE);		static m_data(float, THETAM_SOCIAL, NONE, NONE);		static m_data(float, THETAM_OTHER, NONE, NONE);		static m_data(float, THETAM_WORK, NONE, NONE);
			static m_data(float, THETAR_PICK, NONE, NONE);		static m_data(float, THETAR_OTHER_WORK, NONE, NONE);		static m_data(float, THETAR_SERVICE, NONE, NONE);		static m_data(float, THETAR_MIN_SHOP, NONE, NONE);		static m_data(float, THETAR_MAJ_SHOP, NONE, NONE);		static m_data(float, THETAR_EAT_OUT, NONE, NONE);		static m_data(float, THETAR_CIVIC, NONE, NONE);		static m_data(float, THETAR_LEISURE, NONE, NONE);		static m_data(float, THETAR_SOCIAL, NONE, NONE);		static m_data(float, THETAR_OTHER, NONE, NONE);		static m_data(float, THETAR_WORK, NONE, NONE);
			static m_data(float, THETAS_PICK, NONE, NONE);		static m_data(float, THETAS_OTHER_WORK, NONE, NONE);		static m_data(float, THETAS_SERVICE, NONE, NONE);		static m_data(float, THETAS_MIN_SHOP, NONE, NONE);		static m_data(float, THETAS_MAJ_SHOP, NONE, NONE);		static m_data(float, THETAS_EAT_OUT, NONE, NONE);		static m_data(float, THETAS_CIVIC, NONE, NONE);		static m_data(float, THETAS_LEISURE, NONE, NONE);		static m_data(float, THETAS_SOCIAL, NONE, NONE);		static m_data(float, THETAS_OTHER, NONE, NONE);		static m_data(float, THETAS_WORK, NONE, NONE);
			static m_data(float, THETAI_PICK, NONE, NONE);		static m_data(float, THETAI_OTHER_WORK, NONE, NONE);		static m_data(float, THETAI_SERVICE, NONE, NONE);		static m_data(float, THETAI_MIN_SHOP, NONE, NONE);		static m_data(float, THETAI_MAJ_SHOP, NONE, NONE);		static m_data(float, THETAI_EAT_OUT, NONE, NONE);		static m_data(float, THETAI_CIVIC, NONE, NONE);		static m_data(float, THETAI_LEISURE, NONE, NONE);		static m_data(float, THETAI_SOCIAL, NONE, NONE);		static m_data(float, THETAI_OTHER, NONE, NONE);		static m_data(float, THETAI_WORK, NONE, NONE);
			static m_data(float, THETAO_PICK, NONE, NONE);		static m_data(float, THETAO_OTHER_WORK, NONE, NONE);		static m_data(float, THETAO_SERVICE, NONE, NONE);		static m_data(float, THETAO_MIN_SHOP, NONE, NONE);		static m_data(float, THETAO_MAJ_SHOP, NONE, NONE);		static m_data(float, THETAO_EAT_OUT, NONE, NONE);		static m_data(float, THETAO_CIVIC, NONE, NONE);		static m_data(float, THETAO_LEISURE, NONE, NONE);		static m_data(float, THETAO_SOCIAL, NONE, NONE);		static m_data(float, THETAO_OTHER, NONE, NONE);		static m_data(float, THETAO_WORK, NONE, NONE);	static m_data(float, BEmUnrelated_WORK, NONE, NONE); static m_data(float, THETA_UR_WORK, NONE, NONE);
			static m_data(float, GAMMA_PICK, NONE, NONE);		static m_data(float, GAMMA_OTHER_WORK, NONE, NONE);		static m_data(float, GAMMA_SERVICE, NONE, NONE);		static m_data(float, GAMMA_MIN_SHOP, NONE, NONE);		static m_data(float, GAMMA_MAJ_SHOP, NONE, NONE);		static m_data(float, GAMMA_EAT_OUT, NONE, NONE);		static m_data(float, GAMMA_CIVIC, NONE, NONE);			static m_data(float, GAMMA_LEISURE, NONE, NONE);		static m_data(float, GAMMA_SOCIAL, NONE, NONE);		static m_data(float, GAMMA_OTHER, NONE, NONE);		
			#pragma endregion

			m_prototype(Activity_Location_Components::Prototypes::Activity_Location< typename MasterType::activity_location_type>, previous, NONE, NONE );
			m_prototype(Activity_Location_Components::Prototypes::Activity_Location< typename MasterType::activity_location_type>, destination, NONE, NONE );
			m_prototype(Activity_Location_Components::Prototypes::Activity_Location< typename MasterType::activity_location_type>, next, NONE, NONE );
			m_data(Activity_Components::Types::ACTIVITY_TYPES, activity_type, NONE, NONE);
			
			//====================================================================================================================================
			// Interface definitions
			typedef Prototypes::Person<typename type_of(Parent_Planner)::type_of(Parent_Person)> person_itf;
			typedef Prototypes::Person_Properties<typename person_itf::get_type_of(Static_Properties)> person_properties_itf;
			typedef Prototypes::Person_Scheduler<typename person_itf::get_type_of(Scheduling_Faculty)> scheduler_itf;
			typedef Scenario_Components::Prototypes::Scenario< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference)> _Scenario_Interface;
			typedef Network_Components::Prototypes::Network< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference)> _Network_Interface;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _Network_Interface::get_type_of(skimming_faculty)> _Skim_Interface;
			
			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(_activity_locations_container_itf)>  _Activity_Location_Interface;	
				
			typedef Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links)> _Links_Container_Interface;
			typedef Link_Components::Prototypes::Link<typename get_component_type(_links_container_itf)>  _Link_Interface;
	
			typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
			typedef Zone_Components::Prototypes::Zone<typename get_mapped_component_type(_zones_container_itf)>  _Zone_Interface;

			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Activity_Container)> Activity_Plans;
			typedef Activity_Components::Prototypes::Activity_Planner<typename get_component_type(Activity_Plans)> Activity_Plan;
			
			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Movement_Plans_Container)> Movement_Plans;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename get_component_type(Movement_Plans)> Movement_Plan;

			//------------------------------------------------------------------------------------------------------------------------------------

			template<typename TargetType> void Initialize()
			{	
			}

			template<typename TargetType> TargetType Calculate_Utility()
			{
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
				person_properties_itf* properties = _Parent_Person->Static_Properties<person_properties_itf*>();

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();
				_Activity_Locations_Container_Interface* locations = network->template activity_locations_container<_Activity_Locations_Container_Interface*>();
				_Skim_Interface* los = network->template skimming_faculty<_Skim_Interface*>();

				// variables used for utility calculation
				TargetType ttime_before, ttime_after, ttime_without, ttime_deflected, u;
				TargetType utility_sum = 0;
				TargetType prob_sum = 0;
				_Zone_Interface* zone;

				// select zones to choose from and estimate utility
				zone = _destination->template zone<_Zone_Interface*>();

				// get the deflected travel time due to the addition of the current activity
				ttime_before = los->template Get_TTime<_Activity_Location_Interface*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Minutes>(_previous,_destination,Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				ttime_after = los->template Get_TTime<_Activity_Location_Interface*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Minutes>(_destination,_next,Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				if (_previous == _next) ttime_without = 0;
				else ttime_without = los->template Get_TTime<_Activity_Location_Interface*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Minutes>(_previous,_next,Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				ttime_deflected = ttime_before + ttime_after - ttime_without;
				
				// Get Income/race dif with zone
				float inc_diff = log(abs(zone->average_household_income<Dollars>() - properties->Income<Dollars>()) + 1.0);
				float race_diff;
				if (properties->Race<Person_Components::Types::RACE>() == Person_Components::Types::RACE::WHITE_ALONE) race_diff = 1.0 - zone->race_percent_white<float>();
				else if (properties->Race<Person_Components::Types::RACE>() == Person_Components::Types::RACE::BLACK_ALONE) race_diff = 1.0 - zone->race_percent_black<float>();
				else race_diff = zone->race_percent_white<float>() + zone->race_percent_black<float>();
					
				float area_res = zone->residential_area<Square_Feet>()/1000000.0;
				float area_rec = zone->other_area<Square_Feet>()/1000000.0;
				float area_ret = zone->retail_area<Square_Feet>()/1000000.0;
				float area_ent = zone->entertainment_area<Square_Feet>()/1000000.0;
				float area_ins = zone->institutional_area<Square_Feet>()/1000000.0;
				float area_off = zone->office_area<Square_Feet>()/1000000.0;
				float area_mix = zone->mixed_use_area<Square_Feet>()/1000000.0;
				float area_sch = zone->school_area<Square_Feet>()/1000000.0;
				float emp_oth = zone->employment_other<TargetType>()/1000.0;
				float emp_ind = zone->employment_industrial<TargetType>()/1000.0;
				float emp_man = zone->employment_manufacturing<TargetType>()/1000.0;
				float emp_gov = zone->employment_government<TargetType>()/1000.0;
				float emp_srv = zone->employment_services<TargetType>()/1000.0;
				float emp_ret = zone->employment_retail<TargetType>()/1000.0;

				float thetag = zone->accessibility_employment_government<TargetType>()/1000.0;
				float thetam = zone->accessibility_employment_manufacturing<TargetType>()/1000.0;
				float thetar = zone->accessibility_employment_retail<TargetType>()/1000.0;
				float thetas = zone->accessibility_employment_services<TargetType>()/1000.0;
				float thetai = zone->accessibility_employment_industrial<TargetType>()/1000.0;
				float thetao = zone->accessibility_employment_other<TargetType>()/1000.0;

				if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY)
				{
					Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE industry = properties->Employment_Industry_Simple<Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE>();
					float EMPUR = 0;

					// Mode specific effects
					int ModAuto = 0, ModTran = 0,ModOth = 0;
					Person_Components::Types::JOURNEY_TO_WORK_MODE mode = properties->Journey_To_Work_Mode<Person_Components::Types::JOURNEY_TO_WORK_MODE>();
					if (mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_AUTOMOBILE || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_MOTORCYCLE || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_TAXI)
						ModAuto = 1;
					else if (mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_BUS || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_RAILROAD || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_STREETCAR || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_SUBWAY)
						ModTran = 1;
					else
						ModOth = 1;

					// positive impact of working near home
					float HOME = 0.0; 
					if (zone = _Parent_Person->Home_Location<_Zone_Interface*>()) HOME = 1.0;

					// industry type interaction variables
					float IndR = 0, IndS = 0, IndG = 0, IndI = 0, IndM = 0, IndO = 0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::RETAIL) IndR = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::SERVICE) IndS = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::GOVERNMENT) IndG = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::INDUSTRIAL) IndI = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::MANUFACTURING) IndM = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::OTHER) IndO = 1.0;

					float area_res = log(zone->residential_area<Square_Feet>()+1.0);
					float area_rec = log(zone->other_area<Square_Feet>()+1.0);
					float area_ret = log(zone->retail_area<Square_Feet>()+1.0);
					float area_ent = log(zone->entertainment_area<Square_Feet>()+1.0);
					float area_ins = log(zone->institutional_area<Square_Feet>()+1.0);
					float area_off = log(zone->office_area<Square_Feet>()+1.0);
					float area_mix = log(zone->mixed_use_area<Square_Feet>()+1.0);
					float area_sch = log(zone->school_area<Square_Feet>()+1.0);

					// Unrelated employment
					EMPUR += (max<float>(zone->employment_retail<float>(), 0.0) / 1000.0) * (-1.0 * (IndR - 1.0));
					EMPUR += (max<float>(zone->employment_government<float>(), 0.0) / 1000.0) * (-1.0 * (IndG - 1.0));
					EMPUR += (max<float>(zone->employment_services<float>(), 0.0) / 1000.0) * (-1.0 * (IndS - 1.0));
					EMPUR += (max<float>(zone->employment_industrial<float>(), 0.0) / 1000.0) * (-1.0 * (IndI - 1.0));
					EMPUR += (max<float>(zone->employment_manufacturing<float>(), 0.0) / 1000.0) * (-1.0 * (IndM - 1.0));
					EMPUR += (max<float>(zone->employment_other<float>(), 0.0) / 1000.0) * (-1.0 * (IndO - 1.0));

					float theta_ur = thetar * (1.0 - IndR) + thetag * (1.0 - IndG) + thetas * (1.0 - IndS) + thetai * (1.0 - IndI) + thetam * (1.0 - IndM) + thetao * (1.0 - IndO);

					// cut travel time in half since original work location choice equations only account for the half-tour travel time
					ttime_deflected = ttime_deflected *0.5;

					// Old values - with time strata at (45A/60T/20W)
					u = _BArEnt_WORK * area_ent + _BArIns_WORK * area_ins + _BArOff_WORK * area_off + _BArRec_WORK * area_rec + _BArRet_WORK *area_ret + _BArRes_WORK * area_res + _BEmUnrelated_WORK * EMPUR + _BEmGov_WORK * (zone->employment_government<float>()) / 1000.0 * IndG + _BEmMan_WORK * (zone->employment_manufacturing<float>()) / 1000.0 * IndM + _BEmRet_WORK * (zone->employment_retail<float>()) / 1000.0 * IndR + _BEmSer_WORK * (zone->employment_services<float>()) / 1000.0 * IndS + _BHOME_WORK * HOME + _BTTAUTO_WORK * (ttime_deflected) * ModAuto + _BTTTRAN_WORK * (ttime_deflected) * ModTran + _BTTOTHER_WORK * (ttime_deflected) * ModOth + thetag * _THETAG_WORK + thetam * _THETAM_WORK + thetar * _THETAR_WORK + thetas * _THETAS_WORK + thetai * _THETAI_WORK + thetao * _THETAO_WORK + _THETA_UR_WORK * theta_ur;
					if (zone->employment_total<int>() < 1.0) u = -9999999;
					if (ISNAN(u))
					{
						THROW_WARNING("WARNING: utility is not numeric, possible misspecification in utility function for destination choice. [Pop,emp,ttime]=, " << ttime_deflected);
						u = -99999.9;
					}
				}
				else if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::PICK_UP_OR_DROP_OFF_ACTIVITY)
				{
					u = ttime_deflected * _BTT_PICK+inc_diff * _BINCD_PICK+race_diff * _BRACED_PICK + _GAMMA_PICK * log(area_res * _BArRes_PICK+area_rec * _BArRec_PICK+area_ret * _BArRet_PICK+area_ent * _BArEnt_PICK+area_ins * _BArIns_PICK+area_off * _BArOff_PICK+area_mix * _BArMix_PICK+area_sch * _BArSch_PICK+emp_oth * _BEmOth_PICK+emp_ind * _BEmInd_PICK+emp_gov * _BEmGov_PICK+emp_srv * _BEmSer_PICK+emp_man * _BEmMan_PICK+emp_ret * _BEmRet_PICK + 0.000001) + thetag * _THETAG_PICK+thetam * _THETAM_PICK+thetar * _THETAR_PICK+thetas * _THETAS_PICK+thetai * _THETAI_PICK+thetao * _THETAO_PICK;
				}
				else if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::OTHER_WORK_ACTIVITY)
				{
					u = ttime_deflected * _BTT_OTHER_WORK+inc_diff * _BINCD_OTHER_WORK+race_diff * _BRACED_OTHER_WORK + _GAMMA_OTHER_WORK * log(area_res * _BArRes_OTHER_WORK+area_rec * _BArRec_OTHER_WORK+area_ret * _BArRet_OTHER_WORK+area_ent * _BArEnt_OTHER_WORK+area_ins * _BArIns_OTHER_WORK+area_off * _BArOff_OTHER_WORK+area_mix * _BArMix_OTHER_WORK+area_sch * _BArSch_OTHER_WORK+emp_oth * _BEmOth_OTHER_WORK+emp_ind * _BEmInd_OTHER_WORK+emp_gov * _BEmGov_OTHER_WORK+emp_srv * _BEmSer_OTHER_WORK+emp_man * _BEmMan_OTHER_WORK+emp_ret * _BEmRet_OTHER_WORK + 0.000001) + thetag * _THETAG_OTHER_WORK+thetam * _THETAM_OTHER_WORK+thetar * _THETAR_OTHER_WORK+thetas * _THETAS_OTHER_WORK+thetai * _THETAI_OTHER_WORK+thetao * _THETAO_OTHER_WORK;
				}
				else if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::SERVICE_VEHICLE_ACTIVITY)
				{
					u = ttime_deflected * _BTT_SERVICE+inc_diff * _BINCD_SERVICE+race_diff * _BRACED_SERVICE+ _GAMMA_SERVICE * log(area_res * _BArRes_SERVICE+area_rec * _BArRec_SERVICE+area_ret * _BArRet_SERVICE+area_ent * _BArEnt_SERVICE+area_ins * _BArIns_SERVICE+area_off * _BArOff_SERVICE+area_mix * _BArMix_SERVICE+area_sch * _BArSch_SERVICE+emp_oth * _BEmOth_SERVICE+emp_ind * _BEmInd_SERVICE+emp_gov * _BEmGov_SERVICE+emp_srv * _BEmSer_SERVICE+emp_man * _BEmMan_SERVICE+emp_ret * _BEmRet_SERVICE + 0.000001) + thetag * _THETAG_SERVICE+thetam * _THETAM_SERVICE+thetar * _THETAR_SERVICE+thetas * _THETAS_SERVICE+thetai * _THETAI_SERVICE+thetao * _THETAO_SERVICE;
				}
				else if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::OTHER_SHOPPING_ACTIVITY)
				{
					u = ttime_deflected * _BTT_MIN_SHOP+inc_diff * _BINCD_MIN_SHOP+race_diff * _BRACED_MIN_SHOP+ _GAMMA_MIN_SHOP * log(area_res * _BArRes_MIN_SHOP+area_rec * _BArRec_MIN_SHOP+area_ret * _BArRet_MIN_SHOP+area_ent * _BArEnt_MIN_SHOP+area_ins * _BArIns_MIN_SHOP+area_off * _BArOff_MIN_SHOP+area_mix * _BArMix_MIN_SHOP+area_sch * _BArSch_MIN_SHOP+emp_oth * _BEmOth_MIN_SHOP+emp_ind * _BEmInd_MIN_SHOP+emp_gov * _BEmGov_MIN_SHOP+emp_srv * _BEmSer_MIN_SHOP+emp_man * _BEmMan_MIN_SHOP+emp_ret * _BEmRet_MIN_SHOP + 0.000001) + thetag * _THETAG_MIN_SHOP+thetam * _THETAM_MIN_SHOP+thetar * _THETAR_MIN_SHOP+thetas * _THETAS_MIN_SHOP+thetai * _THETAI_MIN_SHOP+thetao * _THETAO_MIN_SHOP;
				}
				else if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::MAJOR_SHOPPING_ACTIVITY)
				{
					u = ttime_deflected * _BTT_MAJ_SHOP+inc_diff * _BINCD_MAJ_SHOP+race_diff * _BRACED_MAJ_SHOP + _GAMMA_MAJ_SHOP * log(area_res * _BArRes_MAJ_SHOP+area_rec * _BArRec_MAJ_SHOP+area_ret * _BArRet_MAJ_SHOP+area_ent * _BArEnt_MAJ_SHOP+area_ins * _BArIns_MAJ_SHOP+area_off * _BArOff_MAJ_SHOP+area_mix * _BArMix_MAJ_SHOP+area_sch * _BArSch_MAJ_SHOP+emp_oth * _BEmOth_MAJ_SHOP+emp_ind * _BEmInd_MAJ_SHOP+emp_gov * _BEmGov_MAJ_SHOP+emp_srv * _BEmSer_MAJ_SHOP+emp_man * _BEmMan_MAJ_SHOP+emp_ret * _BEmRet_MAJ_SHOP + 0.000001) + thetag * _THETAG_MAJ_SHOP+thetam * _THETAM_MAJ_SHOP+thetar * _THETAR_MAJ_SHOP+thetas * _THETAS_MAJ_SHOP+thetai * _THETAI_MAJ_SHOP+thetao * _THETAO_MAJ_SHOP;
				}
				else if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::EAT_OUT_ACTIVITY)
				{
					u = ttime_deflected * _BTT_EAT_OUT+inc_diff * _BINCD_EAT_OUT+race_diff * _BRACED_EAT_OUT + _GAMMA_EAT_OUT * log(area_res * _BArRes_EAT_OUT+area_rec * _BArRec_EAT_OUT+area_ret * _BArRet_EAT_OUT+area_ent * _BArEnt_EAT_OUT+area_ins * _BArIns_EAT_OUT+area_off * _BArOff_EAT_OUT+area_mix * _BArMix_EAT_OUT+area_sch * _BArSch_EAT_OUT+emp_oth * _BEmOth_EAT_OUT+emp_ind * _BEmInd_EAT_OUT+emp_gov * _BEmGov_EAT_OUT+emp_srv * _BEmSer_EAT_OUT+emp_man * _BEmMan_EAT_OUT+emp_ret * _BEmRet_EAT_OUT + 0.000001) + thetag * _THETAG_EAT_OUT+thetam * _THETAM_EAT_OUT+thetar * _THETAR_EAT_OUT+thetas * _THETAS_EAT_OUT+thetai * _THETAI_EAT_OUT+thetao * _THETAO_EAT_OUT;
				}
				else if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::RELIGIOUS_OR_CIVIC_ACTIVITY)
				{
					u = ttime_deflected * _BTT_CIVIC+inc_diff * _BINCD_CIVIC+race_diff * _BRACED_CIVIC + _GAMMA_CIVIC * log(area_res * _BArRes_CIVIC+area_rec * _BArRec_CIVIC+area_ret * _BArRet_CIVIC+area_ent * _BArEnt_CIVIC+area_ins * _BArIns_CIVIC+area_off * _BArOff_CIVIC+area_mix * _BArMix_CIVIC+area_sch * _BArSch_CIVIC+emp_oth * _BEmOth_CIVIC+emp_ind * _BEmInd_CIVIC+emp_gov * _BEmGov_CIVIC+emp_srv * _BEmSer_CIVIC+emp_man * _BEmMan_CIVIC+emp_ret * _BEmRet_CIVIC + 0.000001) + thetag * _THETAG_CIVIC+thetam * _THETAM_CIVIC+thetar * _THETAR_CIVIC+thetas * _THETAS_CIVIC+thetai * _THETAI_CIVIC+thetao * _THETAO_CIVIC;
				}
				else if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::LEISURE_ACTIVITY || _activity_type == Activity_Components::Types::ACTIVITY_TYPES::RECREATION_ACTIVITY)
				{
					u = ttime_deflected * _BTT_LEISURE+inc_diff * _BINCD_LEISURE+race_diff * _BRACED_LEISURE + _GAMMA_LEISURE * log(area_res * _BArRes_LEISURE+area_rec * _BArRec_LEISURE+area_ret * _BArRet_LEISURE+area_ent * _BArEnt_LEISURE+area_ins * _BArIns_LEISURE+area_off * _BArOff_LEISURE+area_mix * _BArMix_LEISURE+area_sch * _BArSch_LEISURE+emp_oth * _BEmOth_LEISURE+emp_ind * _BEmInd_LEISURE+emp_gov * _BEmGov_LEISURE+emp_srv * _BEmSer_LEISURE+emp_man * _BEmMan_LEISURE+emp_ret * _BEmRet_LEISURE + 0.000001) + thetag * _THETAG_LEISURE+thetam * _THETAM_LEISURE+thetar * _THETAR_LEISURE+thetas * _THETAS_LEISURE+thetai * _THETAI_LEISURE+thetao * _THETAO_LEISURE;
				}
				else if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::SOCIAL_ACTIVITY)
				{
					u = ttime_deflected * _BTT_SOCIAL+inc_diff * _BINCD_SOCIAL+race_diff * _BRACED_SOCIAL + _GAMMA_SOCIAL * log(area_res * _BArRes_SOCIAL+area_rec * _BArRec_SOCIAL+area_ret * _BArRet_SOCIAL+area_ent * _BArEnt_SOCIAL+area_ins * _BArIns_SOCIAL+area_off * _BArOff_SOCIAL+area_mix * _BArMix_SOCIAL+area_sch * _BArSch_SOCIAL+emp_oth * _BEmOth_SOCIAL+emp_ind * _BEmInd_SOCIAL+emp_gov * _BEmGov_SOCIAL+emp_srv * _BEmSer_SOCIAL+emp_man * _BEmMan_SOCIAL+emp_ret * _BEmRet_SOCIAL + 0.000001) + thetag * _THETAG_SOCIAL+thetam * _THETAM_SOCIAL+thetar * _THETAR_SOCIAL+thetas * _THETAS_SOCIAL+thetai * _THETAI_SOCIAL+thetao * _THETAO_SOCIAL;
				}
				else
				{
					u = ttime_deflected * _BTT_OTHER+inc_diff * _BINCD_OTHER+race_diff * _BRACED_OTHER + _GAMMA_OTHER * log(area_res * _BArRes_OTHER+area_rec * _BArRec_OTHER+area_ret * _BArRet_OTHER+area_ent * _BArEnt_OTHER+area_ins * _BArIns_OTHER+area_off * _BArOff_OTHER+area_mix * _BArMix_OTHER+area_sch * _BArSch_OTHER+emp_oth * _BEmOth_OTHER+emp_ind * _BEmInd_OTHER+emp_gov * _BEmGov_OTHER+emp_srv * _BEmSer_OTHER+emp_man * _BEmMan_OTHER+emp_ret * _BEmRet_OTHER + 0.000001) + thetag * _THETAG_OTHER+thetam * _THETAM_OTHER+thetar * _THETAR_OTHER+thetas * _THETAS_OTHER+thetai * _THETAI_OTHER+thetao * _THETAO_OTHER;
				}
				if (u > 100.0)
				{
					THROW_WARNING("WARNING: utility > 200.0 will cause numeric overflow, possible misspecification in utility function for destination choice. [Pop,emp,ttime]=, " << ttime_deflected);
				}
				if (ISNAN(u))
				{
					THROW_WARNING("WARNING: utility is not numeric, possible misspecification in utility function for destination choice. [Pop,emp,ttime]=, " << ttime_deflected);
					u = -9999.9;
				}
				return (TargetType)u;				
			}

			template<typename TargetType> TargetType Print_Utility()
			{
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
				person_properties_itf* properties = _Parent_Person->Static_Properties<person_properties_itf*>();

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();
				_Activity_Locations_Container_Interface* locations = network->template activity_locations_container<_Activity_Locations_Container_Interface*>();
				_Skim_Interface* los = network->template skimming_faculty<_Skim_Interface*>();

				// variables used for utility calculation
				TargetType ttime_before, ttime_after, ttime_without, ttime_deflected, u;
				TargetType utility_sum = 0;
				TargetType prob_sum = 0;
				_Zone_Interface* zone;

				// select zones to choose from and estimate utility
				zone = _destination->template zone<_Zone_Interface*>();

				// get the deflected travel time due to the addition of the current activity
				ttime_before = los->template Get_TTime<_Activity_Location_Interface*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Minutes>(_previous,_destination,Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				ttime_after = los->template Get_TTime<_Activity_Location_Interface*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Minutes>(_destination,_next,Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				if (_previous == _next) ttime_without = 0;
				else ttime_without = los->template Get_TTime<_Activity_Location_Interface*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Minutes>(_previous,_next,Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				ttime_deflected = ttime_before + ttime_after - ttime_without;
				
				// Get Income/race dif with zone
				float inc_diff = log(abs(zone->average_household_income<Dollars>() - properties->Income<Dollars>()) + 1.0);
				float race_diff;
				if (properties->Race<Person_Components::Types::RACE>() == Person_Components::Types::RACE::WHITE_ALONE) race_diff = 1.0 - zone->race_percent_white<float>();
				else if (properties->Race<Person_Components::Types::RACE>() == Person_Components::Types::RACE::BLACK_ALONE) race_diff = 1.0 - zone->race_percent_black<float>();
				else race_diff = zone->race_percent_white<float>() + zone->race_percent_black<float>();
					
				float area_res = zone->residential_area<Square_Feet>()/1000000.0;
				float area_rec = zone->other_area<Square_Feet>()/1000000.0;
				float area_ret = zone->retail_area<Square_Feet>()/1000000.0;
				float area_ent = zone->entertainment_area<Square_Feet>()/1000000.0;
				float area_ins = zone->institutional_area<Square_Feet>()/1000000.0;
				float area_off = zone->office_area<Square_Feet>()/1000000.0;
				float area_mix = zone->mixed_use_area<Square_Feet>()/1000000.0;
				float area_sch = zone->school_area<Square_Feet>()/1000000.0;
				float emp_oth = zone->employment_other<TargetType>()/1000.0;
				float emp_ind = zone->employment_industrial<TargetType>()/1000.0;
				float emp_man = zone->employment_manufacturing<TargetType>()/1000.0;
				float emp_gov = zone->employment_government<TargetType>()/1000.0;
				float emp_srv = zone->employment_services<TargetType>()/1000.0;
				float emp_ret = zone->employment_retail<TargetType>()/1000.0;

				float thetag = zone->accessibility_employment_government<TargetType>()/1000.0;
				float thetam = zone->accessibility_employment_manufacturing<TargetType>()/1000.0;
				float thetar = zone->accessibility_employment_retail<TargetType>()/1000.0;
				float thetas = zone->accessibility_employment_services<TargetType>()/1000.0;
				float thetai = zone->accessibility_employment_industrial<TargetType>()/1000.0;
				float thetao = zone->accessibility_employment_other<TargetType>()/1000.0;

				if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY)
				{
					Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE industry = properties->Employment_Industry_Simple<Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE>();
					float EMPUR = 0;

					// Mode specific effects
					int ModAuto = 0, ModTran = 0,ModOth = 0;
					Person_Components::Types::JOURNEY_TO_WORK_MODE mode = properties->Journey_To_Work_Mode<Person_Components::Types::JOURNEY_TO_WORK_MODE>();
					if (mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_AUTOMOBILE || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_MOTORCYCLE || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_TAXI)
						ModAuto = 1;
					else if (mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_BUS || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_RAILROAD || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_STREETCAR || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_SUBWAY)
						ModTran = 1;
					else
						ModOth = 1;

					// positive impact of working near home
					float HOME = 0.0; 
					if (zone = _Parent_Person->Home_Location<_Zone_Interface*>()) HOME = 1.0;

					// industry type interaction variables
					float IndR = 0, IndS = 0, IndG = 0, IndI = 0, IndM = 0, IndO = 0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::RETAIL) IndR = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::SERVICE) IndS = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::GOVERNMENT) IndG = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::INDUSTRIAL) IndI = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::MANUFACTURING) IndM = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::OTHER) IndO = 1.0;

					float area_res = log(zone->residential_area<Square_Feet>()+1.0);
					float area_rec = log(zone->other_area<Square_Feet>()+1.0);
					float area_ret = log(zone->retail_area<Square_Feet>()+1.0);
					float area_ent = log(zone->entertainment_area<Square_Feet>()+1.0);
					float area_ins = log(zone->institutional_area<Square_Feet>()+1.0);
					float area_off = log(zone->office_area<Square_Feet>()+1.0);
					float area_mix = log(zone->mixed_use_area<Square_Feet>()+1.0);
					float area_sch = log(zone->school_area<Square_Feet>()+1.0);

					// Unrelated employment
					EMPUR += (max<float>(zone->employment_retail<float>(), 0.0) / 1000.0) * (-1.0 * (IndR - 1.0));
					EMPUR += (max<float>(zone->employment_government<float>(), 0.0) / 1000.0) * (-1.0 * (IndG - 1.0));
					EMPUR += (max<float>(zone->employment_services<float>(), 0.0) / 1000.0) * (-1.0 * (IndS - 1.0));
					EMPUR += (max<float>(zone->employment_industrial<float>(), 0.0) / 1000.0) * (-1.0 * (IndI - 1.0));
					EMPUR += (max<float>(zone->employment_manufacturing<float>(), 0.0) / 1000.0) * (-1.0 * (IndM - 1.0));
					EMPUR += (max<float>(zone->employment_other<float>(), 0.0) / 1000.0) * (-1.0 * (IndO - 1.0));

					float theta_ur = thetar * (1.0 - IndR) + thetag * (1.0 - IndG) + thetas * (1.0 - IndS) + thetai * (1.0 - IndI) + thetam * (1.0 - IndM) + thetao * (1.0 - IndO);

					// cut travel time in half since original work location choice equations only account for the half-tour travel time
					ttime_deflected = ttime_deflected *0.5;

					// Old values - with time strata at (45A/60T/20W)
					u = _BArEnt_WORK * area_ent + _BArIns_WORK * area_ins + _BArOff_WORK * area_off + _BArRec_WORK * area_rec + _BArRet_WORK *area_ret + _BArRes_WORK * area_res + _BEmUnrelated_WORK * EMPUR + _BEmGov_WORK * (zone->employment_government<float>()) / 1000.0 * IndG + _BEmMan_WORK * (zone->employment_manufacturing<float>()) / 1000.0 * IndM + _BEmRet_WORK * (zone->employment_retail<float>()) / 1000.0 * IndR + _BEmSer_WORK * (zone->employment_services<float>()) / 1000.0 * IndS + _BHOME_WORK * HOME + _BTTAUTO_WORK * (ttime_deflected) * ModAuto + _BTTTRAN_WORK * (ttime_deflected) * ModTran + _BTTOTHER_WORK * (ttime_deflected) * ModOth + thetag * _THETAG_WORK + thetam * _THETAM_WORK + thetar * _THETAR_WORK + thetas * _THETAS_WORK + thetai * _THETAI_WORK + thetao * _THETAO_WORK + _THETA_UR_WORK * theta_ur;
					if (zone->employment_total<int>() < 1.0) u = -9999999;
					if (ISNAN(u))
					{
						THROW_WARNING("WARNING: utility is not numeric, possible misspecification in utility function for destination choice. [Pop,emp,ttime]=, " << ttime_deflected);
					}
				}
				else if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::PICK_UP_OR_DROP_OFF_ACTIVITY)
				{
					u = ttime_deflected * _BTT_PICK+inc_diff * _BINCD_PICK+race_diff * _BRACED_PICK + _GAMMA_PICK * log(area_res * _BArRes_PICK+area_rec * _BArRec_PICK+area_ret * _BArRet_PICK+area_ent * _BArEnt_PICK+area_ins * _BArIns_PICK+area_off * _BArOff_PICK+area_mix * _BArMix_PICK+area_sch * _BArSch_PICK+emp_oth * _BEmOth_PICK+emp_ind * _BEmInd_PICK+emp_gov * _BEmGov_PICK+emp_srv * _BEmSer_PICK+emp_man * _BEmMan_PICK+emp_ret * _BEmRet_PICK + 0.000001) + thetag * _THETAG_PICK+thetam * _THETAM_PICK+thetar * _THETAR_PICK+thetas * _THETAS_PICK+thetai * _THETAI_PICK+thetao * _THETAO_PICK;
				}
				else if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::OTHER_WORK_ACTIVITY)
				{
					u = ttime_deflected * _BTT_OTHER_WORK+inc_diff * _BINCD_OTHER_WORK+race_diff * _BRACED_OTHER_WORK + _GAMMA_OTHER_WORK * log(area_res * _BArRes_OTHER_WORK+area_rec * _BArRec_OTHER_WORK+area_ret * _BArRet_OTHER_WORK+area_ent * _BArEnt_OTHER_WORK+area_ins * _BArIns_OTHER_WORK+area_off * _BArOff_OTHER_WORK+area_mix * _BArMix_OTHER_WORK+area_sch * _BArSch_OTHER_WORK+emp_oth * _BEmOth_OTHER_WORK+emp_ind * _BEmInd_OTHER_WORK+emp_gov * _BEmGov_OTHER_WORK+emp_srv * _BEmSer_OTHER_WORK+emp_man * _BEmMan_OTHER_WORK+emp_ret * _BEmRet_OTHER_WORK + 0.000001) + thetag * _THETAG_OTHER_WORK+thetam * _THETAM_OTHER_WORK+thetar * _THETAR_OTHER_WORK+thetas * _THETAS_OTHER_WORK+thetai * _THETAI_OTHER_WORK+thetao * _THETAO_OTHER_WORK;
				}
				else if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::SERVICE_VEHICLE_ACTIVITY)
				{
					u = ttime_deflected * _BTT_SERVICE+inc_diff * _BINCD_SERVICE+race_diff * _BRACED_SERVICE+ _GAMMA_SERVICE * log(area_res * _BArRes_SERVICE+area_rec * _BArRec_SERVICE+area_ret * _BArRet_SERVICE+area_ent * _BArEnt_SERVICE+area_ins * _BArIns_SERVICE+area_off * _BArOff_SERVICE+area_mix * _BArMix_SERVICE+area_sch * _BArSch_SERVICE+emp_oth * _BEmOth_SERVICE+emp_ind * _BEmInd_SERVICE+emp_gov * _BEmGov_SERVICE+emp_srv * _BEmSer_SERVICE+emp_man * _BEmMan_SERVICE+emp_ret * _BEmRet_SERVICE + 0.000001) + thetag * _THETAG_SERVICE+thetam * _THETAM_SERVICE+thetar * _THETAR_SERVICE+thetas * _THETAS_SERVICE+thetai * _THETAI_SERVICE+thetao * _THETAO_SERVICE;
				}
				else if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::OTHER_SHOPPING_ACTIVITY)
				{
					u = ttime_deflected * _BTT_MIN_SHOP+inc_diff * _BINCD_MIN_SHOP+race_diff * _BRACED_MIN_SHOP+ _GAMMA_MIN_SHOP * log(area_res * _BArRes_MIN_SHOP+area_rec * _BArRec_MIN_SHOP+area_ret * _BArRet_MIN_SHOP+area_ent * _BArEnt_MIN_SHOP+area_ins * _BArIns_MIN_SHOP+area_off * _BArOff_MIN_SHOP+area_mix * _BArMix_MIN_SHOP+area_sch * _BArSch_MIN_SHOP+emp_oth * _BEmOth_MIN_SHOP+emp_ind * _BEmInd_MIN_SHOP+emp_gov * _BEmGov_MIN_SHOP+emp_srv * _BEmSer_MIN_SHOP+emp_man * _BEmMan_MIN_SHOP+emp_ret * _BEmRet_MIN_SHOP + 0.000001) + thetag * _THETAG_MIN_SHOP+thetam * _THETAM_MIN_SHOP+thetar * _THETAR_MIN_SHOP+thetas * _THETAS_MIN_SHOP+thetai * _THETAI_MIN_SHOP+thetao * _THETAO_MIN_SHOP;
				}
				else if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::MAJOR_SHOPPING_ACTIVITY)
				{
					u = ttime_deflected * _BTT_MAJ_SHOP+inc_diff * _BINCD_MAJ_SHOP+race_diff * _BRACED_MAJ_SHOP + _GAMMA_MAJ_SHOP * log(area_res * _BArRes_MAJ_SHOP+area_rec * _BArRec_MAJ_SHOP+area_ret * _BArRet_MAJ_SHOP+area_ent * _BArEnt_MAJ_SHOP+area_ins * _BArIns_MAJ_SHOP+area_off * _BArOff_MAJ_SHOP+area_mix * _BArMix_MAJ_SHOP+area_sch * _BArSch_MAJ_SHOP+emp_oth * _BEmOth_MAJ_SHOP+emp_ind * _BEmInd_MAJ_SHOP+emp_gov * _BEmGov_MAJ_SHOP+emp_srv * _BEmSer_MAJ_SHOP+emp_man * _BEmMan_MAJ_SHOP+emp_ret * _BEmRet_MAJ_SHOP + 0.000001) + thetag * _THETAG_MAJ_SHOP+thetam * _THETAM_MAJ_SHOP+thetar * _THETAR_MAJ_SHOP+thetas * _THETAS_MAJ_SHOP+thetai * _THETAI_MAJ_SHOP+thetao * _THETAO_MAJ_SHOP;
				}
				else if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::EAT_OUT_ACTIVITY)
				{
					u = ttime_deflected * _BTT_EAT_OUT+inc_diff * _BINCD_EAT_OUT+race_diff * _BRACED_EAT_OUT + _GAMMA_EAT_OUT * log(area_res * _BArRes_EAT_OUT+area_rec * _BArRec_EAT_OUT+area_ret * _BArRet_EAT_OUT+area_ent * _BArEnt_EAT_OUT+area_ins * _BArIns_EAT_OUT+area_off * _BArOff_EAT_OUT+area_mix * _BArMix_EAT_OUT+area_sch * _BArSch_EAT_OUT+emp_oth * _BEmOth_EAT_OUT+emp_ind * _BEmInd_EAT_OUT+emp_gov * _BEmGov_EAT_OUT+emp_srv * _BEmSer_EAT_OUT+emp_man * _BEmMan_EAT_OUT+emp_ret * _BEmRet_EAT_OUT + 0.000001) + thetag * _THETAG_EAT_OUT+thetam * _THETAM_EAT_OUT+thetar * _THETAR_EAT_OUT+thetas * _THETAS_EAT_OUT+thetai * _THETAI_EAT_OUT+thetao * _THETAO_EAT_OUT;
				}
				else if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::RELIGIOUS_OR_CIVIC_ACTIVITY)
				{
					u = ttime_deflected * _BTT_CIVIC+inc_diff * _BINCD_CIVIC+race_diff * _BRACED_CIVIC + _GAMMA_CIVIC * log(area_res * _BArRes_CIVIC+area_rec * _BArRec_CIVIC+area_ret * _BArRet_CIVIC+area_ent * _BArEnt_CIVIC+area_ins * _BArIns_CIVIC+area_off * _BArOff_CIVIC+area_mix * _BArMix_CIVIC+area_sch * _BArSch_CIVIC+emp_oth * _BEmOth_CIVIC+emp_ind * _BEmInd_CIVIC+emp_gov * _BEmGov_CIVIC+emp_srv * _BEmSer_CIVIC+emp_man * _BEmMan_CIVIC+emp_ret * _BEmRet_CIVIC + 0.000001) + thetag * _THETAG_CIVIC+thetam * _THETAM_CIVIC+thetar * _THETAR_CIVIC+thetas * _THETAS_CIVIC+thetai * _THETAI_CIVIC+thetao * _THETAO_CIVIC;
				}
				else if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::LEISURE_ACTIVITY || _activity_type == Activity_Components::Types::ACTIVITY_TYPES::RECREATION_ACTIVITY)
				{
					u = ttime_deflected * _BTT_LEISURE+inc_diff * _BINCD_LEISURE+race_diff * _BRACED_LEISURE + _GAMMA_LEISURE * log(area_res * _BArRes_LEISURE+area_rec * _BArRec_LEISURE+area_ret * _BArRet_LEISURE+area_ent * _BArEnt_LEISURE+area_ins * _BArIns_LEISURE+area_off * _BArOff_LEISURE+area_mix * _BArMix_LEISURE+area_sch * _BArSch_LEISURE+emp_oth * _BEmOth_LEISURE+emp_ind * _BEmInd_LEISURE+emp_gov * _BEmGov_LEISURE+emp_srv * _BEmSer_LEISURE+emp_man * _BEmMan_LEISURE+emp_ret * _BEmRet_LEISURE + 0.000001) + thetag * _THETAG_LEISURE+thetam * _THETAM_LEISURE+thetar * _THETAR_LEISURE+thetas * _THETAS_LEISURE+thetai * _THETAI_LEISURE+thetao * _THETAO_LEISURE;
				}
				else if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::SOCIAL_ACTIVITY)
				{
					u = ttime_deflected * _BTT_SOCIAL+inc_diff * _BINCD_SOCIAL+race_diff * _BRACED_SOCIAL + _GAMMA_SOCIAL * log(area_res * _BArRes_SOCIAL+area_rec * _BArRec_SOCIAL+area_ret * _BArRet_SOCIAL+area_ent * _BArEnt_SOCIAL+area_ins * _BArIns_SOCIAL+area_off * _BArOff_SOCIAL+area_mix * _BArMix_SOCIAL+area_sch * _BArSch_SOCIAL+emp_oth * _BEmOth_SOCIAL+emp_ind * _BEmInd_SOCIAL+emp_gov * _BEmGov_SOCIAL+emp_srv * _BEmSer_SOCIAL+emp_man * _BEmMan_SOCIAL+emp_ret * _BEmRet_SOCIAL + 0.000001) + thetag * _THETAG_SOCIAL+thetam * _THETAM_SOCIAL+thetar * _THETAR_SOCIAL+thetas * _THETAS_SOCIAL+thetai * _THETAI_SOCIAL+thetao * _THETAO_SOCIAL;
				}
				else
				{
					u = ttime_deflected * _BTT_OTHER+inc_diff * _BINCD_OTHER+race_diff * _BRACED_OTHER + _GAMMA_OTHER * log(area_res * _BArRes_OTHER+area_rec * _BArRec_OTHER+area_ret * _BArRet_OTHER+area_ent * _BArEnt_OTHER+area_ins * _BArIns_OTHER+area_off * _BArOff_OTHER+area_mix * _BArMix_OTHER+area_sch * _BArSch_OTHER+emp_oth * _BEmOth_OTHER+emp_ind * _BEmInd_OTHER+emp_gov * _BEmGov_OTHER+emp_srv * _BEmSer_OTHER+emp_man * _BEmMan_OTHER+emp_ret * _BEmRet_OTHER + 0.000001) + thetag * _THETAG_OTHER+thetam * _THETAM_OTHER+thetar * _THETAR_OTHER+thetas * _THETAS_OTHER+thetai * _THETAI_OTHER+thetao * _THETAO_OTHER;
				}
				if (u > 100.0)
				{
					THROW_WARNING("WARNING: utility > 200.0 will cause numeric overflow, possible misspecification in utility function for destination choice. [Pop,emp,ttime]=, " << ttime_deflected);
				}
				if (ISNAN(u))
				{
					THROW_WARNING("WARNING: utility is not numeric, possible misspecification in utility function for destination choice. [Pop,emp,ttime]=, " << ttime_deflected);
					u = -999999.0;
				}
				
				cout << "Utility for zone " << zone->uuid<int>() << " = " << u <<" from origin zone " <<_previous->zone<_Zone_Interface*>()->uuid<int>()<< ": ";
				cout << ", ttime_deflected="<<ttime_deflected;
				cout << ", inc_diff ="<<inc_diff ;
				cout << ", race_diff ="<<race_diff ;
				cout << ", area_res ="<<area_res ;
				cout << ", area_rec ="<<area_rec ;
				cout << ", area_ret ="<<area_ret ;
				cout << ", area_ent ="<<area_ent ;
				cout << ", area_ins ="<<area_ins ;
				cout << ", area_off ="<<area_off ;
				cout << ", area_mix ="<<area_mix ;
				cout << ", area_sch ="<<area_sch ;
				cout << ", emp_oth ="<<emp_oth ;
				cout << ", emp_ind ="<<emp_ind ;
				cout << ", emp_gov ="<<emp_gov ;
				cout << ", emp_srv ="<<emp_srv ;
				cout << ", emp_man ="<<emp_man ;
				cout << ", emp_ret ="<<emp_ret ;
				cout << ",  thetag ="<< thetag ;
				cout << ", thetam ="<<thetam ;
				cout << ", thetar ="<<thetar ;
				cout << ", thetas ="<<thetas ;
				cout << ", thetai ="<<thetai ;
				cout << ", thetao ="<<thetao ;
				cout << endl;

				return (TargetType)u;				
			}
		};

		// INITIALIZE DESTINATION MODEL STATIC PARAMETERS
		#pragma region Choice option parameters	
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BTT_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BTT_PICK = -0.070956;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BINCD_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BINCD_PICK = -0.027821;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BRACED_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BRACED_PICK = -0.660094;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRes_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRes_PICK = 0.204766;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRec_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRec_PICK = 0.027283;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRet_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRet_PICK = 0.544154;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArEnt_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArEnt_PICK = 0.271475;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArIns_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArIns_PICK = 0.167705;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArOff_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArOff_PICK = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArMix_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArMix_PICK = 0.8175;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArSch_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArSch_PICK = 0.416465;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmOth_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmOth_PICK = 1;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmInd_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmInd_PICK = 0.320339;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmGov_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmGov_PICK = 0.867538;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmSer_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmSer_PICK = 0.765281;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmMan_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmMan_PICK = 0.516197;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmRet_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmRet_PICK = 0.62574;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAG_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAG_PICK = 0.137468;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAM_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAM_PICK = -0.516313;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAR_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAR_PICK = 0.027511;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAS_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAS_PICK = 0.011992;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAI_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAI_PICK = 0.08424;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAO_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAO_PICK = -0.30824;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(GAMMA_PICK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_GAMMA_PICK = 0.806092;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BTT_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BTT_OTHER_WORK = -0.028552;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BINCD_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BINCD_OTHER_WORK = 0.038861;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BRACED_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BRACED_OTHER_WORK = -0.025711;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRes_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRes_OTHER_WORK = 0.128388;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRec_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRec_OTHER_WORK = 0.000000322;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRet_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRet_OTHER_WORK = 1.028168;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArEnt_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArEnt_OTHER_WORK = 1.049801;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArIns_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArIns_OTHER_WORK = 0.236772;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArOff_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArOff_OTHER_WORK = 0.249596;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArMix_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArMix_OTHER_WORK = 0.47022;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArSch_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArSch_OTHER_WORK = 0.815026;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmOth_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmOth_OTHER_WORK = 1;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmInd_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmInd_OTHER_WORK = 0.250782;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmGov_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmGov_OTHER_WORK = 0.464937;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmSer_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmSer_OTHER_WORK = 0.768692;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmMan_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmMan_OTHER_WORK = 0.363034;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmRet_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmRet_OTHER_WORK = 0.419426;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAG_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAG_OTHER_WORK = 0.164304;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAM_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAM_OTHER_WORK = -0.225686;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAR_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAR_OTHER_WORK = -0.10824;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAS_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAS_OTHER_WORK = 0.048763;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAI_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAI_OTHER_WORK = -0.16035;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAO_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAO_OTHER_WORK = -0.333137;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(GAMMA_OTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_GAMMA_OTHER_WORK = 0.659908;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BTT_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BTT_SERVICE = -0.06;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BINCD_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BINCD_SERVICE = -0.07;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BRACED_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BRACED_SERVICE = -1.027;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRes_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRes_SERVICE = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRec_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRec_SERVICE = 0.016;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRet_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRet_SERVICE = 0.466;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArEnt_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArEnt_SERVICE = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArIns_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArIns_SERVICE = 0.061;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArOff_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArOff_SERVICE = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArMix_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArMix_SERVICE = 0.341;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArSch_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArSch_SERVICE = 0.348;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmOth_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmOth_SERVICE = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmInd_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmInd_SERVICE = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmGov_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmGov_SERVICE = 0.527;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmSer_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmSer_SERVICE = 0.966;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmMan_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmMan_SERVICE = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmRet_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmRet_SERVICE = 1;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAG_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAG_SERVICE = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAM_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAM_SERVICE = -0.407;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAR_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAR_SERVICE = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAS_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAS_SERVICE = 0.029;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAI_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAI_SERVICE = -0.126;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAO_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAO_SERVICE = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(GAMMA_SERVICE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_GAMMA_SERVICE = 0.635;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRec_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRec_MIN_SHOP = 0.011;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BTT_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BTT_MIN_SHOP = -0.075;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BINCD_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BINCD_MIN_SHOP = -0.027;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BRACED_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BRACED_MIN_SHOP = -0.844;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRes_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRes_MIN_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRec_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRec_MAJ_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRet_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRet_MIN_SHOP = 4.14;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArEnt_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArEnt_MIN_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArIns_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArIns_MIN_SHOP = 0.033;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArOff_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArOff_MIN_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArMix_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArMix_MIN_SHOP = 0.075;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArSch_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArSch_MIN_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmOth_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmOth_MIN_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmInd_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmInd_MIN_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmGov_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmGov_MIN_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmSer_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmSer_MIN_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmMan_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmMan_MIN_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmRet_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmRet_MIN_SHOP = 1;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAG_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAG_MIN_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAM_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAM_MIN_SHOP = -0.326;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAR_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAR_MIN_SHOP = 0.191;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAS_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAS_MIN_SHOP = -0.032;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAI_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAI_MIN_SHOP = -0.385;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAO_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAO_MIN_SHOP = 0.4;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(GAMMA_MIN_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_GAMMA_MIN_SHOP = 0.434;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BTT_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BTT_MAJ_SHOP = -0.062;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BINCD_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BINCD_MAJ_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BRACED_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BRACED_MAJ_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRes_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRes_MAJ_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRec_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRec_EAT_OUT = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRet_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRet_MAJ_SHOP = 4.475;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArEnt_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArEnt_MAJ_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArIns_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArIns_MAJ_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArOff_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArOff_MAJ_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArMix_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArMix_MAJ_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArSch_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArSch_MAJ_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmOth_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmOth_MAJ_SHOP = 1;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmInd_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmInd_MAJ_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmGov_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmGov_MAJ_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmSer_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmSer_MAJ_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmMan_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmMan_MAJ_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmRet_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmRet_MAJ_SHOP = 1.085;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAG_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAG_MAJ_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAM_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAM_MAJ_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAR_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAR_MAJ_SHOP = 0.355;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAS_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAS_MAJ_SHOP = -0.055;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAI_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAI_MAJ_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAO_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAO_MAJ_SHOP = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(GAMMA_MAJ_SHOP) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_GAMMA_MAJ_SHOP = 0.381;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BTT_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BTT_EAT_OUT = -0.067;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BINCD_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BINCD_EAT_OUT = -0.056;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BRACED_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BRACED_EAT_OUT = -1.139;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRes_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRes_EAT_OUT = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRec_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRec_CIVIC = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRet_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRet_EAT_OUT = 4.241;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArEnt_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArEnt_EAT_OUT = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArIns_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArIns_EAT_OUT = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArOff_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArOff_EAT_OUT = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArMix_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArMix_EAT_OUT = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArSch_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArSch_EAT_OUT = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmOth_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmOth_EAT_OUT = 1;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmInd_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmInd_EAT_OUT = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmGov_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmGov_EAT_OUT = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmSer_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmSer_EAT_OUT = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmMan_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmMan_EAT_OUT = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmRet_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmRet_EAT_OUT = 2.815;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAG_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAG_EAT_OUT = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAM_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAM_EAT_OUT = -0.296;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAR_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAR_EAT_OUT = -0.223;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAS_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAS_EAT_OUT = 0.056;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAI_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAI_EAT_OUT = -0.255;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAO_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAO_EAT_OUT = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(GAMMA_EAT_OUT) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_GAMMA_EAT_OUT = 0.274;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BTT_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BTT_CIVIC = -0.076;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BINCD_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BINCD_CIVIC = -0.092;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BRACED_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BRACED_CIVIC = -2.009;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRes_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRes_CIVIC = 0.108;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRec_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRec_LEISURE = 0.017;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRet_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRet_CIVIC = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArEnt_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArEnt_CIVIC = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArIns_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArIns_CIVIC = 0.028;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArOff_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArOff_CIVIC = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArMix_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArMix_CIVIC = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArSch_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArSch_CIVIC = 0.986;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmOth_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmOth_CIVIC = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmInd_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmInd_CIVIC = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmGov_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmGov_CIVIC = 1.547;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmSer_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmSer_CIVIC = 1;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmMan_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmMan_CIVIC = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmRet_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmRet_CIVIC = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAG_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAG_CIVIC = 0.201;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAM_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAM_CIVIC = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAR_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAR_CIVIC = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAS_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAS_CIVIC = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAI_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAI_CIVIC = -0.711;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAO_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAO_CIVIC = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(GAMMA_CIVIC) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_GAMMA_CIVIC = 0.648;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BTT_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BTT_LEISURE = -0.062;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BINCD_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BINCD_LEISURE = -0.046;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BRACED_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BRACED_LEISURE = -1.325;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRes_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRes_LEISURE = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRec_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRec_SOCIAL = 0.043;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRet_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRet_LEISURE = 0.621;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArEnt_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArEnt_LEISURE = 2.285;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArIns_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArIns_LEISURE = 0.035;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArOff_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArOff_LEISURE = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArMix_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArMix_LEISURE = 0.712;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArSch_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArSch_LEISURE = 1;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmOth_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmOth_LEISURE = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmInd_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmInd_LEISURE = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmGov_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmGov_LEISURE = 0.066;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmSer_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmSer_LEISURE = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmMan_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmMan_LEISURE = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmRet_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmRet_LEISURE = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAG_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAG_LEISURE = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAM_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAM_LEISURE = -0.42;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAR_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAR_LEISURE = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAS_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAS_LEISURE = 0.093;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAI_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAI_LEISURE = -0.619;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAO_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAO_LEISURE = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(GAMMA_LEISURE) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_GAMMA_LEISURE = 0.582;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BTT_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BTT_SOCIAL = -0.059;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BINCD_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BINCD_SOCIAL = -0.058;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BRACED_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BRACED_SOCIAL = -0.969;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRes_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRes_SOCIAL = 0.091;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRec_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRec_OTHER = 0.490179;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRet_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRet_SOCIAL = 0.491;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArEnt_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArEnt_SOCIAL = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArIns_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArIns_SOCIAL = 0.15;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArOff_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArOff_SOCIAL = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArMix_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArMix_SOCIAL = 0.717;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArSch_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArSch_SOCIAL = 0.305;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmOth_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmOth_SOCIAL = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmInd_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmInd_SOCIAL = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmGov_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmGov_SOCIAL = 1.403;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmSer_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmSer_SOCIAL = 1;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmMan_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmMan_SOCIAL = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmRet_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmRet_SOCIAL = 0.942;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAG_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAG_SOCIAL = 0.19;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAM_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAM_SOCIAL = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAR_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAR_SOCIAL = 0.285;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAS_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAS_SOCIAL = 0.051;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAI_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAI_SOCIAL = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAO_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAO_SOCIAL = -1.228;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(GAMMA_SOCIAL) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_GAMMA_SOCIAL = 0.979;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BTT_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BTT_OTHER = -0.150126;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BINCD_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BINCD_OTHER = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BRACED_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BRACED_OTHER = -0.297174;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRes_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRes_OTHER = 1.021581;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRet_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRet_OTHER = 6.69056;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArEnt_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArEnt_OTHER = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArIns_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArIns_OTHER = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArOff_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArOff_OTHER = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArMix_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArMix_OTHER = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArSch_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArSch_OTHER = 0.753482;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmOth_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmOth_OTHER = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmInd_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmInd_OTHER = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmGov_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmGov_OTHER = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmSer_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmSer_OTHER = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmMan_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmMan_OTHER = 1;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmRet_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmRet_OTHER = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAG_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAG_OTHER = -0.014155;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAM_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAM_OTHER = -1.168986;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAR_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAR_OTHER = -0.339726;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAS_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAS_OTHER = 0.006282;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAI_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAI_OTHER = 0.331496;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAO_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAO_OTHER = 0.732551;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(GAMMA_OTHER) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_GAMMA_OTHER = 1;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BTTAUTO_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BTTAUTO_WORK = -0.11;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BTTTRAN_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BTTTRAN_WORK = -0.0677;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BTTOTHER_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BTTOTHER_WORK = -0.0386;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BHOME_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BHOME_WORK = 2.23;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BINCD_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BINCD_WORK = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BRACED_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BRACED_WORK = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRes_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRes_WORK = -0.0304;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRec_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRec_WORK = 0.0218;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArRet_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArRet_WORK = 0.0287;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArEnt_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArEnt_WORK = 0.0443;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArIns_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArIns_WORK = 0.0539;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArOff_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArOff_WORK = -0.00637;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArMix_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArMix_WORK = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BArSch_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BArSch_WORK = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmOth_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmOth_WORK = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmInd_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmInd_WORK = 0;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmGov_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmGov_WORK = 0.304;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmSer_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmSer_WORK = 0.108;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmMan_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmMan_WORK = 0.438;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmRet_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmRet_WORK = 0.476;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(BEmUnrelated_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_BEmUnrelated_WORK = 0.0404;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAG_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAG_WORK = 0.0734;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAM_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAM_WORK = 0.184;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAR_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAR_WORK = -0.0266;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAS_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAS_WORK = 0.00686;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAI_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAI_WORK = 0.22;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETAO_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETAO_WORK = 0.244;
		template<typename MasterType, typename InheritanceList> typename Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType, InheritanceList>::type_of(THETA_UR_WORK) Person_Components::Implementations::ADAPTS_Destination_Choice_Option<MasterType,InheritanceList>::_THETA_UR_WORK = -0.00831;

		#pragma endregion


		implementation struct Destination_Choice_Model_Implementation : public Choice_Model_Components::Implementations::MNL_Model_Implementation<MasterType, INHERIT(Destination_Choice_Model_Implementation)>
		{
			typedef Choice_Model_Components::Implementations::MNL_Model_Implementation<MasterType, INHERIT(Destination_Choice_Model_Implementation)> BaseType;
			typedef typename BaseType::Component_Type ComponentType;
			typedef TypeList<Prototypes::Destination_Choice_Option<typename MasterType::person_destination_choice_option_type >> TList;
		};



		implementation struct ADAPTS_Destination_Chooser_Implementation : public Polaris_Component<MasterType,INHERIT(ADAPTS_Destination_Chooser_Implementation),Data_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(ADAPTS_Destination_Chooser_Implementation),Data_Object>::Component_Type ComponentType;

			// Pointer to the Parent class
			m_prototype(Prototypes::Person_Planner< typename MasterType::person_planner_type>, Parent_Planner, NONE, NONE);
			m_prototype(Choice_Model_Components::Prototypes::Choice_Model< Destination_Choice_Model_Implementation<MasterType>>, Choice_Model, NONE, NONE);
			m_prototype(Activity_Components::Prototypes::Activity_Planner< typename MasterType::activity_type>, Current_Activity, NONE, NONE);
			
			static m_data(int, choice_set_size, NONE, NONE);

			// Interface definitions
			typedef Choice_Model_Components::Prototypes::Choice_Model<Destination_Choice_Model_Implementation<MasterType> > _Choice_Model_Interface;
			typedef Prototypes::Destination_Choice_Option<typename MasterType::person_destination_choice_option_type> _Choice_Option_Interface;

			typedef Prototypes::Person<typename type_of(Parent_Planner)::type_of(Parent_Person)> person_itf;
			typedef Prototypes::Person_Scheduler<typename person_itf::get_type_of(Scheduling_Faculty)> scheduler_itf;
			typedef Scenario_Components::Prototypes::Scenario< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference)> _Scenario_Interface;
			typedef Network_Components::Prototypes::Network< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference)> _Network_Interface;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _Network_Interface::get_type_of(skimming_faculty)> _Skim_Interface;

			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(_activity_locations_container_itf)>  _Activity_Location_Interface;	
				
			typedef Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links)> _Links_Container_Interface;
			typedef Link_Components::Prototypes::Link<typename get_component_type(_links_container_itf)>  _Link_Interface;
	
			typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
			typedef Zone_Components::Prototypes::Zone<typename get_mapped_component_type(_zones_container_itf)>  _Zone_Interface;

			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(zone_ids_container),int> _Zone_Ids_Interface;

			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Activity_Container)> Activity_Plans;
			typedef Activity_Components::Prototypes::Activity_Planner<typename get_component_type(Activity_Plans)> Activity_Plan;
			
			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Movement_Plans_Container)> Movement_Plans;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename get_component_type(Movement_Plans)> Movement_Plan;
			

			template<typename TargetType> void Initialize(/*requires(TargetType,check(typename ComponentType::Parent_Type,Concepts::Is_Person))*/)
			{	
			}

			template<typename ActivityItfType, typename ReturnType> ReturnType Choose_Destination(ActivityItfType activity, boost::container::vector<ReturnType>* destinations_to_use=nullptr)
			{
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
				
				// create local choice model
				Destination_Choice_Model_Implementation<MasterType> a;
				_Choice_Model_Interface* choice_model = (_Choice_Model_Interface*)&a;

				// set the current activity from input
				this->_Current_Activity = (Current_Activity_interface*)activity;

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();

				// selecte locations to choose from - use all if destinations to use not specified
				_Activity_Locations_Container_Interface* locations;
				if (destinations_to_use == nullptr) locations= network->template activity_locations_container<_Activity_Locations_Container_Interface*>();
				else locations = (_Activity_Locations_Container_Interface*)destinations_to_use;

				_Skim_Interface* LOS = network->template skimming_faculty<_Skim_Interface*>();


				// Create choice set
				boost::container::vector<_Choice_Option_Interface*> loc_options;
				fill_choice_boost::container::set<ReturnType>(locations,loc_options,choice_model);

				// Make choice
				int selected_index = 0;
				choice_model->template Evaluate_Choices<NT>();

				// Get interface to chosen option
				_Choice_Option_Interface* selected = choice_model->template Choose<_Choice_Option_Interface*>(selected_index);

				// Validate the return value
				ReturnType return_ptr = nullptr;
				if (selected == nullptr) 
				{
					//THROW_WARNING("WARNING: selected is null - no destination choice made, index = " << selected_index <<", prev/next="<<prev->template zone<_Zone_Interface*>()->uuid<int>() << "/"<<next->template zone<_Zone_Interface*>()->uuid<int>());
					cout << "WARNING: selected is null - no destination choice made, index = " << selected_index << " of " << loc_options.size() << " options, utility values for options: "<<endl;
					for (int i=0; i<loc_options.size(); ++i)
					{
						loc_options[i]->Print_Utility<float>();
					}
					cout << endl;
				
				}
				else return_ptr = choice_model->template Choice_At<_Choice_Option_Interface*>(selected_index)->template destination<ReturnType>();

				// free memory allocated locally
				for (int i = 0; i < loc_options.size(); i++) Free<typename _Choice_Option_Interface::Component_Type>((typename _Choice_Option_Interface::Component_Type*)loc_options[i]);

				return return_ptr;
			}

			template<typename TargetType> TargetType Choose_Routine_Destination(Activity_Components::Types::ACTIVITY_TYPES act_type, boost::container::vector<TargetType>* destinations_to_use=nullptr)
			{
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
				
				// create local choice model
				Destination_Choice_Model_Implementation<MasterType> a;
				_Choice_Model_Interface* choice_model = (_Choice_Model_Interface*)&a;


				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();

				// selecte locations to choose from - use all if destinations to use not specified
				_Activity_Locations_Container_Interface* locations;
				if (destinations_to_use == nullptr) locations= network->template activity_locations_container<_Activity_Locations_Container_Interface*>();
				else locations = (_Activity_Locations_Container_Interface*)destinations_to_use;

				_Skim_Interface* LOS = network->template skimming_faculty<_Skim_Interface*>();


				// Create choice set
				boost::container::vector<_Choice_Option_Interface*> loc_options;
				fill_routine_choice_boost::container::set<TargetType>(act_type, locations,loc_options,choice_model);

				// Make choice
				int selected_index = 0;
				choice_model->template Evaluate_Choices<NT>();

				// Get interface to chosen option
				_Choice_Option_Interface* selected = choice_model->template Choose<_Choice_Option_Interface*>(selected_index);

				// Validate the return value
				TargetType return_ptr = nullptr;
				if (selected == nullptr) 
				{
					//THROW_WARNING("WARNING: selected is null - no destination choice made, index = " << selected_index <<", prev/next="<<prev->template zone<_Zone_Interface*>()->uuid<int>() << "/"<<next->template zone<_Zone_Interface*>()->uuid<int>());
					/*cout << "WARNING: selected is null - no destination choice made, index = " << selected_index << " of " << loc_options.size() << " options, utility values for options: "<<endl;
					for (int i=0; i<loc_options.size(); ++i)
					{
						loc_options[i]->Print_Utility<float>();
					}
					cout << endl;*/
				
				}
				else return_ptr = choice_model->template Choice_At<_Choice_Option_Interface*>(selected_index)->template destination<TargetType>();

				// free memory allocated locally
				for (int i = 0; i < loc_options.size(); i++) Free<typename _Choice_Option_Interface::Component_Type>((typename _Choice_Option_Interface::Component_Type*)loc_options[i]);

				return return_ptr;
			}

			template<typename TargetType> void fill_choice_set(_Activity_Locations_Container_Interface* available_set, boost::container::vector<_Choice_Option_Interface*>& choice_set, _Choice_Model_Interface* choice_model, requires(TargetType,check(strip_modifiers(TargetType),is_pointer) && check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				// Get person context and system knowledge
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
				scheduler_itf* scheduler = _Parent_Person->Scheduling_Faculty<scheduler_itf*>();
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();
				_Skim_Interface* LOS = network->template skimming_faculty<_Skim_Interface*>();


				// Get preceding and following activities based on start time, otherwise assume plan a new tour startinga and ending at home
				Current_Activity_interface* prev_act, *next_act;
				_Activity_Location_Interface* prev_loc, *next_loc;
				bool restrict_choice_set = true;
				if (this->_Current_Activity->Start_Is_Planned<bool>())
				{
					prev_act = scheduler->previous_activity_plan<Time_Seconds,Current_Activity_interface*>(this->_Current_Activity->Start_Time<Time_Seconds>());
					next_act = scheduler->previous_activity_plan<Time_Seconds,Current_Activity_interface*>(this->_Current_Activity->Start_Time<Time_Seconds>());
					// check previous act, if it is not known or if its location is not know, do not restrict current choice set
					if (prev_act == nullptr)
					{
						prev_loc = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
						restrict_choice_set = false;
					}
					else if (prev_act->Location_Is_Planned<bool>())
					{
						prev_loc = prev_act->Location<_Activity_Location_Interface*>();
						restrict_choice_set = true;
					}
					else
					{
						prev_loc = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
						restrict_choice_set = false;
					}
					// check next act, if it is not known or if its location is not know, do not restrict current choice set
					if (next_act == nullptr)
					{
						next_loc = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
						restrict_choice_set = false;
					}
					else if(next_act->Location_Is_Planned<bool>())
					{
						next_loc = next_act->Location<_Activity_Location_Interface*>();
						restrict_choice_set = true;
					}
					else
					{
						next_loc = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
						restrict_choice_set = false;
					}
				}
				else
				{
					prev_act = nullptr;
					next_act = nullptr;
					prev_loc = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
					next_loc = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
				}
				

				// variables used for utility calculation
				const int size = (int)available_set->size();
				int loc_id;


				// select zones to choose from and estimate utility
				for (int i=0; i<_choice_set_size; i++)
				{
					loc_id = (int)((Uniform_RNG.template Next_Rand<float>()*0.999999)*size);
					_Activity_Location_Interface* loc = available_set->at(loc_id);
					
					_Choice_Option_Interface* choice = (_Choice_Option_Interface*)Allocate<typename MasterType::person_destination_choice_option_type>();
					choice->template previous<_Activity_Location_Interface*>(prev_loc);
					choice->template destination<_Activity_Location_Interface*>(loc);
					choice->template next<_Activity_Location_Interface*>(next_loc);
					choice->template activity_type<Activity_Components::Types::ACTIVITY_TYPES>(this->_Current_Activity->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>());
					choice->template Parent_Planner<Parent_Planner_interface*>(_Parent_Planner);
					choice_model->template Add_Choice_Option<_Choice_Option_Interface*>(choice);
					choice_set.push_back(choice);
				}
			}

			template<typename TargetType> void fill_routine_choice_set(Activity_Components::Types::ACTIVITY_TYPES act_type, _Activity_Locations_Container_Interface* available_set, boost::container::vector<_Choice_Option_Interface*>& choice_set, _Choice_Model_Interface* choice_model, requires(TargetType,check(strip_modifiers(TargetType),is_pointer) && check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				// Get person context and system knowledge
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
				scheduler_itf* scheduler = _Parent_Person->Scheduling_Faculty<scheduler_itf*>();
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();
				_Zone_Ids_Interface& zone_ids = network->template zone_ids_container<_Zone_Ids_Interface&>();
				_Skim_Interface* LOS = network->template skimming_faculty<_Skim_Interface*>();
				_Zones_Container_Interface::iterator zone_itr;

				// Get preceding and following activities based on start time, otherwise assume plan a new tour startinga and ending at home
				Current_Activity_interface* prev_act, *next_act;
				_Activity_Location_Interface* prev_loc, *next_loc;
				bool restrict_choice_set = true;

				prev_act = nullptr;
				next_act = nullptr;
				prev_loc = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
				next_loc = _Parent_Person->Home_Location<_Activity_Location_Interface*>();


				// select zones to choose from and estimate utility 
				for (zone_itr = zones->begin(); zone_itr != zones->end(); ++zone_itr)
				{
					// First choose a random zone from the zone boost::container::list
					_Zone_Interface* zone = (_Zone_Interface*)zone_itr->second; //network->get_random_zone<_Zone_Interface*>();

					// ignore zone if all employment slots have already been assigned to other agents
					if (act_type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY && zone->employment_simulated<int>() >= zone->employment_total<int>()) continue;

					// Get random location within that zone
					_Activity_Location_Interface* loc = zone->Get_Random_Location<_Activity_Location_Interface*>();

					// try to add a random suitable location up to ten times, if failed 10 times then ignore zone
					int failed_attempts = 0;
					while (failed_attempts < 10 && (act_type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY && !loc->is_work_location<bool>()) || (act_type == Activity_Components::Types::ACTIVITY_TYPES::SCHOOL_ACTIVITY && !loc->is_school_location<bool>()))
					{
						loc = zone->Get_Random_Location<_Activity_Location_Interface*>();
						failed_attempts++;
					}
					if (failed_attempts >= 10) continue;

					_Choice_Option_Interface* choice = (_Choice_Option_Interface*)Allocate<typename MasterType::person_destination_choice_option_type>();
					choice->template previous<_Activity_Location_Interface*>(prev_loc);
					choice->template destination<_Activity_Location_Interface*>(loc);
					choice->template next<_Activity_Location_Interface*>(next_loc);
					choice->template activity_type<Activity_Components::Types::ACTIVITY_TYPES>(act_type);
					choice->template Parent_Planner<Parent_Planner_interface*>(_Parent_Planner);
					choice_model->template Add_Choice_Option<_Choice_Option_Interface*>(choice);
					choice_set.push_back(choice);
				}
			}
		};
		#pragma region Choice option parameters
		// INITIALIZE DESTINATION MODEL STATIC PARAMETERS
		template<typename MasterType, typename InheritanceList> typename ADAPTS_Destination_Chooser_Implementation<MasterType, InheritanceList>::type_of(choice_set_size) ADAPTS_Destination_Chooser_Implementation<MasterType,InheritanceList>::_choice_set_size;
		#pragma endregion

	}
}


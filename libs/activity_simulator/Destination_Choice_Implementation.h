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

		implementation struct ADAPTS_Destination_Choice_Option : public Choice_Model_Components::Implementations::Choice_Option_Base<MasterType,INHERIT(ADAPTS_Destination_Choice_Option)>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(ADAPTS_Destination_Choice_Option),Data_Object>::Component_Type ComponentType;


			// Pointer to the Parent class
			m_prototype(Prototypes::Person_Planner, typename MasterType::person_planner_type, Parent_Planner, NONE, NONE);

			// PARAMETER DECLARATIONS
			#pragma region DESTINATION CHOICE MODEL PARAMETERS
			m_static_data(float, BTT_PICK, NONE, NONE);		m_static_data(float, BTT_OTHER_WORK, NONE, NONE);			m_static_data(float, BTT_SERVICE, NONE, NONE);			m_static_data(float, BTT_MIN_SHOP, NONE, NONE);		m_static_data(float, BTT_MAJ_SHOP, NONE, NONE);		m_static_data(float, BTT_EAT_OUT, NONE, NONE);			m_static_data(float, BTT_CIVIC, NONE, NONE);			m_static_data(float, BTT_LEISURE, NONE, NONE);			m_static_data(float, BTT_SOCIAL, NONE, NONE);			m_static_data(float, BTT_OTHER, NONE, NONE);			m_static_data(float, BTTAUTO_WORK, NONE, NONE); m_static_data(float, BTTTRAN_WORK, NONE, NONE); m_static_data(float, BTTOTHER_WORK, NONE, NONE);
			m_static_data(float, BINCD_PICK, NONE, NONE);		m_static_data(float, BINCD_OTHER_WORK, NONE, NONE);		m_static_data(float, BINCD_SERVICE, NONE, NONE);		m_static_data(float, BINCD_MIN_SHOP, NONE, NONE);		m_static_data(float, BINCD_MAJ_SHOP, NONE, NONE);		m_static_data(float, BINCD_EAT_OUT, NONE, NONE);		m_static_data(float, BINCD_CIVIC, NONE, NONE);			m_static_data(float, BINCD_LEISURE, NONE, NONE);		m_static_data(float, BINCD_SOCIAL, NONE, NONE);		m_static_data(float, BINCD_OTHER, NONE, NONE);			m_static_data(float, BINCD_WORK, NONE, NONE); m_static_data(float, BHOME_WORK, NONE, NONE);
			m_static_data(float, BRACED_PICK, NONE, NONE);		m_static_data(float, BRACED_OTHER_WORK, NONE, NONE);		m_static_data(float, BRACED_SERVICE, NONE, NONE);		m_static_data(float, BRACED_MIN_SHOP, NONE, NONE);		m_static_data(float, BRACED_MAJ_SHOP, NONE, NONE);		m_static_data(float, BRACED_EAT_OUT, NONE, NONE);		m_static_data(float, BRACED_CIVIC, NONE, NONE);		m_static_data(float, BRACED_LEISURE, NONE, NONE);		m_static_data(float, BRACED_SOCIAL, NONE, NONE);		m_static_data(float, BRACED_OTHER, NONE, NONE);		m_static_data(float, BRACED_WORK, NONE, NONE);
			m_static_data(float, BArRes_PICK, NONE, NONE);		m_static_data(float, BArRes_OTHER_WORK, NONE, NONE);		m_static_data(float, BArRes_SERVICE, NONE, NONE);		m_static_data(float, BArRes_MIN_SHOP, NONE, NONE);		m_static_data(float, BArRes_MAJ_SHOP, NONE, NONE);		m_static_data(float, BArRes_EAT_OUT, NONE, NONE);		m_static_data(float, BArRes_CIVIC, NONE, NONE);		m_static_data(float, BArRes_LEISURE, NONE, NONE);		m_static_data(float, BArRes_SOCIAL, NONE, NONE);		m_static_data(float, BArRes_OTHER, NONE, NONE);		m_static_data(float, BArRes_WORK, NONE, NONE);
			m_static_data(float, BArRec_PICK, NONE, NONE);		m_static_data(float, BArRec_OTHER_WORK, NONE, NONE);		m_static_data(float, BArRec_SERVICE, NONE, NONE);		m_static_data(float, BArRec_MIN_SHOP, NONE, NONE);		m_static_data(float, BArRec_MAJ_SHOP, NONE, NONE);		m_static_data(float, BArRec_EAT_OUT, NONE, NONE);		m_static_data(float, BArRec_CIVIC, NONE, NONE);		m_static_data(float, BArRec_LEISURE, NONE, NONE);		m_static_data(float, BArRec_SOCIAL, NONE, NONE);		m_static_data(float, BArRec_OTHER, NONE, NONE);		m_static_data(float, BArRec_WORK, NONE, NONE);
			m_static_data(float, BArRet_PICK, NONE, NONE);		m_static_data(float, BArRet_OTHER_WORK, NONE, NONE);		m_static_data(float, BArRet_SERVICE, NONE, NONE);		m_static_data(float, BArRet_MIN_SHOP, NONE, NONE);		m_static_data(float, BArRet_MAJ_SHOP, NONE, NONE);		m_static_data(float, BArRet_EAT_OUT, NONE, NONE);		m_static_data(float, BArRet_CIVIC, NONE, NONE);		m_static_data(float, BArRet_LEISURE, NONE, NONE);		m_static_data(float, BArRet_SOCIAL, NONE, NONE);		m_static_data(float, BArRet_OTHER, NONE, NONE);		m_static_data(float, BArRet_WORK, NONE, NONE);
			m_static_data(float, BArEnt_PICK, NONE, NONE);		m_static_data(float, BArEnt_OTHER_WORK, NONE, NONE);		m_static_data(float, BArEnt_SERVICE, NONE, NONE);		m_static_data(float, BArEnt_MIN_SHOP, NONE, NONE);		m_static_data(float, BArEnt_MAJ_SHOP, NONE, NONE);		m_static_data(float, BArEnt_EAT_OUT, NONE, NONE);		m_static_data(float, BArEnt_CIVIC, NONE, NONE);		m_static_data(float, BArEnt_LEISURE, NONE, NONE);		m_static_data(float, BArEnt_SOCIAL, NONE, NONE);		m_static_data(float, BArEnt_OTHER, NONE, NONE);		m_static_data(float, BArEnt_WORK, NONE, NONE);
			m_static_data(float, BArIns_PICK, NONE, NONE);		m_static_data(float, BArIns_OTHER_WORK, NONE, NONE);		m_static_data(float, BArIns_SERVICE, NONE, NONE);		m_static_data(float, BArIns_MIN_SHOP, NONE, NONE);		m_static_data(float, BArIns_MAJ_SHOP, NONE, NONE);		m_static_data(float, BArIns_EAT_OUT, NONE, NONE);		m_static_data(float, BArIns_CIVIC, NONE, NONE);		m_static_data(float, BArIns_LEISURE, NONE, NONE);		m_static_data(float, BArIns_SOCIAL, NONE, NONE);		m_static_data(float, BArIns_OTHER, NONE, NONE);		m_static_data(float, BArIns_WORK, NONE, NONE);
			m_static_data(float, BArOff_PICK, NONE, NONE);		m_static_data(float, BArOff_OTHER_WORK, NONE, NONE);		m_static_data(float, BArOff_SERVICE, NONE, NONE);		m_static_data(float, BArOff_MIN_SHOP, NONE, NONE);		m_static_data(float, BArOff_MAJ_SHOP, NONE, NONE);		m_static_data(float, BArOff_EAT_OUT, NONE, NONE);		m_static_data(float, BArOff_CIVIC, NONE, NONE);		m_static_data(float, BArOff_LEISURE, NONE, NONE);		m_static_data(float, BArOff_SOCIAL, NONE, NONE);		m_static_data(float, BArOff_OTHER, NONE, NONE);		m_static_data(float, BArOff_WORK, NONE, NONE);
			m_static_data(float, BArMix_PICK, NONE, NONE);		m_static_data(float, BArMix_OTHER_WORK, NONE, NONE);		m_static_data(float, BArMix_SERVICE, NONE, NONE);		m_static_data(float, BArMix_MIN_SHOP, NONE, NONE);		m_static_data(float, BArMix_MAJ_SHOP, NONE, NONE);		m_static_data(float, BArMix_EAT_OUT, NONE, NONE);		m_static_data(float, BArMix_CIVIC, NONE, NONE);		m_static_data(float, BArMix_LEISURE, NONE, NONE);		m_static_data(float, BArMix_SOCIAL, NONE, NONE);		m_static_data(float, BArMix_OTHER, NONE, NONE);		m_static_data(float, BArMix_WORK, NONE, NONE);
			m_static_data(float, BArSch_PICK, NONE, NONE);		m_static_data(float, BArSch_OTHER_WORK, NONE, NONE);		m_static_data(float, BArSch_SERVICE, NONE, NONE);		m_static_data(float, BArSch_MIN_SHOP, NONE, NONE);		m_static_data(float, BArSch_MAJ_SHOP, NONE, NONE);		m_static_data(float, BArSch_EAT_OUT, NONE, NONE);		m_static_data(float, BArSch_CIVIC, NONE, NONE);		m_static_data(float, BArSch_LEISURE, NONE, NONE);		m_static_data(float, BArSch_SOCIAL, NONE, NONE);		m_static_data(float, BArSch_OTHER, NONE, NONE);		m_static_data(float, BArSch_WORK, NONE, NONE);
			m_static_data(float, BEmOth_PICK, NONE, NONE);		m_static_data(float, BEmOth_OTHER_WORK, NONE, NONE);		m_static_data(float, BEmOth_SERVICE, NONE, NONE);		m_static_data(float, BEmOth_MIN_SHOP, NONE, NONE);		m_static_data(float, BEmOth_MAJ_SHOP, NONE, NONE);		m_static_data(float, BEmOth_EAT_OUT, NONE, NONE);		m_static_data(float, BEmOth_CIVIC, NONE, NONE);		m_static_data(float, BEmOth_LEISURE, NONE, NONE);		m_static_data(float, BEmOth_SOCIAL, NONE, NONE);		m_static_data(float, BEmOth_OTHER, NONE, NONE);		m_static_data(float, BEmOth_WORK, NONE, NONE);
			m_static_data(float, BEmInd_PICK, NONE, NONE);		m_static_data(float, BEmInd_OTHER_WORK, NONE, NONE);		m_static_data(float, BEmInd_SERVICE, NONE, NONE);		m_static_data(float, BEmInd_MIN_SHOP, NONE, NONE);		m_static_data(float, BEmInd_MAJ_SHOP, NONE, NONE);		m_static_data(float, BEmInd_EAT_OUT, NONE, NONE);		m_static_data(float, BEmInd_CIVIC, NONE, NONE);		m_static_data(float, BEmInd_LEISURE, NONE, NONE);		m_static_data(float, BEmInd_SOCIAL, NONE, NONE);		m_static_data(float, BEmInd_OTHER, NONE, NONE);		m_static_data(float, BEmInd_WORK, NONE, NONE);
			m_static_data(float, BEmGov_PICK, NONE, NONE);		m_static_data(float, BEmGov_OTHER_WORK, NONE, NONE);		m_static_data(float, BEmGov_SERVICE, NONE, NONE);		m_static_data(float, BEmGov_MIN_SHOP, NONE, NONE);		m_static_data(float, BEmGov_MAJ_SHOP, NONE, NONE);		m_static_data(float, BEmGov_EAT_OUT, NONE, NONE);		m_static_data(float, BEmGov_CIVIC, NONE, NONE);		m_static_data(float, BEmGov_LEISURE, NONE, NONE);		m_static_data(float, BEmGov_SOCIAL, NONE, NONE);		m_static_data(float, BEmGov_OTHER, NONE, NONE);		m_static_data(float, BEmGov_WORK, NONE, NONE);
			m_static_data(float, BEmSer_PICK, NONE, NONE);		m_static_data(float, BEmSer_OTHER_WORK, NONE, NONE);		m_static_data(float, BEmSer_SERVICE, NONE, NONE);		m_static_data(float, BEmSer_MIN_SHOP, NONE, NONE);		m_static_data(float, BEmSer_MAJ_SHOP, NONE, NONE);		m_static_data(float, BEmSer_EAT_OUT, NONE, NONE);		m_static_data(float, BEmSer_CIVIC, NONE, NONE);		m_static_data(float, BEmSer_LEISURE, NONE, NONE);		m_static_data(float, BEmSer_SOCIAL, NONE, NONE);		m_static_data(float, BEmSer_OTHER, NONE, NONE);		m_static_data(float, BEmSer_WORK, NONE, NONE);
			m_static_data(float, BEmMan_PICK, NONE, NONE);		m_static_data(float, BEmMan_OTHER_WORK, NONE, NONE);		m_static_data(float, BEmMan_SERVICE, NONE, NONE);		m_static_data(float, BEmMan_MIN_SHOP, NONE, NONE);		m_static_data(float, BEmMan_MAJ_SHOP, NONE, NONE);		m_static_data(float, BEmMan_EAT_OUT, NONE, NONE);		m_static_data(float, BEmMan_CIVIC, NONE, NONE);		m_static_data(float, BEmMan_LEISURE, NONE, NONE);		m_static_data(float, BEmMan_SOCIAL, NONE, NONE);		m_static_data(float, BEmMan_OTHER, NONE, NONE);		m_static_data(float, BEmMan_WORK, NONE, NONE);
			m_static_data(float, BEmRet_PICK, NONE, NONE);		m_static_data(float, BEmRet_OTHER_WORK, NONE, NONE);		m_static_data(float, BEmRet_SERVICE, NONE, NONE);		m_static_data(float, BEmRet_MIN_SHOP, NONE, NONE);		m_static_data(float, BEmRet_MAJ_SHOP, NONE, NONE);		m_static_data(float, BEmRet_EAT_OUT, NONE, NONE);		m_static_data(float, BEmRet_CIVIC, NONE, NONE);		m_static_data(float, BEmRet_LEISURE, NONE, NONE);		m_static_data(float, BEmRet_SOCIAL, NONE, NONE);		m_static_data(float, BEmRet_OTHER, NONE, NONE);		m_static_data(float, BEmRet_WORK, NONE, NONE);
			m_static_data(float, THETAG_PICK, NONE, NONE);		m_static_data(float, THETAG_OTHER_WORK, NONE, NONE);		m_static_data(float, THETAG_SERVICE, NONE, NONE);		m_static_data(float, THETAG_MIN_SHOP, NONE, NONE);		m_static_data(float, THETAG_MAJ_SHOP, NONE, NONE);		m_static_data(float, THETAG_EAT_OUT, NONE, NONE);		m_static_data(float, THETAG_CIVIC, NONE, NONE);		m_static_data(float, THETAG_LEISURE, NONE, NONE);		m_static_data(float, THETAG_SOCIAL, NONE, NONE);		m_static_data(float, THETAG_OTHER, NONE, NONE);		m_static_data(float, THETAG_WORK, NONE, NONE);
			m_static_data(float, THETAM_PICK, NONE, NONE);		m_static_data(float, THETAM_OTHER_WORK, NONE, NONE);		m_static_data(float, THETAM_SERVICE, NONE, NONE);		m_static_data(float, THETAM_MIN_SHOP, NONE, NONE);		m_static_data(float, THETAM_MAJ_SHOP, NONE, NONE);		m_static_data(float, THETAM_EAT_OUT, NONE, NONE);		m_static_data(float, THETAM_CIVIC, NONE, NONE);		m_static_data(float, THETAM_LEISURE, NONE, NONE);		m_static_data(float, THETAM_SOCIAL, NONE, NONE);		m_static_data(float, THETAM_OTHER, NONE, NONE);		m_static_data(float, THETAM_WORK, NONE, NONE);
			m_static_data(float, THETAR_PICK, NONE, NONE);		m_static_data(float, THETAR_OTHER_WORK, NONE, NONE);		m_static_data(float, THETAR_SERVICE, NONE, NONE);		m_static_data(float, THETAR_MIN_SHOP, NONE, NONE);		m_static_data(float, THETAR_MAJ_SHOP, NONE, NONE);		m_static_data(float, THETAR_EAT_OUT, NONE, NONE);		m_static_data(float, THETAR_CIVIC, NONE, NONE);		m_static_data(float, THETAR_LEISURE, NONE, NONE);		m_static_data(float, THETAR_SOCIAL, NONE, NONE);		m_static_data(float, THETAR_OTHER, NONE, NONE);		m_static_data(float, THETAR_WORK, NONE, NONE);
			m_static_data(float, THETAS_PICK, NONE, NONE);		m_static_data(float, THETAS_OTHER_WORK, NONE, NONE);		m_static_data(float, THETAS_SERVICE, NONE, NONE);		m_static_data(float, THETAS_MIN_SHOP, NONE, NONE);		m_static_data(float, THETAS_MAJ_SHOP, NONE, NONE);		m_static_data(float, THETAS_EAT_OUT, NONE, NONE);		m_static_data(float, THETAS_CIVIC, NONE, NONE);		m_static_data(float, THETAS_LEISURE, NONE, NONE);		m_static_data(float, THETAS_SOCIAL, NONE, NONE);		m_static_data(float, THETAS_OTHER, NONE, NONE);		m_static_data(float, THETAS_WORK, NONE, NONE);
			m_static_data(float, THETAI_PICK, NONE, NONE);		m_static_data(float, THETAI_OTHER_WORK, NONE, NONE);		m_static_data(float, THETAI_SERVICE, NONE, NONE);		m_static_data(float, THETAI_MIN_SHOP, NONE, NONE);		m_static_data(float, THETAI_MAJ_SHOP, NONE, NONE);		m_static_data(float, THETAI_EAT_OUT, NONE, NONE);		m_static_data(float, THETAI_CIVIC, NONE, NONE);		m_static_data(float, THETAI_LEISURE, NONE, NONE);		m_static_data(float, THETAI_SOCIAL, NONE, NONE);		m_static_data(float, THETAI_OTHER, NONE, NONE);		m_static_data(float, THETAI_WORK, NONE, NONE);
			m_static_data(float, THETAO_PICK, NONE, NONE);		m_static_data(float, THETAO_OTHER_WORK, NONE, NONE);		m_static_data(float, THETAO_SERVICE, NONE, NONE);		m_static_data(float, THETAO_MIN_SHOP, NONE, NONE);		m_static_data(float, THETAO_MAJ_SHOP, NONE, NONE);		m_static_data(float, THETAO_EAT_OUT, NONE, NONE);		m_static_data(float, THETAO_CIVIC, NONE, NONE);		m_static_data(float, THETAO_LEISURE, NONE, NONE);		m_static_data(float, THETAO_SOCIAL, NONE, NONE);		m_static_data(float, THETAO_OTHER, NONE, NONE);		m_static_data(float, THETAO_WORK, NONE, NONE);	m_static_data(float, BEmUnrelated_WORK, NONE, NONE); m_static_data(float, THETA_UR_WORK, NONE, NONE);
			m_static_data(float, GAMMA_PICK, NONE, NONE);		m_static_data(float, GAMMA_OTHER_WORK, NONE, NONE);		m_static_data(float, GAMMA_SERVICE, NONE, NONE);		m_static_data(float, GAMMA_MIN_SHOP, NONE, NONE);		m_static_data(float, GAMMA_MAJ_SHOP, NONE, NONE);		m_static_data(float, GAMMA_EAT_OUT, NONE, NONE);		m_static_data(float, GAMMA_CIVIC, NONE, NONE);			m_static_data(float, GAMMA_LEISURE, NONE, NONE);		m_static_data(float, GAMMA_SOCIAL, NONE, NONE);		m_static_data(float, GAMMA_OTHER, NONE, NONE);		
			#pragma endregion

			m_prototype(Activity_Location_Components::Prototypes::Activity_Location, typename MasterType::activity_location_type, previous, NONE, NONE );
			m_prototype(Activity_Location_Components::Prototypes::Activity_Location, typename MasterType::activity_location_type, destination, NONE, NONE );
			m_prototype(Activity_Location_Components::Prototypes::Activity_Location, typename MasterType::activity_location_type, next, NONE, NONE );
			member_component_and_feature_accessor(start_time, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>)
			m_data(Activity_Components::Types::ACTIVITY_TYPES, activity_type, NONE, NONE);
			m_data(float, bias_correction, NONE, NONE);
			
			//====================================================================================================================================
			// Interface definitions
			typedef Prototypes::Person<typename type_of(Parent_Planner)::type_of(Parent_Person)> person_itf;
			typedef Prototypes::Person_Properties<typename person_itf::get_type_of(Properties)> person_properties_itf;
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

			//RLW%%%
			//typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Activity_Container)> Activity_Plans;
			//typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Plans)> Activity_Plan;
			
			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Movement_Plans_Container)> Movement_Plans;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan<get_component_type(Movement_Plans)> Movement_Plan;

			//------------------------------------------------------------------------------------------------------------------------------------

			static double GetOption_Double(rapidjson::Document& document, const std::string& key, double default_value)
			{
				return reinterpret_cast<_Scenario_Interface*>(_global_scenario)->get_parameter(document, "ADAPTS_Destination_Choice_Option", key, default_value);
			}

			static bool static_initialize()
			{
				// set the base values
				base_static_initializer();

				// now see if there are config file changes
				rapidjson::Document document;
				std::string option_file = reinterpret_cast<_Scenario_Interface*>(_global_scenario)->template adapts_destination_choice_option_param_file<string>();
				if (option_file.length() < 1)
				{
					cout << "Warning: option file for ADAPTS_Destination_Choice_Option was not specified" << endl;
					return true;
				}

				if (!reinterpret_cast<_Scenario_Interface*>(_global_scenario)->parse_option_file(document, option_file))
					return false;

				BTT_PICK		<float>(GetOption_Double(document, "BTT_PICK"   , BTT_PICK			<float>()));
				BINCD_PICK		<float>(GetOption_Double(document, "BINCD_PICK" , BINCD_PICK		<float>()));
				BRACED_PICK		<float>(GetOption_Double(document, "BRACED_PICK", BRACED_PICK		<float>()));
				BArRes_PICK		<float>(GetOption_Double(document, "BArRes_PICK", BArRes_PICK		<float>()));
				BArRec_PICK		<float>(GetOption_Double(document, "BArRec_PICK", BArRec_PICK		<float>()));
				BArRet_PICK		<float>(GetOption_Double(document, "BArRet_PICK", BArRet_PICK		<float>()));
				BArEnt_PICK		<float>(GetOption_Double(document, "BArEnt_PICK", BArEnt_PICK		<float>()));
				BArIns_PICK		<float>(GetOption_Double(document, "BArIns_PICK", BArIns_PICK		<float>()));
				BArOff_PICK		<float>(GetOption_Double(document, "BArOff_PICK", BArOff_PICK		<float>()));
				BArMix_PICK		<float>(GetOption_Double(document, "BArMix_PICK", BArMix_PICK		<float>()));
				BArSch_PICK		<float>(GetOption_Double(document, "BArSch_PICK", BArSch_PICK		<float>()));
				BEmOth_PICK		<float>(GetOption_Double(document, "BEmOth_PICK", BEmOth_PICK		<float>()));
				BEmInd_PICK		<float>(GetOption_Double(document, "BEmInd_PICK", BEmInd_PICK		<float>()));
				BEmGov_PICK		<float>(GetOption_Double(document, "BEmGov_PICK", BEmGov_PICK		<float>()));
				BEmSer_PICK		<float>(GetOption_Double(document, "BEmSer_PICK", BEmSer_PICK		<float>()));
				BEmMan_PICK		<float>(GetOption_Double(document, "BEmMan_PICK", BEmMan_PICK		<float>()));
				BEmRet_PICK		<float>(GetOption_Double(document, "BEmRet_PICK", BEmRet_PICK		<float>()));
				THETAG_PICK		<float>(GetOption_Double(document, "THETAG_PICK", THETAG_PICK		<float>()));
				THETAM_PICK		<float>(GetOption_Double(document, "THETAM_PICK", THETAM_PICK		<float>()));
				THETAR_PICK		<float>(GetOption_Double(document, "THETAR_PICK", THETAR_PICK		<float>()));
				THETAS_PICK		<float>(GetOption_Double(document, "THETAS_PICK", THETAS_PICK		<float>()));
				THETAI_PICK		<float>(GetOption_Double(document, "THETAI_PICK", THETAI_PICK		<float>()));
				THETAO_PICK		<float>(GetOption_Double(document, "THETAO_PICK", THETAO_PICK		<float>()));
				GAMMA_PICK		<float>(GetOption_Double(document, "GAMMA_PICK" , GAMMA_PICK		<float>()));

				BTT_OTHER_WORK		<float>(GetOption_Double(document, "BTT_OTHER_WORK"	  , BTT_OTHER_WORK		<float>()));
				BINCD_OTHER_WORK	<float>(GetOption_Double(document, "BINCD_OTHER_WORK" , BINCD_OTHER_WORK	<float>()));
				BRACED_OTHER_WORK	<float>(GetOption_Double(document, "BRACED_OTHER_WORK", BRACED_OTHER_WORK	<float>()));
				BArRes_OTHER_WORK	<float>(GetOption_Double(document, "BArRes_OTHER_WORK", BArRes_OTHER_WORK	<float>()));
				BArRec_OTHER_WORK	<float>(GetOption_Double(document, "BArRec_OTHER_WORK", BArRec_OTHER_WORK	<float>()));
				BArRet_OTHER_WORK	<float>(GetOption_Double(document, "BArRet_OTHER_WORK", BArRet_OTHER_WORK	<float>()));
				BArEnt_OTHER_WORK	<float>(GetOption_Double(document, "BArEnt_OTHER_WORK", BArEnt_OTHER_WORK	<float>()));
				BArIns_OTHER_WORK	<float>(GetOption_Double(document, "BArIns_OTHER_WORK", BArIns_OTHER_WORK	<float>()));
				BArOff_OTHER_WORK	<float>(GetOption_Double(document, "BArOff_OTHER_WORK", BArOff_OTHER_WORK	<float>()));
				BArMix_OTHER_WORK	<float>(GetOption_Double(document, "BArMix_OTHER_WORK", BArMix_OTHER_WORK	<float>()));
				BArSch_OTHER_WORK	<float>(GetOption_Double(document, "BArSch_OTHER_WORK", BArSch_OTHER_WORK	<float>()));
				BEmOth_OTHER_WORK	<float>(GetOption_Double(document, "BEmOth_OTHER_WORK", BEmOth_OTHER_WORK	<float>()));
				BEmInd_OTHER_WORK	<float>(GetOption_Double(document, "BEmInd_OTHER_WORK", BEmInd_OTHER_WORK	<float>()));
				BEmGov_OTHER_WORK	<float>(GetOption_Double(document, "BEmGov_OTHER_WORK", BEmGov_OTHER_WORK	<float>()));
				BEmSer_OTHER_WORK	<float>(GetOption_Double(document, "BEmSer_OTHER_WORK", BEmSer_OTHER_WORK	<float>()));
				BEmMan_OTHER_WORK	<float>(GetOption_Double(document, "BEmMan_OTHER_WORK", BEmMan_OTHER_WORK	<float>()));
				BEmRet_OTHER_WORK	<float>(GetOption_Double(document, "BEmRet_OTHER_WORK", BEmRet_OTHER_WORK	<float>()));
				THETAG_OTHER_WORK	<float>(GetOption_Double(document, "THETAG_OTHER_WORK", THETAG_OTHER_WORK	<float>()));
				THETAM_OTHER_WORK	<float>(GetOption_Double(document, "THETAM_OTHER_WORK", THETAM_OTHER_WORK	<float>()));
				THETAR_OTHER_WORK	<float>(GetOption_Double(document, "THETAR_OTHER_WORK", THETAR_OTHER_WORK	<float>()));
				THETAS_OTHER_WORK	<float>(GetOption_Double(document, "THETAS_OTHER_WORK", THETAS_OTHER_WORK	<float>()));
				THETAI_OTHER_WORK	<float>(GetOption_Double(document, "THETAI_OTHER_WORK", THETAI_OTHER_WORK	<float>()));
				THETAO_OTHER_WORK	<float>(GetOption_Double(document, "THETAO_OTHER_WORK", THETAO_OTHER_WORK	<float>()));
				GAMMA_OTHER_WORK 	<float>(GetOption_Double(document, "GAMMA_OTHER_WORK",  GAMMA_OTHER_WORK 	<float>()));

				BTT_SERVICE		<float>(GetOption_Double(document, "BTT_SERVICE"   , BTT_SERVICE	<float>()));
				BINCD_SERVICE	<float>(GetOption_Double(document, "BINCD_SERVICE" , BINCD_SERVICE	<float>()));
				BRACED_SERVICE	<float>(GetOption_Double(document, "BRACED_SERVICE", BRACED_SERVICE	<float>()));
				BArRes_SERVICE	<float>(GetOption_Double(document, "BArRes_SERVICE", BArRes_SERVICE	<float>()));
				BArRec_SERVICE	<float>(GetOption_Double(document, "BArRec_SERVICE", BArRec_SERVICE	<float>()));
				BArRet_SERVICE	<float>(GetOption_Double(document, "BArRet_SERVICE", BArRet_SERVICE	<float>()));
				BArEnt_SERVICE	<float>(GetOption_Double(document, "BArEnt_SERVICE", BArEnt_SERVICE	<float>()));
				BArIns_SERVICE	<float>(GetOption_Double(document, "BArIns_SERVICE", BArIns_SERVICE	<float>()));
				BArOff_SERVICE	<float>(GetOption_Double(document, "BArOff_SERVICE", BArOff_SERVICE	<float>()));
				BArMix_SERVICE	<float>(GetOption_Double(document, "BArMix_SERVICE", BArMix_SERVICE	<float>()));
				BArSch_SERVICE	<float>(GetOption_Double(document, "BArSch_SERVICE", BArSch_SERVICE	<float>()));
				BEmOth_SERVICE	<float>(GetOption_Double(document, "BEmOth_SERVICE", BEmOth_SERVICE	<float>()));
				BEmInd_SERVICE	<float>(GetOption_Double(document, "BEmInd_SERVICE", BEmInd_SERVICE	<float>()));
				BEmGov_SERVICE	<float>(GetOption_Double(document, "BEmGov_SERVICE", BEmGov_SERVICE	<float>()));
				BEmSer_SERVICE	<float>(GetOption_Double(document, "BEmSer_SERVICE", BEmSer_SERVICE	<float>()));
				BEmMan_SERVICE	<float>(GetOption_Double(document, "BEmMan_SERVICE", BEmMan_SERVICE	<float>()));
				BEmRet_SERVICE	<float>(GetOption_Double(document, "BEmRet_SERVICE", BEmRet_SERVICE	<float>()));
				THETAG_SERVICE	<float>(GetOption_Double(document, "THETAG_SERVICE", THETAG_SERVICE	<float>()));
				THETAM_SERVICE	<float>(GetOption_Double(document, "THETAM_SERVICE", THETAM_SERVICE	<float>()));
				THETAR_SERVICE	<float>(GetOption_Double(document, "THETAR_SERVICE", THETAR_SERVICE	<float>()));
				THETAS_SERVICE	<float>(GetOption_Double(document, "THETAS_SERVICE", THETAS_SERVICE	<float>()));
				THETAI_SERVICE	<float>(GetOption_Double(document, "THETAI_SERVICE", THETAI_SERVICE	<float>()));
				THETAO_SERVICE	<float>(GetOption_Double(document, "THETAO_SERVICE", THETAO_SERVICE	<float>()));
				GAMMA_SERVICE 	<float>(GetOption_Double(document, "GAMMA_SERVICE" , GAMMA_SERVICE  <float>()));

				BTT_MIN_SHOP	<float>(GetOption_Double(document, "BTT_MIN_SHOP"	, BTT_MIN_SHOP		<float>()));
				BINCD_MIN_SHOP  <float>(GetOption_Double(document, "BINCD_MIN_SHOP" , BINCD_MIN_SHOP	<float>()));
				BRACED_MIN_SHOP	<float>(GetOption_Double(document, "BRACED_MIN_SHOP", BRACED_MIN_SHOP	<float>()));
				BArRes_MIN_SHOP	<float>(GetOption_Double(document, "BArRes_MIN_SHOP", BArRes_MIN_SHOP	<float>()));
				BArRec_MIN_SHOP	<float>(GetOption_Double(document, "BArRec_MIN_SHOP", BArRec_MIN_SHOP	<float>()));
				BArRet_MIN_SHOP	<float>(GetOption_Double(document, "BArRet_MIN_SHOP", BArRet_MIN_SHOP	<float>()));
				BArEnt_MIN_SHOP	<float>(GetOption_Double(document, "BArEnt_MIN_SHOP", BArEnt_MIN_SHOP	<float>()));
				BArIns_MIN_SHOP	<float>(GetOption_Double(document, "BArIns_MIN_SHOP", BArIns_MIN_SHOP	<float>()));
				BArOff_MIN_SHOP	<float>(GetOption_Double(document, "BArOff_MIN_SHOP", BArOff_MIN_SHOP	<float>()));
				BArMix_MIN_SHOP	<float>(GetOption_Double(document, "BArMix_MIN_SHOP", BArMix_MIN_SHOP	<float>()));
				BArSch_MIN_SHOP	<float>(GetOption_Double(document, "BArSch_MIN_SHOP", BArSch_MIN_SHOP	<float>()));
				BEmOth_MIN_SHOP	<float>(GetOption_Double(document, "BEmOth_MIN_SHOP", BEmOth_MIN_SHOP	<float>()));
				BEmInd_MIN_SHOP	<float>(GetOption_Double(document, "BEmInd_MIN_SHOP", BEmInd_MIN_SHOP	<float>()));
				BEmGov_MIN_SHOP	<float>(GetOption_Double(document, "BEmGov_MIN_SHOP", BEmGov_MIN_SHOP	<float>()));
				BEmSer_MIN_SHOP	<float>(GetOption_Double(document, "BEmSer_MIN_SHOP", BEmSer_MIN_SHOP	<float>()));
				BEmMan_MIN_SHOP	<float>(GetOption_Double(document, "BEmMan_MIN_SHOP", BEmMan_MIN_SHOP	<float>()));
				BEmRet_MIN_SHOP	<float>(GetOption_Double(document, "BEmRet_MIN_SHOP", BEmRet_MIN_SHOP	<float>()));
				THETAG_MIN_SHOP	<float>(GetOption_Double(document, "THETAG_MIN_SHOP", THETAG_MIN_SHOP	<float>()));
				THETAM_MIN_SHOP	<float>(GetOption_Double(document, "THETAM_MIN_SHOP", THETAM_MIN_SHOP	<float>()));
				THETAR_MIN_SHOP	<float>(GetOption_Double(document, "THETAR_MIN_SHOP", THETAR_MIN_SHOP	<float>()));
				THETAS_MIN_SHOP	<float>(GetOption_Double(document, "THETAS_MIN_SHOP", THETAS_MIN_SHOP	<float>()));
				THETAI_MIN_SHOP	<float>(GetOption_Double(document, "THETAI_MIN_SHOP", THETAI_MIN_SHOP	<float>()));
				THETAO_MIN_SHOP	<float>(GetOption_Double(document, "THETAO_MIN_SHOP", THETAO_MIN_SHOP	<float>()));
				GAMMA_MIN_SHOP 	<float>(GetOption_Double(document, "GAMMA_MIN_SHOP" , GAMMA_MIN_SHOP 	<float>()));

				BTT_MAJ_SHOP   	<float>(GetOption_Double(document, "BTT_MAJ_SHOP"   , BTT_MAJ_SHOP   	<float>()));
				BINCD_MAJ_SHOP 	<float>(GetOption_Double(document, "BINCD_MAJ_SHOP" , BINCD_MAJ_SHOP 	<float>()));
				BRACED_MAJ_SHOP	<float>(GetOption_Double(document, "BRACED_MAJ_SHOP", BRACED_MAJ_SHOP	<float>()));
				BArRes_MAJ_SHOP	<float>(GetOption_Double(document, "BArRes_MAJ_SHOP", BArRes_MAJ_SHOP	<float>()));
				BArRec_MAJ_SHOP	<float>(GetOption_Double(document, "BArRec_MAJ_SHOP", BArRec_MAJ_SHOP	<float>()));
				BArRet_MAJ_SHOP	<float>(GetOption_Double(document, "BArRet_MAJ_SHOP", BArRet_MAJ_SHOP	<float>()));
				BArEnt_MAJ_SHOP	<float>(GetOption_Double(document, "BArEnt_MAJ_SHOP", BArEnt_MAJ_SHOP	<float>()));
				BArIns_MAJ_SHOP	<float>(GetOption_Double(document, "BArIns_MAJ_SHOP", BArIns_MAJ_SHOP	<float>()));
				BArOff_MAJ_SHOP	<float>(GetOption_Double(document, "BArOff_MAJ_SHOP", BArOff_MAJ_SHOP	<float>()));
				BArMix_MAJ_SHOP	<float>(GetOption_Double(document, "BArMix_MAJ_SHOP", BArMix_MAJ_SHOP	<float>()));
				BArSch_MAJ_SHOP	<float>(GetOption_Double(document, "BArSch_MAJ_SHOP", BArSch_MAJ_SHOP	<float>()));
				BEmOth_MAJ_SHOP	<float>(GetOption_Double(document, "BEmOth_MAJ_SHOP", BEmOth_MAJ_SHOP	<float>()));
				BEmInd_MAJ_SHOP	<float>(GetOption_Double(document, "BEmInd_MAJ_SHOP", BEmInd_MAJ_SHOP	<float>()));
				BEmGov_MAJ_SHOP	<float>(GetOption_Double(document, "BEmGov_MAJ_SHOP", BEmGov_MAJ_SHOP	<float>()));
				BEmSer_MAJ_SHOP	<float>(GetOption_Double(document, "BEmSer_MAJ_SHOP", BEmSer_MAJ_SHOP	<float>()));
				BEmMan_MAJ_SHOP	<float>(GetOption_Double(document, "BEmMan_MAJ_SHOP", BEmMan_MAJ_SHOP	<float>()));
				BEmRet_MAJ_SHOP	<float>(GetOption_Double(document, "BEmRet_MAJ_SHOP", BEmRet_MAJ_SHOP	<float>()));
				THETAG_MAJ_SHOP	<float>(GetOption_Double(document, "THETAG_MAJ_SHOP", THETAG_MAJ_SHOP	<float>()));
				THETAM_MAJ_SHOP	<float>(GetOption_Double(document, "THETAM_MAJ_SHOP", THETAM_MAJ_SHOP	<float>()));
				THETAR_MAJ_SHOP	<float>(GetOption_Double(document, "THETAR_MAJ_SHOP", THETAR_MAJ_SHOP	<float>()));
				THETAS_MAJ_SHOP	<float>(GetOption_Double(document, "THETAS_MAJ_SHOP", THETAS_MAJ_SHOP	<float>()));
				THETAI_MAJ_SHOP	<float>(GetOption_Double(document, "THETAI_MAJ_SHOP", THETAI_MAJ_SHOP	<float>()));
				THETAO_MAJ_SHOP	<float>(GetOption_Double(document, "THETAO_MAJ_SHOP", THETAO_MAJ_SHOP	<float>()));
				GAMMA_MAJ_SHOP 	<float>(GetOption_Double(document, "GAMMA_MAJ_SHOP" , GAMMA_MAJ_SHOP 	<float>()));

				BTT_EAT_OUT	  	<float>(GetOption_Double(document, "BTT_EAT_OUT"   , BTT_EAT_OUT	<float>()));
				BINCD_EAT_OUT 	<float>(GetOption_Double(document, "BINCD_EAT_OUT" , BINCD_EAT_OUT	<float>()));
				BRACED_EAT_OUT	<float>(GetOption_Double(document, "BRACED_EAT_OUT", BRACED_EAT_OUT	<float>()));
				BArRes_EAT_OUT	<float>(GetOption_Double(document, "BArRes_EAT_OUT", BArRes_EAT_OUT	<float>()));
				BArRec_EAT_OUT	<float>(GetOption_Double(document, "BArRec_EAT_OUT", BArRec_EAT_OUT	<float>()));
				BArRet_EAT_OUT	<float>(GetOption_Double(document, "BArRet_EAT_OUT", BArRet_EAT_OUT	<float>()));
				BArEnt_EAT_OUT	<float>(GetOption_Double(document, "BArEnt_EAT_OUT", BArEnt_EAT_OUT	<float>()));
				BArIns_EAT_OUT	<float>(GetOption_Double(document, "BArIns_EAT_OUT", BArIns_EAT_OUT	<float>()));
				BArOff_EAT_OUT	<float>(GetOption_Double(document, "BArOff_EAT_OUT", BArOff_EAT_OUT	<float>()));
				BArMix_EAT_OUT	<float>(GetOption_Double(document, "BArMix_EAT_OUT", BArMix_EAT_OUT	<float>()));
				BArSch_EAT_OUT	<float>(GetOption_Double(document, "BArSch_EAT_OUT", BArSch_EAT_OUT	<float>()));
				BEmOth_EAT_OUT	<float>(GetOption_Double(document, "BEmOth_EAT_OUT", BEmOth_EAT_OUT	<float>()));
				BEmInd_EAT_OUT	<float>(GetOption_Double(document, "BEmInd_EAT_OUT", BEmInd_EAT_OUT	<float>()));
				BEmGov_EAT_OUT	<float>(GetOption_Double(document, "BEmGov_EAT_OUT", BEmGov_EAT_OUT	<float>()));
				BEmSer_EAT_OUT	<float>(GetOption_Double(document, "BEmSer_EAT_OUT", BEmSer_EAT_OUT	<float>()));
				BEmMan_EAT_OUT	<float>(GetOption_Double(document, "BEmMan_EAT_OUT", BEmMan_EAT_OUT	<float>()));
				BEmRet_EAT_OUT	<float>(GetOption_Double(document, "BEmRet_EAT_OUT", BEmRet_EAT_OUT	<float>()));
				THETAG_EAT_OUT	<float>(GetOption_Double(document, "THETAG_EAT_OUT", THETAG_EAT_OUT	<float>()));
				THETAM_EAT_OUT	<float>(GetOption_Double(document, "THETAM_EAT_OUT", THETAM_EAT_OUT	<float>()));
				THETAR_EAT_OUT	<float>(GetOption_Double(document, "THETAR_EAT_OUT", THETAR_EAT_OUT	<float>()));
				THETAS_EAT_OUT	<float>(GetOption_Double(document, "THETAS_EAT_OUT", THETAS_EAT_OUT	<float>()));
				THETAI_EAT_OUT	<float>(GetOption_Double(document, "THETAI_EAT_OUT", THETAI_EAT_OUT	<float>()));
				THETAO_EAT_OUT	<float>(GetOption_Double(document, "THETAO_EAT_OUT", THETAO_EAT_OUT	<float>()));
				GAMMA_EAT_OUT 	<float>(GetOption_Double(document, "GAMMA_EAT_OUT", GAMMA_EAT_OUT 	<float>()));

				BTT_CIVIC   	<float>(GetOption_Double(document, "BTT_CIVIC"   , BTT_CIVIC   	<float>()));
				BINCD_CIVIC 	<float>(GetOption_Double(document, "BINCD_CIVIC" , BINCD_CIVIC 	<float>()));
				BRACED_CIVIC	<float>(GetOption_Double(document, "BRACED_CIVIC", BRACED_CIVIC	<float>()));
				BArRes_CIVIC	<float>(GetOption_Double(document, "BArRes_CIVIC", BArRes_CIVIC	<float>()));
				BArRec_CIVIC	<float>(GetOption_Double(document, "BArRec_CIVIC", BArRec_CIVIC	<float>()));
				BArRet_CIVIC	<float>(GetOption_Double(document, "BArRet_CIVIC", BArRet_CIVIC	<float>()));
				BArEnt_CIVIC	<float>(GetOption_Double(document, "BArEnt_CIVIC", BArEnt_CIVIC	<float>()));
				BArIns_CIVIC	<float>(GetOption_Double(document, "BArIns_CIVIC", BArIns_CIVIC	<float>()));
				BArOff_CIVIC	<float>(GetOption_Double(document, "BArOff_CIVIC", BArOff_CIVIC	<float>()));
				BArMix_CIVIC	<float>(GetOption_Double(document, "BArMix_CIVIC", BArMix_CIVIC	<float>()));
				BArSch_CIVIC	<float>(GetOption_Double(document, "BArSch_CIVIC", BArSch_CIVIC	<float>()));
				BEmOth_CIVIC	<float>(GetOption_Double(document, "BEmOth_CIVIC", BEmOth_CIVIC	<float>()));
				BEmInd_CIVIC	<float>(GetOption_Double(document, "BEmInd_CIVIC", BEmInd_CIVIC	<float>()));
				BEmGov_CIVIC	<float>(GetOption_Double(document, "BEmGov_CIVIC", BEmGov_CIVIC	<float>()));
				BEmSer_CIVIC	<float>(GetOption_Double(document, "BEmSer_CIVIC", BEmSer_CIVIC	<float>()));
				BEmMan_CIVIC	<float>(GetOption_Double(document, "BEmMan_CIVIC", BEmMan_CIVIC	<float>()));
				BEmRet_CIVIC	<float>(GetOption_Double(document, "BEmRet_CIVIC", BEmRet_CIVIC	<float>()));
				THETAG_CIVIC	<float>(GetOption_Double(document, "THETAG_CIVIC", THETAG_CIVIC	<float>()));
				THETAM_CIVIC	<float>(GetOption_Double(document, "THETAM_CIVIC", THETAM_CIVIC	<float>()));
				THETAR_CIVIC	<float>(GetOption_Double(document, "THETAR_CIVIC", THETAR_CIVIC	<float>()));
				THETAS_CIVIC	<float>(GetOption_Double(document, "THETAS_CIVIC", THETAS_CIVIC	<float>()));
				THETAI_CIVIC	<float>(GetOption_Double(document, "THETAI_CIVIC", THETAI_CIVIC	<float>()));
				THETAO_CIVIC	<float>(GetOption_Double(document, "THETAO_CIVIC", THETAO_CIVIC	<float>()));
				GAMMA_CIVIC 	<float>(GetOption_Double(document, "GAMMA_CIVIC" , GAMMA_CIVIC 	<float>()));

				BTT_LEISURE	  	<float>(GetOption_Double(document, "BTT_LEISURE"   , BTT_LEISURE  	<float>()));
				BINCD_LEISURE 	<float>(GetOption_Double(document, "BINCD_LEISURE" , BINCD_LEISURE 	<float>()));
				BRACED_LEISURE	<float>(GetOption_Double(document, "BRACED_LEISURE", BRACED_LEISURE	<float>()));
				BArRes_LEISURE	<float>(GetOption_Double(document, "BArRes_LEISURE", BArRes_LEISURE	<float>()));
				BArRec_LEISURE	<float>(GetOption_Double(document, "BArRec_LEISURE", BArRec_LEISURE	<float>()));
				BArRet_LEISURE	<float>(GetOption_Double(document, "BArRet_LEISURE", BArRet_LEISURE	<float>()));
				BArEnt_LEISURE	<float>(GetOption_Double(document, "BArEnt_LEISURE", BArEnt_LEISURE	<float>()));
				BArIns_LEISURE	<float>(GetOption_Double(document, "BArIns_LEISURE", BArIns_LEISURE	<float>()));
				BArOff_LEISURE	<float>(GetOption_Double(document, "BArOff_LEISURE", BArOff_LEISURE	<float>()));
				BArMix_LEISURE	<float>(GetOption_Double(document, "BArMix_LEISURE", BArMix_LEISURE	<float>()));
				BArSch_LEISURE	<float>(GetOption_Double(document, "BArSch_LEISURE", BArSch_LEISURE	<float>()));
				BEmOth_LEISURE	<float>(GetOption_Double(document, "BEmOth_LEISURE", BEmOth_LEISURE	<float>()));
				BEmInd_LEISURE	<float>(GetOption_Double(document, "BEmInd_LEISURE", BEmInd_LEISURE	<float>()));
				BEmGov_LEISURE	<float>(GetOption_Double(document, "BEmGov_LEISURE", BEmGov_LEISURE	<float>()));
				BEmSer_LEISURE	<float>(GetOption_Double(document, "BEmSer_LEISURE", BEmSer_LEISURE	<float>()));
				BEmMan_LEISURE	<float>(GetOption_Double(document, "BEmMan_LEISURE", BEmMan_LEISURE	<float>()));
				BEmRet_LEISURE	<float>(GetOption_Double(document, "BEmRet_LEISURE", BEmRet_LEISURE	<float>()));
				THETAG_LEISURE	<float>(GetOption_Double(document, "THETAG_LEISURE", THETAG_LEISURE	<float>()));
				THETAM_LEISURE	<float>(GetOption_Double(document, "THETAM_LEISURE", THETAM_LEISURE	<float>()));
				THETAR_LEISURE	<float>(GetOption_Double(document, "THETAR_LEISURE", THETAR_LEISURE	<float>()));
				THETAS_LEISURE	<float>(GetOption_Double(document, "THETAS_LEISURE", THETAS_LEISURE	<float>()));
				THETAI_LEISURE	<float>(GetOption_Double(document, "THETAI_LEISURE", THETAI_LEISURE	<float>()));
				THETAO_LEISURE	<float>(GetOption_Double(document, "THETAO_LEISURE", THETAO_LEISURE	<float>()));
				GAMMA_LEISURE 	<float>(GetOption_Double(document, "GAMMA_LEISURE" , GAMMA_LEISURE 	<float>()));

				BTT_SOCIAL   	<float>(GetOption_Double(document, "BTT_SOCIAL"   , BTT_SOCIAL   	<float>()));
				BINCD_SOCIAL 	<float>(GetOption_Double(document, "BINCD_SOCIAL" , BINCD_SOCIAL 	<float>()));
				BRACED_SOCIAL	<float>(GetOption_Double(document, "BRACED_SOCIAL", BRACED_SOCIAL	<float>()));
				BArRes_SOCIAL	<float>(GetOption_Double(document, "BArRes_SOCIAL", BArRes_SOCIAL	<float>()));
				BArRec_SOCIAL	<float>(GetOption_Double(document, "BArRec_SOCIAL", BArRec_SOCIAL	<float>()));
				BArRet_SOCIAL	<float>(GetOption_Double(document, "BArRet_SOCIAL", BArRet_SOCIAL	<float>()));
				BArEnt_SOCIAL	<float>(GetOption_Double(document, "BArEnt_SOCIAL", BArEnt_SOCIAL	<float>()));
				BArIns_SOCIAL	<float>(GetOption_Double(document, "BArIns_SOCIAL", BArIns_SOCIAL	<float>()));
				BArOff_SOCIAL	<float>(GetOption_Double(document, "BArOff_SOCIAL", BArOff_SOCIAL	<float>()));
				BArMix_SOCIAL	<float>(GetOption_Double(document, "BArMix_SOCIAL", BArMix_SOCIAL	<float>()));
				BArSch_SOCIAL	<float>(GetOption_Double(document, "BArSch_SOCIAL", BArSch_SOCIAL	<float>()));
				BEmOth_SOCIAL	<float>(GetOption_Double(document, "BEmOth_SOCIAL", BEmOth_SOCIAL	<float>()));
				BEmInd_SOCIAL	<float>(GetOption_Double(document, "BEmInd_SOCIAL", BEmInd_SOCIAL	<float>()));
				BEmGov_SOCIAL	<float>(GetOption_Double(document, "BEmGov_SOCIAL", BEmGov_SOCIAL	<float>()));
				BEmSer_SOCIAL	<float>(GetOption_Double(document, "BEmSer_SOCIAL", BEmSer_SOCIAL	<float>()));
				BEmMan_SOCIAL	<float>(GetOption_Double(document, "BEmMan_SOCIAL", BEmMan_SOCIAL	<float>()));
				BEmRet_SOCIAL	<float>(GetOption_Double(document, "BEmRet_SOCIAL", BEmRet_SOCIAL	<float>()));
				THETAG_SOCIAL	<float>(GetOption_Double(document, "THETAG_SOCIAL", THETAG_SOCIAL	<float>()));
				THETAM_SOCIAL	<float>(GetOption_Double(document, "THETAM_SOCIAL", THETAM_SOCIAL	<float>()));
				THETAR_SOCIAL	<float>(GetOption_Double(document, "THETAR_SOCIAL", THETAR_SOCIAL	<float>()));
				THETAS_SOCIAL	<float>(GetOption_Double(document, "THETAS_SOCIAL", THETAS_SOCIAL	<float>()));
				THETAI_SOCIAL	<float>(GetOption_Double(document, "THETAI_SOCIAL", THETAI_SOCIAL	<float>()));
				THETAO_SOCIAL	<float>(GetOption_Double(document, "THETAO_SOCIAL", THETAO_SOCIAL	<float>()));
				GAMMA_SOCIAL 	<float>(GetOption_Double(document, "GAMMA_SOCIAL" , GAMMA_SOCIAL 	<float>()));

				BTT_OTHER   	<float>(GetOption_Double(document, "BTT_OTHER"  , BTT_OTHER   	<float>()));
				BINCD_OTHER 	<float>(GetOption_Double(document, "BINCD_OTHER", BINCD_OTHER 	<float>()));
				BRACED_OTHER	<float>(GetOption_Double(document, "BRACED_OTHER", BRACED_OTHER	<float>()));
				BArRes_OTHER	<float>(GetOption_Double(document, "BArRes_OTHER", BArRes_OTHER	<float>()));
				BArRec_OTHER	<float>(GetOption_Double(document, "BArRec_OTHER", BArRec_OTHER	<float>()));
				BArRet_OTHER	<float>(GetOption_Double(document, "BArRet_OTHER", BArRet_OTHER	<float>()));
				BArEnt_OTHER	<float>(GetOption_Double(document, "BArEnt_OTHER", BArEnt_OTHER	<float>()));
				BArIns_OTHER	<float>(GetOption_Double(document, "BArIns_OTHER", BArIns_OTHER	<float>()));
				BArOff_OTHER	<float>(GetOption_Double(document, "BArOff_OTHER", BArOff_OTHER	<float>()));
				BArMix_OTHER	<float>(GetOption_Double(document, "BArMix_OTHER", BArMix_OTHER	<float>()));
				BArSch_OTHER	<float>(GetOption_Double(document, "BArSch_OTHER", BArSch_OTHER	<float>()));
				BEmOth_OTHER	<float>(GetOption_Double(document, "BEmOth_OTHER", BEmOth_OTHER	<float>()));
				BEmInd_OTHER	<float>(GetOption_Double(document, "BEmInd_OTHER", BEmInd_OTHER	<float>()));
				BEmGov_OTHER	<float>(GetOption_Double(document, "BEmGov_OTHER", BEmGov_OTHER	<float>()));
				BEmSer_OTHER	<float>(GetOption_Double(document, "BEmSer_OTHER", BEmSer_OTHER	<float>()));
				BEmMan_OTHER	<float>(GetOption_Double(document, "BEmMan_OTHER", BEmMan_OTHER	<float>()));
				BEmRet_OTHER	<float>(GetOption_Double(document, "BEmRet_OTHER", BEmRet_OTHER	<float>()));
				THETAG_OTHER	<float>(GetOption_Double(document, "THETAG_OTHER", THETAG_OTHER	<float>()));
				THETAM_OTHER	<float>(GetOption_Double(document, "THETAM_OTHER", THETAM_OTHER	<float>()));
				THETAR_OTHER	<float>(GetOption_Double(document, "THETAR_OTHER", THETAR_OTHER	<float>()));
				THETAS_OTHER	<float>(GetOption_Double(document, "THETAS_OTHER", THETAS_OTHER	<float>()));
				THETAI_OTHER	<float>(GetOption_Double(document, "THETAI_OTHER", THETAI_OTHER	<float>()));
				THETAO_OTHER	<float>(GetOption_Double(document, "THETAO_OTHER", THETAO_OTHER	<float>()));
				GAMMA_OTHER 	<float>(GetOption_Double(document, "GAMMA_OTHER" , GAMMA_OTHER 	<float>()));

				BTTAUTO_WORK	<float>(GetOption_Double(document, "BTTAUTO_WORK", BTTAUTO_WORK	<float>()));
				BINCD_WORK 		<float>(GetOption_Double(document, "BINCD_WORK"	 , BINCD_WORK 	<float>()));
				BRACED_WORK 	<float>(GetOption_Double(document, "BRACED_WORK" , BRACED_WORK 	<float>()));
				BArRes_WORK 	<float>(GetOption_Double(document, "BArRes_WORK" , BArRes_WORK 	<float>()));
				BArRec_WORK 	<float>(GetOption_Double(document, "BArRec_WORK" , BArRec_WORK 	<float>()));
				BArRet_WORK 	<float>(GetOption_Double(document, "BArRet_WORK" , BArRet_WORK 	<float>()));
				BArEnt_WORK 	<float>(GetOption_Double(document, "BArEnt_WORK" , BArEnt_WORK 	<float>()));
				BArIns_WORK 	<float>(GetOption_Double(document, "BArIns_WORK" , BArIns_WORK 	<float>()));
				BArOff_WORK 	<float>(GetOption_Double(document, "BArOff_WORK" , BArOff_WORK 	<float>()));
				BArMix_WORK 	<float>(GetOption_Double(document, "BArMix_WORK" , BArMix_WORK 	<float>()));
				BArSch_WORK 	<float>(GetOption_Double(document, "BArSch_WORK" , BArSch_WORK 	<float>()));
				BEmOth_WORK 	<float>(GetOption_Double(document, "BEmOth_WORK" , BEmOth_WORK 	<float>()));
				BEmInd_WORK 	<float>(GetOption_Double(document, "BEmInd_WORK" , BEmInd_WORK 	<float>()));
				BEmGov_WORK 	<float>(GetOption_Double(document, "BEmGov_WORK" , BEmGov_WORK 	<float>()));
				BEmSer_WORK 	<float>(GetOption_Double(document, "BEmSer_WORK" , BEmSer_WORK 	<float>()));
				BEmMan_WORK 	<float>(GetOption_Double(document, "BEmMan_WORK" , BEmMan_WORK 	<float>()));
				BEmRet_WORK 	<float>(GetOption_Double(document, "BEmRet_WORK" , BEmRet_WORK 	<float>()));
				THETAG_WORK 	<float>(GetOption_Double(document, "THETAG_WORK" , THETAG_WORK 	<float>()));
				THETAM_WORK 	<float>(GetOption_Double(document, "THETAM_WORK" , THETAM_WORK 	<float>()));
				THETAR_WORK 	<float>(GetOption_Double(document, "THETAR_WORK" , THETAR_WORK 	<float>()));
				THETAS_WORK 	<float>(GetOption_Double(document, "THETAS_WORK" , THETAS_WORK 	<float>()));
				THETAI_WORK 	<float>(GetOption_Double(document, "THETAI_WORK" , THETAI_WORK 	<float>()));
				THETAO_WORK 	<float>(GetOption_Double(document, "THETAO_WORK" , THETAO_WORK 	<float>()));

				BTTTRAN_WORK     	<float>(GetOption_Double(document, "BTTTRAN_WORK", BTTTRAN_WORK     		<float>()));
				BTTOTHER_WORK    	<float>(GetOption_Double(document, "BTTOTHER_WORK", BTTOTHER_WORK    		<float>()));
				BHOME_WORK       	<float>(GetOption_Double(document, "BHOME_WORK", BHOME_WORK       			<float>()));
				BEmUnrelated_WORK	<float>(GetOption_Double(document, "BEmUnrelated_WORK", BEmUnrelated_WORK	<float>()));
				THETA_UR_WORK    	<float>(GetOption_Double(document, "THETA_UR_WORK", THETA_UR_WORK    		<float>()));

				return true;
			}

			template<typename TargetType> void Initialize()
			{	
				this->_bias_correction = 1.0;
			}

			virtual double Calculate_Utility()
			{
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
				person_properties_itf* properties = _Parent_Person->template Properties<person_properties_itf*>();
				person_static_properties_itf* static_properties = _Parent_Person->template Static_Properties<person_static_properties_itf*>();
				household_itf* _Parent_Household = _Parent_Person->person_itf::template Household<household_itf*>();
				household_static_properties_itf* household_properties = _Parent_Household->template Static_Properties<household_static_properties_itf*>();

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();
				_Activity_Locations_Container_Interface* locations = network->template activity_locations_container<_Activity_Locations_Container_Interface*>();
				_Skim_Interface* los = network->template skimming_faculty<_Skim_Interface*>();

				// variables used for utility calculation
				double ttime_before, ttime_after, ttime_without, ttime_deflected, u;
				double utility_sum = 0;
				double prob_sum = 0;
				_Zone_Interface* zone;
				Time_Minutes start_time = this->template start_time<Time_Minutes>();

				// select zones to choose from and estimate utility
				zone = _destination->template zone<_Zone_Interface*>();

				// get the deflected travel time due to the addition of the current activity
				ttime_before = los->template Get_TTime<_Activity_Location_Interface*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Minutes, Time_Minutes>(_previous,_destination,Vehicle_Components::Types::Vehicle_Type_Keys::SOV, start_time);
				ttime_after = los->template Get_TTime<_Activity_Location_Interface*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Minutes, Time_Minutes>(_destination,_next,Vehicle_Components::Types::Vehicle_Type_Keys::SOV, start_time);
				if (_previous == _next) ttime_without = 0;
				else ttime_without = los->template Get_TTime<_Activity_Location_Interface*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Minutes, Time_Minutes>(_previous,_next,Vehicle_Components::Types::Vehicle_Type_Keys::SOV, start_time);
				
				//=====================================
				// ACCOUNT FOR VOTT changes
				float VOTT_change = properties->template Value_of_Travel_Time_Adjustment<float>();

				//=====================================
				// Discounted deflected travel time 
				ttime_deflected = (ttime_before + ttime_after - ttime_without)*VOTT_change;
				
				// Get Income/race dif with zone
				float inc_diff = log(abs(zone->template average_household_income<Dollars>() - static_properties->template Income<Dollars>()) + 1.0);
				float race_diff;
				if (static_properties->template Race<Person_Components::Types::RACE>() == Person_Components::Types::RACE::WHITE_ALONE) race_diff = 1.0 - zone->template race_percent_white<float>();
				else if (static_properties->template Race<Person_Components::Types::RACE>() == Person_Components::Types::RACE::BLACK_ALONE) race_diff = 1.0 - zone->template race_percent_black<float>();
				else race_diff = zone->template race_percent_white<float>() + zone->template race_percent_black<float>();
					
				float area_res = zone->template residential_area<Square_Feet>()/1000000.0;
				float area_rec = zone->template other_area<Square_Feet>()/1000000.0;
				float area_ret = zone->template retail_area<Square_Feet>()/1000000.0;
				float area_ent = zone->template entertainment_area<Square_Feet>()/1000000.0;
				float area_ins = zone->template institutional_area<Square_Feet>()/1000000.0;
				float area_off = zone->template office_area<Square_Feet>()/1000000.0;
				float area_mix = zone->template mixed_use_area<Square_Feet>()/1000000.0;
				float area_sch = zone->template school_area<Square_Feet>()/1000000.0;
				float emp_oth = zone->template employment_other<double>()/1000.0;
				float emp_ind = zone->template employment_industrial<double>()/1000.0;
				float emp_man = zone->template employment_manufacturing<double>()/1000.0;
				float emp_gov = zone->template employment_government<double>()/1000.0;
				float emp_srv = zone->template employment_services<double>()/1000.0;
				float emp_ret = zone->template employment_retail<double>()/1000.0;

				// I don't think these need to be divided by 1000
				float thetag = zone->template accessibility_employment_government<double>();// / 1000.0;
				float thetam = zone->template accessibility_employment_manufacturing<double>();
				float thetar = zone->template accessibility_employment_retail<double>();
				float thetas = zone->template accessibility_employment_services<double>();
				float thetai = zone->template accessibility_employment_industrial<double>();
				float thetao = zone->template accessibility_employment_other<double>();

				if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY || _activity_type == Activity_Components::Types::ACTIVITY_TYPES::PART_TIME_WORK_ACTIVITY)
				{
					float WORK_TTIME_CALIBRATION_FACTOR = -0.02;

					Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE industry = static_properties->template Employment_Industry_Simple<Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE>();
					float EMPUR = 0;

					// Mode specific effects
					int ModAuto = 0, ModTran = 0,ModOth = 0;
					Person_Components::Types::JOURNEY_TO_WORK_MODE mode = static_properties->template Journey_To_Work_Mode<Person_Components::Types::JOURNEY_TO_WORK_MODE>();
					if (mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_AUTOMOBILE || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_MOTORCYCLE || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_TAXI)
						ModAuto = 1;
					else if (mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_BUS || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_RAILROAD || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_STREETCAR || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_SUBWAY)
						ModTran = 1;
					else
						ModOth = 1;

					// positive impact of working near home
					float HOME = 0.0; 
					if (zone == _Parent_Person->template Home_Location<_Zone_Interface*>()) HOME = 1.0;

					// industry type interaction variables
					float IndR = 0, IndS = 0, IndG = 0, IndI = 0, IndM = 0, IndO = 0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::RETAIL) IndR = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::SERVICE) IndS = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::GOVERNMENT) IndG = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::INDUSTRIAL) IndI = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::MANUFACTURING) IndM = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::OTHER) IndO = 1.0;

					float area_res = log(zone->template residential_area<Square_Feet>()+1.0);
					float area_rec = log(zone->template other_area<Square_Feet>()+1.0);
					float area_ret = log(zone->template retail_area<Square_Feet>()+1.0);
					float area_ent = log(zone->template entertainment_area<Square_Feet>()+1.0);
					float area_ins = log(zone->template institutional_area<Square_Feet>()+1.0);
					float area_off = log(zone->template office_area<Square_Feet>()+1.0);
					float area_mix = log(zone->template mixed_use_area<Square_Feet>()+1.0);
					float area_sch = log(zone->template school_area<Square_Feet>()+1.0);

					// Unrelated employment
					EMPUR += (max<float>(zone->template employment_retail<float>(), 0.0) / 1000.0) * (-1.0 * (IndR - 1.0));
					EMPUR += (max<float>(zone->template employment_government<float>(), 0.0) / 1000.0) * (-1.0 * (IndG - 1.0));
					EMPUR += (max<float>(zone->template employment_services<float>(), 0.0) / 1000.0) * (-1.0 * (IndS - 1.0));
					EMPUR += (max<float>(zone->template employment_industrial<float>(), 0.0) / 1000.0) * (-1.0 * (IndI - 1.0));
					EMPUR += (max<float>(zone->template employment_manufacturing<float>(), 0.0) / 1000.0) * (-1.0 * (IndM - 1.0));
					EMPUR += (max<float>(zone->template employment_other<float>(), 0.0) / 1000.0) * (-1.0 * (IndO - 1.0));

					float theta_ur = thetar * (1.0 - IndR) + thetag * (1.0 - IndG) + thetas * (1.0 - IndS) + thetai * (1.0 - IndI) + thetam * (1.0 - IndM) + thetao * (1.0 - IndO);

					// cut travel time in half since original work location choice equations only account for the half-tour travel time
					//ttime_deflected = ttime_deflected *0.5;

					// Old values - with time strata at (45A/60T/20W)
					u = _BArEnt_WORK * area_ent + _BArIns_WORK * area_ins + _BArOff_WORK * area_off + _BArRec_WORK * area_rec + _BArRet_WORK *area_ret + _BArRes_WORK * area_res +
						_BEmUnrelated_WORK * EMPUR + _BEmGov_WORK * (zone->template employment_government<float>()) / 1000.0 * IndG + _BEmMan_WORK * (zone->template employment_manufacturing<float>()) / 1000.0 * IndM + _BEmRet_WORK * (zone->template employment_retail<float>()) / 1000.0 * IndR + _BEmSer_WORK * (zone->template employment_services<float>()) / 1000.0 * IndS + 
						_BHOME_WORK * HOME + _BTTAUTO_WORK * (ttime_deflected) * ModAuto + _BTTTRAN_WORK * (ttime_deflected) * ModTran + _BTTOTHER_WORK * (ttime_deflected) * ModOth + ttime_deflected * WORK_TTIME_CALIBRATION_FACTOR + 
						thetag * _THETAG_WORK + thetam * _THETAM_WORK + thetar * _THETAR_WORK + thetas * _THETAS_WORK + thetai * _THETAI_WORK + thetao * _THETAO_WORK + _THETA_UR_WORK * theta_ur;
					if (zone->template employment_total<int>() < 1.0) u = -9999999;
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

				u += log(_bias_correction);

				return u;				
			}

			virtual void Print_Utility()
			{
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
				person_static_properties_itf* static_properties = _Parent_Person->template Static_Properties<person_static_properties_itf*>();

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();
				_Activity_Locations_Container_Interface* locations = network->template activity_locations_container<_Activity_Locations_Container_Interface*>();
				_Skim_Interface* los = network->template skimming_faculty<_Skim_Interface*>();

				// variables used for utility calculation
				double ttime_before, ttime_after, ttime_without, ttime_deflected, u;
				double utility_sum = 0;
				double prob_sum = 0;
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
				float inc_diff = log(abs(zone->template average_household_income<Dollars>() - static_properties->template Income<Dollars>()) + 1.0);
				float race_diff;
				if (static_properties->template Race<Person_Components::Types::RACE>() == Person_Components::Types::RACE::WHITE_ALONE) race_diff = 1.0 - zone->template race_percent_white<float>();
				else if (static_properties->template Race<Person_Components::Types::RACE>() == Person_Components::Types::RACE::BLACK_ALONE) race_diff = 1.0 - zone->template race_percent_black<float>();
				else race_diff = zone->template race_percent_white<float>() + zone->template race_percent_black<float>();
					
				float area_res = zone->template residential_area<Square_Feet>()/1000000.0;
				float area_rec = zone->template other_area<Square_Feet>()/1000000.0;
				float area_ret = zone->template retail_area<Square_Feet>()/1000000.0;
				float area_ent = zone->template entertainment_area<Square_Feet>()/1000000.0;
				float area_ins = zone->template institutional_area<Square_Feet>()/1000000.0;
				float area_off = zone->template office_area<Square_Feet>()/1000000.0;
				float area_mix = zone->template mixed_use_area<Square_Feet>()/1000000.0;
				float area_sch = zone->template school_area<Square_Feet>()/1000000.0;
				float emp_oth = zone->template employment_other<double>()/1000.0;
				float emp_ind = zone->template employment_industrial<double>()/1000.0;
				float emp_man = zone->template employment_manufacturing<double>()/1000.0;
				float emp_gov = zone->template employment_government<double>()/1000.0;
				float emp_srv = zone->template employment_services<double>()/1000.0;
				float emp_ret = zone->template employment_retail<double>()/1000.0;

				float thetag = zone->template accessibility_employment_government<double>()/1000.0;
				float thetam = zone->template accessibility_employment_manufacturing<double>()/1000.0;
				float thetar = zone->template accessibility_employment_retail<double>()/1000.0;
				float thetas = zone->template accessibility_employment_services<double>()/1000.0;
				float thetai = zone->template accessibility_employment_industrial<double>()/1000.0;
				float thetao = zone->template accessibility_employment_other<double>()/1000.0;

				if (_activity_type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY || _activity_type == Activity_Components::Types::ACTIVITY_TYPES::PART_TIME_WORK_ACTIVITY)
				{
					Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE industry = static_properties->template Employment_Industry_Simple<Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE>();
					float EMPUR = 0;

					// Mode specific effects
					int ModAuto = 0, ModTran = 0,ModOth = 0;
					Person_Components::Types::JOURNEY_TO_WORK_MODE mode = static_properties->template Journey_To_Work_Mode<Person_Components::Types::JOURNEY_TO_WORK_MODE>();
					if (mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_AUTOMOBILE || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_MOTORCYCLE || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_TAXI)
						ModAuto = 1;
					else if (mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_BUS || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_RAILROAD || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_STREETCAR || mode == Person_Components::Types::JOURNEY_TO_WORK_MODE::WORKMODE_SUBWAY)
						ModTran = 1;
					else
						ModOth = 1;

					// positive impact of working near home
					float HOME = 0.0; 
					if (zone == _Parent_Person->template Home_Location<_Zone_Interface*>()) HOME = 1.0;

					// industry type interaction variables
					float IndR = 0, IndS = 0, IndG = 0, IndI = 0, IndM = 0, IndO = 0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::RETAIL) IndR = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::SERVICE) IndS = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::GOVERNMENT) IndG = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::INDUSTRIAL) IndI = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::MANUFACTURING) IndM = 1.0;
					if (industry == Person_Components::Types::EMPLOYMENT_INDUSTRY_SIMPLE::OTHER) IndO = 1.0;

					float area_res = log(zone->template residential_area<Square_Feet>()+1.0);
					float area_rec = log(zone->template other_area<Square_Feet>()+1.0);
					float area_ret = log(zone->template retail_area<Square_Feet>()+1.0);
					float area_ent = log(zone->template entertainment_area<Square_Feet>()+1.0);
					float area_ins = log(zone->template institutional_area<Square_Feet>()+1.0);
					float area_off = log(zone->template office_area<Square_Feet>()+1.0);
					float area_mix = log(zone->template mixed_use_area<Square_Feet>()+1.0);
					float area_sch = log(zone->template school_area<Square_Feet>()+1.0);

					// Unrelated employment
					EMPUR += (max<float>(zone->template employment_retail<float>(), 0.0) / 1000.0) * (-1.0 * (IndR - 1.0));
					EMPUR += (max<float>(zone->template employment_government<float>(), 0.0) / 1000.0) * (-1.0 * (IndG - 1.0));
					EMPUR += (max<float>(zone->template employment_services<float>(), 0.0) / 1000.0) * (-1.0 * (IndS - 1.0));
					EMPUR += (max<float>(zone->template employment_industrial<float>(), 0.0) / 1000.0) * (-1.0 * (IndI - 1.0));
					EMPUR += (max<float>(zone->template employment_manufacturing<float>(), 0.0) / 1000.0) * (-1.0 * (IndM - 1.0));
					EMPUR += (max<float>(zone->template employment_other<float>(), 0.0) / 1000.0) * (-1.0 * (IndO - 1.0));

					float theta_ur = thetar * (1.0 - IndR) + thetag * (1.0 - IndG) + thetas * (1.0 - IndS) + thetai * (1.0 - IndI) + thetam * (1.0 - IndM) + thetao * (1.0 - IndO);

					// cut travel time in half since original work location choice equations only account for the half-tour travel time
					ttime_deflected = ttime_deflected *0.5;

					// Old values - with time strata at (45A/60T/20W)
					u = _BArEnt_WORK * area_ent + _BArIns_WORK * area_ins + _BArOff_WORK * area_off + _BArRec_WORK * area_rec + _BArRet_WORK *area_ret + _BArRes_WORK * area_res + _BEmUnrelated_WORK * EMPUR + _BEmGov_WORK * (zone->template employment_government<float>()) / 1000.0 * IndG + _BEmMan_WORK * (zone->template employment_manufacturing<float>()) / 1000.0 * IndM + _BEmRet_WORK * (zone->template employment_retail<float>()) / 1000.0 * IndR + _BEmSer_WORK * (zone->template employment_services<float>()) / 1000.0 * IndS + _BHOME_WORK * HOME + _BTTAUTO_WORK * (ttime_deflected) * ModAuto + _BTTTRAN_WORK * (ttime_deflected) * ModTran + _BTTOTHER_WORK * (ttime_deflected) * ModOth + thetag * _THETAG_WORK + thetam * _THETAM_WORK + thetar * _THETAR_WORK + thetas * _THETAS_WORK + thetai * _THETAI_WORK + thetao * _THETAO_WORK + _THETA_UR_WORK * theta_ur;
					if (zone->template employment_total<int>() < 1.0) u = -9999999;
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
				
				cout << "Utility for zone " << zone->template uuid<int>() << " = " << u <<" from origin zone " <<_previous->template zone<_Zone_Interface*>()->template uuid<int>()<< ": ";
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

				//return (TargetType)u;				
			}


			static void base_static_initializer()
			{
				_BTT_PICK = -0.070956;
				_BINCD_PICK = -0.027821;
				_BRACED_PICK = -0.660094;
				_BArRes_PICK = 0.204766;
				_BArRec_PICK = 0.027283;
				_BArRet_PICK = 0.544154;
				_BArEnt_PICK = 0.271475;
				_BArIns_PICK = 0.167705;
				_BArOff_PICK = 0;
				_BArMix_PICK = 0.8175;
				_BArSch_PICK = 0.416465;
				_BEmOth_PICK = 1;
				_BEmInd_PICK = 0.320339;
				_BEmGov_PICK = 0.867538;
				_BEmSer_PICK = 0.765281;
				_BEmMan_PICK = 0.516197;
				_BEmRet_PICK = 0.62574;
				_THETAG_PICK = 0.137468;
				_THETAM_PICK = -0.516313;
				_THETAR_PICK = 0.027511;
				_THETAS_PICK = 0.011992;
				_THETAI_PICK = 0.08424;
				_THETAO_PICK = -0.30824;
				_GAMMA_PICK = 0.806092;
				_BTT_OTHER_WORK = -0.028552;
				_BINCD_OTHER_WORK = 0.038861;
				_BRACED_OTHER_WORK = -0.025711;
				_BArRes_OTHER_WORK = 0.128388;
				_BArRec_OTHER_WORK = 0.000000322;
				_BArRet_OTHER_WORK = 1.028168;
				_BArEnt_OTHER_WORK = 1.049801;
				_BArIns_OTHER_WORK = 0.236772;
				_BArOff_OTHER_WORK = 0.249596;
				_BArMix_OTHER_WORK = 0.47022;
				_BArSch_OTHER_WORK = 0.815026;
				_BEmOth_OTHER_WORK = 1;
				_BEmInd_OTHER_WORK = 0.250782;
				_BEmGov_OTHER_WORK = 0.464937;
				_BEmSer_OTHER_WORK = 0.768692;
				_BEmMan_OTHER_WORK = 0.363034;
				_BEmRet_OTHER_WORK = 0.419426;
				_THETAG_OTHER_WORK = 0.164304;
				_THETAM_OTHER_WORK = -0.225686;
				_THETAR_OTHER_WORK = -0.10824;
				_THETAS_OTHER_WORK = 0.048763;
				_THETAI_OTHER_WORK = -0.16035;
				_THETAO_OTHER_WORK = -0.333137;
				_GAMMA_OTHER_WORK = 0.659908;
				_BTT_SERVICE = -0.06;
				_BINCD_SERVICE = -0.07;
				_BRACED_SERVICE = -1.027;
				_BArRes_SERVICE = 0;
				_BArRec_SERVICE = 0.016;
				_BArRet_SERVICE = 0.466;
				_BArEnt_SERVICE = 0;
				_BArIns_SERVICE = 0.061;
				_BArOff_SERVICE = 0;
				_BArMix_SERVICE = 0.341;
				_BArSch_SERVICE = 0.348;
				_BEmOth_SERVICE = 0;
				_BEmInd_SERVICE = 0;
				_BEmGov_SERVICE = 0.527;
				_BEmSer_SERVICE = 0.966;
				_BEmMan_SERVICE = 0;
				_BEmRet_SERVICE = 1;
				_THETAG_SERVICE = 0;
				_THETAM_SERVICE = -0.407;
				_THETAR_SERVICE = 0;
				_THETAS_SERVICE = 0.029;
				_THETAI_SERVICE = -0.126;
				_THETAO_SERVICE = 0;
				_GAMMA_SERVICE = 0.635;
				_BArRec_MIN_SHOP = 0.011;
				_BTT_MIN_SHOP = -0.075;
				_BINCD_MIN_SHOP = -0.027;
				_BRACED_MIN_SHOP = -0.844;
				_BArRes_MIN_SHOP = 0;
				_BArRec_MAJ_SHOP = 0;
				_BArRet_MIN_SHOP = 4.14;
				_BArEnt_MIN_SHOP = 0;
				_BArIns_MIN_SHOP = 0.033;
				_BArOff_MIN_SHOP = 0;
				_BArMix_MIN_SHOP = 0.075;
				_BArSch_MIN_SHOP = 0;
				_BEmOth_MIN_SHOP = 0;
				_BEmInd_MIN_SHOP = 0;
				_BEmGov_MIN_SHOP = 0;
				_BEmSer_MIN_SHOP = 0;
				_BEmMan_MIN_SHOP = 0;
				_BEmRet_MIN_SHOP = 1;
				_THETAG_MIN_SHOP = 0;
				_THETAM_MIN_SHOP = -0.326;
				_THETAR_MIN_SHOP = 0.191;
				_THETAS_MIN_SHOP = -0.032;
				_THETAI_MIN_SHOP = -0.385;
				_THETAO_MIN_SHOP = 0.4;
				_GAMMA_MIN_SHOP = 0.434;
				_BTT_MAJ_SHOP = -0.062;
				_BINCD_MAJ_SHOP = 0;
				_BRACED_MAJ_SHOP = 0;
				_BArRes_MAJ_SHOP = 0;
				_BArRec_EAT_OUT = 0;
				_BArRet_MAJ_SHOP = 4.475;
				_BArEnt_MAJ_SHOP = 0;
				_BArIns_MAJ_SHOP = 0;
				_BArOff_MAJ_SHOP = 0;
				_BArMix_MAJ_SHOP = 0;
				_BArSch_MAJ_SHOP = 0;
				_BEmOth_MAJ_SHOP = 1;
				_BEmInd_MAJ_SHOP = 0;
				_BEmGov_MAJ_SHOP = 0;
				_BEmSer_MAJ_SHOP = 0;
				_BEmMan_MAJ_SHOP = 0;
				_BEmRet_MAJ_SHOP = 1.085;
				_THETAG_MAJ_SHOP = 0;
				_THETAM_MAJ_SHOP = 0;
				_THETAR_MAJ_SHOP = 0.355;
				_THETAS_MAJ_SHOP = -0.055;
				_THETAI_MAJ_SHOP = 0;
				_THETAO_MAJ_SHOP = 0;
				_GAMMA_MAJ_SHOP = 0.381;
				_BTT_EAT_OUT = -0.067;
				_BINCD_EAT_OUT = -0.056;
				_BRACED_EAT_OUT = -1.139;
				_BArRes_EAT_OUT = 0;
				_BArRec_CIVIC = 0;
				_BArRet_EAT_OUT = 4.241;
				_BArEnt_EAT_OUT = 0;
				_BArIns_EAT_OUT = 0;
				_BArOff_EAT_OUT = 0;
				_BArMix_EAT_OUT = 0;
				_BArSch_EAT_OUT = 0;
				_BEmOth_EAT_OUT = 1;
				_BEmInd_EAT_OUT = 0;
				_BEmGov_EAT_OUT = 0;
				_BEmSer_EAT_OUT = 0;
				_BEmMan_EAT_OUT = 0;
				_BEmRet_EAT_OUT = 2.815;
				_THETAG_EAT_OUT = 0;
				_THETAM_EAT_OUT = -0.296;
				_THETAR_EAT_OUT = -0.223;
				_THETAS_EAT_OUT = 0.056;
				_THETAI_EAT_OUT = -0.255;
				_THETAO_EAT_OUT = 0;
				_GAMMA_EAT_OUT = 0.274;
				_BTT_CIVIC = -0.076;
				_BINCD_CIVIC = -0.092;
				_BRACED_CIVIC = -2.009;
				_BArRes_CIVIC = 0.108;
				_BArRec_LEISURE = 0.017;
				_BArRet_CIVIC = 0;
				_BArEnt_CIVIC = 0;
				_BArIns_CIVIC = 0.028;
				_BArOff_CIVIC = 0;
				_BArMix_CIVIC = 0;
				_BArSch_CIVIC = 0.986;
				_BEmOth_CIVIC = 0;
				_BEmInd_CIVIC = 0;
				_BEmGov_CIVIC = 1.547;
				_BEmSer_CIVIC = 1;
				_BEmMan_CIVIC = 0;
				_BEmRet_CIVIC = 0;
				_THETAG_CIVIC = 0.201;
				_THETAM_CIVIC = 0;
				_THETAR_CIVIC = 0;
				_THETAS_CIVIC = 0;
				_THETAI_CIVIC = -0.711;
				_THETAO_CIVIC = 0;
				_GAMMA_CIVIC = 0.648;
				_BTT_LEISURE = -0.062;
				_BINCD_LEISURE = -0.046;
				_BRACED_LEISURE = -1.325;
				_BArRes_LEISURE = 0;
				_BArRec_SOCIAL = 0.043;
				_BArRet_LEISURE = 0.621;
				_BArEnt_LEISURE = 2.285;
				_BArIns_LEISURE = 0.035;
				_BArOff_LEISURE = 0;
				_BArMix_LEISURE = 0.712;
				_BArSch_LEISURE = 1;
				_BEmOth_LEISURE = 0;
				_BEmInd_LEISURE = 0;
				_BEmGov_LEISURE = 0.066;
				_BEmSer_LEISURE = 0;
				_BEmMan_LEISURE = 0;
				_BEmRet_LEISURE = 0;
				_THETAG_LEISURE = 0;
				_THETAM_LEISURE = -0.42;
				_THETAR_LEISURE = 0;
				_THETAS_LEISURE = 0.093;
				_THETAI_LEISURE = -0.619;
				_THETAO_LEISURE = 0;
				_GAMMA_LEISURE = 0.582;
				_BTT_SOCIAL = -0.059;
				_BINCD_SOCIAL = -0.058;
				_BRACED_SOCIAL = -0.969;
				_BArRes_SOCIAL = 0.091;
				_BArRec_OTHER = 0.490179;
				_BArRet_SOCIAL = 0.491;
				_BArEnt_SOCIAL = 0;
				_BArIns_SOCIAL = 0.15;
				_BArOff_SOCIAL = 0;
				_BArMix_SOCIAL = 0.717;
				_BArSch_SOCIAL = 0.305;
				_BEmOth_SOCIAL = 0;
				_BEmInd_SOCIAL = 0;
				_BEmGov_SOCIAL = 1.403;
				_BEmSer_SOCIAL = 1;
				_BEmMan_SOCIAL = 0;
				_BEmRet_SOCIAL = 0.942;
				_THETAG_SOCIAL = 0.19;
				_THETAM_SOCIAL = 0;
				_THETAR_SOCIAL = 0.285;
				_THETAS_SOCIAL = 0.051;
				_THETAI_SOCIAL = 0;
				_THETAO_SOCIAL = -1.228;
				_GAMMA_SOCIAL = 0.979;
				_BTT_OTHER = -0.150126;
				_BINCD_OTHER = 0;
				_BRACED_OTHER = -0.297174;
				_BArRes_OTHER = 1.021581;
				_BArRet_OTHER = 6.69056;
				_BArEnt_OTHER = 0;
				_BArIns_OTHER = 0;
				_BArOff_OTHER = 0;
				_BArMix_OTHER = 0;
				_BArSch_OTHER = 0.753482;
				_BEmOth_OTHER = 0;
				_BEmInd_OTHER = 0;
				_BEmGov_OTHER = 0;
				_BEmSer_OTHER = 0;
				_BEmMan_OTHER = 1;
				_BEmRet_OTHER = 0;
				_THETAG_OTHER = -0.014155;
				_THETAM_OTHER = -1.168986;
				_THETAR_OTHER = -0.339726;
				_THETAS_OTHER = 0.006282;
				_THETAI_OTHER = 0.331496;
				_THETAO_OTHER = 0.732551;
				_GAMMA_OTHER = 1;
				_BTTAUTO_WORK = -0.11;
				_BTTTRAN_WORK = -0.0677;
				_BTTOTHER_WORK = -0.0386;
				_BHOME_WORK = 2.23;
				_BINCD_WORK = 0;
				_BRACED_WORK = 0;
				_BArRes_WORK = -0.0304;
				_BArRec_WORK = 0.0218;
				_BArRet_WORK = 0.0287;
				_BArEnt_WORK = 0.0443;
				_BArIns_WORK = 0.0539;
				_BArOff_WORK = -0.00637;
				_BArMix_WORK = 0;
				_BArSch_WORK = 0;
				_BEmOth_WORK = 0;
				_BEmInd_WORK = 0;
				_BEmGov_WORK = 0.304;
				_BEmSer_WORK = 0.108;
				_BEmMan_WORK = 0.438;
				_BEmRet_WORK = 0.476;
				_BEmUnrelated_WORK = 0.0404;
				_THETAG_WORK = 0.0734;
				_THETAM_WORK = 0.184;
				_THETAR_WORK = -0.0266;
				_THETAS_WORK = 0.00686;
				_THETAI_WORK = 0.22;
				_THETAO_WORK = 0.244;
				_THETA_UR_WORK = -0.00831;
			}
		};

		// INITIALIZE DESTINATION MODEL STATIC PARAMETERS
#pragma region Choice option parameters	
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BTT_PICK   );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BINCD_PICK );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BRACED_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRes_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRec_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRet_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArEnt_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArIns_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArOff_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArMix_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArSch_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmOth_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmInd_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmGov_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmSer_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmMan_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmRet_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAG_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAM_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAR_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAS_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAI_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAO_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, GAMMA_PICK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BTT_OTHER_WORK   );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BINCD_OTHER_WORK );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BRACED_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRes_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRec_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRet_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArEnt_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArIns_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArOff_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArMix_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArSch_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmOth_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmInd_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmGov_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmSer_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmMan_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmRet_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAG_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAM_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAR_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAS_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAI_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAO_OTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, GAMMA_OTHER_WORK );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BTT_SERVICE      );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BINCD_SERVICE );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BRACED_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRes_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRec_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRet_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArEnt_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArIns_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArOff_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArMix_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArSch_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmOth_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmInd_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmGov_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmSer_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmMan_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmRet_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAG_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAM_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAR_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAS_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAI_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAO_SERVICE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, GAMMA_SERVICE );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRec_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BTT_MIN_SHOP   );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BINCD_MIN_SHOP );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BRACED_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRes_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRec_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRet_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArEnt_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArIns_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArOff_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArMix_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArSch_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmOth_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmInd_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmGov_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmSer_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmMan_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmRet_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAG_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAM_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAR_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAS_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAI_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAO_MIN_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, GAMMA_MIN_SHOP );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BTT_MAJ_SHOP   );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BINCD_MAJ_SHOP );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BRACED_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRes_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRec_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRet_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArEnt_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArIns_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArOff_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArMix_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArSch_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmOth_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmInd_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmGov_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmSer_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmMan_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmRet_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAG_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAM_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAR_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAS_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAI_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAO_MAJ_SHOP);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, GAMMA_MAJ_SHOP );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BTT_EAT_OUT   );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BINCD_EAT_OUT );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BRACED_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRes_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRec_CIVIC  );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRet_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArEnt_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArIns_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArOff_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArMix_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArSch_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmOth_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmInd_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmGov_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmSer_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmMan_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmRet_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAG_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAM_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAR_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAS_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAI_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAO_EAT_OUT);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, GAMMA_EAT_OUT );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BTT_CIVIC    );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BINCD_CIVIC  );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BRACED_CIVIC );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRes_CIVIC );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRec_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRet_CIVIC);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArEnt_CIVIC);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArIns_CIVIC);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArOff_CIVIC);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArMix_CIVIC);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArSch_CIVIC);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmOth_CIVIC);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmInd_CIVIC);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmGov_CIVIC);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmSer_CIVIC);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmMan_CIVIC);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmRet_CIVIC);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAG_CIVIC);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAM_CIVIC);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAR_CIVIC);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAS_CIVIC);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAI_CIVIC);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAO_CIVIC);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, GAMMA_CIVIC );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BTT_LEISURE );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BINCD_LEISURE );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BRACED_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRes_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRec_SOCIAL );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRet_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArEnt_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArIns_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArOff_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArMix_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArSch_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmOth_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmInd_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmGov_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmSer_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmMan_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmRet_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAG_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAM_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAR_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAS_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAI_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAO_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, GAMMA_LEISURE);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BTT_SOCIAL   );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BINCD_SOCIAL );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BRACED_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRes_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRec_OTHER );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRet_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArEnt_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArIns_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArOff_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArMix_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArSch_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmOth_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmInd_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmGov_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmSer_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmMan_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmRet_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAG_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAM_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAR_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAS_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAI_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAO_SOCIAL);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, GAMMA_SOCIAL );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BTT_OTHER   );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BINCD_OTHER );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BRACED_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRes_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRet_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArEnt_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArIns_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArOff_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArMix_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArSch_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmOth_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmInd_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmGov_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmSer_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmMan_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmRet_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAG_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAM_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAR_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAS_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAI_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAO_OTHER);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, GAMMA_OTHER );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BTTAUTO_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BTTTRAN_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BTTOTHER_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BHOME_WORK );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BINCD_WORK );
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BRACED_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRes_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRec_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArRet_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArEnt_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArIns_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArOff_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArMix_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BArSch_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmOth_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmInd_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmGov_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmSer_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmMan_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmRet_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, BEmUnrelated_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAG_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAM_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAR_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAS_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAI_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETAO_WORK);
		define_static_member_variable(ADAPTS_Destination_Choice_Option, THETA_UR_WORK);
#pragma endregion

		implementation struct ADAPTS_Destination_Chooser_Implementation : public Polaris_Component<MasterType,INHERIT(ADAPTS_Destination_Chooser_Implementation),Data_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(ADAPTS_Destination_Chooser_Implementation),Data_Object>::Component_Type ComponentType;

			// Pointer to the Parent class
			m_prototype(Prototypes::Person_Planner, typename MasterType::person_planner_type, Parent_Planner, NONE, NONE);
			m_prototype(Choice_Model_Components::Prototypes::Choice_Model, typename MasterType::mnl_model_type, Choice_Model, NONE, NONE);
			m_prototype(Activity_Components::Prototypes::Activity_Planner, typename MasterType::activity_type, Current_Activity, NONE, NONE);
			
			static m_data(int, choice_set_size, NONE, NONE);
			static m_data(int, num_strata, NONE, NONE);

			// Interface definitions
			typedef Choice_Model_Components::Prototypes::Choice_Model<typename MasterType::mnl_model_type > _Choice_Model_Interface;
			typedef Prototypes::Destination_Choice_Option<typename MasterType::person_destination_choice_option_type> _Destination_Choice_Option_Interface;
			typedef Choice_Model_Components::Prototypes::Choice_Option<typename MasterType::person_destination_choice_option_type> _Choice_Option_Interface;

			typedef Prototypes::Person<typename type_of(Parent_Planner)::type_of(Parent_Person)> person_itf;
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

			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(zone_ids_container),int> _Zone_Ids_Interface;

			//RLW%%%
			//typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Activity_Container)> Activity_Plans;
			//typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Plans)> Activity_Plan;
			
			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Movement_Plans_Container)> Movement_Plans;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan<get_component_type(Movement_Plans)> Movement_Plan;
			

			template<typename TargetType> void Initialize(/*requires(TargetType,check(typename ComponentType::Parent_Type,Concepts::Is_Person))*/)
			{	
			}

			template<typename ActivityItfType, typename ReturnType> ReturnType Choose_Destination(ActivityItfType activity, std::vector<ReturnType>* destinations_to_use=nullptr)
			{
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
				
				// create local choice model
				//Destination_Choice_Model_Implementation<MasterType> a;

				_Choice_Model_Interface* choice_model = (_Choice_Model_Interface*)Allocate<typename MasterType::mnl_model_type>();//(_Choice_Model_Interface*)&a;

				// set the current activity from input
				this->_Current_Activity = (Current_Activity_type)activity;

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();

				// selecte locations to choose from - use all if destinations to use not specified
				_Activity_Locations_Container_Interface* locations;
				if (destinations_to_use == nullptr) locations= network->template activity_locations_container<_Activity_Locations_Container_Interface*>();
				else locations = (_Activity_Locations_Container_Interface*)destinations_to_use;

				_Skim_Interface* LOS = network->template skimming_faculty<_Skim_Interface*>();


				// Create choice set
				std::vector<_Destination_Choice_Option_Interface*> loc_options;
				fill_stratified_choice_set<ReturnType>(locations,loc_options,choice_model);

				// Make choice
				int selected_index = 0;
				float logsum = choice_model->template Evaluate_Choices<NT>();

				// Get interface to chosen option
				_Choice_Option_Interface* selected = choice_model->template Choose<_Choice_Option_Interface*>(selected_index);

				// Validate the return value
				ReturnType return_ptr = nullptr;

				if (selected == nullptr) 
				{
					#ifdef SHOW_WARNINGS
					//THROW_WARNING("WARNING: selected is null - no destination choice made, index = " << selected_index <<", prev/next="<<prev->template zone<_Zone_Interface*>()->uuid<int>() << "/"<<next->template zone<_Zone_Interface*>()->uuid<int>());
					cout << "WARNING: selected is null - no destination choice made, index = " << selected_index << " of " << loc_options.size() << " options, utility values for options: "<<endl;
					for (int i=0; i<loc_options.size(); ++i)
					{
						((_Choice_Option_Interface*)loc_options[i])->Print_Utility();
					}
					cout << endl;
					#endif
				}
				else return_ptr = choice_model->template Choice_At<_Destination_Choice_Option_Interface*>(selected_index)->template destination<ReturnType>();

				// free memory allocated locally
				for (int i = 0; i < loc_options.size(); i++) Free<typename _Choice_Option_Interface::Component_Type>((typename _Choice_Option_Interface::Component_Type*)loc_options[i]);
				Free<typename MasterType::mnl_model_type>((typename MasterType::mnl_model_type*)choice_model);

				return return_ptr;	
			}

			template<typename ActivityItfType, typename ReturnType> ReturnType Evaluate_Destinations(ActivityItfType activity)
			{
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();

				_Choice_Model_Interface* choice_model = (_Choice_Model_Interface*)Allocate<typename MasterType::mnl_model_type>();//(_Choice_Model_Interface*)&a;

				// set the current activity from input
				this->_Current_Activity = (Current_Activity_type)activity;

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();

				// selecte locations to choose from - use all if destinations to use not specified
				_Activity_Locations_Container_Interface* locations;
				locations= network->template activity_locations_container<_Activity_Locations_Container_Interface*>();

				_Skim_Interface* LOS = network->template skimming_faculty<_Skim_Interface*>();


				// Create choice set
				std::vector<_Destination_Choice_Option_Interface*> loc_options;
				fill_stratified_choice_set<_Activity_Location_Interface*>(locations,loc_options,choice_model);

				// Make choice
				int selected_index = 0;
				ReturnType logsum = choice_model->template Evaluate_Choices<NT>();
				return logsum;
			}

			template<typename TargetType> TargetType Choose_Routine_Destination(Activity_Components::Types::ACTIVITY_TYPES act_type, std::vector<TargetType>* destinations_to_use=nullptr)
			{
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();

				_Current_Activity = nullptr;
				
				// create local choice model
				/*Destination_Choice_Model_Implementation<MasterType> a;
				_Choice_Model_Interface* choice_model = (_Choice_Model_Interface*)&a;*/
				_Choice_Model_Interface* choice_model = (_Choice_Model_Interface*)Allocate<typename MasterType::mnl_model_type>();//(_Choice_Model_Interface*)&a;


				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();

				// selecte locations to choose from - use all if destinations to use not specified
				_Activity_Locations_Container_Interface* locations;
				if (destinations_to_use == nullptr) locations= network->template activity_locations_container<_Activity_Locations_Container_Interface*>();
				else locations = (_Activity_Locations_Container_Interface*)destinations_to_use;

				_Skim_Interface* LOS = network->template skimming_faculty<_Skim_Interface*>();


				// Create choice set
				std::vector<_Destination_Choice_Option_Interface*> loc_options;
				fill_stratified_routine_choice_set<TargetType>(act_type, locations,loc_options,choice_model);

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
				else 
				{
					return_ptr = choice_model->template Choice_At<_Destination_Choice_Option_Interface*>(selected_index)->template destination<TargetType>();
				}

				// free memory allocated locally
				for (int i = 0; i < loc_options.size(); i++) Free<typename _Choice_Option_Interface::Component_Type>((typename _Choice_Option_Interface::Component_Type*)loc_options[i]);
				Free<typename MasterType::mnl_model_type>((typename MasterType::mnl_model_type*)choice_model);

				return return_ptr;
			}

			//template<typename TargetType> void fill_choice_set(_Activity_Locations_Container_Interface* available_set, std::vector<_Destination_Choice_Option_Interface*>& choice_set, _Choice_Model_Interface* choice_model, requires(TargetType,check(TargetType,is_pointer) && check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location)))
			//{
			//	// Get person context and system knowledge
			//	person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
			//	scheduler_itf* scheduler = _Parent_Person->Scheduling_Faculty<scheduler_itf*>();
			//	_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
			//	_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();
			//	_Skim_Interface* LOS = network->template skimming_faculty<_Skim_Interface*>();


			//	// Get preceding and following activities based on start time, otherwise assume plan a new tour startinga and ending at home
			//	Current_Activity_type prev_act, next_act;
			//	_Activity_Location_Interface* prev_loc, *next_loc;
			//	bool restrict_choice_set = true;
			//	if (this->_Current_Activity->Start_Is_Planned<bool>())
			//	{
			//		prev_act = (Current_Activity_type)(scheduler->previous_activity_plan<Time_Seconds,Current_Activity_type>(this->_Current_Activity->Start_Time<Time_Seconds>()));
			//		next_act = (Current_Activity_type)(scheduler->previous_activity_plan<Time_Seconds,Current_Activity_type>(this->_Current_Activity->Start_Time<Time_Seconds>()));
			//		// check previous act, if it is not known or if its location is not know, do not restrict current choice set
			//		if (prev_act == nullptr)
			//		{
			//			prev_loc = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
			//			restrict_choice_set = false;
			//		}
			//		else if (prev_act->Location_Is_Planned<bool>())
			//		{
			//			prev_loc = prev_act->Location<_Activity_Location_Interface*>();
			//			restrict_choice_set = true;
			//		}
			//		else
			//		{
			//			prev_loc = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
			//			restrict_choice_set = false;
			//		}
			//		// check next act, if it is not known or if its location is not know, do not restrict current choice set
			//		if (next_act == nullptr)
			//		{
			//			next_loc = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
			//			restrict_choice_set = false;
			//		}
			//		else if(next_act->Location_Is_Planned<bool>())
			//		{
			//			next_loc = next_act->Location<_Activity_Location_Interface*>();
			//			restrict_choice_set = true;
			//		}
			//		else
			//		{
			//			next_loc = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
			//			restrict_choice_set = false;
			//		}
			//	}
			//	else
			//	{
			//		prev_act = nullptr;
			//		next_act = nullptr;
			//		prev_loc = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
			//		next_loc = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
			//	}
			//	

			//	// variables used for utility calculation
			//	const int size = (int)available_set->size();
			//	int loc_id;


			//	// select zones to choose from and estimate utility
			//	for (int i=0; i<_choice_set_size; i++)
			//	{
			//		loc_id = (int)((Uniform_RNG.template Next_Rand<float>()*0.999999)*size);
			//		_Activity_Location_Interface* loc = (_Activity_Location_Interface*)(available_set->at(loc_id));
			//		
			//		_Destination_Choice_Option_Interface* choice = (_Destination_Choice_Option_Interface*)Allocate<typename MasterType::person_destination_choice_option_type>();
			//		choice->template bias_correction<float>(1.0);
			//		choice->template previous<_Activity_Location_Interface*>(prev_loc);
			//		choice->template destination<_Activity_Location_Interface*>(loc);
			//		choice->template next<_Activity_Location_Interface*>(next_loc);
			//		choice->template activity_type<Activity_Components::Types::ACTIVITY_TYPES>(this->_Current_Activity->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>());
			//		choice->template Parent_Planner<Parent_Planner_type>(_Parent_Planner);
			//		choice_model->template Add_Choice_Option<_Choice_Option_Interface*>((_Choice_Option_Interface*)choice);
			//		choice_set.push_back(choice);

			//		
			//	}
			//}

			template<typename TargetType> void fill_stratified_choice_set(_Activity_Locations_Container_Interface* available_set, std::vector<_Destination_Choice_Option_Interface*>& choice_set, _Choice_Model_Interface* choice_model, requires(TargetType,check(TargetType,is_pointer) && check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				int strata_size = _choice_set_size / _num_strata;
				const float EMP_SPLIT = 1000.0;
				float TTIME_SPLIT = 20.0;

				//----------------------------------------------------------------------------------------
				// Get person context and system knowledge
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
				scheduler_itf* scheduler = _Parent_Person->template Scheduling_Faculty<scheduler_itf*>();
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();
				_Skim_Interface* LOS = network->template skimming_faculty<_Skim_Interface*>();


				// Get preceding and following activities based on start time, otherwise assume plan a new tour starting and ending at home
				Current_Activity_type prev_act, next_act;
				Time_Minutes min_start, max_end, start_time;
				Time_Minutes avail_time = 1440.0;

				_Activity_Location_Interface* prev_loc, *next_loc;
				bool restrict_choice_set = true;
				if (this->_Current_Activity->Start_Is_Planned())
				{
					start_time = _Current_Activity->template Start_Time<Time_Minutes>();

					prev_act = (Current_Activity_type)(scheduler->template previous_activity_plan<Time_Seconds,Current_Activity_type>(GLOBALS::Time_Converter.template Convert_Value<Time_Minutes,Simulation_Timestep_Increment>(start_time)));
					next_act = (Current_Activity_type)(scheduler->template next_activity_plan<Time_Seconds,Current_Activity_type>(GLOBALS::Time_Converter.template Convert_Value<Time_Minutes,Simulation_Timestep_Increment>(start_time)));

					// check previous act, if it is not known or if its location is not know, do not restrict current choice set
					if (prev_act == nullptr)
					{
						prev_loc = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
						restrict_choice_set = false;
					}
					else if (prev_act->Location_Is_Planned())
					{
						prev_loc = prev_act->template Location<_Activity_Location_Interface*>();
						min_start = prev_act->template End_Time<Time_Minutes>();
					}
					else
					{
						prev_loc = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
						min_start = prev_act->template End_Time<Time_Minutes>();
					}
					// check next act, if it is not known or if its location is not know, do not restrict current choice set
					if (next_act == nullptr)
					{
						next_loc = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
						restrict_choice_set = false;
					}
					else if(next_act->Location_Is_Planned())
					{
						next_loc = next_act->template Location<_Activity_Location_Interface*>();
						max_end = next_act->template Start_Time<Time_Minutes>();
					}
					else
					{
						next_loc = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
						max_end = next_act->template Start_Time<Time_Minutes>();
					}

					// Use half the available time to stratify the zone choices
					if (restrict_choice_set)
					{
						avail_time = max_end - min_start - _Current_Activity->template Duration<Time_Minutes>();
						if (avail_time <= TTIME_SPLIT*2.0) TTIME_SPLIT = avail_time / 2.0;
					}
				}
				else
				{
					start_time = 540.0; // default start time to 9AM if not planned, for ttime purposes
					prev_act = nullptr;
					next_act = nullptr;
					prev_loc = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
					next_loc = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
					restrict_choice_set = false;
				}
				// double check that prev/next locations are set properly, if not assume start/end of tour at home
				if (prev_loc == nullptr) prev_loc = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
				if (next_loc == nullptr) next_loc = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();

				//----------------------------------------------------------------------
				// Get the mode of the activity, if not yet planned, assume 9AM
				Vehicle_Components::Types::Vehicle_Type_Keys mode = Vehicle_Components::Types::SOV;
				if (_Current_Activity->Mode_Is_Planned()) mode = _Current_Activity->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>();

				Activity_Components::Types::ACTIVITY_TYPES act_type = _Current_Activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();


				//----------------------------------------------------------------------
				// Get the stratified availability sets
				std::vector<_Zone_Interface*> zones_near;
				LOS->template Get_Locations_Within_Range<_Activity_Location_Interface*, Time_Minutes, Vehicle_Components::Types::Vehicle_Type_Keys, _Zone_Interface*>(zones_near, prev_loc, start_time, 0, TTIME_SPLIT, mode, true);
				std::vector<_Zone_Interface*> zones_far;
				LOS->template Get_Locations_Within_Range<_Activity_Location_Interface*, Time_Minutes, Vehicle_Components::Types::Vehicle_Type_Keys, _Zone_Interface*>(zones_far, prev_loc, start_time, TTIME_SPLIT, avail_time, mode, false);
				std::vector<std::vector<_Zone_Interface*>> available_zones; //0=near-high, 1=near-low, 2=far-high, 3=far-low

				for (int i=0; i<_num_strata; ++i) available_zones.push_back(std::vector<_Zone_Interface*>());

				typename std::vector<_Zone_Interface*>::iterator z_itr;
				for (z_itr = zones_near.begin(); z_itr != zones_near.end(); ++z_itr)
				{
					_Zone_Interface* zone = (_Zone_Interface*)(*z_itr);
					if (zone->template employment_total<int>() > EMP_SPLIT) available_zones[0].push_back(zone);
					else available_zones[1].push_back(zone);
				}
				for (z_itr = zones_far.begin(); z_itr != zones_far.end(); ++z_itr)
				{
					_Zone_Interface* zone = (_Zone_Interface*)(*z_itr);
					if (zone->template employment_total<int>() > EMP_SPLIT) available_zones[2].push_back(zone);
					else available_zones[3].push_back(zone);
				}				

				//----------------------------------------------------------------------
				// Next, select zones to choose from each strata
				for (int i=0; i<_num_strata; i++)
				{
					int available_count = (int)available_zones[i].size();
					// if too few zones to fill strata, add all and set bias correction to 1
					if (available_count <= strata_size)
					{
						for (z_itr = available_zones[i].begin(); z_itr != available_zones[i].end(); ++z_itr)
						{
							Create_New_Choice_Option(choice_set,choice_model,(_Zone_Interface*)(*z_itr),act_type,prev_loc,next_loc,1.0/*,avail_time<1440*/);
						}
					}
					// otherwise, pick randomly from the available zones
					else
					{
						float bias = 1.0/( (float) strata_size / (float) available_count );

						int num_added=0;
						while (num_added < strata_size)
						{
							int r = int( (GLOBALS::Uniform_RNG.Next_Rand<float>()-0.0001) * (float)available_count );
							_Zone_Interface* zone = available_zones[i][r];
							Create_New_Choice_Option(choice_set,choice_model,zone,act_type,prev_loc,next_loc,bias/*,avail_time<1440*/);
							num_added++;
						}

					}
				}
			}

			//template<typename TargetType> void fill_routine_choice_set(Activity_Components::Types::ACTIVITY_TYPES act_type, _Activity_Locations_Container_Interface* available_set, std::vector<_Destination_Choice_Option_Interface*>& choice_set, _Choice_Model_Interface* choice_model, requires(TargetType,check(TargetType,is_pointer) && check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location)))
			//{
			//	// Get person context and system knowledge
			//	person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
			//	scheduler_itf* scheduler = _Parent_Person->Scheduling_Faculty<scheduler_itf*>();
			//	_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
			//	_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();
			//	_Zone_Ids_Interface& zone_ids = network->template zone_ids_container<_Zone_Ids_Interface&>();
			//	_Skim_Interface* LOS = network->template skimming_faculty<_Skim_Interface*>();
			//	_Zones_Container_Interface::iterator zone_itr;

			//	// Get preceding and following activities based on start time, otherwise assume plan a new tour startinga and ending at home
			//	Current_Activity_type prev_act, *next_act;
			//	_Activity_Location_Interface* prev_loc, *next_loc;
			//	bool restrict_choice_set = true;

			//	prev_act = nullptr;
			//	next_act = nullptr;
			//	prev_loc = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
			//	next_loc = _Parent_Person->Home_Location<_Activity_Location_Interface*>();


			//	// select zones to choose from and estimate utility 
			//	for (zone_itr = zones->begin(); zone_itr != zones->end(); ++zone_itr)
			//	{
			//		// First choose a random zone from the zone std::list
			//		_Zone_Interface* zone = (_Zone_Interface*)zone_itr->second; //network->get_random_zone<_Zone_Interface*>();


			//		// Get random location within that zone
			//		_Activity_Location_Interface* loc = zone->Get_Random_Location<_Activity_Location_Interface*>();

			//		// try to add a random suitable location up to ten times, if failed 10 times then ignore zone
			//		int failed_attempts = 0;
			//		while (failed_attempts < 10 && ((act_type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY || act_type == Activity_Components::Types::ACTIVITY_TYPES::PART_TIME_WORK_ACTIVITY) && !loc->is_work_location<bool>()) || (act_type == Activity_Components::Types::ACTIVITY_TYPES::SCHOOL_ACTIVITY && !loc->is_school_location<bool>()))
			//		{
			//			loc = zone->Get_Random_Location<_Activity_Location_Interface*>();
			//			failed_attempts++;
			//		}
			//		if (failed_attempts >= 10) continue;

			//		_Destination_Choice_Option_Interface* choice = (_Destination_Choice_Option_Interface*)Allocate<typename MasterType::person_destination_choice_option_type>();
			//		choice->template initialize<NT>();
			//		choice->template previous<_Activity_Location_Interface*>(prev_loc);
			//		choice->template destination<_Activity_Location_Interface*>(loc);
			//		choice->template next<_Activity_Location_Interface*>(next_loc);
			//		choice->template activity_type<Activity_Components::Types::ACTIVITY_TYPES>(act_type);
			//		choice->template Parent_Planner<Parent_Planner_type>(_Parent_Planner);
			//		choice_model->template Add_Choice_Option<_Choice_Option_Interface*>((_Choice_Option_Interface*)choice);
			//		choice_set.push_back(choice);
			//	}
			//}
			//

			template<typename TargetType> void fill_stratified_routine_choice_set(Activity_Components::Types::ACTIVITY_TYPES act_type, _Activity_Locations_Container_Interface* available_set, std::vector<_Destination_Choice_Option_Interface*>& choice_set, _Choice_Model_Interface* choice_model, requires(TargetType,check(TargetType,is_pointer) && check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				int strata_size = _choice_set_size / _num_strata;
				const float EMP_SPLIT = 1000.0;
				const float TTIME_SPLIT = 45.0;

				//----------------------------------------------------------------------
				// Get person context and system knowledge
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
				scheduler_itf* scheduler = _Parent_Person->template Scheduling_Faculty<scheduler_itf*>();
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();
				_Zone_Ids_Interface& zone_ids = network->template zone_ids_container<_Zone_Ids_Interface&>();
				_Skim_Interface* LOS = network->template skimming_faculty<_Skim_Interface*>();
				typename _Zones_Container_Interface::iterator zone_itr;

				//----------------------------------------------------------------------
				// Get preceding and following activities based on start time, otherwise assume plan a new tour startinga and ending at home
				Current_Activity_type prev_act, next_act;
				_Activity_Location_Interface* prev_loc, *next_loc;
				bool restrict_choice_set = true;

				prev_act = nullptr;
				next_act = nullptr;
				prev_loc = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
				next_loc = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();

				//----------------------------------------------------------------------
				// Get the start time of the activity, if not yet planned, assume 9AM
				Time_Minutes start_time = 9.0 * 60.0;
				if (_Current_Activity!=nullptr)
				{
					if (_Current_Activity->Start_Is_Planned()) start_time = _Current_Activity->template Start_Time<Time_Minutes>();
				}

				//----------------------------------------------------------------------
				// Get the mode of the activity, if not yet planned, assume 9AM
				Vehicle_Components::Types::Vehicle_Type_Keys mode = Vehicle_Components::Types::SOV;
				if (_Current_Activity!=nullptr)
				{
					if (_Current_Activity->Mode_Is_Planned()) mode = _Current_Activity->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>();
				}


				//----------------------------------------------------------------------
				// Get the stratified availability sets
				std::vector<_Zone_Interface*> zones_near;
				LOS->template Get_Locations_Within_Range<_Activity_Location_Interface*, Time_Minutes, Vehicle_Components::Types::Vehicle_Type_Keys, _Zone_Interface*>(zones_near, prev_loc, start_time, 0, TTIME_SPLIT, mode, true);
				std::vector<_Zone_Interface*> zones_far;
				LOS->template Get_Locations_Within_Range<_Activity_Location_Interface*, Time_Minutes, Vehicle_Components::Types::Vehicle_Type_Keys, _Zone_Interface*>(zones_far, prev_loc, start_time, TTIME_SPLIT, 1440, mode, false);
				std::vector<std::vector<_Zone_Interface*>> available_zones; //0=near-high, 1=near-low, 2=far-high, 3=far-low

				for (int i=0; i<_num_strata; ++i) available_zones.push_back(std::vector<_Zone_Interface*>());

				typename std::vector<_Zone_Interface*>::iterator z_itr;
				for (z_itr = zones_near.begin(); z_itr != zones_near.end(); ++z_itr)
				{
					_Zone_Interface* zone = (_Zone_Interface*)(*z_itr);
					if (zone->template employment_total<int>() > EMP_SPLIT) available_zones[0].push_back(zone);
					else available_zones[1].push_back(zone);
				}
				for (z_itr = zones_far.begin(); z_itr != zones_far.end(); ++z_itr)
				{
					_Zone_Interface* zone = (_Zone_Interface*)(*z_itr);
					if (zone->template employment_total<int>() > EMP_SPLIT) available_zones[2].push_back(zone);
					else available_zones[3].push_back(zone);
				}


				//----------------------------------------------------------------------
				// First, always add the home zone (i.e. work at home or nearby or attend school near home) as an option
				_Zone_Interface* home_zone = _Parent_Person->template Home_Location<_Zone_Interface*>();
				//Create_New_Choice_Option(choice_set,choice_model,home_zone,act_type,prev_loc,next_loc);


				//----------------------------------------------------------------------
				// Next, select zones to choose from each strata
				for (int i=0; i<_num_strata; i++)
				{
					int available_count = (int)available_zones[i].size();
					// if too few zones to fill strata, add all and set bias correction to 1
					if (available_count <= strata_size)
					{
						for (z_itr = available_zones[i].begin(); z_itr != available_zones[i].end(); ++z_itr)
						{
							Create_New_Choice_Option(choice_set,choice_model,(_Zone_Interface*)(*z_itr),act_type,prev_loc,next_loc);
						}
					}
					// otherwise, pick randomly from the available zones
					else
					{
						float bias = 1.0/( (float) strata_size / (float) available_count );

						int num_added=0;
						while (num_added < strata_size)
						{
							int r = int( (GLOBALS::Uniform_RNG.Next_Rand<float>()-0.0001) * (float)available_count );
							_Zone_Interface* zone = available_zones[i][r];
							Create_New_Choice_Option(choice_set,choice_model,zone,act_type,prev_loc,next_loc,bias);
							num_added++;
						}

					}
				}
			}
		
			void Create_New_Choice_Option(std::vector<_Destination_Choice_Option_Interface*>& choice_set, _Choice_Model_Interface* choice_model,_Zone_Interface* zone, Activity_Components::Types::ACTIVITY_TYPES act_type, _Activity_Location_Interface* prev_loc, _Activity_Location_Interface* next_loc, float bias_correction=1.0, bool display=false)
			{
				// ignore zone if all employment slots have already been assigned to other agents
				if ((act_type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY || act_type == Activity_Components::Types::ACTIVITY_TYPES::PART_TIME_WORK_ACTIVITY) && zone->template employment_simulated<int>() >= zone->template employment_total<int>()) return;

				// Get random location within that zone
				_Activity_Location_Interface* loc;

				// try to add a random suitable location, if failed then ignore zone
				if(act_type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY || act_type == Activity_Components::Types::ACTIVITY_TYPES::PART_TIME_WORK_ACTIVITY)
				{
					loc = zone->template Get_Random_Work_Location<_Activity_Location_Interface*>();
				}
				else if (act_type == Activity_Components::Types::ACTIVITY_TYPES::SCHOOL_ACTIVITY)
				{
					loc = zone->template Get_Random_School_Location<_Activity_Location_Interface*>();
				}
				else
				{
					loc = zone->template Get_Random_Location<_Activity_Location_Interface*>();
				}

				if (loc == nullptr) return;

				// set the start time, if not planned, assume 9AM
				Time_Minutes start_time = 540.0;
				if (_Current_Activity != nullptr)
				{
					if (_Current_Activity->Start_Is_Planned()) start_time = _Current_Activity->template Start_Time<Time_Minutes>();
				}


				_Destination_Choice_Option_Interface* choice = (_Destination_Choice_Option_Interface*)Allocate<typename MasterType::person_destination_choice_option_type>();
				choice->template bias_correction<int>(bias_correction);
				choice->template previous<_Activity_Location_Interface*>(prev_loc);
				choice->template destination<_Activity_Location_Interface*>(loc);
				choice->template next<_Activity_Location_Interface*>(next_loc);
				choice->template activity_type<Activity_Components::Types::ACTIVITY_TYPES>(act_type);
				choice->template Parent_Planner<Parent_Planner_type>(_Parent_Planner);
				choice->template start_time<Time_Minutes>(start_time);
				choice_model->template Add_Choice_Option<_Choice_Option_Interface*>((_Choice_Option_Interface*)choice);
				choice_set.push_back(choice);
			}
		};
		#pragma region Choice option parameters
		// INITIALIZE DESTINATION MODEL STATIC PARAMETERS
		template<typename MasterType, typename InheritanceList> typename ADAPTS_Destination_Chooser_Implementation<MasterType, InheritanceList>::type_of(choice_set_size) ADAPTS_Destination_Chooser_Implementation<MasterType,InheritanceList>::_choice_set_size;
		template<typename MasterType, typename InheritanceList> typename ADAPTS_Destination_Chooser_Implementation<MasterType, InheritanceList>::type_of(num_strata) ADAPTS_Destination_Chooser_Implementation<MasterType,InheritanceList>::_num_strata=4;
		#pragma endregion

	}
}


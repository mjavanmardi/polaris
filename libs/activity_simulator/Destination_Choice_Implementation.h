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

			static bool static_initialize(const string& option_file)
			{
				// set the base values
				default_static_initializer();

				// now see if there are config file changes
				Scenario_Components::Types::ScenarioData document;
				if (option_file.length() < 1)
				{
					cout << "Warning: option file for ADAPTS_Destination_Choice_Option was not specified" << endl;
					return true;
				}

				_Scenario_Interface* scenario = static_cast<_Scenario_Interface*>(_global_scenario);
				if (!scenario->parse_option_file(document, option_file))
					return false;

				scenario->set_parameter<float>(document, "BTT_PICK"   , _BTT_PICK	  	);
				scenario->set_parameter<float>(document, "BINCD_PICK" , _BINCD_PICK 	);
				scenario->set_parameter<float>(document, "BRACED_PICK", _BRACED_PICK	);
				scenario->set_parameter<float>(document, "BArRes_PICK", _BArRes_PICK	);
				scenario->set_parameter<float>(document, "BArRec_PICK", _BArRec_PICK	);
				scenario->set_parameter<float>(document, "BArRet_PICK", _BArRet_PICK	);
				scenario->set_parameter<float>(document, "BArEnt_PICK", _BArEnt_PICK	);
				scenario->set_parameter<float>(document, "BArIns_PICK", _BArIns_PICK	);
				scenario->set_parameter<float>(document, "BArOff_PICK", _BArOff_PICK	);
				scenario->set_parameter<float>(document, "BArMix_PICK", _BArMix_PICK	);
				scenario->set_parameter<float>(document, "BArSch_PICK", _BArSch_PICK	);
				scenario->set_parameter<float>(document, "BEmOth_PICK", _BEmOth_PICK	);
				scenario->set_parameter<float>(document, "BEmInd_PICK", _BEmInd_PICK	);
				scenario->set_parameter<float>(document, "BEmGov_PICK", _BEmGov_PICK	);
				scenario->set_parameter<float>(document, "BEmSer_PICK", _BEmSer_PICK	);
				scenario->set_parameter<float>(document, "BEmMan_PICK", _BEmMan_PICK	);
				scenario->set_parameter<float>(document, "BEmRet_PICK", _BEmRet_PICK	);
				scenario->set_parameter<float>(document, "THETAG_PICK", _THETAG_PICK	);
				scenario->set_parameter<float>(document, "THETAM_PICK", _THETAM_PICK	);
				scenario->set_parameter<float>(document, "THETAR_PICK", _THETAR_PICK	);
				scenario->set_parameter<float>(document, "THETAS_PICK", _THETAS_PICK	);
				scenario->set_parameter<float>(document, "THETAI_PICK", _THETAI_PICK	);
				scenario->set_parameter<float>(document, "THETAO_PICK", _THETAO_PICK	);
				scenario->set_parameter<float>(document, "GAMMA_PICK" , _GAMMA_PICK 	);

				scenario->set_parameter<float>(document, "BTT_OTHER_WORK"	  , _BTT_OTHER_WORK		);
				scenario->set_parameter<float>(document, "BINCD_OTHER_WORK" , _BINCD_OTHER_WORK	);
				scenario->set_parameter<float>(document, "BRACED_OTHER_WORK", _BRACED_OTHER_WORK	);
				scenario->set_parameter<float>(document, "BArRes_OTHER_WORK", _BArRes_OTHER_WORK	);
				scenario->set_parameter<float>(document, "BArRec_OTHER_WORK", _BArRec_OTHER_WORK	);
				scenario->set_parameter<float>(document, "BArRet_OTHER_WORK", _BArRet_OTHER_WORK	);
				scenario->set_parameter<float>(document, "BArEnt_OTHER_WORK", _BArEnt_OTHER_WORK	);
				scenario->set_parameter<float>(document, "BArIns_OTHER_WORK", _BArIns_OTHER_WORK	);
				scenario->set_parameter<float>(document, "BArOff_OTHER_WORK", _BArOff_OTHER_WORK	);
				scenario->set_parameter<float>(document, "BArMix_OTHER_WORK", _BArMix_OTHER_WORK	);
				scenario->set_parameter<float>(document, "BArSch_OTHER_WORK", _BArSch_OTHER_WORK	);
				scenario->set_parameter<float>(document, "BEmOth_OTHER_WORK", _BEmOth_OTHER_WORK	);
				scenario->set_parameter<float>(document, "BEmInd_OTHER_WORK", _BEmInd_OTHER_WORK	);
				scenario->set_parameter<float>(document, "BEmGov_OTHER_WORK", _BEmGov_OTHER_WORK	);
				scenario->set_parameter<float>(document, "BEmSer_OTHER_WORK", _BEmSer_OTHER_WORK	);
				scenario->set_parameter<float>(document, "BEmMan_OTHER_WORK", _BEmMan_OTHER_WORK	);
				scenario->set_parameter<float>(document, "BEmRet_OTHER_WORK", _BEmRet_OTHER_WORK	);
				scenario->set_parameter<float>(document, "THETAG_OTHER_WORK", _THETAG_OTHER_WORK	);
				scenario->set_parameter<float>(document, "THETAM_OTHER_WORK", _THETAM_OTHER_WORK	);
				scenario->set_parameter<float>(document, "THETAR_OTHER_WORK", _THETAR_OTHER_WORK	);
				scenario->set_parameter<float>(document, "THETAS_OTHER_WORK", _THETAS_OTHER_WORK	);
				scenario->set_parameter<float>(document, "THETAI_OTHER_WORK", _THETAI_OTHER_WORK	);
				scenario->set_parameter<float>(document, "THETAO_OTHER_WORK", _THETAO_OTHER_WORK	);
				scenario->set_parameter<float>(document, "GAMMA_OTHER_WORK",  _GAMMA_OTHER_WORK 	);

				scenario->set_parameter<float>(document, "BTT_SERVICE"   , _BTT_SERVICE		);
				scenario->set_parameter<float>(document, "BINCD_SERVICE" , _BINCD_SERVICE		);
				scenario->set_parameter<float>(document, "BRACED_SERVICE", _BRACED_SERVICE	);
				scenario->set_parameter<float>(document, "BArRes_SERVICE", _BArRes_SERVICE	);
				scenario->set_parameter<float>(document, "BArRec_SERVICE", _BArRec_SERVICE	);
				scenario->set_parameter<float>(document, "BArRet_SERVICE", _BArRet_SERVICE	);
				scenario->set_parameter<float>(document, "BArEnt_SERVICE", _BArEnt_SERVICE	);
				scenario->set_parameter<float>(document, "BArIns_SERVICE", _BArIns_SERVICE	);
				scenario->set_parameter<float>(document, "BArOff_SERVICE", _BArOff_SERVICE	);
				scenario->set_parameter<float>(document, "BArMix_SERVICE", _BArMix_SERVICE	);
				scenario->set_parameter<float>(document, "BArSch_SERVICE", _BArSch_SERVICE	);
				scenario->set_parameter<float>(document, "BEmOth_SERVICE", _BEmOth_SERVICE	);
				scenario->set_parameter<float>(document, "BEmInd_SERVICE", _BEmInd_SERVICE	);
				scenario->set_parameter<float>(document, "BEmGov_SERVICE", _BEmGov_SERVICE	);
				scenario->set_parameter<float>(document, "BEmSer_SERVICE", _BEmSer_SERVICE	);
				scenario->set_parameter<float>(document, "BEmMan_SERVICE", _BEmMan_SERVICE	);
				scenario->set_parameter<float>(document, "BEmRet_SERVICE", _BEmRet_SERVICE	);
				scenario->set_parameter<float>(document, "THETAG_SERVICE", _THETAG_SERVICE	);
				scenario->set_parameter<float>(document, "THETAM_SERVICE", _THETAM_SERVICE	);
				scenario->set_parameter<float>(document, "THETAR_SERVICE", _THETAR_SERVICE	);
				scenario->set_parameter<float>(document, "THETAS_SERVICE", _THETAS_SERVICE	);
				scenario->set_parameter<float>(document, "THETAI_SERVICE", _THETAI_SERVICE	);
				scenario->set_parameter<float>(document, "THETAO_SERVICE", _THETAO_SERVICE	);
				scenario->set_parameter<float>(document, "GAMMA_SERVICE" , _GAMMA_SERVICE		);

				scenario->set_parameter<float>(document, "BTT_MIN_SHOP"	, _BTT_MIN_SHOP		);
				scenario->set_parameter<float>(document, "BINCD_MIN_SHOP" , _BINCD_MIN_SHOP	);
				scenario->set_parameter<float>(document, "BRACED_MIN_SHOP", _BRACED_MIN_SHOP	);
				scenario->set_parameter<float>(document, "BArRes_MIN_SHOP", _BArRes_MIN_SHOP	);
				scenario->set_parameter<float>(document, "BArRec_MIN_SHOP", _BArRec_MIN_SHOP	);
				scenario->set_parameter<float>(document, "BArRet_MIN_SHOP", _BArRet_MIN_SHOP	);
				scenario->set_parameter<float>(document, "BArEnt_MIN_SHOP", _BArEnt_MIN_SHOP	);
				scenario->set_parameter<float>(document, "BArIns_MIN_SHOP", _BArIns_MIN_SHOP	);
				scenario->set_parameter<float>(document, "BArOff_MIN_SHOP", _BArOff_MIN_SHOP	);
				scenario->set_parameter<float>(document, "BArMix_MIN_SHOP", _BArMix_MIN_SHOP	);
				scenario->set_parameter<float>(document, "BArSch_MIN_SHOP", _BArSch_MIN_SHOP	);
				scenario->set_parameter<float>(document, "BEmOth_MIN_SHOP", _BEmOth_MIN_SHOP	);
				scenario->set_parameter<float>(document, "BEmInd_MIN_SHOP", _BEmInd_MIN_SHOP	);
				scenario->set_parameter<float>(document, "BEmGov_MIN_SHOP", _BEmGov_MIN_SHOP	);
				scenario->set_parameter<float>(document, "BEmSer_MIN_SHOP", _BEmSer_MIN_SHOP	);
				scenario->set_parameter<float>(document, "BEmMan_MIN_SHOP", _BEmMan_MIN_SHOP	);
				scenario->set_parameter<float>(document, "BEmRet_MIN_SHOP", _BEmRet_MIN_SHOP	);
				scenario->set_parameter<float>(document, "THETAG_MIN_SHOP", _THETAG_MIN_SHOP	);
				scenario->set_parameter<float>(document, "THETAM_MIN_SHOP", _THETAM_MIN_SHOP	);
				scenario->set_parameter<float>(document, "THETAR_MIN_SHOP", _THETAR_MIN_SHOP	);
				scenario->set_parameter<float>(document, "THETAS_MIN_SHOP", _THETAS_MIN_SHOP	);
				scenario->set_parameter<float>(document, "THETAI_MIN_SHOP", _THETAI_MIN_SHOP	);
				scenario->set_parameter<float>(document, "THETAO_MIN_SHOP", _THETAO_MIN_SHOP	);
				scenario->set_parameter<float>(document, "GAMMA_MIN_SHOP" , _GAMMA_MIN_SHOP 	);

				scenario->set_parameter<float>(document, "BTT_MAJ_SHOP"   , _BTT_MAJ_SHOP   	);
				scenario->set_parameter<float>(document, "BINCD_MAJ_SHOP" , _BINCD_MAJ_SHOP 	);
				scenario->set_parameter<float>(document, "BRACED_MAJ_SHOP", _BRACED_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "BArRes_MAJ_SHOP", _BArRes_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "BArRec_MAJ_SHOP", _BArRec_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "BArRet_MAJ_SHOP", _BArRet_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "BArEnt_MAJ_SHOP", _BArEnt_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "BArIns_MAJ_SHOP", _BArIns_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "BArOff_MAJ_SHOP", _BArOff_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "BArMix_MAJ_SHOP", _BArMix_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "BArSch_MAJ_SHOP", _BArSch_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "BEmOth_MAJ_SHOP", _BEmOth_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "BEmInd_MAJ_SHOP", _BEmInd_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "BEmGov_MAJ_SHOP", _BEmGov_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "BEmSer_MAJ_SHOP", _BEmSer_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "BEmMan_MAJ_SHOP", _BEmMan_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "BEmRet_MAJ_SHOP", _BEmRet_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "THETAG_MAJ_SHOP", _THETAG_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "THETAM_MAJ_SHOP", _THETAM_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "THETAR_MAJ_SHOP", _THETAR_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "THETAS_MAJ_SHOP", _THETAS_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "THETAI_MAJ_SHOP", _THETAI_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "THETAO_MAJ_SHOP", _THETAO_MAJ_SHOP	);
				scenario->set_parameter<float>(document, "GAMMA_MAJ_SHOP" , _GAMMA_MAJ_SHOP 	);

				scenario->set_parameter<float>(document, "BTT_EAT_OUT"   , _BTT_EAT_OUT		);
				scenario->set_parameter<float>(document, "BINCD_EAT_OUT" , _BINCD_EAT_OUT		);
				scenario->set_parameter<float>(document, "BRACED_EAT_OUT", _BRACED_EAT_OUT	);
				scenario->set_parameter<float>(document, "BArRes_EAT_OUT", _BArRes_EAT_OUT	);
				scenario->set_parameter<float>(document, "BArRec_EAT_OUT", _BArRec_EAT_OUT	);
				scenario->set_parameter<float>(document, "BArRet_EAT_OUT", _BArRet_EAT_OUT	);
				scenario->set_parameter<float>(document, "BArEnt_EAT_OUT", _BArEnt_EAT_OUT	);
				scenario->set_parameter<float>(document, "BArIns_EAT_OUT", _BArIns_EAT_OUT	);
				scenario->set_parameter<float>(document, "BArOff_EAT_OUT", _BArOff_EAT_OUT	);
				scenario->set_parameter<float>(document, "BArMix_EAT_OUT", _BArMix_EAT_OUT	);
				scenario->set_parameter<float>(document, "BArSch_EAT_OUT", _BArSch_EAT_OUT	);
				scenario->set_parameter<float>(document, "BEmOth_EAT_OUT", _BEmOth_EAT_OUT	);
				scenario->set_parameter<float>(document, "BEmInd_EAT_OUT", _BEmInd_EAT_OUT	);
				scenario->set_parameter<float>(document, "BEmGov_EAT_OUT", _BEmGov_EAT_OUT	);
				scenario->set_parameter<float>(document, "BEmSer_EAT_OUT", _BEmSer_EAT_OUT	);
				scenario->set_parameter<float>(document, "BEmMan_EAT_OUT", _BEmMan_EAT_OUT	);
				scenario->set_parameter<float>(document, "BEmRet_EAT_OUT", _BEmRet_EAT_OUT	);
				scenario->set_parameter<float>(document, "THETAG_EAT_OUT", _THETAG_EAT_OUT	);
				scenario->set_parameter<float>(document, "THETAM_EAT_OUT", _THETAM_EAT_OUT	);
				scenario->set_parameter<float>(document, "THETAR_EAT_OUT", _THETAR_EAT_OUT	);
				scenario->set_parameter<float>(document, "THETAS_EAT_OUT", _THETAS_EAT_OUT	);
				scenario->set_parameter<float>(document, "THETAI_EAT_OUT", _THETAI_EAT_OUT	);
				scenario->set_parameter<float>(document, "THETAO_EAT_OUT", _THETAO_EAT_OUT	);
				scenario->set_parameter<float>(document, "GAMMA_EAT_OUT", _GAMMA_EAT_OUT		);

				scenario->set_parameter<float>(document, "BTT_CIVIC"   , _BTT_CIVIC   	);
				scenario->set_parameter<float>(document, "BINCD_CIVIC" , _BINCD_CIVIC 	);
				scenario->set_parameter<float>(document, "BRACED_CIVIC", _BRACED_CIVIC	);
				scenario->set_parameter<float>(document, "BArRes_CIVIC", _BArRes_CIVIC	);
				scenario->set_parameter<float>(document, "BArRec_CIVIC", _BArRec_CIVIC	);
				scenario->set_parameter<float>(document, "BArRet_CIVIC", _BArRet_CIVIC	);
				scenario->set_parameter<float>(document, "BArEnt_CIVIC", _BArEnt_CIVIC	);
				scenario->set_parameter<float>(document, "BArIns_CIVIC", _BArIns_CIVIC	);
				scenario->set_parameter<float>(document, "BArOff_CIVIC", _BArOff_CIVIC	);
				scenario->set_parameter<float>(document, "BArMix_CIVIC", _BArMix_CIVIC	);
				scenario->set_parameter<float>(document, "BArSch_CIVIC", _BArSch_CIVIC	);
				scenario->set_parameter<float>(document, "BEmOth_CIVIC", _BEmOth_CIVIC	);
				scenario->set_parameter<float>(document, "BEmInd_CIVIC", _BEmInd_CIVIC	);
				scenario->set_parameter<float>(document, "BEmGov_CIVIC", _BEmGov_CIVIC	);
				scenario->set_parameter<float>(document, "BEmSer_CIVIC", _BEmSer_CIVIC	);
				scenario->set_parameter<float>(document, "BEmMan_CIVIC", _BEmMan_CIVIC	);
				scenario->set_parameter<float>(document, "BEmRet_CIVIC", _BEmRet_CIVIC	);
				scenario->set_parameter<float>(document, "THETAG_CIVIC", _THETAG_CIVIC	);
				scenario->set_parameter<float>(document, "THETAM_CIVIC", _THETAM_CIVIC	);
				scenario->set_parameter<float>(document, "THETAR_CIVIC", _THETAR_CIVIC	);
				scenario->set_parameter<float>(document, "THETAS_CIVIC", _THETAS_CIVIC	);
				scenario->set_parameter<float>(document, "THETAI_CIVIC", _THETAI_CIVIC	);
				scenario->set_parameter<float>(document, "THETAO_CIVIC", _THETAO_CIVIC	);
				scenario->set_parameter<float>(document, "GAMMA_CIVIC" , _GAMMA_CIVIC 	);

				scenario->set_parameter<float>(document, "BTT_LEISURE"   , _BTT_LEISURE		);
				scenario->set_parameter<float>(document, "BINCD_LEISURE" , _BINCD_LEISURE 	);
				scenario->set_parameter<float>(document, "BRACED_LEISURE", _BRACED_LEISURE	);
				scenario->set_parameter<float>(document, "BArRes_LEISURE", _BArRes_LEISURE	);
				scenario->set_parameter<float>(document, "BArRec_LEISURE", _BArRec_LEISURE	);
				scenario->set_parameter<float>(document, "BArRet_LEISURE", _BArRet_LEISURE	);
				scenario->set_parameter<float>(document, "BArEnt_LEISURE", _BArEnt_LEISURE	);
				scenario->set_parameter<float>(document, "BArIns_LEISURE", _BArIns_LEISURE	);
				scenario->set_parameter<float>(document, "BArOff_LEISURE", _BArOff_LEISURE	);
				scenario->set_parameter<float>(document, "BArMix_LEISURE", _BArMix_LEISURE	);
				scenario->set_parameter<float>(document, "BArSch_LEISURE", _BArSch_LEISURE	);
				scenario->set_parameter<float>(document, "BEmOth_LEISURE", _BEmOth_LEISURE	);
				scenario->set_parameter<float>(document, "BEmInd_LEISURE", _BEmInd_LEISURE	);
				scenario->set_parameter<float>(document, "BEmGov_LEISURE", _BEmGov_LEISURE	);
				scenario->set_parameter<float>(document, "BEmSer_LEISURE", _BEmSer_LEISURE	);
				scenario->set_parameter<float>(document, "BEmMan_LEISURE", _BEmMan_LEISURE	);
				scenario->set_parameter<float>(document, "BEmRet_LEISURE", _BEmRet_LEISURE	);
				scenario->set_parameter<float>(document, "THETAG_LEISURE", _THETAG_LEISURE	);
				scenario->set_parameter<float>(document, "THETAM_LEISURE", _THETAM_LEISURE	);
				scenario->set_parameter<float>(document, "THETAR_LEISURE", _THETAR_LEISURE	);
				scenario->set_parameter<float>(document, "THETAS_LEISURE", _THETAS_LEISURE	);
				scenario->set_parameter<float>(document, "THETAI_LEISURE", _THETAI_LEISURE	);
				scenario->set_parameter<float>(document, "THETAO_LEISURE", _THETAO_LEISURE	);
				scenario->set_parameter<float>(document, "GAMMA_LEISURE" , _GAMMA_LEISURE 	);

				scenario->set_parameter<float>(document, "BTT_SOCIAL"   , _BTT_SOCIAL   	);
				scenario->set_parameter<float>(document, "BINCD_SOCIAL" , _BINCD_SOCIAL 	);
				scenario->set_parameter<float>(document, "BRACED_SOCIAL", _BRACED_SOCIAL	);
				scenario->set_parameter<float>(document, "BArRes_SOCIAL", _BArRes_SOCIAL	);
				scenario->set_parameter<float>(document, "BArRec_SOCIAL", _BArRec_SOCIAL	);
				scenario->set_parameter<float>(document, "BArRet_SOCIAL", _BArRet_SOCIAL	);
				scenario->set_parameter<float>(document, "BArEnt_SOCIAL", _BArEnt_SOCIAL	);
				scenario->set_parameter<float>(document, "BArIns_SOCIAL", _BArIns_SOCIAL	);
				scenario->set_parameter<float>(document, "BArOff_SOCIAL", _BArOff_SOCIAL	);
				scenario->set_parameter<float>(document, "BArMix_SOCIAL", _BArMix_SOCIAL	);
				scenario->set_parameter<float>(document, "BArSch_SOCIAL", _BArSch_SOCIAL	);
				scenario->set_parameter<float>(document, "BEmOth_SOCIAL", _BEmOth_SOCIAL	);
				scenario->set_parameter<float>(document, "BEmInd_SOCIAL", _BEmInd_SOCIAL	);
				scenario->set_parameter<float>(document, "BEmGov_SOCIAL", _BEmGov_SOCIAL	);
				scenario->set_parameter<float>(document, "BEmSer_SOCIAL", _BEmSer_SOCIAL	);
				scenario->set_parameter<float>(document, "BEmMan_SOCIAL", _BEmMan_SOCIAL	);
				scenario->set_parameter<float>(document, "BEmRet_SOCIAL", _BEmRet_SOCIAL	);
				scenario->set_parameter<float>(document, "THETAG_SOCIAL", _THETAG_SOCIAL	);
				scenario->set_parameter<float>(document, "THETAM_SOCIAL", _THETAM_SOCIAL	);
				scenario->set_parameter<float>(document, "THETAR_SOCIAL", _THETAR_SOCIAL	);
				scenario->set_parameter<float>(document, "THETAS_SOCIAL", _THETAS_SOCIAL	);
				scenario->set_parameter<float>(document, "THETAI_SOCIAL", _THETAI_SOCIAL	);
				scenario->set_parameter<float>(document, "THETAO_SOCIAL", _THETAO_SOCIAL	);
				scenario->set_parameter<float>(document, "GAMMA_SOCIAL" , _GAMMA_SOCIAL 	);

				scenario->set_parameter<float>(document, "BTT_OTHER"  , _BTT_OTHER		);
				scenario->set_parameter<float>(document, "BINCD_OTHER", _BINCD_OTHER 		);
				scenario->set_parameter<float>(document, "BRACED_OTHER", _BRACED_OTHER	);
				scenario->set_parameter<float>(document, "BArRes_OTHER", _BArRes_OTHER	);
				scenario->set_parameter<float>(document, "BArRec_OTHER", _BArRec_OTHER	);
				scenario->set_parameter<float>(document, "BArRet_OTHER", _BArRet_OTHER	);
				scenario->set_parameter<float>(document, "BArEnt_OTHER", _BArEnt_OTHER	);
				scenario->set_parameter<float>(document, "BArIns_OTHER", _BArIns_OTHER	);
				scenario->set_parameter<float>(document, "BArOff_OTHER", _BArOff_OTHER	);
				scenario->set_parameter<float>(document, "BArMix_OTHER", _BArMix_OTHER	);
				scenario->set_parameter<float>(document, "BArSch_OTHER", _BArSch_OTHER	);
				scenario->set_parameter<float>(document, "BEmOth_OTHER", _BEmOth_OTHER	);
				scenario->set_parameter<float>(document, "BEmInd_OTHER", _BEmInd_OTHER	);
				scenario->set_parameter<float>(document, "BEmGov_OTHER", _BEmGov_OTHER	);
				scenario->set_parameter<float>(document, "BEmSer_OTHER", _BEmSer_OTHER	);
				scenario->set_parameter<float>(document, "BEmMan_OTHER", _BEmMan_OTHER	);
				scenario->set_parameter<float>(document, "BEmRet_OTHER", _BEmRet_OTHER	);
				scenario->set_parameter<float>(document, "THETAG_OTHER", _THETAG_OTHER	);
				scenario->set_parameter<float>(document, "THETAM_OTHER", _THETAM_OTHER	);
				scenario->set_parameter<float>(document, "THETAR_OTHER", _THETAR_OTHER	);
				scenario->set_parameter<float>(document, "THETAS_OTHER", _THETAS_OTHER	);
				scenario->set_parameter<float>(document, "THETAI_OTHER", _THETAI_OTHER	);
				scenario->set_parameter<float>(document, "THETAO_OTHER", _THETAO_OTHER	);
				scenario->set_parameter<float>(document, "GAMMA_OTHER" , _GAMMA_OTHER 	);

				scenario->set_parameter<float>(document, "BTTAUTO_WORK", _BTTAUTO_WORK	);
				scenario->set_parameter<float>(document, "BINCD_WORK"	 , _BINCD_WORK		);
				scenario->set_parameter<float>(document, "BRACED_WORK" , _BRACED_WORK 	);
				scenario->set_parameter<float>(document, "BArRes_WORK" , _BArRes_WORK 	);
				scenario->set_parameter<float>(document, "BArRec_WORK" , _BArRec_WORK 	);
				scenario->set_parameter<float>(document, "BArRet_WORK" , _BArRet_WORK 	);
				scenario->set_parameter<float>(document, "BArEnt_WORK" , _BArEnt_WORK 	);
				scenario->set_parameter<float>(document, "BArIns_WORK" , _BArIns_WORK 	);
				scenario->set_parameter<float>(document, "BArOff_WORK" , _BArOff_WORK 	);
				scenario->set_parameter<float>(document, "BArMix_WORK" , _BArMix_WORK 	);
				scenario->set_parameter<float>(document, "BArSch_WORK" , _BArSch_WORK 	);
				scenario->set_parameter<float>(document, "BEmOth_WORK" , _BEmOth_WORK 	);
				scenario->set_parameter<float>(document, "BEmInd_WORK" , _BEmInd_WORK 	);
				scenario->set_parameter<float>(document, "BEmGov_WORK" , _BEmGov_WORK 	);
				scenario->set_parameter<float>(document, "BEmSer_WORK" , _BEmSer_WORK 	);
				scenario->set_parameter<float>(document, "BEmMan_WORK" , _BEmMan_WORK 	);
				scenario->set_parameter<float>(document, "BEmRet_WORK" , _BEmRet_WORK 	);
				scenario->set_parameter<float>(document, "THETAG_WORK" , _THETAG_WORK 	);
				scenario->set_parameter<float>(document, "THETAM_WORK" , _THETAM_WORK 	);
				scenario->set_parameter<float>(document, "THETAR_WORK" , _THETAR_WORK 	);
				scenario->set_parameter<float>(document, "THETAS_WORK" , _THETAS_WORK 	);
				scenario->set_parameter<float>(document, "THETAI_WORK" , _THETAI_WORK 	);
				scenario->set_parameter<float>(document, "THETAO_WORK" , _THETAO_WORK 	);

				scenario->set_parameter<float>(document, "BTTTRAN_WORK", _BTTTRAN_WORK     		);
				scenario->set_parameter<float>(document, "BTTOTHER_WORK", _BTTOTHER_WORK    		);
				scenario->set_parameter<float>(document, "BHOME_WORK", _BHOME_WORK       			);
				scenario->set_parameter<float>(document, "BEmUnrelated_WORK", _BEmUnrelated_WORK	);
				scenario->set_parameter<float>(document, "THETA_UR_WORK", _THETA_UR_WORK    		);

				return true;
			}

			static void print_parameters()
			{
				cout << "ADAPTS_Destination_Choice_Option parameters" << endl;
				cout << "\tBTT_PICK = " << BTT_PICK			<float>() << endl;
				cout << "\tBINCD_PICK = " << BINCD_PICK		<float>() << endl;
				cout << "\tBRACED_PICK = " << BRACED_PICK		<float>() << endl;
				cout << "\tBArRes_PICK = " << BArRes_PICK		<float>() << endl;
				cout << "\tBArRec_PICK = " << BArRec_PICK		<float>() << endl;
				cout << "\tBArRet_PICK = " << BArRet_PICK		<float>() << endl;
				cout << "\tBArEnt_PICK = " << BArEnt_PICK		<float>() << endl;
				cout << "\tBArIns_PICK = " << BArIns_PICK		<float>() << endl;
				cout << "\tBArOff_PICK = " << BArOff_PICK		<float>() << endl;
				cout << "\tBArMix_PICK = " << BArMix_PICK		<float>() << endl;
				cout << "\tBArSch_PICK = " << BArSch_PICK		<float>() << endl;
				cout << "\tBEmOth_PICK = " << BEmOth_PICK		<float>() << endl;
				cout << "\tBEmInd_PICK = " << BEmInd_PICK		<float>() << endl;
				cout << "\tBEmGov_PICK = " << BEmGov_PICK		<float>() << endl;
				cout << "\tBEmSer_PICK = " << BEmSer_PICK		<float>() << endl;
				cout << "\tBEmMan_PICK = " << BEmMan_PICK		<float>() << endl;
				cout << "\tBEmRet_PICK = " << BEmRet_PICK		<float>() << endl;
				cout << "\tTHETAG_PICK = " << THETAG_PICK		<float>() << endl;
				cout << "\tTHETAM_PICK = " << THETAM_PICK		<float>() << endl;
				cout << "\tTHETAR_PICK = " << THETAR_PICK		<float>() << endl;
				cout << "\tTHETAS_PICK = " << THETAS_PICK		<float>() << endl;
				cout << "\tTHETAI_PICK = " << THETAI_PICK		<float>() << endl;
				cout << "\tTHETAO_PICK = " << THETAO_PICK		<float>() << endl;
				cout << "\tGAMMA_PICK = " << GAMMA_PICK		<float>() << endl;

				cout << "\tBTT_OTHER_WORK = " << BTT_OTHER_WORK		<float>() << endl;
				cout << "\tBINCD_OTHER_WORK = " << BINCD_OTHER_WORK	<float>() << endl;
				cout << "\tBRACED_OTHER_WORK = " << BRACED_OTHER_WORK	<float>() << endl;
				cout << "\tBArRes_OTHER_WORK = " << BArRes_OTHER_WORK	<float>() << endl;
				cout << "\tBArRec_OTHER_WORK = " << BArRec_OTHER_WORK	<float>() << endl;
				cout << "\tBArRet_OTHER_WORK = " << BArRet_OTHER_WORK	<float>() << endl;
				cout << "\tBArEnt_OTHER_WORK = " << BArEnt_OTHER_WORK	<float>() << endl;
				cout << "\tBArIns_OTHER_WORK = " << BArIns_OTHER_WORK	<float>() << endl;
				cout << "\tBArOff_OTHER_WORK = " << BArOff_OTHER_WORK	<float>() << endl;
				cout << "\tBArMix_OTHER_WORK = " << BArMix_OTHER_WORK	<float>() << endl;
				cout << "\tBArSch_OTHER_WORK = " << BArSch_OTHER_WORK	<float>() << endl;
				cout << "\tBEmOth_OTHER_WORK = " << BEmOth_OTHER_WORK	<float>() << endl;
				cout << "\tBEmInd_OTHER_WORK = " << BEmInd_OTHER_WORK	<float>() << endl;
				cout << "\tBEmGov_OTHER_WORK = " << BEmGov_OTHER_WORK	<float>() << endl;
				cout << "\tBEmSer_OTHER_WORK = " << BEmSer_OTHER_WORK	<float>() << endl;
				cout << "\tBEmMan_OTHER_WORK = " << BEmMan_OTHER_WORK	<float>() << endl;
				cout << "\tBEmRet_OTHER_WORK = " << BEmRet_OTHER_WORK	<float>() << endl;
				cout << "\tTHETAG_OTHER_WORK = " << THETAG_OTHER_WORK	<float>() << endl;
				cout << "\tTHETAM_OTHER_WORK = " << THETAM_OTHER_WORK	<float>() << endl;
				cout << "\tTHETAR_OTHER_WORK = " << THETAR_OTHER_WORK	<float>() << endl;
				cout << "\tTHETAS_OTHER_WORK = " << THETAS_OTHER_WORK	<float>() << endl;
				cout << "\tTHETAI_OTHER_WORK = " << THETAI_OTHER_WORK	<float>() << endl;
				cout << "\tTHETAO_OTHER_WORK = " << THETAO_OTHER_WORK	<float>() << endl;
				cout << "\tGAMMA_OTHER_WORK = " << GAMMA_OTHER_WORK 	<float>() << endl;

				cout << "\tBTT_SERVICE = " << BTT_SERVICE	<float>() << endl;
				cout << "\tBINCD_SERVICE = " << BINCD_SERVICE	<float>() << endl;
				cout << "\tBRACED_SERVICE = " << BRACED_SERVICE	<float>() << endl;
				cout << "\tBArRes_SERVICE = " << BArRes_SERVICE	<float>() << endl;
				cout << "\tBArRec_SERVICE = " << BArRec_SERVICE	<float>() << endl;
				cout << "\tBArRet_SERVICE = " << BArRet_SERVICE	<float>() << endl;
				cout << "\tBArEnt_SERVICE = " << BArEnt_SERVICE	<float>() << endl;
				cout << "\tBArIns_SERVICE = " << BArIns_SERVICE	<float>() << endl;
				cout << "\tBArOff_SERVICE = " << BArOff_SERVICE	<float>() << endl;
				cout << "\tBArMix_SERVICE = " << BArMix_SERVICE	<float>() << endl;
				cout << "\tBArSch_SERVICE = " << BArSch_SERVICE	<float>() << endl;
				cout << "\tBEmOth_SERVICE = " << BEmOth_SERVICE	<float>() << endl;
				cout << "\tBEmInd_SERVICE = " << BEmInd_SERVICE	<float>() << endl;
				cout << "\tBEmGov_SERVICE = " << BEmGov_SERVICE	<float>() << endl;
				cout << "\tBEmSer_SERVICE = " << BEmSer_SERVICE	<float>() << endl;
				cout << "\tBEmMan_SERVICE = " << BEmMan_SERVICE	<float>() << endl;
				cout << "\tBEmRet_SERVICE = " << BEmRet_SERVICE	<float>() << endl;
				cout << "\tTHETAG_SERVICE = " << THETAG_SERVICE	<float>() << endl;
				cout << "\tTHETAM_SERVICE = " << THETAM_SERVICE	<float>() << endl;
				cout << "\tTHETAR_SERVICE = " << THETAR_SERVICE	<float>() << endl;
				cout << "\tTHETAS_SERVICE = " << THETAS_SERVICE	<float>() << endl;
				cout << "\tTHETAI_SERVICE = " << THETAI_SERVICE	<float>() << endl;
				cout << "\tTHETAO_SERVICE = " << THETAO_SERVICE	<float>() << endl;
				cout << "\tGAMMA_SERVICE = " << GAMMA_SERVICE  <float>() << endl;

				cout << "\tBTT_MIN_SHOP = " << BTT_MIN_SHOP		<float>() << endl;
				cout << "\tBINCD_MIN_SHOP = " << BINCD_MIN_SHOP	<float>() << endl;
				cout << "\tBRACED_MIN_SHOP = " << BRACED_MIN_SHOP	<float>() << endl;
				cout << "\tBArRes_MIN_SHOP = " << BArRes_MIN_SHOP	<float>() << endl;
				cout << "\tBArRec_MIN_SHOP = " << BArRec_MIN_SHOP	<float>() << endl;
				cout << "\tBArRet_MIN_SHOP = " << BArRet_MIN_SHOP	<float>() << endl;
				cout << "\tBArEnt_MIN_SHOP = " << BArEnt_MIN_SHOP	<float>() << endl;
				cout << "\tBArIns_MIN_SHOP = " << BArIns_MIN_SHOP	<float>() << endl;
				cout << "\tBArOff_MIN_SHOP = " << BArOff_MIN_SHOP	<float>() << endl;
				cout << "\tBArMix_MIN_SHOP = " << BArMix_MIN_SHOP	<float>() << endl;
				cout << "\tBArSch_MIN_SHOP = " << BArSch_MIN_SHOP	<float>() << endl;
				cout << "\tBEmOth_MIN_SHOP = " << BEmOth_MIN_SHOP	<float>() << endl;
				cout << "\tBEmInd_MIN_SHOP = " << BEmInd_MIN_SHOP	<float>() << endl;
				cout << "\tBEmGov_MIN_SHOP = " << BEmGov_MIN_SHOP	<float>() << endl;
				cout << "\tBEmSer_MIN_SHOP = " << BEmSer_MIN_SHOP	<float>() << endl;
				cout << "\tBEmMan_MIN_SHOP = " << BEmMan_MIN_SHOP	<float>() << endl;
				cout << "\tBEmRet_MIN_SHOP = " << BEmRet_MIN_SHOP	<float>() << endl;
				cout << "\tTHETAG_MIN_SHOP = " << THETAG_MIN_SHOP	<float>() << endl;
				cout << "\tTHETAM_MIN_SHOP = " << THETAM_MIN_SHOP	<float>() << endl;
				cout << "\tTHETAR_MIN_SHOP = " << THETAR_MIN_SHOP	<float>() << endl;
				cout << "\tTHETAS_MIN_SHOP = " << THETAS_MIN_SHOP	<float>() << endl;
				cout << "\tTHETAI_MIN_SHOP = " << THETAI_MIN_SHOP	<float>() << endl;
				cout << "\tTHETAO_MIN_SHOP = " << THETAO_MIN_SHOP	<float>() << endl;
				cout << "\tGAMMA_MIN_SHOP = " << GAMMA_MIN_SHOP 	<float>() << endl;

				cout << "\tBTT_MAJ_SHOP = " << BTT_MAJ_SHOP   	<float>() << endl;
				cout << "\tBINCD_MAJ_SHOP = " << BINCD_MAJ_SHOP 	<float>() << endl;
				cout << "\tBRACED_MAJ_SHOP = " << BRACED_MAJ_SHOP	<float>() << endl;
				cout << "\tBArRes_MAJ_SHOP = " << BArRes_MAJ_SHOP	<float>() << endl;
				cout << "\tBArRec_MAJ_SHOP = " << BArRec_MAJ_SHOP	<float>() << endl;
				cout << "\tBArRet_MAJ_SHOP = " << BArRet_MAJ_SHOP	<float>() << endl;
				cout << "\tBArEnt_MAJ_SHOP = " << BArEnt_MAJ_SHOP	<float>() << endl;
				cout << "\tBArIns_MAJ_SHOP = " << BArIns_MAJ_SHOP	<float>() << endl;
				cout << "\tBArOff_MAJ_SHOP = " << BArOff_MAJ_SHOP	<float>() << endl;
				cout << "\tBArMix_MAJ_SHOP = " << BArMix_MAJ_SHOP	<float>() << endl;
				cout << "\tBArSch_MAJ_SHOP = " << BArSch_MAJ_SHOP	<float>() << endl;
				cout << "\tBEmOth_MAJ_SHOP = " << BEmOth_MAJ_SHOP	<float>() << endl;
				cout << "\tBEmInd_MAJ_SHOP = " << BEmInd_MAJ_SHOP	<float>() << endl;
				cout << "\tBEmGov_MAJ_SHOP = " << BEmGov_MAJ_SHOP	<float>() << endl;
				cout << "\tBEmSer_MAJ_SHOP = " << BEmSer_MAJ_SHOP	<float>() << endl;
				cout << "\tBEmMan_MAJ_SHOP = " << BEmMan_MAJ_SHOP	<float>() << endl;
				cout << "\tBEmRet_MAJ_SHOP = " << BEmRet_MAJ_SHOP	<float>() << endl;
				cout << "\tTHETAG_MAJ_SHOP = " << THETAG_MAJ_SHOP	<float>() << endl;
				cout << "\tTHETAM_MAJ_SHOP = " << THETAM_MAJ_SHOP	<float>() << endl;
				cout << "\tTHETAR_MAJ_SHOP = " << THETAR_MAJ_SHOP	<float>() << endl;
				cout << "\tTHETAS_MAJ_SHOP = " << THETAS_MAJ_SHOP	<float>() << endl;
				cout << "\tTHETAI_MAJ_SHOP = " << THETAI_MAJ_SHOP	<float>() << endl;
				cout << "\tTHETAO_MAJ_SHOP = " << THETAO_MAJ_SHOP	<float>() << endl;
				cout << "\tGAMMA_MAJ_SHOP = " << GAMMA_MAJ_SHOP 	<float>() << endl;

				cout << "\tBTT_EAT_OUT = " << BTT_EAT_OUT	<float>() << endl;
				cout << "\tBINCD_EAT_OUT = " << BINCD_EAT_OUT	<float>() << endl;
				cout << "\tBRACED_EAT_OUT = " << BRACED_EAT_OUT	<float>() << endl;
				cout << "\tBArRes_EAT_OUT = " << BArRes_EAT_OUT	<float>() << endl;
				cout << "\tBArRec_EAT_OUT = " << BArRec_EAT_OUT	<float>() << endl;
				cout << "\tBArRet_EAT_OUT = " << BArRet_EAT_OUT	<float>() << endl;
				cout << "\tBArEnt_EAT_OUT = " << BArEnt_EAT_OUT	<float>() << endl;
				cout << "\tBArIns_EAT_OUT = " << BArIns_EAT_OUT	<float>() << endl;
				cout << "\tBArOff_EAT_OUT = " << BArOff_EAT_OUT	<float>() << endl;
				cout << "\tBArMix_EAT_OUT = " << BArMix_EAT_OUT	<float>() << endl;
				cout << "\tBArSch_EAT_OUT = " << BArSch_EAT_OUT	<float>() << endl;
				cout << "\tBEmOth_EAT_OUT = " << BEmOth_EAT_OUT	<float>() << endl;
				cout << "\tBEmInd_EAT_OUT = " << BEmInd_EAT_OUT	<float>() << endl;
				cout << "\tBEmGov_EAT_OUT = " << BEmGov_EAT_OUT	<float>() << endl;
				cout << "\tBEmSer_EAT_OUT = " << BEmSer_EAT_OUT	<float>() << endl;
				cout << "\tBEmMan_EAT_OUT = " << BEmMan_EAT_OUT	<float>() << endl;
				cout << "\tBEmRet_EAT_OUT = " << BEmRet_EAT_OUT	<float>() << endl;
				cout << "\tTHETAG_EAT_OUT = " << THETAG_EAT_OUT	<float>() << endl;
				cout << "\tTHETAM_EAT_OUT = " << THETAM_EAT_OUT	<float>() << endl;
				cout << "\tTHETAR_EAT_OUT = " << THETAR_EAT_OUT	<float>() << endl;
				cout << "\tTHETAS_EAT_OUT = " << THETAS_EAT_OUT	<float>() << endl;
				cout << "\tTHETAI_EAT_OUT = " << THETAI_EAT_OUT	<float>() << endl;
				cout << "\tTHETAO_EAT_OUT = " << THETAO_EAT_OUT	<float>() << endl;
				cout << "\tGAMMA_EAT_OUT = " << GAMMA_EAT_OUT 	<float>() << endl;

				cout << "\tBTT_CIVIC = " << BTT_CIVIC   	<float>() << endl;
				cout << "\tBINCD_CIVIC = " << BINCD_CIVIC 	<float>() << endl;
				cout << "\tBRACED_CIVIC = " << BRACED_CIVIC	<float>() << endl;
				cout << "\tBArRes_CIVIC = " << BArRes_CIVIC	<float>() << endl;
				cout << "\tBArRec_CIVIC = " << BArRec_CIVIC	<float>() << endl;
				cout << "\tBArRet_CIVIC = " << BArRet_CIVIC	<float>() << endl;
				cout << "\tBArEnt_CIVIC = " << BArEnt_CIVIC	<float>() << endl;
				cout << "\tBArIns_CIVIC = " << BArIns_CIVIC	<float>() << endl;
				cout << "\tBArOff_CIVIC = " << BArOff_CIVIC	<float>() << endl;
				cout << "\tBArMix_CIVIC = " << BArMix_CIVIC	<float>() << endl;
				cout << "\tBArSch_CIVIC = " << BArSch_CIVIC	<float>() << endl;
				cout << "\tBEmOth_CIVIC = " << BEmOth_CIVIC	<float>() << endl;
				cout << "\tBEmInd_CIVIC = " << BEmInd_CIVIC	<float>() << endl;
				cout << "\tBEmGov_CIVIC = " << BEmGov_CIVIC	<float>() << endl;
				cout << "\tBEmSer_CIVIC = " << BEmSer_CIVIC	<float>() << endl;
				cout << "\tBEmMan_CIVIC = " << BEmMan_CIVIC	<float>() << endl;
				cout << "\tBEmRet_CIVIC = " << BEmRet_CIVIC	<float>() << endl;
				cout << "\tTHETAG_CIVIC = " << THETAG_CIVIC	<float>() << endl;
				cout << "\tTHETAM_CIVIC = " << THETAM_CIVIC	<float>() << endl;
				cout << "\tTHETAR_CIVIC = " << THETAR_CIVIC	<float>() << endl;
				cout << "\tTHETAS_CIVIC = " << THETAS_CIVIC	<float>() << endl;
				cout << "\tTHETAI_CIVIC = " << THETAI_CIVIC	<float>() << endl;
				cout << "\tTHETAO_CIVIC = " << THETAO_CIVIC	<float>() << endl;
				cout << "\tGAMMA_CIVIC = " << GAMMA_CIVIC 	<float>() << endl;

				cout << "\tBTT_LEISURE = " << BTT_LEISURE  	<float>() << endl;
				cout << "\tBINCD_LEISURE = " << BINCD_LEISURE 	<float>() << endl;
				cout << "\tBRACED_LEISURE = " << BRACED_LEISURE	<float>() << endl;
				cout << "\tBArRes_LEISURE = " << BArRes_LEISURE	<float>() << endl;
				cout << "\tBArRec_LEISURE = " << BArRec_LEISURE	<float>() << endl;
				cout << "\tBArRet_LEISURE = " << BArRet_LEISURE	<float>() << endl;
				cout << "\tBArEnt_LEISURE = " << BArEnt_LEISURE	<float>() << endl;
				cout << "\tBArIns_LEISURE = " << BArIns_LEISURE	<float>() << endl;
				cout << "\tBArOff_LEISURE = " << BArOff_LEISURE	<float>() << endl;
				cout << "\tBArMix_LEISURE = " << BArMix_LEISURE	<float>() << endl;
				cout << "\tBArSch_LEISURE = " << BArSch_LEISURE	<float>() << endl;
				cout << "\tBEmOth_LEISURE = " << BEmOth_LEISURE	<float>() << endl;
				cout << "\tBEmInd_LEISURE = " << BEmInd_LEISURE	<float>() << endl;
				cout << "\tBEmGov_LEISURE = " << BEmGov_LEISURE	<float>() << endl;
				cout << "\tBEmSer_LEISURE = " << BEmSer_LEISURE	<float>() << endl;
				cout << "\tBEmMan_LEISURE = " << BEmMan_LEISURE	<float>() << endl;
				cout << "\tBEmRet_LEISURE = " << BEmRet_LEISURE	<float>() << endl;
				cout << "\tTHETAG_LEISURE = " << THETAG_LEISURE	<float>() << endl;
				cout << "\tTHETAM_LEISURE = " << THETAM_LEISURE	<float>() << endl;
				cout << "\tTHETAR_LEISURE = " << THETAR_LEISURE	<float>() << endl;
				cout << "\tTHETAS_LEISURE = " << THETAS_LEISURE	<float>() << endl;
				cout << "\tTHETAI_LEISURE = " << THETAI_LEISURE	<float>() << endl;
				cout << "\tTHETAO_LEISURE = " << THETAO_LEISURE	<float>() << endl;
				cout << "\tGAMMA_LEISURE = " << GAMMA_LEISURE 	<float>() << endl;

				cout << "\tBTT_SOCIAL = " << BTT_SOCIAL   	<float>() << endl;
				cout << "\tBINCD_SOCIAL = " << BINCD_SOCIAL 	<float>() << endl;
				cout << "\tBRACED_SOCIAL = " << BRACED_SOCIAL	<float>() << endl;
				cout << "\tBArRes_SOCIAL = " << BArRes_SOCIAL	<float>() << endl;
				cout << "\tBArRec_SOCIAL = " << BArRec_SOCIAL	<float>() << endl;
				cout << "\tBArRet_SOCIAL = " << BArRet_SOCIAL	<float>() << endl;
				cout << "\tBArEnt_SOCIAL = " << BArEnt_SOCIAL	<float>() << endl;
				cout << "\tBArIns_SOCIAL = " << BArIns_SOCIAL	<float>() << endl;
				cout << "\tBArOff_SOCIAL = " << BArOff_SOCIAL	<float>() << endl;
				cout << "\tBArMix_SOCIAL = " << BArMix_SOCIAL	<float>() << endl;
				cout << "\tBArSch_SOCIAL = " << BArSch_SOCIAL	<float>() << endl;
				cout << "\tBEmOth_SOCIAL = " << BEmOth_SOCIAL	<float>() << endl;
				cout << "\tBEmInd_SOCIAL = " << BEmInd_SOCIAL	<float>() << endl;
				cout << "\tBEmGov_SOCIAL = " << BEmGov_SOCIAL	<float>() << endl;
				cout << "\tBEmSer_SOCIAL = " << BEmSer_SOCIAL	<float>() << endl;
				cout << "\tBEmMan_SOCIAL = " << BEmMan_SOCIAL	<float>() << endl;
				cout << "\tBEmRet_SOCIAL = " << BEmRet_SOCIAL	<float>() << endl;
				cout << "\tTHETAG_SOCIAL = " << THETAG_SOCIAL	<float>() << endl;
				cout << "\tTHETAM_SOCIAL = " << THETAM_SOCIAL	<float>() << endl;
				cout << "\tTHETAR_SOCIAL = " << THETAR_SOCIAL	<float>() << endl;
				cout << "\tTHETAS_SOCIAL = " << THETAS_SOCIAL	<float>() << endl;
				cout << "\tTHETAI_SOCIAL = " << THETAI_SOCIAL	<float>() << endl;
				cout << "\tTHETAO_SOCIAL = " << THETAO_SOCIAL	<float>() << endl;
				cout << "\tGAMMA_SOCIAL = " << GAMMA_SOCIAL 	<float>() << endl;

				cout << "\tBTT_OTHER = " << BTT_OTHER   	<float>() << endl;
				cout << "\tBINCD_OTHER = " << BINCD_OTHER 	<float>() << endl;
				cout << "\tBRACED_OTHER = " << BRACED_OTHER	<float>() << endl;
				cout << "\tBArRes_OTHER = " << BArRes_OTHER	<float>() << endl;
				cout << "\tBArRec_OTHER = " << BArRec_OTHER	<float>() << endl;
				cout << "\tBArRet_OTHER = " << BArRet_OTHER	<float>() << endl;
				cout << "\tBArEnt_OTHER = " << BArEnt_OTHER	<float>() << endl;
				cout << "\tBArIns_OTHER = " << BArIns_OTHER	<float>() << endl;
				cout << "\tBArOff_OTHER = " << BArOff_OTHER	<float>() << endl;
				cout << "\tBArMix_OTHER = " << BArMix_OTHER	<float>() << endl;
				cout << "\tBArSch_OTHER = " << BArSch_OTHER	<float>() << endl;
				cout << "\tBEmOth_OTHER = " << BEmOth_OTHER	<float>() << endl;
				cout << "\tBEmInd_OTHER = " << BEmInd_OTHER	<float>() << endl;
				cout << "\tBEmGov_OTHER = " << BEmGov_OTHER	<float>() << endl;
				cout << "\tBEmSer_OTHER = " << BEmSer_OTHER	<float>() << endl;
				cout << "\tBEmMan_OTHER = " << BEmMan_OTHER	<float>() << endl;
				cout << "\tBEmRet_OTHER = " << BEmRet_OTHER	<float>() << endl;
				cout << "\tTHETAG_OTHER = " << THETAG_OTHER	<float>() << endl;
				cout << "\tTHETAM_OTHER = " << THETAM_OTHER	<float>() << endl;
				cout << "\tTHETAR_OTHER = " << THETAR_OTHER	<float>() << endl;
				cout << "\tTHETAS_OTHER = " << THETAS_OTHER	<float>() << endl;
				cout << "\tTHETAI_OTHER = " << THETAI_OTHER	<float>() << endl;
				cout << "\tTHETAO_OTHER = " << THETAO_OTHER	<float>() << endl;
				cout << "\tGAMMA_OTHER = " << GAMMA_OTHER 	<float>() << endl;

				cout << "\tBTTAUTO_WORK = " << BTTAUTO_WORK	<float>() << endl;
				cout << "\tBINCD_WORK = " << BINCD_WORK 	<float>() << endl;
				cout << "\tBRACED_WORK = " << BRACED_WORK 	<float>() << endl;
				cout << "\tBArRes_WORK = " << BArRes_WORK 	<float>() << endl;
				cout << "\tBArRec_WORK = " << BArRec_WORK 	<float>() << endl;
				cout << "\tBArRet_WORK = " << BArRet_WORK 	<float>() << endl;
				cout << "\tBArEnt_WORK = " << BArEnt_WORK 	<float>() << endl;
				cout << "\tBArIns_WORK = " << BArIns_WORK 	<float>() << endl;
				cout << "\tBArOff_WORK = " << BArOff_WORK 	<float>() << endl;
				cout << "\tBArMix_WORK = " << BArMix_WORK 	<float>() << endl;
				cout << "\tBArSch_WORK = " << BArSch_WORK 	<float>() << endl;
				cout << "\tBEmOth_WORK = " << BEmOth_WORK 	<float>() << endl;
				cout << "\tBEmInd_WORK = " << BEmInd_WORK 	<float>() << endl;
				cout << "\tBEmGov_WORK = " << BEmGov_WORK 	<float>() << endl;
				cout << "\tBEmSer_WORK = " << BEmSer_WORK 	<float>() << endl;
				cout << "\tBEmMan_WORK = " << BEmMan_WORK 	<float>() << endl;
				cout << "\tBEmRet_WORK = " << BEmRet_WORK 	<float>() << endl;
				cout << "\tTHETAG_WORK = " << THETAG_WORK 	<float>() << endl;
				cout << "\tTHETAM_WORK = " << THETAM_WORK 	<float>() << endl;
				cout << "\tTHETAR_WORK = " << THETAR_WORK 	<float>() << endl;
				cout << "\tTHETAS_WORK = " << THETAS_WORK 	<float>() << endl;
				cout << "\tTHETAI_WORK = " << THETAI_WORK 	<float>() << endl;
				cout << "\tTHETAO_WORK = " << THETAO_WORK 	<float>() << endl;

				cout << "\tBTTTRAN_WORK = " << BTTTRAN_WORK     		<float>() << endl;
				cout << "\tBTTOTHER_WORK = " << BTTOTHER_WORK    		<float>() << endl;
				cout << "\tBHOME_WORK = " << BHOME_WORK       			<float>() << endl;
				cout << "\tBEmUnrelated_WORK = " << BEmUnrelated_WORK	<float>() << endl;
				cout << "\tTHETA_UR_WORK = " << THETA_UR_WORK    		<float>() << endl;
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

				// These need to be divided by 1000 to match employees above, as per the paper
				float thetag = zone->template accessibility_employment_government<double>() / 1000.0;// ;
				float thetam = zone->template accessibility_employment_manufacturing<double>() / 1000.0;
				float thetar = zone->template accessibility_employment_retail<double>() / 1000.0;
				float thetas = zone->template accessibility_employment_services<double>() / 1000.0;
				float thetai = zone->template accessibility_employment_industrial<double>() / 1000.0;
				float thetao = zone->template accessibility_employment_other<double>() / 1000.0;

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


			static void default_static_initializer()
			{
				_BTT_PICK = -0.1;
				_BINCD_PICK = 0.0;
				_BRACED_PICK = 0.0;
				_BArRes_PICK = 0.0;
				_BArRec_PICK = 0.0;
				_BArRet_PICK = 0.0;
				_BArEnt_PICK = 0.0;
				_BArIns_PICK = 0.0;
				_BArOff_PICK = 0.0;
				_BArMix_PICK = 0.0;
				_BArSch_PICK = 0.0;
				_BEmOth_PICK = 0.0;
				_BEmInd_PICK = 0.0;
				_BEmGov_PICK = 0.0;
				_BEmSer_PICK = 0.0;
				_BEmMan_PICK = 0.0;
				_BEmRet_PICK = 0.0;
				_THETAG_PICK = 0.0;
				_THETAM_PICK = 0.0;
				_THETAR_PICK = 0.0;
				_THETAS_PICK = 0.0;
				_THETAI_PICK = 0.0;
				_THETAO_PICK = 0.0;
				_GAMMA_PICK = 0.0;
				_BTT_OTHER_WORK = -0.1;
				_BINCD_OTHER_WORK = 0.0;
				_BRACED_OTHER_WORK = 0.0;
				_BArRes_OTHER_WORK = 0.0;
				_BArRec_OTHER_WORK = 0.0;
				_BArRet_OTHER_WORK = 0.0;
				_BArEnt_OTHER_WORK = 0.0;
				_BArIns_OTHER_WORK = 0.0;
				_BArOff_OTHER_WORK = 0.0;
				_BArMix_OTHER_WORK = 0.0;
				_BArSch_OTHER_WORK = 0.0;
				_BEmOth_OTHER_WORK = 0.0;
				_BEmInd_OTHER_WORK = 0.0;
				_BEmGov_OTHER_WORK = 0.0;
				_BEmSer_OTHER_WORK = 0.0;
				_BEmMan_OTHER_WORK = 0.0;
				_BEmRet_OTHER_WORK = 0.0;
				_THETAG_OTHER_WORK = 0.0;
				_THETAM_OTHER_WORK = 0.0;
				_THETAR_OTHER_WORK = 0.0;
				_THETAS_OTHER_WORK = 0.0;
				_THETAI_OTHER_WORK = 0.0;
				_THETAO_OTHER_WORK = 0.0;
				_GAMMA_OTHER_WORK = 0.0;
				_BTT_SERVICE = -0.1;
				_BINCD_SERVICE = 0.0;
				_BRACED_SERVICE = 0.0;
				_BArRes_SERVICE = 0.0;
				_BArRec_SERVICE = 0.0;
				_BArRet_SERVICE = 0.0;
				_BArEnt_SERVICE = 0.0;
				_BArIns_SERVICE = 0.0;
				_BArOff_SERVICE = 0.0;
				_BArMix_SERVICE = 0.0;
				_BArSch_SERVICE = 0.0;
				_BEmOth_SERVICE = 0.0;
				_BEmInd_SERVICE = 0.0;
				_BEmGov_SERVICE = 0.0;
				_BEmSer_SERVICE = 0.0;
				_BEmMan_SERVICE = 0.0;
				_BEmRet_SERVICE = 0.0;
				_THETAG_SERVICE = 0.0;
				_THETAM_SERVICE = 0.0;
				_THETAR_SERVICE = 0.0;
				_THETAS_SERVICE = 0.0;
				_THETAI_SERVICE = 0.0;
				_THETAO_SERVICE = 0.0;
				_GAMMA_SERVICE = 0.0;
				_BArRec_MIN_SHOP = 0.0;
				_BTT_MIN_SHOP = -0.1;
				_BINCD_MIN_SHOP = 0.0;
				_BRACED_MIN_SHOP = 0.0;
				_BArRes_MIN_SHOP = 0.0;
				_BArRec_MAJ_SHOP = 0.0;
				_BArRet_MIN_SHOP = 0.0;
				_BArEnt_MIN_SHOP = 0.0;
				_BArIns_MIN_SHOP = 0.0;
				_BArOff_MIN_SHOP = 0.0;
				_BArMix_MIN_SHOP = 0.0;
				_BArSch_MIN_SHOP = 0.0;
				_BEmOth_MIN_SHOP = 0.0;
				_BEmInd_MIN_SHOP = 0.0;
				_BEmGov_MIN_SHOP = 0.0;
				_BEmSer_MIN_SHOP = 0.0;
				_BEmMan_MIN_SHOP = 0.0;
				_BEmRet_MIN_SHOP = 0.0;
				_THETAG_MIN_SHOP = 0.0;
				_THETAM_MIN_SHOP = 0.0;
				_THETAR_MIN_SHOP = 0.0;
				_THETAS_MIN_SHOP = 0.0;
				_THETAI_MIN_SHOP = 0.0;
				_THETAO_MIN_SHOP = 0.0;
				_GAMMA_MIN_SHOP = 0.0;
				_BTT_MAJ_SHOP = -0.1;
				_BINCD_MAJ_SHOP = 0.0;
				_BRACED_MAJ_SHOP = 0.0;
				_BArRes_MAJ_SHOP = 0.0;
				_BArRec_EAT_OUT = 0.0;
				_BArRet_MAJ_SHOP = 0.0;
				_BArEnt_MAJ_SHOP = 0.0;
				_BArIns_MAJ_SHOP = 0.0;
				_BArOff_MAJ_SHOP = 0.0;
				_BArMix_MAJ_SHOP = 0.0;
				_BArSch_MAJ_SHOP = 0.0;
				_BEmOth_MAJ_SHOP = 0.0;
				_BEmInd_MAJ_SHOP = 0.0;
				_BEmGov_MAJ_SHOP = 0.0;
				_BEmSer_MAJ_SHOP = 0.0;
				_BEmMan_MAJ_SHOP = 0.0;
				_BEmRet_MAJ_SHOP = 0.0;
				_THETAG_MAJ_SHOP = 0.0;
				_THETAM_MAJ_SHOP = 0.0;
				_THETAR_MAJ_SHOP = 0.0;
				_THETAS_MAJ_SHOP = 0.0;
				_THETAI_MAJ_SHOP = 0.0;
				_THETAO_MAJ_SHOP = 0.0;
				_GAMMA_MAJ_SHOP = 0.0;
				_BTT_EAT_OUT = -0.1;
				_BINCD_EAT_OUT = 0.0;
				_BRACED_EAT_OUT = 0.0;
				_BArRes_EAT_OUT = 0.0;
				_BArRec_CIVIC = 0.0;
				_BArRet_EAT_OUT = 0.0;
				_BArEnt_EAT_OUT = 0.0;
				_BArIns_EAT_OUT = 0.0;
				_BArOff_EAT_OUT = 0.0;
				_BArMix_EAT_OUT = 0.0;
				_BArSch_EAT_OUT = 0.0;
				_BEmOth_EAT_OUT = 0.0;
				_BEmInd_EAT_OUT = 0.0;
				_BEmGov_EAT_OUT = 0.0;
				_BEmSer_EAT_OUT = 0.0;
				_BEmMan_EAT_OUT = 0.0;
				_BEmRet_EAT_OUT = 0.0;
				_THETAG_EAT_OUT = 0.0;
				_THETAM_EAT_OUT = 0.0;
				_THETAR_EAT_OUT = 0.0;
				_THETAS_EAT_OUT = 0.0;
				_THETAI_EAT_OUT = 0.0;
				_THETAO_EAT_OUT = 0.0;
				_GAMMA_EAT_OUT = 0.0;
				_BTT_CIVIC = -0.1;
				_BINCD_CIVIC = 0.0;
				_BRACED_CIVIC = 0.0;
				_BArRes_CIVIC = 0.0;
				_BArRec_LEISURE = 0.0;
				_BArRet_CIVIC = 0.0;
				_BArEnt_CIVIC = 0.0;
				_BArIns_CIVIC = 0.0;
				_BArOff_CIVIC = 0.0;
				_BArMix_CIVIC = 0.0;
				_BArSch_CIVIC = 0.0;
				_BEmOth_CIVIC = 0.0;
				_BEmInd_CIVIC = 0.0;
				_BEmGov_CIVIC = 0.0;
				_BEmSer_CIVIC = 0.0;
				_BEmMan_CIVIC = 0.0;
				_BEmRet_CIVIC = 0.0;
				_THETAG_CIVIC = 0.0;
				_THETAM_CIVIC = 0.0;
				_THETAR_CIVIC = 0.0;
				_THETAS_CIVIC = 0.0;
				_THETAI_CIVIC = 0.0;
				_THETAO_CIVIC = 0.0;
				_GAMMA_CIVIC = 0.0;
				_BTT_LEISURE = -0.1;
				_BINCD_LEISURE = 0.0;
				_BRACED_LEISURE = 0.0;
				_BArRes_LEISURE = 0.0;
				_BArRec_SOCIAL = 0.0;
				_BArRet_LEISURE = 0.0;
				_BArEnt_LEISURE = 0.0;
				_BArIns_LEISURE = 0.0;
				_BArOff_LEISURE = 0.0;
				_BArMix_LEISURE = 0.0;
				_BArSch_LEISURE = 0.0;
				_BEmOth_LEISURE = 0.0;
				_BEmInd_LEISURE = 0.0;
				_BEmGov_LEISURE = 0.0;
				_BEmSer_LEISURE = 0.0;
				_BEmMan_LEISURE = 0.0;
				_BEmRet_LEISURE = 0.0;
				_THETAG_LEISURE = 0.0;
				_THETAM_LEISURE = 0.0;
				_THETAR_LEISURE = 0.0;
				_THETAS_LEISURE = 0.0;
				_THETAI_LEISURE = 0.0;
				_THETAO_LEISURE = 0.0;
				_GAMMA_LEISURE = 0.0;
				_BTT_SOCIAL = -0.1;
				_BINCD_SOCIAL = 0.0;
				_BRACED_SOCIAL = 0.0;
				_BArRes_SOCIAL = 0.0;
				_BArRec_OTHER = 0.0;
				_BArRet_SOCIAL = 0.0;
				_BArEnt_SOCIAL = 0.0;
				_BArIns_SOCIAL = 0.0;
				_BArOff_SOCIAL = 0.0;
				_BArMix_SOCIAL = 0.0;
				_BArSch_SOCIAL = 0.0;
				_BEmOth_SOCIAL = 0.0;
				_BEmInd_SOCIAL = 0.0;
				_BEmGov_SOCIAL = 0.0;
				_BEmSer_SOCIAL = 0.0;
				_BEmMan_SOCIAL = 0.0;
				_BEmRet_SOCIAL = 0.0;
				_THETAG_SOCIAL = 0.0;
				_THETAM_SOCIAL = 0.0;
				_THETAR_SOCIAL = 0.0;
				_THETAS_SOCIAL = 0.0;
				_THETAI_SOCIAL = 0.0;
				_THETAO_SOCIAL = 0.0;
				_GAMMA_SOCIAL = 0.0;
				_BTT_OTHER = -0.1;
				_BINCD_OTHER = 0.0;
				_BRACED_OTHER = 0.0;
				_BArRes_OTHER = 0.0;
				_BArRet_OTHER = 0.0;
				_BArEnt_OTHER = 0.0;
				_BArIns_OTHER = 0.0;
				_BArOff_OTHER = 0.0;
				_BArMix_OTHER = 0.0;
				_BArSch_OTHER = 0.0;
				_BEmOth_OTHER = 0.0;
				_BEmInd_OTHER = 0.0;
				_BEmGov_OTHER = 0.0;
				_BEmSer_OTHER = 0.0;
				_BEmMan_OTHER = 0.0;
				_BEmRet_OTHER = 0.0;
				_THETAG_OTHER = 0.0;
				_THETAM_OTHER = 0.0;
				_THETAR_OTHER = 0.0;
				_THETAS_OTHER = 0.0;
				_THETAI_OTHER = 0.0;
				_THETAO_OTHER = 0.0;
				_GAMMA_OTHER = 0.0;
				_BTTAUTO_WORK = -0.1;
				_BTTTRAN_WORK = -0.1;
				_BTTOTHER_WORK = -0.1;
				_BHOME_WORK = 0.0;
				_BINCD_WORK = 0.0;
				_BRACED_WORK = 0.0;
				_BArRes_WORK = 0.0;
				_BArRec_WORK = 0.0;
				_BArRet_WORK = 0.0;
				_BArEnt_WORK = 0.0;
				_BArIns_WORK = 0.0;
				_BArOff_WORK = 0.0;
				_BArMix_WORK = 0.0;
				_BArSch_WORK = 0.0;
				_BEmOth_WORK = 0.0;
				_BEmInd_WORK = 0.0;
				_BEmGov_WORK = 0.0;
				_BEmSer_WORK = 0.0;
				_BEmMan_WORK = 0.0;
				_BEmRet_WORK = 0.0;
				_BEmUnrelated_WORK = 0.0;
				_THETAG_WORK = 0.0;
				_THETAM_WORK = 0.0;
				_THETAR_WORK = 0.0;
				_THETAS_WORK = 0.0;
				_THETAI_WORK = 0.0;
				_THETAO_WORK = 0.0;
				_THETA_UR_WORK = 0.0;
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
			
			m_static_data(int, choice_set_size, NONE, NONE);
			m_static_data(int, num_strata, NONE, NONE);

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

				//----------------------------------------------------------------------
				// Get the mode of the activity, if not yet planned, assume 9AM
				Vehicle_Components::Types::Vehicle_Type_Keys mode = Vehicle_Components::Types::Vehicle_Type_Keys::SOV;
				if (_Current_Activity->Mode_Is_Planned()) mode = _Current_Activity->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>();


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
						Time_Minutes min_avail_time = network->Get_TTime<_Activity_Location_Interface*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Minutes, Time_Minutes>(prev_loc, next_loc, mode, start_time) + 5.0;
						avail_time = max_end - min_start - _Current_Activity->template Duration<Time_Minutes>();
						// if there is no time available between previous and next activity, use the travel time from previous to next location as the available time (i.e. search for stops along the travel route, more or less....) - add 5 minutes slack time...
						if (avail_time < min_avail_time) avail_time = min_avail_time;

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

				//---------------------------------------------
				// If no zones are available add the current location as the only option
				if (available_zones[0].size() + available_zones[1].size() + available_zones[2].size() + available_zones[3].size() == 0)
				{
					Create_New_Choice_Option(choice_set, choice_model, prev_loc->zone<_Zone_Interface*>(), act_type, prev_loc, next_loc, 1.0/*,avail_time<1440*/);
					return;
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
							Create_New_Choice_Option(choice_set,choice_model, (_Zone_Interface*)(*z_itr), act_type,prev_loc,next_loc,1.0/*,avail_time<1440*/);
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
				Vehicle_Components::Types::Vehicle_Type_Keys mode = Vehicle_Components::Types::Vehicle_Type_Keys::SOV;
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


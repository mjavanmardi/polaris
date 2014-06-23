#include "Repository\Repository.h"


using namespace polaris;

prototype struct Mode_Chooser ADD_DEBUG_INFO
{
	tag_as_prototype;

	template<typename ReturnType> ReturnType Choose_Mode()
	{
		return this_component()->template Choose_Mode<ActivityItfType, ReturnType>(activity);
	}
};
prototype struct Mode_Choice_Option : public Choice_Model_Components::Prototypes::Choice_Option<ComponentType>
{
	tag_as_prototype;

	// accessor to parent class
	accessor(cost, NONE, NONE);
	accessor(travel_time, NONE, NONE);
};

implementation struct Mode_Chooser_Implementation : public Polaris_Component<MasterType,INHERIT(Mode_Chooser_Implementation),Data_Object>
{
	// Tag as Implementation
	typedef typename Polaris_Component<MasterType,INHERIT(Mode_Chooser_Implementation),Data_Object>::Component_Type ComponentType;

	// Pointer to the Parent class
	m_prototype(Choice_Model_Components::Prototypes::Choice_Model, typename MasterType::nested_logit_model_type, Choice_Model, NONE, NONE);
			
	static m_data(int, choice_set_size, NONE, NONE);

	// Interface definitions	
	typedef Choice_Model_Components::Prototypes::Choice_Model<typename MasterType::mnl_model_type > _Choice_Model_Interface;
	typedef Mode_Choice_Option<typename MasterType::mode_choice_option_type> _Mode_Choice_Option_Interface;
	typedef Choice_Model_Components::Prototypes::Choice_Option<typename MasterType::mode_choice_option_type> _Choice_Option_Interface;


	template<typename TargetType> void Initialize()
	{	
	}

	template<typename ActivityItfType, typename ReturnType> ReturnType Choose_Mode(ActivityItfType activity)
	{
		person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
		scheduler_itf* scheduler = _Parent_Person->Scheduling_Faculty<scheduler_itf*>();
		household_itf* _Parent_Household = _Parent_Person->Household<household_itf*>();
		household_static_properties_itf* household_properties = _Parent_Household->Static_Properties<household_static_properties_itf*>();
				
		// If no vehicles in the household, automatically assume transit
		bool auto_available = true;
		if (household_properties->Number_of_vehicles<int>() < 1) auto_available = false;

		// create local choice model
		_Choice_Model_Interface* choice_model = (_Choice_Model_Interface*)Allocate<typename MasterType::mnl_model_type>();
		boost::container::vector<_Mode_Choice_Option_Interface*> mode_options;

		// external knowledge references
		_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
		_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();

		Activity_Plan* cur_act = (Activity_Plan*)activity;
		Activity_Plan* prev_act, *next_act;
		_Activity_Location_Interface* prev_location, *next_location, *dest_location;

		// If the start time is known, set the previous activity/location and the next activity/location to do mode choice planning
		if (cur_act->Start_Is_Planned<bool>())
		{
			prev_act = _Parent_Person->previous_activity_plan<Time_Seconds,Activity_Plan*>(cur_act->Start_Time<Time_Seconds>());
			prev_location = scheduler->previous_location<Activity_Plan*,_Activity_Location_Interface*>(cur_act);
			if (prev_location == nullptr) prev_location = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
			next_act = _Parent_Person->next_activity_plan<Time_Seconds,Activity_Plan*>(cur_act->Start_Time<Time_Seconds>());
			next_location = scheduler->next_location<Activity_Plan*,_Activity_Location_Interface*>(cur_act);
			if (next_location == nullptr) next_location = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
		}
		// Otherwise, next activities not known, assume start and end tour location is home
		else
		{
			prev_act = nullptr;
			prev_location = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
			next_act = nullptr;
			next_location = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
		}
		if (cur_act->Location_Is_Planned<bool>())
		{
			dest_location = cur_act->Location<_Activity_Location_Interface*>();
		}
		else
		{
			dest_location = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
		}
				
		//============================================================================================
		// add the SOV choice option
		_Mode_Choice_Option_Interface* choice = (_Mode_Choice_Option_Interface*)Allocate<typename MasterType::mode_choice_option_type>();
		choice->Parent_Planner<Parent_Planner_type>(_Parent_Planner);
		choice->mode_type<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::SOV);
		choice->current_activity<ActivityItfType>(activity);
		choice_model->template Add_Choice_Option<_Choice_Option_Interface*>((_Choice_Option_Interface*)choice);
		mode_options.push_back(choice);

		// add the transit choice option
		choice = (_Mode_Choice_Option_Interface*)Allocate<typename MasterType::mode_choice_option_type>();
		choice->Parent_Planner<Parent_Planner_type>(_Parent_Planner);
		choice->mode_type<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::BUS);
		choice->current_activity<ActivityItfType>(activity);
		choice->destination<_Activity_Location_Interface*>(dest_location);
		choice->previous_activity<Activity_Plan*>(prev_act);
		choice->previous_location<_Activity_Location_Interface*>(prev_location);
		choice->next_activity<Activity_Plan*>(next_act);
		choice->next_location<_Activity_Location_Interface*>(next_location);
		choice->auto_available<bool>(auto_available);
		choice_model->template Add_Choice_Option<_Choice_Option_Interface*>((_Choice_Option_Interface*)choice);
		mode_options.push_back(choice);

		// Make choice
		int selected_index = 0;
		choice_model->template Evaluate_Choices<NT>();
		_Choice_Option_Interface* selected = choice_model->template Choose<_Choice_Option_Interface*>(selected_index);
		Vehicle_Components::Types::Vehicle_Type_Keys selected_mode = Vehicle_Components::Types::Vehicle_Type_Keys::SOV;

		if (selected == nullptr ) {THROW_WARNING("WARNING: selected is null - no mode choice made, defaulted to auto mode." << selected_index);}
		else selected_mode = ((_Mode_Choice_Option_Interface*)selected)->mode_type<ReturnType>();

		// Add the temporary HOV correction - this should eventually be replaced by including HOV as an option in the choice model
		if (Assign_To_HOV<ActivityItfType>(activity, selected_mode)) selected_mode = Vehicle_Components::Types::Vehicle_Type_Keys::HOV;

		//============================================================================================
		//Account for touring - if previous act is not at an anchor location and not using auto, then auto not available
		//-need to update to disable auto mode when not available at the anchor location as well
		if (prev_location != _Parent_Person->Home_Location<_Activity_Location_Interface*>() &&
			prev_location != _Parent_Person->Work_Location<_Activity_Location_Interface*>() &&
			prev_location != _Parent_Person->School_Location<_Activity_Location_Interface*>() &&
			prev_act != nullptr)
		{
			//if (prev_act->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == BUS || prev_act->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == HOV)
			//{
			//	if (selected_mode == Vehicle_Components::Types::Vehicle_Type_Keys::SOV) selected_mode = Vehicle_Components::Types::Vehicle_Type_Keys::HOV;
			//}
		}

		// free memory allocated locally
		for (int i = 0; i < mode_options.size(); i++) Free<typename _Choice_Option_Interface::Component_Type>((typename _Choice_Option_Interface::Component_Type*)mode_options[i]);
		Free<typename MasterType::mnl_model_type>((typename MasterType::mnl_model_type*)choice_model);

		// return the chosen mode
		return selected_mode;
	}

};
implementation struct Mode_Choice_Option_Impl : public Choice_Model_Components::Implementations::Nested_Choice_Option_Base<MasterType,INHERIT(Mode_Choice_Option_Impl)>
{
	// Tag as Implementation
	typedef typename Choice_Model_Components::Implementations::Nested_Choice_Option_Base<MasterType,INHERIT(Mode_Choice_Option_Impl)>::Component_Type ComponentType;


	// data members
	m_data(bool, cost, NONE, NONE);
	m_data(bool, travel_time, NONE, NONE);


	// PARAMETER DECLARATIONS
	m_data(float, INCLUSIVE_VALUE, NONE, NONE);
	m_data(float, B_COST, NONE, NONE);
	m_data(float, B_TTIME, NONE, NONE);

	//====================================================================================================================================
	// Interface definitions
	//------------------------------------------------------------------------------------------------------------------------------------

	// Feature called from prototype and by Choice_Model
	virtual double Calculate_Utility()
	{
		double utility;


		return utility;				
	}

	virtual void Print_Utility()
	{
		//return 0.0;
	}

	// Local features
	template<typename TargetType> TargetType Calculate_Utility_For_Known_Location()
	{
		person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
		scheduler_itf* scheduler = _Parent_Person->template Scheduling_Faculty<scheduler_itf*>();

		// external knowledge references
		_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
		_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();
		_Activity_Locations_Container_Interface* locations = network->template activity_locations_container<_Activity_Locations_Container_Interface*>();
		_Skim_Interface* skim = network->template skimming_faculty<_Skim_Interface*>();
		_Zone_Interface* dest_zone = _destination->zone<_Zone_Interface*>();

		// get the travel time to the destination
		los_itf* los;
		if (_current_activity->Start_Is_Planned<bool>()) los = skim->template Get_LOS<_Activity_Location_Interface*, Time_Seconds,los_itf*>(_previous_location,_destination, _current_activity->Start_Time<Time_Seconds>());
		else los = skim->template Get_LOS<_Activity_Location_Interface*, Time_Hours,los_itf*>(_previous_location,_destination, 12.0);
				
				
		// Get the differences in characteristics for transit compared to auto mode (CMAP model specified as difference)
		float ivtt_dif = los->auto_ttime<Time_Minutes>() - los->transit_ttime<Time_Minutes>();
		float wait_dif = -1.0 * los->transit_wait_time<Time_Minutes>();
		float transfer_dif = -1.0 * los->transit_walk_access_time<Time_Minutes>();
		if (los->transit_walk_access_time<Time_Minutes>() > 60.0) transfer_dif = 0;
		float walk_time_dif = -4.0; // * los->transit_walk_access_time<Time_Minutes>();
		float cost_dif = los->auto_distance<Miles>()*20.0 + dest_zone->Parking_Cost<Cents>() + los->auto_tolls<Cents>() - los->transit_fare<Cents>();

		// modify the values if no auto in the household (i.e. auto mode becomes like carpool with wait times, walk times, transfer time)
		if (!this->_auto_available)
		{
			wait_dif += 30.0;
			transfer_dif += 2.0;
			walk_time_dif += 2.0;
		}

		if (abs(ivtt_dif) > 1440) return -FLT_MAX;
		float u = Calculate_Utility_Value(ivtt_dif, wait_dif, transfer_dif, walk_time_dif, cost_dif);	
		if (u > 100.0) THROW_WARNING("WARNING: utility > 200.0 will cause numeric overflow, possible misspecification in utility function for mode choice (ivtt,wait,transfer,walk,cost): "<<ivtt_dif<<","<<wait_dif<<","<<transfer_dif<<","<<walk_time_dif<<","<<cost_dif);
				

		//cout << "O/D:"<<_previous_location->zone<_Zone_Interface*>()->uuid<int>() <<"/"<<_destination->zone<_Zone_Interface*>()->uuid<int>();
		//cout << ",Auto_TT:"<<los->auto_ttime<Time_Minutes>() << ",Transit_TT:"<<los->transit_ttime<Time_Minutes>();
		//cout <<",Transit_wait:" << -1.0* wait_dif << ",transfer_time:"<<-1.0 *transfer_dif;
		//cout <<",Auto_cost:" << los->auto_distance<Miles>()*15.0 + dest_zone->Parking_Cost<Cents>() + los->auto_tolls<Cents>();
		//cout <<",Transit_fare:" << los->transit_fare<Cents>() << ",utility:"<< u << endl;
				
		return (TargetType)u;				
	}
	template<typename TargetType> TargetType Calculate_Utility_For_Unknown_Location()
	{		
		_Zone_Interface* origin_zone = _previous_location->zone<_Zone_Interface*>();
		float ivtt_dif = origin_zone->avg_ttime_auto_to_transit_accessible_zones<Time_Minutes>() - origin_zone->avg_ttime_transit<Time_Minutes>();
		float wait_dif = -5.0; // assumed wait time of 5 minutes for transit trips
		float transfer_dif = 0;
		float walk_time_dif = -5.0; // assumed average walk time of 5 minutes for transit trips, given walk speed of 3 mph and max distance of 0.5 miles
		float cost_dif = 0;
		float utility = -1.0 * FLT_MAX;

		// modify the values if no auto in the household (i.e. auto mode becomes like carpool with wait times, walk times, transfer time)
		if (!this->_auto_available)
		{
			wait_dif += 30.0;
			transfer_dif += 2.0;
			walk_time_dif += 2.0;
		}

		// If the transit mode is  accessible from the current zone, calculate utility, otherwise utility is flt_max		
		if (origin_zone->avg_ttime_transit<Time_Days>() < 1.0) utility = Calculate_Utility_Value(ivtt_dif, wait_dif, transfer_dif, walk_time_dif, cost_dif);
		else return utility;

		if (utility > 100.0) THROW_WARNING("WARNING: utility > 200.0 will cause numeric overflow, possible misspecification in utility function for mode choice (ivtt,wait,transfer,walk,cost): "<<ivtt_dif<<","<<wait_dif<<","<<transfer_dif<<","<<walk_time_dif<<","<<cost_dif);

		// Otherwise return the transit utility based on Zonal average characteristics
		return utility;			
	}
	float Calculate_Utility_Value(float ivtt_dif, float wait_dif, float transfer_dif, float walk_time_dif, float cost_dif)
	{
		float utility;

		Activity_Components::Types::ACTIVITY_TYPES activity_type = _current_activity->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();

		// Split mode choice model by in/out of CBD for Home-based Work, Home-based other and non-home based
		if (_to_CBD)
		{
			if (_home_based)
			{
				if (activity_type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY || activity_type == Activity_Components::Types::ACTIVITY_TYPES::OTHER_WORK_ACTIVITY)
				{
					utility = ivtt_dif * this->_BHW_CBD_IVTT + wait_dif * this->_BHW_CBD_WAIT + transfer_dif * this->_BHW_CBD_TRANSFER + walk_time_dif * this->_BHW_CBD_WALK + cost_dif * this->_BHW_CBD_COST + this->_BHW_CBD_BIAS;
				}
				else
				{
					utility = ivtt_dif * this->_BHO_CBD_IVTT + wait_dif * this->_BHO_CBD_WAIT + transfer_dif * this->_BHO_CBD_TRANSFER + walk_time_dif * this->_BHO_CBD_WALK + cost_dif * this->_BHO_CBD_COST + this->_BHO_CBD_BIAS;
				}
			}
			else
			{
				utility = ivtt_dif * this->_BNH_CBD_IVTT + wait_dif * this->_BNH_CBD_WAIT + transfer_dif * this->_BNH_CBD_TRANSFER + walk_time_dif * this->_BNH_CBD_WALK + cost_dif * this->_BNH_CBD_COST + this->_BNH_CBD_BIAS;
			}
		}
		else
		{
			if (_home_based)
			{
				if (activity_type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY || activity_type == Activity_Components::Types::ACTIVITY_TYPES::OTHER_WORK_ACTIVITY)
				{
					utility = ivtt_dif * this->_BHW_IVTT + wait_dif * this->_BHW_WAIT + transfer_dif * this->_BHW_TRANSFER + walk_time_dif * this->_BHW_WALK + cost_dif * this->_BHW_COST + this->_BHW_BIAS;
				}
				else
				{
					utility = ivtt_dif * this->_BHO_IVTT + wait_dif * this->_BHO_WAIT + transfer_dif * this->_BHO_TRANSFER + walk_time_dif * this->_BHO_WALK + cost_dif * this->_BHO_COST + this->_BHO_BIAS;
				}
			}
			else
			{
				utility = ivtt_dif * this->_BNH_IVTT + wait_dif * this->_BNH_WAIT + transfer_dif * this->_BNH_TRANSFER + walk_time_dif * this->_BNH_WALK + cost_dif * this->_BNH_COST + this->_BNH_BIAS;
			}
		}
		return utility;
	}

};

struct MasterType
{
	// Choice model types
	typedef Mode_Chooser_Implementation<MT> person_mode_chooser_type;
	typedef Mode_Choice_Option_Impl<MT> mode_choice_option_type;
	typedef Choice_Model_Components::Implementations::Nested_Logit_Model_Implementation<MT> nl_model_type;
};

int main()
{


	cout <<"Press 'any' key to end: ";
	
	char end;
	cin >> end;
}
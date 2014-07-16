#include "Repository\Repository.h"

using namespace polaris;

enum Vehicle_Types
{
	SOV = 1,
	HOV,
	BUS,
	RAIL
};

prototype struct Mode_Chooser : public Choice_Model_Components::Prototypes::Choice_Model<ComponentType>
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
	accessor(B_COST, NONE, NONE);
	accessor(B_TTIME, NONE, NONE);
};

implementation struct Mode_Chooser_Implementation : public Polaris_Component<MasterType,INHERIT(Mode_Chooser_Implementation),Data_Object>
{
	// Tag as Implementation
	typedef typename Polaris_Component<MasterType,INHERIT(Mode_Chooser_Implementation),Data_Object>::Component_Type ComponentType;

	// Pointer to the Parent class
	m_prototype(Choice_Model_Components::Prototypes::Choice_Model, typename MasterType::nl_model_type, Choice_Model, NONE, NONE);
			
	static m_data(int, choice_set_size, NONE, NONE);

	// Interface definitions	
	typedef Choice_Model_Components::Prototypes::Choice_Model<typename MasterType::nl_model_type > _Choice_Model_Interface;
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
implementation struct Mode_Choice_Nest_Impl : public Choice_Model_Components::Implementations::Nested_Choice_Option_Base<MasterType,INHERIT(Mode_Choice_Nest_Impl)>
{
	// Tag as Implementation
	typedef typename Choice_Model_Components::Implementations::Nested_Choice_Option_Base<MasterType,INHERIT(Mode_Choice_Nest_Impl)>::Component_Type ComponentType;

	//====================================================================================================================================
	// Interface definitions
	//------------------------------------------------------------------------------------------------------------------------------------

	// Feature called from prototype and by Choice_Model
	virtual double Calculate_Utility()
	{
		return 0;				
	}

	virtual void Print_Utility()
	{
		//return 0.0;
	}
};
implementation struct Mode_Choice_Option_Impl : public Choice_Model_Components::Implementations::Nested_Choice_Option_Base<MasterType,INHERIT(Mode_Choice_Option_Impl)>
{
	// Tag as Implementation
	typedef typename Choice_Model_Components::Implementations::Nested_Choice_Option_Base<MasterType,INHERIT(Mode_Choice_Option_Impl)>::Component_Type ComponentType;

	// data members
	m_data(float, cost, NONE, NONE);
	m_data(float, travel_time, NONE, NONE);
	m_data(Vehicle_Types, mode, NONE, NONE);

	// PARAMETER DECLARATIONS
	m_data(float, B_COST, NONE, NONE);
	m_data(float, B_TTIME, NONE, NONE);

	//====================================================================================================================================
	// Interface definitions
	//------------------------------------------------------------------------------------------------------------------------------------

	// Feature called from prototype and by Choice_Model
	virtual double Calculate_Utility()
	{
		double utility = _B_COST * _cost + _B_TTIME * _travel_time;
		return utility;				
	}

	virtual void Print_Utility()
	{
		//return 0.0;
	}
};

struct MasterType
{
	// Choice model types
	typedef Mode_Chooser_Implementation<MT> mode_chooser_type;
	typedef Mode_Choice_Option_Impl<MT> mode_choice_option_type;
	typedef Mode_Choice_Nest_Impl<MT> mode_choice_nest_type;
	typedef Choice_Model_Components::Implementations::Nested_Logit_Model_Implementation<MT> nl_model_type;
};

int main()
{
	Simulation_Configuration cfg;
	cfg.Single_Threaded_Setup(1000);
	INITIALIZE_SIMULATION(cfg);


	Choice_Model_Components::Prototypes::Choice_Model<typename MasterType::nl_model_type>* model = (Mode_Chooser<typename MasterType::nl_model_type>*)Allocate<MasterType::nl_model_type>();

	// Create the transit nest and nest options
	Mode_Choice_Option<MasterType::mode_choice_nest_type>* transit_nest = (Mode_Choice_Option<typename MasterType::mode_choice_nest_type>*)Allocate<MasterType::mode_choice_nest_type>();
	transit_nest->inclusive_value_parameter<double>(0.75);
	
	Mode_Choice_Option<MasterType::mode_choice_option_type>* bus = (Mode_Choice_Option<typename MasterType::mode_choice_option_type>*)Allocate<MasterType::mode_choice_option_type>();
	Mode_Choice_Option<MasterType::mode_choice_option_type>* rail = (Mode_Choice_Option<typename MasterType::mode_choice_option_type>*)Allocate<MasterType::mode_choice_option_type>();
	bus->B_COST<double>(-2.0);	bus->cost<double>(2.25);
	rail->B_COST<double>(-2.0);	rail->cost<double>(2.25);
	bus->B_TTIME<double>(-0.3);	bus->travel_time<double>(15.0);
	rail->B_TTIME<double>(-0.2);	rail->travel_time<double>(12.0);
	transit_nest->Add_Sub_Choice_Option<Mode_Choice_Option<MasterType::mode_choice_option_type>*>(bus);
	transit_nest->Add_Sub_Choice_Option<Mode_Choice_Option<MasterType::mode_choice_option_type>*>(rail);

	// create the auto nest and nest options
	Mode_Choice_Option<MasterType::mode_choice_nest_type>* auto_nest = (Mode_Choice_Option<typename MasterType::mode_choice_nest_type>*)Allocate<MasterType::mode_choice_nest_type>();
	auto_nest->inclusive_value_parameter<double>(0.5);

	Mode_Choice_Option<MasterType::mode_choice_option_type>* sov = (Mode_Choice_Option<typename MasterType::mode_choice_option_type>*)Allocate<MasterType::mode_choice_option_type>();
	Mode_Choice_Option<MasterType::mode_choice_option_type>* hov = (Mode_Choice_Option<typename MasterType::mode_choice_option_type>*)Allocate<MasterType::mode_choice_option_type>();
	sov->B_COST<double>(-2.0);	sov->cost<double>(2.50);
	hov->B_COST<double>(-2.5);	hov->cost<double>(3.00);
	sov->B_TTIME<double>(-0.1);	sov->travel_time<double>(8.0);
	hov->B_TTIME<double>(-0.15);	hov->travel_time<double>(13.0);
	auto_nest->Add_Sub_Choice_Option<Mode_Choice_Option<MasterType::mode_choice_option_type>*>(sov);
	auto_nest->Add_Sub_Choice_Option<Mode_Choice_Option<MasterType::mode_choice_option_type>*>(hov);

	model->Add_Choice_Option(transit_nest);
	model->Add_Choice_Option(auto_nest);

	model->Evaluate_Choices<NT>();

	cout << "SOV: "<<sov->choice_utility<float>() <<", "<<sov->choice_probability<float>()<<endl;
	cout << "HOV: "<<hov->choice_utility<float>() <<", "<<hov->choice_probability<float>()<<endl;
	cout << "BUS: "<<bus->choice_utility<float>() <<", "<<bus->choice_probability<float>()<<endl;
	cout << "RAIL: "<<rail->choice_utility<float>() <<", "<<rail->choice_probability<float>()<<endl;
	cout << "AUTO_NEST: "<<auto_nest->choice_utility<float>() <<", "<<auto_nest->choice_probability<float>()<<endl;
	cout << "TRAN_NEST: "<<transit_nest->choice_utility<float>() <<", "<<transit_nest->choice_probability<float>()<<endl;


	cout <<"Press 'any' key to end: ";
	
	char end;
	cin >> end;
}
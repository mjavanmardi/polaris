#pragma once
#include "User_Space_Includes.h"
#include "Person_Prototype.h"
#include "Activity_Location_Prototype.h"
#include "Vehicle_Prototype.h"
#include "Scenario_Prototype.h"

namespace Activity_Components
{
	namespace Types
	{
		enum FLEXIBILITY_VALUES
		{
			LOW_FLEXIBILITY,
			MODERATE_FLEXIBILITY,
			HIGH_FLEXIBILITY
		};
		enum PLAN_HORIZON_VALUES
		{
			ROUTINE,
			PREPLANNED_OVER_7_DAYS,
			PREPLANNED_2_TO_7_DAYS,
			PREPLANNED_PREVIOUS_DAY,
			PREPLANNED_SAME_DAY,
			IMPULSIVE
		};
		enum ACTIVITY_TYPES
		{
			WORK_AT_HOME_ACTIVITY,
			AT_HOME_ACTIVITY,
			PRIMARY_WORK_ACTIVITY,
			OTHER_WORK_ACTIVITY,
			SCHOOL_ACTIVITY,
			MAJOR_SHOPPING_ACTIVITY,
			OTHER_SHOPPING_ACTIVITY,
			SERVICE_VEHICLE_ACTIVITY,
			HEALTHCARE_ACTIVITY,
			PERSONAL_BUSINESS_ACTIVITY,
			ERRANDS_ACTIVITY,
			PICK_UP_OR_DROP_OFF_ACTIVITY,
			EAT_OUT_ACTIVITY,
			RELIGIOUS_OR_CIVIC_ACTIVITY,
			LEISURE_ACTIVITY,
			RECREATION_ACTIVITY,
			SOCIAL_ACTIVITY,
			CHANGE_TRANSPORTATION_ACTIVITY,
			OTHER_ACTIVITY
		};

	}

	namespace Concepts
	{
		concept struct Is_Activity_Plan
		{
			check_getter(has_id,Activity_Plan_ID);
			define_default_check(has_id);
		};
		concept struct Is_Activity_Plan_Prototype
		{
			check_getter(has_id,Component_Type::Activity_Plan_ID);
			define_default_check(has_id);
		};
	}
	
	namespace Prototypes
	{
		prototype struct Activity_Planner ADD_DEBUG_INFO
		{
			declare_feature_conditional(Activity_Planning_Conditional)
			{
				//----------------------------------------------
				// CONDITIONALS FOR BASIC AGENT SCHEDULING
				// 1.) Activity Generation (Occurs 
				// Create alias for this to use in conditional
				typedef Activity_Planner<ComponentType, ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;

				//------------------------------------------------------------------------------------------------------------------------------
				// DURATION_PLANNING Iteration
				if (this_ptr->Is_Current_Iteration(this_ptr->Duration_Planning_Time<Revision&>()))
				{
					// swap in durtion planning event, and make sure it won't be called again
					_pthis->Swap_Event((Event)&Activity_Planner::Duration_Planning_Event<NULLTYPE>);
					this_ptr->Duration_Planning_Time<Revision&>()._iteration = END;
					this_ptr->Duration_Planning_Time<Revision&>()._sub_iteration = END;
					response.result = true;

					// If involved persons to be performed next	
					if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Involved_Persons_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Involved_Persons_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Involved_Persons_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Location_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Location_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Location_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Mode_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Mode_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Mode_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Start_Time_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Start_Time_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Start_Time_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Route_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Route_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Route_Planning_Time<Revision&>()._sub_iteration;
					}
					else
					{
						response.next._iteration = _iteration+1;
						response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
				}
				//------------------------------------------------------------------------------------------------------------------------------
				// INVOLVED_PERSONS_PLANNING Iteration
				else if (this_ptr->Is_Current_Iteration(this_ptr->Involved_Persons_Planning_Time<Revision&>()))
				{
					// swap in durtion planning event, and make sure it won't be called again
					_pthis->Swap_Event((Event)&Activity_Planner::Involved_Persons_Planning_Event<NULLTYPE>);
					this_ptr->Involved_Persons_Planning_Time<Revision&>()._iteration = END;
					this_ptr->Involved_Persons_Planning_Time<Revision&>()._sub_iteration = END;
					response.result = true;

					// If involved persons to be performed next	
					if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Duration_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Duration_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Duration_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Location_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Location_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Location_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Mode_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Mode_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Mode_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Start_Time_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Start_Time_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Start_Time_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Route_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Route_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Route_Planning_Time<Revision&>()._sub_iteration;
					}
					else
					{
						response.next._iteration = _iteration+1;
						response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
				}
				//------------------------------------------------------------------------------------------------------------------------------
				// LOCATION_PLANNING Iteration
				else if (this_ptr->Is_Current_Iteration(this_ptr->Location_Planning_Time<Revision&>()))
				{
					// swap in durtion planning event, and make sure it won't be called again
					_pthis->Swap_Event((Event)&Activity_Planner::Location_Planning_Event<NULLTYPE>);
					this_ptr->Location_Planning_Time<Revision&>()._iteration = END;
					this_ptr->Location_Planning_Time<Revision&>()._sub_iteration = END;
					response.result = true;

					// If involved persons to be performed next	
					if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Duration_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Duration_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Duration_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Involved_Persons_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Involved_Persons_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Involved_Persons_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Mode_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Mode_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Mode_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Start_Time_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Start_Time_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Start_Time_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Route_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Route_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Route_Planning_Time<Revision&>()._sub_iteration;
					}
					else
					{
						response.next._iteration = _iteration+1;
						response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
				}
				//------------------------------------------------------------------------------------------------------------------------------
				// MODE_PLANNING Iteration
				else if (this_ptr->Is_Current_Iteration(this_ptr->Mode_Planning_Time<Revision&>()))
				{
					// swap in durtion planning event, and make sure it won't be called again
					_pthis->Swap_Event((Event)&Activity_Planner::Mode_Planning_Event<NULLTYPE>);
					this_ptr->Mode_Planning_Time<Revision&>()._iteration = END;
					this_ptr->Mode_Planning_Time<Revision&>()._sub_iteration = END;
					response.result = true;

					// If involved persons to be performed next	
					if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Duration_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Duration_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Duration_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Involved_Persons_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Involved_Persons_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Involved_Persons_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Location_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Location_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Location_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Start_Time_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Start_Time_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Start_Time_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Route_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Route_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Route_Planning_Time<Revision&>()._sub_iteration;
					}
					else
					{
						response.next._iteration = _iteration+1;
						response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
				}
				//------------------------------------------------------------------------------------------------------------------------------
				// START_TIME_PLANNING Iteration
				else if (this_ptr->Is_Current_Iteration(this_ptr->Start_Time_Planning_Time<Revision&>()))
				{
					// swap in durtion planning event, and make sure it won't be called again
					_pthis->Swap_Event((Event)&Activity_Planner::Start_Time_Planning_Event<NULLTYPE>);
					this_ptr->Start_Time_Planning_Time<Revision&>()._iteration = END;
					this_ptr->Start_Time_Planning_Time<Revision&>()._sub_iteration = END;
					response.result = true;
				
					// If involved persons to be performed next	
					if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Duration_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Duration_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Duration_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Involved_Persons_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Involved_Persons_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Involved_Persons_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Location_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Location_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Location_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Mode_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Mode_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Mode_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Route_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Route_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Route_Planning_Time<Revision&>()._sub_iteration;
					}
					else
					{
						response.next._iteration = _iteration+1;
						response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
				}
				//------------------------------------------------------------------------------------------------------------------------------
				// START_TIME_PLANNING Iteration
				else if (this_ptr->Is_Current_Iteration(this_ptr->Route_Planning_Time<Revision&>()))
				{
					// swap in durtion planning event, and make sure it won't be called again
					_pthis->Swap_Event((Event)&Activity_Planner::Route_Planning_Event<NULLTYPE>);
					this_ptr->Route_Planning_Time<Revision&>()._iteration = END;
					this_ptr->Route_Planning_Time<Revision&>()._sub_iteration = END;
					response.result = true;
				
					// If involved persons to be performed next	
					if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Duration_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Duration_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Duration_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Involved_Persons_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Involved_Persons_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Involved_Persons_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Location_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Location_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Location_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Mode_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Mode_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Mode_Planning_Time<Revision&>()._sub_iteration;
					}
					else if(this_ptr->Is_Minimum_Plan_Time(this_ptr->Start_Time_Planning_Time<Revision&>())) 
					{	
						response.next._iteration = this_ptr->Start_Time_Planning_Time<Revision&>()._iteration;
						response.next._sub_iteration = this_ptr->Start_Time_Planning_Time<Revision&>()._sub_iteration;
					}
					else
					{
						response.next._iteration = _iteration+1;
						response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
				}
				else if (_sub_iteration == Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION)
				{
					// add activity to schedule
					_pthis->Swap_Event((Event)&Activity_Planner::Add_Activity_To_Schedule_Event<NULLTYPE>);
					response.result = true;
					response.next._iteration = END;
					response.next._sub_iteration = END;
				}
			}
			declare_feature_event(Location_Planning_Event)
			{
				typedef Activity_Planner<ComponentType, ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->Location_Planning_Event_Handler<NT>();
			}
			feature_prototype void Location_Planning_Event_Handler()
			{
				this_component()->Location_Planning_Event_Handler<ComponentType,CallerType,TargetType>();
			}
			declare_feature_event(Mode_Planning_Event)
			{
				typedef Activity_Planner<ComponentType, ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->Mode_Planning_Event_Handler<NT>();
			}
			feature_prototype void Mode_Planning_Event_Handler()
			{
				this_component()->Mode_Planning_Event_Handler<ComponentType,CallerType,TargetType>();
			}
			declare_feature_event(Start_Time_Planning_Event)
			{
				typedef Activity_Planner<ComponentType, ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->Start_Time_Planning_Event_Handler<NT>();
			}
			feature_prototype void Start_Time_Planning_Event_Handler()
			{
				this_component()->Start_Time_Planning_Event_Handler<ComponentType,CallerType,TargetType>();
			}
			declare_feature_event(Duration_Planning_Event)
			{
				typedef Activity_Planner<ComponentType, ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->Duration_Planning_Event_Handler<NT>();
			}
			feature_prototype void Duration_Planning_Event_Handler()
			{
				this_component()->Duration_Planning_Event_Handler<ComponentType,CallerType,TargetType>();
			}
			declare_feature_event(Involved_Persons_Planning_Event)
			{
				typedef Activity_Planner<ComponentType, ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->Involved_Persons_Planning_Event_Handler<NT>();
			}
			feature_prototype void Involved_Persons_Planning_Event_Handler()
			{
				this_component()->Involved_Persons_Planning_Event_Handler<ComponentType,CallerType,TargetType>();
			}
			declare_feature_event(Route_Planning_Event)
			{
				typedef Activity_Planner<ComponentType, ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->Route_Planning_Event_Handler<NT>();
			}
			feature_prototype void Route_Planning_Event_Handler()
			{
				this_component()->Route_Planning_Event_Handler<ComponentType,CallerType,TargetType>();
			}
			declare_feature_event(Add_Activity_To_Schedule_Event)
			{
				typedef Activity_Planner<ComponentType, ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->Add_Activity_To_Schedule_Event_Handler<NT>();
			}
			feature_prototype void Add_Activity_To_Schedule_Event_Handler()
			{
				this_component()->Add_Activity_To_Schedule_Event_Handler<ComponentType,CallerType,TargetType>();
			}

			// Pointer back to planner
			feature_accessor(Parent_Planner, none, none);

			// Fundamental activity properties
			feature_accessor(Activity_Plan_ID, check(ReturnValueType,is_arithmetic), check(SetValueType,is_arithmetic));
			feature_accessor(Activity_Plan_Horizon,none,none);
			feature_accessor(Activity_Planning_Time, check(strip_modifiers(ReturnValueType),Basic_Units::Concepts::Is_Time_Value), check(strip_modifiers(SetValueType),Basic_Units::Concepts::Is_Time_Value));
			feature_accessor(Location_Planning_Time, check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			feature_accessor(Mode_Planning_Time, check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			feature_accessor(Start_Time_Planning_Time, check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			feature_accessor(Duration_Planning_Time, check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			feature_accessor(Involved_Persons_Planning_Time, check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			feature_accessor(Route_Planning_Time, check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));

			// Activity attribute planning properties		
			feature_accessor(Location_Plan_Horizon, none, none);
			feature_accessor(Location_Flexibility, none, none);
			feature_accessor(Mode_Plan_Horizon, none, none);
			feature_accessor(Mode_Flexibility, none, none);
			feature_accessor(Start_Time_Plan_Horizon,none,none);
			feature_accessor(Start_Time_Flexibility,none,none);
			feature_accessor(Duration_Plan_Horizon,none,none);
			feature_accessor(Duration_Flexibility,none,none);
			feature_accessor(Involved_Persons_Plan_Horizon,none,none);
			feature_accessor(Involved_Persons_Flexibility,none,none);

			// Activity attributes
			feature_accessor(Activity_Type, none,none);
			feature_accessor(Location, check(ReturnValueType,Activity_Location_Components::Concepts::Is_Activity_Location), check(SetValueType,Activity_Location_Components::Concepts::Is_Activity_Location));
			feature_accessor(Mode, none, none);
			feature_accessor(Start_Time, check(ReturnValueType,Basic_Units::Concepts::Is_Time_Value), check(SetValueType,Basic_Units::Concepts::Is_Time_Value)); 
			feature_accessor(Duration, check(ReturnValueType,Basic_Units::Concepts::Is_Time_Value), check(SetValueType,Basic_Units::Concepts::Is_Time_Value)); 
			feature_accessor(Involved_Persons_Container,none,none);
			feature_accessor(Expected_Travel_Time, check(ReturnValueType,Basic_Units::Concepts::Is_Time_Value), check(SetValueType,Basic_Units::Concepts::Is_Time_Value)); 

			feature_prototype void Initialize(typename TargetType::ParamType activity_type, typename TargetType::Param2Type planning_time, requires(check(TargetType,Is_Target_Type_Struct) && check_2(typename TargetType::ParamType, Types::ACTIVITY_TYPES, is_same)))
			{
				this_component()->Initialize<ComponentType, ComponentType, typename TargetType::ParamType>(activity_type);
				this->Set_Attribute_Planning_Times<typename TargetType::Param2Type>(planning_time);
			}
			feature_prototype void Initialize(typename TargetType::ParamType activity_type, typename TargetType::Param2Type planning_time, requires(!check(TargetType,Is_Target_Type_Struct) || !check_2(typename TargetType::ParamType, Types::ACTIVITY_TYPES, is_same)))
			{
				assert_check(TargetType,Is_Target_Type_Struct, "The TargetType for Activity.Initialize must be a Target_Type struct.");
				assert_check_2(typename TargetType::ParamType, Types::ACTIVITY_TYPES, is_same, "TargetType::ParamType must be an ActivityType enumerator");
			}
			feature_prototype void Set_Attribute_Planning_Times(TargetType planning_time)
			{
				// Call the model to determine the attribute planning times
				this_component()->Set_Attribute_Planning_Times<ComponentType,CallerType,TargetType>(planning_time);

				// store for later use
				Revision& persons = this->Involved_Persons_Planning_Time<Revision&>();
				Revision& mode = this->Mode_Planning_Time<Revision&>();
				Revision& duration = this->Duration_Planning_Time<Revision&>();
				Revision& location = this->Location_Planning_Time<Revision&>();
				Revision& start_time = this->Start_Time_Planning_Time<Revision&>();
				Revision& route = this->Route_Planning_Time<Revision&>();

				// Load the first event
				if (Is_Minimum_Plan_Time(persons)) load_event(ComponentType,Activity_Planning_Conditional, Involved_Persons_Planning_Event, persons._iteration, persons._sub_iteration,NT);
				else if (Is_Minimum_Plan_Time(mode)) load_event(ComponentType,Activity_Planning_Conditional, Mode_Planning_Event, mode._iteration, mode._sub_iteration,NT);
				else if (Is_Minimum_Plan_Time(duration)) load_event(ComponentType,Activity_Planning_Conditional, Duration_Planning_Event, duration._iteration, duration._sub_iteration,NT);
				else if (Is_Minimum_Plan_Time(location)) load_event(ComponentType,Activity_Planning_Conditional, Location_Planning_Event, location._iteration, location._sub_iteration,NT);
				else if (Is_Minimum_Plan_Time(start_time)) load_event(ComponentType,Activity_Planning_Conditional, Start_Time_Planning_Event, start_time._iteration, start_time._sub_iteration,NT);
				else if (Is_Minimum_Plan_Time(route)) load_event(ComponentType,Activity_Planning_Conditional, Route_Planning_Event, route._iteration, route._sub_iteration,NT);
				else {THROW_EXCEPTION("ERROR: Invalid planning times set");}
			}
			feature_prototype bool Is_Minimum_Plan_Time(TargetType plan_time, requires(check_2(TargetType, Revision,is_same)))
			{
				Revision& rev = plan_time;
				Revision& persons = this->Involved_Persons_Planning_Time<Revision&>();
				Revision& mode = this->Mode_Planning_Time<Revision&>();
				Revision& duration = this->Duration_Planning_Time<Revision&>();
				Revision& location = this->Location_Planning_Time<Revision&>();
				Revision& start_time = this->Start_Time_Planning_Time<Revision&>();
				Revision& route = this->Route_Planning_Time<Revision&>();

				if (rev._iteration <= persons._iteration && rev._sub_iteration <= persons._sub_iteration && rev._iteration <= mode._iteration && rev._sub_iteration <= mode._sub_iteration && 
					rev._iteration <= duration._iteration && rev._sub_iteration <= duration._sub_iteration && rev._iteration <= location._iteration && rev._sub_iteration <= location._sub_iteration && 
					rev._iteration <= start_time._iteration && rev._sub_iteration <= start_time._sub_iteration && rev._iteration <= route._iteration && rev._sub_iteration <= route._sub_iteration&& rev._iteration != END)
				{
					return true;
				}
				return false;
			}
			feature_prototype bool Is_Current_Iteration(TargetType plan_time, requires(check_2(TargetType, Revision,is_same)))
			{
				return (plan_time._iteration == _iteration && plan_time._sub_iteration == _sub_iteration);
			}
		};


	}
}

using namespace Activity_Components::Prototypes;
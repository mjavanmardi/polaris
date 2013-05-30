#pragma once
#include "User_Space_Includes.h"
#include "Person_Prototype.h"
#include "Activity_Location_Prototype.h"
#include "Movement_Plan_Prototype.h"
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
			DELETED=-1,
			EMPTY=-2,
			WORK_AT_HOME_ACTIVITY=0,
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
		concept struct Is_Activity_Plan_Prototype
		{
			check_getter(has_id,ComponentType::Activity_Plan_ID);
			define_default_check(has_id);
		};
		concept struct Is_Activity_Plan
		{
			check_getter(has_id,Activity_Plan_ID);
			check_concept(is_prototype, Is_Activity_Plan_Prototype);
			define_default_check(has_id || is_prototype);
		};
	}
	
	namespace Prototypes
	{
		prototype struct Activity_Planner ADD_DEBUG_INFO
		{
			//===========================================
			// Pointer back to planner parent class
			//-------------------------------------------
			feature_accessor(Parent_Planner, none, none);
			feature_accessor(Parent_ID, none, none);
			feature_accessor(movement_plan, none, none);
			feature_accessor(movement_record,check(ReturnValueType,Movement_Plan_Components::Concepts::Is_Movement_Record_Prototype),none);

			//===========================================
			// Activity Plan execution scheduling functionality
			//-------------------------------------------
			declare_feature_conditional(Activity_Planning_Conditional)
			{		
				//----------------------------------------------
				// CONDITIONALS FOR BASIC ACTIVITY PLAN SCHEDULING
				typedef Activity_Planner<ComponentType, ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				response.result = true;

				//------------------------------------------------------------------------------------------------------------------------------
				// DURATION_PLANNING Iteration
				if (this_ptr->Is_Current_Iteration(this_ptr->template Duration_Planning_Time<Revision&>()))
				{
					// swap in durtion planning event, and make sure it won't be called again
					_pthis->Swap_Event((Event)&Activity_Planner::Duration_Planning_Event<NULLTYPE>);
					this_ptr->template Duration_Planning_Time<Revision&>()._iteration = END;
					this_ptr->template Duration_Planning_Time<Revision&>()._sub_iteration = END;
					
					// Determine the next event to fire
					if		(this_ptr->Is_Minimum_Plan_Time(this_ptr->template Involved_Persons_Planning_Time<Revision&>())) this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Involved_Persons_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Location_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Location_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Mode_Planning_Time<Revision&>()))				this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Mode_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Start_Time_Planning_Time<Revision&>()))		this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Start_Time_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Route_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Route_Planning_Time<Revision&>(),true);
					else
					{
						response.next._iteration = _iteration+5;
						response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
				}
				//------------------------------------------------------------------------------------------------------------------------------
				// INVOLVED_PERSONS_PLANNING Iteration
				else if (this_ptr->Is_Current_Iteration(this_ptr->template Involved_Persons_Planning_Time<Revision&>()))
				{
					// swap in durtion planning event, and make sure it won't be called again
					_pthis->Swap_Event((Event)&Activity_Planner::Involved_Persons_Planning_Event<NULLTYPE>);
					this_ptr->template Involved_Persons_Planning_Time<Revision&>()._iteration = END;
					this_ptr->template Involved_Persons_Planning_Time<Revision&>()._sub_iteration = END;

					// Determine the next event to fire
					if		(this_ptr->Is_Minimum_Plan_Time(this_ptr->template Duration_Planning_Time<Revision&>()))		this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Duration_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Location_Planning_Time<Revision&>()))		this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Location_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Mode_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Mode_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Start_Time_Planning_Time<Revision&>()))	this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Start_Time_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Route_Planning_Time<Revision&>()))		this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Route_Planning_Time<Revision&>(),true);
					else
					{
						response.next._iteration = _iteration+5;
						response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
				}
				//------------------------------------------------------------------------------------------------------------------------------
				// LOCATION_PLANNING Iteration
				else if (this_ptr->Is_Current_Iteration(this_ptr->template Location_Planning_Time<Revision&>()))
				{
					// swap in durtion planning event, and make sure it won't be called again
					_pthis->Swap_Event((Event)&Activity_Planner::Location_Planning_Event<NULLTYPE>);
					this_ptr->template Location_Planning_Time<Revision&>()._iteration = END;
					this_ptr->template Location_Planning_Time<Revision&>()._sub_iteration = END;

					// Determine the next event to fire
					if		(this_ptr->Is_Minimum_Plan_Time(this_ptr->template Involved_Persons_Planning_Time<Revision&>())) this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Involved_Persons_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Duration_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Duration_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Mode_Planning_Time<Revision&>()))				this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Mode_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Start_Time_Planning_Time<Revision&>()))		this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Start_Time_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Route_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Route_Planning_Time<Revision&>(),true);
					else
					{
						response.next._iteration = _iteration+5;
						response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
				}
				//------------------------------------------------------------------------------------------------------------------------------
				// MODE_PLANNING Iteration
				else if (this_ptr->Is_Current_Iteration(this_ptr->template Mode_Planning_Time<Revision&>()))
				{
					// swap in durtion planning event, and make sure it won't be called again
					_pthis->Swap_Event((Event)&Activity_Planner::Mode_Planning_Event<NULLTYPE>);
					this_ptr->template Mode_Planning_Time<Revision&>()._iteration = END;
					this_ptr->template Mode_Planning_Time<Revision&>()._sub_iteration = END;

					// Determine the next event to fire
					if		(this_ptr->Is_Minimum_Plan_Time(this_ptr->template Involved_Persons_Planning_Time<Revision&>())) this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Involved_Persons_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Duration_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Duration_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Location_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Location_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Start_Time_Planning_Time<Revision&>()))		this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Start_Time_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Route_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Route_Planning_Time<Revision&>(),true);
					else
					{
						response.next._iteration = _iteration+5;
						response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
				}
				//------------------------------------------------------------------------------------------------------------------------------
				// START_TIME_PLANNING Iteration
				else if (this_ptr->Is_Current_Iteration(this_ptr->template Start_Time_Planning_Time<Revision&>()))
				{
					// swap in durtion planning event, and make sure it won't be called again
					_pthis->Swap_Event((Event)&Activity_Planner::Start_Time_Planning_Event<NULLTYPE>);
					this_ptr->template Start_Time_Planning_Time<Revision&>()._iteration = END;
					this_ptr->template Start_Time_Planning_Time<Revision&>()._sub_iteration = END;
									
					// Determine the next event to fire
					if		(this_ptr->Is_Minimum_Plan_Time(this_ptr->template Involved_Persons_Planning_Time<Revision&>())) this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Involved_Persons_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Duration_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Duration_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Location_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Location_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Mode_Planning_Time<Revision&>()))				this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Mode_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Route_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Route_Planning_Time<Revision&>(),true);
					else
					{
						response.next._iteration = _iteration+5;
						response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
				}
				//------------------------------------------------------------------------------------------------------------------------------
				// ROUTE_PLANNING Iteration
				else if (this_ptr->Is_Current_Iteration(this_ptr->template Route_Planning_Time<Revision&>()))
				{
					// swap in durtion planning event, and make sure it won't be called again
					_pthis->Swap_Event((Event)&Activity_Planner::Route_Planning_Event<NULLTYPE>);
					this_ptr->template Route_Planning_Time<Revision&>()._iteration = END;
					this_ptr->template Route_Planning_Time<Revision&>()._sub_iteration = END;
									
					// Determine the next event to fire
					if		(this_ptr->Is_Minimum_Plan_Time(this_ptr->template Involved_Persons_Planning_Time<Revision&>())) this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Involved_Persons_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Duration_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Duration_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Location_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Location_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Mode_Planning_Time<Revision&>()))				this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Mode_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Start_Time_Planning_Time<Revision&>()))		this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Start_Time_Planning_Time<Revision&>(),true);
					else
					{
						response.next._iteration = _iteration+5;
						response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
				}
				else if (_sub_iteration == Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION)
				{
					// add activity to schedule
					_pthis->Swap_Event((Event)&Activity_Planner::Add_Activity_To_Schedule_Event<NULLTYPE>);
					response.next._iteration = END;
					response.next._sub_iteration = END;
				}
				// catch any sub_iterations which have been modified (currenlty limited to routing, which can be changed by start_time_plan iteration)
				// this will hit only if start time plan iteration is called immidiately be
				else
				{
					Revision& route_itr = this_ptr->template Route_Planning_Time<Revision&>();
					if(route_itr._iteration  != END && route_itr._iteration >= _iteration) 
					{
						response.next._iteration = route_itr._iteration;
						response.next._sub_iteration = route_itr._sub_iteration;
					}
					response.result = false;
				}
			}
			feature_prototype void Set_As_Next_Revison(Conditional_Response& response, Revision& revision, bool result)
			{
				response.result = result;
				response.next._iteration = revision._iteration;
				response.next._sub_iteration = revision._sub_iteration;
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

				if (rev._revision <= persons._revision && rev._revision <= mode._revision && 
					rev._revision <= duration._revision && rev._revision <= location._revision && 
					rev._revision <= start_time._revision && rev._revision <= route._revision && rev._iteration != END)
				{
					// set next revision
					return true;
				}
				return false;
			}
			feature_prototype bool Is_Current_Iteration(TargetType plan_time, requires(check_2(TargetType, Revision,is_same)))
			{
				return (plan_time._iteration == _iteration && plan_time._sub_iteration == _sub_iteration);
			}

			//===========================================
			// Activity Planning events
			//-------------------------------------------
			declare_feature_event(Location_Planning_Event)
			{
				typedef Activity_Planner<ComponentType, ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->template Location_Planning_Event_Handler<NT>();
			}
			feature_prototype void Location_Planning_Event_Handler()
			{
				this_component()->template Location_Planning_Event_Handler<ComponentType,CallerType,TargetType>();
				this->Location_Planning_Time<Revision&>()._iteration = END+1;
				this->Location_Planning_Time<Revision&>()._sub_iteration = END+1;
			}
			declare_feature_event(Mode_Planning_Event)
			{
				typedef Activity_Planner<ComponentType, ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->template Mode_Planning_Event_Handler<NT>();
			}
			feature_prototype void Mode_Planning_Event_Handler()
			{
				this_component()->template Mode_Planning_Event_Handler<ComponentType,CallerType,TargetType>();
				this->Mode_Planning_Time<Revision&>()._iteration = END+1;
				this->Mode_Planning_Time<Revision&>()._sub_iteration = END+1;
			}
			declare_feature_event(Start_Time_Planning_Event)
			{
				typedef Activity_Planner<ComponentType, ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->template Start_Time_Planning_Event_Handler<NT>();
			}
			feature_prototype void Start_Time_Planning_Event_Handler()
			{
				this_component()->template Start_Time_Planning_Event_Handler<ComponentType,CallerType,TargetType>();
				this->Start_Time_Planning_Time<Revision&>()._iteration = END+1;
				this->Start_Time_Planning_Time<Revision&>()._sub_iteration = END+1;
			}
			declare_feature_event(Duration_Planning_Event)
			{
				typedef Activity_Planner<ComponentType, ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->template Duration_Planning_Event_Handler<NT>();
			}
			feature_prototype void Duration_Planning_Event_Handler()
			{
				this_component()->template Duration_Planning_Event_Handler<ComponentType,CallerType,TargetType>();
				// set duration planning time schedule to END - duration Is Planned
				this->Duration_Planning_Time<Revision&>()._iteration = END;
				this->Duration_Planning_Time<Revision&>()._sub_iteration = END;
			}
			declare_feature_event(Involved_Persons_Planning_Event)
			{
				typedef Activity_Planner<ComponentType, ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->template Involved_Persons_Planning_Event_Handler<NT>();
			}
			feature_prototype void Involved_Persons_Planning_Event_Handler()
			{
				this_component()->template Involved_Persons_Planning_Event_Handler<ComponentType,CallerType,TargetType>();
				this->Involved_Persons_Planning_Time<Revision&>()._iteration = END+1;
				this->Involved_Persons_Planning_Time<Revision&>()._sub_iteration = END+1;
			}
			declare_feature_event(Route_Planning_Event)
			{
				typedef Activity_Planner<ComponentType, ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->template Route_Planning_Event_Handler<NT>();
			}
			feature_prototype void Route_Planning_Event_Handler()
			{
				this_component()->template Route_Planning_Event_Handler<ComponentType,CallerType,TargetType>();
				this->Route_Planning_Time<Revision&>()._iteration = END+1;
				this->Route_Planning_Time<Revision&>()._sub_iteration = END+1;
			}
			declare_feature_event(Add_Activity_To_Schedule_Event)
			{
				typedef Activity_Planner<ComponentType, ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->template Add_Activity_To_Schedule_Event_Handler<NT>();
			}
			feature_prototype void Add_Activity_To_Schedule_Event_Handler()
			{
				this_component()->template Add_Activity_To_Schedule_Event_Handler<ComponentType,CallerType,TargetType>();
			}

			//===========================================
			// Fundamental activity 'meta-attributes'
			//-------------------------------------------
			feature_accessor(Activity_Plan_ID, check(ReturnValueType,is_arithmetic), check(SetValueType,is_arithmetic));
			feature_accessor(Activity_Plan_Horizon,none,none);
			feature_accessor(Activity_Planning_Time, check(strip_modifiers(ReturnValueType),Basic_Units::Concepts::Is_Time_Value), check(strip_modifiers(SetValueType),Basic_Units::Concepts::Is_Time_Value));
			feature_accessor(Location_Planning_Time, check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			feature_accessor(Mode_Planning_Time, check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			feature_accessor(Start_Time_Planning_Time, check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			feature_accessor(Duration_Planning_Time, check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			feature_accessor(Involved_Persons_Planning_Time, check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			feature_accessor(Route_Planning_Time, none,none/*check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same)*/);
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

			//===========================================
			// Primary Activity attribute accessors
			//-------------------------------------------
			feature_accessor(Activity_Type, none,none);
			feature_accessor(Location, check(ReturnValueType,Activity_Location_Components::Concepts::Is_Activity_Location), check(SetValueType,Activity_Location_Components::Concepts::Is_Activity_Location));
			feature_accessor(Mode, none, none);
			feature_accessor(Start_Time, check(ReturnValueType,Basic_Units::Concepts::Is_Time_Value), check(SetValueType,Basic_Units::Concepts::Is_Time_Value)); 
			feature_accessor(Duration, check(ReturnValueType,Basic_Units::Concepts::Is_Time_Value), check(SetValueType,Basic_Units::Concepts::Is_Time_Value)); 
			feature_accessor(Involved_Persons_Container,none,none);
			feature_accessor(Expected_Travel_Time, check(ReturnValueType,Basic_Units::Concepts::Is_Time_Value), check(SetValueType,Basic_Units::Concepts::Is_Time_Value)); 
			feature_accessor(Actual_Travel_Time, check(ReturnValueType,Basic_Units::Concepts::Is_Time_Value), check(SetValueType,Basic_Units::Concepts::Is_Time_Value)); 

			//===========================================
			// Activity Planner Methods
			//-------------------------------------------
			feature_prototype void Initialize(TargetType activity/*, requires(check(TargetType,Concepts::Is_Activity_Plan_Prototype))*/)
			{
				this_component()->template Initialize<ComponentType,CallerType,TargetType>(activity);
			}
			feature_prototype void Initialize(typename TargetType::ParamType act_type, typename TargetType::Param2Type planning_time, requires(check(TargetType,Is_Target_Type_Struct) && check_2(typename TargetType::ParamType, Types::ACTIVITY_TYPES, is_same)))
			{
				this_component()->template Initialize<ComponentType, ComponentType, typename TargetType::ParamType>(act_type);
				this->Set_Meta_Attributes<void>();
				this->Set_Attribute_Planning_Times<typename TargetType::Param2Type>(planning_time);
			}
			feature_prototype void Initialize(typename TargetType::ParamType act_type, typename TargetType::Param2Type planning_time, requires(!check(TargetType,Is_Target_Type_Struct) || !check_2(typename TargetType::ParamType, Types::ACTIVITY_TYPES, is_same)))
			{
				assert_check(TargetType,Is_Target_Type_Struct, "The TargetType for Activity.Initialize must be a Target_Type struct.");
				assert_check_2(typename TargetType::ParamType, Types::ACTIVITY_TYPES, is_same, "TargetType::ParamType must be an ActivityType enumerator");
			}
			feature_prototype void Set_Attribute_Planning_Times(TargetType planning_time)
			{
				// Call the model to determine the attribute planning times
				this_component()->template Set_Attribute_Planning_Times<ComponentType,CallerType,TargetType>(planning_time);

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
				else 
				{
					stringstream err;
					err << "ERROR: Invalid planning times set: ";
					err << endl << "Duration: "<<duration._iteration <<","<<duration._sub_iteration;
					err << endl << "Start: "<<start_time._iteration <<","<<start_time._sub_iteration;
					err << endl << "Location: "<<location._iteration <<","<<location._sub_iteration; 
					err << endl << "Route: "<<route._iteration <<","<<route._sub_iteration; 
					THROW_WARNING(err);
				}
			}
			feature_method_void(Set_Meta_Attributes,none);	

			feature_prototype void Arrive_At_Activity()
			{
				define_component_interface(_Logger_Interface, MasterType::person_data_logger_type, Person_Components::Prototypes::Person_Data_Logger, NULLTYPE);	
				((_Logger_Interface*)_global_person_logger)->Add_Record<Activity_Planner<ComponentType,CallerType>*>(this,true);
			}

			// features to check if activity attributes have been planned - calculated based on reviion times for attribute planes (true if set to END)
			feature_method_void(Location_Is_Planned,check_2(ReturnValueType,bool,is_same));
			feature_method_void(Mode_Is_Planned,check_2(ReturnValueType,bool,is_same));
			feature_method_void(Start_Is_Planned,check_2(ReturnValueType,bool,is_same));
			feature_method_void(Duration_Is_Planned,check_2(ReturnValueType,bool,is_same));
			feature_method_void(Involved_Persons_Is_Planned,check_2(ReturnValueType,bool,is_same));
			feature_method_void(Route_Is_Planned,check_2(ReturnValueType,bool,is_same));
		};


	}
}

using namespace Activity_Components::Prototypes;
using namespace Activity_Components::Types;
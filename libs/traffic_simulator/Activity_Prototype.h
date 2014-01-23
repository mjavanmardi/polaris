#pragma once
#include "User_Space_Includes.h"
#include "Person_Prototype.h"
#include "Person_Data_Logger_Prototype.h"
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
			check_template_method_name(has_id,Component_Type::Activity_Plan_ID);
			define_default_check(has_id);
		};
		concept struct Is_Activity_Plan
		{
			check_template_method_name(has_id,Activity_Plan_ID);
			check_concept(is_prototype, Is_Activity_Plan_Prototype, T, V);
			define_default_check(has_id || is_prototype);
		};
	}
	
	namespace Prototypes
	{
		prototype struct Activity_Planner ADD_DEBUG_INFO
		{
			tag_as_prototype;

			//===========================================
			// Pointer back to planner parent class
			//-------------------------------------------
			accessor(Parent_Planner, NONE, NONE);
			accessor(Parent_ID, NONE, NONE);
			accessor(movement_plan, NONE, NONE);
			accessor(movement_record,check(strip_modifiers(TargetType),Movement_Plan_Components::Concepts::Is_Movement_Record_Prototype), NONE);

			//===========================================
			// Activity Plan execution scheduling functionality
			//-------------------------------------------
			template<typename TargetType> void Activity_Planning_Conditional()
			{		
				//----------------------------------------------
				// CONDITIONALS FOR BASIC ACTIVITY PLAN SCHEDULING
				typedef Activity_Planner<ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				response.result = true;

				int id =  this_ptr->Parent_ID<int>();
				int actid = this_ptr->Activity_Plan_ID<int>();

				//------------------------------------------------------------------------------------------------------------------------------
				// DURATION_PLANNING Iteration
				if (this_ptr->Is_Current_Iteration(this_ptr->template Duration_Planning_Time<Revision&>()))
				{
					// swap in durtion planning event, and make sure it won't be called again
					_pthis->Swap_Event((Event)&Activity_Planner::Duration_Planning_Event<NULLTYPE>);
					this_ptr->template Duration_Planning_Time<Revision&>().iteration() = END;
					this_ptr->template Duration_Planning_Time<Revision&>()._subiteration() = END;
					
					// Determine the next event to fire
					if		(this_ptr->Is_Minimum_Plan_Time(this_ptr->template Involved_Persons_Planning_Time<Revision&>())) this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Involved_Persons_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Location_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Location_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Mode_Planning_Time<Revision&>()))				this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Mode_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Start_Time_Planning_Time<Revision&>()))		this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Start_Time_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Route_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Route_Planning_Time<Revision&>(),true);
					else
					{
						response.next.iteration() = iteration()+5;
						response.next._subiteration() = Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
				}
				//------------------------------------------------------------------------------------------------------------------------------
				// INVOLVED_PERSONS_PLANNING Iteration
				else if (this_ptr->Is_Current_Iteration(this_ptr->template Involved_Persons_Planning_Time<Revision&>()))
				{
					// swap in durtion planning event, and make sure it won't be called again
					_pthis->Swap_Event((Event)&Activity_Planner::Involved_Persons_Planning_Event<NULLTYPE>);
					this_ptr->template Involved_Persons_Planning_Time<Revision&>().iteration() = END;
					this_ptr->template Involved_Persons_Planning_Time<Revision&>()._subiteration() = END;

					// Determine the next event to fire
					if		(this_ptr->Is_Minimum_Plan_Time(this_ptr->template Duration_Planning_Time<Revision&>()))		this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Duration_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Location_Planning_Time<Revision&>()))		this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Location_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Mode_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Mode_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Start_Time_Planning_Time<Revision&>()))	this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Start_Time_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Route_Planning_Time<Revision&>()))		this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Route_Planning_Time<Revision&>(),true);
					else
					{
						response.next.iteration() = iteration()+5;
						response.next._subiteration() = Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
				}
				//------------------------------------------------------------------------------------------------------------------------------
				// LOCATION_PLANNING Iteration
				else if (this_ptr->Is_Current_Iteration(this_ptr->template Location_Planning_Time<Revision&>()))
				{
					// swap in durtion planning event, and make sure it won't be called again
					_pthis->Swap_Event((Event)&Activity_Planner::Location_Planning_Event<NULLTYPE>);
					this_ptr->template Location_Planning_Time<Revision&>().iteration() = END;
					this_ptr->template Location_Planning_Time<Revision&>()._subiteration() = END;

					// Determine the next event to fire
					if		(this_ptr->Is_Minimum_Plan_Time(this_ptr->template Involved_Persons_Planning_Time<Revision&>())) this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Involved_Persons_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Duration_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Duration_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Mode_Planning_Time<Revision&>()))				this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Mode_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Start_Time_Planning_Time<Revision&>()))		this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Start_Time_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Route_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Route_Planning_Time<Revision&>(),true);
					else
					{
						response.next.iteration() = iteration()+5;
						response.next._subiteration() = Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
				}
				//------------------------------------------------------------------------------------------------------------------------------
				// MODE_PLANNING Iteration
				else if (this_ptr->Is_Current_Iteration(this_ptr->template Mode_Planning_Time<Revision&>()))
				{
					// swap in durtion planning event, and make sure it won't be called again
					_pthis->Swap_Event((Event)&Activity_Planner::Mode_Planning_Event<NULLTYPE>);
					this_ptr->template Mode_Planning_Time<Revision&>().iteration() = END;
					this_ptr->template Mode_Planning_Time<Revision&>()._subiteration() = END;

					// Determine the next event to fire
					if		(this_ptr->Is_Minimum_Plan_Time(this_ptr->template Involved_Persons_Planning_Time<Revision&>())) this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Involved_Persons_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Duration_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Duration_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Location_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Location_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Start_Time_Planning_Time<Revision&>()))		this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Start_Time_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Route_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Route_Planning_Time<Revision&>(),true);
					else
					{
						response.next.iteration() = iteration()+5;
						response.next._subiteration() = Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
				}
				//------------------------------------------------------------------------------------------------------------------------------
				// START_TIME_PLANNING Iteration
				else if (this_ptr->Is_Current_Iteration(this_ptr->template Start_Time_Planning_Time<Revision&>()))
				{
					// swap in durtion planning event, and make sure it won't be called again
					_pthis->Swap_Event((Event)&Activity_Planner::Start_Time_Planning_Event<NULLTYPE>);
					this_ptr->template Start_Time_Planning_Time<Revision&>().iteration() = END;
					this_ptr->template Start_Time_Planning_Time<Revision&>()._subiteration() = END;
									
					// Determine the next event to fire
					if		(this_ptr->Is_Minimum_Plan_Time(this_ptr->template Involved_Persons_Planning_Time<Revision&>())) this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Involved_Persons_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Duration_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Duration_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Location_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Location_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Mode_Planning_Time<Revision&>()))				this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Mode_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Route_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Route_Planning_Time<Revision&>(),true);
					else
					{
						response.next.iteration() = iteration()+5;
						response.next._subiteration() = Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
				}
				//------------------------------------------------------------------------------------------------------------------------------
				// ROUTE_PLANNING Iteration
				else if (this_ptr->Is_Current_Iteration(this_ptr->template Route_Planning_Time<Revision&>()))
				{
					// swap in durtion planning event, and make sure it won't be called again
					_pthis->Swap_Event((Event)&Activity_Planner::Route_Planning_Event<NULLTYPE>);
					this_ptr->template Route_Planning_Time<Revision&>().iteration() = END;
					this_ptr->template Route_Planning_Time<Revision&>()._subiteration() = END;
									
					// Determine the next event to fire
					if		(this_ptr->Is_Minimum_Plan_Time(this_ptr->template Involved_Persons_Planning_Time<Revision&>())) this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Involved_Persons_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Duration_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Duration_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Location_Planning_Time<Revision&>()))			this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Location_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Mode_Planning_Time<Revision&>()))				this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Mode_Planning_Time<Revision&>(),true);
					else if (this_ptr->Is_Minimum_Plan_Time(this_ptr->template Start_Time_Planning_Time<Revision&>()))		this_ptr->template Set_As_Next_Revison<NT>(response,this_ptr->template Start_Time_Planning_Time<Revision&>(),true);
					else
					{
						response.next.iteration() = iteration()+5;
						response.next._subiteration() = Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
				}
				else if (_subiteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION)
				{
					// add activity to schedule
					_pthis->Swap_Event((Event)&Activity_Planner::Add_Activity_To_Schedule_Event<NULLTYPE>);
					response.next.iteration() = END;
					response.next._subiteration() = END;
				}
				// catch any subiteration()s which have been modified (currenlty limited to routing, which can be changed by start_time_plan iteration)
				// this will hit only if start time plan iteration is called immidiately
				// also used to catch deleted activities and remove them from event scheduling
				else
				{
					Revision& route_itr = this_ptr->template Route_Planning_Time<Revision&>();
					if(route_itr.iteration()  < (int)END && route_itr.iteration() >= iteration()) 
					{
						response.next.iteration() = route_itr.iteration();
						response.next._subiteration() = route_itr._subiteration();
					}
					else
					{
						response.next.iteration() = END;
						response.next._subiteration() = END;
					}
					response.result = false;
				}

				CHECK_CONDITIONAL
			}
			template<typename TargetType> void Set_As_Next_Revison(Event_Response& response, Revision& revision, bool result)
			{
				response.result = result;
				response.next.iteration() = revision.iteration();
				response.next._subiteration() = revision._subiteration();
			}
			template<typename TargetType> void Unschedule_Activity_Events()
			{
				Revision& persons = this->Involved_Persons_Planning_Time<Revision&>();
				Revision& mode = this->Mode_Planning_Time<Revision&>();
				Revision& duration = this->Duration_Planning_Time<Revision&>();
				Revision& location = this->Location_Planning_Time<Revision&>();
				Revision& start_time = this->Start_Time_Planning_Time<Revision&>();
				Revision& route = this->Route_Planning_Time<Revision&>();
				persons.iteration() = END+1;
				persons._subiteration() = END+1;
				mode.iteration() = END+1;
				mode._subiteration() = END+1;
				duration.iteration() = END+1;
				duration._subiteration() = END+1;
				location.iteration() = END+1;
				location._subiteration() = END+1;
				start_time.iteration() = END+1;
				start_time._subiteration() = END+1;
				route.iteration() = END+1;
				route._subiteration() = END+1;
			}
			template<typename TargetType> bool Is_Minimum_Plan_Time(TargetType plan_time, requires(TargetType,check_2(TargetType, Revision,is_same)))
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
					rev._revision <= start_time._revision && rev._revision <= route._revision && rev.iteration() != END)
				{
					// set next revision
					return true;
				}
				return false;
			}
			template<typename TargetType> bool Is_Current_Iteration(TargetType plan_time, requires(TargetType,check_2(TargetType, Revision,is_same)))
			{
				return (plan_time.iteration() == iteration() && plan_time._subiteration() == _subiteration());
			}

			//===========================================
			// Activity Planning events
			//-------------------------------------------
			declare_event(Location_Planning_Event)
			{
				typedef Activity_Planner<ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->template Location_Planning_Event_Handler<NT>();
			}
			template<typename TargetType> void Location_Planning_Event_Handler()
			{
				this_component()->template Location_Planning_Event_Handler<TargetType>();
				this->Location_Planning_Time<Revision&>().iteration() = END+1;
				this->Location_Planning_Time<Revision&>()._subiteration() = END+1;
			}
			declare_event(Mode_Planning_Event)
			{
				typedef Activity_Planner<ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->template Mode_Planning_Event_Handler<NT>();
			}
			template<typename TargetType> void Mode_Planning_Event_Handler()
			{
				this_component()->template Mode_Planning_Event_Handler<TargetType>();
				this->Mode_Planning_Time<Revision&>().iteration() = END+1;
				this->Mode_Planning_Time<Revision&>()._subiteration() = END+1;
			}
			declare_event(Start_Time_Planning_Event)
			{
				typedef Activity_Planner<ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->template Start_Time_Planning_Event_Handler<NT>();
			}
			template<typename TargetType> void Start_Time_Planning_Event_Handler()
			{
				this_component()->template Start_Time_Planning_Event_Handler<TargetType>();
				this->Start_Time_Planning_Time<Revision&>().iteration() = END+1;
				this->Start_Time_Planning_Time<Revision&>()._subiteration() = END+1;
			}
			declare_event(Duration_Planning_Event)
			{
				typedef Activity_Planner<ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->template Duration_Planning_Event_Handler<NT>();
			}
			template<typename TargetType> void Duration_Planning_Event_Handler()
			{
				this_component()->template Duration_Planning_Event_Handler<TargetType>();
				// set duration planning time schedule to END - duration Is Planned
				this->Duration_Planning_Time<Revision&>().iteration() = END;
				this->Duration_Planning_Time<Revision&>()._subiteration() = END;
			}
			declare_event(Involved_Persons_Planning_Event)
			{
				typedef Activity_Planner<ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->template Involved_Persons_Planning_Event_Handler<NT>();
			}
			template<typename TargetType> void Involved_Persons_Planning_Event_Handler()
			{
				this_component()->template Involved_Persons_Planning_Event_Handler<TargetType>();
				this->Involved_Persons_Planning_Time<Revision&>().iteration() = END+1;
				this->Involved_Persons_Planning_Time<Revision&>()._subiteration() = END+1;
			}
			declare_event(Route_Planning_Event)
			{
				typedef Activity_Planner<ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->template Route_Planning_Event_Handler<NT>();
			}
			template<typename TargetType> void Route_Planning_Event_Handler()
			{
				this_component()->template Route_Planning_Event_Handler<TargetType>();
				this->Route_Planning_Time<Revision&>().iteration() = END+1;
				this->Route_Planning_Time<Revision&>()._subiteration() = END+1;
			}
			declare_event(Add_Activity_To_Schedule_Event)
			{
				typedef Activity_Planner<ComponentType> _Activity_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Activity_Interface* this_ptr=(_Activity_Interface*)_pthis;
				this_ptr->template Add_Activity_To_Schedule_Event_Handler<NT>();
			}
			template<typename TargetType> void Add_Activity_To_Schedule_Event_Handler()
			{
				this_component()->template Add_Activity_To_Schedule_Event_Handler<TargetType>();
			}

			//===========================================
			// Fundamental activity 'meta-attributes'
			//-------------------------------------------
			accessor(Activity_Plan_ID, NONE, NONE);
			accessor(Activity_Plan_Horizon, NONE, NONE);
			accessor(Activity_Planning_Time, check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value), check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value));
			accessor(Location_Planning_Time, check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			accessor(Mode_Planning_Time, check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			accessor(Start_Time_Planning_Time, check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			accessor(Duration_Planning_Time, check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			accessor(Involved_Persons_Planning_Time, check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			accessor(Route_Planning_Time, NONE, NONE/*check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same)*/);
			accessor(Location_Plan_Horizon, NONE, NONE);
			accessor(Location_Flexibility, NONE, NONE);
			accessor(Mode_Plan_Horizon, NONE, NONE);
			accessor(Mode_Flexibility, NONE, NONE);
			accessor(Start_Time_Plan_Horizon, NONE, NONE);
			accessor(Start_Time_Flexibility, NONE, NONE);
			accessor(Duration_Plan_Horizon, NONE, NONE);
			accessor(Duration_Flexibility, NONE, NONE);
			accessor(Involved_Persons_Plan_Horizon, NONE, NONE);
			accessor(Involved_Persons_Flexibility, NONE, NONE);

			accessor(Stored_Location_Planning_Time, check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			accessor(Stored_Mode_Planning_Time, check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			accessor(Stored_Start_Time_Planning_Time, check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			accessor(Stored_Duration_Planning_Time, check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			accessor(Stored_Involved_Persons_Planning_Time, check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			accessor(Stored_Route_Planning_Time, NONE, NONE/*check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same)*/);

			//===========================================
			// Primary Activity attribute accessors
			//-------------------------------------------
			accessor(Activity_Type, NONE, NONE);
			template<typename TargetType> string Get_Type_String()
			{
				return this_component()->template Get_Type_String<TargetType>();
			}
			accessor(Location, check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location), check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location));
			accessor(Mode, NONE, NONE);
			accessor(Start_Time, check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value), check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value));
			//Only used for return home activities as the departure time for these acts is always the end of the preceding activity
			accessor(Fixed_Departure, check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value), check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value)); 
			//accessor(End_Time, check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value), check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value)); 
			template<typename TargetType> TargetType End_Time(requires(TargetType,check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value)))
			{
				return this_component()->template End_Time<TargetType>();
			}
			template<typename TargetType> void End_Time(TargetType value, bool maintain_duration, requires(TargetType,check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value)))
			{
				this_component()->template End_Time<TargetType>(value,maintain_duration);
			}
			accessor(Duration, check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value), check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value)); 
			accessor(Involved_Persons_Container, NONE, NONE);
			accessor(Expected_Travel_Time, check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value), check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value)); 
			accessor(Actual_Travel_Time, check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value), check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value)); 

			template<typename TargetType> TargetType Minimum_Duration(requires(TargetType,check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value)))
			{
//TODO
//				TargetType min_seconds = GLOBALS::Time_Converter.Convert_Value<Target_Type<NT,TargetType,Time_Seconds>>(330.0);
				TargetType min_duration = this->Duration<TargetType>()*0.5;
				
				float val = max<float>(min_seconds,min_duration);
				return (TargetType)val;
				
			}

			//===========================================
			// Activity Planner Methods
			//-------------------------------------------
			template<typename TargetType> void Initialize(TargetType activity, requires(TargetType,check(strip_modifiers(TargetType),Concepts::Is_Activity_Plan_Prototype)))
			{
				this_component()->template Initialize<TargetType>(activity);
			}
			template<typename TargetType> void Initialize(typename TargetType::ParamType act_type, typename TargetType::Param2Type planning_time, requires(TargetType,check(strip_modifiers(TargetType),Is_Target_Type_Struct) && check_2(typename TargetType::ParamType, Types::ACTIVITY_TYPES, is_same)))
			{
				this_component()->template Initialize< typename TargetType::ParamType>(act_type);
				this->Set_Meta_Attributes<void>();
				this->Set_Attribute_Planning_Times<typename TargetType::Param2Type>(planning_time);
			}
			template<typename TargetType> void Initialize(typename TargetType::ParamType act_type, typename TargetType::Param2Type planning_time, requires(TargetType,!check(strip_modifiers(TargetType),Is_Target_Type_Struct) || !check_2(typename TargetType::ParamType, Types::ACTIVITY_TYPES, is_same)))
			{
				assert_check(strip_modifiers(TargetType),Is_Target_Type_Struct, "The TargetType for Activity.Initialize must be a Target_Type struct.");
				assert_check_2(typename TargetType::ParamType, Types::ACTIVITY_TYPES, is_same, "TargetType::ParamType must be an ActivityType enumerator");
			}
			template<typename TargetType> void Initialize(typename TargetType::ParamType start_time, typename TargetType::ParamType duration, typename TargetType::Param2Type mode)
			{
				this_component()->template Initialize<TargetType>(start_time,duration,mode);
				this->Set_Meta_Attributes<void>();
				this->Set_Attribute_Planning_Times<typename TargetType::ParamType>(iteration());
			}
			template<typename TargetType> void Initialize(typename TargetType::ParamType departure_time, typename TargetType::ParamType start_time, typename TargetType::ParamType duration, typename TargetType::Param2Type mode)
			{
				this_component()->template Initialize<TargetType>(departure_time,start_time,duration,mode);
				this->Set_Meta_Attributes<void>();
				this->Set_Attribute_Planning_Times<typename TargetType::ParamType>(iteration());
			}
			template<typename TargetType> void Set_Attribute_Planning_Times(TargetType planning_time)
			{
				// Call the model to determine the attribute planning times
				this_component()->template Set_Attribute_Planning_Times<TargetType>(planning_time);

				// store for later use
				Revision& persons = this->Involved_Persons_Planning_Time<Revision&>();
				Revision& mode = this->Mode_Planning_Time<Revision&>();
				Revision& duration = this->Duration_Planning_Time<Revision&>();
				Revision& location = this->Location_Planning_Time<Revision&>();
				Revision& start_time = this->Start_Time_Planning_Time<Revision&>();
				Revision& route = this->Route_Planning_Time<Revision&>();

				// Load the first event
				if (Is_Minimum_Plan_Time(persons)) //TODO
//load_event(ComponentType,Activity_Planning_Conditional, Involved_Persons_Planning_Event, persons.iteration(), persons._subiteration(),NT);
				else if (Is_Minimum_Plan_Time(mode)) //TODO
//load_event(ComponentType,Activity_Planning_Conditional, Mode_Planning_Event, mode.iteration(), mode._subiteration(),NT);
				else if (Is_Minimum_Plan_Time(duration)) //TODO
//load_event(ComponentType,Activity_Planning_Conditional, Duration_Planning_Event, duration.iteration(), duration._subiteration(),NT);
				else if (Is_Minimum_Plan_Time(location)) //TODO
//load_event(ComponentType,Activity_Planning_Conditional, Location_Planning_Event, location.iteration(), location._subiteration(),NT);
				else if (Is_Minimum_Plan_Time(start_time)) //TODO
//load_event(ComponentType,Activity_Planning_Conditional, Start_Time_Planning_Event, start_time.iteration(), start_time._subiteration(),NT);
				else if (Is_Minimum_Plan_Time(route)) //TODO
//load_event(ComponentType,Activity_Planning_Conditional, Route_Planning_Event, route.iteration(), route._subiteration(),NT);
				else 
				{
					stringstream err;
					err << "ERROR: Invalid planning times set: ";
					err << endl << "Duration: "<<duration.iteration() <<","<<duration._subiteration();
					err << endl << "Start: "<<start_time.iteration() <<","<<start_time._subiteration();
					err << endl << "Location: "<<location.iteration() <<","<<location._subiteration(); 
					err << endl << "Route: "<<route.iteration() <<","<<route._subiteration(); 
					THROW_EXCEPTION(err);
				}
			}
			template<typename TargetType>_method_void(Set_Meta_Attributes, NONE);	
			template<typename TargetType> void Update_Movement_Plan(TargetType origin, TargetType destination, Simulation_Timestep_Increment min_departure, requires(TargetType,check(strip_modifiers(TargetType),is_pointer) && check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location_Prototype)))
			{
				this_component()->Update_Movement_Plan<TargetType>(origin,destination,min_departure);
			}

			template<typename TargetType> void Arrive_At_Activity()
			{
				this_component()->template Arrive_At_Activity<TargetType>();
			}

			// features to check if activity attributes have been planned - calculated based on reviion times for attribute planes (true if set to END)
			template<typename TargetType>_method_void(Location_Is_Planned,check_2(TargetType,bool,is_same));
			template<typename TargetType>_method_void(Mode_Is_Planned,check_2(TargetType,bool,is_same));
			template<typename TargetType>_method_void(Start_Is_Planned,check_2(TargetType,bool,is_same));
			template<typename TargetType>_method_void(Duration_Is_Planned,check_2(TargetType,bool,is_same));
			template<typename TargetType>_method_void(Involved_Persons_Is_Planned,check_2(TargetType,bool,is_same));
			template<typename TargetType>_method_void(Route_Is_Planned,check_2(TargetType,bool,is_same));
		};
	}
}

using namespace Activity_Components::Prototypes;
using namespace Activity_Components::Types;

#pragma once

#include "Vehicle_Prototype.h"
#include "Person_Prototype.h"
#include "Household_Prototype.h"
#include "Person_Planner_Prototype.h"
#include "Activity_Generator_Prototype.h"
#include "Person_Properties_Prototype.h"
#include "activity_simulator/Person_Scheduler_Implementations.h"
#include "Activity_Prototype.h"
#include "Activity_Simulator_Forward_Declaration.h"

namespace Activity_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		//=======================================================================
		// Activity Classes
		//-----------------------------------------------------------------------
		implementation struct Basic_Activity_Plan_Implementation : public Polaris_Component<MasterType,INHERIT(Basic_Activity_Plan_Implementation),Execution_Object>
		{
			// Tag As Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Basic_Activity_Plan_Implementation),Execution_Object>::Component_Type ComponentType;

			m_data(bool, is_valid, NONE,NONE);

			//=================================================================
			// Pointer back to planner
			m_prototype(Person_Components::Prototypes::Person_Planner, typename MasterType::person_planner_type, Parent_Planner, NONE, NONE);

			// pointer to movement plan associated with activity
			m_prototype(Movement_Plan_Components::Prototypes::Movement_Plan, typename MasterType::movement_plan_type, movement_plan, NONE, NONE);

			m_static_data(int, Route_Planning_Count, NONE, NONE);
			//m_static_data(_lock, update_lock, NONE, NONE);

			//================================================================================================================================================================================================
			//================================================================================================================================================================================================
			// Interfaces
			typedef Prototypes::Activity_Planner<ComponentType> this_itf;
			typedef Person_Components::Prototypes::Person_Planner<Parent_Planner_component_type> _planning_itf;
			typedef Person_Components::Prototypes::Person<typename _planning_itf::get_type_of(Parent_Person)> _person_itf;
			typedef Person_Components::Prototypes::Person_Perception<typename _person_itf::get_type_of(Perception_Faculty)> _perception_itf;
			typedef Household_Components::Prototypes::Household< typename _person_itf::get_type_of(Household)> _household_itf;
			typedef Person_Components::Prototypes::Person_Scheduler< typename _person_itf::get_type_of(Scheduling_Faculty)> _scheduler_itf;
			//typedef Person_Components::Prototypes::Person_Scheduler< typename MasterType::person_scheduler_type> _scheduler_itf;
			typedef Person_Components::Prototypes::Activity_Generator< typename MasterType::activity_generator_type> _generator_itf;
			typedef Person_Components::Prototypes::Person_Properties< typename _person_itf::get_type_of(Properties)> _properties_itf;
			typedef Person_Components::Prototypes::Person_Properties< typename _person_itf::get_type_of(Static_Properties)> _static_properties_itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename _person_itf::get_type_of(vehicle)> _vehicle_Itf;
			
			typedef Person_Components::Prototypes::Destination_Chooser<typename _planning_itf::get_type_of(Destination_Choice_Faculty)> _dest_choice_itf;
			typedef Scenario_Components::Prototypes::Scenario< typename _perception_itf::get_type_of(Scenario)> _scenario_itf;
			typedef Network_Components::Prototypes::Network< typename _perception_itf::get_type_of(Network)> _network_itf;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _network_itf::get_type_of(skimming_faculty)> _skim_itf;
			typedef Prototypes::Activity_Planner<typename MasterType::at_home_activity_plan_type> _home_activity_itf;

			typedef Random_Access_Sequence< typename _network_itf::get_type_of(activity_locations_container)> _activity_locations_container_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(_activity_locations_container_itf)>  _activity_location_itf;
			
			typedef Random_Access_Sequence< typename _activity_location_itf::get_type_of(origin_links)> _links_container_itf;
			typedef Link_Components::Prototypes::Link<get_component_type(_links_container_itf)>  _link_itf;
			
			typedef Random_Access_Sequence< typename _link_itf::get_type_of(outbound_turn_movements)> _turns_container_itf;
			typedef Turn_Movement_Components::Prototypes::Movement<get_component_type(_turns_container_itf)>  _turn_itf;
	
			typedef Pair_Associative_Container< typename _network_itf::get_type_of(zones_container)> _zones_container_itf;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_zones_container_itf)>  _zone_itf;
		
			//typedef typename _scheduler_itf::Component_Type ctype;
			//RLW%%%	typedef Back_Insertion_Sequence< typename _scheduler_itf::get_type_of(Activity_Container)> _activity_plans_container_itf;
			typedef Prototypes::Activity_Planner<ComponentType> _activity_plan_itf;

			typedef Back_Insertion_Sequence< typename _scheduler_itf::get_type_of(Movement_Plans_Container)> _movement_plans_container_itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename MasterType::movement_plan_type> _movement_plan_itf;

			
			//================================================================================================================================================================================================
			//================================================================================================================================================================================================

		
			// Fundamental activity properties
			m_data(int, Activity_Plan_ID, NONE, NONE);
			m_data(Types::ACTIVITY_TYPES, Activity_Type, NONE, NONE);
			template<typename TargetType> TargetType Parent_ID()
			{
				return this->_Parent_Planner->template Parent_Person<_person_itf*>()->template uuid<TargetType>();
			}	tag_getter_as_available(Parent_ID);
			
			m_data(Types::FLEXIBILITY_VALUES, Location_Flexibility, NONE, NONE);
			m_data(Types::FLEXIBILITY_VALUES, Mode_Flexibility, NONE, NONE);
			m_data(Types::FLEXIBILITY_VALUES, Start_Time_Flexibility, NONE, NONE);
			m_data(Types::FLEXIBILITY_VALUES, Duration_Flexibility, NONE, NONE);
			m_data(Types::FLEXIBILITY_VALUES, Involved_Persons_Flexibility, NONE, NONE);

			// Activity attributes
			m_prototype(Activity_Location_Components::Prototypes::Activity_Location, typename MasterType::activity_location_type, Location, check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location), check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location));
			m_data(Vehicle_Components::Types::Vehicle_Type_Keys, Mode, NONE, NONE);
			member_component_and_feature_accessor(_Start_Time, Value, Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>)
			member_component_and_feature_accessor(Duration, Value, Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>)
			member_component_and_feature_accessor(Expected_Travel_Time, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(Actual_Travel_Time, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);
			m_container(std::vector<Person_Components::Prototypes::Person<typename MasterType::person_type>*>, Involved_Persons_Container, NONE, NONE);
			
			// Error checking features for the _Start_Time member feature
			template<typename TargetType> TargetType Start_Time();
			template<typename TargetType> void Start_Time(TargetType value);
			// End time feature
			template<typename TargetType> TargetType End_Time();
			template<typename TargetType> void End_Time(TargetType value, bool maintain_duration);

			// departure time feature - set by route_handler, =0 before the route is planned
			template<typename TargetType, requires(TargetType, check(strip_modifiers(TargetType), Basic_Units::Concepts::Is_Time_Value))> 
			TargetType Departure_Time();
			
			// Planning event time members
			m_data(Revision,Location_Planning_Time,check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			m_data(Revision,Mode_Planning_Time,check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			m_data(Revision,Start_Time_Planning_Time,check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			m_data(Revision,Duration_Planning_Time,check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			m_data(Revision,Involved_Persons_Planning_Time,check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			m_data(Revision,Route_Planning_Time,check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			m_data(Revision,Deletion_Time,check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));

			bool Location_Is_Planned(){return (_Location_Planning_Time._iteration >= (int)END && _Location_Planning_Time._sub_iteration >= (int)END);}
			bool Mode_Is_Planned(){return (_Mode_Planning_Time._iteration >= (int)END && _Mode_Planning_Time._sub_iteration >= (int)END);}
			bool Duration_Is_Planned(){return (_Duration_Planning_Time._iteration >= (int)END && _Duration_Planning_Time._sub_iteration >= (int)END);}
			bool Involved_Persons_Is_Planned(){return (_Involved_Persons_Planning_Time._iteration >= (int)END && _Involved_Persons_Planning_Time._sub_iteration >= (int)END);}
			bool Start_Is_Planned(){return (_Start_Time_Planning_Time._iteration >= (int)END && _Start_Time_Planning_Time._sub_iteration >= (int)END);}
			bool Route_Is_Planned(){return (_Route_Planning_Time._iteration >=(int)END && _Route_Planning_Time._sub_iteration >= (int)END);}

			template<typename TargetType> void Copy(TargetType activity);

			template<typename TargetType> void Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType, check_2(TargetType, Simulation_Timestep_Increment, is_same)));
			
			template<typename TargetType> void Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType, !check_2(TargetType, Simulation_Timestep_Increment, is_same)));

			template<typename TargetType> void Route_Planning_Event_Handler();
			template<typename TargetType> void Add_Activity_To_Schedule_Event_Handler();
			template<typename TargetType> void Deletion_Event_Handler();

			template<typename TargetType> void Update_Movement_Plan(TargetType origin, TargetType destination, Simulation_Timestep_Increment min_departure);
			void Arrive_At_Activity();
			void Depart_From_Activity();
			string Get_Type_String();
			void Display_Activity();
		};

		// Error checking features for the _Start_Time member feature
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		TargetType Basic_Activity_Plan_Implementation<MasterType, InheritanceList>::Start_Time()
		{
			return this->_Start_Time<TargetType>();
		}
		
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Basic_Activity_Plan_Implementation<MasterType, InheritanceList>::Start_Time(TargetType value)
		{
			if (value > (END)*2.0)
			{
				THROW_WARNING("Error, invalid value when setting start-time: " << value);
			}
			else
			{
				this->template _Start_Time<TargetType>(value);
			}
		}
		
		// End time feature
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		TargetType Basic_Activity_Plan_Implementation<MasterType, InheritanceList>::End_Time()
		{
			return this->template Start_Time<TargetType>() + this->template Duration<TargetType>();
		}
		
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Basic_Activity_Plan_Implementation<MasterType, InheritanceList>::End_Time(TargetType value, bool maintain_duration)
		{
			if (value > (END)*2.0)
			{
				THROW_WARNING("Error, invalid value when setting end-time: " << value);
			}

			if (maintain_duration) this->template Start_Time<TargetType>(value - this->template Duration<TargetType>());
			else
			{
				if (value < this->template Start_Time<TargetType>())
				{
					//TODO: CHANGE back to throw exception after testing
					//THROW_WARNING("WARNING: new end time less than start time is not possible if start time is fixed (maintain_duration=false)");
				}
				else this->template Duration<TargetType>(value - this->template Start_Time<TargetType>());
			}
		}
		
		// departure time feature - set by route_handler, =0 before the route is planned
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType, requires(TargetType, check(strip_modifiers(TargetType), Basic_Units::Concepts::Is_Time_Value))>
		TargetType Basic_Activity_Plan_Implementation<MasterType, InheritanceList>::Departure_Time()
		{
			if (this->Route_Is_Planned<NT>())
			{
				_movement_plan_itf* move = this->movement_plan<_movement_plan_itf*>();
				return move->template departed_time<TargetType>(depart);
			}
			else
			{
				return this->template Start_Time<TargetType>();
			}
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Basic_Activity_Plan_Implementation<MasterType, InheritanceList>::Copy(TargetType activity)
		{
			this_itf* pthis = static_cast<this_itf*>(this);

			pthis->is_valid(activity->is_valid<bool>());
			pthis->Parent_Planner(activity->Parent_Planner<Parent_Planner_type>());

			// allocate movement plan
			pthis->Activity_Plan_ID(activity->Activity_Plan_ID<int>());
			pthis->Activity_Type(activity->Activity_Type<Types::ACTIVITY_TYPES>());

			pthis->Location_Flexibility(activity->Location_Flexibility<Types::FLEXIBILITY_VALUES>());
			pthis->Mode_Flexibility(activity->Mode_Flexibility<Types::FLEXIBILITY_VALUES>());
			pthis->Start_Time_Flexibility(activity->Start_Time_Flexibility<Types::FLEXIBILITY_VALUES>());
			pthis->Duration_Flexibility(activity->Duration_Flexibility<Types::FLEXIBILITY_VALUES>());
			pthis->Involved_Persons_Flexibility(activity->Involved_Persons_Flexibility<Types::FLEXIBILITY_VALUES>());

			pthis->Location<_activity_location_itf*>(activity->template Location<_activity_location_itf*>());
			pthis->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(activity->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>());
			pthis->Start_Time(activity->template Start_Time<Time_Seconds>());
			pthis->Duration(activity->template Duration<Time_Seconds>());

			for (auto itr = activity->template Involved_Persons_Container<Involved_Persons_Container_type&>().begin(); itr != activity->template Involved_Persons_Container<Involved_Persons_Container_type&>().end(); ++itr)
			{
				_Involved_Persons_Container.push_back(*itr);
			}

			pthis->Location_Planning_Time(activity->template Location_Planning_Time<Revision>());
			pthis->Mode_Planning_Time(activity->template Mode_Planning_Time<Revision>());
			pthis->Start_Time_Planning_Time(activity->template Start_Time_Planning_Time<Revision>());
			pthis->Duration_Planning_Time(activity->template Duration_Planning_Time<Revision>());
			pthis->Involved_Persons_Planning_Time(activity->template Involved_Persons_Planning_Time<Revision>());
			pthis->Route_Planning_Time(activity->template Route_Planning_Time<Revision>());
			pthis->Deletion_Time(activity->template Deletion_Time<Revision>());
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Basic_Activity_Plan_Implementation<MasterType, InheritanceList>::Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType, check_2(TargetType, Simulation_Timestep_Increment, is_same)))
		{
			Revision &start = this->Start_Time_Planning_Time<  Revision&>();
			Revision &dur = this->Duration_Planning_Time<  Revision&>();
			Revision &loc = this->Location_Planning_Time<  Revision&>();
			Revision & mode = this->Mode_Planning_Time<  Revision&>();
			Revision & persons = this->Involved_Persons_Planning_Time<  Revision&>();
			Revision &route = this->Route_Planning_Time<  Revision&>();

			start._iteration = END;
			dur._iteration = END;
			loc._iteration = END;
			mode._iteration = END;
			persons._iteration = END;
			route._iteration = min(_iteration + 1, (int)planning_time);

			start._sub_iteration = 0;
			dur._sub_iteration = 0;
			loc._sub_iteration = 0;
			mode._sub_iteration = 0;
			persons._sub_iteration = 0;
			route._sub_iteration = 5;
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Basic_Activity_Plan_Implementation<MasterType, InheritanceList>::Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType, !check_2(TargetType, Simulation_Timestep_Increment, is_same)))
		{
			assert_check_2(TargetType, Simulation_Timestep_Increment, is_same, "Error: planning_time must be passed as a Simulation_Timestep_Increment type when using this function.");
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Basic_Activity_Plan_Implementation<MasterType, InheritanceList>::Route_Planning_Event_Handler()
		{
			// General interfaces, to parent and global classes
			_planning_itf* planner = this->Parent_Planner<_planning_itf*>();
			_person_itf* person = planner->template Parent_Person<_person_itf*>();
			_household_itf* household = person->_person_itf::template Household<_household_itf*>();
			_network_itf* network = person->template network_reference<_network_itf*>();
			_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
			_scenario_itf* scenario = static_cast<_scenario_itf*>(_global_scenario);


			// Create movement plan and give it an ID
			_movement_plan_itf* move = static_cast<_movement_plan_itf*>(Allocate<get_component_type(_movement_plans_container_itf)>());
			move->template initialize_trajectory<NULLTYPE>();
			move->template destination_activity_reference<ComponentType*>(static_cast<ComponentType*>(this));
			move->template network<_network_itf*>(network);
			move->traveler_id(person->template uuid<int>());

			// Get the origin and destination locations
			_activity_location_itf* orig;
			_activity_location_itf* dest = this->Location<_activity_location_itf*>();
			if (dest == nullptr) dest = person->template Home_Location<_activity_location_itf*>();

			Time_Seconds start = this->Start_Time<Time_Seconds>();
			this_itf* prev_act = scheduler->template previous_activity_plan<Time_Seconds, this_itf*>(this->Start_Time<Time_Seconds>());
			if (prev_act == nullptr)  orig = person->template Home_Location<_activity_location_itf*>();
			else
			{
				orig = prev_act->template Location<_activity_location_itf*>();
				if (orig == nullptr) orig = person->template Home_Location<_activity_location_itf*>();

				// check if a stop at home will fit prior to activity
				Time_Seconds time_before = start - (prev_act->template Start_Time<Time_Seconds>() + prev_act->template Duration<Time_Seconds>());
				Time_Seconds ttime_prev_to_home = network->template Get_TTime<_activity_location_itf*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Seconds, Time_Seconds>(orig, person->template Home_Location<_activity_location_itf*>(), Vehicle_Components::Types::Vehicle_Type_Keys::SOV, start);
				Time_Seconds ttime_home_to_this = network->template Get_TTime<_activity_location_itf*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Seconds, Time_Seconds>(person->template Home_Location<_activity_location_itf*>(), dest, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, start);
				Time_Seconds ttime_prev_to_this = network->template Get_TTime<_activity_location_itf*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Seconds, Time_Seconds>(orig, dest, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, start);
				// enough time between previous activity and this activity to go home, stay there for a minimimum amount of time (equal to the shortest leg of the return home trip) and get to this activity
				float min_home_time = min((float)ttime_prev_to_home, (float)ttime_home_to_this);
				// don't reset origin, however, if this trip is a return-to-home trip
				if (ttime_prev_to_home + ttime_home_to_this < time_before - min_home_time && dest != person->template Home_Location<_activity_location_itf*>())
					orig = person->template Home_Location<_activity_location_itf*>();
			}

			// set the reference to the movement plan
			this->movement_plan<_movement_plan_itf*>(move);

			if (orig == nullptr || dest == nullptr)
			{
				THROW_WARNING("Null origin or destination values specified");
				return;
			}

			// update the movement plan with the specified origin/destination
			this->Update_Movement_Plan<_activity_location_itf*>(orig, dest, iteration());

			// exit if no trip is needed
			if (orig->template internal_id<int>() == dest->template internal_id<int>())
			{
				return;
			}

			// route if planning router specified
			if (scenario->template do_planner_routing<bool>())
			{
				planner->template Schedule_New_Routing<_movement_plan_itf*>(iteration() + 1, move);
			}

		}
		
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Basic_Activity_Plan_Implementation<MasterType, InheritanceList>::Add_Activity_To_Schedule_Event_Handler()
		{
			// General interfaces
			_person_itf* person = this->_Parent_Planner->template Parent_Person<_person_itf*>();
			_static_properties_itf* properties = person->template Static_Properties<_static_properties_itf*>();
			_household_itf* household = person->_person_itf::template Household<_household_itf*>();
			_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
			_movement_plan_itf* move = this->movement_plan<_movement_plan_itf*>();
			_scenario_itf* scenario = static_cast<_scenario_itf*>(_global_scenario);


			if (!this->Route_Is_Planned())
			{
				THROW_EXCEPTION("ERROR: attempting to add activity to schedule before the route handler is called.");
			}


			// exit if movement plan origin/destination not set
			if (move->template origin<_activity_location_itf*>() == nullptr || move->template destination<_activity_location_itf*>() == nullptr) return;


			// If this is a child needing an escort
			if (properties->template Age<int>()<6 && this->Activity_Type<Types::ACTIVITY_TYPES>() != Types::AT_HOME_ACTIVITY)
			{
				// Require parent escort for preschool children
				// get a free adult available for escort
				_person_itf* adult = household->template Get_Free_Escort<_person_itf*, Time_Seconds>(this->Start_Time<Time_Seconds>(), this->End_Time<Time_Seconds>());

				// If no adults free and not school trip - cancel act
				if (adult == nullptr && this->Activity_Type<Types::ACTIVITY_TYPES>() != Types::SCHOOL_ACTIVITY) return;
				// if no adult but is school activity, force trip to transit
				else if (adult == nullptr && this->Activity_Type<Types::ACTIVITY_TYPES>() == Types::SCHOOL_ACTIVITY) this->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::SCHOOLBUS);
				// otherwise, assign escort duty to adult
				else
				{
					_generator_itf* gen = adult->template Planning_Faculty<_planning_itf*>()->template Activity_Generation_Faculty<_generator_itf*>();
					gen->template Create_Activity<Types::ACTIVITY_TYPES, _activity_location_itf*, /*Vehicle_Components::Types::Vehicle_Type_Keys,*/ Time_Seconds>(Types::PICK_UP_OR_DROP_OFF_ACTIVITY, iteration() + 2, this->Location<_activity_location_itf*>(), /*Vehicle_Components::Types::Vehicle_Type_Keys::SOV,*/ this->Start_Time<Time_Seconds>(), 300);
				}
			}
			// require escort for all modes except bus (i.e. kids can take schoolbus by themselves)
			else if (properties->template Age<int>()<10 && this->Activity_Type<Types::ACTIVITY_TYPES>() != Types::AT_HOME_ACTIVITY)
			{
				// Attempt parent escort for all modes but transit - if fails force to transit
				if (this->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() != Vehicle_Components::Types::Vehicle_Type_Keys::SCHOOLBUS)
				{
					// get a free adult available for escort
					//%%%RLW
					_person_itf* adult = household->template Get_Free_Escort<_person_itf*, Time_Seconds>(this->Start_Time<Time_Seconds>(), this->End_Time<Time_Seconds>());

					// If no adults free and not school trip - cancel act
					if (adult == nullptr && this->Activity_Type<Types::ACTIVITY_TYPES>() != Types::SCHOOL_ACTIVITY) return;
					// if no adult but is school activity, force trip to transit
					else if (adult == nullptr && this->Activity_Type<Types::ACTIVITY_TYPES>() == Types::SCHOOL_ACTIVITY) this->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::SCHOOLBUS);
					// otherwise, assign escort duty to adult
					else
					{
						_generator_itf* gen = adult->template Planning_Faculty<_planning_itf*>()->template Activity_Generation_Faculty<_generator_itf*>();
						gen->template Create_Activity<Types::ACTIVITY_TYPES, _activity_location_itf*, /*Vehicle_Components::Types::Vehicle_Type_Keys,*/ Time_Seconds>(Types::PICK_UP_OR_DROP_OFF_ACTIVITY, iteration() + 2, this->Location<_activity_location_itf*>(), /*Vehicle_Components::Types::Vehicle_Type_Keys::SOV,*/ this->Start_Time<Time_Seconds>(), 300);
					}
				}
			}
			else if (properties->template Age<int>()<16 && this->Activity_Type<Types::ACTIVITY_TYPES>() != Types::AT_HOME_ACTIVITY)
			{
				// Attempt parent escort when using HOV - if fails force to transit
				if (this->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::HOV)
				{
					// get a free adult available for escort
					_person_itf* adult = household->template Get_Free_Escort<_person_itf*, Time_Seconds>(this->Start_Time<Time_Seconds>(), this->End_Time<Time_Seconds>());

					// If no adults free, force trip to transit
					if (adult == nullptr)
					{
						//TODO OMER: Change back to bus when done
						this->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::TRUCK);

						//TODO OMER: Delete when done
						char myLine[2000];
						std::string bus_mode_paragraph;
						stringstream bus_mode_stream;

						Meters walkThreshold = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::walkThreshold<float>();
						Meters walkThreshold_init = walkThreshold / 5.0;

						sprintf_s(myLine, "%s\t%f\t%f\t%d\t%d\t%f\t%s\n",
							"Child_Forced",
							walkThreshold_init,
							-1.0,
							move->template origin<_activity_location_itf*>(),
							move->template destination<_activity_location_itf*>(),
							this->template Start_Time<Time_Minutes>(),
							"TRUCK");
						bus_mode_paragraph.insert(0, myLine);
						bus_mode_stream << bus_mode_paragraph;
						fw_bus_mode.Write_NoDelim(bus_mode_stream);

					}
					else
					{
						// if adult free, add new escort activity to the adult schedule
						_generator_itf* gen = adult->template Planning_Faculty<_planning_itf*>()->template Activity_Generation_Faculty<_generator_itf*>();
						gen->template Create_Activity<Types::ACTIVITY_TYPES, _activity_location_itf*, /*Vehicle_Components::Types::Vehicle_Type_Keys,*/ Time_Seconds>(Types::PICK_UP_OR_DROP_OFF_ACTIVITY, iteration() + 2, this->Location<_activity_location_itf*>(), /*Vehicle_Components::Types::Vehicle_Type_Keys::SOV,*/ this->Start_Time<Time_Seconds>(), 300);
					}
				}
			}


			if (scenario->template do_planner_routing<bool>())
			{
				// Assign the movement plan to the persons activity schedule, if null movement, leave valid_trajectory to false
				if (move->template valid_trajectory<bool>() || move->template origin<_activity_location_itf*>() == move->template destination<_activity_location_itf*>())
				{
					Simulation_Timestep_Increment ttime = move->template routed_travel_time<Simulation_Timestep_Increment>();
					Simulation_Timestep_Increment depart = this->Start_Time<Simulation_Timestep_Increment>() - ttime;
					if (depart < _Parent_Planner->template Next_Planning_Time<Simulation_Timestep_Increment>())
					{
						depart = _Parent_Planner->template Next_Planning_Time<Simulation_Timestep_Increment>();
						this->Start_Time<Simulation_Timestep_Increment>(depart + ttime);
					}

					this->Expected_Travel_Time<Simulation_Timestep_Increment>(move->template routed_travel_time<Simulation_Timestep_Increment>());
					move->template departed_time<Simulation_Timestep_Increment>(depart);
					scheduler->template Add_Movement_Plan<_movement_plan_itf*>(move);

				}
				else THROW_WARNING("WARNING: ACTIVITY NOT SCHEDULED, no valid route found from origin to destination. (PERID,ACTID,O,D) " << person->template uuid<int>() << "," << concat(this->Activity_Plan_ID<int>()) << "," << concat(move->template origin<_activity_location_itf*>()->template uuid<int>()) << ", " << concat(move->template destination<_activity_location_itf*>()->template uuid<int>()) << ",at iteration " << iteration() << "." << sub_iteration() << ". Scheduled for route planning @ " << move->template planning_time<Simulation_Timestep_Increment>() << ", and departure @ " << move->template departed_time<Simulation_Timestep_Increment>());
			}
			else
			{
				Simulation_Timestep_Increment ttime = this->Expected_Travel_Time<Simulation_Timestep_Increment>();
				Simulation_Timestep_Increment depart = move->template departed_time<Simulation_Timestep_Increment>();
				if (depart < _Parent_Planner->template Next_Planning_Time<Simulation_Timestep_Increment>() && ttime > 0)
				{
					depart = _Parent_Planner->template Next_Planning_Time<Simulation_Timestep_Increment>();
					move->template departed_time<Simulation_Timestep_Increment>(depart);
					this->Start_Time<Simulation_Timestep_Increment>(depart + ttime);
				}
				scheduler->template Add_Movement_Plan<_movement_plan_itf*>(move);
			}
		}
		
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Basic_Activity_Plan_Implementation<MasterType, InheritanceList>::Deletion_Event_Handler()
		{
			// Free movement through interface
			_movement_plan_itf* move = this->movement_plan<_movement_plan_itf*>();
			move->Free_Movement();
				this->movement_plan<_movement_plan_itf*>(nullptr);
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Basic_Activity_Plan_Implementation<MasterType, InheritanceList>::Update_Movement_Plan(TargetType origin, TargetType destination, Simulation_Timestep_Increment min_departure)
		{
			_movement_plan_itf* move = this->movement_plan<_movement_plan_itf*>();
			_activity_location_itf* orig = static_cast<_activity_location_itf*>(origin);
			_activity_location_itf* dest = static_cast<_activity_location_itf*>(destination);
			_activity_plan_itf* activity = move->template destination_activity_reference<_activity_plan_itf*>();

			// General interfaces, to parent and global classes
			_person_itf* person = this->_Parent_Planner->template Parent_Person<_person_itf*>();
			_network_itf* network = person->template network_reference<_network_itf*>();
			_household_itf* household = person->_person_itf::template Household<_household_itf*>();

			if (move->template Update_Locations<_activity_location_itf*>(orig, dest))
			{
				// shift departure time by estimated travel time, and make sure that it does not occur before next iteration
				Simulation_Timestep_Increment start = this->Start_Time<Simulation_Timestep_Increment>();
				Simulation_Timestep_Increment ttime;
				if (orig == dest) ttime = 0;
				else ttime = network->template Get_TTime<_activity_location_itf*, Vehicle_Components::Types::Vehicle_Type_Keys, Simulation_Timestep_Increment, Simulation_Timestep_Increment>(orig, dest, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, start);
				this->Expected_Travel_Time<Simulation_Timestep_Increment>(ttime);

				// recalculate the departure time based on new estimated travel time
				Simulation_Timestep_Increment depart = start - ttime;
				// unless the activity is a return home activity, in which case, use the fixed departure time
				if (activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::AT_HOME_ACTIVITY)
				{
					//RLW%%% - YIKES! _home_activity_itf* home_activity = static_cast<_home_activity_itf*>(activity);
					_home_activity_itf* home_activity = (_home_activity_itf*)activity;
					depart = home_activity->template Fixed_Departure<Simulation_Timestep_Increment>();
				}

				// make sure not departure before minimum allowed
				if (depart < min_departure)
				{
					depart = min_departure + 1;

					if (depart + ttime >(END)*2.0)
					{
						THROW_WARNING("Bad start time, depart=" << depart << ", ttime=" << ttime);
					}
					this->Start_Time<Simulation_Timestep_Increment>(depart + ttime);
				}

				// schedule the routing and do routin if requested through scenario, otherwise move to the activity scheduling phase
				move->template departed_time<Simulation_Timestep_Increment>(depart);
			}
		}
		
		template<typename MasterType, typename InheritanceList>
		void Basic_Activity_Plan_Implementation<MasterType, InheritanceList>::Arrive_At_Activity()
		{

			// Person has arrived at activity destination
			_Parent_Planner->template Parent_Person<_person_itf*>()->Arrive_At_Destination();

		}
		
		template<typename MasterType, typename InheritanceList>
		void Basic_Activity_Plan_Implementation<MasterType, InheritanceList>::Depart_From_Activity()
		{

		}

		template<typename MasterType, typename InheritanceList>
		string Basic_Activity_Plan_Implementation<MasterType, InheritanceList>::Get_Type_String()
		{
			if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::AT_HOME_ACTIVITY)
				return string("HOME");
			else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::EAT_OUT_ACTIVITY)
				return string("EAT OUT");
			else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::ERRANDS_ACTIVITY)
				return string("ERRANDS");
			else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::HEALTHCARE_ACTIVITY)
				return string("HEALTHCARE");
			else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::LEISURE_ACTIVITY)
				return string("LEISURE");
			else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::MAJOR_SHOPPING_ACTIVITY)
				return string("SHOP-MAJOR");
			else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::OTHER_SHOPPING_ACTIVITY)
				return string("SHOP-OTHER");
			else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::PERSONAL_BUSINESS_ACTIVITY)
				return string("PERSONAL");
			else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::PICK_UP_OR_DROP_OFF_ACTIVITY)
				return string("PICKUP-DROPOFF");
			else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY)
				return string("WORK");
			else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::PART_TIME_WORK_ACTIVITY)
				return string("PART_WORK");
			else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::RECREATION_ACTIVITY)
				return string("RECREATION");
			else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::RELIGIOUS_OR_CIVIC_ACTIVITY)
				return string("RELIGIOUS-CIVIC");
			else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::SCHOOL_ACTIVITY)
				return string("SCHOOL");
			else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::SERVICE_VEHICLE_ACTIVITY)
				return string("SERVICE");
			else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::SOCIAL_ACTIVITY)
				return string("SOCIAL");
			else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::WORK_AT_HOME_ACTIVITY)
				return string("WORK AT HOME");
			else
				return string("OTHER");
		}

		template<typename MasterType, typename InheritanceList>
		void Basic_Activity_Plan_Implementation<MasterType, InheritanceList>::Display_Activity()
		{
			cout << endl << "---------------------------------------" << endl;
			cout << "Ptr=" << this << endl;
			cout << "ACTIVITY: ID = " << this->_Activity_Plan_ID << endl;
			cout << "Type = " << this->Get_Type_String() << endl;
			cout << endl;
			cout << "Duration = " << this->__Duration._Value << endl;
			cout << "Location ptr = " << this->_Location << endl;
			cout << "Mode = " << this->_Mode << endl;
			cout << "Movement ptr = " << this->_movement_plan << endl;
			cout << "Parent ptr = " << this->_Parent_Planner << endl;
			cout << "Start time = " << this->___Start_Time._Value << endl;
			cout << "Expected travel time = " << this->__Expected_Travel_Time._Value << endl;
			cout << "Actual Travel Time = " << this->__Actual_Travel_Time._Value << endl;
			cout << "Is Valid = " << this->_is_valid << endl;
			cout << endl;
			cout << "Start plan iteration = " << this->_Start_Time_Planning_Time._iteration << "," << this->_Start_Time_Planning_Time._sub_iteration << endl;
			cout << "Route plan iteration = " << this->_Route_Planning_Time._iteration << "," << this->_Route_Planning_Time._sub_iteration << endl;
			cout << "Mode plan iteration = " << this->_Mode_Planning_Time._iteration << "," << this->_Mode_Planning_Time._sub_iteration << endl;
			cout << "Location plan iteration = " << this->_Location_Planning_Time._iteration << "," << this->_Location_Planning_Time._sub_iteration << endl;
			cout << "Persons plan iteration = " << this->_Involved_Persons_Planning_Time._iteration << "," << this->_Involved_Persons_Planning_Time._sub_iteration << endl;
			cout << "Duration plan iteration = " << this->_Duration_Planning_Time._iteration << "," << this->_Duration_Planning_Time._sub_iteration << endl;
			cout << "Delete iteration = " << this->_Deletion_Time._iteration << "," << this->_Deletion_Time._sub_iteration << endl;
			cout << endl;
			cout << "Duration flexibility = " << this->_Duration_Flexibility << endl;
			cout << "Persons flexibility = " << this->_Involved_Persons_Flexibility << endl;
			cout << "Location flexibility = " << this->_Location_Flexibility << endl;
			cout << "Mode flexibility = " << this->_Mode_Flexibility << endl;
			cout << "Start flexibility = " << this->_Start_Time_Flexibility << endl;
			cout << "---------------------------------------" << endl;
		}



		// Specialization for ADAPTS-style activities with plan horizon info and more detailed set_plan_times function
		implementation struct ADAPTS_Activity_Plan_Implementation : public Basic_Activity_Plan_Implementation<MasterType,INHERIT(ADAPTS_Activity_Plan_Implementation)>
		{
			typedef Basic_Activity_Plan_Implementation<MasterType,INHERIT(ADAPTS_Activity_Plan_Implementation)> base_type;
			typedef typename Basic_Activity_Plan_Implementation<MasterType,INHERIT(ADAPTS_Activity_Plan_Implementation)>::Component_Type ComponentType;

			//================================================================================================================================================================================================
			//================================================================================================================================================================================================
			// Interfaces
			typedef Prototypes::Activity_Planner<ComponentType> this_itf;
			typedef Person_Components::Prototypes::Person_Properties< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Properties)> _properties_itf;
			typedef Person_Components::Prototypes::Person_Scheduler< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Scheduling_Faculty)> _scheduler_itf;
			typedef Person_Components::Prototypes::Person_Properties< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Static_Properties)> _static_properties_itf;
			typedef Person_Components::Prototypes::Person_Planner<typename type_of(base_type::Parent_Planner)> _planning_itf;
			typedef Person_Components::Prototypes::Person<typename _planning_itf::get_type_of(Parent_Person)> _person_itf;
			typedef Person_Components::Prototypes::Destination_Chooser<typename _planning_itf::get_type_of(Destination_Choice_Faculty)> _dest_choice_itf;
			typedef Person_Components::Prototypes::Mode_Chooser<typename _planning_itf::get_type_of(Mode_Choice_Faculty)> _mode_choice_itf;
			typedef Person_Components::Prototypes::Activity_Timing_Chooser<typename _planning_itf::get_type_of(Timing_Chooser)> _timing_choice_itf;
			typedef Scenario_Components::Prototypes::Scenario< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference)> _scenario_itf;
			typedef Network_Components::Prototypes::Network< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(network_reference)> _network_itf;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _network_itf::get_type_of(skimming_faculty)> _skim_itf;
			typedef Network_Skimming_Components::Prototypes::LOS<typename MasterType::los_value_type> los_itf;

			typedef Random_Access_Sequence< typename _network_itf::get_type_of(activity_locations_container)> _activity_locations_container_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(_activity_locations_container_itf)>  _activity_location_itf;
				
			typedef Random_Access_Sequence< typename _activity_location_itf::get_type_of(origin_links)> _links_container_itf;
			typedef Link_Components::Prototypes::Link<get_component_type(_links_container_itf)>  _link_itf;
	
			typedef Pair_Associative_Container< typename _network_itf::get_type_of(zones_container)> _zones_container_itf;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_zones_container_itf)>  _zone_itf;

			//================================================================================================================================================================================================
			//================================================================================================================================================================================================
			member_component_and_feature_accessor(Activity_Planning_Time, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>)

			// Activity attribute planning properties		
			m_data(Types::PLAN_HORIZON_VALUES, Activity_Plan_Horizon, NONE, NONE);
			m_data(Types::PLAN_HORIZON_VALUES, Location_Plan_Horizon, NONE, NONE);
			m_data(Types::PLAN_HORIZON_VALUES, Mode_Plan_Horizon, NONE, NONE);
			m_data(Types::PLAN_HORIZON_VALUES, Start_Time_Plan_Horizon, NONE, NONE);
			m_data(Types::PLAN_HORIZON_VALUES, Duration_Plan_Horizon, NONE, NONE);
			m_data(Types::PLAN_HORIZON_VALUES, Involved_Persons_Plan_Horizon, NONE, NONE);

			template<typename TargetType> void Set_Meta_Attributes();
			template<typename TargetType> void Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType, check_2(TargetType, Simulation_Timestep_Increment, is_same)));
			template<typename TargetType> void Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType, !check_2(TargetType, Simulation_Timestep_Increment, is_same)));

			// Attribute planning events
			template<typename TargetType> void Location_Planning_Event_Handler();
			template<typename TargetType> void Mode_Planning_Event_Handler();
			template<typename TargetType> void Duration_Planning_Event_Handler();
			template<typename TargetType> void Start_Time_Planning_Event_Handler();
			template<typename TargetType> void Involved_Persons_Planning_Event_Handler();
			
		};

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void ADAPTS_Activity_Plan_Implementation<MasterType, InheritanceList>::Set_Meta_Attributes()
		{
			using namespace Activity_Components::Types;
			//===========================================================================================================================
			// Initialize local variables for use
			//---------------------------------------------------------------------------------------------------------------------------

			// get references to use in model
			base_type* base_this = static_cast<base_type*>(this);
			this_itf* pthis = static_cast<this_itf*>(this);
			_planning_itf* planner = pthis->template Parent_Planner<_planning_itf*>();
			_person_itf* person = planner->template Parent_Person<_person_itf*>();
			_properties_itf* properties = person->template Properties<_properties_itf*>();
			_static_properties_itf* static_props = person->template Static_Properties<_static_properties_itf*>();

			//Create local variables
			int ACT1, ACT2, ACT3, ACT4, ACT5, Employed, Student, Male, Senior, TELEWORK, ICT_USE;
			ACT1 = 0; ACT2 = 0; ACT3 = 0; ACT4 = 0; ACT5 = 0; Employed = 0; Student = 0; Male = 0; Senior = 0; TELEWORK = 0; ICT_USE = 0;
			float AvgDur, AvgFreq;

			if (this->_Activity_Type == Types::WORK_AT_HOME_ACTIVITY || this->_Activity_Type == Types::PRIMARY_WORK_ACTIVITY || this->_Activity_Type == Types::PART_TIME_WORK_ACTIVITY || this->_Activity_Type == Types::SCHOOL_ACTIVITY || this->_Activity_Type == Types::OTHER_WORK_ACTIVITY) ACT1 = 1;
			else if (this->_Activity_Type == Types::HEALTHCARE_ACTIVITY || this->_Activity_Type == Types::RELIGIOUS_OR_CIVIC_ACTIVITY || this->_Activity_Type == Types::PERSONAL_BUSINESS_ACTIVITY) ACT2 = 1;
			else if (this->_Activity_Type == Types::ERRANDS_ACTIVITY || this->_Activity_Type == Types::PICK_UP_OR_DROP_OFF_ACTIVITY || this->_Activity_Type == Types::SERVICE_VEHICLE_ACTIVITY) ACT3 = 1;
			else if (this->_Activity_Type == Types::LEISURE_ACTIVITY || this->_Activity_Type == Types::EAT_OUT_ACTIVITY || this->_Activity_Type == Types::RECREATION_ACTIVITY || this->_Activity_Type == Types::SOCIAL_ACTIVITY) ACT4 = 1;
			else if (this->_Activity_Type == Types::OTHER_SHOPPING_ACTIVITY || this->_Activity_Type == Types::MAJOR_SHOPPING_ACTIVITY) ACT5 = 1;
			if (static_props->template Is_Employed<bool>()) Employed = 1;
			if (static_props->template Is_Student<bool>()) Student = 1;
			if (static_props->template Gender<GENDER>() == GENDER::MALE) Male = 1;
			if (static_props->template Age<int>() >= 65) Senior = 1;
			//if (PER.PersonData.ICT_Use != IctType.NULL || PER.PersonData.ICT_Use != IctType.UseLow) ICT_USE = 1;
			if (static_props->template Journey_To_Work_Mode<Person_Components::Types::JOURNEY_TO_WORK_MODE>() == JOURNEY_TO_WORK_MODE::WORKMODE_WORK_AT_HOME) TELEWORK = 1;
			AvgFreq = properties->template Average_Activity_Frequency<Types::ACTIVITY_TYPES, float, typename _properties_itf::Component_Type>(this->_Activity_Type);
			AvgDur = properties->template Average_Activity_Duration<Types::ACTIVITY_TYPES, Time_Days>(this->_Activity_Type);


			//===========================================================================================================================
			// FLEXIBILITY CALCULATIONS
			//---------------------------------------------------------------------------------------------------------------------------
			matrix<double> alpha = matrix<double>(matrix<double>::index_type(3, 5)); // limits matrix
			alpha(0, 0) = -99999999;	alpha(0, 1) = -99999999;	alpha(0, 2) = -99999999;	alpha(0, 3) = -99999999;	alpha(0, 4) = -99999999;
			alpha(1, 0) = 0;			alpha(1, 1) = 0;			alpha(1, 2) = 0;			alpha(1, 3) = 0;			alpha(1, 4) = 0;
			alpha(2, 0) = 99999999;	alpha(2, 1) = 99999999;	alpha(2, 2) = 99999999;	alpha(2, 3) = 99999999;	alpha(2, 4) = 99999999;

			matrix<double> Sigma = matrix<double>(matrix<double>::index_type(5, 5)); // covariance matrix
			Sigma(0, 0) = 1;			Sigma(0, 1) = 0;			Sigma(0, 2) = 0;			Sigma(0, 3) = 0;			Sigma(0, 4) = 0;
			Sigma(1, 0) = 0.099646;	Sigma(1, 1) = 1;			Sigma(1, 2) = 0;			Sigma(1, 3) = 0;			Sigma(1, 4) = 0;
			Sigma(2, 0) = -0.053507;	Sigma(2, 1) = -0.03372;	Sigma(2, 2) = 1;			Sigma(2, 3) = 0;			Sigma(2, 4) = 0;
			Sigma(3, 0) = 0.095299;	Sigma(3, 1) = 0.305332;	Sigma(3, 2) = -0.113887;	Sigma(3, 3) = 1;			Sigma(3, 4) = 0;
			Sigma(4, 0) = 0.124233;	Sigma(4, 1) = 0.168151;	Sigma(4, 2) = 0.025553;	Sigma(4, 3) = -0.099794;	Sigma(4, 4) = 1;

			double P_per[2]; // Holds probability for each level (index by i) of each response (index by j)
			double P_mod[2];
			double P_loc[2];
			double P_tim[2];
			double P_dur[2];

			//Calculate xB values for each response
			double xB_mod = -0.383692 + 0.362839 * Employed + 0.4215 * Student - 0.217904 * Male - 0.267344 * Senior + 0.691999 * Male * Senior + 0.231177 * AvgFreq + 0.568976 * ACT1 * ICT_USE - 1.039075 * ACT1 * AvgFreq;
			double xB_per = -0.338294 + 0.347752 * Student - 0.536749 * Male + 0.259776 * Senior + 0.478152 * Male * Senior + 0.532325 * ICT_USE + 1.508163 * TELEWORK - 1.655547 * ICT_USE * TELEWORK - 1.390818 * ACT1 * +1.352461 * ACT1 * TELEWORK - 0.598393 * ACT2 + 11.034344 * ACT2 * AvgDur - 0.617948 * ACT3 + 1.727868 * AvgFreq * ACT3 + 0.437761 * AvgFreq * ACT4 + 6.708181 * ACT4 * AvgDur - 0.606538 * ACT5 + 0.325567 * AvgFreq * ACT5;
			double xB_loc = -0.368786 - 1.174171 * Male - 0.1914 * Senior + 1.346607 * Male * Senior - 0.3674 * ICT_USE + 0.99392 * TELEWORK - 0.815509 * ICT_USE * TELEWORK + 1.023933 * ACT1 * TELEWORK + 2.325112 * ACT1 * AvgFreq - 8.966545 * ACT2 * AvgDur - 1.02207 * AvgFreq * ACT3 + 0.904036 * ACT4 + 0.637477 * AvgFreq * ACT4 - 8.32642 * ACT4 * AvgDur + 2.382859 * ACT5;
			double xB_tim = 1.326259 - 0.38711 * Male - 0.226637 * Senior + 0.484623 * Male * Senior - 0.302255 * ICT_USE + 0.196488 * TELEWORK + 3.3758 * ACT2 * AvgDur - 0.400564 * ACT3 + 1.301273 * AvgFreq * ACT3 - 0.601686 * ACT5 + 0.192867 * AvgFreq * ACT5;
			double xB_dur = -1.239259 + 1.294471 * Employed + 1.461113 * Student - 0.246144 * Senior - 0.374409 * Male * Senior - 0.23527 * ICT_USE * TELEWORK + 1.30007 * AvgDur + 0.621008 * ACT1 * +0.994951 * ACT1 * ICT_USE - 1.124459 * ACT1 * TELEWORK - 0.56222 * ACT1 * AvgFreq - 2.070631 * ACT1 * AvgDur - 0.167278 * ICT_USE * ACT4 + 0.769745 * AvgFreq * ACT4 + 0.487925 * AvgFreq * ACT5;

			// Get probabilities of each response level
			for (int i = 0; i < 2; i++)
			{
				P_mod[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 0) - xB_mod);
				P_per[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 1) - xB_per);
				P_loc[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 2) - xB_loc);
				P_tim[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 3) - xB_tim);
				P_dur[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 4) - xB_dur);
			}

			// Set correlated random variables
			std::vector<double> rand;
			GLOBALS::Uniform_RNG.template Correlated_Rands<double>(rand, Sigma);

			// Set flexibility values
			for (int i = 0; i < 2; i++)
			{
				if (rand[0] < P_mod[i]) { this->_Mode_Flexibility = (Types::FLEXIBILITY_VALUES)i;				rand[0] = 999; }
				if (rand[1] < P_per[i]) { this->_Involved_Persons_Flexibility = (Types::FLEXIBILITY_VALUES)i;	rand[1] = 999; }
				if (rand[2] < P_loc[i]) { this->_Location_Flexibility = (Types::FLEXIBILITY_VALUES)i;			rand[2] = 999; }
				if (rand[3] < P_tim[i]) { this->_Start_Time_Flexibility = (Types::FLEXIBILITY_VALUES)i;			rand[3] = 999; }
				if (rand[4] < P_dur[i]) { this->_Duration_Flexibility = (Types::FLEXIBILITY_VALUES)i;			rand[4] = 999; }
			}

			// Set results of flexiblity model to use in next models
			double IFLOC, IFTIM, IFDUR, IFMOD;
			IFMOD = P_mod[0];
			IFLOC = P_loc[0];
			IFTIM = P_tim[0];
			IFDUR = P_dur[0];




			//===========================================================================================================================
			// Activity Plan Horizon Calculations
			//---------------------------------------------------------------------------------------------------------------------------
			double xB = 0.130 + 0.640303 * IFLOC - 0.63621 * IFTIM - 1.498554 * IFDUR + 0.767701 * Employed + 0.485362 * ICT_USE
				- 0.560652 * TELEWORK + 1.049907 * ACT1 - 0.446779 * ACT1 * AvgFreq - 0.789474 * ACT1 * IFLOC + 2.241387 * ACT1 * IFDUR
				- 0.892963 * ACT1 * Employed + 1.717603 * ACT1 * Student + 15.248565 * ACT2 * AvgDur + 1.497258 * IFDUR * ACT2
				- 0.613958 * ICT_USE * ACT2 + 1.573672 * AvgFreq * ACT3 + 0.633234 * IFDUR * ACT3 - 0.745918 * Employed * ACT3
				- 1.115487 * Student * ACT3 + 1.049812 * ACT3 * Senior - 0.593425 * ACT3 * Male + 0.519961 * AvgFreq * ACT4 + 2.200494 * AvgDur * ACT4
				+ 1.403807 * IFDUR * ACT4 + 0.837724 * Student * ACT4 + 0.681377 * Senior * ACT4 - 0.793118 * Male * ACT4 - 0.432584 * ICT_USE * ACT4
				+ 0.289143 * AvgFreq * ACT5 + 1.128966 * IFDUR * ACT5 - 0.610606 * Employed * ACT5 + 0.415577 * Senior * ACT5
				- 0.752066 * ICT_USE * ACT5;
			double Pimp, Pday, Pweek;
			double a1 = 1.646398;
			double a2 = 3.505000;
			double r = Uniform_RNG.template Next_Rand<double>();

			// calculate probabilities
			Pimp = 1.0 / (1.0 + exp(xB));
			Pday = 1.0 / (1.0 + exp(-a1 + xB));
			Pweek = 1.0 / (1.0 + exp(-a2 + xB));

			// Set activity plan horizon
			if (r < Pimp) this->_Activity_Plan_Horizon = Types::PLAN_HORIZON_VALUES::IMPULSIVE;
			else if (r < Pday) this->_Activity_Plan_Horizon = Types::PLAN_HORIZON_VALUES::PREPLANNED_SAME_DAY;
			else if (r < Pweek) this->_Activity_Plan_Horizon = Types::PLAN_HORIZON_VALUES::PREPLANNED_2_TO_7_DAYS;
			else this->_Activity_Plan_Horizon = Types::PLAN_HORIZON_VALUES::PREPLANNED_OVER_7_DAYS;




			//===========================================================================================================================
			// ATTRIBUTE Plan Horizon Calculations
			//---------------------------------------------------------------------------------------------------------------------------
			double PLANIMP = Pimp, PLANSAME = Pday - Pimp, PLANWEEK = Pweek - Pday, PLANONTOUR;
			PLANONTOUR = PLANIMP * 1;

			matrix<double> alpha2 = matrix<double>(matrix<double>::index_type(6, 5));
			alpha2(0, 0) = -99999999;	alpha2(0, 1) = -99999999;	alpha2(0, 2) = -99999999;	alpha2(0, 3) = -99999999;	alpha2(0, 4) = -99999999;
			alpha2(1, 0) = 0;			alpha2(1, 1) = 0;			alpha2(1, 2) = 0;			alpha2(1, 3) = 0;			alpha2(1, 4) = 0;
			alpha2(2, 0) = 0.835415;	alpha2(2, 1) = 2.029384;	alpha2(2, 2) = 1.305499;	alpha2(2, 3) = 1.151026;	alpha2(2, 4) = 0.434582;
			alpha2(3, 0) = 1.486357;	alpha2(3, 1) = 3.614181;	alpha2(3, 2) = 2.316088;	alpha2(3, 3) = 2.026512;	alpha2(3, 4) = 0.879016;
			alpha2(4, 0) = 1.794494;	alpha2(4, 1) = 4.401609;	alpha2(4, 2) = 2.796206;	alpha2(4, 3) = 2.643064;	alpha2(4, 4) = 1.095505;
			alpha2(5, 0) = 99999999;	alpha2(5, 1) = 99999999;	alpha2(5, 2) = 99999999;	alpha2(5, 3) = 99999999;	alpha2(5, 4) = 99999999;

			matrix<double> Sigma2 = matrix<double>(matrix<double>::index_type(5, 5));
			Sigma2(0, 0) = 1;			Sigma2(0, 1) = 0;			Sigma2(0, 2) = 0;			Sigma2(0, 3) = 0;			Sigma2(0, 4) = 0;
			Sigma2(1, 0) = 0.14308;	Sigma2(1, 1) = 1;			Sigma2(1, 2) = 0;			Sigma2(1, 3) = 0;			Sigma2(1, 4) = 0;
			Sigma2(2, 0) = 0.148218;	Sigma2(2, 1) = 0.104424;	Sigma2(2, 2) = 1;			Sigma2(2, 3) = 0;			Sigma2(2, 4) = 0;
			Sigma2(3, 0) = 0.159259;	Sigma2(3, 1) = 0.180953;	Sigma2(3, 2) = 0.352088;	Sigma2(3, 3) = 1;			Sigma2(3, 4) = 0;
			Sigma2(4, 0) = 0.186676;	Sigma2(4, 1) = 0.177994;	Sigma2(4, 2) = 0.218223;	Sigma2(4, 3) = 0.538872;	Sigma2(4, 4) = 1;

			double P_pmod[5];
			double P_pper[5]; // Holds probability for each level (index by i) of each response (index by j)
			double P_ploc[5];
			double P_ptim[5];
			double P_pdur[5];

			//Calculate xB values for each response
			double xB_pmod = 0.618 + 0.262288 * ACT2 + 0.376779 * ACT4 + 0.195715 * ACT5 + 0.575386 * PLANONTOUR - 1.527302 * PLANIMP
				- 0.492435 * PLANSAME - 0.271618 * PLANWEEK + 0.633625 * Employed + 1.043977 * Student - 0.115081 * Senior
				+ 0.106084 * IFDUR + 0.785746 * ACT1 * AvgDur + 0.353502 * PLANWEEK * ACT3 - 3.92232 * ACT4 * AvgDur
				+ 0.456054 * ACT1 * TELEWORK + 0.130093 * ICT_USE;
			double xB_pper = 4.096 - 5.532536 * PLANIMP - 2.912268 * PLANSAME - 1.231383 * PLANWEEK + 0.825496 * ACT2 * Student
				- 5.690886 * ACT4 * AvgDur + 0.358763 * ACT4 * ICT_USE;
			double xB_ploc = 2.079 - 3.2086 * PLANIMP - 1.40061 * PLANSAME - 0.581665 * PLANWEEK + 0.202875 * Employed + 0.350806 * Student
				+ 1.846201 * AvgDur + 0.951409 * ACT1 * IFLOC + 0.278635 * ACT4 * IFLOC + 0.839625 * ICT_USE
				- 0.736206 * ACT2 * ICT_USE - 0.692824 * ACT3 * ICT_USE - 1.033742 * ACT4 * ICT_USE - 1.166372 * ACT5 * ICT_USE;
			double xB_ptim = 1.612 + 0.206278 * ACT5 - 3.178382 * PLANIMP - 1.570305 * PLANSAME - 0.932677 * PLANWEEK + 0.359391 * Employed
				+ 0.163188 * IFDUR + 0.616433 * ACT3 * IFTIM + 0.756115 * ACT1 * TELEWORK - 0.145507 * ICT_USE
				+ 0.185875 * ACT4 * ICT_USE - 0.497667 * ACT5 * ICT_USE;
			double xB_pdur = -1.264 + 0.323092 * ACT2 - 1.466871 * PLANIMP - 0.518569 * PLANSAME - 0.35598 * PLANWEEK + 0.742175 * Employed
				+ 0.157659 * Senior + 0.130234 * Male + 0.297442 * IFTIM + 0.835193 * IFDUR + 0.266187 * AvgFreq
				- 0.395776 * ACT2 * IFDUR - 0.282278 * ACT5 * IFDUR - 0.338128 * ACT5 * IFTIM + 0.879233 * ACT1 * TELEWORK;

			// Get probabilities of each response level
			for (int i = 0; i < 5; i++)
			{
				P_pmod[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha2(i + 1, 0) - xB_pmod, 0, 1);
				P_pper[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha2(i + 1, 1) - xB_pper, 0, 1);
				P_ploc[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha2(i + 1, 2) - xB_ploc, 0, 1);
				P_ptim[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha2(i + 1, 3) - xB_ptim, 0, 1);
				P_pdur[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha2(i + 1, 4) - xB_pdur, 0, 1);
			}

			// Set correlated random variables
			std::vector<double> rand2;
			GLOBALS::Uniform_RNG.template Correlated_Rands<double>(rand2, Sigma2);

			// Set attribute plan horizon values
			for (int i = 0; i < 5; i++)
			{
				// convert from i to planhorizon values (skip 'previous day')
				int x = i + 1; if (i > 1) x = x + 1;
				PLAN_HORIZON_VALUES value;
				switch (i)
				{
				case 0: value = PLAN_HORIZON_VALUES::IMPULSIVE; break;
				case 1: value = PLAN_HORIZON_VALUES::PREPLANNED_SAME_DAY; break;
				case 2: value = PLAN_HORIZON_VALUES::PREPLANNED_2_TO_7_DAYS; break;
				case 3: value = PLAN_HORIZON_VALUES::PREPLANNED_OVER_7_DAYS; break;
				default: value = ROUTINE;
				}

				if (rand2[0] < P_pmod[i]) { this->_Mode_Plan_Horizon = value;				rand2[0] = 999; }
				if (rand2[1] < P_pper[i]) { this->_Involved_Persons_Plan_Horizon = value;	rand2[1] = 999; }
				if (rand2[2] < P_ploc[i]) { this->_Location_Plan_Horizon = value;			rand2[2] = 999; }
				if (rand2[3] < P_ptim[i]) { this->_Start_Time_Plan_Horizon = value;			rand2[3] = 999; }
				if (rand2[4] < P_pdur[i]) { this->_Duration_Plan_Horizon = value;			rand2[4] = 999; }
			}
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void ADAPTS_Activity_Plan_Implementation<MasterType, InheritanceList>::Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType, check_2(TargetType, Simulation_Timestep_Increment, is_same)))
		{
			// get references to use in model
			base_type* base_this = (base_type*)this;

			Revision &start = this->template Start_Time_Planning_Time<  Revision&>();
			Revision &dur = this->template Duration_Planning_Time<  Revision&>();
			Revision &loc = this->template Location_Planning_Time<  Revision&>();
			Revision &mode = this->template Mode_Planning_Time<  Revision&>();
			Revision &persons = this->template Involved_Persons_Planning_Time<  Revision&>();
			Revision &route = this->template Route_Planning_Time<  Revision&>();


			// Set revisions based on the plan-horizons for each attribute
			int dur_sub = (int)(this->_Duration_Plan_Horizon);
			int loc_sub = (int)(this->_Location_Plan_Horizon);
			int mode_sub = (int)(this->_Mode_Plan_Horizon);
			int inv_sub = (int)(this->_Involved_Persons_Plan_Horizon);
			int start_sub = (int)(this->_Start_Time_Plan_Horizon);

			std::vector<int*> order;
			order.push_back(&dur_sub);
			order.push_back(&loc_sub);
			order.push_back(&mode_sub);
			order.push_back(&inv_sub);
			order.push_back(&start_sub);
			sort(order.begin(), order.end(), Ptr_Comparer<int*>);
			for (int i = 0; i<order.size(); i++) (*(order[i])) = i + 1;

			// give intrinsic order when simultaneous plan horizons are specified
			//if (start_sub == dur_sub) dur_sub++;
			//if (start_sub == loc_sub) loc_sub++;
			//if (start_sub == mode_sub) mode_sub++;
			//if (start_sub == inv_sub) inv_sub++;
			//if (dur_sub == loc_sub) loc_sub++;
			//if (dur_sub == mode_sub) mode_sub++;
			//if (dur_sub == inv_sub) inv_sub++;
			//if (dur_sub == start_sub) start_sub++;
			//if (loc_sub == mode_sub) mode_sub++;
			//if (loc_sub == inv_sub) inv_sub++;
			//if (loc_sub == dur_sub) dur_sub++;
			//if (loc_sub == start_sub) start_sub++;
			//if (mode_sub == inv_sub) inv_sub++;
			//if (mode_sub == loc_sub) loc_sub++;
			//if (mode_sub == dur_sub) dur_sub++;
			//if (mode_sub == start_sub) start_sub++;

			int route_sub = max(start_sub, max(dur_sub, max(loc_sub, max(mode_sub, inv_sub)))) + 1;

			start._iteration = max(iteration() + 1, (int)planning_time);
			dur._iteration = END; //dur._iteration = max(_iteration+1, (int)planning_time);
			loc._iteration = max(iteration() + 1, (int)planning_time);
			mode._iteration = max(iteration() + 1, (int)planning_time);
			persons._iteration = max(iteration() + 1, (int)planning_time);
			route._iteration = max(iteration() + 1, (int)planning_time);

			start._sub_iteration = start_sub;
			dur._sub_iteration = END; //dur._sub_iteration = dur_sub;
			loc._sub_iteration = loc_sub;
			mode._sub_iteration = mode_sub;
			persons._sub_iteration = inv_sub;
			route._sub_iteration = route_sub;
		}
		
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void ADAPTS_Activity_Plan_Implementation<MasterType, InheritanceList>::Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType, !check_2(TargetType, Simulation_Timestep_Increment, is_same)))
		{
			assert_check_2(TargetType, Simulation_Timestep_Increment, is_same, "Error: planning_time must be passed as a Simulation_Timestep_Increment type when using this function.");
		}

		// Attribute planning events
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void ADAPTS_Activity_Plan_Implementation<MasterType, InheritanceList>::Location_Planning_Event_Handler()
		{
			base_type* bthis = (base_type*)this;
			this_itf* pthis = (this_itf*)this;

			// references to external agents
			_planning_itf* planner = bthis->template Parent_Planner<_planning_itf*>();
			_person_itf* person = planner->template Parent_Person<_person_itf*>();
			_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
			_dest_choice_itf* dest_chooser = planner->template Destination_Choice_Faculty<_dest_choice_itf*>();
			_network_itf* network = person->template network_reference<_network_itf*>();

			_activity_location_itf *home_loc = person->template Home_Location<_activity_location_itf*>();


			// call destination choice - set to home of no location could be chosen
			_activity_location_itf* dest = dest_chooser->template Choose_Destination<this_itf*, _activity_location_itf*>(pthis);


			// check that origin and destination are valid
			if (dest != nullptr)
			{
				bthis->template Location<_activity_location_itf*>(dest);
			}
			else
			{
				// Delete the activity from schedule if no location chosen
				//pthis->template Free_Activity<NT>();
				scheduler->template Remove_Activity_Plan<this_itf*>(pthis);
			}
		}
		
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void ADAPTS_Activity_Plan_Implementation<MasterType, InheritanceList>::Mode_Planning_Event_Handler()
		{
			base_type* bthis = (base_type*)this;
			this_itf* pthis = (this_itf*)this;

			// references to external agents
			_planning_itf* planner = bthis->template Parent_Planner<_planning_itf*>();
			_person_itf* person = planner->template Parent_Person<_person_itf*>();
			_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
			_mode_choice_itf* mode_chooser = planner->template Mode_Choice_Faculty<_mode_choice_itf*>();
			_network_itf* network = person->template network_reference<_network_itf*>();

			Vehicle_Components::Types::Vehicle_Type_Keys mode = mode_chooser->template Choose_Mode<this_itf*, Vehicle_Components::Types::Vehicle_Type_Keys>(pthis);
			pthis->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(mode);

		}
		
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void ADAPTS_Activity_Plan_Implementation<MasterType, InheritanceList>::Duration_Planning_Event_Handler()
		{

		}
		
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void ADAPTS_Activity_Plan_Implementation<MasterType, InheritanceList>::Start_Time_Planning_Event_Handler()
		{
			this_itf* pthis = (this_itf*)this;
			base_type* bthis = (base_type*)this;

			// interfaces
			_planning_itf* planner = pthis->template Parent_Planner<_planning_itf*>();
			_person_itf* person = planner->template Parent_Person<_person_itf*>();
			_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
			_scenario_itf* scenario = (_scenario_itf*)_global_scenario;

			// get the combined start time and duration
			_timing_choice_itf* timing_planner = planner->template Timing_Chooser<_timing_choice_itf*>();
			//pair<Time_Seconds, Time_Seconds> start_and_duration = timing_planner->template Get_Start_Time_and_Duration<ComponentType*, Time_Seconds>(this);
			pair<Time_Seconds, Time_Seconds> start_and_duration = timing_planner->template Timing_Choice<ComponentType*, Time_Seconds>(this);

			// make sure start time is not prior to current iteration
			Time_Seconds time_min = Simulation_Time.template Future_Time<Time_Seconds, Time_Seconds>(planner->template Planning_Time_Increment<Time_Seconds>());
			pthis->template Start_Time<Time_Seconds>(max<int>(start_and_duration.first, time_min.Value));


			// set the duration, making sure it fits into current schedule slot
			pthis->template Duration<Time_Seconds>(0.0f);
			float duration = max<float>(start_and_duration.second, (float)pthis->template Minimum_Duration<Time_Seconds>());
			pthis->template Duration<Time_Seconds>(duration);

			////========================================================================
			//// Resolve timing conflicts when timing is known
			//bool is_scheduled =	scheduler->template Resolve_Timing_Conflict<this_itf*>(pthis,false);
			//// if conflict not resolved remove activity from schedule and modify routing response time so no further planning is done
			//if (!is_scheduled) 
			//{
			//	pthis->template Unschedule_Activity_Events<NT>();
			//	scheduler->template Remove_Activity_Plan<this_itf*>(pthis);
			//}


			//========================================================================
			// END HERE if no planner routing is requested through scenario
			if (!scenario->template do_planner_routing<bool>())
			{
				return;
			}
			//------------------------------------------------------------------------




			//========================================================================
			// start routing on the planning timestep at 2 times the estimated travel time from skims prior to departure - rounded to nearest minute
			Time_Minutes exp_ttime = 0.0f;
			if (bthis->Location_Is_Planned())
			{
				_network_itf* network = person->template network_reference<_network_itf*>();
				_activity_location_itf* orig = person->template Home_Location<_activity_location_itf*>();
				_activity_location_itf* dest = pthis->template Location<_activity_location_itf*>();
				exp_ttime = network->template Get_TTime<_activity_location_itf*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Minutes, Time_Minutes>(orig, dest, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, pthis->template Start_Time<Time_Minutes>());
			}
			exp_ttime = max<float>(exp_ttime, 30.0f);


			//==============================================================================================
			// Reset the route planning time based on the expected departure time, and aggregate as needed
			//---------------------------
			// Aggregate plan routing
			if (scenario->template aggregate_routing<bool>())
			{
				Time_Minutes start_minutes = (int)(pthis->template Start_Time<Time_Minutes>() - (exp_ttime * 2.0));
				int start_increment = max<int>(Simulation_Time.template Convert_Time_To_Simulation_Timestep<Time_Minutes>(start_minutes), iteration());
				pthis->template Route_Planning_Time<Revision&>()._iteration = start_increment;
			}
			//---------------------------
			// Disaggregate plan routing
			else
			{
				Simulation_Timestep_Increment start_seconds = pthis->template Start_Time<Simulation_Timestep_Increment>() - Simulation_Time.template Convert_Time_To_Simulation_Timestep<Time_Minutes>(exp_ttime * 2.0);
				int start_increment = max<int>(start_seconds, iteration());
				pthis->template Route_Planning_Time<Revision&>()._iteration = start_increment;
			}

		}
		
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void ADAPTS_Activity_Plan_Implementation<MasterType, InheritanceList>::Involved_Persons_Planning_Event_Handler()
		{
		}



		// Specialization for ADAPTS-style routine activities
		implementation struct ADAPTS_Routine_Activity_Plan_Implementation : public Basic_Activity_Plan_Implementation<MasterType,INHERIT(ADAPTS_Routine_Activity_Plan_Implementation)>
		{
			typedef Basic_Activity_Plan_Implementation<MasterType,INHERIT(ADAPTS_Routine_Activity_Plan_Implementation)> base_type;
			typedef typename Basic_Activity_Plan_Implementation<MasterType,INHERIT(ADAPTS_Routine_Activity_Plan_Implementation)>::Component_Type ComponentType;
			typedef Prototypes::Activity_Planner<ComponentType> this_itf;
		
			//================================================================================================================================================================================================
			//================================================================================================================================================================================================
			// Interfaces
			typedef Person_Components::Prototypes::Person_Properties< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Properties)> _properties_itf;
			typedef Person_Components::Prototypes::Person_Properties< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Static_Properties)> _static_properties_itf;
			typedef Person_Components::Prototypes::Person_Planner<typename type_of(base_type::Parent_Planner)> _planning_itf;
			typedef Person_Components::Prototypes::Person_Scheduler< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Scheduling_Faculty)> _scheduler_itf;
			typedef Person_Components::Prototypes::Person<typename _planning_itf::get_type_of(Parent_Person)> _person_itf;
			typedef Person_Components::Prototypes::Destination_Chooser<typename _planning_itf::get_type_of(Destination_Choice_Faculty)> _dest_choice_itf;
			typedef Person_Components::Prototypes::Mode_Chooser<typename _planning_itf::get_type_of(Mode_Choice_Faculty)> _mode_choice_itf;
			typedef Scenario_Components::Prototypes::Scenario< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference)> _scenario_itf;
			typedef Network_Components::Prototypes::Network< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(network_reference)> _network_itf;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _network_itf::get_type_of(skimming_faculty)> _skim_itf;
			typedef Person_Components::Prototypes::Activity_Timing_Chooser<typename _planning_itf::get_type_of(Timing_Chooser)> _timing_choice_itf;
			
			typedef Random_Access_Sequence< typename _network_itf::get_type_of(activity_locations_container)> _activity_locations_container_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(_activity_locations_container_itf)>  _activity_location_itf;
				
			typedef Random_Access_Sequence< typename _activity_location_itf::get_type_of(origin_links)> _links_container_itf;
			typedef Link_Components::Prototypes::Link<get_component_type(_links_container_itf)>  _link_itf;
	
			typedef Pair_Associative_Container< typename _network_itf::get_type_of(zones_container)> _zones_container_itf;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_zones_container_itf)>  _zone_itf;

			//================================================================================================================================================================================================
			//================================================================================================================================================================================================

			// Fundamental activity properties
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Activity_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}

			// Activity attribute planning properties
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Location_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Mode_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Start_Time_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Duration_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Involved_Persons_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}


			template<typename TargetType> void Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType,check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				base_type* base_this = (base_type*)this;

				this->template Start_Time_Planning_Time<  Revision&>()._iteration = std::max(iteration()+1, (int)planning_time);
				this->template Start_Time_Planning_Time<  Revision&>()._sub_iteration = 0;
				this->template Duration_Planning_Time<  Revision&>()._iteration = std::max(iteration()+1, (int)planning_time);
				this->template Duration_Planning_Time<  Revision&>()._sub_iteration = 1;
				this->template Location_Planning_Time<  Revision&>()._iteration = std::max(iteration()+1, (int)planning_time);
				this->template Location_Planning_Time<  Revision&>()._sub_iteration = 2;
				this->template Mode_Planning_Time<  Revision&>()._iteration = std::max(iteration()+1, (int)planning_time);
				this->template Mode_Planning_Time<  Revision&>()._sub_iteration = 3;
				this->template Involved_Persons_Planning_Time<  Revision&>()._iteration = std::max(iteration()+1, (int)planning_time);
				this->template Involved_Persons_Planning_Time<  Revision&>()._sub_iteration = 4;
				this->template Route_Planning_Time<  Revision&>()._iteration = std::max(iteration()+1, (int)planning_time);
				this->template Route_Planning_Time<  Revision&>()._sub_iteration = 5;
			}
			template<typename TargetType> void Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType,!check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				assert_check_2(TargetType, Simulation_Timestep_Increment, is_same, "Error: planning_time must be passed as a Simulation_Timestep_Increment type when using this function.");
			}

			template<typename TargetType> void Location_Planning_Event_Handler()
			{
				this_itf* pthis = (this_itf*)this;
				base_type* base_this = (base_type*)this;
				_person_itf* person = this->_Parent_Planner->template Parent_Person<_person_itf*>();
				_activity_location_itf* orig = person->template Home_Location<_activity_location_itf*>();

				// Select the location based on the activity type
				_activity_location_itf* dest = nullptr;		
				if (pthis->template Activity_Type<Types::ACTIVITY_TYPES>() == Types::AT_HOME_ACTIVITY)
				{
					dest = person->template Home_Location<_activity_location_itf*>();
				}
				else if (pthis->template Activity_Type<Types::ACTIVITY_TYPES>() == Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY || pthis->template Activity_Type<Types::ACTIVITY_TYPES>() == Types::ACTIVITY_TYPES::PART_TIME_WORK_ACTIVITY)
				{
					dest = person->template Work_Location<_activity_location_itf*>();
				}
				else if (pthis->template Activity_Type<Types::ACTIVITY_TYPES>() == Types::ACTIVITY_TYPES::SCHOOL_ACTIVITY)
				{
					dest = person->template School_Location<_activity_location_itf*>();
				}
				else if (pthis->template Activity_Type<Types::ACTIVITY_TYPES>() == Types::ACTIVITY_TYPES::WORK_AT_HOME_ACTIVITY)
				{
					dest = person->template Home_Location<_activity_location_itf*>();
				}
				else
				{
					//----------------------------------------------------------------
					// Print to log file
					//stringstream s;
					//s <<"ACTIVITY NOT GENERATED, only work, home, school and work at home activities can be routine at this point: "<< person->template uuid<int>();
					//s << "," <<orig << ", " <<dest<<endl;
					//base_this->_Parent_Planner->template Write_To_Log<stringstream&>(s);
					//----------------------------------------------------------------
					return;
				}

				// check that origin and destination are valid
				if (orig != nullptr && dest != nullptr) 
				{
					pthis->template Location<_activity_location_itf*>(dest);
				}
				else
				{
					//----------------------------------------------------------------
					// Print to log file
					//stringstream s;
					//s <<"ACTIVITY NOT GENERATED, null origin or destination: "<< person->template uuid<int>();
					//s << "," <<orig << ", " <<dest<<endl;
					//base_this->_Parent_Planner->template Write_To_Log<stringstream&>(s);
					//----------------------------------------------------------------
				}
			}

			template<typename TargetType> void Mode_Planning_Event_Handler()
			{
				base_type* bthis = (base_type*)this;
				this_itf* pthis = (this_itf*)this;

				// references to external agents
				_planning_itf* planner = bthis->template Parent_Planner<_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
				_mode_choice_itf* mode_chooser = planner->template Mode_Choice_Faculty<_mode_choice_itf*>();
				_network_itf* network = person->template network_reference<_network_itf*>();


				Vehicle_Components::Types::Vehicle_Type_Keys mode = mode_chooser->template Choose_Mode<this_itf*,Vehicle_Components::Types::Vehicle_Type_Keys>(pthis);	
				pthis->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(mode);
			}

			template<typename TargetType> void Duration_Planning_Event_Handler()
			{
			}

			template<typename TargetType> void Start_Time_Planning_Event_Handler()
			{
				using namespace Activity_Components::Types;

				this_itf* pthis = (this_itf*)this;
				base_type* bthis = (base_type*)this;

				_person_itf* person = this->_Parent_Planner->template Parent_Person<_person_itf*>();
				_static_properties_itf* static_properties = person->template Static_Properties<_static_properties_itf*>();
				_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
				_scenario_itf* scenario = (_scenario_itf*)_global_scenario;


				ACTIVITY_TYPES act_type = pthis->template Activity_Type<ACTIVITY_TYPES>();

				// School Activity start time (randomly between 7 and 9AM)
				if (act_type == ACTIVITY_TYPES::SCHOOL_ACTIVITY || act_type == ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY || act_type == ACTIVITY_TYPES::PART_TIME_WORK_ACTIVITY || act_type == ACTIVITY_TYPES::WORK_AT_HOME_ACTIVITY )
				{
					// interfaces
					_planning_itf* planner = pthis->template Parent_Planner<_planning_itf*>();
					_person_itf* person = planner->template Parent_Person<_person_itf*>();

					// get the combined start time and duration
					_timing_choice_itf* timing_planner = planner->template Timing_Chooser<_timing_choice_itf*>();
					pair<Time_Seconds,Time_Seconds> start_and_duration = timing_planner->template Get_Start_Time_and_Duration<this_itf*,Time_Seconds>(pthis);

					// make sure start time is not prior to current iteration
					Time_Seconds time_min = Simulation_Time.template Future_Time<Time_Seconds,Time_Seconds>(planner->template Planning_Time_Increment<Time_Seconds>());
					pthis->template Start_Time<Time_Seconds>(max<int>(start_and_duration.first,time_min.Value));
					pthis->template Duration<Time_Seconds>(start_and_duration.second);
				}
				else THROW_EXCEPTION("ERROR: only work and school activities are currently allowed to be routine.");


				////========================================================================
				//// Resolve timing conflicts when timing is known
				////------------------------------------------------------------------------
				//bool is_scheduled =	scheduler->template Resolve_Timing_Conflict<this_itf*>(pthis,false);
				//
				//// if conflict not resolved remove activity from schedule and modify routing response time so no further planning is done
				//if (!is_scheduled) 
				//{
				//	//pthis->template Free_Activity<NT>();
				//	scheduler->template Remove_Activity_Plan<this_itf*>(pthis);
				//}


				//========================================================================
				// END HERE if no planner routing is requested through scenario
				if (!scenario->template do_planner_routing<bool>())
				{
					return;
				}
				//------------------------------------------------------------------------



				//==============================================================================================
				// OTHERWISE, reset the event time for the planning router based on expected departure time
				//------------------------------------------------------------------------
				// start routing on the planning timestep at 1.5 times the estimated travel time from skims prior to departure - rounded to nearest minute
				Time_Minutes exp_ttime=0.0f;
				if (bthis->Location_Is_Planned())
				{
					_network_itf* network = person->template network_reference<_network_itf*>();
					_activity_location_itf* orig = person->template Home_Location<_activity_location_itf*>();
					_activity_location_itf* dest = bthis->template Location<_activity_location_itf*>();

					exp_ttime = network->template Get_TTime<_activity_location_itf*,Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours,Time_Minutes >(orig,dest,Vehicle_Components::Types::Vehicle_Type_Keys::SOV, pthis->template Start_Time<Time_Hours>());
				}
				exp_ttime = max<float>(exp_ttime,30.0f);
				
				//---------------------------
				// Aggregate plan routing
				if (scenario->template aggregate_routing<bool>())
				{
					Time_Minutes start_minutes = (int)(pthis->template Start_Time<Time_Minutes>() - (exp_ttime * 2.0));
					int start_increment = max<int>(Simulation_Time.template Convert_Time_To_Simulation_Timestep<Time_Minutes>(start_minutes), iteration());
					pthis->template Route_Planning_Time<Revision&>()._iteration = start_increment;
				}
				//---------------------------
				// Disaggregate plan routing
				else
				{
					Simulation_Timestep_Increment start_seconds = pthis->template Start_Time<Simulation_Timestep_Increment>() - Simulation_Time.template Convert_Time_To_Simulation_Timestep<Time_Minutes>(exp_ttime * 2.0);
					int start_increment = max<int>(start_seconds, iteration());
					pthis->template Route_Planning_Time<Revision&>()._iteration = start_increment;
				}

			}

			template<typename TargetType> void Involved_Persons_Planning_Event_Handler()
			{
				
			}

			template<typename TargetType> void Set_Meta_Attributes()
			{
				using namespace Activity_Components::Types;
				//===========================================================================================================================
				// Initialize local variables for use
				//---------------------------------------------------------------------------------------------------------------------------

				// get references to use in model
				base_type* base_this = (base_type*)this;
				this_itf* pthis = (this_itf*)this;
				_planning_itf* planner = pthis->template Parent_Planner<_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				_properties_itf* properties = person->template Properties<_properties_itf*>();
				_static_properties_itf* static_props = person->template Static_Properties<_static_properties_itf*>();

				//Create local variables
				int ACT1, ACT2, ACT3, ACT4, ACT5, Employed, Student, Male, Senior, TELEWORK, ICT_USE;
				ACT1 = 0; ACT2 = 0; ACT3 = 0; ACT4 = 0; ACT5 = 0; Employed = 0; Student = 0; Male = 0; Senior = 0; TELEWORK = 0; ICT_USE = 0;
				float AvgDur, AvgFreq;

				if (this->_Activity_Type == WORK_AT_HOME_ACTIVITY || this->_Activity_Type == PRIMARY_WORK_ACTIVITY || this->_Activity_Type == PART_TIME_WORK_ACTIVITY || this->_Activity_Type == SCHOOL_ACTIVITY || this->_Activity_Type == OTHER_WORK_ACTIVITY) ACT1 = 1;
				else if (this->_Activity_Type == HEALTHCARE_ACTIVITY || this->_Activity_Type == RELIGIOUS_OR_CIVIC_ACTIVITY || this->_Activity_Type == PERSONAL_BUSINESS_ACTIVITY) ACT2 = 1;
				else if (this->_Activity_Type == Types::ERRANDS_ACTIVITY || this->_Activity_Type == PICK_UP_OR_DROP_OFF_ACTIVITY || this->_Activity_Type == SERVICE_VEHICLE_ACTIVITY) ACT3 = 1;
				else if (this->_Activity_Type == Types::LEISURE_ACTIVITY|| this->_Activity_Type == Types::EAT_OUT_ACTIVITY || this->_Activity_Type == Types::RECREATION_ACTIVITY || this->_Activity_Type == Types::SOCIAL_ACTIVITY) ACT4 = 1;
				else if (this->_Activity_Type == Types::OTHER_SHOPPING_ACTIVITY ||this-> _Activity_Type == Types::MAJOR_SHOPPING_ACTIVITY) ACT5 = 1;
				if (static_props->template Is_Employed<bool>()) Employed = 1;
				if (static_props->template Is_Student<bool>()) Student = 1;
				if (static_props->template Gender<GENDER>() == GENDER::MALE) Male = 1;
				if (static_props->template Age<int>() >= 65) Senior = 1;
				//if (PER.PersonData.ICT_Use != IctType.NULL || PER.PersonData.ICT_Use != IctType.UseLow) ICT_USE = 1;
				if (static_props->template Journey_To_Work_Mode<Person_Components::Types::JOURNEY_TO_WORK_MODE>() == JOURNEY_TO_WORK_MODE::WORKMODE_WORK_AT_HOME) TELEWORK = 1;
				AvgFreq = properties->template Average_Activity_Frequency<ACTIVITY_TYPES,float, typename _properties_itf::Component_Type>(this->_Activity_Type);
				AvgDur = properties->template Average_Activity_Duration<ACTIVITY_TYPES,Time_Minutes>(this->_Activity_Type);


				//===========================================================================================================================
				// FLEXIBILITY CALCULATIONS
				//---------------------------------------------------------------------------------------------------------------------------
				matrix<double> alpha = matrix<double>(matrix<double>::index_type(3,5));
				alpha(0,0)=-99999999;	alpha(0,1)=-99999999;	alpha(0,2)=-99999999;	alpha(0,3)=-99999999;	alpha(0,4)=-99999999;
				alpha(1,0)=0;			alpha(1,1)=0;			alpha(1,2)=0;			alpha(1,3)=0;			alpha(1,4)=0;
				alpha(2,0)=99999999;	alpha(2,1)=99999999;	alpha(2,2)=99999999;	alpha(2,3)=99999999;	alpha(2,4)=99999999;

				matrix<double> Sigma = matrix<double>(matrix<double>::index_type(5,5)); // covariance matrix
				Sigma(0,0)=1;			Sigma(0,1)=0;			Sigma(0,2)=0;			Sigma(0,3)=0;			Sigma(0,4)=0;
				Sigma(1,0)=0.099646;	Sigma(1,1)=1;			Sigma(1,2)=0;			Sigma(1,3)=0;			Sigma(1,4)=0;
				Sigma(2,0)=-0.053507;	Sigma(2,1)=-0.03372;	Sigma(2,2)=1;			Sigma(2,3)=0;			Sigma(2,4)=0;
				Sigma(3,0)=0.095299;	Sigma(3,1)=0.305332;	Sigma(3,2)=-0.113887;	Sigma(3,3)=1;			Sigma(3,4)=0;
				Sigma(4,0)=0.124233;	Sigma(4,1)=0.168151;	Sigma(4,2)=0.025553;	Sigma(4,3)=-0.099794;	Sigma(4,4)=1;

				double P_per[2]; // Holds probability for each level (index by i) of each response (index by j)
				double P_mod[2];
				double P_loc[2];
				double P_tim[2];
				double P_dur[2];

				//Calculate xB values for each response
				double xB_mod = -0.383692 + 0.362839 * Employed + 0.4215 * Student - 0.217904 * Male - 0.267344 * Senior + 0.691999 * Male * Senior + 0.231177 * AvgFreq + 0.568976 * ACT1 * ICT_USE - 1.039075 * ACT1 * AvgFreq;
				double xB_per = -0.338294 + 0.347752 * Student - 0.536749 * Male + 0.259776 * Senior + 0.478152 * Male * Senior + 0.532325 * ICT_USE + 1.508163 * TELEWORK - 1.655547 * ICT_USE * TELEWORK - 1.390818 * ACT1 * +1.352461 * ACT1 * TELEWORK - 0.598393 * ACT2 + 11.034344 * ACT2 * AvgDur - 0.617948 * ACT3 + 1.727868 * AvgFreq * ACT3 + 0.437761 * AvgFreq * ACT4 + 6.708181 * ACT4 * AvgDur - 0.606538 * ACT5 + 0.325567 * AvgFreq * ACT5;
				double xB_loc = -0.368786 - 1.174171 * Male - 0.1914 * Senior + 1.346607 * Male * Senior - 0.3674 * ICT_USE + 0.99392 * TELEWORK - 0.815509 * ICT_USE * TELEWORK + 1.023933 * ACT1 * TELEWORK + 2.325112 * ACT1 * AvgFreq - 8.966545 * ACT2 * AvgDur - 1.02207 * AvgFreq * ACT3 + 0.904036 * ACT4 + 0.637477 * AvgFreq * ACT4 - 8.32642 * ACT4 * AvgDur + 2.382859 * ACT5;
				double xB_tim = 1.326259 - 0.38711 * Male - 0.226637 * Senior + 0.484623 * Male * Senior - 0.302255 * ICT_USE + 0.196488 * TELEWORK + 3.3758 * ACT2 * AvgDur - 0.400564 * ACT3 + 1.301273 * AvgFreq * ACT3 - 0.601686 * ACT5 + 0.192867 * AvgFreq * ACT5;
				double xB_dur = -1.239259 + 1.294471 * Employed + 1.461113 * Student - 0.246144 * Senior - 0.374409 * Male * Senior - 0.23527 * ICT_USE * TELEWORK + 1.30007 * AvgDur + 0.621008 * ACT1 * +0.994951 * ACT1 * ICT_USE - 1.124459 * ACT1 * TELEWORK - 0.56222 * ACT1 * AvgFreq - 2.070631 * ACT1 * AvgDur - 0.167278 * ICT_USE * ACT4 + 0.769745 * AvgFreq * ACT4 + 0.487925 * AvgFreq * ACT5;			

				// Get probabilities of each response level
				for (int i = 0; i < 2; i++)
				{
					P_mod[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 0) - xB_mod);
					P_per[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 1) - xB_per);
					P_loc[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 2) - xB_loc);
					P_tim[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 3) - xB_tim);
					P_dur[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 4) - xB_dur);
				}

				// Set correlated random variables
				std::vector<double> rand;
				GLOBALS::Uniform_RNG.Correlated_Rands<double>(rand, Sigma);

				// Set flexibility values
				for (int i = 0; i < 2; i++)
				{
					if (rand[0] < P_mod[i]) { this->_Mode_Flexibility = (Types::FLEXIBILITY_VALUES)i;				rand[0] = 999;}
					if (rand[1] < P_per[i]) { this->_Involved_Persons_Flexibility = (Types::FLEXIBILITY_VALUES)i;	rand[1] = 999; }
					if (rand[2] < P_loc[i]) { this->_Location_Flexibility = (Types::FLEXIBILITY_VALUES)i;			rand[2] = 999; }
					if (rand[3] < P_tim[i]) { this->_Start_Time_Flexibility = (Types::FLEXIBILITY_VALUES)i;			rand[3] = 999; }
					if (rand[4] < P_dur[i]) { this->_Duration_Flexibility = (Types::FLEXIBILITY_VALUES)i;			rand[4] = 999; }
				}
            
				// Set results of flexiblity model to use in next models
				double IFLOC, IFTIM, IFDUR, IFMOD;
				IFMOD = P_mod[0];
				IFLOC = P_loc[0];
				IFTIM = P_tim[0];
				IFDUR = P_dur[0];

				pthis->template Location_Flexibility<Types::FLEXIBILITY_VALUES>(Types::FLEXIBILITY_VALUES::LOW_FLEXIBILITY);

				// allow flexible timing for work at home activities
				if (pthis->Activity_Type<Types::ACTIVITY_TYPES>() == Types::WORK_AT_HOME_ACTIVITY)
				{
					pthis->template Start_Time_Flexibility<Types::FLEXIBILITY_VALUES>(Types::FLEXIBILITY_VALUES::HIGH_FLEXIBILITY);
					pthis->template Duration_Flexibility<Types::FLEXIBILITY_VALUES>(Types::FLEXIBILITY_VALUES::HIGH_FLEXIBILITY);
				}
			}
		};



		// Specialization for ADAPTS-style at-home activities
		implementation struct ADAPTS_At_Home_Activity_Plan_Implementation : public Basic_Activity_Plan_Implementation<MasterType,INHERIT(ADAPTS_At_Home_Activity_Plan_Implementation)>
		{
			typedef Basic_Activity_Plan_Implementation<MasterType,INHERIT(ADAPTS_At_Home_Activity_Plan_Implementation)> base_type;
			typedef typename Basic_Activity_Plan_Implementation<MasterType,INHERIT(ADAPTS_At_Home_Activity_Plan_Implementation)>::Component_Type ComponentType;
			typedef Prototypes::Activity_Planner<ComponentType> this_itf;
		
			member_component_and_feature_accessor(Fixed_Departure, Value, Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>)
			
			//================================================================================================================================================================================================
			//================================================================================================================================================================================================
			// Interfaces
			typedef Person_Components::Prototypes::Person_Properties< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Properties)> _properties_itf;
			typedef Person_Components::Prototypes::Person_Properties< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Static_Properties)> _static_properties_itf;
			typedef Person_Components::Prototypes::Person_Planner<typename type_of(base_type::Parent_Planner)> _planning_itf;
			typedef Person_Components::Prototypes::Person<typename _planning_itf::get_type_of(Parent_Person)> _person_itf;
			typedef Person_Components::Prototypes::Destination_Chooser<typename _planning_itf::get_type_of(Destination_Choice_Faculty)> _dest_choice_itf;
			typedef Scenario_Components::Prototypes::Scenario< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference)> _scenario_itf;
			typedef Network_Components::Prototypes::Network< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(network_reference)> _network_itf;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _network_itf::get_type_of(skimming_faculty)> _skim_itf;
			typedef Person_Components::Prototypes::Activity_Timing_Chooser<typename _planning_itf::get_type_of(Timing_Chooser)> _timing_choice_itf;
			
			typedef Random_Access_Sequence< typename _network_itf::get_type_of(activity_locations_container)> _activity_locations_container_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(_activity_locations_container_itf)>  _activity_location_itf;
				
			typedef Random_Access_Sequence< typename _activity_location_itf::get_type_of(origin_links)> _links_container_itf;
			typedef Link_Components::Prototypes::Link<get_component_type(_links_container_itf)>  _link_itf;
	
			typedef Pair_Associative_Container< typename _network_itf::get_type_of(zones_container)> _zones_container_itf;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_zones_container_itf)>  _zone_itf;

			//================================================================================================================================================================================================
			//================================================================================================================================================================================================

			// Fundamental activity properties
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Activity_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Activity_Plan_Horizon);

			// Activity attribute planning properties
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Location_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Location_Plan_Horizon);
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Mode_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Mode_Plan_Horizon);
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Start_Time_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Start_Time_Plan_Horizon);
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Duration_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Duration_Plan_Horizon);
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Involved_Persons_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Involved_Persons_Plan_Horizon);


			// Activity methods
			template<typename TimeType, typename ModeType> void Initialize(TimeType departure_time, TimeType start_time, TimeType duration, ModeType mode)
			{
				// reasonableness checking
				if(start_time > (END)*2.0 || duration > END)
				{
					THROW_WARNING("Invalid start/duration for at home activity. Start="<<start_time<<", duration="<<duration);
				}

				this_itf* pthis = (this_itf*)this;

				_planning_itf* planner = pthis->template Parent_Planner<_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				
				//pthis->template Activity_Type<Types::ACTIVITY_TYPES>(Types::ACTIVITY_TYPES::AT_HOME_ACTIVITY);
				pthis->template Duration<TimeType>(duration);
				pthis->template Start_Time<TimeType>(start_time);
				pthis->template Location<_activity_location_itf*>(person->template Home_Location<_activity_location_itf*>());
				pthis->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(mode);
				pthis->template Fixed_Departure<TimeType>(departure_time);
				
			}

			template<typename TargetType> void Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType,check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				base_type* base_this = (base_type*)this;

				this->template Start_Time_Planning_Time<  Revision&>()._iteration = END+1;
				this->template Start_Time_Planning_Time<  Revision&>()._sub_iteration = END+1;
				this->template Duration_Planning_Time<  Revision&>()._iteration = END+1;
				this->template Duration_Planning_Time<  Revision&>()._sub_iteration = END+1;
				this->template Location_Planning_Time<  Revision&>()._iteration = END+1;
				this->template Location_Planning_Time<  Revision&>()._sub_iteration = END+1;
				this->template Mode_Planning_Time<  Revision&>()._iteration = END+1;
				this->template Mode_Planning_Time<  Revision&>()._sub_iteration = END+1;
				this->template Involved_Persons_Planning_Time<  Revision&>()._iteration = END+1;
				this->template Involved_Persons_Planning_Time<  Revision&>()._sub_iteration = END+1;
				this->template Route_Planning_Time<  Revision&>()._iteration = iteration()+1;
				this->template Route_Planning_Time<  Revision&>()._sub_iteration = Scenario_Components::Types::ACTIVITY_ATTRIBUTE_PLANNING_SUB_ITERATION;
			}
			template<typename TargetType> void Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType,!check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				assert_check_2(TargetType, Simulation_Timestep_Increment, is_same, "Error: planning_time must be passed as a Simulation_Timestep_Increment type when using this function.");
			}

			template<typename TargetType> void Location_Planning_Event_Handler(){}
			template<typename TargetType> void Mode_Planning_Event_Handler(){}
			template<typename TargetType> void Duration_Planning_Event_Handler(){}
			template<typename TargetType> void Start_Time_Planning_Event_Handler(){}
			template<typename TargetType> void Involved_Persons_Planning_Event_Handler(){}

			template<typename TargetType> void Set_Meta_Attributes()
			{
				base_type* base_this = (base_type*)this;
				this->_Duration_Flexibility = Types::FLEXIBILITY_VALUES::HIGH_FLEXIBILITY;
				this->_Location_Flexibility = Types::FLEXIBILITY_VALUES::HIGH_FLEXIBILITY;
				this->_Mode_Flexibility = Types::FLEXIBILITY_VALUES::HIGH_FLEXIBILITY;
				this->_Start_Time_Flexibility = Types::FLEXIBILITY_VALUES::HIGH_FLEXIBILITY;
				this->_Involved_Persons_Flexibility = Types::FLEXIBILITY_VALUES::HIGH_FLEXIBILITY;
		
				this->_Duration_Planning_Time = Types::PLAN_HORIZON_VALUES::IMPULSIVE;
				this->_Location_Planning_Time = Types::PLAN_HORIZON_VALUES::IMPULSIVE;
				this->_Mode_Planning_Time = Types::PLAN_HORIZON_VALUES::IMPULSIVE;
				this->_Start_Time_Planning_Time = Types::PLAN_HORIZON_VALUES::IMPULSIVE;
				this->_Involved_Persons_Planning_Time = Types::PLAN_HORIZON_VALUES::IMPULSIVE;
			}
		};



		// Stripped down activity record with minimal memory usage (used for storing and printing completed activities)
		implementation struct Activity_Record : public Polaris_Component< MasterType,INHERIT(Activity_Record), Data_Object>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Activity_Record),Data_Object>::Component_Type ComponentType;

			template<typename TargetType> void Initialize(TargetType object/*,requires(TargetType,check(strip_modifiers(TargetType), Concepts::Is_Activity_Plan_Prototype))*/)
			{
				if (object == nullptr) {cout <<"Warning: cannot initialize activity record from a null activity."; return;}

				typedef Activity_Location_Components::Prototypes::Activity_Location<typename MasterType::activity_location_type> location_itf;
				typedef Zone_Components::Prototypes::Zone<typename MasterType::zone_type> zone_itf;
				typedef Prototypes::Activity_Planner<typename MasterType::activity_type> object_itf;
				object_itf* obj = (object_itf*)object;

				this->_Activity_Plan_ID = obj->Activity_Plan_ID<char>();
				this->_Activity_Type = (char)obj->Activity_Type<Types::ACTIVITY_TYPES>();
				this->Location<location_itf*>(obj->Location<location_itf*>());
				this->_Start_Time = obj->Start_Time<Time_Minutes>();
				this->_Duration = obj->Duration<Time_Minutes>();
			}

			// Fundamental activity properties
			m_data(char, Activity_Plan_ID, NONE, NONE);
			m_data(char, Activity_Type, NONE, NONE);
			m_prototype(Activity_Location_Components::Prototypes::Activity_Location, typename MasterType::activity_location_type, Location, check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location), check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location));
			m_data(Time_Minutes, Start_Time, check_2(TargetType,Time_Minutes, is_same),check_2(TargetType,Time_Minutes, is_same));
			m_data(Time_Minutes, Duration, check_2(TargetType,Time_Minutes, is_same),check_2(TargetType,Time_Minutes, is_same));
			m_data(Time_Minutes, Travel_Time, check_2(TargetType,Time_Minutes, is_same),check_2(TargetType,Time_Minutes, is_same));
		};



		//=======================================================================
		// Static activity plan member initialization
		//-----------------------------------------------------------------------
		template<typename MasterType, typename InheritanceList> int Basic_Activity_Plan_Implementation<MasterType,InheritanceList>::_Route_Planning_Count=0;
	}
}


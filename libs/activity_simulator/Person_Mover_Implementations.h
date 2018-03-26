#pragma once

#include "Person_Mover_Prototype.h"

namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Person Agent classes
		//----------------------------------------------------------------------------------
		implementation struct Person_Mover_Implementation : public Polaris_Component<MasterType,INHERIT(Person_Mover_Implementation),Execution_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Person_Mover_Implementation),Execution_Object>::Component_Type ComponentType;
			typedef typename MasterType::base_network_event_type base_network_event_type;
			typedef typename MasterType::weather_network_event_type weather_network_event_type;
			typedef typename MasterType::accident_network_event_type accident_network_event_type;

			Person_Mover_Implementation()
			{
				_Movement_Scheduled=false;
				_Artificial_Movement_Scheduled=false;
				_Replanning_Needed=false;
				_Is_Moving = false;
			}

			m_prototype(Prototypes::Person,typename MasterType::person_type,Parent_Person, NONE, NONE);
			m_prototype(Movement_Plan_Components::Prototypes::Movement_Plan,typename MasterType::integrated_movement_plan_type,Movement, NONE, NONE);
			m_data(bool, Movement_Scheduled, NONE, NONE);
			m_data(bool, Is_Moving, NONE, NONE);
			m_data(bool, Artificial_Movement_Scheduled, NONE, NONE);
			m_data(Simulation_Timestep_Increment, Artificial_Arrival_Time, NONE, NONE);
			m_data(bool, Replanning_Needed, NONE, NONE);

			//TODO: Omer
			member_component_and_feature_accessor(Next_Simulation_Time, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);
			m_data(int, Next_Sub_Iteration, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			template<typename TargetType> void person_boarded_transit_vehicle(int next_simulation_time);
		};

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Person_Mover_Implementation<MasterType, InheritanceList>::person_boarded_transit_vehicle(int next_simulation_time)
		{
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef  Movement_Plan_Components::Prototypes::Multimodal_Trajectory_Unit<typename remove_pointer< typename movement_itf::get_type_of(multimodal_trajectory_container)::value_type>::type>  _Multimodal_Trajectory_Unit_Interface;
			typedef  Random_Access_Sequence< typename movement_itf::get_type_of(multimodal_trajectory_container), _Multimodal_Trajectory_Unit_Interface*> _Multimodal_Trajectory_Container_Interface;
			typedef Link_Components::Prototypes::Link< typename _Multimodal_Trajectory_Unit_Interface::get_type_of(link)> _Link_Interface;
			typedef Intersection_Components::Prototypes::Intersection<typename _Link_Interface::get_type_of(upstream_intersection)>  _Intersection_Interface;
			typedef Prototypes::Person< typename get_type_of(Parent_Person)> person_itf;
			typedef typename _Multimodal_Trajectory_Unit_Interface::get_type_of(transit_vehicle_trip) _Transit_Vehicle_Trip_Interface;
			typedef Transit_Pattern_Components::Prototypes::Transit_Pattern<typename MasterType::transit_pattern_type> _Transit_Pattern_Interface;
			typedef  typename _Transit_Pattern_Interface::get_type_of(pattern_links) _Pattern_Links_Container_Interface;
			typedef Scenario_Components::Prototypes::Scenario< typename person_itf::get_type_of(scenario_reference)> scenario_itf;
			stringstream trajectory_stream;

			int cur_iter = iteration();
			int cur_sub = sub_iteration();

			//Get the person
			person_itf* person = this->Parent_Person<person_itf*>();
			//Get their status
			Person_Components::Types::Movement_Status_Keys current_status = person->template simulation_status<Person_Components::Types::Movement_Status_Keys>();
			//Get the movement plan
			movement_itf* movement = this->Movement<movement_itf*>();
			//Get the planned trajectory
			_Multimodal_Trajectory_Container_Interface& trajectory = movement->template multimodal_trajectory_container<_Multimodal_Trajectory_Container_Interface&>();
			//Get the person's position along the trajectory
			int current_position = movement->template current_multimodal_trajectory_position<int>();
			//Get the trajectory members at that position
			_Multimodal_Trajectory_Unit_Interface* trajectory_unit = (_Multimodal_Trajectory_Unit_Interface*)trajectory[current_position];
			// Get the transit vehicle trip
			_Transit_Vehicle_Trip_Interface* transit_vehicle_trip = trajectory_unit->template transit_vehicle_trip<_Transit_Vehicle_Trip_Interface*>();
			//Get the link at that position
			_Link_Interface* link = trajectory_unit->template link<_Link_Interface*>();
			//Get the link mode
			Link_Components::Types::Link_Type_Keys link_type = link->template link_type<Link_Components::Types::Link_Type_Keys>();

			//Set the person's current vehicle to the boarded vehicle
			person->template current_transit_vehicle<_Transit_Vehicle_Trip_Interface*>(transit_vehicle_trip);
			//int next_simulation_time = iteration();
			int next_sub = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRAVELER_MOVING_SUBITERATION;

			//Set the values
			this->template Next_Simulation_Time<Simulation_Timestep_Increment>(next_simulation_time);
			this->template Next_Sub_Iteration<int>(next_sub);

			//Calculate the waiting time as departure time minus arrival time
			float wait_time = next_simulation_time - trajectory_unit->template actual_arrival_time<float>();
			//Set the waiting time
			trajectory_unit->template actual_wait_time<float>(wait_time);

			//To set the actual experienced values
			if (current_position > 0)
			{
				//Get the previous trajectory position
				int previous_position = current_position - 1;
				//Get the previous trajectory unit
				_Multimodal_Trajectory_Unit_Interface* previous_trajectory_unit = (_Multimodal_Trajectory_Unit_Interface*)trajectory[previous_position];
				//Get the previous waiting count
				int wait_count = previous_trajectory_unit->template actual_wait_count<int>();
				//Augment the waiting count
				wait_count++;
				//Set the current waiting_count;
				trajectory_unit->template actual_wait_count<int>(wait_count);
				//Get the base transfer penalty
				float transferPenalty = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::transferPenalty<float>();
				//Get the previous transfer penalty
				float previousTransferPen = previous_trajectory_unit->template actual_transfer_penalty<float>();
				//Calculate the transfer penalty
				float effectiveTransferPen = std::max(wait_count - 1, 0) * transferPenalty;
				//Set the actual transfer penalty
				trajectory_unit->template actual_transfer_penalty<float>(previousTransferPen + effectiveTransferPen);
				//Get the waiting weight
				float waitWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::waitWeight<float>();
				//Augment the generalized cost
				float gen_cost = trajectory_unit->template actual_gen_cost<float>() + waitWeight * wait_time + effectiveTransferPen;
				//Set the generalized cost
				trajectory_unit->template actual_gen_cost<float>(gen_cost);
			}

			trajectory_stream << "I am person:\t" << person->template uuid<int>() << "\t";
			trajectory_stream << "At position:\t" << current_position << "\t";
			trajectory_stream << "Time is:\t" << trajectory_unit->estimated_arrival_time<int>() << "\t";
			trajectory_stream << "Simulation time is:\t" << cur_iter << "\t";
			trajectory_stream << "Subiteration is:\t" << cur_sub << "\t";
			trajectory_stream << "Mode is:\t" << link_type << "\t";
			trajectory_stream << "Node is:\t" << link->template upstream_intersection<_Intersection_Interface*>()->template dbid<std::string>() << "\t";
			trajectory_stream << "I have boarded trip:\t" << transit_vehicle_trip->template dbid<std::string>() << endl;

			//Schedule departure event for person
			static_cast<ComponentType*>(this)->template Load_Event<ComponentType>(&move_persons_in_multimodal_network_conditional, next_simulation_time, next_sub);

			if ((int)num_sim_threads() == 1)
			{
				fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
			}
		}

	}
}

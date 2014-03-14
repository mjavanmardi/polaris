#pragma once

#include "Mode_Chooser_Prototype.h"
#include "Person_Prototype.h"

namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Planning classes
		//----------------------------------------------------------------------------------

		implementation struct Mode_Choice_Option : public Choice_Model_Components::Implementations::Choice_Option_Base<MasterType,INHERIT(Mode_Choice_Option)>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Mode_Choice_Option),Data_Object>::Component_Type ComponentType;


			// data members
			m_prototype(Prototypes::Person_Planner, typename MasterType::person_planner_type, Parent_Planner, NONE, NONE);
			m_prototype(Activity_Components::Prototypes::Activity_Planner, typename MasterType::activity_type, current_activity, NONE, NONE );
			m_prototype(Activity_Components::Prototypes::Activity_Planner, typename MasterType::activity_type, previous_activity, NONE, NONE );
			m_prototype(Activity_Components::Prototypes::Activity_Planner, typename MasterType::activity_type, next_activity, NONE, NONE );
			m_prototype(Activity_Location_Components::Prototypes::Activity_Location, typename MasterType::activity_location_type, destination, NONE, NONE );
			m_prototype(Activity_Location_Components::Prototypes::Activity_Location, typename MasterType::activity_location_type, previous_location, NONE, NONE );
			m_prototype(Activity_Location_Components::Prototypes::Activity_Location, typename MasterType::activity_location_type, next_location, NONE, NONE );
			m_data(Vehicle_Components::Types::Vehicle_Type_Keys, mode_type, NONE, NONE);
			m_data(bool, home_based, NONE, NONE);
			m_data(bool, auto_available, NONE, NONE);
			m_data(bool, to_CBD, NONE, NONE);

			// PARAMETER DECLARATIONS
			static m_data(float, BHW_IVTT, NONE, NONE);
			static m_data(float, BHW_CBD_IVTT, NONE, NONE);
			static m_data(float, BHO_IVTT, NONE, NONE);
			static m_data(float, BHO_CBD_IVTT, NONE, NONE);
			static m_data(float, BNH_IVTT, NONE, NONE);
			static m_data(float, BNH_CBD_IVTT, NONE, NONE);

			static m_data(float, BHW_WAIT, NONE, NONE);
			static m_data(float, BHW_CBD_WAIT, NONE, NONE);
			static m_data(float, BHO_WAIT, NONE, NONE);
			static m_data(float, BHO_CBD_WAIT, NONE, NONE);
			static m_data(float, BNH_WAIT, NONE, NONE);
			static m_data(float, BNH_CBD_WAIT, NONE, NONE);
			
			static m_data(float, BHW_TRANSFER, NONE, NONE);
			static m_data(float, BHW_CBD_TRANSFER, NONE, NONE);
			static m_data(float, BHO_TRANSFER, NONE, NONE);
			static m_data(float, BHO_CBD_TRANSFER, NONE, NONE);
			static m_data(float, BNH_TRANSFER, NONE, NONE);
			static m_data(float, BNH_CBD_TRANSFER, NONE, NONE);
			
			static m_data(float, BHW_WALK, NONE, NONE);
			static m_data(float, BHW_CBD_WALK, NONE, NONE);
			static m_data(float, BHO_WALK, NONE, NONE);
			static m_data(float, BHO_CBD_WALK, NONE, NONE);
			static m_data(float, BNH_WALK, NONE, NONE);
			static m_data(float, BNH_CBD_WALK, NONE, NONE);
			
			static m_data(float, BHW_COST, NONE, NONE);
			static m_data(float, BHW_CBD_COST, NONE, NONE);
			static m_data(float, BHO_COST, NONE, NONE);
			static m_data(float, BHO_CBD_COST, NONE, NONE);
			static m_data(float, BNH_COST, NONE, NONE);
			static m_data(float, BNH_CBD_COST, NONE, NONE);
			
			static m_data(float, BHW_BIAS, NONE, NONE);
			static m_data(float, BHW_CBD_BIAS, NONE, NONE);
			static m_data(float, BHO_BIAS, NONE, NONE);
			static m_data(float, BHO_CBD_BIAS, NONE, NONE);
			static m_data(float, BNH_BIAS, NONE, NONE);
			static m_data(float, BNH_CBD_BIAS, NONE, NONE);


			//====================================================================================================================================
			// Interface definitions
			typedef Prototypes::Person<typename type_of(Parent_Planner)::type_of(Parent_Person)> person_itf;
			typedef Prototypes::Person_Scheduler<typename person_itf::get_type_of(Scheduling_Faculty)> scheduler_itf;
			typedef Scenario_Components::Prototypes::Scenario< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference)> _Scenario_Interface;
			typedef Network_Components::Prototypes::Network< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference)> _Network_Interface;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _Network_Interface::get_type_of(skimming_faculty)> _Skim_Interface;
			
			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(_Activity_Locations_Container_Interface)>  _Activity_Location_Interface;	
				
			typedef Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links)> _Links_Container_Interface;
			typedef Link_Components::Prototypes::Link<typename get_component_type(_Links_Container_Interface)>  _Link_Interface;
	
			typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
			typedef Zone_Components::Prototypes::Zone<typename get_mapped_component_type(_Zones_Container_Interface)>  _Zone_Interface;

			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Activity_Container)> Activity_Plans;
			typedef Activity_Components::Prototypes::Activity_Planner<typename get_component_type(Activity_Plans)> Activity_Plan;
			
			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Movement_Plans_Container)> Movement_Plans;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename get_component_type(Movement_Plans)> Movement_Plan;

			
			typedef Network_Skimming_Components::Prototypes::LOS<typename MasterType::los_value_type> los_itf;
			//------------------------------------------------------------------------------------------------------------------------------------

			// Feature called from prototype and by Choice_Model
			virtual double Calculate_Utility()
			{
				double utility;

				// Since the model is bimodal, treat SOV as the reference class
				if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::SOV)
				{
					utility = 0.0;
				}
				else
				{
					// Determine if the trip is from home
					person_itf* _Parent_Person = _Parent_Planner->Parent_Person<person_itf*>();
					if (_previous_location == _Parent_Person->Home_Location<_Activity_Location_Interface*>()) _home_based=true;
					else _home_based = false;

					// Determine if the trip destination is known, and if it is destined for the CBD
					if (this->_current_activity->Location_Is_Planned<bool>())
					{
						_Zone_Interface* dest_zone = this->_destination->zone<_Zone_Interface*>();
						if (dest_zone->uuid<int>() < 88) _to_CBD = true;
						else _to_CBD = false;
						
						utility = Calculate_Utility_For_Known_Location<double>();
					}
					else 
					{
						_to_CBD = false;
						utility = Calculate_Utility_For_Unknown_Location<double>();
					}
				}
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
				

				//cout << "O/D=unknown:"<<_previous_location->zone<_Zone_Interface*>()->uuid<int>() <<"/xxx";
				//cout << ",Auto_TT:"<<origin_zone->avg_ttime_auto_to_transit_accessible_zones<Time_Minutes>()<< ",Transit_TT:"<<origin_zone->avg_ttime_transit<Time_Minutes>();
				//cout <<",Transit_wait:" << -1.0* wait_dif << ",transfer_time:"<<0;
				//cout <<",Auto_cost:" << 0;
				//cout <<",Transit_fare:" << 0 << ",utility:"<< utility << endl;

				// Otherwise return the transit utility based on Zonal average characteristics
				return utility;			
			}
			//template<typename TargetType> TargetType Calculate_Utility_Value(Activity_Components::Types::ACTIVITY_TYPES activity_type, los_itf* LOS)
			//{
			//	float utility;
			//	float ivtt_dif = LOS->auto_ttime<Time_Minutes>() - LOS->transit_ttime<Time_Minutes>();
			//	float wait_dif = -1.0 * LOS->transit_wait_time<Time_Minutes>();
			//	float transfer_dif = 0;
			//	float walk_time_dif = -1.0 * LOS->transit_walk_access_time<Time_Minutes>();
			//	float cost_dif = LOS->auto_distance<Miles>()*15.0 + LOS->auto_parking_cost<Cents>() + LOS->auto_tolls<Cents>() - LOS->transit_fare<Cents>();

			//	// Split mode choice model by in/out of CBD for Home-based Work, Home-based other and non-home based
			//	if (_to_CBD)
			//	{
			//		if (_home_based)
			//		{
			//			if (activity_type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY || activity_type == Activity_Components::Types::ACTIVITY_TYPES::OTHER_WORK_ACTIVITY)
			//			{
			//				utility = ivtt_dif * this->_BHW_CBD_IVTT + wait_dif * this->_BHW_CBD_WAIT + transfer_dif * this->_BHW_CBD_TRANSFER + walk_time_dif * this->_BHW_CBD_WALK + cost_dif * this->_BHW_CBD_COST + this->_BHW_CBD_BIAS;
			//			}
			//			else
			//			{
			//				utility = ivtt_dif * this->_BHO_CBD_IVTT + wait_dif * this->_BHO_CBD_WAIT + transfer_dif * this->_BHO_CBD_TRANSFER + walk_time_dif * this->_BHO_CBD_WALK + cost_dif * this->_BHO_CBD_COST + this->_BHO_CBD_BIAS;
			//			}
			//		}
			//		else
			//		{
			//			utility = ivtt_dif * this->_BNH_CBD_IVTT + wait_dif * this->_BNH_CBD_WAIT + transfer_dif * this->_BNH_CBD_TRANSFER + walk_time_dif * this->_BNH_CBD_WALK + cost_dif * this->_BNH_CBD_COST + this->_BNH_CBD_BIAS;
			//		}
			//	}
			//	else
			//	{
			//		if (_home_based)
			//		{
			//			if (activity_type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY || activity_type == Activity_Components::Types::ACTIVITY_TYPES::OTHER_WORK_ACTIVITY)
			//			{
			//				utility = ivtt_dif * this->_BHW_IVTT + wait_dif * this->_BHW_WAIT + transfer_dif * this->_BHW_TRANSFER + walk_time_dif * this->_BHW_WALK + cost_dif * this->_BHW_COST + this->_BHW_BIAS;
			//			}
			//			else
			//			{
			//				utility = ivtt_dif * this->_BHO_IVTT + wait_dif * this->_BHO_WAIT + transfer_dif * this->_BHO_TRANSFER + walk_time_dif * this->_BHO_WALK + cost_dif * this->_BHO_COST + this->_BHO_BIAS;
			//			}
			//		}
			//		else
			//		{
			//			utility = ivtt_dif * this->_BNH_IVTT + wait_dif * this->_BNH_WAIT + transfer_dif * this->_BNH_TRANSFER + walk_time_dif * this->_BNH_WALK + cost_dif * this->_BNH_COST + this->_BNH_BIAS;
			//		}
			//	}
			//	return (TargetType)utility;
			//}
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

		// INITIALIZE DESTINATION MODEL STATIC PARAMETERS
		#pragma region Choice option parameters	
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHW_IVTT) Mode_Choice_Option<MasterType,InheritanceList>::_BHW_IVTT = 0.0186;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHW_CBD_IVTT) Mode_Choice_Option<MasterType,InheritanceList>::_BHW_CBD_IVTT = 0.0159;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHO_IVTT) Mode_Choice_Option<MasterType,InheritanceList>::_BHO_IVTT = 0.0114;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHO_CBD_IVTT) Mode_Choice_Option<MasterType,InheritanceList>::_BHO_CBD_IVTT = 0.0159;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BNH_IVTT) Mode_Choice_Option<MasterType,InheritanceList>::_BNH_IVTT = 0.0114;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BNH_CBD_IVTT) Mode_Choice_Option<MasterType,InheritanceList>::_BNH_CBD_IVTT = 0.0159;

		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHW_WAIT) Mode_Choice_Option<MasterType,InheritanceList>::_BHW_WAIT = 0.0811;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHW_CBD_WAIT) Mode_Choice_Option<MasterType,InheritanceList>::_BHW_CBD_WAIT = 0.0173;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHO_WAIT) Mode_Choice_Option<MasterType,InheritanceList>::_BHO_WAIT = 0.061;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHO_CBD_WAIT) Mode_Choice_Option<MasterType,InheritanceList>::_BHO_CBD_WAIT = 0.0173;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BNH_WAIT) Mode_Choice_Option<MasterType,InheritanceList>::_BNH_WAIT = 0.061;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BNH_CBD_WAIT) Mode_Choice_Option<MasterType,InheritanceList>::_BNH_CBD_WAIT = 0.0173;

		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHW_TRANSFER) Mode_Choice_Option<MasterType,InheritanceList>::_BHW_TRANSFER = 0.0399;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHW_CBD_TRANSFER) Mode_Choice_Option<MasterType,InheritanceList>::_BHW_CBD_TRANSFER = 0.029;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHO_TRANSFER) Mode_Choice_Option<MasterType,InheritanceList>::_BHO_TRANSFER = 0.0589;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHO_CBD_TRANSFER) Mode_Choice_Option<MasterType,InheritanceList>::_BHO_CBD_TRANSFER = 0.029;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BNH_TRANSFER) Mode_Choice_Option<MasterType,InheritanceList>::_BNH_TRANSFER = 0.0589;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BNH_CBD_TRANSFER) Mode_Choice_Option<MasterType,InheritanceList>::_BNH_CBD_TRANSFER = 0.029;

		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHW_WALK) Mode_Choice_Option<MasterType,InheritanceList>::_BHW_WALK = 0.0584;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHW_CBD_WALK) Mode_Choice_Option<MasterType,InheritanceList>::_BHW_CBD_WALK = 0.0486;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHO_WALK) Mode_Choice_Option<MasterType,InheritanceList>::_BHO_WALK = 0.0663;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHO_CBD_WALK) Mode_Choice_Option<MasterType,InheritanceList>::_BHO_CBD_WALK = 0.0486;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BNH_WALK) Mode_Choice_Option<MasterType,InheritanceList>::_BNH_WALK = 0.0663;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BNH_CBD_WALK) Mode_Choice_Option<MasterType,InheritanceList>::_BNH_CBD_WALK = 0.0486;

		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHW_COST) Mode_Choice_Option<MasterType,InheritanceList>::_BHW_COST = 0.00119;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHW_CBD_COST) Mode_Choice_Option<MasterType,InheritanceList>::_BHW_CBD_COST = 0.00141;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHO_COST) Mode_Choice_Option<MasterType,InheritanceList>::_BHO_COST = 0.00546;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHO_CBD_COST) Mode_Choice_Option<MasterType,InheritanceList>::_BHO_CBD_COST = 0.00141;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BNH_COST) Mode_Choice_Option<MasterType,InheritanceList>::_BNH_COST = 0.00546;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BNH_CBD_COST) Mode_Choice_Option<MasterType,InheritanceList>::_BNH_CBD_COST = 0.00141;

		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHW_BIAS) Mode_Choice_Option<MasterType,InheritanceList>::_BHW_BIAS = -2;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHW_CBD_BIAS) Mode_Choice_Option<MasterType,InheritanceList>::_BHW_CBD_BIAS = -1;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHO_BIAS) Mode_Choice_Option<MasterType,InheritanceList>::_BHO_BIAS = -1.9;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BHO_CBD_BIAS) Mode_Choice_Option<MasterType,InheritanceList>::_BHO_CBD_BIAS = -1;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BNH_BIAS) Mode_Choice_Option<MasterType,InheritanceList>::_BNH_BIAS = -2;
		template<typename MasterType, typename InheritanceList> typename Mode_Choice_Option<MasterType, InheritanceList>::type_of(BNH_CBD_BIAS) Mode_Choice_Option<MasterType,InheritanceList>::_BNH_CBD_BIAS = -1;
		#pragma endregion


		//implementation struct Mode_Choice_Model_Implementation : public Choice_Model_Components::Implementations::MNL_Model_Implementation<MasterType, INHERIT(Mode_Choice_Model_Implementation)>
		//{
		//	typedef Choice_Model_Components::Implementations::MNL_Model_Implementation<MasterType, INHERIT(Mode_Choice_Model_Implementation)> BaseType;
		//	typedef typename BaseType::Component_Type ComponentType;
		//	typedef TypeList<Prototypes::Mode_Choice_Option<typename MasterType::mode_choice_option_type >> TList;
		//};



		implementation struct Mode_Chooser_Implementation : public Polaris_Component<MasterType,INHERIT(Mode_Chooser_Implementation),Data_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Mode_Chooser_Implementation),Data_Object>::Component_Type ComponentType;

			// Pointer to the Parent class
			m_prototype(Prototypes::Person_Planner, typename MasterType::person_planner_type, Parent_Planner, NONE, NONE);
			m_prototype(Choice_Model_Components::Prototypes::Choice_Model, typename MasterType::mnl_model_type, Choice_Model, NONE, NONE);
			
			static m_data(int, choice_set_size, NONE, NONE);

			// Interface definitions	
			typedef Choice_Model_Components::Prototypes::Choice_Model<typename MasterType::mnl_model_type > _Choice_Model_Interface;
			typedef Prototypes::Mode_Choice_Option<typename MasterType::mode_choice_option_type> _Mode_Choice_Option_Interface;
			typedef Choice_Model_Components::Prototypes::Choice_Option<typename MasterType::mode_choice_option_type> _Choice_Option_Interface;

			typedef Prototypes::Person<typename type_of(Parent_Planner)::type_of(Parent_Person)> person_itf;
			typedef Prototypes::Person_Properties<typename person_itf::get_type_of(Static_Properties)> person_static_properties_itf;
			typedef Household_Components::Prototypes::Household<typename person_itf::get_type_of(Household)> household_itf;
			typedef Household_Components::Prototypes::Household_Properties<typename household_itf::get_type_of(Static_Properties)> household_static_properties_itf;
			typedef Prototypes::Person_Scheduler<typename person_itf::get_type_of(Scheduling_Faculty)> scheduler_itf;
			typedef Scenario_Components::Prototypes::Scenario< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference)> _Scenario_Interface;
			typedef Network_Components::Prototypes::Network< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference)> _Network_Interface;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _Network_Interface::get_type_of(skimming_faculty)> _Skim_Interface;
			
			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(_Activity_Locations_Container_Interface)>  _Activity_Location_Interface;	
				
			typedef Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links)> _Links_Container_Interface;
			typedef Link_Components::Prototypes::Link<typename get_component_type(_Links_Container_Interface)>  _Link_Interface;
	
			typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
			typedef Zone_Components::Prototypes::Zone<typename get_mapped_component_type(_Zones_Container_Interface)>  _Zone_Interface;

			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(zone_ids_container),int> _Zone_Ids_Interface;

			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Activity_Container)> Activity_Plans;
			typedef Activity_Components::Prototypes::Activity_Planner<typename get_component_type(Activity_Plans)> Activity_Plan;
			
			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Movement_Plans_Container)> Movement_Plans;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename get_component_type(Movement_Plans)> Movement_Plan;


			template<typename TargetType> void Initialize(/*requires(TargetType,check(typename ComponentType::Parent_Type,Concepts::Is_Person))*/)
			{	
			}
			/*template<typename TargetType> void Initialize(requires(TargetType,check(typename ComponentType::Parent_Type,!Concepts::Is_Person)))
			{	
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Initialize_Defined, "The specified ParentType is not a valid Person type.");
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Properties_Defined, "The specified ParentType does not have the required Properties member defined.");
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Planner_Defined, "The specified ParentType does not have the required Planner member defined.");
			}*/

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
				/*Mode_Choice_Model_Implementation<MasterType> a;
				_Choice_Model_Interface* choice_model = (_Choice_Model_Interface*)&a;*/
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
					next_act = _Parent_Person->next_activity_plan<Time_Seconds,Activity_Plan*>(cur_act->Start_Time<Time_Seconds>());
					next_location = scheduler->next_location<Activity_Plan*,_Activity_Location_Interface*>(cur_act);
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
					dest_location = nullptr;
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
				else selected_mode = ((_Choice_Option_Interface*)selected)->mode_type<ReturnType>();

				// free memory allocated locally
				for (int i = 0; i < mode_options.size(); i++) Free<typename _Choice_Option_Interface::Component_Type>((typename _Choice_Option_Interface::Component_Type*)mode_options[i]);
				Free<typename MasterType::mnl_model_type>(choice_model);

				// return the chosen mode
				return selected_mode;
			}
		};


		#pragma region Choice option parameters
		// INITIALIZE DESTINATION MODEL STATIC PARAMETERS
		template<typename MasterType, typename InheritanceList> typename Mode_Chooser_Implementation<MasterType, InheritanceList>::type_of(choice_set_size) Mode_Chooser_Implementation<MasterType,InheritanceList>::_choice_set_size;		
		#pragma endregion

	}
}


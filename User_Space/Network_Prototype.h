#pragma once
#include <hash_map>
#include "Activity_Location_Prototype.h"
//using namespace Basic_Units::Data_Structures;
//using namespace Signal_Components::Data_Structures;

/*
 * Needed for Linux compatability. Do not hurt Windows compilation.
 */
namespace Scenario_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Scenario_Prototype;
	}
};

namespace Activity_Location_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Activity_Location_Prototype;
	}
};
namespace Link_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Link_Prototype;
	}
};
namespace Vehicle_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Vehicle_Prototype;
	}
};
namespace Routing_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Routable_Network_Prototype;
	}
};
namespace Intersection_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Intersection_Prototype;
		forward_declare_prototype struct Movement_Prototype;
		forward_declare_prototype struct Outbound_Inbound_Movements_Prototype;
		forward_declare_prototype struct Inbound_Outbound_Movements_Prototype;
	}
};
/*------------------------------------------------*/

namespace Network_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Network_Prototype
		{
			tag_as_prototype;

			feature_accessor(max_free_flow_speed, none, none);
			feature_accessor(intersections_container, none, none);
			feature_accessor(links_container, none, none);
			feature_accessor(turn_movements_container, none, none);
			feature_accessor(activity_locations_container, none, none);
			feature_accessor(routable_network, none, none);

			feature_accessor(routable_networks_container, none, none);

			feature_prototype void read_network_data()
			{
//				using namespace Signal_Components::Data_Structures;


				int i = 0;
				float link_length = 5280.0;
				float speed_limit = 60.0;
				float speed_limit_ramp = 30.0;
				float maximum_flow_rate = 2200.0;
				float maximum_flow_rate_ramp = 600.0;
				float maximum_flow_rate_arterial = 900;
				float jam_density = 220.0;
				float backward_wave_speed = 12.0;
				float distance_factor = 1.5;
				
				max_free_flow_speed<int>(-1);

				//Bo: to be uncommented for signal
				//typedef typename ComponentType::Master_Type::SIGNAL_TYPE Signal_Type;
				//typedef typename Signal_Components::Prototypes::Signal_Prototype<Signal_Type,NULLTYPE> Signal_Interface;
				//typedef typename ComponentType::Master_Type::INDICATOR_TYPE Indicator_Type;
				//typedef typename Signal_Components::Prototypes::Signal_Indicator_Prototype<Indicator_Type,NULLTYPE> Indicator_Interface;

				//define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype,Signal_Type::Phases,phases_itf,Signal_Components::Prototypes::Phase_Prototype,Phase_Interface,ComponentType);
				//define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype,Signal_Type::Approaches,approaches_itf,Signal_Components::Prototypes::Approach_Prototype,Approach_Interface,ComponentType);
				//define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype,Phase_Interface_type::Lane_Groups,lane_groups_itf,Signal_Components::Prototypes::Lane_Group_Prototype,Lane_Group_Interface,ComponentType);
				define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				//define_component_interface(Detector_Interface,Signal_Components::Prototypes::Detector_Prototype,Lane_Group_Interface_type::Detector_Left,ComponentType);
				intersections_container<_Intersections_Container_Interface&>().clear();

				i = 0;
				
				_Intersection_Interface* intersection_0=(_Intersection_Interface*)Allocate<typename _Intersection_Interface::Component_Type>();
				intersection_0->template uuid<int>(0);
				intersection_0->template x_position<float>(0.0);
				intersection_0->template y_position<float>((float) 2.0*link_length);
				intersection_0->template intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections_container<_Intersections_Container_Interface&>().push_back(intersection_0);

				i = 1;

				_Intersection_Interface* intersection_1=(_Intersection_Interface*)Allocate<typename _Intersection_Interface::Component_Type>();
				intersection_1->template uuid<int>(1);
				intersection_1->template x_position<float>(i * link_length);
				intersection_1->template y_position<float>((float) 2.0*link_length);
				intersection_1->template intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections_container<_Intersections_Container_Interface&>().push_back(intersection_1);

				i = 2;

				_Intersection_Interface* intersection_2=(_Intersection_Interface*)Allocate<typename _Intersection_Interface::Component_Type>();
				intersection_2->template uuid<int>(2);
				intersection_2->template x_position<float>(i * link_length);
				intersection_2->template y_position<float>((float) 2.0*link_length);
				intersection_2->template intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections_container<_Intersections_Container_Interface&>().push_back(intersection_2);
				
				//Bo: to be uncommented for signal
				// Signal at intersection 2
				//Signal_Interface* signal = (Signal_Interface*)Allocate<typename Signal_Type>();
				//signal->template Initialize<int>(2,2);
				//signal->template Signal_ID<int>(2);
				//signal->template name<char*>("Signal_1");
				//signal->template output_stream<ostream*>(stream_ptr);
				//signal->template in_CBD<bool>(false);
				//signal->template analysis_period<Time_Minutes>(15.0);
				//signal->template degree_of_saturation<float>(0.9);
				//signal->template peak_hour_factor<float>(0.95);
				//signal->template max_cycle_length<Time_Seconds>(100.0);
				//signal->template min_cycle_length<Time_Seconds>(20.0);
				//signal->template num_cycles_between_updates<int>(5);

				//// signal indicator
				//Indicator_Interface* indicator = (Indicator_Interface*)Allocate<Indicator_Type>();
				//indicator->template Initialize<NULLTYPE>();
				//indicator->template Signal<Signal_Interface*>(signal);

				//
				//phases_itf* phases = signal->template Phases<phases_itf*>();
				//approaches_itf* approaches = signal->template Approaches<approaches_itf*>();
				//Phase_Interface *phase;
				//Lane_Group_Interface *lane, *lane2;

				//// Initialize phase 1
				//phase = (*phases)[0];
				//phase->template Initialize<int>(1);
				//phase->template phase_id<int>(1);
				//phase->template name<char*>("EB-Thru");
				//phase->template yellow_and_all_red_time<Time_Seconds>(4.0);
				//lane_groups_itf* lanes = phase->template Lane_Groups<lane_groups_itf*>();
				//lane = (*lanes)[0];
				//(*approaches)[0]->template Add_Lane_Group<Lane_Group_Interface*>(lane);
				//(*approaches)[0]->template name<char*>("EB");
				//lane->template Initialize<float>();
				//lane->template avg_lane_width<Feet>(11.0);
				//lane->template demand_left<Flow_Per_Hour>(0);
				//lane->template demand_right<Flow_Per_Hour>(0);
				//lane->template demand_thru<Flow_Per_Hour>(0);
				//lane->template has_left_turn<bool>(false);
				//lane->template has_parking<bool>(false);
				//lane->template has_right_turn<bool>(false);
				//lane->template has_thru_move<bool>(true);
				//lane->template is_actuated<bool>(false);
				//lane->template right_turn_lane_type<Turn_Lane_Types>(Turn_Lane_Types::No_Turn);
				//lane->template left_turn_lane_type<Turn_Lane_Types>(Turn_Lane_Types::No_Turn);
				//lane->template left_turn_type<Left_Turn_Types>(Left_Turn_Types::None);
				//lane->template number_of_left_lanes(0);
				//lane->template number_of_lanes<int>(2);
				//lane->template opposing_lane<Lane_Group_Interface*>((Lane_Group_Interface*)NULL);
				//lane->template Detector_Left<Detector_Interface*>((Detector_Interface*)Allocate<typename Detector_Interface_type>());
				//lane->template Detector_Right<Detector_Interface*>((Detector_Interface*)Allocate<typename Detector_Interface_type>());
				//lane->template Detector_Thru<Detector_Interface*>((Detector_Interface*)Allocate<typename Detector_Interface_type>());
				//// Initialize phase 2
				//phase = (*phases)[1];
				//phase->template Initialize<int>(1);
				//phase->template yellow_and_all_red_time<Time_Seconds>(4.0);
				//phase->template phase_id<int>(4);
				//phase->template name<char*>("SB-Left");
				//lanes = phase->template Lane_Groups<lane_groups_itf*>();
				//lane2 = (*lanes)[0];
				//(*approaches)[1]->template Add_Lane_Group<Lane_Group_Interface*>(lane);
				//(*approaches)[1]->template name<char*>("SB");
				//lane2->template Initialize<float>();
				//lane2->template avg_lane_width<Feet>(11.0);
				//lane2->template demand_left<Flow_Per_Hour>(0);
				//lane2->template demand_right<Signal_Components::Data_Structures::Flow_Per_Hour>(0);
				//lane2->template demand_thru<Flow_Per_Hour>(0);
				//lane2->template has_left_turn<bool>(true);
				//lane2->template has_parking<bool>(false);
				//lane2->template has_right_turn<bool>(false);
				//lane2->template has_thru_move<bool>(false);
				//lane2->template is_actuated<bool>(false);
				//lane2->template right_turn_lane_type<Turn_Lane_Types>(Turn_Lane_Types::No_Turn);
				//lane2->template left_turn_lane_type<Turn_Lane_Types>(Turn_Lane_Types::Exclusive);
				//lane2->template left_turn_type<Left_Turn_Types>(Left_Turn_Types::Unopposed);
				//lane2->template number_of_left_lanes(1);
				//lane2->template number_of_lanes<int>(1);
				//lane2->template opposing_lane<Lane_Group_Interface*>((Lane_Group_Interface*)NULL);
				//lane2->template Detector_Left<Detector_Interface*>((Detector_Interface*)Allocate<typename Detector_Interface_type>());
				//lane2->template Detector_Right<Detector_Interface*>((Detector_Interface*)Allocate<typename Detector_Interface_type>());
				//lane2->template Detector_Thru<Detector_Interface*>((Detector_Interface*)Allocate<typename Detector_Interface_type>());

				//intersection_2->template signal<Signal_Interface*>(signal);

				i = 3;

				_Intersection_Interface* intersection_3=(_Intersection_Interface*)Allocate<typename _Intersection_Interface::Component_Type>();
				intersection_3->template uuid<int>(3);
				intersection_3->template x_position<float>(i * link_length);
				intersection_3->template y_position<float>((float) 2.0*link_length);
				intersection_3->template intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections_container<_Intersections_Container_Interface&>().push_back(intersection_3);

				i = 4;

				_Intersection_Interface* intersection_4=(_Intersection_Interface*)Allocate<typename _Intersection_Interface::Component_Type>();
				intersection_4->template uuid<int>(4);
				intersection_4->template x_position<float>(i * link_length);
				intersection_4->template y_position<float>((float) 2.0*link_length);
				intersection_4->template intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections_container<_Intersections_Container_Interface&>().push_back(intersection_4);

				i = 5;

				_Intersection_Interface* intersection_5=(_Intersection_Interface*)Allocate<typename _Intersection_Interface::Component_Type>();
				intersection_5->template uuid<int>(5);
				intersection_5->template x_position<float>((float) 1.0 * link_length);
				intersection_5->template y_position<float>((float) 1.0 * link_length);
				intersection_5->template intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections_container<_Intersections_Container_Interface&>().push_back(intersection_5);

				i = 6;

				_Intersection_Interface* intersection_6=(_Intersection_Interface*)Allocate<typename _Intersection_Interface::Component_Type>();
				intersection_6->template uuid<int>(6);
				intersection_6->template x_position<float>(0.0);
				intersection_6->template y_position<float>(0.0);
				intersection_6->template intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections_container<_Intersections_Container_Interface&>().push_back(intersection_6);

				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);

				links_container<_Links_Container_Interface&>().clear();
				
				_Link_Interface* link_0 = (_Link_Interface*)Allocate<typename _Link_Interface::Component_Type>();
				_Link_Interface* link_1 = (_Link_Interface*)Allocate<typename _Link_Interface::Component_Type>();
				_Link_Interface* link_2 = (_Link_Interface*)Allocate<typename _Link_Interface::Component_Type>();
				_Link_Interface* link_3 = (_Link_Interface*)Allocate<typename _Link_Interface::Component_Type>();
				_Link_Interface* link_4 = (_Link_Interface*)Allocate<typename _Link_Interface::Component_Type>();
				_Link_Interface* link_5 = (_Link_Interface*)Allocate<typename _Link_Interface::Component_Type>();

				i = 0;

				link_0->template upstream_intersection<_Intersection_Interface*>(intersection_0);
				link_0->template downstream_intersection<_Intersection_Interface*>(intersection_1);
				link_0->template uuid<int>(0);
				link_0->template num_lanes<int>(2);
				link_0->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::FREEWAY);
				link_0->template length<float>(link_length*distance_factor);
				link_0->template speed_limit<float>(speed_limit);
				link_0->template maximum_flow_rate<float>(maximum_flow_rate);
				link_0->template backward_wave_speed<float>(backward_wave_speed);
				link_0->template free_flow_speed<float>(speed_limit + 15.0);
				link_0->template jam_density<float>(jam_density);
				link_0->template left_turn_bay_length<float>(0.0);
				link_0->template right_turn_bay_length<float>(0.0);
				link_0->template num_left_turn_bays<int>(0);
				link_0->template num_right_turn_bays<int>(0);
				


				links_container<_Links_Container_Interface&>().push_back(link_0);

				

				max_free_flow_speed<float>(max(max_free_flow_speed<float>(),link_0->template free_flow_speed<float>()));
				
				i = 1;

				link_1->template upstream_intersection<_Intersection_Interface*>(intersection_1);
				link_1->template downstream_intersection<_Intersection_Interface*>(intersection_2);
				link_1->template uuid<int>(1);
				link_1->template num_lanes<int>(2);
				link_1->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::FREEWAY);
				link_1->template length<float>(link_length*distance_factor);
				link_1->template speed_limit<float>(speed_limit);
				link_1->template maximum_flow_rate<float>(maximum_flow_rate);
				link_1->template backward_wave_speed<float>(backward_wave_speed);
				link_1->template free_flow_speed<float>(speed_limit + 15.0);
				link_1->template jam_density<float>(jam_density);
				link_1->template left_turn_bay_length<float>(0.0);
				link_1->template right_turn_bay_length<float>(0.0);
				link_1->template num_left_turn_bays<int>(0);
				link_1->template num_right_turn_bays<int>(0);
				links_container<_Links_Container_Interface&>().push_back(link_1);

				max_free_flow_speed<float>(max(max_free_flow_speed<float>(),link_1->template free_flow_speed<float>()));

				i = 2;

				link_2->template upstream_intersection<_Intersection_Interface*>(intersection_2);
				link_2->template downstream_intersection<_Intersection_Interface*>(intersection_3);
				link_2->template uuid<int>(2);
				link_2->template num_lanes<int>(2);
				link_2->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::FREEWAY);
				link_2->template length<float>(link_length*distance_factor);
				link_2->template speed_limit<float>(speed_limit);
				link_2->template maximum_flow_rate<float>(maximum_flow_rate);
				link_2->template backward_wave_speed<float>(backward_wave_speed);
				link_2->template free_flow_speed<float>(speed_limit + 15.0);
				link_2->template jam_density<float>(jam_density);
				link_2->template left_turn_bay_length<float>(0.0);
				link_2->template right_turn_bay_length<float>(0.0);
				link_2->template num_left_turn_bays<int>(0);
				link_2->template num_right_turn_bays<int>(0);
				links_container<_Links_Container_Interface&>().push_back(link_2);

				max_free_flow_speed<float>(max(max_free_flow_speed<float>(),link_2->template free_flow_speed<float>()));

				i = 3;

				link_3->template upstream_intersection<_Intersection_Interface*>(intersection_3);
				link_3->template downstream_intersection<_Intersection_Interface*>(intersection_4);
				link_3->template uuid<int>(3);
				link_3->template num_lanes<int>(2);
				link_3->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::FREEWAY);
				link_3->template length<float>(link_length*distance_factor);
				link_3->template speed_limit<float>(speed_limit);
				link_3->template maximum_flow_rate<float>(maximum_flow_rate);
				link_3->template backward_wave_speed<float>(backward_wave_speed);
				link_3->template free_flow_speed<float>(speed_limit + 15.0);
				link_3->template jam_density<float>(jam_density);
				link_3->template left_turn_bay_length<float>(0.0);
				link_3->template right_turn_bay_length<float>(0.0);
				link_3->template num_left_turn_bays<int>(0);
				link_3->template num_right_turn_bays<int>(0);
				links_container<_Links_Container_Interface&>().push_back(link_3);

				max_free_flow_speed<float>(max(max_free_flow_speed<float>(),link_3->template free_flow_speed<float>()));


				i = 4;

				link_4->template upstream_intersection<_Intersection_Interface*>(intersection_5);
				link_4->template downstream_intersection<_Intersection_Interface*>(intersection_2);
				link_4->template uuid<int>(4);
				link_4->template num_lanes<int>(1);
				link_4->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::ON_RAMP);
				link_4->template length<float>(link_length*distance_factor);
				link_4->template speed_limit<float>(speed_limit_ramp);
				link_4->template maximum_flow_rate<float>(maximum_flow_rate_ramp);
				link_4->template backward_wave_speed<float>(backward_wave_speed);
				link_4->template free_flow_speed<float>(speed_limit + 15.0);
				link_4->template jam_density<float>(jam_density);
				link_4->template left_turn_bay_length<float>(0.0);
				link_4->template right_turn_bay_length<float>(0.0);
				link_4->template num_left_turn_bays<int>(0);
				link_4->template num_right_turn_bays<int>(0);
				links_container<_Links_Container_Interface&>().push_back(link_4);

				max_free_flow_speed<float>(max(max_free_flow_speed<float>(),link_4->template free_flow_speed<float>()));

				i = 5;

				link_5->template upstream_intersection<_Intersection_Interface*>(intersection_6);
				link_5->template downstream_intersection<_Intersection_Interface*>(intersection_5);
				link_5->template uuid<int>(5);
				link_5->template num_lanes<int>(1);
				link_5->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::ARTERIAL);
				link_5->template length<float>(link_length*distance_factor);
				link_5->template speed_limit<float>(speed_limit_ramp);
				link_5->template maximum_flow_rate<float>(maximum_flow_rate_arterial);
				link_5->template backward_wave_speed<float>(backward_wave_speed);
				link_5->template free_flow_speed<float>(link_5->template speed_limit<float>() + 10.0);
				link_5->template jam_density<float>(jam_density);
				link_5->template left_turn_bay_length<float>(0.0);
				link_5->template right_turn_bay_length<float>(0.0);
				link_5->template num_left_turn_bays<int>(0);
				link_5->template num_right_turn_bays<int>(0);
				links_container<_Links_Container_Interface&>().push_back(link_5);

				max_free_flow_speed<float>(max(max_free_flow_speed<float>(),link_5->template free_flow_speed<float>()));

				//========================================================================================================
				//Create inbound/outbound links for each intersection
				//--------------------------------------------------------------------------------------------------------
				//
				typename _Links_Container_Interface::iterator links_itr;
				for (links_itr = links_container<_Links_Container_Interface&>().begin(); links_itr != links_container<_Links_Container_Interface&>().end(); links_itr++)
				{
					_Link_Interface* link = (_Link_Interface*)(*links_itr);
					link->template upstream_intersection<_Intersection_Interface*>()->template outbound_links<_Links_Container_Interface&>().push_back(link);
					link->template downstream_intersection<_Intersection_Interface*>()->template inbound_links<_Links_Container_Interface&>().push_back(link);
				}

				//========================================================================================================
				//Create turn movements for each intersection
				//--------------------------------------------------------------------------------------------------------
				//
				
				//turn connectivities

				define_container_and_value_interface(_Turn_Movements_Container_Interface, _Turn_Movement_Interface, get_type_of(turn_movements_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				//define_component_interface(Detector_Interface,Signal_Components::Prototypes::Detector_Prototype,_Turn_Movement_Interface_type::detector,ComponentType);
				_Turn_Movement_Interface* turn_movement_0 = (_Turn_Movement_Interface*)Allocate<typename _Turn_Movement_Interface::Component_Type>();
				turn_movement_0->template inbound_link<_Link_Interface*>(link_0);
				turn_movement_0->template outbound_link<_Link_Interface*>(link_1);

				int c=turn_movement_0->template outbound_link<_Link_Interface*>()->template uuid<int>();

				turn_movement_0->template movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(Intersection_Components::Types::THROUGH_TURN);
				turn_movement_0->template movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys>(Intersection_Components::Types::ALLOWED);
				turn_movement_0->template id<int>(0);
				//// assign the detector
				//turn_movement_0->template detector<Detector_Interface*>(NULL);
				turn_movements_container<_Turn_Movements_Container_Interface&>().push_back(turn_movement_0);

				_Turn_Movement_Interface* turn_movement_1 = (_Turn_Movement_Interface*)Allocate<typename _Turn_Movement_Interface::Component_Type>();
				turn_movement_1->template inbound_link<_Link_Interface*>(link_1);
				turn_movement_1->template outbound_link<_Link_Interface*>(link_2);
				turn_movement_1->template movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(Intersection_Components::Types::THROUGH_TURN);
				turn_movement_1->template movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys>(Intersection_Components::Types::ALLOWED);
				turn_movement_1->template id<int>(1);
				//// assign the detector
				//turn_movement_1->template detector<Detector_Interface*>(lane->template Detector_Thru<Detector_Interface*>());
				turn_movements_container<_Turn_Movements_Container_Interface&>().push_back(turn_movement_1);

				_Turn_Movement_Interface* turn_movement_2 = (_Turn_Movement_Interface*)Allocate<typename _Turn_Movement_Interface::Component_Type>();
				turn_movement_2->template inbound_link<_Link_Interface*>(link_2);
				turn_movement_2->template outbound_link<_Link_Interface*>(link_3);
				turn_movement_2->template movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(Intersection_Components::Types::THROUGH_TURN);
				turn_movement_2->template movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys>(Intersection_Components::Types::ALLOWED);
				turn_movement_2->template id<int>(2);
				//turn_movement_2->template detector<Detector_Interface*>(NULL);
				turn_movements_container<_Turn_Movements_Container_Interface&>().push_back(turn_movement_2);

				_Turn_Movement_Interface* turn_movement_3 = (_Turn_Movement_Interface*)Allocate<typename _Turn_Movement_Interface::Component_Type>();
				turn_movement_3->template inbound_link<_Link_Interface*>(link_4);
				turn_movement_3->template outbound_link<_Link_Interface*>(link_2);
				turn_movement_3->template movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(Intersection_Components::Types::RIGHT_TURN);
				turn_movement_3->template movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys>(Intersection_Components::Types::ALLOWED);
				turn_movement_3->template id<int>(3);
				//// assign the detector
				//turn_movement_3->template detector<Detector_Interface*>(lane2->template Detector_Left<Detector_Interface*>());
				turn_movements_container<_Turn_Movements_Container_Interface&>().push_back(turn_movement_3);

				
				_Turn_Movement_Interface* turn_movement_4 = (_Turn_Movement_Interface*)Allocate<typename _Turn_Movement_Interface::Component_Type>();
				turn_movement_4->template inbound_link<_Link_Interface*>(link_5);
				turn_movement_4->template outbound_link<_Link_Interface*>(link_4);
				turn_movement_4->template movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(Intersection_Components::Types::THROUGH_TURN);
				turn_movement_4->template movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys>(Intersection_Components::Types::ALLOWED);
				turn_movement_4->template id<int>(4);
				//turn_movement_4->template detector<Detector_Interface*>(NULL);
				turn_movements_container<_Turn_Movements_Container_Interface&>().push_back(turn_movement_4);

				typename _Turn_Movements_Container_Interface::iterator turn_movements_itr;
				for (turn_movements_itr = turn_movements_container<_Turn_Movements_Container_Interface&>().begin(); turn_movements_itr != turn_movements_container<_Turn_Movements_Container_Interface&>().end(); turn_movements_itr++)
				{
					_Turn_Movement_Interface* turn_mvmt=(_Turn_Movement_Interface*)(*turn_movements_itr);

					//outbound turn movement of an inbound link
					_Link_Interface* inbound_link = turn_mvmt->template inbound_link<_Link_Interface*>();
					inbound_link->template outbound_turn_movements<_Turn_Movements_Container_Interface&>().push_back(turn_mvmt);
					//inbound turn movement of an outbound link
					_Link_Interface* outbound_link = turn_mvmt->template outbound_link<_Link_Interface*>();
					outbound_link->template inbound_turn_movements<_Turn_Movements_Container_Interface&>().push_back(turn_mvmt);
				}

				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, _Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);			

				// configure outbound_inbound_movements
				typename _Intersections_Container_Interface::iterator intersections_itr;
				for (intersections_itr = intersections_container<_Intersections_Container_Interface&>().begin(); intersections_itr != intersections_container<_Intersections_Container_Interface&>().end(); intersections_itr++)
				{
					_Intersection_Interface* intersection = (_Intersection_Interface*)(*intersections_itr);
					_Links_Container_Interface& outbound_links = intersection->template outbound_links<_Links_Container_Interface&>();
					// skip intersections_container that do not have any outbound links
					if (outbound_links.size() == 0)
						continue;
					typename _Links_Container_Interface::iterator links_itr;
					
					for (links_itr = outbound_links.begin(); links_itr != outbound_links.end(); links_itr++)
					{
						_Link_Interface* outboundLink = (_Link_Interface*)(*links_itr);
						
						_Outbound_Inbound_Movements_Interface* outboundInboundMovements = (_Outbound_Inbound_Movements_Interface*)Allocate<typename _Outbound_Inbound_Movements_Interface::Component_Type>();
						
						outboundInboundMovements->template outbound_link_reference<_Link_Interface*>(outboundLink);
						_Turn_Movements_Container_Interface& inboundTurnMovements = outboundLink->template inbound_turn_movements<_Turn_Movements_Container_Interface&>();
						
						for (int k = 0; k < (int)inboundTurnMovements.size(); k++)
						{
							outboundInboundMovements->template inbound_movements<_Turn_Movements_Container_Interface&>().push_back(inboundTurnMovements[k]);
						}
						intersection->template outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>().push_back(outboundInboundMovements);
					}
					size_t size = intersection->template outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>().size();
				}

				define_container_and_value_interface(_Inbound_Outbound_Movements_Container_Interface, _Inbound_Outbound_Movements_Interface, _Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);

						
				// configure inbound_outbound_movements
				for (i = 0; i < (int)intersections_container<_Intersections_Container_Interface&>().size(); i++)
				{
					_Intersection_Interface* intersection = (_Intersection_Interface*)intersections_container<_Intersections_Container_Interface&>()[i];
					// skip intersections_container that do not have any inbound links
					if (intersection->template inbound_links<_Links_Container_Interface&>().size() == 0)
						continue;
					
					for (int j = 0; j < (int)intersection->template inbound_links<_Links_Container_Interface&>().size(); j++) 
					{
						_Inbound_Outbound_Movements_Interface* inboundOutboundMovements = (_Inbound_Outbound_Movements_Interface*)Allocate<typename _Inbound_Outbound_Movements_Interface::Component_Type>();
						_Link_Interface* inboundLink = (_Link_Interface*)intersection->template inbound_links<_Links_Container_Interface&>()[j];
						inboundOutboundMovements->template inbound_link_reference<_Link_Interface*>(inboundLink);
						_Turn_Movements_Container_Interface& outboundTurnMovements = inboundLink->template outbound_turn_movements<_Turn_Movements_Container_Interface&>();
						//cout<<"link id = " << inboundLink->template uuid<int>()<<endl;
						//cout<<"intersection id = " << intersection->template uuid<int>()<<endl;
						for (int k = 0; k < (int)outboundTurnMovements.size(); k++)
						{
							inboundOutboundMovements->template outbound_movements<_Turn_Movements_Container_Interface&>().push_back(outboundTurnMovements[k]);
						}
						intersection->template inbound_outbound_movements<_Inbound_Outbound_Movements_Container_Interface&>().push_back(inboundOutboundMovements);
					}
				}

				//activity location

				define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);

				activity_locations_container<_Activity_Locations_Container_Interface&>().clear();
				
				_Activity_Location_Interface* activity_location_0 = (_Activity_Location_Interface*)Allocate<typename _Activity_Location_Interface::Component_Type>();
				_Activity_Location_Interface* activity_location_1 = (_Activity_Location_Interface*)Allocate<typename _Activity_Location_Interface::Component_Type>();
				_Activity_Location_Interface* activity_location_2 = (_Activity_Location_Interface*)Allocate<typename _Activity_Location_Interface::Component_Type>();

				i=0;
				activity_location_0->template origin_links<_Links_Container_Interface&>().push_back(link_0);
				activity_location_0->template zone<int&>(i);
				activity_locations_container<_Activity_Locations_Container_Interface&>().push_back(activity_location_0);

				i=1;
				activity_location_1->template destination_links<_Links_Container_Interface&>().push_back(link_3);
				activity_location_1->template zone<int&>(i);
				activity_locations_container<_Activity_Locations_Container_Interface&>().push_back(activity_location_1);

				i=2;
				activity_location_2->template origin_links<_Links_Container_Interface&>().push_back(link_5);
				activity_location_2->template zone<int&>(i);
				activity_locations_container<_Activity_Locations_Container_Interface&>().push_back(activity_location_2);
				

			}

			feature_prototype void simulation_initialize(void* scenario_data)
			{
				typedef typename Scenario_Components::Prototypes::Scenario_Prototype<typename ComponentType::scenario_type, ComponentType> _Scenario_Interface;

				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_simple_container_interface(_Int_Container_Interface, _Link_Interface::get_type_of(cached_link_downstream_cumulative_vehicles_array), Random_Access_Sequence_Prototype, int, ComponentType);
				define_container_and_value_interface(_Vehicles_Vector_Interface, _Vehicle_Interface, _Link_Interface::get_type_of(link_origin_vehicle_array), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				typename _Links_Container_Interface::iterator links_itr;
				int i, j;
				for (links_itr = links_container<_Links_Container_Interface&>().begin(); links_itr != links_container<_Links_Container_Interface&>().end(); links_itr++)
				{
					_Link_Interface* link = (_Link_Interface*)(*links_itr);
							
					//network_data
					link->template link_origin_cumulative_arrived_vehicles<int>(0);
					link->template link_origin_vehicle_array<_Vehicles_Vector_Interface&>().clear();
					
					link->template scenario_reference<_Scenario_Interface*>((_Scenario_Interface*)scenario_data);

					//init link simulation model
					link->template link_capacity<int>(0);
					//link_simulation_model: in Kuilin's code but not converted
					link->template link_upstream_arrived_vehicles<int>(0);
					link->template link_downstream_departed_vehicles<int>(0);
					link->template link_origin_arrived_vehicles<int>(0);
					link->template link_origin_departed_vehicles<int>(0);
					link->template link_destination_arrived_vehicles<int>(0);
					link->template link_origin_vehicle_current_position<int>(0);
					
					//supply
					link->template link_supply<float>(link->template num_lanes<int>() * link->template length<float>() * link->template jam_density<float>());
					
					//cumulative vehicles
					link->template link_destination_arrived_vehicles<int>(0);
					link->template link_origin_cumulative_arrived_vehicles<int>(0);
					link->template link_origin_cumulative_departed_vehicles<int>(0);
					link->template link_upstream_cumulative_arrived_vehicles<int>(0);
					link->template link_upstream_cumulative_vehicles<int>(0);
				
					////bwtt and fftt
					float bwtt = (float) (link->template length<float>()/(link->template backward_wave_speed<float>()*5280.0/3600.0)); // in seconds
					float fftt = (float) (link->template length<float>()/(link->template free_flow_speed<float>()*5280.0/3600.0)); //in seconds
					link->template link_fftt<float>(fftt);
					link->template link_bwtt<float>(bwtt);
				
					link->template link_fftt_cached_simulation_interval_size<int>(int(ceil(float(fftt/(float(link->template scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<float>()))))));
					link->template link_bwtt_cached_simulation_interval_size<int>(int(ceil(float(bwtt/(float(link->template scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<float>()))))));

					//downstream cumulative vehicles
					link->template cached_link_downstream_cumulative_vehicles_array<_Int_Container_Interface&>().clear();
					link->template cached_link_downstream_cumulative_vehicles_array<_Int_Container_Interface&>().resize(link->template link_bwtt_cached_simulation_interval_size<int>());
					for (j = 0; j < (int)link->template cached_link_downstream_cumulative_vehicles_array<_Int_Container_Interface&>().size(); j++)
					{
						link->template cached_link_downstream_cumulative_vehicles_array<_Int_Container_Interface&>()[j] = 0;
					}

					//upstream cumulative vehicles
					link->template cached_link_upstream_cumulative_vehicles_array<_Int_Container_Interface&>().clear();
					link->template cached_link_upstream_cumulative_vehicles_array<_Int_Container_Interface&>().resize(link->template link_fftt_cached_simulation_interval_size<int>());
					for (j = 0; j < (int)link->template cached_link_upstream_cumulative_vehicles_array<_Int_Container_Interface&>().size(); j++)
					{
						link->template cached_link_upstream_cumulative_vehicles_array<_Int_Container_Interface&>()[j] = 0;
					}
					
				}
				//determine minimum merge rate
				define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, _Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Inbound_Movements_Container_Interface, _Inbound_Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);

				define_simple_container_interface(_Float_Container_Interface, _Inbound_Movement_Interface::get_type_of(cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array), Random_Access_Sequence_Prototype, float, ComponentType);
				define_container_and_value_interface(_Vehicles_Queue_Interface, _Vehicle_Interface_2, _Inbound_Movement_Interface::get_type_of(vehicles_container), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);


				for (i = 0; i < (int)intersections_container<_Intersections_Container_Interface&>().size(); i++)
				{
					_Intersection_Interface* intersection = (_Intersection_Interface*)intersections_container<_Intersections_Container_Interface&>()[i];

					unsigned long seed = ((_Scenario_Interface*)scenario_data)->template iseed<unsigned int>()+intersection->template uuid<int>()+1;
					(intersection->template rng_stream<typename _Intersection_Interface::get_type_of(rng_stream)&>()).SetSeed(seed);

					intersection->template scenario_reference<_Scenario_Interface*>((_Scenario_Interface*)scenario_data);

					_Outbound_Inbound_Movements_Container_Interface& outbound_inbound_movements_container = intersection->template outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
					typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_inbound_movements_itr;
					for (outbound_inbound_movements_itr=outbound_inbound_movements_container.begin(),j=0;outbound_inbound_movements_itr!=outbound_inbound_movements_container.end();outbound_inbound_movements_itr++,j++)
					{
						_Outbound_Inbound_Movements_Interface* outbound_inbound_movements = (_Outbound_Inbound_Movements_Interface*)(*outbound_inbound_movements_itr);

						_Inbound_Movements_Container_Interface& inbound_movements_container = outbound_inbound_movements->template inbound_movements<_Inbound_Movements_Container_Interface&>();
						typename _Inbound_Movements_Container_Interface::iterator inbound_movement_itr;
						for (inbound_movement_itr=inbound_movements_container.begin();inbound_movement_itr!=inbound_movements_container.end();inbound_movement_itr++)
						{
							_Inbound_Movement_Interface* inbound_movement = (_Inbound_Movement_Interface*)(*inbound_movement_itr);
							inbound_movement->template movement_capacity<float>(0.0);
							inbound_movement->template movement_demand<float>(0.0);
							inbound_movement->template movement_supply<float>(0.0);
							inbound_movement->template movement_flow<float>(0.0);
							inbound_movement->template movement_transferred<int>(0);
							inbound_movement->template turn_movement_cumulative_vehicles<int>(0);
							inbound_movement->template turn_movement_cumulative_arrived_vehicles<int>(0);
							inbound_movement->template turn_movement_cumulative_shifted_arrived_vehicles<int>(0);
							
							//inbound_movement->template vehicles_container<_Vehicles_Queue_Interface&>().clear();
							
							inbound_movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(0.0);
							inbound_movement->template inbound_link_departed_time_based_experienced_link_turn_travel_delay<float>(0.0);
							inbound_movement->template cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>().clear();
							inbound_movement->template cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>().clear();
							inbound_movement->template cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>().resize(((_Scenario_Interface*)scenario_data)->template num_simulation_intervals_per_assignment_interval<int>());
							inbound_movement->template cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>().resize(((_Scenario_Interface*)scenario_data)->template num_simulation_intervals_per_assignment_interval<int>());
							
							
							for (j=0;j<((_Scenario_Interface*)scenario_data)->template num_simulation_intervals_per_assignment_interval<int>();j++)
							{
								inbound_movement->template cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>()[j] = 0.0;
								inbound_movement->template cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>()[j] = 0.0;
							}
									
							_Link_Interface* inbound_link = inbound_movement->template inbound_link<_Link_Interface*>();
							inbound_movement->template cached_turn_movement_cumulative_shifted_arrived_vehicles_array<_Int_Container_Interface&>().clear();
							inbound_movement->template cached_turn_movement_cumulative_shifted_arrived_vehicles_array<_Int_Container_Interface&>().resize(inbound_link->template link_bwtt_cached_simulation_interval_size<int>());
							for (j=0;j<inbound_link->template link_bwtt_cached_simulation_interval_size<int>();j++)
							{
								inbound_movement->template cached_turn_movement_cumulative_shifted_arrived_vehicles_array<_Int_Container_Interface&>()[j] = 0; 
							}

							if (inbound_movement->template movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>() == Intersection_Components::Types::THROUGH_TURN)
							{
								inbound_movement->template minimum_merge_rate<float>(1.0);
							}
							else
							{
								inbound_movement->template minimum_merge_rate<float>(0.2);
							}
						}
					}
				}
			}

			feature_prototype void printNetwork()
			{
				// for debug, we printout everything of the network
				
				cout<<endl<<"***********************************Regular Network************************************************"<<endl;
				cout<<"all links"<<endl;
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				typename _Links_Container_Interface::iterator links_itr;
				for (links_itr = links_container<_Links_Container_Interface&>().begin(); links_itr != links_container<_Links_Container_Interface&>().end(); links_itr++)
				{			

					_Link_Interface* link = (_Link_Interface*)(*links_itr);
					cout<<"\t link_"<<link->template uuid<int>()<<endl;
					cout<<"\t\t upstream: " << link->template upstream_intersection<_Intersection_Interface*>()->template uuid<int>()<<endl;
					cout<<"\t\t downstream: " << link->template downstream_intersection<_Intersection_Interface*>()->template uuid<int>()<<endl;
					cout<<"\t\t num_lanes: " << link->template num_lanes<int>()<<endl;
					cout<<"\t\t link_type: " << link->template link_type<Link_Components::Types::Link_Type_Keys>()<<endl;
					cout<<"\t\t length: " << link->template length<float>()<<endl;
					cout<<"\t\t speed_limit: " << link->template speed_limit<float>()<<endl;
					cout<<"\t\t maximum_flow_rate: " << link->template maximum_flow_rate<float>()<<endl;
					cout<<"\t\t backward_wave_speed: " << link->template backward_wave_speed<float>()<<endl;
					cout<<"\t\t free_flow_speed: " << link->template free_flow_speed<float>()<<endl;
					cout<<"\t\t jam_density: " << link->template jam_density<float>()<<endl;
					cout<<"\t\t left_turn_bay_length: " << link->template left_turn_bay_length<float>()<<endl;
					cout<<"\t\t right_turn_bay_length: " << link->template right_turn_bay_length<float>()<<endl;
					cout<<"\t\t num_left_turn_bays: " << link->template num_left_turn_bays<int>()<<endl;
					cout<<"\t\t num_right_turn_bays: " << link->template num_right_turn_bays<int>()<<endl;
					cout<<"\t\t travel_time: " <<link->template travel_time<float>()<<endl;
					cout<<"\t\t -----------------------------------"<<endl<<endl;
				}

				cout <<"-------------------------------------------------------------------------------------"<<endl;
				cout << "all intersections" << endl;
				
				define_container_and_value_interface(_Turn_Movements_Container_Interface, _Turn_Movement_Interface, get_type_of(turn_movements_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, _Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);	
				define_container_and_value_interface(_Inbound_Outbound_Movements_Container_Interface, _Inbound_Outbound_Movements_Interface, _Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);
				
				int j, k;
				typename _Intersections_Container_Interface::iterator intersections_itr;
				for (intersections_itr = intersections_container<_Intersections_Container_Interface&>().begin(); intersections_itr != intersections_container<_Intersections_Container_Interface&>().end(); intersections_itr++)
				{			

					_Intersection_Interface* intersection = (_Intersection_Interface*)(*intersections_itr);
					cout<<"\t intersection_"<<intersection->template uuid<int>()<<endl;
					cout<<"\t\t x_position: "<<intersection->template x_position<float>()<<endl;
					cout<<"\t\t y_position: "<<intersection->template y_position<float>()<<endl;
					cout<<"\t\t intersection_type: "<<intersection->template intersection_type<int>()<<endl;
					
					
					
					cout<<"\t\t all inbound_outbound_movements: "<<endl;
					_Inbound_Outbound_Movements_Container_Interface& inbound_outbound_movements_container = intersection->template inbound_outbound_movements<_Inbound_Outbound_Movements_Container_Interface&>();
					typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_outbound_movements_itr;
					for (inbound_outbound_movements_itr=inbound_outbound_movements_container.begin(),j=0;inbound_outbound_movements_itr!=inbound_outbound_movements_container.end();inbound_outbound_movements_itr++,j++)
					{
						_Inbound_Outbound_Movements_Interface* inbound_outbound_movements = (_Inbound_Outbound_Movements_Interface*)(*inbound_outbound_movements_itr);
						cout<<"\t\t inbound_outbound_movements_"<<j<<endl;
						cout<<"\t\t\t inbound_link: "<<inbound_outbound_movements->template inbound_link_reference<_Link_Interface*>()->template uuid<int>()<<endl;
						cout<<"\t\t\t outbound_movements: "<<endl;
						_Turn_Movements_Container_Interface& outbound_movements_container = inbound_outbound_movements->template outbound_movements<_Turn_Movements_Container_Interface&>();
						typename _Turn_Movements_Container_Interface::iterator outbound_movement_itr;
						for (outbound_movement_itr=outbound_movements_container.begin(),k=0;outbound_movement_itr!=outbound_movements_container.end();outbound_movement_itr++,k++)
						{
							_Turn_Movement_Interface* outbound_movement = (_Turn_Movement_Interface*)(*outbound_movement_itr);
							cout<<"\t\t\t\t outbound_movements_"<<k<<endl;
							cout<<"\t\t\t\t\t outbound_link: "<<outbound_movement->template outbound_link<_Link_Interface*>()->template uuid<int>()<<endl;
							cout<<"\t\t\t\t\t movement_type: "<<outbound_movement->template movement_type<int>()<<endl;
							cout<<"\t\t\t\t\t forward_link_turn_travel_time: "<<outbound_movement->template forward_link_turn_travel_time<float>()<<endl;
						}
					}

					cout<<"\t\t all outbound_inbound_movements: " <<endl;
					_Outbound_Inbound_Movements_Container_Interface& outbound_inbound_movements_container = intersection->template outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
					typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_inbound_movements_itr;
					for (outbound_inbound_movements_itr=outbound_inbound_movements_container.begin(),j=0;outbound_inbound_movements_itr!=outbound_inbound_movements_container.end();outbound_inbound_movements_itr++,j++)
					{
						_Outbound_Inbound_Movements_Interface* outbound_inbound_movements = (_Outbound_Inbound_Movements_Interface*)(*outbound_inbound_movements_itr);
						cout<<"\t\t outbound_inbound_movements_"<<j<<endl;
						cout<<"\t\t\t outbound_link: "<<outbound_inbound_movements->template outbound_link_reference<_Link_Interface*>()->template uuid<int>()<<endl;
						cout<<"\t\t\t inbound_movements: "<<endl;
						_Turn_Movements_Container_Interface& inbound_movements_container = outbound_inbound_movements->template inbound_movements<_Turn_Movements_Container_Interface&>();
						typename _Turn_Movements_Container_Interface::iterator inbound_movement_itr;
						for (inbound_movement_itr=inbound_movements_container.begin(),k=0;inbound_movement_itr!=inbound_movements_container.end();inbound_movement_itr++,k++)
						{
							_Turn_Movement_Interface* inbound_movement = (_Turn_Movement_Interface*)(*inbound_movement_itr);
							cout<<"\t\t\t\t inbound_movements_"<<k<<endl;
							cout<<"\t\t\t\t\t inbound_link: "<<inbound_movement->template inbound_link<_Link_Interface*>()->template uuid<int>()<<endl;
							cout<<"\t\t\t\t\t movement_type: "<<inbound_movement->template movement_type<int>()<<endl;
							cout<<"\t\t\t\t\t forward_link_turn_travel_time: "<<inbound_movement->template forward_link_turn_travel_time<float>()<<endl;
						}
					}
					cout<<"\t\t --------------------------------"<<endl<<endl;
				}

				cout <<"***********************************************************************************"<<endl;
				cout << "all activity locations" << endl;

				define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
                int i;
                typename _Activity_Locations_Container_Interface::iterator activity_locations_itr;
                for (activity_locations_itr = activity_locations_container<_Activity_Locations_Container_Interface&>().begin(), i = 0; activity_locations_itr != activity_locations_container<_Activity_Locations_Container_Interface&>().end(); activity_locations_itr++, i++)
                {
                    _Activity_Location_Interface* activity_location = (_Activity_Location_Interface*)(*activity_locations_itr);
					cout<<"\t activity_location_"<<i<<endl;
					cout<<"\t\t zone: "<<activity_location->template zone<int>()<<endl;
					cout<<"\t\t origin_links:"<<endl;
					typename _Links_Container_Interface::iterator origin_link_itr;
					_Links_Container_Interface& origin_links_container = activity_location->template origin_links<_Links_Container_Interface&>();
					for (origin_link_itr=origin_links_container.begin(); origin_link_itr!=origin_links_container.end(); origin_link_itr++)
					{
						cout<<"\t\t link_"<<((_Link_Interface*)(*origin_link_itr))->template uuid<int>()<<endl;
					}
					cout<<"\t\t destination_links:"<<endl;
					typename _Links_Container_Interface::iterator destination_link_itr;
					_Links_Container_Interface& destination_links_container = activity_location->template destination_links<_Links_Container_Interface&>();
					for (destination_link_itr=destination_links_container.begin(); destination_link_itr!=destination_links_container.end(); destination_link_itr++)
					{
						cout<<"\t\t link_"<<((_Link_Interface*)(*destination_link_itr))->template uuid<int>()<<endl;
					}
				}

			}

			feature_prototype void construct_network_cost()
			{

				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				typename _Links_Container_Interface::iterator links_itr;
				for (links_itr = links_container<_Links_Container_Interface&>().begin(); links_itr != links_container<_Links_Container_Interface&>().end(); links_itr++)
				{			

					_Link_Interface* link = (_Link_Interface*)(*links_itr);
					float free_flow_speed = (float) ((5280.0) * link->template free_flow_speed<float>()/3600.0); // feet per second
					float link_travel_time = float (link->template length<float>() / free_flow_speed);
					
					max_free_flow_speed<float>(max(max_free_flow_speed<float>(),free_flow_speed));
					link_travel_time = max((float)1.0,link_travel_time);
					link->template travel_time<float>(link_travel_time);
				}
				
				define_container_and_value_interface(_Turn_Movements_Container_Interface, _Turn_Movement_Interface, get_type_of(turn_movements_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				typename _Turn_Movements_Container_Interface::iterator turn_movements_itr;
				for (turn_movements_itr = turn_movements_container<_Turn_Movements_Container_Interface&>().begin(); turn_movements_itr != turn_movements_container<_Turn_Movements_Container_Interface&>().end(); turn_movements_itr++)
				{
					_Turn_Movement_Interface* mvmt_itf=(_Turn_Movement_Interface*)(*turn_movements_itr);

					_Link_Interface* inboundLink = mvmt_itf->template inbound_link<_Link_Interface*>();
					_Link_Interface* outboundLink = mvmt_itf->template outbound_link<_Link_Interface*>();

					float turn_travel_penalty = (float) ((inboundLink->template travel_time<float>() + outboundLink->template travel_time<float>())*0.10);
					float forward_link_turn_travel_time = inboundLink->template travel_time<float>() + turn_travel_penalty;

					mvmt_itf->template turn_travel_penalty<float>(turn_travel_penalty);
					mvmt_itf->template forward_link_turn_travel_time<float>(forward_link_turn_travel_time);
					for (int j = 0; j < (int)mvmt_itf->template replicas_container<_Turn_Movements_Container_Interface&>().size(); j++)
					{
						_Turn_Movement_Interface* replica = (_Turn_Movement_Interface*)mvmt_itf->template replicas_container<_Turn_Movements_Container_Interface&>()[j];
						replica->template forward_link_turn_travel_time<float>(forward_link_turn_travel_time);
				}
			}
			}

			feature_prototype void construct_routable_network()
			{

				define_container_and_value_interface(_Routable_Networks_Container_Interface, _Routable_Network_Interface, get_type_of(routable_networks_container), Random_Access_Sequence_Prototype, Routing_Components::Prototypes::Routable_Network_Prototype, ComponentType);
				_Routable_Networks_Container_Interface& routable_networks=routable_networks_container<_Routable_Networks_Container_Interface&>();

				for(int i=0;i<_num_threads;i++)
				{
					_Routable_Network_Interface* routable_network = (_Routable_Network_Interface*)Allocate<typename _Routable_Network_Interface::Component_Type>();
					routable_network->template read_routable_network_data<NULLTYPE>(this);
					routable_networks.push_back(routable_network);
				}
			}
		};
	}
}

using namespace Network_Components::Prototypes;
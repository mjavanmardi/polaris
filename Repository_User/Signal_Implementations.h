#pragma once

#include "Signal_Interfaces.h"


namespace Signal_Components
{
	//==================================================================================================================
	/// DETECTOR BASE Namespace of Base classes and Components related to the Interface(s) from the component namespace.
	//------------------------------------------------------------------------------------------------------------------
	namespace Bases
	{
		//==================================================================================================================
		/// Signal Detector Base
		//------------------------------------------------------------------------------------------------------------------
		template<typename MasterType>
		struct Signal_Detector_Base
		{
			Signal_Detector_Base() : _last_access(Data_Structures::Time_Second(0)), _count(0){}
			Data_Structures::Time_Second _last_access;
			int _count;

			feature_implementation typename TargetType::ReturnType count(typename TargetType::ParamType time, call_requirements(
				requires(typename TargetType::ReturnType,Concepts::Is_Flow_Per_Hour) &&
				requires(typename TargetType::ParamType,Concepts::Is_Time_Seconds)))
			{
				typename TargetType::ReturnType value = 0;
				Data_Structures::Time_Second time_since = (float)time.Value - (float)_last_access.Value;
				if (time_since <=0) return value;
				_last_access.Value = time.Value;
				value = (_count / time_since * 3600.f);
				_count = 0;
				return value;
				//return _count;
			}
			feature_implementation typename TargetType::ReturnType count(typename TargetType::ParamType time, call_requirements(
				requires(typename TargetType::ReturnType,Concepts::Is_Flow_Per_15_Minutes) &&
				requires(typename TargetType::ParamType,Concepts::Is_Time_Seconds)))
			{
				Data_Structures::Time_Second time_since = time - _last_access;
				_last_access = time;
				_count = 0;
				return (_count / time_since / 900.f);
			}

			feature_implementation void detect_vehicle(TargetType vehicle_detection_count)
			{
				_count += (int)vehicle_detection_count;
			}
			feature_implementation TargetType Get_Count()
			{
				return (TargetType)_count;
			}
		};
	}
	//==================================================================================================================
	/// DETECTOR COMPONENT Namespace:  Namespace for the creation of All signal components
	//------------------------------------------------------------------------------------------------------------------
	namespace Components
	{
		template<typename MasterType>
		struct Signal_Detector
		{
			typedef Polaris_Component<Interfaces::Detector_Interface, Bases::Signal_Detector_Base<MasterType>,NULLTYPE,MasterType> type;
		};
	}



	//==================================================================================================================
	/// SIGNAL ELEMENTS BASES Namespace of Base classes and Components related to the Interface(s) from the component namespace.
	//------------------------------------------------------------------------------------------------------------------
	namespace Bases
	{
		//==================================================================================================================
		/// Lane Group Bases
		//------------------------------------------------------------------------------------------------------------------
		/// Holds common data for all HCM Lanegroup Bases
		//------------------------------------------------------------------------------------------------------------------
		template<typename MasterType>
		struct Lane_Group_HCM_Base
		{
			//============================================================
			//  Lane Group Initializer
			//------------------------------------------------------------
			feature_implementation void Initialize()
			{
				this->_avg_lane_width = 10.0;
				this->_base_saturation_flow = 1900.0;
				this->_demand_lane_group = 0;
				this->_demand_lane_max = 0;
				this->_demand_left = 0;
				this->_demand_right = 0;
				this->_demand_thru = 0;
				this->_has_left_turn = false;
				this->_has_parking = false;
				this->_has_right_turn = false;
				this->_has_thru_move = false;
				this->_is_actuated = false;
				this->_left_turn_type = Data_Structures::Left_Turn_Types::None;
				this->_min_pedestrian_green = 0;
				this->_number_of_lanes = 1;
				this->_number_of_left_lanes = 0;
				this->_number_of_right_lanes = 0;
				this->_peak_hour_factor = 1.0;
				this->_opposing_lane = NULL;
			}

			//============================================================
			//  PARENT CLASS ACCESS HANDLERS
			//------------------------------------------------------------
			tag_getter(in_CBD);
			feature_implementation TargetType in_CBD(call_requirements(requires(TargetType,is_integral)))			
			{
				Interfaces::Phase_Interface<ComponentType::Parent_Type,NULLTYPE>* parent = (Interfaces::Phase_Interface<ComponentType::Parent_Type,NULLTYPE>*)(((ComponentType*)this)->_parent);
				return parent->in_CBD<TargetType>();
			}
			feature_implementation TargetType in_CBD(call_requirements(requires(TargetType,!is_integral)))			
			{
				assert_requirements_std(TargetType,is_integral,"Your TargetType is not an integral type.");
			}
			tag_getter(cycle_length);
			feature_implementation TargetType cycle_length(call_requirements(requires(TargetType,Concepts::Is_Time_Seconds)))			
			{
				Interfaces::Phase_Interface<ComponentType::Parent_Type,NULLTYPE>* parent = (Interfaces::Phase_Interface<ComponentType::Parent_Type,NULLTYPE>*)(((ComponentType*)this)->_parent);
				return parent->cycle_length<TargetType>();
			}
			feature_implementation TargetType cycle_length(call_requirements(requires(TargetType,!Concepts::Is_Time_Seconds)))			
			{
				assert_requirements_std(TargetType,Concepts::Is_Time_Seconds,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(max_cycle_length);
			feature_implementation TargetType max_cycle_length(call_requirements(requires(TargetType,Concepts::Is_Time_Seconds)))			
			{
				Interfaces::Phase_Interface<ComponentType::Parent_Type,NULLTYPE>* parent = (Interfaces::Phase_Interface<ComponentType::Parent_Type,NULLTYPE>*)(((ComponentType*)this)->_parent);
				return parent->max_cycle_length<TargetType>();
			}
			feature_implementation TargetType max_cycle_length(call_requirements(requires(TargetType,!Concepts::Is_Time_Seconds)))			
			{
				assert_requirements_std(TargetType,Concepts::Is_Time_Seconds,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(min_cycle_length);
			feature_implementation TargetType min_cycle_length(call_requirements(requires(TargetType,Concepts::Is_Time_Seconds)))			
			{
				Interfaces::Phase_Interface<ComponentType::Parent_Type,NULLTYPE>* parent = (Interfaces::Phase_Interface<ComponentType::Parent_Type,NULLTYPE>*)(((ComponentType*)this)->_parent);
				return parent->min_cycle_length<TargetType>();
			}
			feature_implementation TargetType min_cycle_length(call_requirements(requires(TargetType,!Concepts::Is_Time_Seconds)))			
			{
				assert_requirements(TargetType,Concepts::Is_Time_Seconds,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(green_time);
			feature_implementation TargetType green_time(call_requirements(requires(TargetType,Concepts::Is_Time_Seconds)))			
			{
				Interfaces::Phase_Interface<ComponentType::Parent_Type,NULLTYPE>* parent = (Interfaces::Phase_Interface<ComponentType::Parent_Type,NULLTYPE>*)(((ComponentType*)this)->_parent);
				return parent->green_time<TargetType>();
			}
			feature_implementation TargetType green_time(call_requirements(requires(TargetType,!Concepts::Is_Time_Seconds)))			
			{
				assert_requirements(TargetType,Concepts::Is_Time_Seconds,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(yellow_and_all_red_time);
			feature_implementation TargetType yellow_and_all_red_time(call_requirements(requires(TargetType,Concepts::Is_Time_Seconds)))			
			{
				Interfaces::Phase_Interface<ComponentType::Parent_Type,NULLTYPE>* parent = (Interfaces::Phase_Interface<ComponentType::Parent_Type,NULLTYPE>*)(((ComponentType*)this)->_parent);
				return parent->yellow_and_all_red_time<TargetType>();
			}
			feature_implementation TargetType yellow_and_all_red_time(call_requirements(requires(TargetType,!Concepts::Is_Time_Seconds)))			
			{
				assert_requirements(TargetType,Concepts::Is_Time_Seconds,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(analysis_period);
			feature_implementation TargetType analysis_period(call_requirements(requires(TargetType,Concepts::Is_Time_Minutes)))			
			{
				Interfaces::Phase_Interface<ComponentType::Parent_Type,NULLTYPE>* parent = (Interfaces::Phase_Interface<ComponentType::Parent_Type,NULLTYPE>*)(((ComponentType*)this)->_parent);
				return parent->analysis_period<TargetType>();
			}
			feature_implementation TargetType analysis_period(call_requirements(requires(TargetType,!Concepts::Is_Time_Minutes)))			
			{
				assert_requirements_std(TargetType,is_arithmetic,"Your TargetType is not an arithmetic type.");
			}



			//============================================================
			// Opposing lane_group interface 
			//------------------------------------------------------------
			void* _opposing_lane;
			tag_getter_setter(opposing_lane);
			feature_implementation typename TargetType::Interface_Type<TargetType,CallerType>::type* opposing_lane(call_requirements(requires(ComponentType,Is_Dispatched) && requires_2(ComponentType,TargetType,Is_Same_Component)))			
			{
				return  (typename TargetType::Interface_Type<TargetType,CallerType>::type*) _opposing_lane;
			}
			feature_implementation void opposing_lane(typename TargetType::Interface_Type<TargetType,CallerType>::type* set_value, call_requirements(requires(ComponentType,Is_Dispatched) && requires_2(ComponentType,TargetType,Is_Same_Component)))			
			{
				_opposing_lane = (void*) set_value;
			}
			//member_component_basic(Components::HCM_LaneGroup_Simple,opposing_lane);


			//============================================================
			// Detector information
			//------------------------------------------------------------
			member_component_basic(typename MasterType::DETECTOR_TYPE,Detector_Left);
			member_component_basic(typename MasterType::DETECTOR_TYPE,Detector_Right);
			member_component_basic(typename MasterType::DETECTOR_TYPE,Detector_Thru);


			//============================================================
			// Common Geometry conditions 
			//------------------------------------------------------------
			member_data(int,number_of_lanes,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));		///< N
			member_data(int,number_of_right_lanes,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));	
			member_data(int,number_of_left_lanes,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));	

			//============================================================
			/// Lane width data member
			//------------------------------------------------------------
			float _avg_lane_width;
			tag_getter_setter(avg_lane_width);
			feature_implementation  void avg_lane_width(TargetType set_value,call_requirements(requires(ComponentType,Is_Dispatched) && requires(TargetType,Concepts::Is_Foot_Measure)))
			{
				State_Checks::valid_lane_width<ComponentType,CallerType,TargetType>(this,set_value);
				_avg_lane_width=(float)set_value;
			}
			feature_implementation  void avg_lane_width(TargetType set_value,call_requirements(requires(ComponentType,Is_Dispatched) && requires(TargetType,Concepts::Is_Meter_Measure)))
			{
				State_Checks::valid_lane_width<TargetType>(set_value.value * 3.28084);
				_avg_lane_width=(float)set_value.value * 3.28084;
			}
			feature_implementation  void avg_lane_width(TargetType set_value,call_requirements(!(requires(ComponentType,Is_Dispatched) && (requires(TargetType, Concepts::Is_Foot_Measure) || requires(TargetType, Concepts::Is_Meter_Measure)))))
			{
				assert_requirements(ComponentType,Is_Dispatched,"ComponentType is not dispatched");
				assert_requirements(TargetType,Concepts::Is_Foot_Measure,"TargetType is not a foot or meter measure");
			}
			feature_implementation  TargetType avg_lane_width(call_requirements(requires(ComponentType,Is_Dispatched) && requires(TargetType,Concepts::Is_Foot_Measure)))
			{
				return (typename TargetType::ValueType)_avg_lane_width;
			}
			feature_implementation  TargetType avg_lane_width(call_requirements(requires(ComponentType,Is_Dispatched) && requires(TargetType,Concepts::Is_Meter_Measure)))
			{
				return (typename TargetType::ValueType)(_avg_lane_width * 0.3048);
			}
			feature_implementation  TargetType avg_lane_width(call_requirements(!(requires(ComponentType,Is_Dispatched) && (requires(TargetType, Concepts::Is_Foot_Measure) || requires(TargetType, Concepts::Is_Meter_Measure)))))
			{
				assert_requirements(ComponentType,Is_Dispatched,"ComponentType is not dispatched");
				assert_requirements(TargetType,Concepts::Is_Foot_Measure,"TargetType is not a foot or meter measure");
			}

			//============================================================
			// Other geometry data members
			//------------------------------------------------------------
			member_data(bool,has_left_turn,requires(TargetType,is_integral),requires(TargetType,is_integral));			///<
			member_data(Data_Structures::Left_Turn_Types,left_turn_type,requires_2(TargetType,Data_Structures::Left_Turn_Types,is_convertible),requires_2(TargetType,Data_Structures::Left_Turn_Types,is_convertible));		///< Protected, Permitted, Unopposed, None
			member_data(Data_Structures::Turn_Lane_Types,left_turn_lane_type,requires_2(TargetType,Data_Structures::Turn_Lane_Types,is_convertible),requires_2(TargetType,Data_Structures::Turn_Lane_Types,is_convertible));		///< Protected, Permitted, Unopposed, None
			member_data(Data_Structures::Turn_Lane_Types,right_turn_lane_type,requires_2(TargetType,Data_Structures::Turn_Lane_Types,is_convertible),requires_2(TargetType,Data_Structures::Turn_Lane_Types,is_convertible));

			member_data(bool,has_right_turn,requires(TargetType,is_integral),requires(TargetType,is_integral));		///<
			member_data(bool,has_thru_move,requires(TargetType,is_integral),requires(TargetType,is_integral));			///<
			member_data(bool,has_parking,requires(TargetType,is_integral),requires(TargetType,is_integral));			///<

			//============================================================
			// Traffic conditions 
			//------------------------------------------------------------
			member_data(float,demand_left,requires(TargetType,Concepts::Is_Flow_Per_Hour),requires(TargetType,Concepts::Is_Flow_Per_Hour));			///< V_l (veh/h)
			member_data(float,demand_right,requires(TargetType,Concepts::Is_Flow_Per_Hour),requires(TargetType,Concepts::Is_Flow_Per_Hour));			///< V_r (veh/h)
			member_data(float,demand_thru,requires(TargetType,Concepts::Is_Flow_Per_Hour),requires(TargetType,Concepts::Is_Flow_Per_Hour));			///< V (veh/h)
			member_data(float,demand_lane_group,requires(TargetType,Concepts::Is_Flow_Per_Hour),requires(TargetType,Concepts::Is_Flow_Per_Hour));		///< Vg (veh/h)
			member_data(float,demand_lane_max,requires(TargetType,Concepts::Is_Flow_Per_Hour),requires(TargetType,Concepts::Is_Flow_Per_Hour));		///< Vg1
			member_data(float,base_saturation_flow,requires(TargetType,Concepts::Is_Flow_Per_Hour),requires(TargetType,Concepts::Is_Flow_Per_Hour));	///< s_o (pc/h/ln)
			member_data(float,peak_hour_factor,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));		///< PHF
			
			//============================================================
			// Signalization conditions 
			//------------------------------------------------------------
			member_data(bool,is_actuated,requires(TargetType,is_integral),requires(TargetType,is_integral));			///< true if actuated, false if pre-timed
			member_data(float,min_pedestrian_green,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));	///< G_p (s)	

		};
		//------------------------------------------------------------------------------------------------------------------
		/// HCM Lane Group Full Base
		/// A signalized intersection Lane Group class which calculates saturated flow rates 
		/// according to HCM 2000 CH16
		//------------------------------------------------------------------------------------------------------------------
		template<typename MasterType>
		struct Lane_Group_HCM_Full_Base : public Lane_Group_HCM_Base<MasterType>
		{
			feature_implementation  void Initialize()
			{
				Lane_Group_HCM_Base::Initialize<ComponentType,CallerType,TargetType>();

			}
			/// Type definitions to define the context of the base class
			typedef Types::Solution_Types::HCM_Full HCM_Full;		///< The solution type applied to this base

			feature_implementation TargetType Calculate_Saturation_Flow_Rate()
			{
				/*s = saturation flow rate for subject lane group, expressed as a total for all lanes in lane group (veh/h)
				 s o = base saturation flow rate per lane (pc/h/ln)
				 N = number of lanes in lane group
				 fw = adjustment factor for lane width
				 fHV = adjustment factor for heavy vehicles in traffic stream
				 f g = adjustment factor for approach grade
				 f p = adjustment factor for existence of a parking lane and parking activity adjacent to lane group
				 f bb = adjustment factor for blocking effect of local buses that stop within intersection area
				 f a = adjustment factor for area type
				 f LU = adjustment factor for lane utilization
				 f LT = adjustment factor for left turns in lane group
				 fRT = adjustment factor for right turns in lane group
				 fLpb = pedestrian adjustment factor for left-turn movements
				 fRpb = pedestrian-bicycle adjustment factor for right-turn movements.*/

				TargetType  s_o = this->base_saturation_flow<ComponentType,CallerType,Data_Structures::Flow_Per_Hour>();
				TargetType  N = this->number_of_lanes<ComponentType,CallerType,TargetType>();
				TargetType  fw = this->HCM_adjustment_fw<ComponentType,CallerType,TargetType>();
				TargetType  fHV = this->HCM_adjustment_fHV<ComponentType,CallerType,TargetType>();
				TargetType  fg = this->HCM_adjustment_fg<ComponentType,CallerType,TargetType>() ;
				TargetType  fp = this->HCM_adjustment_fp<ComponentType,CallerType,TargetType>() ;
				TargetType  fbb = this->HCM_adjustment_fbb<ComponentType,CallerType,TargetType>() ;
				TargetType  fa = this->HCM_adjustment_fa<ComponentType,CallerType,TargetType>() ;
				TargetType  fLU = this->HCM_adjustment_fLU<ComponentType,CallerType,TargetType>() ;
				TargetType  fLT = this->HCM_adjustment_fLT<ComponentType,CallerType,TargetType>() ;
				TargetType  fRT = this->HCM_adjustment_fRT<ComponentType,CallerType,TargetType>() ;
				TargetType  fLpb = this->HCM_adjustment_fLpb<ComponentType,CallerType,TargetType>() ;
				TargetType  fRpb = this->HCM_adjustment_fRpb<ComponentType,CallerType,TargetType>();

				TargetType s = s_o * N * fw * fHV * fg * fp * fbb * fa * fLU * fLT * fRT * fLpb * fRpb;

				return s;
			}


			//-------------------------------------
			// Common Geometry conditions specific to Full analysis
			member_data(float,grade,requires(TargetType,Concepts::Is_Percentage),requires(TargetType,Concepts::Is_Percentage));					///< G(%)
			member_data(float,length_left_turn,requires(TargetType,Concepts::Is_Foot_Measure),requires(TargetType,Concepts::Is_Foot_Measure));		///< Ls s (ft)
			member_data(float,length_right_turn,requires(TargetType,Concepts::Is_Foot_Measure),requires(TargetType,Concepts::Is_Foot_Measure));		///< Lr (ft)

			//-------------------------------------
			// Traffic conditions specific to the Full analysis
			member_data(float,peak_hour_left,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));			///< PHF
			member_data(float,peak_hour_right,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));		///< PHF
			member_data(float,peak_hour_thru,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));			///< PHF
			member_data(float,percent_heavy_vehicles,requires(TargetType,Concepts::Is_Percentage),requires(TargetType,Concepts::Is_Percentage)); ///< HV for the entire lane group (%)
			member_data(float,percent_heavy_veh_left,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic)); ///< HV for left turn lanes in the group(%)
			member_data(float,percent_heavy_veh_right,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));///< HV for right turn lanes in the group(%)
			member_data(float,percent_heavy_veh_thru,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));	///< HV for through lanes in teh group(%)
			member_data(float,pedestrian_flow_rate,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));	///< v_ped (p/h)
			member_data(float,buses_per_hour,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));			///< N_b (buses/h)
			member_data(float,parking_activity,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));		///< Nm (maneuvers/h)
			member_data(float,arrival_type,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));			///< AT
			member_data(float,percent_arrive_on_green,requires(TargetType,Concepts::Is_Percentage),requires(TargetType,Concepts::Is_Percentage));///< P
			member_data(float,approach_speed,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));			///< S_a	(mi/h)

			//-------------------------------------
			// Signalization conditions specific to the Full analysis
			member_data(bool,has_pedestrian_button,requires(TargetType,is_integral),requires(TargetType,is_integral));	///< true if has pedestrian push button

			// adjustment factor for lane width
			feature_implementation float HCM_adjustment_fw()
			{				
				float width = ((Lane_Group_HCM_Base<MasterType>*)this)->avg_lane_width<ComponentType,CallerType,Data_Structures::Length_Foot>();
				State_Checks::valid_lane_width<ComponentType,CallerType, TargetType>(this, width);
				return (1.0f + (width - 12.0f)/30.0f);
			}
			// adjustment factor for heavy vehicles in traffic stream
			feature_implementation float HCM_adjustment_fHV()
			{
				return 100.0f/(100.0f + this->percent_heavy_vehicles<ComponentType,CallerType,Data_Structures::Percentage>());
			}
			// adjustment factor for approach grade
			feature_implementation float HCM_adjustment_fg()
			{
				float grade = this->grade<ComponentType,CallerType,Data_Structures::Percentage>();
				State_Checks::valid_grade<ComponentType,CallerType,TargetType>(this, grade);
				return 1.0f - grade / 200.0f;
			}
			// adjustment factor for existence of a parking lane and parking activity adjacent to lane group
			feature_implementation float HCM_adjustment_fp()
			{
				float parking = this->parking_activity<ComponentType,CallerType,float>();
				State_Checks::valid_parking_manuevers<ComponentType,CallerType,TargetType>(this, parking);
				if (parking == 0.0f) return 1.0f;
				float lanes = THIS->number_of_lanes<float>();
				return (lanes - 0.1f - 18.0f * parking /3600.0f)/lanes;
			}
			// adjustment factor for blocking effect of local buses that stop within intersection area
			feature_implementation float HCM_adjustment_fbb()
			{
				float buses = THIS->buses_per_hour<float>();
				State_Checks::valid_num_bus_stops<ComponentType,CallerType,TargetType>(this, buses);
				float lanes = THIS->number_of_lanes<float>();
				return (lanes - 14.4f * buses /3600.0f)/lanes;
			}
			// adjustment factor for area type
			feature_implementation float HCM_adjustment_fa()
			{
				bool in_cbd = THIS->in_CBD<bool>();
				if (in_cbd) return 0.9f;
				else return 1.0f;
			}
			//  adjustment factor for lane utilization
			feature_implementation float HCM_adjustment_fLU()
			{
				return 1.0f;
				//return THIS->demand_lane_group<Data_Structures::Flow_Per_Hour>() / (THIS->demand_lane_max<Data_Structures::Flow_Per_Hour>() * THIS->number_of_lanes<float>());
			}
			// adjustment factor for left turns in lane group
			feature_implementation float HCM_adjustment_fLT()
			{
				bool has_left, has_right, has_thru;
				Data_Structures::Left_Turn_Types turn_type = THIS->left_turn_type<Data_Structures::Left_Turn_Types>();
				has_left = THIS->has_left_turn<bool>();
				has_right = THIS->has_right_turn<bool>();
				has_thru = THIS->has_thru_move<bool>();

				if (turn_type == Data_Structures::Left_Turn_Types::Protected)
				{
					if (has_left && !has_thru)
					{
						return 0.95f;
					}
					else if (has_left && has_thru)
					{
						return 1.0f / (1.0f + 0.05f * THIS->demand_left<Data_Structures::Flow_Per_Hour>() / THIS->demand_lane_group<Data_Structures::Flow_Per_Hour>());
					}
					else
					{
						return 1.0f;
					}
				}
				return 1.0f;
			}
			// adjustment factor for right turns in lane group
			feature_implementation float HCM_adjustment_fRT()
			{
				bool has_left, has_right, has_thru;
				has_left = THIS->has_left_turn<bool>();
				has_right = THIS->has_right_turn<bool>();
				has_thru = THIS->has_thru_move<bool>();

				if (has_right && !has_thru)
				{
					return 0.85f;
				}
				else if (has_right && has_thru)
				{
					int nlane = THIS->number_of_lanes<int>();
					if (nlane > 1)
					{
						return 1.0f - 0.15f * THIS->demand_right<Data_Structures::Flow_Per_Hour>() / THIS->demand_lane_group<Data_Structures::Flow_Per_Hour>();
					}
					else
					{
						return 1.0f - 0.135f * THIS->demand_right<Data_Structures::Flow_Per_Hour>() / THIS->demand_lane_group<Data_Structures::Flow_Per_Hour>();
					}
				}
				else
				{
					return 1.0f;
				}

			}
			// pedestrian adjustment factor for left-turn movements
			feature_implementation float HCM_adjustment_fLpb()
			{
				return 1.0f;
			}
			// pedestrian-bicycle adjustment factor for right-turn movements
			feature_implementation float HCM_adjustment_fRpb()
			{
				return 1.0f;
			}

		};
		//------------------------------------------------------------------------------------------------------------------
		/// HCM Lane Group Simple Base.
		/// A signalized intersection Lane Group class which calculates saturated flow rates 
		/// according to HCM 2000 CH10 - simplified method
		//------------------------------------------------------------------------------------------------------------------
		template<typename MasterType>
		struct Lane_Group_HCM_Simple_Base : public Lane_Group_HCM_Base<MasterType>
		{
			/// Type definitions to define the context of the base class
			typedef Types::Solution_Types::HCM_Simple HCM_Simple;	///< The solution type applied to this base
		};

	

		//==================================================================================================================
		/// Phase of a signal Bases 
		//------------------------------------------------------------------------------------------------------------------
		/// HCM Phase Base class.
		/// Used to store common functionality for HCM Phase bases
		//------------------------------------------------------------------------------------------------------------------
		template<typename MasterType>
		struct Phase_HCM_Base
		{
			feature_implementation void Initialize(TargetType number_of_lane_groups)
			{
				this->_green_time = 0.0;
				this->_yellow_and_all_red_time = 4.0;
				this->_name = "";
				this->_phase_id = 0;
				this->_weight = 1.0f;
			}


			//============================================================
			//  BASIC LOCAL DATA AND ACCESSOR
			//------------------------------------------------------------
			member_data_basic(Data_Structures::Signal_State, signal_state);
			/// Phase green time
			member_data(float, green_time, requires(TargetType, Concepts::Is_Time_Seconds), requires(TargetType, Concepts::Is_Time_Seconds));	///< G (s)
			/// Phase lost time
			member_data(float, yellow_and_all_red_time, requires(TargetType, Concepts::Is_Time_Seconds), requires(TargetType, Concepts::Is_Time_Seconds));	///< Y (s)
			member_data_basic(int, phase_id);
			member_data_basic(char*,name);
			member_data(float, weight, requires(TargetType, is_arithmetic), requires(TargetType, is_arithmetic));
			

			//============================================================
			// CHILD CLASS ACCESS HANDLERS
			//-------------------------------------------------------------
			// Phase lane groups container
			vector<void*> _Lane_Groups; 
			// create a tag that says the getter and setter have been defined - the existence of this tag is checked if the interface accessors are invoked
			tag_getter_setter(Lane_Groups);
			// create a handler for the GET version of the lane groups accessor created in the interface
			feature_implementation TargetType Lane_Groups(call_requirements(requires(ComponentType,Is_Dispatched)))
			{
				return (TargetType) &_Lane_Groups; // return the local data member cast to the TargetType
			}
			// Make sure to create an error handler for the accessors by negating the requirements
			feature_implementation TargetType Lane_Groups(call_requirements(!(requires(ComponentType,Is_Dispatched))))
			{
				assert_requirements(ComponentType,Is_Dispatched,"ComponentType is not dispatched");
				assert_requirements(TargetType, Is_Polaris_Component,"TargetType is not a polaris component");
			}
			

			//============================================================
			//  PARENT CLASS ACCESS HANDLERS
			//------------------------------------------------------------
			tag_getter(in_CBD);
			feature_implementation TargetType in_CBD(call_requirements(requires(TargetType,is_integral)))			
			{
				Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>* parent = (Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>*)(((ComponentType*)this)->_parent);
				return parent->in_CBD<TargetType>();
			}
			feature_implementation TargetType in_CBD(call_requirements(requires(TargetType,!is_integral)))			
			{
				assert_requirements_std(TargetType,is_integral,"Your TargetType is not an integral type.");
			}
			tag_getter(cycle_length);
			feature_implementation TargetType cycle_length(call_requirements(requires(TargetType,Concepts::Is_Time_Seconds)))			
			{
				Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>* parent = (Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>*)(((ComponentType*)this)->_parent);
				return parent->cycle_length<TargetType>();
			}
			feature_implementation TargetType cycle_length(call_requirements(requires(TargetType,!Concepts::Is_Time_Seconds)))			
			{
				assert_requirements(TargetType,Concepts::Is_Time_Seconds,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(max_cycle_length);
			feature_implementation TargetType max_cycle_length(call_requirements(requires(TargetType,Concepts::Is_Time_Seconds)))			
			{
				Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>* parent = (Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>*)(((ComponentType*)this)->_parent);
				return parent->max_cycle_length<TargetType>();
			}
			feature_implementation TargetType max_cycle_length(call_requirements(requires(TargetType,!Concepts::Is_Time_Seconds)))			
			{
				assert_requirements(TargetType,Concepts::Is_Time_Seconds,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(min_cycle_length);
			feature_implementation TargetType min_cycle_length(call_requirements(requires(TargetType,Concepts::Is_Time_Seconds)))			
			{
				Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>* parent = (Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>*)(((ComponentType*)this)->_parent);
				return parent->min_cycle_length<TargetType>();
			}
			feature_implementation TargetType min_cycle_length(call_requirements(requires(TargetType,!Concepts::Is_Time_Seconds)))			
			{
				assert_requirements(TargetType,Concepts::Is_Time_Seconds,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(analysis_period);
			feature_implementation TargetType analysis_period(call_requirements(requires(TargetType,Concepts::Is_Time_Minutes)))			
			{
				Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>* parent = (Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>*)(((ComponentType*)this)->_parent);
				return parent->analysis_period<TargetType>();
			}
			feature_implementation TargetType analysis_period(call_requirements(requires(TargetType,!Concepts::Is_Time_Minutes)))			
			{
				assert_requirements(TargetType,Concepts::Is_Time_Minutes,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(degree_of_saturation);
			feature_implementation TargetType degree_of_saturation(call_requirements(requires(TargetType,is_arithmetic)))			
			{
				Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>* parent = (Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>*)(((ComponentType*)this)->_parent);
				return parent->degree_of_saturation<TargetType>();
			}
			feature_implementation TargetType degree_of_saturation(call_requirements(requires(TargetType,!is_arithmetic)))			
			{
				assert_requirements_std(TargetType,is_arithmetic,"Your TargetType is not an arithmetic type.");
			}	

		};
		//------------------------------------------------------------------------------------------------------------------
		/// HCM Phase class.
		/// A signalized intersection phase class which links to LaneGroup child classes and calculates 
		/// phasing according to HCM 2000 CH16
		//------------------------------------------------------------------------------------------------------------------
		template<typename MasterType>
		struct Phase_HCM_Full_Base : public Phase_HCM_Base<MasterType>
		{
			feature_implementation  void Initialize(TargetType number_of_lane_groups)
			{
				Phase_HCM_Base::Initialize<ComponentType,CallerType,TargetType>(number_of_lane_groups);

				for (int i=0; i<(int)number_of_lane_groups; i++)
				{
					typedef Interfaces::Lane_Group_Interface<typename MasterType::LANE_GROUP_TYPE,NULLTYPE>* ITF_TYPE;
					 ITF_TYPE itf = (ITF_TYPE)Allocate<typename MasterType::LANE_GROUP_TYPE>();
					((typename MasterType::LANE_GROUP_TYPE*)itf)->_parent = PTHIS(ComponentType);
					this->_Lane_Groups.push_back((double*)itf);
				}

			}

			// TYPEDEFS to define the context of the base class
			typedef Types::Solution_Types::HCM_Full HCM_Full;		///< The solution type applied to this base
		};
		//------------------------------------------------------------------------------------------------------------------
		/// HCM Phase class.
		/// A signalized intersection phase class which links to LaneGroup child classes and calculates 
		/// phasing according to HCM 2000 CH10 - simplified method
		//------------------------------------------------------------------------------------------------------------------
		template<typename MasterType>
		struct Phase_HCM_Simple_Base : public Phase_HCM_Base<MasterType>
		{
			feature_implementation void Initialize(TargetType number_of_lane_groups)
			{
				Phase_HCM_Base::Initialize<ComponentType,CallerType,TargetType>(number_of_lane_groups);

				for (int i=0; i<(int)number_of_lane_groups; i++)
				{
					ComponentType::Lane_Group_Interface<Lane_Group_Type<Execution_Object,typename ComponentType::This_Type>::type,NULLTYPE>::type* itf = (ComponentType::Lane_Group_Interface<Lane_Group_Type<Execution_Object,typename ComponentType::This_Type>::type,NULLTYPE>::type*)Allocate<Lane_Group_Type<Execution_Object, typename ComponentType::This_Type>::type>();
					((Lane_Group_Type<Execution_Object,typename ComponentType::This_Type>::type*)itf)->_parent = PTHIS(typename ComponentType::This_Type);
					this->_Lane_Groups.push_back((double*)itf);
				}

			}

			/// Type definitions to define the context of the base class
			typedef Types::Solution_Types::HCM_Simple HCM_Simple;	///< The solution type applied to this base
		};



		//==================================================================================================================
		/// Approach of a signal Base
		//------------------------------------------------------------------------------------------------------------------
		/// HCM Approach Base class.
		/// Used to store common functionality for HCM Approach base
		//------------------------------------------------------------------------------------------------------------------
		template<typename MasterType>
		struct Approach_HCM_Base
		{
			feature_implementation void Initialize(TargetType number_of_lane_groups)
			{
				this->_approach_flow_rate = 0.0;
				this->_delay = 0.0;
				this->_LOS = 'A';
				this->_name = "";
			}
			feature_implementation void Add_Lane_Group(typename TargetType::Interface_Type<TargetType,NULLTYPE>::type* lane_group)
			{
				this->_Lane_Groups.push_back(lane_group);
			}


			//============================================================
			//  BASIC LOCAL DATA AND ACCESSOR
			//------------------------------------------------------------
			member_data(Data_Structures::Flow_Per_Hour, approach_flow_rate, requires(TargetType, Concepts::Is_Flow_Per_Hour), requires(TargetType, Concepts::Is_Flow_Per_Hour));
			/// Phase green time
			member_data(Data_Structures::Time_Second, delay, requires(TargetType, Concepts::Is_Time_Seconds), requires(TargetType, Concepts::Is_Time_Seconds));	///< G (s)
			/// Phase lost time
			member_data(char, LOS, requires(TargetType, is_integral), requires(TargetType, is_integral));	///< Y (s)
			/// Phase lost time
			member_data_basic(char*, name);	///< Y (s)

			//============================================================
			// CHILD CLASS ACCESS HANDLERS
			//-------------------------------------------------------------
			// Phase lane groups container
			vector<void*> _Lane_Groups; 
			// create a tag that says the getter and setter have been defined - the existence of this tag is checked if the interface accessors are invoked
			tag_getter_setter(Lane_Groups);
			// create a handler for the GET version of the lane groups accessor created in the interface
			feature_implementation TargetType Lane_Groups(call_requirements(requires(ComponentType,Is_Dispatched)))
			{
				return (TargetType) &_Lane_Groups; // return the local data member cast to the TargetType
			}
			// Make sure to create an error handler for the accessors by negating the requirements
			feature_implementation TargetType Lane_Groups(call_requirements(!(requires(ComponentType,Is_Dispatched))))
			{
				assert_requirements(ComponentType,Is_Dispatched,"ComponentType is not dispatched");
				assert_requirements(TargetType, Is_Polaris_Component,"TargetType is not a polaris component");
			}
			

			//============================================================
			//  PARENT CLASS ACCESS HANDLERS
			//------------------------------------------------------------
			tag_getter(in_CBD);
			feature_implementation TargetType in_CBD(call_requirements(requires(TargetType,is_integral)))			
			{
				Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>* parent = (Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>*)(((ComponentType*)this)->_parent);
				return parent->in_CBD<TargetType>();
			}
			feature_implementation TargetType in_CBD(call_requirements(requires(TargetType,!is_integral)))			
			{
				assert_requirements_std(TargetType,is_integral,"Your TargetType is not an integral type.");
			}
			tag_getter(cycle_length);
			feature_implementation TargetType cycle_length(call_requirements(requires(TargetType,is_arithmetic)))			
			{
				Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>* parent = (Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>*)(((ComponentType*)this)->_parent);
				return parent->cycle_length<TargetType>();
			}
			feature_implementation TargetType cycle_length(call_requirements(requires(TargetType,!is_arithmetic)))			
			{
				assert_requirements_std(TargetType,is_arithmetic,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(max_cycle_length);
			feature_implementation TargetType max_cycle_length(call_requirements(requires(TargetType,Concepts::Is_Time_Seconds)))			
			{
				Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>* parent = (Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>*)(((ComponentType*)this)->_parent);
				return parent->max_cycle_length<TargetType>();
			}
			feature_implementation TargetType max_cycle_length(call_requirements(requires(TargetType,!Concepts::Is_Time_Seconds)))			
			{
				assert_requirements_std(TargetType,Concepts::Is_Time_Seconds,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(min_cycle_length);
			feature_implementation TargetType min_cycle_length(call_requirements(requires(TargetType,is_arithmetic)))			
			{
				Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>* parent = (Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>*)(((ComponentType*)this)->_parent);
				return parent->min_cycle_length<TargetType>();
			}
			feature_implementation TargetType min_cycle_length(call_requirements(requires(TargetType,!is_arithmetic)))			
			{
				assert_requirements_std(TargetType,is_arithmetic,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(analysis_period);
			feature_implementation TargetType analysis_period(call_requirements(requires(TargetType,Concepts::Is_Time_Minutes)))			
			{
				Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>* parent = (Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>*)(((ComponentType*)this)->_parent);
				return parent->analysis_period<TargetType>();
			}
			feature_implementation TargetType analysis_period(call_requirements(requires(TargetType,!Concepts::Is_Time_Minutes)))			
			{
				assert_requirements_std(TargetType,is_arithmetic,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(degree_of_saturation);
			feature_implementation TargetType degree_of_saturation(call_requirements(requires(TargetType,is_arithmetic)))			
			{
				Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>* parent = (Interfaces::Signal_Interface<ComponentType::Parent_Type,NULLTYPE>*)(((ComponentType*)this)->_parent);
				return parent->degree_of_saturation<TargetType>();
			}
			feature_implementation TargetType degree_of_saturation(call_requirements(requires(TargetType,!is_arithmetic)))			
			{
				assert_requirements_std(TargetType,is_arithmetic,"Your TargetType is not an arithmetic type.");
			}	

		};



		//==================================================================================================================
		/// Signal Bases 
		//------------------------------------------------------------------------------------------------------------------
		/// HCM_Signal class.
		/// Used to store common base functionality amonst all HCM signal bases
		//------------------------------------------------------------------------------------------------------------------
		template<typename MasterType>
		struct Signal_HCM_Base
		{
			//===========================================================
			// SIGNAL event info
			//-----------------------------------------------------------
			member_data_basic(int, Next_Event_Iteration);
			member_data_basic(int, Next_Timing_Event_Iteration);
			member_data_basic(bool, Event_Has_Fired);
			member_data_basic(bool, Timing_Event_Has_Fired);
			member_data_basic(bool, Event_Conditional_Hit);
			member_data_basic(bool, Timing_Event_Conditional_Hit);
			member_data_basic(std::ostream*, output_stream);
			

			//============================================================
			// CHILD CLASS ACCESS HANDLERS
			//-------------------------------------------------------------
			// Create Phases data member
			vector<void*> _Phases; 
			// create a tag that says the getter and setter have been defined - the existence of this tag is checked if the interface accessors are invoked
			tag_getter(Phases);
			// create a handler for the GET version of the ACCESSOR_NAME accessor created in the interface
			feature_implementation TargetType Phases(call_requirements(requires(ComponentType,Is_Dispatched)))
			{
				//(vector<typename TargetType::Interface_Type<TargetType,CallerType>*>&)
				return (TargetType) &_Phases; // return the local data member cast to the TargetType
			}
			// Make sure to create an error handler for the accessors by negating the requirements
			feature_implementation TargetType Phases(call_requirements(!(requires(ComponentType,Is_Dispatched))))
			{
				assert_requirements(ComponentType,Is_Dispatched,"ComponentType is not dispatched");
				assert_requirements(TargetType,Is_Dispatched,"TargetType is not polaris component");
			}
			
			// Create Approaches data member
			vector<void*> _Approaches; 
			// create a tag that says the getter and setter have been defined - the existence of this tag is checked if the interface accessors are invoked
			tag_getter(Approaches);
			// create a handler for the GET version of the ACCESSOR_NAME accessor created in the interface
			feature_implementation TargetType Approaches(call_requirements(requires(ComponentType,Is_Dispatched)))
			{
				return (TargetType) &_Approaches; // return the local data member cast to the TargetType
			}
			// Make sure to create an error handler for the accessors by negating the requirements
			feature_implementation TargetType Approaches(call_requirements(!(requires(ComponentType,Is_Dispatched))))
			{
				assert_requirements(ComponentType,Is_Dispatched,"ComponentType is not dispatched");
				assert_requirements(TargetType,Is_Dispatched,"TargetType is not polaris component");
			}


			//==========================================================================================
			// Signal local data members
			//------------------------------------------------------------------------------------------
			/// Number of cycles to run before the timing plan is updated, currently defaults to infinity (no timing updates)
			member_data(int, num_cycles_between_updates,requires(TargetType, is_integral),requires(TargetType,is_integral));
			/// Currently active phase
			member_data(int, active_phase,requires(TargetType, is_integral),requires(TargetType,is_integral));
			/// Total cycle length
			member_data(float,cycle_length,requires(TargetType,Concepts::Is_Time_Seconds),requires(TargetType,Concepts::Is_Time_Seconds));			///< C (s)
			/// maximum cycle length
			member_data(float,max_cycle_length,requires(TargetType,Concepts::Is_Time_Seconds),requires(TargetType,Concepts::Is_Time_Seconds));	
			/// minimum cycle length
			member_data(float,min_cycle_length,requires(TargetType,Concepts::Is_Time_Seconds),requires(TargetType,Concepts::Is_Time_Seconds));	
			/// target degree of saturation (default of 0.9)
			member_data(float,degree_of_saturation,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));	///< X (s)
			/// Area type
			member_data(bool,in_CBD,requires(TargetType,is_integral),requires(TargetType,is_integral));/// 0.9 for CBD otherwise 1.0
			/// Signal ID accessor
			member_data(float,Signal_ID,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));
			/// Signal name accessor
			member_data(char*,name,true,true);
			/// Analyisis Period accessor
			member_data(float,analysis_period,requires(TargetType,Concepts::Is_Time_Minutes),requires(TargetType,Concepts::Is_Time_Minutes));
			/// Peak hour factor used to convert hourly to 15 minute maximum volume
			member_data(float,peak_hour_factor,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));
			/// Overall Signal Level of service
			member_data(char,LOS,requires(TargetType,is_integral), requires(TargetType, is_integral));
			/// Average intersection delay in seconds / vehicle
			member_data(char,delay,requires(TargetType,Concepts::Is_Time_Seconds), requires(TargetType, Concepts::Is_Time_Seconds));
		};
		//------------------------------------------------------------------------------------------------------------------
		/// HCM_Signal class.
		/// A signalized intersection base class which links to phase child classes and recalculates signal timing accoriding to HCM 2000 ch16
		//------------------------------------------------------------------------------------------------------------------
		template<typename MasterType>
		struct Signal_HCM_Full_Base : public Signal_HCM_Base<MasterType>
		{
			/// Type definitions to define the context of the base class
			typedef Types::Solution_Types::HCM_Full HCM_Full;	///< The solution type applied to this base

			/// Handler for a general Initializer dispatched from an Interface
			feature_implementation void Initialize(TargetType number_of_phases, TargetType number_of_approaches, call_requirements(requires(ComponentType,Is_Dispatched)))
			{
				Signal_HCM_Base<MasterType>* _this = ((Signal_HCM_Base<MasterType>*)this);
				this->_Signal_ID = 0;
				this->_name = "";
				this->_output_stream = &cout;
				this->_in_CBD = false;
				this->_cycle_length = 0.0;
				this->_degree_of_saturation = 0.9;
				
				for (int i=0; i<(int)number_of_phases; i++)
				{
					typedef Interfaces::Phase_Interface<typename MasterType::PHASE_TYPE, NULLTYPE>* ITF_TYPE;
					ITF_TYPE itf= (ITF_TYPE)Allocate<typename MasterType::PHASE_TYPE>();
					((typename MasterType::PHASE_TYPE*)itf)->_parent = PTHIS(ComponentType);

					_Phases.push_back((void*)itf);
				}
				for (int i=0; i<(int)number_of_approaches; i++)
				{
					typedef Interfaces::Approach_Interface<typename MasterType::APPROACH_TYPE,NULLTYPE>* ITF_TYPE;
					ITF_TYPE itf= (ITF_TYPE)Allocate<typename MasterType::APPROACH_TYPE>();
					((typename MasterType::APPROACH_TYPE*)itf)->_parent = PTHIS(ComponentType);

					_Approaches.push_back((void*)itf);
				}
			}
		};
		//------------------------------------------------------------------------------------------------------------------
		/// HCM_Signal class.
		/// A Simple signalized intersection base class which links to phase child classes and recalculates signal timing accoriding to HCM 2000 ch 10
		//------------------------------------------------------------------------------------------------------------------	
		template<typename MasterType>
		struct Signal_HCM_Simple_Base : public Signal_HCM_Base<MasterType>
		{
			/// Type definitions to define the context of the base class
			typedef Types::Solution_Types::HCM_Simple HCM_Simple;	///< The solution type applied to this base


			/// Handler for a general Initializer dispatched from an Interface
			feature_implementation void Initialize(TargetType number_of_phases, TargetType number_of_approaches, call_requirements(requires(ComponentType,Is_Dispatched)))
			{
				Signal_HCM_Base<MasterType>* _this = ((Signal_HCM_Base<MasterType>*)this);
				
				// Set member variables to defaults
				this->_Signal_ID = 0;
				this->_name = "";
				this->_output_stream = &cout;
				this->_in_CBD = false;
				this->_cycle_length = 0.0;
				this->_degree_of_saturation = 0.9;

				// Create new phases according to specified number of phases
				for (int i=0; i<(int)number_of_phases; i++)
				{
					
					ComponentType::Phase_Interface<Phase_Type<Execution_Object, typename ComponentType::This_Type>::type,NULLTYPE>::type* itf= (ComponentType::Phase_Interface<Phase_Type<Execution_Object, typename ComponentType::This_Type>::type,NULLTYPE>::type*)Allocate<Phase_Type<Execution_Object, typename ComponentType::This_Type>::type>();
					((Phase_Type<Execution_Object, typename ComponentType::This_Type>::type*)itf)->_parent = PTHIS(ComponentType::This_Type);

					_Phases.push_back((void*)itf);
				}
				for (int i=0; i<(int)number_of_approaches; i++)
				{
					typedef ComponentType::Approach_Interface<Approach_Type<Execution_Object, typename ComponentType::This_Type>::type,NULLTYPE>::type* ITF_TYPE;
					ITF_TYPE itf= (ITF_TYPE)Allocate<Approach_Type<Execution_Object, typename ComponentType::This_Type>::type>();
					((Approach_Type<Execution_Object, typename ComponentType::This_Type>::type*)itf)->_parent = PTHIS(ComponentType::This_Type);

					_Approaches.push_back((void*)itf);
				}

			}
		};

	}
	//==================================================================================================================
	/// SIGNAL ELEMENTS COMPONENTS Namespace:  Namespace for the creation of All signal components
	//------------------------------------------------------------------------------------------------------------------
	namespace Components
	{
		//------------------
		// Signals
		template<typename MasterType>
		struct HCM_Signal_Full
		{
			typedef Polaris_Component_Execution<Interfaces::Signal_Interface, Bases::Signal_HCM_Full_Base<MasterType>,NULLTYPE,MasterType> type;
		};
		template<typename MasterType>
		struct HCM_Signal_Simple
		{
			typedef Polaris_Component_Execution<Interfaces::Signal_Interface, Bases::Signal_HCM_Simple_Base<MasterType>,NULLTYPE,MasterType> type;
		};

		//------------------
		// Approaches
		template<typename MasterType>
		struct HCM_Approach_Full
		{
			typedef Polaris_Component_Execution<Interfaces::Approach_Interface, Bases::Approach_HCM_Base<MasterType>,typename HCM_Signal_Full<MasterType>::type,MasterType> type;
		};
		template<typename MasterType>
		struct HCM_Approach_Simple
		{
			typedef Polaris_Component_Execution<Interfaces::Approach_Interface, Bases::Approach_HCM_Base<MasterType>,typename HCM_Signal_Simple<MasterType>::type,MasterType> type;
		};

		//------------------
		// Phases
		template<typename MasterType>
		struct HCM_Phase_Full
		{
			typedef Polaris_Component_Execution<Interfaces::Phase_Interface, Bases::Phase_HCM_Full_Base<MasterType>,typename HCM_Signal_Full<MasterType>::type,MasterType> type;
		};
		template<typename MasterType>
		struct HCM_Phase_Simple
		{
			typedef Polaris_Component_Execution<Interfaces::Phase_Interface, Bases::Phase_HCM_Simple_Base<MasterType>,typename HCM_Signal_Simple<MasterType>::type,MasterType> type;
		};

		//------------------
		// Lane Groups
		template<typename MasterType>
		struct HCM_LaneGroup_Full
		{
			typedef Polaris_Component_Execution<Interfaces::Lane_Group_Interface, Bases::Lane_Group_HCM_Full_Base<MasterType>,typename HCM_Phase_Full<MasterType>::type,MasterType> type;
		};
		template<typename MasterType>
		struct HCM_LaneGroup_Simple
		{
			typedef Polaris_Component_Execution<Interfaces::Lane_Group_Interface, Bases::Lane_Group_HCM_Simple_Base<MasterType>,typename HCM_Phase_Simple<MasterType>::type,MasterType> type;
		};
	}	



	//==================================================================================================================
	/// INDICATOR BASE Namespace of Base classes and Components related to the Interface(s) from the component namespace.
	//------------------------------------------------------------------------------------------------------------------
	namespace Bases
	{
		//==================================================================================================================
		/// Signal Indicator Base
		//------------------------------------------------------------------------------------------------------------------
		template <typename MasterType>
		struct Signal_Indicator_Display_Base
		{
			feature_implementation void Initialize(call_requirements(requires(ComponentType,Is_Dispatched)))
			{
				this->_output_stream = (ofstream*)&cout;
				this->_signal = NULL;
			}

			member_data_basic(bool,Conditional_Has_Fired);
			member_data_basic(ostream*, output_stream);

			// Local data member for signal interface
			void* _signal;
			tag_getter_setter(Signal);
			feature_implementation void Signal(typename TargetType::Interface_Type<TargetType,CallerType>::type* set_value, call_requires(TargetType,Is_Polaris_Component))
			{
				_signal = (void*)set_value;
			}
			feature_implementation typename TargetType::Interface_Type<TargetType,NULLTYPE>::type* Signal(call_requires(TargetType,Is_Polaris_Component))
			{
				return (typename TargetType::Interface_Type<TargetType,NULLTYPE>::type*)_signal;
			}
		};

		template <typename MasterType>
		struct Signal_Indicator_Base
		{
			feature_implementation void Initialize(call_requirements(requires(ComponentType,Is_Dispatched)))
			{
				this->_output_stream = (ostream*)&cout;
				this->_signal = NULL;
			}

			member_data_basic(ostream*, output_stream);
			member_component_basic(typename MasterType::SIGNAL_TYPE,Signal);

			// Local data member for signal interface
			void* _signal;
			tag_getter_setter(Signal);
			feature_implementation void Signal(typename TargetType::Interface_Type<TargetType,CallerType>::type* set_value, call_requires(TargetType,Is_Polaris_Component))
			{
				_signal = (void*)set_value;
			}
			feature_implementation typename TargetType::Interface_Type<TargetType,NULLTYPE>::type* Signal(call_requires(TargetType,Is_Polaris_Component))
			{
				return (typename TargetType::Interface_Type<TargetType,NULLTYPE>::type*)_signal;
			}

		};

	}
	//==================================================================================================================
	/// INDICATOR COMPONENT Namespace:  Namespace for the creation of All signal components
	//------------------------------------------------------------------------------------------------------------------
	namespace Components
	{
		template<typename MasterType>
		struct Signal_Indicator_Display
		{
			typedef Polaris_Component_Execution<Interfaces::Signal_Indicator_Interface, Bases::Signal_Indicator_Display_Base<MasterType>,NULLTYPE,MasterType> type;
		};
		template<typename MasterType>
		struct Signal_Indicator
		{
			typedef Polaris_Component<Interfaces::Signal_Indicator_Interface, Bases::Signal_Indicator_Base<MasterType>,NULLTYPE,MasterType> type;
		};
	}	
}
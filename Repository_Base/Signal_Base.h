#pragma once

#include "Signal_Interfaces.h"


namespace Signal_Components
{
	struct HCM_Signal_Full;
	struct HCM_Signal_Simple;

	//------------------
	// Phases
	struct HCM_Phase_Full;
	struct HCM_Phase_Simple;

	//------------------
	// Lane Groups
	struct HCM_LaneGroup_Full;
	struct HCM_LaneGroup_Simple;


	//==================================================================================================================
	/// Namespace of Base classes and Components related to the Interface(s) from the component namespace.
	/// The bases are combined with the interfaces to create components by adding data.
	//------------------------------------------------------------------------------------------------------------------

	//==================================================================================================================
	/// Lane Group Bases
	//------------------------------------------------------------------------------------------------------------------
	namespace Bases
	{
		//==================================================================================================================
		/// HCM Lane Group BASE class.
		/// Holds common data for all HCM Lanegroup Bases
		//------------------------------------------------------------------------------------------------------------------
		struct Lane_Group_HCM_Base
		{
			//============================================================
			//  Lane Group Initializer
			//------------------------------------------------------------
			facet_base void Initialize()
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
			}


			//============================================================
			//  PARENT CLASS ACCESS HANDLERS
			//------------------------------------------------------------
			tag_getter(in_CBD);
			facet_base TargetType in_CBD(call_requirements(requires(TargetType,is_integral)))			
			{
				Interfaces::Phase_Interface<ThisType::Parent_Type,NULLTYPE>* parent = (Interfaces::Phase_Interface<ThisType::Parent_Type,NULLTYPE>*)(((ThisType*)this)->_parent);
				return parent->in_CBD<TargetType>();
			}
			facet_base TargetType in_CBD(call_requirements(requires(TargetType,!is_integral)))			
			{
				assert_requirements_std(TargetType,is_integral,"Your TargetType is not an integral type.");
			}
			tag_getter(cycle_length);
			facet_base TargetType cycle_length(call_requirements(requires(TargetType,is_arithmetic)))			
			{
				Interfaces::Phase_Interface<ThisType::Parent_Type,NULLTYPE>* parent = (Interfaces::Phase_Interface<ThisType::Parent_Type,NULLTYPE>*)(((ThisType*)this)->_parent);
				return parent->cycle_length<TargetType>();
			}
			facet_base TargetType cycle_length(call_requirements(requires(TargetType,!is_arithmetic)))			
			{
				assert_requirements_std(TargetType,is_arithmetic,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(max_cycle_length);
			facet_base TargetType max_cycle_length(call_requirements(requires(TargetType,Concepts::Is_Time_Seconds)))			
			{
				Interfaces::Phase_Interface<ThisType::Parent_Type,NULLTYPE>* parent = (Interfaces::Phase_Interface<ThisType::Parent_Type,NULLTYPE>*)(((ThisType*)this)->_parent);
				return parent->max_cycle_length<TargetType>();
			}
			facet_base TargetType max_cycle_length(call_requirements(requires(TargetType,!Concepts::Is_Time_Seconds)))			
			{
				assert_requirements_std(TargetType,is_arithmetic,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(min_cycle_length);
			facet_base TargetType min_cycle_length(call_requirements(requires(TargetType,is_arithmetic)))			
			{
				Interfaces::Phase_Interface<ThisType::Parent_Type,NULLTYPE>* parent = (Interfaces::Phase_Interface<ThisType::Parent_Type,NULLTYPE>*)(((ThisType*)this)->_parent);
				return parent->min_cycle_length<TargetType>();
			}
			facet_base TargetType min_cycle_length(call_requirements(requires(TargetType,!is_arithmetic)))			
			{
				assert_requirements_std(TargetType,is_arithmetic,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(green_time);
			facet_base TargetType green_time(call_requirements(requires(TargetType,is_arithmetic)))			
			{
				Interfaces::Phase_Interface<ThisType::Parent_Type,NULLTYPE>* parent = (Interfaces::Phase_Interface<ThisType::Parent_Type,NULLTYPE>*)(((ThisType*)this)->_parent);
				return parent->green_time<TargetType>();
			}
			facet_base TargetType green_time(call_requirements(requires(TargetType,!is_arithmetic)))			
			{
				assert_requirements_std(TargetType,is_arithmetic,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(yellow_and_all_red_time);
			facet_base TargetType yellow_and_all_red_time(call_requirements(requires(TargetType,is_arithmetic)))			
			{
				Interfaces::Phase_Interface<ThisType::Parent_Type,NULLTYPE>* parent = (Interfaces::Phase_Interface<ThisType::Parent_Type,NULLTYPE>*)(((ThisType*)this)->_parent);
				return parent->yellow_and_all_red_time<TargetType>();
			}
			facet_base TargetType yellow_and_all_red_time(call_requirements(requires(TargetType,!is_arithmetic)))			
			{
				assert_requirements_std(TargetType,is_arithmetic,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(analysis_period);
			facet_base TargetType analysis_period(call_requirements(requires(TargetType,is_arithmetic)))			
			{
				Interfaces::Phase_Interface<ThisType::Parent_Type,NULLTYPE>* parent = (Interfaces::Phase_Interface<ThisType::Parent_Type,NULLTYPE>*)(((ThisType*)this)->_parent);
				return parent->analysis_period<TargetType>();
			}
			facet_base TargetType analysis_period(call_requirements(requires(TargetType,!is_arithmetic)))			
			{
				assert_requirements_std(TargetType,is_arithmetic,"Your TargetType is not an arithmetic type.");
			}

			//-------------------------------------
			// Opposing lane_group interface 
			void* _opposing_lane;
			tag_getter_setter(opposing_lane);
			facet_base typename TargetType::Interface_Type<TargetType,CallerType>::type* opposing_lane(call_requirements(requires(ThisType,Is_Dispatched) && requires_2(ThisType,TargetType,Is_Same_Component)))			
			{
				return  (typename TargetType::Interface_Type<TargetType,CallerType>::type*) _opposing_lane;
			}
			facet_base void opposing_lane(typename TargetType::Interface_Type<TargetType,CallerType>::type* set_value, call_requirements(requires(ThisType,Is_Dispatched) && requires_2(ThisType,TargetType,Is_Same_Component)))			
			{
				_opposing_lane = (void*) set_value;
			}
			//member_component_basic(Components::HCM_LaneGroup_Simple,opposing_lane);

			//-------------------------------------
			//Common Geometry conditions 
			member_data(int,number_of_lanes,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));		///< N
			member_data(int,number_of_right_lanes,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));	
			member_data(int,number_of_left_lanes,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));	

			/// Lane width data member
			float _avg_lane_width;
			tag_getter_setter(avg_lane_width);
			facet_base  void avg_lane_width(TargetType set_value,call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,Concepts::Is_Foot_Measure)))
			{
				State_Checks::valid_lane_width<ThisType>(this,set_value.value);
				_avg_lane_width=(float)set_value.value;
			}
			facet_base  void avg_lane_width(TargetType set_value,call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,Concepts::Is_Meter_Measure)))
			{
				State_Checks::valid_lane_width<TargetType>(set_value.value * 3.28084);
				_avg_lane_width=(float)set_value.value * 3.28084;
			}
			facet_base  void avg_lane_width(TargetType set_value,call_requirements(!(requires(ThisType,Is_Dispatched) && (requires(TargetType, Concepts::Is_Foot_Measure) || requires(TargetType, Concepts::Is_Meter_Measure)))))
			{
				assert_requirements(ThisType,Is_Dispatched,"ThisType is not dispatched");
				assert_requirements(TargetType,Concepts::Is_Foot_Measure,"TargetType is not a foot or meter measure");
			}
			facet_base  TargetType avg_lane_width(call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,Concepts::Is_Foot_Measure)))
			{
				return (typename TargetType::ValueType)_avg_lane_width;
			}
			facet_base  TargetType avg_lane_width(call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,Concepts::Is_Meter_Measure)))
			{
				return (typename TargetType::ValueType)(_avg_lane_width * 0.3048);
			}
			facet_base  TargetType avg_lane_width(call_requirements(!(requires(ThisType,Is_Dispatched) && (requires(TargetType, Concepts::Is_Foot_Measure) || requires(TargetType, Concepts::Is_Meter_Measure)))))
			{
				assert_requirements(ThisType,Is_Dispatched,"ThisType is not dispatched");
				assert_requirements(TargetType,Concepts::Is_Foot_Measure,"TargetType is not a foot or meter measure");
			}


			/// Other geometry data members
			member_data(bool,has_left_turn,requires(TargetType,is_integral),requires(TargetType,is_integral));			///<
			member_data(Data_Structures::Left_Turn_Types,left_turn_type,requires_2(TargetType,Data_Structures::Left_Turn_Types,is_convertible),requires_2(TargetType,Data_Structures::Left_Turn_Types,is_convertible));		///< Protected, Permitted, Unopposed, None
			member_data(Data_Structures::Turn_Lane_Types,left_turn_lane_type,requires_2(TargetType,Data_Structures::Turn_Lane_Types,is_convertible),requires_2(TargetType,Data_Structures::Turn_Lane_Types,is_convertible));		///< Protected, Permitted, Unopposed, None
			member_data(Data_Structures::Turn_Lane_Types,right_turn_lane_type,requires_2(TargetType,Data_Structures::Turn_Lane_Types,is_convertible),requires_2(TargetType,Data_Structures::Turn_Lane_Types,is_convertible));

			member_data(bool,has_right_turn,requires(TargetType,is_integral),requires(TargetType,is_integral));		///<
			member_data(bool,has_thru_move,requires(TargetType,is_integral),requires(TargetType,is_integral));			///<
			member_data(bool,has_parking,requires(TargetType,is_integral),requires(TargetType,is_integral));			///<

			//-------------------------------------
			//Traffic conditions 
			member_data(float,demand_left,requires(TargetType,Concepts::Is_Flow_Per_Hour),requires(TargetType,Concepts::Is_Flow_Per_Hour));			///< V_l (veh/h)
			member_data(float,demand_right,requires(TargetType,Concepts::Is_Flow_Per_Hour),requires(TargetType,Concepts::Is_Flow_Per_Hour));			///< V_r (veh/h)
			member_data(float,demand_thru,requires(TargetType,Concepts::Is_Flow_Per_Hour),requires(TargetType,Concepts::Is_Flow_Per_Hour));			///< V (veh/h)
			member_data(float,demand_lane_group,requires(TargetType,Concepts::Is_Flow_Per_Hour),requires(TargetType,Concepts::Is_Flow_Per_Hour));		///< Vg (veh/h)
			member_data(float,demand_lane_max,requires(TargetType,Concepts::Is_Flow_Per_Hour),requires(TargetType,Concepts::Is_Flow_Per_Hour));		///< Vg1
			member_data(float,base_saturation_flow,requires(TargetType,Concepts::Is_Flow_Per_Hour),requires(TargetType,Concepts::Is_Flow_Per_Hour));	///< s_o (pc/h/ln)
			member_data(float,peak_hour_factor,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));		///< PHF
			
			//-------------------------------------
			//Signalization conditions 
			member_data(bool,is_actuated,requires(TargetType,is_integral),requires(TargetType,is_integral));			///< true if actuated, false if pre-timed
			member_data(float,min_pedestrian_green,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));	///< G_p (s)	

		};
		/// HCM Lane Group Full Base
		/// A signalized intersection Lane Group class which calculates saturated flow rates 
		/// according to HCM 2000 CH16
		//------------------------------------------------------------------------------------------------------------------
		struct Lane_Group_HCM_Full_Base : public Lane_Group_HCM_Base
		{
			facet_base  void Initialize()
			{
				Lane_Group_HCM_Base::Initialize<ThisType,CallerType,TargetType>();

			}
			/// Type definitions to define the context of the base class
			typedef Types::Solution_Types::HCM_Full HCM_Full;		///< The solution type applied to this base

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

		};
		//------------------------------------------------------------------------------------------------------------------
		/// HCM Lane Group Simple Base.
		/// A signalized intersection Lane Group class which calculates saturated flow rates 
		/// according to HCM 2000 CH10 - simplified method
		//------------------------------------------------------------------------------------------------------------------
		struct Lane_Group_HCM_Simple_Base : public Lane_Group_HCM_Base
		{
			/// Type definitions to define the context of the base class
			typedef Types::Solution_Types::HCM_Simple HCM_Simple;	///< The solution type applied to this base
		};
	}
	

	//==================================================================================================================
	/// Phase of a signal Bases 
	//------------------------------------------------------------------------------------------------------------------
	namespace Bases
	{
		//==================================================================================================================
		/// HCM Phase Base class.
		/// Used to store common functionality for HCM Phase bases
		//------------------------------------------------------------------------------------------------------------------
		struct Phase_HCM_Base
		{
			facet_base void Initialize(TargetType number_of_lane_groups)
			{
				this->_green_time = 0.0;
				this->_yellow_and_all_red_time = 4.0;
			}

			//===========================================================
			// Lane Group Related Typedefs
			//-----------------------------------------------------------
			template<typename ThisType=NULLTYPE,typename CallerType=NULLTYPE>
			struct Lane_Group_Interface
			{
				typedef Interfaces::Lane_Group_Interface<ThisType, CallerType> type;
				typedef double* unknown_component;
			};	


			//============================================================
			//  BASIC LOCAL DATA AND ACCESSOR
			//------------------------------------------------------------
			member_data_basic(Data_Structures::Signal_State, signal_state);
			/// Phase green time
			member_data(float, green_time, requires(TargetType, Concepts::Is_Time_Seconds), requires(TargetType, Concepts::Is_Time_Seconds));	///< G (s)
			/// Phase lost time
			member_data(float, yellow_and_all_red_time, requires(TargetType, Concepts::Is_Time_Seconds), requires(TargetType, Concepts::Is_Time_Seconds));	///< Y (s)
			

			//============================================================
			// CHILD CLASS ACCESS HANDLERS
			//-------------------------------------------------------------
			// Phase lane groups container
			vector<Lane_Group_Interface<>::unknown_component> _Lane_Groups; 
			// create a tag that says the getter and setter have been defined - the existence of this tag is checked if the interface accessors are invoked
			tag_getter_setter(Lane_Groups);
			// create a handler for the GET version of the lane groups accessor created in the interface
			facet_base TargetType Lane_Groups(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType) &_Lane_Groups; // return the local data member cast to the TargetType
			}
			// Make sure to create an error handler for the accessors by negating the requirements
			facet_base TargetType Lane_Groups(call_requirements(!(requires(ThisType,Is_Dispatched))))
			{
				assert_requirements(ThisType,Is_Dispatched,"ThisType is not dispatched");
				assert_requirements(TargetType, Is_Polaris_Component,"TargetType is not a polaris component");
			}
			

			//============================================================
			//  PARENT CLASS ACCESS HANDLERS
			//------------------------------------------------------------
			tag_getter(in_CBD);
			facet_base TargetType in_CBD(call_requirements(requires(TargetType,is_integral)))			
			{
				Interfaces::Signal_Interface<ThisType::Parent_Type,NULLTYPE>* parent = (Interfaces::Signal_Interface<ThisType::Parent_Type,NULLTYPE>*)(((ThisType*)this)->_parent);
				return parent->in_CBD<TargetType>();
			}
			facet_base TargetType in_CBD(call_requirements(requires(TargetType,!is_integral)))			
			{
				assert_requirements_std(TargetType,is_integral,"Your TargetType is not an integral type.");
			}
			tag_getter(cycle_length);
			facet_base TargetType cycle_length(call_requirements(requires(TargetType,is_arithmetic)))			
			{
				Interfaces::Signal_Interface<ThisType::Parent_Type,NULLTYPE>* parent = (Interfaces::Signal_Interface<ThisType::Parent_Type,NULLTYPE>*)(((ThisType*)this)->_parent);
				return parent->cycle_length<TargetType>();
			}
			facet_base TargetType cycle_length(call_requirements(requires(TargetType,!is_arithmetic)))			
			{
				assert_requirements_std(TargetType,is_arithmetic,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(max_cycle_length);
			facet_base TargetType max_cycle_length(call_requirements(requires(TargetType,Concepts::Is_Time_Seconds)))			
			{
				Interfaces::Signal_Interface<ThisType::Parent_Type,NULLTYPE>* parent = (Interfaces::Signal_Interface<ThisType::Parent_Type,NULLTYPE>*)(((ThisType*)this)->_parent);
				return parent->max_cycle_length<TargetType>();
			}
			facet_base TargetType max_cycle_length(call_requirements(requires(TargetType,!Concepts::Is_Time_Seconds)))			
			{
				assert_requirements_std(TargetType,Concepts::Is_Time_Seconds,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(min_cycle_length);
			facet_base TargetType min_cycle_length(call_requirements(requires(TargetType,is_arithmetic)))			
			{
				Interfaces::Signal_Interface<ThisType::Parent_Type,NULLTYPE>* parent = (Interfaces::Signal_Interface<ThisType::Parent_Type,NULLTYPE>*)(((ThisType*)this)->_parent);
				return parent->min_cycle_length<TargetType>();
			}
			facet_base TargetType min_cycle_length(call_requirements(requires(TargetType,!is_arithmetic)))			
			{
				assert_requirements_std(TargetType,is_arithmetic,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(analysis_period);
			facet_base TargetType analysis_period(call_requirements(requires(TargetType,is_arithmetic)))			
			{
				Interfaces::Signal_Interface<ThisType::Parent_Type,NULLTYPE>* parent = (Interfaces::Signal_Interface<ThisType::Parent_Type,NULLTYPE>*)(((ThisType*)this)->_parent);
				return parent->analysis_period<TargetType>();
			}
			facet_base TargetType analysis_period(call_requirements(requires(TargetType,!is_arithmetic)))			
			{
				assert_requirements_std(TargetType,is_arithmetic,"Your TargetType is not an arithmetic type.");
			}
			tag_getter(degree_of_saturation);
			facet_base TargetType degree_of_saturation(call_requirements(requires(TargetType,is_arithmetic)))			
			{
				Interfaces::Signal_Interface<ThisType::Parent_Type,NULLTYPE>* parent = (Interfaces::Signal_Interface<ThisType::Parent_Type,NULLTYPE>*)(((ThisType*)this)->_parent);
				return parent->degree_of_saturation<TargetType>();
			}
			facet_base TargetType degree_of_saturation(call_requirements(requires(TargetType,!is_arithmetic)))			
			{
				assert_requirements_std(TargetType,is_arithmetic,"Your TargetType is not an arithmetic type.");
			}	

		};
		//------------------------------------------------------------------------------------------------------------------
		/// HCM Phase class.
		/// A signalized intersection phase class which links to LaneGroup child classes and calculates 
		/// phasing according to HCM 2000 CH16
		//------------------------------------------------------------------------------------------------------------------
		struct Phase_HCM_Full_Base : public Phase_HCM_Base
		{
			facet_base  void Initialize(TargetType number_of_lane_groups)
			{
				Phase_HCM_Base::Initialize<ThisType,CallerType,TargetType>(number_of_lane_groups);

				for (int i=0; i<(int)number_of_lane_groups; i++)
				{
					ThisType::Lane_Group_Interface<Lane_Group_Type<Execution_Object,typename ThisType::This_Type>::type,NULLTYPE>::type* itf = Allocate<Lane_Group_Type<Execution_Object, typename ThisType::This_Type>::type>();
					((Lane_Group_Type<Execution_Object,typename ThisType::This_Type>::type*)itf)->_parent = PTHIS(typename ThisType::This_Type);
					this->_Lane_Groups.push_back((double*)itf);
				}

			}

			//============================================================
			// TYPEDEFS to define the context of the base class
			//------------------------------------------------------------
			typedef Types::Solution_Types::HCM_Full HCM_Full;		///< The solution type applied to this base
			//typedef Components::HCM_LaneGroup_Full LaneGroupType;	///< The type of the child phase class	

			typedef Bases::Lane_Group_HCM_Full_Base Lane_Group_Base;

			//=============================================================
			// CHILD CLASS TYPE DEFINITION - required to create a child class interface for use in the phase interface
			//-------------------------------------------------------------
			template<typename ObjectType, typename ParentType=NULLTYPE>
			struct Lane_Group_Type
			{
				typedef Polaris_Component<Interfaces::Lane_Group_Interface, Lane_Group_Base,ParentType,typename ParentType::Entity_Tag,ObjectType> type;
			};	
			template<typename ObjectType>
			struct Lane_Group_Type<ObjectType,NULLTYPE>
			{
				typedef Polaris_Component<Interfaces::Lane_Group_Interface, Lane_Group_Base,NULLTYPE,NULLTYPE,ObjectType> type;
			};	

		};
		//------------------------------------------------------------------------------------------------------------------
		/// HCM Phase class.
		/// A signalized intersection phase class which links to LaneGroup child classes and calculates 
		/// phasing according to HCM 2000 CH10 - simplified method
		//------------------------------------------------------------------------------------------------------------------
		struct Phase_HCM_Simple_Base : public Phase_HCM_Base
		{
			facet_base void Initialize(TargetType number_of_lane_groups)
			{
				Phase_HCM_Base::Initialize<ThisType,CallerType,TargetType>(number_of_lane_groups);

				for (int i=0; i<(int)number_of_lane_groups; i++)
				{
					ThisType::Lane_Group_Interface<Lane_Group_Type<Execution_Object,typename ThisType::This_Type>::type,NULLTYPE>::type* itf = Allocate<Lane_Group_Type<Execution_Object, typename ThisType::This_Type>::type>();
					((Lane_Group_Type<Execution_Object,typename ThisType::This_Type>::type*)itf)->_parent = PTHIS(typename ThisType::This_Type);
					this->_Lane_Groups.push_back((double*)itf);
				}

			}

			/// Type definitions to define the context of the base class
			typedef Types::Solution_Types::HCM_Simple HCM_Simple;	///< The solution type applied to this base


			typedef Bases::Lane_Group_HCM_Simple_Base Lane_Group_Base;

			//=============================================================
			// CHILD CLASS TYPE DEFINITION - required to create a child class interface for use in the phase interface
			//-------------------------------------------------------------
			template<typename ObjectType, typename ParentType=NULLTYPE>
			struct Lane_Group_Type
			{
				typedef Polaris_Component<Interfaces::Lane_Group_Interface, Lane_Group_Base,ParentType,typename ParentType::Entity_Tag,ObjectType> type;
			};	
			template<typename ObjectType>
			struct Lane_Group_Type<ObjectType,NULLTYPE>
			{
				typedef Polaris_Component<Interfaces::Lane_Group_Interface, Lane_Group_Base,NULLTYPE,NULLTYPE,ObjectType> type;
			};	
		};
	}


	//==================================================================================================================
	/// Signal Bases 
	//------------------------------------------------------------------------------------------------------------------
	namespace Bases
	{
		//==================================================================================================================
		/// HCM_Signal class.
		/// Used to store common base functionality amonst all HCM signal bases
		//------------------------------------------------------------------------------------------------------------------
		struct Signal_HCM_Base
		{
			//===========================================================
			// PHASE Related Typedefs
			//-----------------------------------------------------------
			template<typename ThisType=NULLTYPE,typename CallerType=NULLTYPE>
			struct Phase_Interface
			{
				typedef Interfaces::Phase_Interface<ThisType, CallerType> type;
				typedef void* unknown_component;
			};	


			//============================================================
			// CHILD CLASS ACCESS HANDLERS
			//-------------------------------------------------------------
			// Create Phases data member
			vector<void*> _Phases; 
			// create a tag that says the getter and setter have been defined - the existence of this tag is checked if the interface accessors are invoked
			tag_getter(Phases);

			// create a handler for the GET version of the ACCESSOR_NAME accessor created in the interface
			facet_base TargetType Phases(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				//(vector<typename TargetType::Interface_Type<TargetType,CallerType>*>&)
				return (TargetType) &_Phases; // return the local data member cast to the TargetType
			}
			// Make sure to create an error handler for the accessors by negating the requirements
			facet_base TargetType Phases(call_requirements(!(requires(ThisType,Is_Dispatched))))
			{
				assert_requirements(ThisType,Is_Dispatched,"ThisType is not dispatched");
				assert_requirements(TargetType,Is_Dispatched,"TargetType is not polaris component");
			}
			

			//==========================================================================================
			// Signal local data members
			//------------------------------------------------------------------------------------------
			member_data(int, active_phase,requires(TargetType, is_integral),requires(TargetType,is_integral));
			/// Total cycle length
			member_data(float,cycle_length,requires(TargetType,Concepts::Is_Time_Seconds),requires(TargetType,Concepts::Is_Time_Seconds));			///< C (s)
			/// Total cycle length
			member_data(float,max_cycle_length,requires(TargetType,Concepts::Is_Time_Seconds),requires(TargetType,Concepts::Is_Time_Seconds));	
			/// Total cycle length
			member_data(float,min_cycle_length,requires(TargetType,Concepts::Is_Time_Seconds),requires(TargetType,Concepts::Is_Time_Seconds));	
			/// Total cycle length
			member_data(float,degree_of_saturation,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));	///< X (s)
			/// Area type
			member_data(bool,in_CBD,requires(TargetType,is_integral),requires(TargetType,is_integral));/// 0.9 for CBD otherwise 1.0
			/// Signal ID accessor
			member_data(float,Signal_ID,requires(TargetType,is_arithmetic),false);
			/// Analyisis Period accessor
			member_data(float,analysis_period,requires(TargetType,Concepts::Is_Time_Minutes),requires(TargetType,Concepts::Is_Time_Minutes));
			member_data(float,peak_hour_factor,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));


	
		};
		//------------------------------------------------------------------------------------------------------------------
		/// HCM_Signal class.
		/// A signalized intersection base class which links to phase child classes and recalculates signal timing accoriding to HCM 2000 ch16
		//------------------------------------------------------------------------------------------------------------------
		struct Signal_HCM_Full_Base : public Signal_HCM_Base
		{
			/// Type definitions to define the context of the base class
			typedef Types::Solution_Types::HCM_Full HCM_Full;	///< The solution type applied to this base



			/// Handler for a general Initializer dispatched from an Interface
			facet_base void Initialize(TargetType number_of_phases, call_requirements(requires(ThisType,Is_Dispatched)))
			{
				for (int i=0; i<(int)number_of_phases; i++)
				{
					ThisType::Phase_Interface<Phase_Type<Execution_Object, typename ThisType::This_Type>::type,NULLTYPE>::type* itf= Allocate<Phase_Type<Execution_Object, typename ThisType::This_Type>::type>();
					((Phase_Type<Execution_Object, typename ThisType::This_Type>::type*)itf)->_parent = PTHIS(ThisType::This_Type);

					_Phases.push_back((void*)itf);
				}
			}

			
			//typedef Components::HCM_Phase_Full PhaseType;		///< The type of the child phase class	

			typedef Bases::Phase_HCM_Full_Base Phase_Base;
			template<typename ObjectType, typename ParentType=NULLTYPE>
			struct Phase_Type
			{
				typedef Polaris_Component<Interfaces::Phase_Interface,Phase_Base,ParentType,typename ParentType::Entity_Tag,ObjectType> type;
			};	
			template<typename ObjectType>
			struct Phase_Type<ObjectType,NULLTYPE>
			{
				typedef Polaris_Component<Interfaces::Phase_Interface,Phase_Base,NULLTYPE,NULLTYPE,ObjectType> type;
			};

			// GETTER OVERLOADS UNTIL CHECK IS FIXED
			tag_getter(Phases);
			tag_getter_setter(cycle_length);
			tag_getter_setter(degree_of_saturation);
			tag_getter(in_CBD);
			tag_getter(Signal_ID);
		};
		//------------------------------------------------------------------------------------------------------------------
		/// HCM_Signal class.
		/// A Simple signalized intersection base class which links to phase child classes and recalculates signal timing accoriding to HCM 2000 ch 10
		//------------------------------------------------------------------------------------------------------------------
		struct Signal_HCM_Simple_Base : public Signal_HCM_Base
		{
			/// Type definitions to define the context of the base class
			typedef Types::Solution_Types::HCM_Simple HCM_Simple;	///< The solution type applied to this base


			/// Handler for a general Initializer dispatched from an Interface
			facet_base void Initialize(TargetType number_of_phases, call_requirements(requires(ThisType,Is_Dispatched)))
			{
				// Set member variables to defaults
				this->_in_CBD = false;
				this->_cycle_length = 0.0;
				this->_degree_of_saturation = 0.9;
				this->_Signal_ID = 0;

				// Create new phases according to specified number of phases
				for (int i=0; i<(int)number_of_phases; i++)
				{
					
					ThisType::Phase_Interface<Phase_Type<Execution_Object, typename ThisType::This_Type>::type,NULLTYPE>::type* itf= Allocate<Phase_Type<Execution_Object, typename ThisType::This_Type>::type>();
					((Phase_Type<Execution_Object, typename ThisType::This_Type>::type*)itf)->_parent = PTHIS(ThisType::This_Type);

					_Phases.push_back((void*)itf);
				}

			}

			/// Child class type definition - required so that parent class can utilize child class interface
			typedef Bases::Phase_HCM_Simple_Base Phase_Base;
			template<typename ObjectType, typename ParentType=NULLTYPE>
			struct Phase_Type
			{
				typedef Polaris_Component<Interfaces::Phase_Interface,Phase_Base,ParentType,typename ParentType::Entity_Tag,ObjectType> type;
			};	
			template<typename ObjectType>
			struct Phase_Type<ObjectType,NULLTYPE>
			{
				typedef Polaris_Component<Interfaces::Phase_Interface,Phase_Base,NULLTYPE,NULLTYPE,ObjectType> type;
			};

			//// GETTER OVERLOADS UNTIL CHECK IS FIXED
			//tag_getter(Phases);
			//tag_getter_setter(cycle_length);
			//tag_getter_setter(degree_of_saturation);
			//tag_getter_setter(in_CBD);
			//tag_getter_setter(analysis_period);
			//tag_getter(Signal_ID);
		};
	}


	//==================================================================================================================
	/// COMPONENT Namespace:  Namespace for the creation of All signal components
	//------------------------------------------------------------------------------------------------------------------
	namespace Components
	{
		//------------------
		// Signals
		typedef Polaris_Component_Execution<Interfaces::Signal_Interface,Bases::Signal_HCM_Full_Base> HCM_Signal_Full;
		typedef Polaris_Component_Execution<Interfaces::Signal_Interface,Bases::Signal_HCM_Simple_Base> HCM_Signal_Simple;

		//------------------
		// Phases
		typedef Polaris_Component_Execution<Interfaces::Phase_Interface,Bases::Phase_HCM_Full_Base, HCM_Signal_Full> HCM_Phase_Full;
		typedef Polaris_Component_Execution<Interfaces::Phase_Interface,Bases::Phase_HCM_Simple_Base, HCM_Signal_Simple> HCM_Phase_Simple;

		//------------------
		// Lane Groups
		typedef Polaris_Component_Execution<Interfaces::Lane_Group_Interface,Bases::Lane_Group_HCM_Full_Base,HCM_Phase_Full> HCM_LaneGroup_Full;
		typedef Polaris_Component_Execution<Interfaces::Lane_Group_Interface,Bases::Lane_Group_HCM_Simple_Base, HCM_Phase_Simple> HCM_LaneGroup_Simple;
	}	
}
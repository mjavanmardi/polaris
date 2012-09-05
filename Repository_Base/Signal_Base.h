#pragma once

#include "Signal_Interfaces.h"


namespace Signal_Components
{
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
			facet void Initialize()
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
				this->_peak_hour_factor = 1.0;
			}


			//============================================================
			//  PARENT CLASS ACCESS HANDLERS
			//------------------------------------------------------------
			facet TargetType in_CBD(call_requirements(requires(TargetType,is_integral)))			
			{
				Interfaces::Phase_Interface* parent = (Interfaces::Phase_Interface*)(((ThisType*)this)->_parent);
				return parent->in_CBD<typename ThisType::Parent_Type,ThisType,TargetType>();
			}
			facet TargetType cycle_length(call_requirements(requires(TargetType,is_arithmetic)))			
			{
				Interfaces::Phase_Interface* parent = (Interfaces::Phase_Interface*)(((ThisType*)this)->_parent);
				return parent->cycle_length<typename ThisType::Parent_Type,ThisType,TargetType>();
			}
			facet TargetType green_time(call_requirements(requires(TargetType,is_arithmetic)))			
			{
				Interfaces::Phase_Interface* parent = (Interfaces::Phase_Interface*)(((ThisType*)this)->_parent);
				return parent->green_time<typename ThisType::Parent_Type,ThisType,TargetType>();
			}
			facet TargetType yellow_and_all_red_time(call_requirements(requires(TargetType,is_arithmetic)))			
			{
				Interfaces::Phase_Interface* parent = (Interfaces::Phase_Interface*)(((ThisType*)this)->_parent);
				return parent->yellow_and_all_red_time<typename ThisType::Parent_Type,ThisType,TargetType>();
			}
			facet TargetType analysis_period(call_requirements(requires(TargetType,is_arithmetic)))			
			{
				Interfaces::Phase_Interface* parent = (Interfaces::Phase_Interface*)(((ThisType*)this)->_parent);
				return parent->analysis_period<typename ThisType::Parent_Type,ThisType,TargetType>();
			}


			//-------------------------------------
			//Common Geometry conditions 
			member_data(int,number_of_lanes,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));		///< N

			/// Lane width data member
			float _avg_lane_width;
			tag_getter_setter(avg_lane_width);
			facet void avg_lane_width(TargetType set_value,call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,Concepts::Is_Foot_Measure)))
			{
				State_Checks::valid_lane_width<ThisType,CallerType,TargetType>(this,set_value.value);
				_avg_lane_width=(float)set_value.value;
			}
			facet void avg_lane_width(TargetType set_value,call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,Concepts::Is_Meter_Measure)))
			{
				State_Checks::valid_lane_width<ThisType,CallerType,TargetType>(set_value.value * 3.28084);
				_avg_lane_width=(float)set_value.value * 3.28084;
			}
			facet void avg_lane_width(TargetType set_value,call_requirements(!(requires(ThisType,Is_Dispatched) && (requires(TargetType, Concepts::Is_Foot_Measure) || requires(TargetType, Concepts::Is_Meter_Measure)))))
			{
				assert_requirements(ThisType,Is_Dispatched,"ThisType is not dispatched");
				assert_requirements(TargetType,Concepts::Is_Foot_Measure,"TargetType is not a foot or meter measure");
			}
			facet TargetType avg_lane_width(call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,Concepts::Is_Foot_Measure)))
			{
				return (typename TargetType::ValueType)_avg_lane_width;
			}
			facet TargetType avg_lane_width(call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,Concepts::Is_Meter_Measure)))
			{
				return (typename TargetType::ValueType)(_avg_lane_width * 0.3048);
			}
			facet TargetType avg_lane_width(call_requirements(!(requires(ThisType,Is_Dispatched) && (requires(TargetType, Concepts::Is_Foot_Measure) || requires(TargetType, Concepts::Is_Meter_Measure)))))
			{
				assert_requirements(ThisType,Is_Dispatched,"ThisType is not dispatched");
				assert_requirements(TargetType,Concepts::Is_Foot_Measure,"TargetType is not a foot or meter measure");
			}


			/// Other geometry data members
			member_data(bool,has_left_turn,requires(TargetType,is_integral),requires(TargetType,is_integral));			///<
			member_data(Data_Structures::Left_Turn_Types,left_turn_type,requires_2(TargetType,Data_Structures::Left_Turn_Types,is_convertible),requires_2(TargetType,Data_Structures::Left_Turn_Types,is_convertible));		///< Protected, Permitted, Unopposed, None
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
			facet void Initialize()
			{
				Lane_Group_HCM_Base::Initialize<ThisType,CallerType,TargetType>();

			}
			/// Type definitions to define the context of the base class
			typedef Types::Solution_Types::HCM_Full HCM_Full;		///< The solution type applied to this base

			//-------------------------------------
			// Common Geometry conditions specific to Full analysis
			member_data(float,grade,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));					///< G(%)
			member_data(float,length_left_turn,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));		///< Ls s (ft)
			member_data(float,length_right_turn,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));		///< Lr (ft)

			//-------------------------------------
			// Traffic conditions specific to the Full analysis
			member_data(float,peak_hour_left,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));			///< PHF
			member_data(float,peak_hour_right,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));		///< PHF
			member_data(float,peak_hour_thru,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));			///< PHF
			member_data(float,percent_heavy_vehicles,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic)); ///< HV for the entire lane group (%)
			member_data(float,percent_heavy_veh_left,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic)); ///< HV for left turn lanes in the group(%)
			member_data(float,percent_heavy_veh_right,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));///< HV for right turn lanes in the group(%)
			member_data(float,percent_heavy_veh_thru,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));	///< HV for through lanes in teh group(%)
			member_data(float,pedestrian_flow_rate,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));	///< v_ped (p/h)
			member_data(float,buses_per_hour,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));			///< N_b (buses/h)
			member_data(float,parking_activity,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));		///< Nm (maneuvers/h)
			member_data(float,arrival_type,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));			///< AT
			member_data(float,percent_arrive_on_green,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));///< P
			member_data(float,approach_speed,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));			///< S_a	(mi/h)

			//-------------------------------------
			// Signalization conditions specific to the Full analysis
			member_data(bool,has_pedestrian_button,requires(TargetType,is_integral),requires(TargetType,is_integral));	///< true if has pedestrian push button


			// TEMPORARY TAGS - REMOVE WHEN FIXED
			tag_getter(in_CBD);	
			tag_getter(cycle_length);			
			tag_getter(green_time);			
			tag_getter(yellow_and_all_red_time);			
			tag_getter(analysis_period);		
			tag_getter(number_of_lanes);		///< N
			tag_getter(avg_lane_width);			///< W (ft)
			tag_getter(has_left_turn);			///<
			tag_getter(left_turn_type);			///< Protected, Permitted, Unopposed, None
			tag_getter(has_right_turn);			///<
			tag_getter(has_thru_move);			///<
			tag_getter(has_parking);			///<
			tag_getter(demand_left);			///< V_l (veh/h)
			tag_getter(demand_right);			///< V_r (veh/h)
			tag_getter(demand_thru);			///< V (veh/h)
			tag_getter(demand_lane_group);		///< Vg (veh/h)
			tag_getter(demand_lane_max);		///< Vg1
			tag_getter(base_saturation_flow);	///< s_o (pc/h/ln)
			tag_getter(peak_hour_factor);		///< PHF
			tag_getter(is_actuated);			///< true if actuated, false if pre-timed
			tag_getter(min_pedestrian_green);	///< G_p (s)	

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


			// TEMPORARY TAGS - REMOVE WHEN FIXED
			tag_getter_setter(in_CBD);	
			tag_getter_setter(cycle_length);			
			tag_getter_setter(green_time);			
			tag_getter_setter(yellow_and_all_red_time);			
			tag_getter_setter(analysis_period);		
			tag_getter_setter(number_of_lanes);		///< N
			tag_getter_setter(avg_lane_width);			///< W (ft)
			tag_getter_setter(has_left_turn);			///<
			tag_getter_setter(left_turn_type);			///< Protected, Permitted, Unopposed, None
			tag_getter_setter(has_right_turn);			///<
			tag_getter_setter(has_thru_move);			///<
			tag_getter_setter(has_parking);			///<
			tag_getter_setter(demand_left);			///< V_l (veh/h)
			tag_getter_setter(demand_right);			///< V_r (veh/h)
			tag_getter_setter(demand_thru);			///< V (veh/h)
			tag_getter_setter(demand_lane_group);		///< Vg (veh/h)
			tag_getter_setter(demand_lane_max);		///< Vg1
			tag_getter_setter(base_saturation_flow);	///< s_o (pc/h/ln)
			tag_getter_setter(peak_hour_factor);		///< PHF
			tag_getter_setter(is_actuated);			///< true if actuated, false if pre-timed
			tag_getter_setter(min_pedestrian_green);	///< G_p (s)	
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
			facet void Initialize(TargetType number_of_lane_groups)
			{
				this->_green_time = 0.0;
				this->_yellow_and_all_red_time = 4.0;
			}

			//============================================================
			//  BASIC LOCAL DATA AND ACCESSOR
			//------------------------------------------------------------
			/// Phase green time
			member_data(float, green_time, requires(TargetType, Concepts::Is_Time_Seconds), requires(TargetType, Concepts::Is_Time_Seconds) && requires_2(ThisType,CallerType,Is_Same_Entity));	///< G (s)
			/// Phase lost time
			member_data(float, yellow_and_all_red_time, requires(TargetType, Concepts::Is_Time_Seconds), requires(TargetType, Concepts::Is_Time_Seconds) && requires_2(ThisType,CallerType,Is_Same_Entity));	///< Y (s)
			

			//============================================================
			// CHILD CLASS ACCESS HANDLERS
			//-------------------------------------------------------------
			// Phase lane groups container
			vector<Interfaces::Lane_Group_Interface*> _Lane_Groups; 
			// create a tag that says the getter and setter have been defined - the existence of this tag is checked if the interface accessors are invoked
			tag_getter_setter(Lane_Groups);
			// create a handler for the GET version of the lane groups accessor created in the interface
			template<typename ThisType,  typename CallerType, typename TargetType>
			vector<typename TargetType::Interface_Type*>& Lane_Groups(call_requirements(
				requires(ThisType,Is_Dispatched) &&
				requires(TargetType, Is_Polaris_Component)))
			{
				return (vector<TargetType::Interface_Type*>&) _Lane_Groups; // return the local data member cast to the TargetType
			}
			// Make sure to create an error handler for the accessors by negating the requirements
			template<typename ThisType, typename CallerType, typename TargetType>
			vector<typename TargetType::Interface_Type*>& Lane_Groups(call_requirements(!(
				requires(ThisType,Is_Dispatched) && 
				requires(TargetType, Is_Polaris_Component))))
			{
				assert_requirements(ThisType,Is_Dispatched,"ThisType is not dispatched");
				assert_requirements(TargetType, Is_Polaris_Component,"TargetType is not a polaris component");
			}
			

			//============================================================
			//  PARENT CLASS ACCESS HANDLERS
			//------------------------------------------------------------
			facet TargetType cycle_length(call_requirements(requires(TargetType,is_arithmetic)))			///< C (s)
			{
				Interfaces::Signal_Interface* parent = (Interfaces::Signal_Interface*)(((ThisType*)this)->_parent);
				return parent->cycle_length<ThisType::Parent_Type,ThisType,TargetType>();		
			}
			facet TargetType in_CBD(call_requirements(requires(TargetType,is_integral)))			///< C (s)
			{
				Interfaces::Signal_Interface* parent = (Interfaces::Signal_Interface*)(((ThisType*)this)->_parent);
				return parent->in_CBD<ThisType::Parent_Type,ThisType,TargetType>();		
			}
			facet TargetType analysis_period(call_requirements(requires(TargetType,is_arithmetic)))			///< C (s)
			{
				Interfaces::Signal_Interface* parent = (Interfaces::Signal_Interface*)(((ThisType*)this)->_parent);
				return parent->analysis_period<ThisType::Parent_Type,ThisType,TargetType>();		
			}
			facet TargetType degree_of_saturation(call_requirements(requires(TargetType,is_arithmetic)))			///< C (s)
			{
				Interfaces::Signal_Interface* parent = (Interfaces::Signal_Interface*)(((ThisType*)this)->_parent);
				return parent->degree_of_saturation<ThisType::Parent_Type,ThisType,TargetType>();		
			}

		};
		//------------------------------------------------------------------------------------------------------------------
		/// HCM Phase class.
		/// A signalized intersection phase class which links to LaneGroup child classes and calculates 
		/// phasing according to HCM 2000 CH16
		//------------------------------------------------------------------------------------------------------------------
		struct Phase_HCM_Full_Base : public Phase_HCM_Base
		{
			facet void Initialize(TargetType number_of_lane_groups)
			{
				Phase_HCM_Base::Initialize<ThisType,CallerType,TargetType>(number_of_lane_groups);

				for (int i=0; i<(int)number_of_lane_groups; i++)
				{
					Phase_HCM_Full_Base::Lane_Group_Interface* itf = Allocate<CHILD(Lane_Group_Interface,Lane_Group_Base)>();
					((CHILD(Lane_Group_Interface,Lane_Group_Base)*)itf)->_parent = PTHIS(ThisType);

					this->_Lane_Groups.push_back(itf);
				}

			}

			//============================================================
			// TYPEDEFS to define the context of the base class
			//------------------------------------------------------------
			typedef Types::Solution_Types::HCM_Full HCM_Full;		///< The solution type applied to this base
			//typedef Components::HCM_LaneGroup_Full LaneGroupType;	///< The type of the child phase class	

			typedef Interfaces::Lane_Group_Interface Lane_Group_Interface;
			typedef Bases::Lane_Group_HCM_Full_Base Lane_Group_Base;

			//TEMP ACCESSOR TAGS
			tag_getter_setter(cycle_length);
			tag_getter_setter(in_CBD);
			tag_getter_setter(analysis_period);
			tag_getter_setter(degree_of_saturation);
			tag_getter_setter(green_time);
			tag_getter_setter(yellow_and_all_red_time);
		};
		//------------------------------------------------------------------------------------------------------------------
		/// HCM Phase class.
		/// A signalized intersection phase class which links to LaneGroup child classes and calculates 
		/// phasing according to HCM 2000 CH10 - simplified method
		//------------------------------------------------------------------------------------------------------------------
		struct Phase_HCM_Simple_Base : public Phase_HCM_Base
		{
			facet void Initialize(TargetType number_of_lane_groups)
			{
				Phase_HCM_Base::Initialize<ThisType,CallerType,TargetType>(number_of_lane_groups);

				for (int i=0; i<(int)number_of_lane_groups; i++)
				{
					Phase_HCM_Simple_Base::Lane_Group_Interface* itf = Allocate<CHILD(Lane_Group_Interface,Lane_Group_Base)>();
					((CHILD(Lane_Group_Interface,Lane_Group_Base)*)itf)->_parent = PTHIS(typename ThisType::This_Type);

					this->_Lane_Groups.push_back(itf);
				}

			}

			/// Type definitions to define the context of the base class
			typedef Types::Solution_Types::HCM_Simple HCM_Simple;	///< The solution type applied to this base
			//typedef Components::HCM_LaneGroup_Simple LaneGroupType;	///< The type of the child phase class	
			
			typedef Interfaces::Lane_Group_Interface Lane_Group_Interface;
			typedef Bases::Lane_Group_HCM_Simple_Base Lane_Group_Base;

			//TEMP ACCESSOR TAGS
			tag_getter_setter(cycle_length);
			tag_getter_setter(in_CBD);
			tag_getter_setter(analysis_period);
			tag_getter_setter(degree_of_saturation);
			tag_getter_setter(green_time);
			tag_getter_setter(yellow_and_all_red_time);
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
			//============================================================
			// CHILD CLASS ACCESS HANDLERS
			//-------------------------------------------------------------
			// Create Phases data member
			vector<Interfaces::Phase_Interface*> _Phases; 
			// create a tag that says the getter and setter have been defined - the existence of this tag is checked if the interface accessors are invoked
			tag_getter(Phases);
			// create a handler for the GET version of the ACCESSOR_NAME accessor created in the interface
			facet vector<typename TargetType::Interface_Type*>& Phases(call_requirements(
				requires(ThisType,Is_Dispatched) && requires(TargetType, Is_Polaris_Component)))
			{
				return (vector<typename TargetType::Interface_Type*>&) _Phases; // return the local data member cast to the TargetType
			}
			// Make sure to create an error handler for the accessors by negating the requirements
			facet vector<typename TargetType::Interface_Type*>& Phases(call_requirements(!(
				requires(ThisType,Is_Dispatched)&& requires(TargetType, Is_Polaris_Component))))
			{
				assert_requirements(ThisType,Is_Dispatched,"ThisType is not dispatched");
				assert_requirements(TargetType,Is_Dispatched,"TargetType is not polaris component");
			}
			

			//==========================================================================================
			// Signal local data members
			//------------------------------------------------------------------------------------------
			/// Total cycle length
			member_data(float,cycle_length,requires(TargetType,Concepts::Is_Time_Seconds),requires(TargetType,Concepts::Is_Time_Seconds));			///< C (s)
			/// Total cycle length
			member_data(float,degree_of_saturation,requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));	///< X (s)
			/// Area type
			member_data(bool,in_CBD,requires(TargetType,is_integral),requires(TargetType,is_integral) && requires_2(ThisType,CallerType,Is_Same_Component));/// 0.9 for CBD otherwise 1.0
			/// Signal ID accessor
			member_data(float,Signal_ID,requires(TargetType,is_arithmetic),false);
			/// Analyisis Period accessor
			member_data(float,analysis_period,requires(TargetType,Concepts::Is_Time_Minutes),requires(TargetType,Concepts::Is_Time_Minutes) && requires_2(ThisType,CallerType,Is_Same_Component));
		};
		//------------------------------------------------------------------------------------------------------------------
		/// HCM_Signal class.
		/// A signalized intersection base class which links to phase child classes and recalculates signal timing accoriding to HCM 2000 ch16
		//------------------------------------------------------------------------------------------------------------------
		struct Signal_HCM_Full_Base : public Signal_HCM_Base
		{
			/// Handler for a general Initializer dispatched from an Interface
			facet void Initialize(TargetType number_of_phases, call_requirements(requires(ThisType,Is_Dispatched)))
			{
				for (int i=0; i<(int)number_of_phases; i++)
				{
					Signal_HCM_Full_Base::Phase_Interface* itf= Allocate<CHILD(Phase_Interface,Phase_Base)>();
					this->_Phases.push_back(itf);
				}
			}

			/// Type definitions to define the context of the base class
			typedef Types::Solution_Types::HCM_Full HCM_Full;	///< The solution type applied to this base
			//typedef Components::HCM_Phase_Full PhaseType;		///< The type of the child phase class	
			typedef Interfaces::Phase_Interface Phase_Interface;
			typedef Bases::Phase_HCM_Full_Base Phase_Base;

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
			/// Handler for a general Initializer dispatched from an Interface
			facet void Initialize(TargetType number_of_phases, call_requirements(requires(ThisType,Is_Dispatched)))
			{
				// Set member variables to defaults
				this->_in_CBD = false;
				this->_cycle_length = 0.0;
				this->_degree_of_saturation = 0.9;
				this->_Signal_ID = 0;

				// Create new phases according to specified number of phases
				for (int i=0; i<(int)number_of_phases; i++)
				{
					Signal_HCM_Simple_Base::Phase_Interface* itf= Allocate<CHILD(Phase_Interface,Phase_Base)>();
					((CHILD(Phase_Interface,Phase_Base)*)itf)->_parent = PTHIS(ThisType);

					this->_Phases.push_back(itf);
				}
			}

			/// Type definitions to define the context of the base class
			typedef Types::Solution_Types::HCM_Simple HCM_Simple;	///< The solution type applied to this base
			//typedef Components::HCM_Phase_Simple PhaseType;		///< The type of the child phase class	
			typedef Interfaces::Phase_Interface Phase_Interface;
			typedef Bases::Phase_HCM_Simple_Base Phase_Base;

			// GETTER OVERLOADS UNTIL CHECK IS FIXED
			tag_getter(Phases);
			tag_getter_setter(cycle_length);
			tag_getter_setter(degree_of_saturation);
			tag_getter_setter(in_CBD);
			tag_getter_setter(analysis_period);
			tag_getter(Signal_ID);
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
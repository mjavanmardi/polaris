#pragma once

#include "Repository_Base_Includes.h"

#define NAMESPACE(namespace_name, main_interface_name) \
	struct namespace_name_typedefs {typedef main_interface_name base_interface;}; \
	struct namespace_name : namespace_name_typedefs

#define THIS ((base_interface*)this)
#define SUB_THIS(other_interface) ((other_interface*)this)


namespace Signal_Components
{
	//==================================================================================================================
	/// Namespace of useful types.  Add any types that convey semantic infomration regarding inputs, outpute, requirements, etc. 
	/// for components in this namespace.  Use well defined typenames that convey the purpose of the Tag
	//------------------------------------------------------------------------------------------------------------------
	namespace Types
	{
		namespace Controller_Types
		{
			typedef true_type Pre_Timed;
			typedef true_type Actuated;
		}
		namespace Approach_Types
		{
			typedef true_type Thru;
			typedef true_type Left;
			typedef true_type Right;
		}
		namespace Lane_Group_Types
		{
			typedef true_type Has_Parking;
			typedef true_type Has_Pedestrian_Button;
			typedef true_type In_CBD;
		}
		namespace General_Types
		{
			typedef true_type Percentage;
		}
	}


	//==================================================================================================================
	/// Namespace which holds all the concepts which enforce requirements and restrictions on template members, including components 
	/// List all concepts that may be used by this namespace here
	//------------------------------------------------------------------------------------------------------------------
	namespace Concepts
	{
		/// EXAMPLE Concept: replace CONCEPT_NAME and CHECK below, and add/remove requirements as necessary
		concept CONCEPT_NAME
		{
			begin_requirements_list;
			requires_typename_state(none, CHECK, true_type,"ERROR_MESSAGE");
			end_requirements_list(CHECK);
		};

		concept Is_Percentage
		{
			begin_requirements_list;
			requires_typename_state(none, Precentage, true_type,"Type does not model a percentage concept.");
			end_requirements_list(Precentage);
		};
		concept Is_In_CBD
		{
			begin_requirements_list;
			requires_typename_state(none, In_CBD, true_type,"Type does not contain a a valid in CBD tag.");
			end_requirements_list(In_CBD);
		};
		concept Has_Left_Turn
		{
			begin_requirements_list;
			requires_typename_state(none, Left, true_type,"Type does not contain a a valid left turn lane tag.");
			end_requirements_list(Left);
		};
		concept Has_Right_Turn
		{
			begin_requirements_list;
			requires_typename_state(none, Right, true_type,"Type does not contain a a valid right turn lane tag.");
			end_requirements_list(Left);
		};
		concept Has_Thru_Movement
		{
			begin_requirements_list;
			requires_typename_state(none, Thru, true_type,"Type does not contain a a valid thru lane tag.");
			end_requirements_list(Left);
		};

		define_state_check(valid_lane_width, float width)
		{
			begin_state_checks;
			requires_condition(width >= 16.0,"Warning, lane widths less than 8.0ft are not allowed",true,cout);
			end_state_checks;
		};
		define_state_check(valid_grade, float grade)
		{
			begin_state_checks;
			requires_condition(grade >= -6.0 && grade <= 10.0 ,"Warning, grade must be between -6.0% and 10.0%",true,cout);
			end_state_checks;
		};
		define_state_check(valid_parking_manuevers, float Nm)
		{
			begin_state_checks;
			requires_condition(Nm >= 0.0 && Nm <= 180.0 ,"Warning, parking maneuvers must be less than 180/hour",true,cout);
			end_state_checks;
		};
		define_state_check(valid_num_bus_stops, float Nb)
		{
			begin_state_checks;
			requires_condition(Nb >= 0.0 && Nb<= 250.0 ,"Warning, number of buses stopping must be less than 250/hour",true,cout);
			end_state_checks;
		};
	}


	//==================================================================================================================
	/// Namespace which includes the interface(or interfaces if necessary) which links to the various Bases regarding the transportation object
	//------------------------------------------------------------------------------------------------------------------
	namespace Interfaces
	{
		//------------------------------------------------------------------------------------------------------------------
		/// Inteface to the basic POLARIS Signal component
		//------------------------------------------------------------------------------------------------------------------
		struct Signal_Interface
		{
			//=======================================================
			// Initialization and Calculations
			//-------------------------------------------------------
			/// Dispatch the Initialize function call to the component base
			facet void Initialize()
			{
				return PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TargetType>();
			}

			/// Add a new lane group to the signal controller
			facet void Add_Lane_Group(Target_Type::Interface_Type* new_lane_group)
			{
				PTHIS(ThisType)->Add_Lane_Group(new_lane_group);
			}

			/// HCM Cycle length estimation
			facet void Estimate_Cycle_Length()
			{
				typedef ThisType T;

				vector<Interfaces::Phase_Interface*>& phases = this->Phases<T,TargetType>();
				vector<Interfaces::Phase_Interface*>::iterator itr = phases.begin();

				// Sum total lost time for all phases
				float lost_time = 0;
				for (itr; itr != phases.end(); itr++)
				{
					lost_time += ((Interfaces::Phase_Interface*)(*itr))->yellow_and_all_red_time<TargetType,T,float>();
				}

			}

			/// HCM Retiming calculations
			facet void Update_Timing()
			{
				vector<Interfaces::Lane_Group_Interface*>& lane_group = this->Lane_Groups<ThisType,TargetType>();
				vector<Interfaces::Lane_Group_Interface*>::iterator itr = lane_group.begin();
				for (itr; itr != lane_group.end(); itr++)
				{
					((TargetType::Interface_Type*)(*itr))->Calculate_Saturation_Flow_Rate<TargetType,ThisType,float>();
				}
			}
			 

			//=======================================================
			// Accessor facets
			//-------------------------------------------------------
			/// Total cycle length
			facet_accessor(cycle_length);			///< C (s)
			/// Total cycle length
			facet_accessor(degree_of_saturation);	///< X (s)
			/// Area type
			facet_accessor(area_type);
			/// Signal ID accessor
			facet_accessor(Signal_ID);
			/// Local facet to return the list of lane groups associated with signal
			facet_local vector<TargetType::Interface_Type*>& Phases(call_requirements(requires(ThisType, Is_Dispatchable) && requires(TargetType, Is_Polaris_Component)))
			{
				return PTHIS(ThisType)->Phases<Dispatch<ThisType>,TargetType>();
			}
			/// Local facet to return the list of lane groups associated with signal
			facet_local vector<TargetType::Interface_Type*>& Lane_Groups(call_requirements(requires(ThisType, Is_Dispatchable) && requires(TargetType, Is_Polaris_Component)))
			{
				return PTHIS(ThisType)->Lane_Groups<Dispatch<ThisType>,TargetType>();
			}

		};

		//------------------------------------------------------------------------------------------------------------------
		/// Inteface to the basic POLARIS Signal Phase Component
		//------------------------------------------------------------------------------------------------------------------
		struct Phase_Interface
		{
			//============================================================
			//  Member functions
			//------------------------------------------------------------
			/// Get the maximum saturation ratio from amonst the individual lane_group in the phase
			facet TargetType::ReturnType Find_Critical_VS_Ratio(call_requirements(requires(TargetType::LangeGroupType, Is_Polaris_Component) && requires_2(TargetType::LaneGroupType::Interface_Type&, TargetType::Lane_Group_Interface&, is_convertible)))
			{
				vector<Interfaces::Lane_Group_Interface*>& lane_group = this->Lane_Groups<ThisType,LaneGroupType>();
				vector<Interfaces::Lane_Group_Interface*>::iterator itr = lane_group.begin();

				for (itr; itr != lane_group.end(); itr++)
				{
					float vs_i = (*itr)->Calculate_Saturation_Flow_Rate<LaneGroupType,ThisType,float>();
				}
			}
			facet TargetType::ReturnType Find_Critical_VS_Ratio(call_requirements(!(requires(LangeGroupType, Is_Polaris_Component) && requires_2(LaneGroupType::Interface_Type&, Lane_Group_Interface&, is_convertible))))
			{
				assert_requirements(LaneGroupType,Is_Polaris_Component,"Your lane group is not a valid polaris component");
				assert_requirements_2(LaneGroupType::Interface_Type&,Lange_Group_Interface,is_convertible,"Error - your lane group type is not convertible to the base lane group interface.");
			}

			
			//============================================================
			//  BASIC ACCESSORS
			//------------------------------------------------------------
			/// Phase green time
			facet_accessor(green_time);					///< G (s)
			/// Phase lost time
			facet_accessor(yellow_and_all_red_time);	///< Y (s)
			/// Local facet to return the list of lane groups associated with phase
			facet_local vector<TargetType::Interface_Type*>& Lane_Groups(call_requirements(requires(ThisType, Is_Dispatchable)))
			{
				return PTHIS(ThisType)->Lane_Groups<Dispatch<ThisType>,TargetType>();
			}

			//============================================================
			//  PARENT CLASS ACCESSORS
			//------------------------------------------------------------
			facet_accessor(cycle_length);			///< C (s)
		};

		//------------------------------------------------------------------------------------------------------------------
		/// Inteface to the basic POLARIS Lane Group component as defined in HCM
		//------------------------------------------------------------------------------------------------------------------
		struct Lane_Group_Interface
		{
			//=======================================================
			// General Lane Group Calculation Facets
			//-------------------------------------------------------
			facet float Calculate_VC_ratio()
			{
				return this->cycle_length<ThisType,float>() * this->demand_lane_group<ThisType,float>() / this->Calculate_Saturation_Flow_Rate<ThisType,float>() / this->green_time<ThisType,float>();
			}
			facet float Calculate_VS_ratio()
			{
				return this->demand_lane_group<ThisType,float>() / this->Calculate_Saturation_Flow_Rate<ThisType,float>();
			}


			//=======================================================
			// Lane_Group Interface Members Specific to HCM Analysis
			//-------------------------------------------------------
			NAMESPACE(HCM_Lane_Group, Lane_Group_Interface)
			{
				// Get the saturation flow rate for the lane group
				facet float HCM_Calculate_Saturation_Flow_Rate()
				{
					//s = saturation flow rate for subject lane group, expressed as a total for all lanes in lane group (veh/h)
					// s o = base saturation flow rate per lane (pc/h/ln)
					// N = number of lanes in lane group
					// fw = adjustment factor for lane width
					// fHV = adjustment factor for heavy vehicles in traffic stream
					// f g = adjustment factor for approach grade
					// f p = adjustment factor for existence of a parking lane and parking activity adjacent to lane group
					// f bb = adjustment factor for blocking effect of local buses that stop within intersection area
					// f a = adjustment factor for area type
					// f LU = adjustment factor for lane utilization
					// f LT = adjustment factor for left turns in lane group
					// fRT = adjustment factor for right turns in lane group
					// fLpb = pedestrian adjustment factor for left-turn movements
					// fRpb = pedestrian-bicycle adjustment factor for right-turn movements.
					s = this->base_saturation_flow<ThisType, float>() *
						this->number_of_lanes<ThisType,float>() *
						this->HCM_adjustment_fw<ThisType, float>()*
						this->HCM_adjustment_fHV<ThisType, float>() *
						this->HCM_adjustment_fg<ThisType, float>() *
						this->HCM_adjustment_fp<ThisType, float>() *
						this->HCM_adjustment_fbb<ThisType, float>() *
						this->HCM_adjustment_fa<ThisType, float>() *
						this->HCM_adjustment_fLU<ThisType, float>() *
						this->HCM_adjustment_fLT<ThisType, float>() *
						this->HCM_adjustment_fRT<ThisType, float>() *
						this->HCM_adjustment_fLpb<ThisType, float>() *
						this->HCM_adjustment_fRpb<ThisType, float>();
				}

				// adjustment factor for lane width
				facet float HCM_adjustment_fw()
				{
					
					float width = THIS->avg_lane_width<ThisType,float>();
					Concepts::valid_lane_width<ThisType,CallerType,TargetType>(width);
					return (1.0 + (width - 12.0)/30.0);
				}
				// adjustment factor for heavy vehicles in traffic stream
				facet float HCM_adjustment_fHV()
				{
					return 100.0/(100.0 + (THIS->percent_heavy_vehicles<ThisType,Percentage_Struct>()).value;
				}
				// adjustment factor for approach grade
				facet float HCM_adjustment_fg()
				{
					float grade = (THIS->grade<ThisType,Percentage_Struct>()).value;
					Concepts::valid_grade<ThisType,CallerType,TargetType>(grade);
					return 1.0 - grade / 200.0;
				}
				// adjustment factor for existence of a parking lane and parking activity adjacent to lane group
				facet float HCM_adjustment_fp()
				{
					float parking = THIS->parking_activity<ThisType,float>();
					Concepts::valid_parking_manuevers<ThisType,CallerType,float>(parking);
					float lanes = THIS->number_of_lanes<ThisType,float>();
					return (lanes - 0.1 - 18.0 * parking /3600.0)/lanes;
				}
				// adjustment factor for blocking effect of local buses that stop within intersection area
				facet float HCM_adjustment_fbb()
				{
					float buses = THIS->buses_per_hour<ThisType,float>();
					Concepts::valid_num_bus_stops<ThisType,CallerType,float>(buses);
					float lanes = THIS->number_of_lanes<ThisType,float>();
					return (lanes - 14.4 * buses /3600.0)/lanes;
				}
				// adjustment factor for area type
				facet float HCM_adjustment_fa(call_requires(ThisType,Concepts::Is_In_CBD))
				{
					return 0.9;
				}
				facet float HCM_adjustment_fa()
				{
					return 1.0;
				}
				//  adjustment factor for lane utilization
				facet float HCM_adjustment_fLU()
				{
					return THIS->demand_lane_group<ThisType,float>() / (THIS->demand_lane_max<ThisType,float>() * THIS->number_of_lanes<ThisType,float>());
				}
				// adjustment factor for left turns in lane group
				facet float HCM_adjustment_fLT()
				{
					bool has_left, has_right, has_thru;
					has_left = ((Lane_Group_Interface*)this)->has_left_turn<ThisType,bool>();
					has_right = ((Lane_Group_Interface*)this)->has_right_turn<ThisType,bool>();
					has_thru = ((Lane_Group_Interface*)this)->has_thru_move<ThisType,bool>();

					if (has_left && !has_thru)
					{
						return 0.95;
					}
					else if (has_left && has_thru)
					{
						return 1.0 / (1.0 + 0.05 * this->demand_left<ThisType, float>() / this->demand_lane_group<ThisType,float>());
					}
					else
					{
						return 1.0;
					}
				}
				// adjustment factor for right turns in lane group
				facet float HCM_adjustment_fRT()
				{
					bool has_left, has_right, has_thru;
					has_left = ((T)this)->has_left_turn<ThisType,bool>();
					has_right = ((Lane_Group_Interface*)this)->has_right_turn<ThisType,bool>();
					has_thru = ((Lane_Group_Interface*)this)->has_thru_move<ThisType,bool>();

					if (has_right && !has_thru)
					{
						return 0.85;
					}
					else if (has_right && has_thru)
					{
						int nlane = ((Lane_Group_Interface*)this)->number_of_lanes<ThisType,int>();
						if (nlane > 1)
						{
							return 1.0 - 0.15 * this->demand_right<ThisType, float>() / this->demand_lane_group<ThisType,float>();
						}
						else
						{
							return 1.0 - 0.135 * this->demand_right<ThisType, float>() / this->demand_lane_group<ThisType,float>();
						}
					}
					else
					{
						return 1.0;
					}

				}
				// pedestrian adjustment factor for left-turn movements
				facet float HCM_adjustment_fLpb()
				{
					return 1.0;
				}
				// pedestrian-bicycle adjustment factor for right-turn movements
				facet float HCM_adjustment_fRpb()
				{
					return 1.0;
				}
			};


			//=======================================================
			// DATA Access Facets
			//-------------------------------------
			//Geometric conditions 
			facet_accessor(area_type);
			facet_accessor(number_of_lanes);		///< N
			facet_accessor(avg_lane_width);			///< W (ft)
			facet_accessor(grade);					///< G(%)
			facet_accessor(has_left_turn);			///<
			facet_accessor(has_right_turn);			///<
			facet_accessor(has_thru_move);			///<
			facet_accessor(in_CBD);					///<
			facet_accessor(length_left_turn);		///< Ls s (ft)
			facet_accessor(length_right_turn);		///< Lr (ft)
			facet_accessor(has_parking);			///<

			//-------------------------------------
			//Traffic conditions 
			facet_accessor(demand_left);			///< V_l (veh/h)
			facet_accessor(demand_right);			///< V_r (veh/h)
			facet_accessor(demand_thru);			///< V (veh/h)
			facet_accessor(demand_lane_group);		///< Vg (veh/h)
			facet_accessor(demand_lane_max);		///< Vg1
			facet_accessor(base_saturation_flow);	///< s_o (pc/h/ln)
			facet_accessor(peak_hour_factor);		///< PHF
			facet_accessor(peak_hour_left);			///< PHF
			facet_accessor(peak_hour_right);		///< PHF
			facet_accessor(peak_hour_thru);			///< PHF
			facet_accessor(percent_heavy_vehicles); ///< HV for the entire lane group (%)
			facet_accessor(percent_heavy_veh_left); ///< HV for left turn lanes in the group(%)
			facet_accessor(percent_heavy_veh_right);///< HV for right turn lanes in the group(%)
			facet_accessor(percent_heavy_veh_thru);	///< HV for through lanes in teh group(%)
			facet_accessor(pedestrian_flow_rate);	///< v_ped (p/h)
			facet_accessor(buses_per_hour);			///< N_b (buses/h)
			facet_accessor(parking_activity);		///< Nm (maneuvers/h)
			facet_accessor(arrival_type);			///< AT
			facet_accessor(percent_arrive_on_green);///< P
			facet_accessor(approach_speed);			///< S_a	(mi/h)

			//-------------------------------------
			//Signalization conditions 
			facet_accessor(cycle_length);			///< C (s)
			facet_accessor(green_time);				///< G (s)
			facet_accessor(yellow_and_all_red_time);///< Y (s)
			facet_accessor(is_actuated);			///< true if actuated, false if pre-timed
			facet_accessor(has_pedestrian_button);	///< true if has pedestrian push button
			facet_accessor(min_pedestrian_green);	///< G_p (s)
			facet_accessor(phase_plan);				///< 
			facet_accessor(analysis_period);		///< T (h)
		};

	}


	//==================================================================================================================
	/// Namespace of useful related data structures for the components
	//------------------------------------------------------------------------------------------------------------------
	namespace Data_Structures
	{
		//------------------------------------------------------------------------------------------------------------------
		// Example of a useful structure
		//------------------------------------------------------------------------------------------------------------------
		struct Percentage_Struct
		{
			// to tag a datastructure give it knowledge of a type from the API type list, by redefining the API type as a new type with the scope information removed
			typedef Types::General_Types::Percentage Percentage;
			typedef float VALUE_TYPE;
			// then define the data member(s) that the struct contains
			VALUE_TYPE value;
		};
	}

	namespace Target_Types
	{

	}
}
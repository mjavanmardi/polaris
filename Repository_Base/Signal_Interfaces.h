#pragma once

#include "Repository_Base_Includes.h"

//================================================================================================================================================================
//================================= TEMPORARY CORE METASTRUCTURE UPDATES =========================================================================================
//================================================================================================================================================================
#pragma region TEMP_CORE
#define NAMESPACE(namespace_name, main_interface_name) \
	struct namespace_name_typedefs {typedef main_interface_name base_interface;}; \
	struct namespace_name : namespace_name_typedefs

#undef THIS
#define THIS ((base_interface*)this)
#define SUB_THIS(other_interface) ((other_interface*)this)

#define CHILD(child_interface,child_base) Polaris_Component<typename ThisType::child_interface,typename ThisType::child_base,ThisType::This_Type>

template<typename Return_Type, typename Param_Type=NULLTYPE, typename Param2_Type=NULLTYPE, typename Param3_Type=NULLTYPE, typename Param4_Type=NULLTYPE>
struct Target_Type
{
	typedef Return_Type ReturnType;
	typedef Param_Type ParamType;
	typedef Param2_Type Param2Type;
	typedef Param3_Type Param3Type;
	typedef Param4_Type Param4Type;
};

#define Basic_Data_Struct(DATA_STRUCT_NAME, DATA_VALUE_TYPE, TYPE_TRAIT_TAGS,...) \
struct DATA_STRUCT_NAME \
{													\
	typedef true_type TYPE_TRAIT_TAGS, __VA_ARGS__; \
	typedef DATA_VALUE_TYPE ValueType; \
	DATA_STRUCT_NAME(DATA_VALUE_TYPE val): value(val){} \
	DATA_STRUCT_NAME(): value(0){} \
	DATA_VALUE_TYPE value; \
	operator DATA_VALUE_TYPE(){return value;}; \
	DATA_STRUCT_NAME& operator=(const DATA_VALUE_TYPE& obj){value = obj; return *this;}  \
};

//================================================================================================================================================================
//================================================================================================================================================================
//================================================================================================================================================================
#pragma endregion


namespace Signal_Components
{
	//==================================================================================================================
	/// Namespace of useful types.  Add any types that convey semantic infomration regarding inputs, outpute, requirements, etc. 
	/// for components in this namespace.  Use well defined typenames that convey the purpose of the Tag
	//------------------------------------------------------------------------------------------------------------------
	namespace Types
	{
		namespace Solution_Types
		{
			typedef true_type HCM_Simple;
			typedef true_type HCM_Full;
		}
		namespace Flow_Types
		{
			typedef true_type Veh_per_hour;
			typedef true_type Veh_per_hour_per_lane;
			typedef true_type Veh_per_15_min;
			typedef true_type Veh_per_15_min_per_lane;
		}
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
		namespace Units_Types
		{
			typedef true_type Feet;
			typedef true_type Meters;
		}
		//namespace Left_Turn_Types
		//{
		//	typedef true_type Left_Turn_Permitted;
		//	typedef true_type Left_Turn_Protected;
		//	typedef true_type Left_Turn_Unopposed;
		//	typedef true_type Left_Turn_None;
		//}
	}


	//==================================================================================================================
	/// Namespace which holds all the concepts which enforce requirements and restrictions on template members, including components 
	/// List all concepts that may be used by this namespace here
	//------------------------------------------------------------------------------------------------------------------
	namespace Concepts
	{
		/// EXAMPLE Concept: replace CONCEPT_NAME and CHECK below, and add/remove requirements as necessary
		#pragma region Signal Concepts
		concept CONCEPT_NAME
		{
			begin_requirements_list;
			requires_typename_state(none, CHECK, true_type,"ERROR_MESSAGE");
			end_requirements_list(CHECK);
		};
		concept Has_Child_Phase
		{
			begin_requirements_list;
			requires_typename_defined(none, Phase_Base, "ThisType does not have a Phase child type: no Phase_Base type is defined.");
			end_requirements_list(Phase_Base);
		};
		#pragma endregion

		#pragma region Phase Concepts
		concept Has_Child_Lane_Group
		{
			begin_requirements_list;
			requires_typename_defined(none, Lane_Group_Base, "ThisType does not have a Lane_Group child type: no Lane_Group_Base type is defined.");
			end_requirements_list(Lane_Group_Base);
		};
		#pragma endregion

		#pragma region Units Concepts
		concept Is_Percentage
		{
			begin_requirements_list;
			requires_typename_state(none, Percent, true_type,"Type does not model a percentage concept.");
			end_requirements_list(Percent);
		};
		concept Is_Foot_Measure
		{
			begin_requirements_list;
			requires_typename_defined(none, Feet, "Type does not model a foot measurement concept.");
			requires_typename_defined(Feet,ValueType,"Your measure type does not have an arithmetic ValueType defined.");
			end_requirements_list(ValueType);
		};
		concept Is_Meter_Measure
		{
			begin_requirements_list;
			requires_typename_defined(none, Meters, "Type does not model a meter measure concept.");
			requires_typename_defined(Meters,ValueType,"Your measure type does not have an arithmetic ValueType defined.");
			end_requirements_list(ValueType);
		};
		concept Is_Time_Seconds
		{
			begin_requirements_list;
			requires_typename_defined(none, Time, "Type does not model a Time concept.");
			requires_typename_defined(Time, Seconds, "Type does not model a Time in Seconds concept.");
			requires_typename_defined(Seconds,ValueType,"Your measure type does not have an arithmetic ValueType defined.");
			end_requirements_list(ValueType);
		};
		concept Is_Time_Minutes
		{
			begin_requirements_list;
			requires_typename_defined(none, Time, "Type does not model a Time concept.");
			requires_typename_defined(Time, Minutes, "Type does not model a Time in Seconds concept.");
			requires_typename_defined(Minutes,ValueType,"Your measure type does not have an arithmetic ValueType defined.");
			end_requirements_list(ValueType);
		};
		concept Is_Flow_Per_Hour
		{
			begin_requirements_list;
			requires_typename_defined(none, Unit_Per_Hour, "Type does not model a Flow per hour concept.");
			requires_typename_defined(Unit_Per_Hour, Flow, "Type does not model a Flow concept.");
			requires_typename_defined(Flow,ValueType,"Your measure type does not have an arithmetic ValueType defined.");
			end_requirements_list(ValueType);
		};



		#pragma endregion

		#pragma region Solution Type Concepts
		concept Is_HCM_Simple_Solution
		{
			begin_requirements_list;
			requires_typename_state(none, HCM_Simple, true_type,"Type does not model an HCM simple solution type.");
			end_requirements_list(HCM_Simple);
		};
		concept Is_HCM_Full_Solution
		{
			begin_requirements_list;
			requires_typename_state(none, HCM_Full, true_type,"Type does not model an HCM full solution type.");
			end_requirements_list(HCM_Full);
		};
		#pragma endregion
	}
	

	//==================================================================================================================
	/// Namespace which holds all the run-time state checks which enforce requirements on variable states
	/// List all state-checks that may be used by this namespace here
	//------------------------------------------------------------------------------------------------------------------
	namespace State_Checks
	{
		#pragma region Basic Numeric Checks
		define_state_check(is_0_to_100_percentage, float value)
		{
			begin_state_checks;
			requires_condition(value >= 0.0 && value < 100.0,"Warning, must be a percentage between 0 and 100",true,cout);
			end_state_checks;
		}
		define_state_check(ispositive, float value)
		{
			begin_state_checks;
			requires_condition(value >= 0.0,"Warning, must be a positive number",true,cout);
			end_state_checks;
		}

		#pragma endregion
		
		#pragma region Lane Group State Checks
		define_state_check(valid_lane_width, float width)
		{
			begin_state_checks;
			requires_condition((width >= 8.0 && width < 16.0),"Warning, lane widths less than 8.0ft or over 16ft are not allowed",true,cout);
			end_state_checks;
		};
		define_state_check(valid_grade, float grade)
		{
			begin_state_checks;
			requires_condition((grade >= -6.0 && grade <= 10.0) ,"Warning, grade must be between -6.0% and 10.0%",true,cout);
			end_state_checks;
		};
		define_state_check(valid_parking_manuevers, float Nm)
		{
			begin_state_checks;
			requires_condition((Nm >= 0.0 && Nm <= 180.0) ,"Warning, parking maneuvers must be less than 180/hour",true,cout);
			end_state_checks;
		};
		define_state_check(valid_num_bus_stops, float Nb)
		{
			begin_state_checks;
			requires_condition((Nb >= 0.0 && Nb<= 250.0) ,"Warning, number of buses stopping must be less than 250/hour",true,cout);
			end_state_checks;
		};
		#pragma endregion

	}


	//==================================================================================================================
	/// Namespace which includes the interface(or interfaces if necessary) which links to the various Bases regarding the transportation object
	//------------------------------------------------------------------------------------------------------------------
	namespace Interfaces
	{
		//------------------------------------------------------------------------------------------------------------------
		/// Inteface to the basic POLARIS Lane Group component as defined in HCM
		//------------------------------------------------------------------------------------------------------------------
		template<typename ThisType=NULLTYPE, typename CallerType=NULLTYPE>
		struct Lane_Group_Interface
		{
			facet void Initialize()
			{
				return PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TargetType>();
			}

			//=======================================================
			// General Lane Group Calculation Facets
			//-------------------------------------------------------
			// Get the volumeto capacity ratio for lane group
			facet TargetType Calculate_VC_ratio(call_requires(TargetType,is_arithmetic))
			{
				return this->cycle_length<TargetType>() * this->demand_lane_group<TargetType>() / this->_Calculate_Saturation_Flow_Rate<TargetType>() / this->green_time<TargetType>();
			}
			facet TargetType Calculate_VC_ratio(call_requires(TargetType,!is_arithmetic))
			{
				assert_requirements(TargetType,is_arithmetic,"Error - your TargetType is not arithmetic.");
			}
			// Get the flow ratio for the lane group - used in Full analysis
			facet TargetType Calculate_VS_ratio(call_requires(TargetType,is_arithmetic))
			{
				return this->demand_lane_group<Data_Structures::Flow_Per_Hour>() / this->Calculate_Saturation_Flow_Rate<TargetType>();
			}
			facet TargetType Calculate_VS_ratio(call_requires(TargetType,!is_arithmetic))
			{
				assert_requirements(TargetType,is_arithmetic,"Error - your TargetType is not arithmetic.");
			}
			// Get the critical volume for the lane group - used in simple analysis
			facet TargetType Calculate_Lane_Volume(call_requires(TargetType,Concepts::Is_Flow_Per_Hour))
			{
				TargetType vol_tot, vol_right, vol_left, vol_thru, vol_right_shared, vol_left_unopposed, vol_opposing, left_proportion;
				vol_right = this->demand_right<TargetType>();
				vol_left = this->demand_left<TargetType>();
				vol_thru = this->demand_thru<TargetType>();
				vol_tot = vol_right + vol_left + vol_thru;
				vol_right_shared = 0;
				vol_left_unopposed = 0;

				int nl, nr, nt, N;
				nl = this->number_of_left_lanes<int>();
				nr = this->number_of_right_lanes<int>();
				nt = this->number_of_lanes<int>() - nl - nr;
				N = nl + nr + nt;

				float rt_adj, lt_adj, park_adj;
				park_adj = 1.0;



				//=====================================================================================================
				// get the adjusted right turn volume - for exclusive RT lanes
				if (this->right_turn_lane_type<Data_Structures::Turn_Lane_Types>() == Data_Structures::Turn_Lane_Types::Exclusive && nr > 1) rt_adj = 0.75;
				else rt_adj = 0.85;
				//----------------------------------------------------------------
				// correction for shared RT Lanes
				if (this->right_turn_lane_type<Data_Structures::Turn_Lane_Types>() == Data_Structures::Turn_Lane_Types::Shared)
				{
					nr = 1;
					vol_right_shared = vol_right / (nr * rt_adj);
				}
				vol_right = vol_right / (nr * rt_adj);



				//=====================================================================================================
				// get the adjusted left turn volume - if permitted use 0, and return the max of Vl, Vr, Vthru
				//----------------------------------------------------------------
				if (this->left_turn_type<Data_Structures::Left_Turn_Types>() == Data_Structures::Left_Turn_Types::Permitted || this->left_turn_type<Data_Structures::Left_Turn_Types>() == Data_Structures::Left_Turn_Types::None) vol_left = 0;
				//----------------------------------------------------------------
				// get the adjusted left turn volume - for protected / unopposed lanes
				else
				{
					if (nl > 1) lt_adj = 0.92;
					else lt_adj = 0.95;

					if (this->left_turn_type<Data_Structures::Left_Turn_Types>() == Data_Structures::Left_Turn_Types::Unopposed)
					{
						if (nl > 1) lt_adj = 0.75;
						else lt_adj = 0.85;
						vol_left_unopposed = vol_left / (nl * lt_adj);
					}
					vol_left = vol_left / (nl * lt_adj);
				}


				//=====================================================================================================
				// get the adjusted thru movement volume - with exclusive LT lane
				//----------------------------------------------------------------
				vol_thru = (vol_right_shared + vol_thru + vol_left_unopposed);
				if (this->left_turn_lane_type<Data_Structures::Turn_Lane_Types>() == Data_Structures::Turn_Lane_Types::Exclusive)
				{
					vol_thru = vol_thru / nt;
					return (max(vol_left_unopposed,max(vol_right-vol_right_shared,vol_thru)));
				}
				//----------------------------------------------------------------
				// get the adjusted thru movement volume - with shared LT lane
				else
				{
					// left turn proportion
					left_proportion = vol_left / (vol_left + vol_right + vol_thru);

					// through car equivalence
					float thru_car_equivalent;
					Interfaces::Lane_Group_Interface<ThisType,NULLTYPE>* opposing = this->opposing_lane<ThisType>();
					vol_opposing = opposing->demand_thru<TargetType>();
					if (vol_opposing < 200) thru_car_equivalent = 1.4;
					else if (vol_opposing < 400) thru_car_equivalent = 1.7;
					else if (vol_opposing < 600) thru_car_equivalent = 2.1;
					else if (vol_opposing < 800) thru_car_equivalent = 2.5;
					else if (vol_opposing < 1000) thru_car_equivalent = 3.1;
					else if (vol_opposing < 1200) thru_car_equivalent = 3.7;
					else if (vol_opposing > 1200) thru_car_equivalent = 4.5;

					// Left_Turn_adjustment
					float plan_lt_adj;
					if (this->left_turn_type<Data_Structures::Left_Turn_Types>() == Data_Structures::Left_Turn_Types::Permitted && N >= 2.0)
					{
						plan_lt_adj = max(  ((nt-1) + exp((-nt*vol_left*thru_car_equivalent)/600.0))/nt,  ((nt-1) + exp((-vol_left*this->max_cycle_length<Data_Structures::Time_Second>())/3600.0))/nt  ) ;
					}
					else if (this->left_turn_type<Data_Structures::Left_Turn_Types>() == Data_Structures::Left_Turn_Types::Permitted && N < 2.0)
					{
						plan_lt_adj = exp( -0.02 * (thru_car_equivalent + 10.0*left_proportion) * (vol_left * this->max_cycle_length<Data_Structures::Time_Second>()) / 3600.0);
					}
					else
					{
						if (vol_opposing < 1220.0) plan_lt_adj = 1.0 / ( 1.0 + (left_proportion * (235.0 + 0.435 * vol_opposing) / (1400.0 - vol_opposing)) );
						else plan_lt_adj = 1.0 / (1.0 + 4.525 * left_proportion);
					}

					// thru volume per lane
					vol_thru = vol_tot / (nt * plan_lt_adj);
					return (max(vol_right-vol_right_shared,vol_thru));
				}

			}
			facet TargetType Calculate_Lane_Volume(call_requires(TargetType,!Concepts::Is_Flow_Per_Hour))
			{
				assert_requirements(TargetType,Concepts::Is_Flow_Per_Hour,"Error - your TargetType is not a flow per hour measure.");
			}
			// Get the saturation flow rate for the lane group
			facet TargetType Calculate_Saturation_Flow_Rate(call_requirements(requires(ThisType,Concepts::Is_HCM_Full_Solution)))
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

				TargetType  s_o = this->base_saturation_flow<Data_Structures::Flow_Per_Hour>();
				TargetType  N = this->number_of_lanes<TargetType>();
				TargetType  fw = SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fw<TargetType>();
				TargetType  fHV = SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fHV<TargetType>();
				TargetType  fg = SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fg<TargetType>() ;
				TargetType  fp = SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fp<TargetType>() ;
				TargetType  fbb = SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fbb<TargetType>() ;
				TargetType  fa = SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fa<TargetType>() ;
				TargetType  fLU = SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fLU<TargetType>() ;
				TargetType  fLT = SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fLT<TargetType>() ;
				TargetType  fRT = SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fRT<TargetType>() ;
				TargetType  fLpb = SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fLpb<TargetType>() ;
				TargetType  fRpb = SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fRpb<TargetType>();

				TargetType s = this->base_saturation_flow<Data_Structures::Flow_Per_Hour>() *
					this->number_of_lanes<TargetType>() *
					SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fw<TargetType>()*
					SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fHV<TargetType>() *
					SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fg<TargetType>() *
					SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fp<TargetType>() *
					SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fbb<TargetType>() *
					SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fa<TargetType>() *
					SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fLU<TargetType>() *
					SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fLT<TargetType>() *
					SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fRT<TargetType>() *
					SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fLpb<TargetType>() *
					SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fRpb<TargetType>();

				return s;
			}
			facet TargetType Calculate_Saturation_Flow_Rate(call_requirements(requires(ThisType,Concepts::Is_HCM_Simple_Solution)))
			{
				TargetType s = this->base_saturation_flow<Data_Structures::Flow_Per_Hour>() *
					this->number_of_lanes<TargetType>() *
					this->peak_hour_factor<TargetType>() * 
					SUB_THIS(HCM_Lane_Group)->HCM_adjustment_fa<TargetType>();
				return s;
			}
			facet TargetType Calculate_Saturation_Flow_Rate(call_requirements(!(requires(ThisType,Concepts::Is_HCM_Simple_Solution) || requires(ThisType,Concepts::Is_HCM_Full_Solution))))
			{
				static_assert(false,"ThisType does not state whether it is a HCM_Full_Solution or an HCM_Simple_Solution.");
			}

			//=======================================================
			// Lane_Group Interface Members Specific to HCM Analysis
			//-------------------------------------------------------
			NAMESPACE(HCM_Lane_Group, Lane_Group_Interface)
			{
				// adjustment factor for lane width
				facet float HCM_adjustment_fw()
				{				
					float width = THIS->avg_lane_width<Data_Structures::Length_Foot>().value;
					State_Checks::valid_lane_width<ThisType>(this, width);
					return (1.0f + (width - 12.0f)/30.0f);
				}
				// adjustment factor for heavy vehicles in traffic stream
				facet float HCM_adjustment_fHV()
				{
					return 100.0f/(100.0f + THIS->percent_heavy_vehicles<Data_Structures::Percentage>());
				}
				// adjustment factor for approach grade
				facet float HCM_adjustment_fg()
				{
					float grade = THIS->grade<Data_Structures::Percentage>();
					State_Checks::valid_grade<ThisType>(this, grade);
					return 1.0f - grade / 200.0f;
				}
				// adjustment factor for existence of a parking lane and parking activity adjacent to lane group
				facet float HCM_adjustment_fp()
				{
					float parking = THIS->parking_activity<float>();
					State_Checks::valid_parking_manuevers<ThisType>(this, parking);
					float lanes = THIS->number_of_lanes<float>();
					return (lanes - 0.1f - 18.0f * parking /3600.0f)/lanes;
				}
				// adjustment factor for blocking effect of local buses that stop within intersection area
				facet float HCM_adjustment_fbb()
				{
					float buses = THIS->buses_per_hour<float>();
					State_Checks::valid_num_bus_stops<ThisType>(this, buses);
					float lanes = THIS->number_of_lanes<float>();
					return (lanes - 14.4f * buses /3600.0f)/lanes;
				}
				// adjustment factor for area type
				facet float HCM_adjustment_fa()
				{
					bool in_cbd = THIS->in_CBD<bool>();
					if (in_cbd) return 0.9f;
					else return 1.0f;
				}
				//  adjustment factor for lane utilization
				facet float HCM_adjustment_fLU()
				{
					return 1.0f;
					//return THIS->demand_lane_group<Data_Structures::Flow_Per_Hour>() / (THIS->demand_lane_max<Data_Structures::Flow_Per_Hour>() * THIS->number_of_lanes<float>());
				}
				// adjustment factor for left turns in lane group
				facet float HCM_adjustment_fLT()
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
				}
				// adjustment factor for right turns in lane group
				facet float HCM_adjustment_fRT()
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
				facet float HCM_adjustment_fLpb()
				{
					return 1.0f;
				}
				// pedestrian-bicycle adjustment factor for right-turn movements
				facet float HCM_adjustment_fRpb()
				{
					return 1.0f;
				}
			};


			//=======================================================
			// DATA Access Facets
			//-------------------------------------
			//Geometric conditions 
			facet_accessor(number_of_lanes);		///< N
			facet_accessor(number_of_right_lanes);	///< Nr
			facet_accessor(number_of_left_lanes);	///< Nl
			facet_accessor(avg_lane_width);			///< W (ft)
			facet_accessor(grade);					///< G(%)
			facet_accessor(has_left_turn);			///<
			facet_accessor(left_turn_type);			///< Protected, Permitted, Unopposed, None
			facet_accessor(left_turn_lane_type);	///< Exclusive, Shared, None
			facet_accessor(right_turn_lane_type);	///< Exclusive, Shared, None
			facet_accessor(has_right_turn);			///<
			facet_accessor(has_thru_move);			///<
			facet_accessor(in_CBD);					///<
			facet_accessor(length_left_turn);		///< Ls s (ft)
			facet_accessor(length_right_turn);		///< Lr (ft)
			facet_accessor(has_parking);			///<

			facet_accessor_interface(opposing_lane);

			//-------------------------------------
			//Traffic conditions 
			facet_accessor(demand_left);			///< V_l (veh/h)
			facet_accessor(demand_right);			///< V_r (veh/h)
			facet_accessor(demand_thru);			///< V (veh/h)
			facet TargetType demand_lane_group()
			{
				return (this->demand_left<TargetType>() + this->demand_right<TargetType>() + this->demand_thru<TargetType>());
			}
			facet TargetType demand_lane_max()
			{
				return max(this->demand_left<TargetType>() , max(this->demand_right<TargetType>() , this->demand_thru<TargetType>()));
			}
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
			facet_accessor(max_cycle_length);			///< C (s)
			facet_accessor(min_cycle_length);			///< C (s)
			facet_accessor(green_time);				///< G (s)
			facet_accessor(yellow_and_all_red_time);///< Y (s)
			facet_accessor(is_actuated);			///< true if actuated, false if pre-timed
			facet_accessor(has_pedestrian_button);	///< true if has pedestrian push button
			facet_accessor(min_pedestrian_green);	///< G_p (s)
			facet_accessor(phase_plan);				///< 
			facet_accessor(analysis_period);		///< T (h)
		};


		//------------------------------------------------------------------------------------------------------------------
		/// Inteface to the basic POLARIS Signal Phase Component
		//------------------------------------------------------------------------------------------------------------------
		template<typename ThisType=NULLTYPE, typename CallerType=NULLTYPE>
		struct Phase_Interface
		{
			//============================================================
			//  Intialize dispatched
			//------------------------------------------------------------
			facet void Initialize(TargetType number_of_lane_groups)
			{
				return PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TargetType>(number_of_lane_groups);
			}


			//============================================================
			//  Member functions
			//------------------------------------------------------------
			/// Get the maximum saturation ratio from amonst the individual lane_group in the phase
			facet TargetType Find_Critical_VS_Ratio(call_requirements(
				requires(ThisType, Concepts::Has_Child_Lane_Group) && 
				requires(ThisType, Concepts::Is_HCM_Full_Solution) && 
				requires(TargetType, is_arithmetic) /*&& 
				requires_2(typename ThisType::Lane_Group_Interface<typename ThisType::Lane_Group_Type::type>::type&, Interfaces::Lane_Group_Interface<typename ThisType::Lane_Group_Type::type>&, is_convertible)*/))
			{
				// Get reference to the Lane Groups in the current phase
				typedef ThisType T;
				typedef Interfaces::Lane_Group_Interface<typename T::Lane_Group_Type<Execution_Object,T>::type, CallerType>* LaneGroupItf;
				vector<LaneGroupItf>* lane_group = this->Lane_Groups<vector<LaneGroupItf>*>();
				vector<LaneGroupItf>::iterator itr = lane_group->begin();


				// initialize critical vs ratio
				TargetType vs_crit = 0;

				// search each lane group in the phase, return the one with highest vs
				for (itr; itr != lane_group->end(); itr++)
				{
					TargetType vs_i = (*itr)->Calculate_VS_ratio<TargetType>();
					if (vs_i > vs_crit) vs_crit = vs_i;
				}
				return vs_crit;
			}

			facet TargetType Find_Critical_VS_Ratio(call_requirements(!(
				requires(ThisType,Concepts::Has_Child_Lane_Group) && 
				requires(ThisType, Concepts::Is_HCM_Full_Solution) && 
				requires(TargetType, is_arithmetic) /*&& 
				requires_2(typename ThisType::Lane_Group_Interface<typename ThisType::Lane_Group_Type::type>::type&, Interfaces::Lane_Group_Interface<typename ThisType::Lane_Group_Type::type>::type&, is_convertible)*/)))
			{
				assert_requirements(ThisType,Concepts::Has_Child_Lane_Group,"Your Phase component does not have a LaneGroupType defined.  Please add a lane group type to the base.");
				assert_requirements(ThisType,Concepts::Is_HCM_Full_Solution,"Your Phase component does not have a solution type defined.  Please add 'HCM_Simple' or HCM_Full' type tag.");
				assert_requirements_std(TargetType,is_arithmetic,"Your TargetType is not arithmetic.");
			}

			/// Get the maximum critical phase volume from amonst the individual lane_group in the phase
			facet TargetType Find_Critical_Phase_Volume(call_requirements(
				requires(ThisType,Concepts::Has_Child_Lane_Group) && 
				requires(ThisType, Concepts::Is_HCM_Simple_Solution) && 
				requires(TargetType, Concepts::Is_Flow_Per_Hour) /*&& 
				requires_2(typename ThisType::Lane_Group_Interface<typename ThisType::Lane_Group_Type::type>::type&, Interfaces::Lane_Group_Interface<typename ThisType::Lane_Group_Type::type>::type&, is_convertible)*/))
			{
				// Get reference to the Lane Groups in the current phase
				typedef ThisType T;
				typedef Interfaces::Lane_Group_Interface<typename T::Lane_Group_Type<Execution_Object,T>::type, CallerType>* LaneGroupItf;
				vector<LaneGroupItf>* lane_group = this->Lane_Groups<vector<LaneGroupItf>*>();
				vector<LaneGroupItf>::iterator itr = lane_group->begin();

				
				// initialize critical vs ratio
				TargetType v_crit = 0;

				// search each lane group in the phase, return the one with highest volume
				for (itr; itr != lane_group->end(); itr++)
				{
					TargetType v_i = (*itr)->Calculate_Lane_Volume<TargetType>();
					if (v_i > v_crit) v_crit = v_i;
				}
				return v_crit;
			}
			
			facet TargetType Find_Critical_Phase_Volume(call_requirements(!(
				requires(ThisType, Concepts::Has_Child_Lane_Group) && 
				requires(ThisType, Concepts::Is_HCM_Simple_Solution) && 
				requires(TargetType, Concepts::Is_Flow_Per_Hour) /*&& 
				requires_2(typename ThisType::Lane_Group_Interface<typename ThisType::Lane_Group_Type::type>::type&, Interfaces::Lane_Group_Interface<typename ThisType::Lane_Group_Type::type>::type&, is_convertible)*/)))
			{
				assert_requirements(ThisType,Concepts::Has_Child_Lane_Group,"Your Phase component does not have a LaneGroupType defined.  Please add a lane group type to the base.");
				assert_requirements(ThisType,Concepts::Is_HCM_Simple_Solution,"Your Phase component does not have a solution type defined.  Please add 'HCM_Simple' or HCM_Full' type tag.");
				assert_requirements_std(TargetType,Concepts::Is_Flow_Per_Hour,"Your TargetType is not a flow per hour measure.");
				//assert_requirements(ThisType::Lane_Group_Type::type,Is_Polaris_Component,"Your lane group is not a valid polaris component");
				//assert_requirements_2(ThisType::LaneGroupType::Interface_Type&,Lange_Group_Interface,is_convertible,"Error - your lane group type is not convertible to the base lane group interface.");
			}


			//============================================================
			//  SIGNAL STATUS ACCESSORS
			//------------------------------------------------------------
			facet_accessor(signal_state);


			//============================================================
			//  BASIC ACCESSORS
			//------------------------------------------------------------
			/// Phase green time
			facet_accessor(green_time);					///< G (s)
			/// Phase lost time
			facet_accessor(yellow_and_all_red_time);	///< Y (s)
			/// Local facet to return the list of lane groups associated with phase
			facet_accessor(Lane_Groups);


			//============================================================
			//  PARENT CLASS ACCESSORS
			//------------------------------------------------------------
			facet_accessor(cycle_length);			///< C (s)
			facet_accessor(max_cycle_length);			///< C (s)
			facet_accessor(min_cycle_length);			///< C (s)
			facet_accessor(in_CBD);				/// 0.9 for CBD else 1.0
			facet_accessor(analysis_period);		///< T (h)
		};


		//------------------------------------------------------------------------------------------------------------------
		/// Inteface to the basic POLARIS Signal component
		//------------------------------------------------------------------------------------------------------------------
		template<typename ThisType=NULLTYPE, typename CallerType=NULLTYPE>
		struct Signal_Interface
		{
			//=======================================================
			// Initialization and Calculations
			//-------------------------------------------------------
			/// Dispatch the Initialize function call to the component base
			facet void Initialize(TargetType num_phases)
			{
				return PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TargetType>(num_phases);
			}

			/// Add a new lane group to the signal controller
			facet void Add_Phase(typename TargetType::Interface_Type* new_lane_group)
			{
				PTHIS(ThisType)->Add_Lane_Group(new_lane_group);
			}

			/// HCM Retiming calculations
			facet void Update_Timing(call_requirements(
				requires(ThisType,Concepts::Has_Child_Phase) && 
				requires(TargetType, Concepts::Is_Time_Seconds) && 
				requires(typename ThisType, Concepts::Is_HCM_Full_Solution) /*&& 
				requires_2(typename ThisType::Phase_Interface<typename ThisType::Phase_Type<>::type>&, Interfaces::Phase_Interface<typename ThisType::Phase_Type<>::type>&, is_convertible)*/))
			{
				// Simplify ThisType name
				typedef ThisType T;

				// Get reference to the phases in the signal phase diagram
				vector<Interfaces::Phase_Interface<typename T::Phase_Type<Execution_Object,T>::type,NULLTYPE>*>* phases = this->Phases<vector<Interfaces::Phase_Interface<T::Phase_Type<Execution_Object,T>::type,NULLTYPE>*>*>();
				vector<Interfaces::Phase_Interface<typename T::Phase_Type<Execution_Object,T>::type,NULLTYPE>*>::iterator itr = phases->begin();

				// Sum total lost time and critical vs for all phases
				float lost_time = 0;
				float vs_sum = 0;
				for (itr; itr != phases->end(); itr++)
				{
					lost_time += (*itr)->yellow_and_all_red_time<TargetType>();
					vs_sum += (*itr)->Find_Critical_VS_Ratio<float>();
				}

				// Get estimated cycle length
				TargetType cycle;
				cycle = (TargetType)((float)lost_time) * (this->degree_of_saturation<float>()) / (1.0f - vs_sum);
				this->cycle_length<TargetType>(cycle);
				TargetType effective_cycle = cycle - lost_time;
				

				// Set green time for each phase
				TargetType vs;
				for (itr=phases->begin(); itr != phases->end(); itr++)
				{
					vs = (*itr)->Find_Critical_VS_Ratio<float>();
					(*itr)->green_time<TargetType>(effective_cycle * vs / vs_sum);
				}
			}

			/// HCM Retiming calculations
			facet void Update_Timing(call_requirements(
				requires(ThisType,Concepts::Has_Child_Phase) && 
				requires(TargetType, Concepts::Is_Time_Seconds) && 
				requires(typename ThisType, Concepts::Is_HCM_Simple_Solution) /*&& 
				requires_2(typename ThisType::Phase_Interface<typename ThisType::Phase_Type<>::type>&, Interfaces::Phase_Interface<typename ThisType::Phase_Type<>::type>&, is_convertible)*/))
			{
				// Simplify ThisType name
				typedef ThisType T;

				// Get reference to the phases in the signal phase diagram
				vector<Interfaces::Phase_Interface<typename T::Phase_Type<Execution_Object,T>::type,NULLTYPE>*>* phases = this->Phases<vector<Interfaces::Phase_Interface<T::Phase_Type<Execution_Object,T>::type,NULLTYPE>*>*>();
				vector<Interfaces::Phase_Interface<typename T::Phase_Type<Execution_Object,T>::type,NULLTYPE>*>::iterator itr = phases->begin();

				// Sum total lost time and critical vs for all phases
				float lost_time = 0;
				float critical_sum = 0;
				for (itr; itr != phases->end(); itr++)
				{
					lost_time += (*itr)->yellow_and_all_red_time<Data_Structures::Time_Second>();
					critical_sum += (*itr)->Find_Critical_Phase_Volume<Data_Structures::Flow_Per_Hour>();
				}

				//=============================================================================================
				// Get estimated cycle length
				//---------------------------------------------------------------------------------------------
				TargetType cycle;
				// area factor
				float f_a = 1.0;
				if (this->in_CBD<bool>()) f_a = 0.9;
				// calculate reference sum
				float reference_sum = 1530 * this->peak_hour_factor<float>() * f_a;
				// cycle length calculation
				cycle = (TargetType)((float)lost_time) / (1.0f - critical_sum / reference_sum);
				this->cycle_length<Data_Structures::Time_Second>(cycle);
				TargetType effective_cycle = cycle - lost_time;
				

				// Set green time for each phase
				for (itr=phases->begin(); itr != phases->end(); itr++)
				{
					float vol = (*itr)->Find_Critical_Phase_Volume<Data_Structures::Flow_Per_Hour>();
					(*itr)->green_time<Data_Structures::Time_Second>((TargetType)((float)effective_cycle * vol / critical_sum));
				}
			}

			/// HCM Retiming calculations
			facet void Update_Timing(call_requirements(!(
				requires(ThisType,Concepts::Has_Child_Phase) && 
				requires(TargetType, Concepts::Is_Time_Seconds) && 
				(requires(ThisType, Concepts::Is_HCM_Full_Solution) || requires(ThisType, Concepts::Is_HCM_Simple_Solution)) /*&&
				requires_2(ThisType::Phase_Interface<typename ThisType::Phase_Type<>::type>&, Interfaces::Phase_Interface<typename ThisType::Phase_Type<>::type>&, is_convertible)*/)))
			{
				assert_requirements(ThisType, Concepts::Has_Child_Phase,"Your Signal Type does not have a Phase child class type defined.");
				assert_requirements(TargetType, Concepts::Is_Time_Seconds, "Your TargetType is not a measure of time in seconds.");
				assert_requirements(ThisType, Concepts::Is_HCM_Full_Solution, "Your SignalType does not specify a full HCM solution, and");
				assert_requirements(ThisType, Concepts::Is_HCM_Full_Solution, "Your SignalType does not specify a simple HCM solution");
				assert_requirements_2(ThisType::Phase_Interface&, Interfaces::Phase_Interface&, is_convertible, "Your ThisType::Phase_Interface specifies can not be cast to a Interface::Phase_Interface.");
			}


			//=======================================================
			// Signal Events
			//-------------------------------------------------------
			declare_facet_event


			//=======================================================
			// Accessor facets
			//-------------------------------------------------------
			/// Analysis Period
			facet_accessor(analysis_period);		///< T (h)
			/// Total cycle length
			facet_accessor(cycle_length);			///< C (s)
			/// Total cycle length
			facet_accessor(max_cycle_length);			///< C (s)
			/// Total cycle length
			facet_accessor(min_cycle_length);			///< C (s)
			/// Total cycle length
			facet_accessor(degree_of_saturation);	///< X (s)
			/// Area type
			facet_accessor(in_CBD);
			/// Signal ID accessor
			facet_accessor(Signal_ID);
			/// Local facet to return the list of phases associated with signal
			facet_accessor(Phases);
			// phf for the signal as a whole
			facet_accessor(peak_hour_factor);

			// Currently active (green) signal phase
			facet_accessor(active_phase);
		};
	}


	//==================================================================================================================
	/// Namespace of useful related data structures for the components
	//------------------------------------------------------------------------------------------------------------------
	namespace Data_Structures
	{
		// Flow Measures
		Basic_Data_Struct(Flow_Per_Day,float, Unit_Per_Day, Flow);
		Basic_Data_Struct(Flow_Per_Hour,float, Unit_Per_Hour, Flow);
		Basic_Data_Struct(Flow_Per_15_Minute,float, Unit_Per_15_Minute, Flow);
		Basic_Data_Struct(Flow_Per_Minute,float, Unit_Per_Minute, Flow);
		Basic_Data_Struct(Flow_Per_Second,float, Unit_Per_Second, Flow);

		// Length Measures
		Basic_Data_Struct(Length_Foot,float,Feet,Length);
		Basic_Data_Struct(Length_Meter,float,Meters,Length);
		Basic_Data_Struct(Length_Kilometer,float,Kilometers,Length);
		Basic_Data_Struct(Length_Mile,float,Miles,Length);

		// Time Measures
		Basic_Data_Struct(Time_Second, float,Seconds,Time);
		Basic_Data_Struct(Time_Minute, float,Minutes,Time);
		Basic_Data_Struct(Time_Hour, float,Hours,Time);
		Basic_Data_Struct(Time_Day, float,Days,Time);

		// Numerical Types
		Basic_Data_Struct(Percentage,float,Percent);


		enum Left_Turn_Types { Protected, Permitted, Unopposed, None };

		enum Turn_Lane_Types { Exclusive, Shared };

		enum Signal_State { GREEN, YELLOW, RED };

	}

}
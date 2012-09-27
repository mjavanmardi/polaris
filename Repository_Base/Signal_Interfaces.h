#pragma once

#include "Repository_Base_Includes.h"


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
			requires_named_member(none, Phases, "ThisType does not have a Phase child type: no Phase_Base type is defined.");
			end_requirements_list(Phases);
		};
		#pragma endregion

		#pragma region Phase Concepts
		concept Has_Child_Lane_Group
		{
			begin_requirements_list;
			requires_named_member(none, Lane_Groups, "ThisType does not have a Lane_Group child type: no Lane_Group_Base type is defined.");
			end_requirements_list(Lane_Groups);
		};
		#pragma endregion

		#pragma region Units Concepts
		concept Is_Percentage
		{
			begin_requirements_list;
			requires_typename_state(none, Percent, true_type,"Type does not model a percentage concept.");
			end_requirements_list(Percent);
		};


		concept Is_Measure
		{
			begin_requirements_list;
			requires_typename_defined(none, Length, "Type does not model a measurement concept.");
			requires_typename_defined(Length,ValueType,"Your measure type does not have an arithmetic ValueType defined.");
			end_requirements_list(ValueType);
		};
		concept Is_Foot_Measure
		{
			begin_requirements_list;
			requires_concept(none, Is_Measure);
			requires_typename_defined(Is_Measure, Feet, "Type does not model a foot measurement concept.");
			end_requirements_list(Feet);
		};
		concept Is_Meter_Measure
		{
			begin_requirements_list;
			requires_concept(none, Is_Measure);
			requires_typename_defined(Is_Measure, Meters, "Type does not model a meter measure concept.");
			end_requirements_list(Meters);
		};


		concept Is_Time
		{
			begin_requirements_list;
			requires_typename_defined(none, Time, "Type does not model a Time concept.");
			requires_typename_defined(Time,ValueType,"Your measure type does not have an arithmetic ValueType defined.");
			end_requirements_list(ValueType);
		};
		concept Is_Time_Seconds
		{
			begin_requirements_list;
			requires_concept(none, Is_Time);
			requires_typename_defined(Is_Time, Seconds, "Type does not model a Time in Seconds concept.");
			end_requirements_list(Seconds);
		};
		concept Is_Time_Minutes
		{
			begin_requirements_list;
			requires_concept(none, Is_Time);
			requires_typename_defined(Is_Time, Minutes, "Type does not model a Time in Seconds concept.");
			end_requirements_list(Minutes);
		};


		concept Is_Flow
		{
			begin_requirements_list;
			requires_typename_defined(none, Flow, "Type does not model a Flow concept.");
			requires_typename_defined(Flow,ValueType,"Your measure type does not have an arithmetic ValueType defined.");
			end_requirements_list(ValueType);
		};
		concept Is_Flow_Per_Hour
		{
			begin_requirements_list;
			requires_concept(none, Is_Flow);
			requires_typename_defined(Is_Flow, Unit_Per_Hour, "Type does not model a Flow per hour concept.");
			end_requirements_list(Unit_Per_Hour);
		};
		concept Is_Flow_Per_15_Minutes
		{
			begin_requirements_list;
			requires_concept(none, Is_Flow);
			requires_typename_defined(Is_Flow, Unit_Per_15_Minute, "Type does not model a Flow per 15 minute time period concept.");
			end_requirements_list(Unit_Per_15_Minute);
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
			facet void Update_Demand(TargetType time, call_requires(TargetType, Concepts::Is_Time))
			{
				Interfaces::Detector_Interface<typename ThisType::Master_Type::DETECTOR_TYPE,NULLTYPE>* left = this->Detector_Left<typename ThisType::Master_Type::DETECTOR_TYPE>();
				Interfaces::Detector_Interface<typename ThisType::Master_Type::DETECTOR_TYPE,NULLTYPE>* right = this->Detector_Right<typename ThisType::Master_Type::DETECTOR_TYPE>();
				Interfaces::Detector_Interface<typename ThisType::Master_Type::DETECTOR_TYPE,NULLTYPE>* thru = this->Detector_Thru<typename ThisType::Master_Type::DETECTOR_TYPE>();
				Data_Structures::Flow_Per_Hour flow;
				flow = left->count<Target_Type<Data_Structures::Flow_Per_Hour,Data_Structures::Time_Second>>(time);
				this->demand_left<Data_Structures::Flow_Per_Hour>(flow);
				flow = right->count<Target_Type<Data_Structures::Flow_Per_Hour,Data_Structures::Time_Second>>(time);
				this->demand_right<Data_Structures::Flow_Per_Hour>(flow);
				flow = thru->count<Target_Type<Data_Structures::Flow_Per_Hour,Data_Structures::Time_Second>>(time);
				this->demand_thru<Data_Structures::Flow_Per_Hour>(flow);
			}
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
				if (this->has_parking<bool>()) park_adj = 0.9;



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
					vol_right = vol_right_shared;
				}
				else if (this->right_turn_lane_type<Data_Structures::Turn_Lane_Types>() == Data_Structures::Turn_Lane_Types::No_Turn)
				{
					vol_right = 0;
				}
				else
				{
					vol_right = vol_right / (nr * rt_adj);
				}



				//=====================================================================================================
				// get the adjusted left turn volume - if permitted use 0, and return the max of Vl, Vr, Vthru
				//----------------------------------------------------------------
				if (this->left_turn_type<Data_Structures::Left_Turn_Types>() == Data_Structures::Left_Turn_Types::Permitted || this->left_turn_type<Data_Structures::Left_Turn_Types>() == Data_Structures::Left_Turn_Types::None) vol_left = 0;
				//----------------------------------------------------------------
				// get the adjusted left turn volume - for protected / unopposed lanes
				else
				{			

					if (this->left_turn_lane_type<Data_Structures::Turn_Lane_Types>() == Data_Structures::Turn_Lane_Types::Shared) nl = 1;

					if (this->left_turn_type<Data_Structures::Left_Turn_Types>() == Data_Structures::Left_Turn_Types::Unopposed)
					{
						if (nl > 1) lt_adj = 0.75;
						else lt_adj = 0.85;		
					}
					else
					{
						if (nl > 1) lt_adj = 0.92;
						else lt_adj = 0.95;
					}
					vol_left_unopposed = vol_left / (nl * lt_adj);
					vol_left = vol_left / (nl * lt_adj);
				}


				//=====================================================================================================
				// get the adjusted thru movement volume - with exclusive LT lane
				//----------------------------------------------------------------
				vol_thru = (vol_right_shared + vol_thru + vol_left_unopposed) / park_adj;
				if (this->left_turn_lane_type<Data_Structures::Turn_Lane_Types>() == Data_Structures::Turn_Lane_Types::Exclusive)
				{
					if (nt == 0) vol_thru = 0;
					else vol_thru = vol_thru / nt;

					return (max(vol_left_unopposed,max(vol_right-vol_right_shared,vol_thru)));
				}
				//----------------------------------------------------------------
				// get the adjusted thru movement volume - with shared LT lane
				else
				{
					// left turn proportion
					left_proportion = vol_left / (max(vol_left + vol_right + vol_thru,1));

					// through car equivalence
					float thru_car_equivalent=1.0;
					Interfaces::Lane_Group_Interface<ThisType,NULLTYPE>* opposing = this->opposing_lane<ThisType>();
					if (opposing == NULL) vol_opposing = 0;
					else vol_opposing = opposing->demand_thru<TargetType>();
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
						plan_lt_adj = (float)max(  ((nt-1) + exp((-nt*vol_left*thru_car_equivalent)/600.0))/nt,  ((nt-1) + exp((-vol_left*this->max_cycle_length<Data_Structures::Time_Second>())/3600.0))/nt  ) ;
					}
					else if (this->left_turn_type<Data_Structures::Left_Turn_Types>() == Data_Structures::Left_Turn_Types::Permitted && N < 2.0)
					{
						plan_lt_adj = (float)exp( -0.02 * (thru_car_equivalent + 10.0*left_proportion) * (vol_left * this->max_cycle_length<Data_Structures::Time_Second>()) / 3600.0);
					}
					else
					{
						if (vol_opposing < 1220.0) plan_lt_adj = 1.0f / ( 1.0f + (left_proportion * (235.0 + 0.435 * vol_opposing) / (1400.0 - vol_opposing)) );
						else plan_lt_adj = 1.0 / (1.0 + 4.525 * left_proportion);
					}

					// thru volume per lane
					if (nt == 0) vol_thru = 0;
					else vol_thru = vol_thru / (nt * plan_lt_adj);
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

				TargetType s = s_o * N * fw * fHV * fg * fp * fbb * fa * fLU * fLT * fRT * fLpb * fRpb;

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
					float width = THIS->avg_lane_width<Data_Structures::Length_Foot>();
					State_Checks::valid_lane_width<ThisType, CallerType, TargetType>(this, width);
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
					State_Checks::valid_grade<ThisType,CallerType,TargetType>(this, grade);
					return 1.0f - grade / 200.0f;
				}
				// adjustment factor for existence of a parking lane and parking activity adjacent to lane group
				facet float HCM_adjustment_fp()
				{
					float parking = THIS->parking_activity<float>();
					State_Checks::valid_parking_manuevers<ThisType,CallerType,TargetType>(this, parking);
					if (parking == 0.0f) return 1.0f;
					float lanes = THIS->number_of_lanes<float>();
					return (lanes - 0.1f - 18.0f * parking /3600.0f)/lanes;
				}
				// adjustment factor for blocking effect of local buses that stop within intersection area
				facet float HCM_adjustment_fbb()
				{
					float buses = THIS->buses_per_hour<float>();
					State_Checks::valid_num_bus_stops<ThisType,CallerType,TargetType>(this, buses);
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
					return 1.0f;
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
			// Demand data source Facets
			//-------------------------------------------------------
			facet_accessor_interface(Detector_Left);
			facet_accessor_interface(Detector_Right);
			facet_accessor_interface(Detector_Thru);
			facet_accessor(Lane_Group_LOS);

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
				this->signal_state<Data_Structures::Signal_State>(Data_Structures::Signal_State::RED);
				return PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TargetType>(number_of_lane_groups);
			}


			//============================================================
			//  Phase Calculation Members
			//------------------------------------------------------------
			/// Get the maximum saturation ratio from amonst the individual lane_group in the phase
			facet TargetType Find_Critical_VS_Ratio(call_requirements(
				requires(ThisType, Concepts::Is_HCM_Full_Solution) && 
				requires(TargetType, is_arithmetic)))
			{
				// Get reference to the Lane Groups in the current phase
				typedef ThisType T;
				typedef Interfaces::Lane_Group_Interface<typename ThisType::Master_Type::LANE_GROUP_TYPE, CallerType>* LaneGroupItf;
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
				requires(ThisType, Concepts::Is_HCM_Full_Solution) && 
				requires(TargetType, is_arithmetic))))
			{
				assert_requirements(ThisType,Concepts::Has_Child_Lane_Group,"Your Phase component does not have a LaneGroupType defined.  Please add a lane group type to the base.");
				assert_requirements(ThisType,Concepts::Is_HCM_Full_Solution,"Your Phase component does not have a solution type defined.  Please add 'HCM_Simple' or HCM_Full' type tag.");
				assert_requirements_std(TargetType,is_arithmetic,"Your TargetType is not arithmetic.");
			}

			/// Get the maximum critical phase volume from amonst the individual lane_group in the phase
			facet TargetType Find_Critical_Phase_Volume(call_requirements(
				requires(ThisType, Concepts::Is_HCM_Simple_Solution) && 
				requires(TargetType, Concepts::Is_Flow_Per_Hour)))
			{
				// Get reference to the Lane Groups in the current phase
				typedef ThisType T;
				typedef Interfaces::Lane_Group_Interface<typename ThisType::Master_Type::LANE_GROUP_TYPE, CallerType>* LaneGroupItf;
				vector<LaneGroupItf>* lane_group = this->Lane_Groups<vector<LaneGroupItf>*>();
				vector<LaneGroupItf>::iterator itr = lane_group->begin();

				
				// initialize critical vs ratio
				TargetType v_crit = 0;

				// search each lane group in the phase, return the one with highest volume
				for (itr; itr != lane_group->end(); itr++)
				{
					//TargetType v_i = (*itr)->demand_lane_max<TargetType>();
					TargetType v_i = (*itr)->Calculate_Lane_Volume<TargetType>();
					if (v_i > v_crit) v_crit = v_i;
				}
				return v_crit;
			}
			
			facet TargetType Find_Critical_Phase_Volume(call_requirements(!(
				requires(ThisType, Concepts::Is_HCM_Simple_Solution) && 
				requires(TargetType, Concepts::Is_Flow_Per_Hour))))
			{
				assert_requirements(ThisType,Concepts::Is_HCM_Simple_Solution,"Your Phase component does not have a solution type defined.  Please add 'HCM_Simple' or HCM_Full' type tag.");
				assert_requirements(TargetType,Concepts::Is_Flow_Per_Hour,"Your TargetType is not a flow per hour measure.");
				//assert_requirements(ThisType::Lane_Group_Type::type,Is_Polaris_Component,"Your lane group is not a valid polaris component");
				//assert_requirements_2(ThisType::LaneGroupType::Interface_Type&,Lange_Group_Interface,is_convertible,"Error - your lane group type is not convertible to the base lane group interface.");
			}

			facet void Update_Demand(TargetType time, call_requirements(
				requires(TargetType, Concepts::Is_Time)))
			{
				// Get reference to the Lane Groups in the current phase
				typedef ThisType T;
				typedef Interfaces::Lane_Group_Interface<typename ThisType::Master_Type::LANE_GROUP_TYPE, CallerType>* LaneGroupItf;
				vector<LaneGroupItf>* lane_group = this->Lane_Groups<vector<LaneGroupItf>*>();
				vector<LaneGroupItf>::iterator itr = lane_group->begin();

				for (itr; itr!=lane_group->end(); itr++)
				{
					(*itr)->Update_Demand<TargetType>(time);
				}
			}
	
			facet void Update_Demand(TargetType time, call_requirements(!(
				requires(TargetType, Concepts::Is_Time))))
			{
				assert_requirements(ThisType, Concepts::Has_Child_Lane_Group, "ThisType does not have an associated LaneGroupType as a child element.");
				assert_requirements(TargetType, Concepts::Is_Time, "The TargetType specified is not  a Time object.");
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
			/// Weight factor used when setting the phase green times (multiplier for the phase volume)
			facet_accessor(weight);
			/// Local facet to return the list of lane groups associated with phase
			facet_accessor(Lane_Groups);
			facet_accessor(name);
			facet_accessor(phase_id);


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
		/// Inteface to the basic POLARIS Signal Phase Component
		//------------------------------------------------------------------------------------------------------------------
		template<typename ThisType=NULLTYPE, typename CallerType=NULLTYPE>
		struct Approach_Interface
		{
			//============================================================
			//  Intialize dispatched
			//------------------------------------------------------------
			facet void Initialize()
			{
				return PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TargetType>();
			}
			facet void Add_Lane_Group(typename TargetType::Interface_Type<TargetType,NULLTYPE>::type* lane_group)
			{
				PTHIS(ThisType)->Add_Lane_Group<Dispatch<ThisType>,CallerType,TargetType>(lane_group);
			}

			//============================================================
			//  Approach LOS and Delay Members
			//------------------------------------------------------------
			/// Get the maximum saturation ratio from amonst the individual lane_group in the phase
			facet void Calculate_Approach_LOS(call_requirements(
				requires(TargetType, is_arithmetic)))
			{
				// Get reference to the Lane Groups in the current phase
				typedef ThisType T;
				typedef Interfaces::Lane_Group_Interface<typename ThisType::Master_Type::LANE_GROUP_TYPE, CallerType>* LaneGroupItf;
				vector<LaneGroupItf>* lane_group = this->Lane_Groups<vector<LaneGroupItf>*>();
				vector<LaneGroupItf>::iterator itr = lane_group->begin();

				// initialize critical vs ratio
				float sum_delay = 0;
				float sum_flow = 0;

				// search each lane group in the approach and set the LOS and delay
				for (itr; itr != lane_group->end(); itr++)
				{
					LaneGroupItf lg = *itr;
					float g = lg->green_time<Data_Structures::Time_Second>();
					float cycle = lg->cycle_length<Data_Structures::Time_Second>();
					float v = lg->demand_lane_group<Data_Structures::Flow_Per_Hour>();
					float gC = g / cycle;
					float c = lg->Calculate_Saturation_Flow_Rate<float>() * gC;
					float X = v/c;

					float d1 = 0.5 * lg->cycle_length<Data_Structures::Time_Second>() * pow((1.0f - gC),2.0f) / (1.0f - min(1.0,X)*gC);

					float T = lg->analysis_period<Data_Structures::Time_Minute>()/60.0f;
					float k = 0.5f;
					float d2 = 900.0f * T * ((X-1.0f) + sqrt(pow((X-1.0f),2.0f) + 8.0f*k*1.0*X/(c*T)));
					float PF = 1.0;
					float d3 = 0.0f;

					sum_flow += v;
					sum_delay += (d1*PF + d2 + d3) * v;
				}

				sum_flow > 0 ? this->delay<Data_Structures::Time_Second>(sum_delay/sum_flow) : this->delay<Data_Structures::Time_Second>(0.0f);

				// convert delay to LOS
				float delay = this->delay<Data_Structures::Time_Second>();
				if (delay < 10) this->LOS<char>('A');
				else if (delay < 20) this->LOS<char>('B');
				else if (delay < 35) this->LOS<char>('C');
				else if (delay < 55) this->LOS<char>('D');
				else if (delay < 80) this->LOS<char>('E');
				else this->LOS<char>('F');

				// get total approach flow rate, for use in calculating signal LOS
				this->approach_flow_rate<Data_Structures::Flow_Per_Hour>(sum_flow);
			}

			facet void Calculate_Approach_LOS(call_requirements(!(
				requires(TargetType, is_arithmetic))))
			{
				assert_requirements_std(TargetType,is_arithmetic,"Your TargetType is not arithmetic.");
			}


			//============================================================
			//  BASIC ACCESSORS
			//------------------------------------------------------------
			/// Phase green time
			facet_accessor(delay);					///< d (s)
			/// Flow rate for combined lane groups in the approach
			facet_accessor(approach_flow_rate);
			/// Phase lost time
			facet_accessor(LOS);					
			/// Local facet to return the list of lane groups associated with phase
			facet_accessor(Lane_Groups);
			/// Local facet to return the approach name (if applicable)
			facet_accessor(name);

			//============================================================
			//  PARENT CLASS ACCESSORS
			//------------------------------------------------------------
			facet_accessor(cycle_length);				///< C (s)
			facet_accessor(max_cycle_length);			///< C (s)
			facet_accessor(min_cycle_length);			///< C (s)
			facet_accessor(in_CBD);						///< 0.9 for CBD else 1.0
			facet_accessor(analysis_period);			///< T (h)
		};


		//------------------------------------------------------------------------------------------------------------------
		/// Inteface to the basic POLARIS Signal component
		//------------------------------------------------------------------------------------------------------------------
		template<typename ThisType=NULLTYPE, typename CallerType=NULLTYPE>
		struct Signal_Interface
		{
			//=======================================================
			// Initialization
			//-------------------------------------------------------
			/// Dispatch the Initialize function call to the component base
			facet void Initialize(TargetType num_phases, TargetType number_of_approaches)
			{		
				PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TargetType>(num_phases, number_of_approaches);
				this->Next_Event_Iteration<int>(0);
				this->Event_Has_Fired<bool>(false);
				this->Event_Conditional_Hit<bool>(false);
				this->Next_Timing_Event_Iteration<int>(0);
				this->Timing_Event_Has_Fired<bool>(false);
				this->Timing_Event_Conditional_Hit<bool>(false);
				this->active_phase<int>(0);
				this->num_cycles_between_updates<int>(0);
				schedule_event_local(ThisType,Signal_Check_Conditional, Change_Signal_State, 0, NULLTYPE);
			}
	
			//=======================================================
			// HCM Retiming calculations
			//-------------------------------------------------------
			/// HCM Retiming calculations for complex signals, with full information
			facet void Update_Timing(call_requirements(
				requires(TargetType, Concepts::Is_Time_Seconds) && 
				requires(typename ThisType, Concepts::Is_HCM_Full_Solution)))
			{

				//float weight;
				//cout <<endl<< "Enter a weight: ";
				//cin >> weight;

				// Simplify ThisType name
				typedef ThisType T;

				Interfaces::Signal_Interface<typename ThisType::Master_Type::SIMPLE_SIGNAL_TYPE,NULLTYPE>* simple_signal = (Interfaces::Signal_Interface<typename ThisType::Master_Type::SIMPLE_SIGNAL_TYPE,NULLTYPE>*)this;
				simple_signal->Update_Timing<Data_Structures::Time_Second>();

				ostream* out = this->output_stream<ostream*>();

				// Get reference to the phases in the signal phase diagram
				vector<Interfaces::Phase_Interface<typename ThisType::Master_Type::FULL_PHASE_TYPE,NULLTYPE>*>* phases = this->Phases<vector<Interfaces::Phase_Interface<typename ThisType::Master_Type::FULL_PHASE_TYPE,NULLTYPE>*>*>();
				vector<Interfaces::Phase_Interface<typename ThisType::Master_Type::FULL_PHASE_TYPE,NULLTYPE>*>::iterator itr = phases->begin();

				// Sum total lost time and critical vs for all phases
				float lost_time = 0;
				float vs_sum = 0;
				float vs_i=0;
				for (itr; itr != phases->end(); itr++)
				{
					//(*itr)->Update_Demand<Data_Structures::Time_Second>(iteration);
					lost_time += (*itr)->yellow_and_all_red_time<TargetType>();
					vs_i= (*itr)->Find_Critical_VS_Ratio<float>()*(*itr)->weight<float>();
					vs_sum += vs_i;
					if (out != NULL) (*out) << "\t"<<vs_i;
				}

				// Get estimated cycle length
				TargetType cycle;
				if (this->degree_of_saturation<float>() - vs_sum <= 0.0f) cycle = this->max_cycle_length<TargetType>();
				else cycle = (TargetType)((float)lost_time) * (this->degree_of_saturation<float>()) / (this->degree_of_saturation<float>() - vs_sum);
				if (cycle < this->min_cycle_length<TargetType>()) cycle = this->min_cycle_length<TargetType>();
				this->cycle_length<TargetType>(cycle);
				TargetType effective_cycle = cycle - lost_time;
				

				// Set green time for each phase
				TargetType cycle_rounded = 0;
				for (itr=phases->begin(); itr != phases->end(); itr++)
				{
					float vs;
					if (vs_sum > 0)	vs = (*itr)->Find_Critical_VS_Ratio<float>()/vs_sum * (*itr)->weight<float>();
					else vs = 1.0f / (float)phases->size();

					float temp_green = max(5.0f,(float)effective_cycle * vs);
					float remain = temp_green - (float)((int)temp_green);
					if (remain >=0.5f) temp_green = (int)temp_green + 1;
					else temp_green = (int)temp_green;
					(*itr)->green_time<Data_Structures::Time_Second>((TargetType)temp_green);
					cycle_rounded = cycle_rounded + (TargetType)temp_green;
				}
				cycle_rounded = cycle_rounded + lost_time;
				this->cycle_length<Data_Structures::Time_Second>(cycle_rounded);

				// Finally, output the LOS information
				this->Calculate_Signal_LOS<char>();
				//this->Display_Timing<NULLTYPE>();
				//this->Display_Signal_LOS<NULLTYPE>();
			}
			/// HCM Retiming calculations for simple signals
			facet void Update_Timing(call_requirements(
				requires(TargetType, Concepts::Is_Time_Seconds) && 
				requires(typename ThisType, Concepts::Is_HCM_Simple_Solution)))
			{
				// Simplify ThisType name
				typedef ThisType T;

				// Get reference to the phases in the signal phase diagram
				vector<Interfaces::Phase_Interface<typename ThisType::Master_Type::SIMPLE_PHASE_TYPE,NULLTYPE>*>* phases = this->Phases<vector<Interfaces::Phase_Interface<typename ThisType::Master_Type::SIMPLE_PHASE_TYPE,NULLTYPE>*>*>();
				vector<Interfaces::Phase_Interface<typename ThisType::Master_Type::SIMPLE_PHASE_TYPE,NULLTYPE>*>::iterator itr = phases->begin();

				ostream* out = this->output_stream<ostream*>();

				// Sum total lost time and critical vs for all phases
				float lost_time = 0;
				float critical_sum = 0;
				float vol;
				for (itr; itr != phases->end(); itr++)
				{
					(*itr)->Update_Demand<Data_Structures::Time_Second>(iteration);

					lost_time += (*itr)->yellow_and_all_red_time<Data_Structures::Time_Second>();
					vol = (*itr)->Find_Critical_Phase_Volume<Data_Structures::Flow_Per_Hour>();
					if (out != NULL) (*out) << "\t"<<vol;
					critical_sum += vol;
				}

				//---------------------------------------------------------------------------------------------
				// Get estimated cycle length	
				TargetType cycle;
				// area factor
				float f_a = 1.0;
				if (this->in_CBD<bool>()) f_a = 0.9;
				// calculate reference sum
				float reference_sum = 1710 * this->peak_hour_factor<float>() * f_a;
				// cycle length calculation
				cycle = (TargetType)((float)lost_time) / (1.0f - critical_sum / reference_sum);
				TargetType effective_cycle = cycle - lost_time;
				

				// Set green time for each phase
				TargetType cycle_rounded = 0;
				for (itr=phases->begin(); itr != phases->end(); itr++)
				{
					float vol = (*itr)->Find_Critical_Phase_Volume<Data_Structures::Flow_Per_Hour>();

					float temp_green = max(5.0f,(float)effective_cycle * vol / max(1.0f,critical_sum));
					float remain = temp_green - (float)((int)temp_green);
					if (remain >=0.5f) temp_green = (int)temp_green + 1;
					else temp_green = (int)temp_green;
					(*itr)->green_time<Data_Structures::Time_Second>((TargetType)temp_green);
					cycle_rounded = cycle_rounded + (TargetType)temp_green;
				}
				cycle_rounded = cycle_rounded + lost_time;
				this->cycle_length<Data_Structures::Time_Second>(cycle_rounded);

				// FIRST, output the LOS information
				Calculate_Signal_LOS<char>();
			}
			/// HCM Retiming calculation error handler
			facet void Update_Timing(call_requirements(!(
				requires(TargetType, Concepts::Is_Time_Seconds) && 
				(requires(ThisType, Concepts::Is_HCM_Full_Solution) || requires(ThisType, Concepts::Is_HCM_Simple_Solution)))))
			{
				assert_requirements(TargetType, Concepts::Is_Time_Seconds, "Your TargetType is not a measure of time in seconds.");
				assert_requirements(ThisType, Concepts::Is_HCM_Full_Solution, "Your SignalType does not specify a full HCM solution, and");
				assert_requirements(ThisType, Concepts::Is_HCM_Full_Solution, "Your SignalType does not specify a simple HCM solution");
				assert_requirements_2(ThisType::Phase_Interface&, Interfaces::Phase_Interface&, is_convertible, "Your ThisType::Phase_Interface specifies can not be cast to a Interface::Phase_Interface.");
			}
			/// HCM LOS Calculation
			facet void Calculate_Signal_LOS(call_requirements(requires(TargetType,is_integral)))
			{
				// Simplify ThisType name
				typedef ThisType T;

				// Get reference to the phases in the signal phase diagram
				vector<Interfaces::Approach_Interface<typename ThisType::Master_Type::APPROACH_TYPE,NULLTYPE>*>* approaches = this->Approaches<vector<Interfaces::Approach_Interface<typename ThisType::Master_Type::APPROACH_TYPE,NULLTYPE>*>*>();
				vector<Interfaces::Approach_Interface<typename ThisType::Master_Type::APPROACH_TYPE,NULLTYPE>*>::iterator itr = approaches->begin();

				// Sum total lost time and critical vs for all approaches
				float sum_delay = 0;
				float sum_flow = 0;
				for (itr; itr != approaches->end(); itr++)
				{
					(*itr)->Calculate_Approach_LOS<char>();

					sum_delay += (*itr)->delay<Data_Structures::Time_Second>() * (*itr)->approach_flow_rate<Data_Structures::Flow_Per_Hour>();
					sum_flow += (*itr)->approach_flow_rate<Data_Structures::Flow_Per_Hour>();
				}

				// convert delay to LOS
				float delay = sum_delay / sum_flow;
				this->delay<Data_Structures::Time_Second>(delay);
				if (delay < 10) this->LOS<char>('A');
				else if (delay < 20) this->LOS<char>('B');
				else if (delay < 35) this->LOS<char>('C');
				else if (delay < 55) this->LOS<char>('D');
				else if (delay < 80) this->LOS<char>('E');
				else this->LOS<char>('F');
			}


			//=======================================================
			// Display functionality
			//-------------------------------------------------------
			/// Output the cycle length and phasing plan information for signal	
			facet void Display_Timing()
			{
				// Simplify ThisType name
				typedef ThisType T;
				

				// Get reference to the phases in the signal phase diagram
				vector<Interfaces::Phase_Interface<typename ThisType::Master_Type::PHASE_TYPE,NULLTYPE>*>* phases = this->Phases<vector<Interfaces::Phase_Interface<typename ThisType::Master_Type::PHASE_TYPE,NULLTYPE>*>*>();
				vector<Interfaces::Phase_Interface<typename ThisType::Master_Type::PHASE_TYPE,NULLTYPE>*>::iterator itr = phases->begin();

				
				ostream* out = this->output_stream<ostream*>();	

				if (out == NULL) return;

				(*out) <<"Cycle Length:\t"<< this->cycle_length<Data_Structures::Time_Second>();
				
				int i=0; 
				for (itr; itr != phases->end(); itr++, i++)
				{
					(*out) <<"\t"<<"Phase "<<i+1<<":\t";
					(*out) <<"Green:\t"<<(*phases)[i]->green_time<Data_Structures::Time_Second>()<<"\t";
					(*out) <<"Yellow/red:\t"<<(*phases)[i]->yellow_and_all_red_time<Data_Structures::Time_Second>()<<"\t";
				}
				(*out)<<endl;
				//(*out).flush();
				
			}	
			/// Display the signal LOS to the output stream
			facet void Display_Signal_LOS()
			{
				// Simplify ThisType name
				typedef ThisType T;

				// Get reference to the phases in the signal phase diagram
				vector<Interfaces::Approach_Interface<typename ThisType::Master_Type::APPROACH_TYPE,NULLTYPE>*>* approaches = this->Approaches<vector<Interfaces::Approach_Interface<typename ThisType::Master_Type::APPROACH_TYPE,NULLTYPE>*>*>();
				vector<Interfaces::Approach_Interface<typename ThisType::Master_Type::APPROACH_TYPE,NULLTYPE>*>::iterator itr = approaches->begin();
				Interfaces::Approach_Interface<typename ThisType::Master_Type::APPROACH_TYPE,NULLTYPE>* approach_ptr;

				// Sum total lost time and critical vs for all approaches
				cout<<endl<<this->name<char*>()<<" LOS = "<<this->LOS<char>();
				for (itr; itr != approaches->end(); itr++)
				{
					approach_ptr = (*itr);
					cout<<endl<<approach_ptr->name<char*>()<<" Approach LOS = "<<approach_ptr->LOS<char>();
				}
			}


			//=======================================================
			// Signal Events
			//-------------------------------------------------------
			declare_facet_conditional(Signal_Check_Conditional)
			{
				// Get Current Interface
				typedef ThisType T;
				Signal_Interface<ThisType,NULLTYPE>* _this=(Signal_Interface<ThisType,NULLTYPE>*)pthis;
			
				// Get interface to phases in signal
				vector<Interfaces::Phase_Interface<typename ThisType::Master_Type::PHASE_TYPE,NULLTYPE>*>* phases = _this->Phases<vector<Interfaces::Phase_Interface<typename ThisType::Master_Type::PHASE_TYPE,NULLTYPE>*>*>();
				Interfaces::Phase_Interface<typename ThisType::Master_Type::PHASE_TYPE,NULLTYPE>* phase;

				// Get the currently active phase
				int active_phase = _this->active_phase<int>();
				phase = (*phases)[active_phase];


				//=======================================================================================================================
				// If timing updating is turned off (num_cycles = 0) then set the time of the next timing update to infinity so it is never called
				//-----------------------------------------------------------------------------------------------------------------------
				if (_this->num_cycles_between_updates<int>() == 0) _this->Next_Timing_Event_Iteration<int>(INT_MAX);


				//=======================================================================================================================
				// Handle the timing update sequence - occurs once every analysis period (calculated as a multiple of the current cycle length
				//-----------------------------------------------------------------------------------------------------------------------
				if (iteration == _this->Next_Timing_Event_Iteration<int>() && _this->Timing_Event_Conditional_Hit<bool>()==false)
				{

					((Execution_Object*)pthis)->Swap_Event((Event)&Signal_Interface<ThisType,NULLTYPE>::Change_Signal_Timing<NULLTYPE>);
					response.result = false;
					response.next = iteration;
					_this->Timing_Event_Conditional_Hit<bool>(true);
					return;
				}
				// Check to see if a signal update event is needed
				else if (iteration == _this->Next_Timing_Event_Iteration<int>() && _this->Timing_Event_Conditional_Hit<bool>()==true && _this->Timing_Event_Has_Fired<bool>()==false)
				{
					response.result = true;
					response.next = iteration;
					return;
				}

				else if (iteration == _this->Next_Timing_Event_Iteration<int>() && _this->Timing_Event_Has_Fired<bool>()==true)
				{
					((Execution_Object*)pthis)->Swap_Event((Event)&Signal_Interface<ThisType,NULLTYPE>::Change_Signal_State<NULLTYPE>);
					response.result = false;
					response.next = iteration;
					int next = iteration + (int)_this->cycle_length<Data_Structures::Time_Second>() * _this->num_cycles_between_updates<int>();
					_this->Next_Timing_Event_Iteration<int>(next);
					return;
				}


				//=======================================================================================================================
				// Handle the regular signal state updating sequence (happens every time the conditional is called)
				//-----------------------------------------------------------------------------------------------------------------------
				if (_this->Event_Has_Fired<bool>() == true)
				{
					response.result = false;
					response.next = _this->Next_Event_Iteration<int>();
					_this->Event_Has_Fired<bool>(false);
					_this->Event_Conditional_Hit<bool>(false);
					_this->Timing_Event_Conditional_Hit<bool>(false);
					_this->Timing_Event_Has_Fired<bool>(false);
				}
				/// INITIALIZE THE ITERATION AND GO INTO PROCESSING
				else
				{
					response.result=true;
					response.next=iteration;
					//_this->Next_Event_Iteration<int>(iteration + (int)phase->green_time<Data_Structures::Time_Second>());
					_this->Event_Has_Fired<bool>(false);

				}
				//else if (phase->signal_state<Data_Structures::Signal_State>() == Data_Structures::RED)
				//{
				//	response.result=true;
				//	response.next=iteration;
				//	//_this->Next_Event_Iteration<int>(iteration + (int)phase->green_time<Data_Structures::Time_Second>());
				//	_this->Event_Has_Fired<bool>(false);
				//}
				//else if (phase->signal_state<Data_Structures::Signal_State>() == Data_Structures::GREEN)
				//{
				//	response.result=true;
				//	response.next=iteration; // increment the iteration by one to turn of the event has fired indicator
				//	//_this->Next_Event_Iteration<int>(iteration + (int)phase->yellow_and_all_red_time<Data_Structures::Time_Second>());
				//	_this->Event_Has_Fired<bool>(false);
				//}
				//else
				//{
				//	assert("Error, should not get here");
				//}
			}
			declare_facet_event(Change_Signal_State)
			{
				// Get Current Interface
				typedef ThisType T;
				Signal_Interface<ThisType,NULLTYPE>* _this = (Signal_Interface<ThisType,NULLTYPE>*)pthis;

				// Display the signal information if the output stream is not null
				ostream* out = _this->output_stream<ostream*>();

				// Get interface to phases in signal
				vector<Interfaces::Phase_Interface<typename ThisType::Master_Type::PHASE_TYPE,NULLTYPE>*>* phases = _this->Phases<vector<Interfaces::Phase_Interface<typename ThisType::Master_Type::PHASE_TYPE,NULLTYPE>*>*>();
				vector<Interfaces::Phase_Interface<typename ThisType::Master_Type::PHASE_TYPE,NULLTYPE>*>::iterator phase_itr = phases->begin();
				Interfaces::Phase_Interface<typename ThisType::Master_Type::PHASE_TYPE,NULLTYPE>* phase;

				// Get the currently active phase
				int active_phase = _this->active_phase<int>();

				phase = (*phases)[active_phase];

				// If state of active phase is RED (i.e. All Red) Change current phase to Green (Each phases starts in all red)
				if (phase->signal_state<Data_Structures::Signal_State>() == Data_Structures::RED)
				{
					phase->signal_state<Data_Structures::Signal_State>(Data_Structures::GREEN);
					_this->Next_Event_Iteration<int>(iteration + (int)phase->green_time<Data_Structures::Time_Second>());
				}

				// Else If state of active phase is GREEN change current phase to YELLOW
				else if (phase->signal_state<Data_Structures::Signal_State>() == Data_Structures::GREEN)
				{
					phase->signal_state<Data_Structures::Signal_State>(Data_Structures::YELLOW);
					_this->Next_Event_Iteration<int>(iteration + (int)phase->yellow_and_all_red_time<Data_Structures::Time_Second>()-2);
				}

				// Else if active phase is YELLOW, change state to RED and transition to next phase (Start of next all red period)
				else
				{
					phase->signal_state<Data_Structures::Signal_State>(Data_Structures::RED);
					_this->Next_Event_Iteration<int>(iteration + 2);

					// Move to next phase
					active_phase++;
					if (active_phase >= phases->size()) active_phase=0;
					_this->active_phase<int>(active_phase);
				}

				// print out end of previous cycle message
				if (active_phase == 0 && phase->signal_state<Data_Structures::Signal_State>() == Data_Structures::GREEN)
				{
					int start_time = iteration - _this->cycle_length<Data_Structures::Time_Second>();
					int cur_time = start_time;
					
					for (phase_itr; phase_itr != phases->end(); phase_itr++)
					{
						cout << "cycle_end:"<<(*phase_itr)->phase_id<int>()<<":"<<_this->Signal_ID<int>()<<":"<<start_time<<":"<<cur_time<<":"<<cur_time+(*phase_itr)->green_time<Data_Structures::Time_Second>()<<":"<<iteration<<endl;
						cur_time = cur_time+(*phase_itr)->green_time<Data_Structures::Time_Second>() + (*phase_itr)->yellow_and_all_red_time<Data_Structures::Time_Second>();
					}
				}

				// Set has fired to true, indicates signal events done for current iteration
				_this->Event_Has_Fired<bool>(true);
			}
			declare_facet_event(Change_Signal_Timing)
			{
				// Get Current Interface
				typedef ThisType T;
				Signal_Interface<ThisType,NULLTYPE>* _this = (Signal_Interface<ThisType,NULLTYPE>*)pthis;

				// Call the routine to update the signal timing
				_this->Update_Timing<Data_Structures::Time_Second>();

				// Indicate that the routine has been performed so that control can pass to the 'Change_Signal_State' routine
				_this->Timing_Event_Has_Fired<bool>(true);


				// Display the signal information if the output stream is not null
				ostream* out = _this->output_stream<ostream*>();
				if (out != NULL)
				{
					//(*out) <<endl<<endl<<"===================================================================";
					//(*out) <<endl<<" CHANGE SIGNAL TIMING CALLED ";
					//(*out) <<endl<<"==================================================================="<<endl<<endl;
					//(*out) <<"\t"<< iteration<<"\t";
					_this->Display_Timing<NULLTYPE>();
					//(*out) <<endl<<"==================================================================="<<endl<<endl;
				}
			}
			//--------------------------------------------
			// Signal Event timing and iteration accessors
			//--------------------------------------------
			facet_accessor(Next_Event_Iteration);
			facet_accessor(Next_Timing_Event_Iteration);
			facet_accessor(Event_Has_Fired);
			facet_accessor(Event_Conditional_Hit);
			facet_accessor(Timing_Event_Has_Fired);
			facet_accessor(Timing_Event_Conditional_Hit);


			//=======================================================
			// Accessor facets
			//-------------------------------------------------------
			/// Stream to write messages to
			facet_accessor(output_stream);
			/// Number of cycles until signal update is called
			facet_accessor(num_cycles_between_updates);  
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
			/// Signal name accessor
			facet_accessor(name);
			// phf for the signal as a whole
			facet_accessor(peak_hour_factor);
			// Currently active (green) signal phase
			facet_accessor(active_phase);
			// Current signal delay per vehicle
			facet_accessor(delay);
			// Current signal LOS
			facet_accessor(LOS);

			//========================================================================
			// COLLECTIONS
			//------------------------------------------------------------------------
			/// Local facet to return the list of phases associated with signal
			facet_accessor(Phases);
			/// Local facet to return the list of phases associated with signal
			facet_accessor(Approaches);
		};


		//------------------------------------------------------------------------------------------------------------------
		/// Signal Indicator Component
		//------------------------------------------------------------------------------------------------------------------
		template <typename ThisType, typename CallerType>
		struct Signal_Indicator_Interface
		{
			// Initialize the signal indicator
			facet void Initialize(call_requirements(requires(ThisType,Is_Execution_Object)))
			{
				PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TargetType>();
				schedule_event_local(ThisType,Signal_Indicator_Conditional,Signal_Indicator_Event,0,NULLTYPE);
				this->Conditional_Has_Fired<bool>(false);
			}
			facet void Initialize(call_requirements(requires(ThisType,Is_Data_Object)))
			{
				PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TargetType>();
			}
			facet void Initialize(call_requirements(requires(ThisType,!Is_Data_Object) && requires(ThisType,!Is_Execution_Object)))
			{
				assert_requirements(ThisType,Is_Data_Object,"ThisType is not a data object or an execution object.");
			}

			// Initialize the signal indicator and provide its linked signal
			facet void Initialize(typename TargetType::Interface_Type<TargetType,CallerType>::type* signal_interface, call_requirements(requires(ThisType,Is_Execution_Object) && requires(TargetType, Is_Polaris_Component)))
			{
				PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TargetType>();
				schedule_event_local(ThisType,Signal_Indicator_Conditional,Signal_Indicator_Event,0,NULLTYPE);
				this->Conditional_Has_Fired<bool>(false);
				this->Signal<TargetType>(signal_interface);
			}
			facet void Initialize(typename TargetType::Interface_Type<TargetType,CallerType>::type* signal_interface, call_requirements(requires(ThisType,Is_Data_Object) && requires(TargetType, Is_Polaris_Component)))
			{
				PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TargetType>();
				this->Signal<TargetType>(signal_interface);
			}
			facet void Initialize(typename TargetType::Interface_Type<TargetType,CallerType>::type* signal_interface, call_requirements((requires(ThisType,!Is_Data_Object) && requires(ThisType,!Is_Execution_Object)) || requires(TargetType, !Is_Polaris_Component)))
			{
				assert_requirements(ThisType,Is_Data_Object,"ThisType is not a data object or an execution object.");
				assert_requirements(TargetType,Is_Polaris_Component,"TargetType is not a polaris component.");
			}

			// Signal Interface accessor
			facet_accessor_interface(Signal);
			facet_accessor(Conditional_Has_Fired);
			facet_accessor(output_stream);

			// Event definition
			declare_facet_conditional(Signal_Indicator_Conditional)
			{
				// Get Current Interface
				Signal_Indicator_Interface<ThisType,NULLTYPE>* _this=(Signal_Indicator_Interface<ThisType,NULLTYPE>*)pthis;

				// Get Signal Interface from this
				Signal_Components::Interfaces::Signal_Interface<typename ThisType::Master_Type::SIGNAL_TYPE,NULLTYPE>* signal = _this->Signal<typename ThisType::Master_Type::SIGNAL_TYPE>();

				if (iteration == signal->Next_Event_Iteration<int>())
				{
					response.result = false;
					response.next = iteration;
				}
				else
				{
					response.result = true;
					response.next = iteration + 1;
				}
			}
			declare_facet_event(Signal_Indicator_Event)
			{
				// Sleep Now
				Sleep(50);

				// Get Current Interface
				Signal_Indicator_Interface<ThisType,NULLTYPE>* _this=(Signal_Indicator_Interface<ThisType,NULLTYPE>*)pthis;
				ostream* out = _this->output_stream<ostream*>();

				// Get Signal Interface from this
				Signal_Components::Interfaces::Signal_Interface<typename ThisType::Master_Type::SIGNAL_TYPE,NULLTYPE>* signal = _this->Signal<typename ThisType::Master_Type::SIGNAL_TYPE>();

				// Get reference to the phases in the signal phase diagram
				vector<Interfaces::Phase_Interface<typename ThisType::Master_Type::PHASE_TYPE,NULLTYPE>*>* phases = signal->Phases<vector<Interfaces::Phase_Interface<typename ThisType::Master_Type::PHASE_TYPE,NULLTYPE>*>*>();
				vector<Interfaces::Phase_Interface<typename ThisType::Master_Type::PHASE_TYPE,NULLTYPE>*>::iterator itr = phases->begin();

				// Display the signal state info if the output stream is not null
				if (out != NULL)
				{
					//(*out) <<endl<<"ITERATION: " <<iteration <<", "<< signal->name<char*>();
					int i;
					for (itr, i=0; itr != phases->end(); itr++, i++)
					{
						(*out) << "signal_state:"<<(*itr)->phase_id<int>()<<":"<<signal->Signal_ID<int>()<<":";
						int signal_status = 1;
						if ((*itr)->signal_state<Data_Structures::Signal_State>() == Data_Structures::GREEN) signal_status = 3;
						else if ((*itr)->signal_state<Data_Structures::Signal_State>() == Data_Structures::YELLOW) signal_status = 2;
						(*out) <<signal_status<<":"<<iteration<<endl;
						(*out).flush();
						
						//char* signal_status = "RED   ";
						//if ((*itr)->signal_state<Data_Structures::Signal_State>() == Data_Structures::GREEN) signal_status = "GREEN ";
						//else if ((*itr)->signal_state<Data_Structures::Signal_State>() == Data_Structures::YELLOW) signal_status = "YELLOW";
						//(*out) <<", Phase "<<(*itr)->name<char*>()<<": "<<signal_status;
					}
				}
			}
		};


		//------------------------------------------------------------------------------------------------------------------
		/// Detector Interface
		//------------------------------------------------------------------------------------------------------------------
		template <typename ThisType, typename CallerType>
		struct Detector_Interface
		{
			// Getter for the detector count data
			facet typename TargetType::ReturnType count(typename TargetType::ParamType time, call_requirements(
				requires(ThisType, Is_Dispatchable) &&
				requires(TargetType, Is_Target_Type_Struct) &&
				requires(typename TargetType::ParamType, Concepts::Is_Time)))
			{
				return PTHIS(ThisType)->count<Dispatch<ThisType>, CallerType, TargetType>(time);
			}
			// Getter for the detector count data
			facet typename TargetType::ReturnType count(typename TargetType::ParamType time, call_requirements(!(
				requires(ThisType, Is_Dispatchable) &&
				requires(TargetType,Is_Target_Type_Struct) &&
				requires(typename TargetType::ParamType, Concepts::Is_Time))))
			{
				assert_requirements(TargetType, Concepts::Is_Target_Type_Struct, "TargetType is not an Target_Type struct");
				assert_requirements(typename TargetType::ParamType, Concepts::Is_Time, "TargetType::ParamType is not a time data structure");
			}

			// Updater for the detector count data - call whenever a vehicle crosses detector (or use for aggregate updates by passing a vehicle count value in)
			facet void detect_vehicles(TargetType vehicle_detection_count)
			{
				PTHIS(ThisType)->detect_vehicle<Dispatch<ThisType>,CallerType,TargetType>(vehicle_detection_count);
			}
			facet void detect_vehicle()
			{
				PTHIS(ThisType)->detect_vehicle<Dispatch<ThisType>,CallerType,TargetType>(1);
			}
			facet TargetType Get_Count()
			{
				return PTHIS(ThisType)->Get_Count<Dispatch<ThisType>,CallerType,TargetType>();
			}
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

		// Enumerations
		enum Left_Turn_Types { Protected, Permitted, Unopposed, None };
		enum Turn_Lane_Types { Exclusive, Shared, No_Turn };
		enum Signal_State { GREEN, YELLOW, RED };

	}

}
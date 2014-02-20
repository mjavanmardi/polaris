
#pragma once

#include "Core.h"


namespace PopSyn
{

	namespace Concepts
	{
		concept struct Uses_Linker_File
		{
			check_data_member_name(Has_Use_Linker_File_Defined, _linker_file_path);
			define_default_check(Has_Use_Linker_File_Defined);
		};

		concept struct Scenario_Has_Popsyn_Configuration_Data
		{
			check_template_method_name(has_percent_to_synthesize_p, Component_Type::percent_to_synthesize);
			check_template_method_name(has_ipf_tolerance_p, Component_Type::ipf_tolerance);
			check_template_method_name(has_marginal_tolerance_p, Component_Type::marginal_tolerance);
			check_template_method_name(has_maximum_iterations_p, Component_Type::maximum_iterations);
			check_template_method_name(has_write_marginal_output_p, Component_Type::write_marginal_output);
			check_template_method_name(has_write_full_output_p, Component_Type::write_full_output);
			check_template_method_name(has_popsyn_control_file_name_p, Component_Type::popsyn_control_file_name);

			check_template_method_name(has_percent_to_synthesize, percent_to_synthesize);
			check_template_method_name(has_ipf_tolerance, ipf_tolerance);
			check_template_method_name(has_marginal_tolerance, marginal_tolerance);
			check_template_method_name(has_maximum_iterations, maximum_iterations);
			check_template_method_name(has_write_marginal_output,write_marginal_output);
			check_template_method_name(has_write_full_output,write_full_output);
			check_template_method_name(has_popsyn_control_file_name,popsyn_control_file_name);
			define_sub_check(Has_Popsyn_Configuration_Data_Prototype, has_percent_to_synthesize_p && has_ipf_tolerance_p && has_marginal_tolerance_p && has_maximum_iterations_p && has_write_marginal_output_p && has_write_full_output_p && has_popsyn_control_file_name_p);
			define_sub_check(Has_Popsyn_Configuration_Data_Component, has_percent_to_synthesize && has_ipf_tolerance && has_marginal_tolerance && has_maximum_iterations && has_write_marginal_output && has_write_full_output && has_popsyn_control_file_name);
			define_default_check(Has_Popsyn_Configuration_Data_Prototype || Has_Popsyn_Configuration_Data_Component);
		};

		concept struct Is_Synthesis_Zone
		{
			check_template_method_name(Has_Target_Joint_Distribution_P,Component_Type::Target_Joint_Distribution);
			check_template_method_name(Has_Target_Marginal_Distribution_P,Component_Type::Target_Marginal_Distribution);
			check_template_method_name(Has_Synthetic_Households_Container_P,Component_Type::Synthetic_Households_Container);
			check_template_method_name(Has_Activity_Locations_Container_P,Component_Type::Activity_Locations_Container);
			check_template_method_name(Has_scenario_reference_P, Component_Type::scenario_reference);
			check_template_method_name(Has_network_reference_P, Component_Type::network_reference);

			define_sub_check(Is_Synthesis_Usable_Prototype, Has_Target_Joint_Distribution_P && Has_Target_Marginal_Distribution_P && Has_scenario_reference_P);
			define_sub_check(Is_Simulator_Usable_Prototype, Has_Synthetic_Households_Container_P && Has_network_reference_P && Has_Activity_Locations_Container_P);
			define_sub_check(Is_Synthesis_Zone_Prototype, Is_Synthesis_Usable_Prototype && Is_Simulator_Usable_Prototype);

			check_template_method_name(Has_Target_Joint_Distribution,Target_Joint_Distribution);
			check_template_method_name(Has_Target_Marginal_Distribution,Target_Marginal_Distribution);
			check_template_method_name(Has_Synthetic_Households_Container,Synthetic_Households_Container);
			check_template_method_name(Has_Activity_Locations_Container,Activity_Locations_Container);
			check_template_method_name(Has_scenario_reference, scenario_reference);
			check_template_method_name(Has_network_reference, network_reference);

			define_sub_check(Is_Synthesis_Usable, Has_Target_Joint_Distribution && Has_Target_Marginal_Distribution && Has_scenario_reference);
			define_sub_check(Is_Simulator_Usable, Has_network_reference && Has_Synthetic_Households_Container && Has_Activity_Locations_Container);
			define_sub_check(Is_Synthesis_Zone_Component, Is_Synthesis_Usable && Is_Simulator_Usable);

			define_default_check(Is_Synthesis_Zone_Prototype || Is_Synthesis_Zone_Component);
		};

		concept struct Is_IPF_Capable
		{
			check_typedef_name(Has_Value_Type, Value_Type);
			check_typedef_type(Has_Marginals, Has_Marginals_In_Distribution, true_type);
			define_default_check(Has_Marginals && Has_Value_Type);
		};

		concept struct Is_Probabilistic_Selection
		{
			check_typedef_name(Has_Value_Type, Value_Type);
			check_typedef_type(Has_Probabilistic_Selection_Defined, Probabilistic_Selection_Type, true_type);
			define_default_check(Has_Probabilistic_Selection_Defined && Has_Value_Type);
		};

		concept struct Is_Loss_Function_Selection
		{
			check_typedef_name(Has_Value_Type, Value_Type);
			check_typedef_type(Has_Loss_Function_Selection_Defined, Loss_Function_Selection_Type, true_type);
			define_default_check(Has_Loss_Function_Selection_Defined && Has_Value_Type);
		};

		concept struct Is_IPF_Solver_Setting
		{
			check_data_member_type(Has_IPF_Tolerance_d, IPF_Tolerance, double);
			check_data_member_type(Has_IPF_Tolerance_f, IPF_Tolerance, float);
			check_data_member_type(Has_Max_Iterations_int, Max_Iterations, int);
			check_data_member_type(Has_Max_Iterations_uint, Max_Iterations, unsigned int);
			check_data_member_type(Has_Max_Iterations_long, Max_Iterations, long);
			check_data_member_type(Has_Max_Iterations_ulong, Max_Iterations, long);

			define_default_check((Has_IPF_Tolerance_d || Has_IPF_Tolerance_f) && (Has_Max_Iterations_int || Has_Max_Iterations_uint || Has_Max_Iterations_long || Has_Max_Iterations_ulong));
		};
	}


}
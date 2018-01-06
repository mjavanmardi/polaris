#pragma once

#include "Mode_Chooser_Prototype.h"
#include "Person_Prototype.h"
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/error/en.h>
#include <fstream>

namespace Person_Components
{
	namespace Implementations
	{
		//=============================================================================================
		// NEW NESTED LOGIT IMPLEMENTATION 
		//---------------------------------------------------------------------------------------------
		implementation struct ADAPTS_Mode_Choice_Option : public Choice_Model_Components::Implementations::Nested_Choice_Option_Base<MasterType, INHERIT(ADAPTS_Mode_Choice_Option)>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType, INHERIT(ADAPTS_Mode_Choice_Option), Data_Object>::Component_Type ComponentType;

			static bool static_initialize(const string& option_file)
			{
				// set the base values
				default_static_initializer();

				// now see if there are config file changes
				Scenario_Components::Types::ScenarioData document;
				if (option_file.length() < 1)
				{
					cout << "Warning: option file for Mode_Choice_Option was not specified" << endl;
					return true;
				}

				_Scenario_Interface* scenario = static_cast<_Scenario_Interface*>(_global_scenario);
				if (!scenario->parse_option_file(document, option_file))
					return false;

				scenario->set_parameter<float>(document, "HBW_ASC_AUTO"		, _HBW_ASC_AUTO			);
				scenario->set_parameter<float>(document, "HBO_ASC_AUTO"		, _HBO_ASC_AUTO			);
				scenario->set_parameter<float>(document, "NHB_ASC_AUTO"		, _NHB_ASC_AUTO			);
				scenario->set_parameter<float>(document, "HBW_ASC_PASS"		, _HBW_ASC_PASS			);
				scenario->set_parameter<float>(document, "HBO_ASC_PASS"		, _HBO_ASC_PASS			);
				scenario->set_parameter<float>(document, "NHB_ASC_PASS"		, _NHB_ASC_PASS			);
				scenario->set_parameter<float>(document, "HBW_ASC_TAXI"		, _HBW_ASC_TAXI			);
				scenario->set_parameter<float>(document, "HBO_ASC_TAXI"		, _HBO_ASC_TAXI			);
				scenario->set_parameter<float>(document, "NHB_ASC_TAXI"		, _NHB_ASC_TAXI			);
				scenario->set_parameter<float>(document, "HBW_ASC_WALK"		, _HBW_ASC_WALK			);
				scenario->set_parameter<float>(document, "HBO_ASC_WALK"		, _HBO_ASC_WALK			);
				scenario->set_parameter<float>(document, "NHB_ASC_WALK"		, _NHB_ASC_WALK			);
				scenario->set_parameter<float>(document, "HBW_ASC_TRAN"		, _HBW_ASC_TRAN			);
				scenario->set_parameter<float>(document, "HBO_ASC_TRAN"		, _HBO_ASC_TRAN			);
				scenario->set_parameter<float>(document, "NHB_ASC_TRAN"		, _NHB_ASC_TRAN			);
				scenario->set_parameter<float>(document, "HBW_ASC_BIKE"		, _HBW_ASC_BIKE			);
				scenario->set_parameter<float>(document, "HBO_ASC_BIKE"		, _HBO_ASC_BIKE			);
				scenario->set_parameter<float>(document, "NHB_ASC_BIKE"		, _NHB_ASC_BIKE			);
				scenario->set_parameter<float>(document, "HBW_B_cbd_pa"		, _HBW_B_cbd_pa			);
				scenario->set_parameter<float>(document, "HBO_B_cbd_pa"		, _HBO_B_cbd_pa			);
				scenario->set_parameter<float>(document, "NHB_B_cbd_pa"		, _NHB_B_cbd_pa			);
				scenario->set_parameter<float>(document, "HBW_B_cost"			, _HBW_B_cost			);
				scenario->set_parameter<float>(document, "HBO_B_cost"			, _HBO_B_cost			);
				scenario->set_parameter<float>(document, "NHB_B_cost"			, _NHB_B_cost			);
				scenario->set_parameter<float>(document, "HBW_B_cost_hinc"	, _HBW_B_cost_hinc		);
				scenario->set_parameter<float>(document, "HBO_B_cost_hinc"	, _HBO_B_cost_hinc		);
				scenario->set_parameter<float>(document, "NHB_B_cost_hinc"	, _NHB_B_cost_hinc		);
				scenario->set_parameter<float>(document, "HBW_B_cost_minc"	, _HBW_B_cost_minc		);
				scenario->set_parameter<float>(document, "HBO_B_cost_minc"	, _HBO_B_cost_minc		);
				scenario->set_parameter<float>(document, "NHB_B_cost_minc"	, _NHB_B_cost_minc		);
				scenario->set_parameter<float>(document, "HBW_B_dens_bike"	, _HBW_B_dens_bike		);
				scenario->set_parameter<float>(document, "HBO_B_dens_bike"	, _HBO_B_dens_bike		);
				scenario->set_parameter<float>(document, "NHB_B_dens_bike"	, _NHB_B_dens_bike		);
				scenario->set_parameter<float>(document, "HBW_B_dens_walk"	, _HBW_B_dens_walk		);
				scenario->set_parameter<float>(document, "HBO_B_dens_walk"	, _HBO_B_dens_walk		);
				scenario->set_parameter<float>(document, "NHB_B_dens_walk"	, _NHB_B_dens_walk		);
				scenario->set_parameter<float>(document, "HBW_B_male_taxi"	, _HBW_B_male_taxi		);
				scenario->set_parameter<float>(document, "HBO_B_male_taxi"	, _HBO_B_male_taxi		);
				scenario->set_parameter<float>(document, "NHB_B_male_taxi"	, _NHB_B_male_taxi		);
				scenario->set_parameter<float>(document, "HBW_B_notalone_pass", _HBW_B_notalone_pass	);
				scenario->set_parameter<float>(document, "HBO_B_notalone_pass", _HBO_B_notalone_pass	);
				scenario->set_parameter<float>(document, "NHB_B_notalone_pass", _NHB_B_notalone_pass	);
				scenario->set_parameter<float>(document, "HBW_B_over65_pass"	, _HBW_B_over65_pass	);
				scenario->set_parameter<float>(document, "HBO_B_over65_pass"	, _HBO_B_over65_pass	);
				scenario->set_parameter<float>(document, "NHB_B_over65_pass"	, _NHB_B_over65_pass 	);
				scenario->set_parameter<float>(document, "HBW_B_over65_tran"	, _HBW_B_over65_tran	);
				scenario->set_parameter<float>(document, "HBO_B_over65_tran"	, _HBO_B_over65_tran	);
				scenario->set_parameter<float>(document, "NHB_B_over65_tran"	, _NHB_B_over65_tran 	);
				scenario->set_parameter<float>(document, "HBW_B_ovttime_tran"	, _HBW_B_ovttime_tran	);
				scenario->set_parameter<float>(document, "HBO_B_ovttime_tran"	, _HBO_B_ovttime_tran	);
				scenario->set_parameter<float>(document, "NHB_B_ovttime_tran"	, _NHB_B_ovttime_tran	);
				scenario->set_parameter<float>(document, "HBW_B_peak_auto"	, _HBW_B_peak_auto   	);
				scenario->set_parameter<float>(document, "HBO_B_peak_auto"	, _HBO_B_peak_auto   	);
				scenario->set_parameter<float>(document, "NHB_B_peak_auto"	, _NHB_B_peak_auto   	);
				scenario->set_parameter<float>(document, "HBW_B_ttime_bike"	, _HBW_B_ttime_bike		);
				scenario->set_parameter<float>(document, "HBO_B_ttime_bike"	, _HBO_B_ttime_bike		);
				scenario->set_parameter<float>(document, "NHB_B_ttime_bike"	, _NHB_B_ttime_bike  	);
				scenario->set_parameter<float>(document, "HBW_B_ttime_tran"	, _HBW_B_ttime_tran		);
				scenario->set_parameter<float>(document, "HBO_B_ttime_tran"	, _HBO_B_ttime_tran		);
				scenario->set_parameter<float>(document, "NHB_B_ttime_tran"	, _NHB_B_ttime_tran  	);
				scenario->set_parameter<float>(document, "HBW_B_ttime_walk"	, _HBW_B_ttime_walk		);
				scenario->set_parameter<float>(document, "HBO_B_ttime_walk"	, _HBO_B_ttime_walk		);
				scenario->set_parameter<float>(document, "NHB_B_ttime_walk"	, _NHB_B_ttime_walk  	);
				scenario->set_parameter<float>(document, "HBW_B_u18_pass"		, _HBW_B_u18_pass		);
				scenario->set_parameter<float>(document, "HBO_B_u18_pass"		, _HBO_B_u18_pass		);
				scenario->set_parameter<float>(document, "NHB_B_u18_pass"		, _NHB_B_u18_pass    	);
				scenario->set_parameter<float>(document, "HBW_B_vehavail_pass", _HBW_B_vehavail_pass	);
				scenario->set_parameter<float>(document, "HBO_B_vehavail_pass", _HBO_B_vehavail_pass	);
				scenario->set_parameter<float>(document, "NHB_B_vehavail_pass", _NHB_B_vehavail_pass	);
				scenario->set_parameter<float>(document, "HBW_B_vehavail_taxi", _HBW_B_vehavail_taxi	);
				scenario->set_parameter<float>(document, "HBO_B_vehavail_taxi", _HBO_B_vehavail_taxi	);
				scenario->set_parameter<float>(document, "NHB_B_vehavail_taxi", _NHB_B_vehavail_taxi	);
				scenario->set_parameter<float>(document, "HBW_B_vehavail_tran", _HBW_B_vehavail_tran	);
				scenario->set_parameter<float>(document, "HBO_B_vehavail_tran", _HBO_B_vehavail_tran	);
				scenario->set_parameter<float>(document, "NHB_B_vehavail_tran", _NHB_B_vehavail_tran	);
				scenario->set_parameter<float>(document, "HBW_B_waittime_tran", _HBW_B_waittime_tran	);
				scenario->set_parameter<float>(document, "HBO_B_waittime_tran", _HBO_B_waittime_tran	);
				scenario->set_parameter<float>(document, "NHB_B_waittime_tran", _NHB_B_waittime_tran	);
				scenario->set_parameter<float>(document, "HBW_ASC_N_AUTO"		, _HBW_ASC_N_AUTO		);
				scenario->set_parameter<float>(document, "HBO_ASC_N_AUTO"		, _HBO_ASC_N_AUTO		);
				scenario->set_parameter<float>(document, "NHB_ASC_N_AUTO"		, _NHB_ASC_N_AUTO    	);
				scenario->set_parameter<float>(document, "HBW_ASC_N_NM"		, _HBW_ASC_N_NM			);
				scenario->set_parameter<float>(document, "HBO_ASC_N_NM"		, _HBO_ASC_N_NM			);
				scenario->set_parameter<float>(document, "NHB_ASC_N_NM"		, _NHB_ASC_N_NM      	);
				scenario->set_parameter<float>(document, "HBW_B_vehavail_nm"	, _HBW_B_vehavail_nm	);
				scenario->set_parameter<float>(document, "HBO_B_vehavail_nm"	, _HBO_B_vehavail_nm	);
				scenario->set_parameter<float>(document, "NHB_B_vehavail_nm"	, _NHB_B_vehavail_nm 	);
				scenario->set_parameter<float>(document, "HBW_B_male_nm"		, _HBW_B_male_nm		);
				scenario->set_parameter<float>(document, "HBO_B_male_nm"		, _HBO_B_male_nm		);
				scenario->set_parameter<float>(document, "NHB_B_male_nm"		, _NHB_B_male_nm     	);
				scenario->set_parameter<float>(document, "HBW_B_cbd_nm"		, _HBW_B_cbd_nm			);
				scenario->set_parameter<float>(document, "HBO_B_cbd_nm"		, _HBO_B_cbd_nm			);
				scenario->set_parameter<float>(document, "NHB_B_cbd_nm"		, _NHB_B_cbd_nm      	);
				scenario->set_parameter<float>(document, "HBW_NEST_AUTO"		, _HBW_NEST_AUTO		);
				scenario->set_parameter<float>(document, "HBO_NEST_AUTO"		, _HBO_NEST_AUTO		);
				scenario->set_parameter<float>(document, "NHB_NEST_AUTO"		, _NHB_NEST_AUTO     	);
				scenario->set_parameter<float>(document, "HBW_NEST_NM"		, _HBW_NEST_NM	    	);
				scenario->set_parameter<float>(document, "HBO_NEST_NM"		, _HBO_NEST_NM	    	);
				scenario->set_parameter<float>(document, "NHB_NEST_NM"		, _NHB_NEST_NM       	);

				return true;
			}

			static void default_static_initializer()
			{
				_HBW_ASC_AUTO = 0.0;
				_HBW_ASC_PASS = 0.0;
				_HBW_ASC_TAXI = 0.0;
				_HBW_ASC_WALK = 0.0;
				_HBW_ASC_TRAN = 0.0;
				_HBW_ASC_BIKE = 0.0;
				_HBW_B_cbd_pa = 0.0;
				_HBW_B_cost = 0.0;
				_HBW_B_cost_hinc = 0.0;
				_HBW_B_cost_minc = 0.0;
				_HBW_B_dens_bike = 0.0;
				_HBW_B_dens_walk = 0.0;
				_HBW_B_male_taxi = 0.0;
				_HBW_B_notalone_pass = 0.0;
				_HBW_B_over65_pass = 0.0;
				_HBW_B_over65_tran = 0.0;
				_HBW_B_ovttime_tran = 0.0;
				_HBW_B_peak_auto = 0.0;
				_HBW_B_ttime_bike = 0.0;
				_HBW_B_ttime_tran = 0.0;
				_HBW_B_ttime_walk = 0.0;
				_HBW_B_u18_pass = 0.0;
				_HBW_B_vehavail_pass = 0.0;
				_HBW_B_vehavail_taxi = 0.0;
				_HBW_B_vehavail_tran = 0.0;
				_HBW_B_waittime_tran = 0.0;
				_HBW_ASC_N_AUTO = 0.0;
				_HBW_ASC_N_NM = 0.0;
				_HBW_B_vehavail_nm = 0.0;
				_HBW_B_male_nm = 0.0;
				_HBW_B_cbd_nm = 0.0;
				_HBW_NEST_AUTO = 0.0;
				_HBW_NEST_NM = 0.0;

				_HBO_ASC_AUTO = 0.0;
				_HBO_ASC_PASS = 0.0;
				_HBO_ASC_TAXI = 0.0;
				_HBO_ASC_WALK = 0.0;
				_HBO_ASC_TRAN = 0.0;
				_HBO_ASC_BIKE = 0.0;
				_HBO_B_cbd_pa = 0.0;
				_HBO_B_cost = 0.0;
				_HBO_B_cost_hinc = 0.0;
				_HBO_B_cost_minc = 0.0;
				_HBO_B_dens_bike = 0.0;
				_HBO_B_dens_walk = 0.0;
				_HBO_B_male_taxi = 0.0;
				_HBO_B_notalone_pass = 0.0;
				_HBO_B_over65_pass = 0.0;
				_HBO_B_over65_tran = 0.0;
				_HBO_B_ovttime_tran = 0.0;
				_HBO_B_peak_auto = 0.0;
				_HBO_B_ttime_bike = 0.0;
				_HBO_B_ttime_tran = 0.0;
				_HBO_B_ttime_walk = 0.0;
				_HBO_B_u18_pass = 0.0;
				_HBO_B_vehavail_pass = 0.0;
				_HBO_B_vehavail_taxi = 0.0;
				_HBO_B_vehavail_tran = 0.0;
				_HBO_B_waittime_tran = 0.0;
				_HBO_ASC_N_AUTO = 0.0;
				_HBO_ASC_N_NM = 0.0;
				_HBO_B_vehavail_nm = 0.0;
				_HBO_B_male_nm = 0.0;
				_HBO_B_cbd_nm = 0.0;
				_HBO_NEST_AUTO = 0.0;
				_HBO_NEST_NM = 0.0;

				_NHB_ASC_AUTO = 0.0;
				_NHB_ASC_PASS = 0.0;
				_NHB_ASC_TAXI = 0.0;
				_NHB_ASC_WALK = 0.0;
				_NHB_ASC_TRAN = 0.0;
				_NHB_ASC_BIKE = 0.0;
				_NHB_B_cbd_pa = 0.0;
				_NHB_B_cost = 0.0;
				_NHB_B_cost_hinc = 0.0;
				_NHB_B_cost_minc = 0.0;
				_NHB_B_dens_bike = 0.0;
				_NHB_B_dens_walk = 0.0;
				_NHB_B_male_taxi = 0.0;
				_NHB_B_notalone_pass = 0.0;
				_NHB_B_over65_tran = 0.0;
				_NHB_B_over65_pass = 0.0;
				_NHB_B_ovttime_tran = 0.0;
				_NHB_B_peak_auto = 0.0;
				_NHB_B_ttime_bike = 0.0;
				_NHB_B_ttime_tran = 0.0;
				_NHB_B_ttime_walk = 0.0;
				_NHB_B_u18_pass = 0.0;
				_NHB_B_vehavail_pass = 0.0;
				_NHB_B_vehavail_tran = 0.0;
				_NHB_B_vehavail_taxi = 0.0;
				_NHB_B_waittime_tran = 0.0;
				_NHB_ASC_N_AUTO = 0.0;
				_NHB_ASC_N_NM = 0.0;
				_NHB_B_vehavail_nm = 0.0;
				_NHB_B_male_nm = 0.0;
				_NHB_B_cbd_nm = 0.0;
				_NHB_NEST_AUTO = 0.0;
				_NHB_NEST_NM = 0.0;

			}

			static void print_parameters()
			{
				cout << "Detroit_Mode_Choice_Option parameters" << endl;
				cout << "\tHBW_ASC_AUTO = " << HBW_ASC_AUTO<float>() << endl;
				cout << "\tHBO_ASC_AUTO = " << HBO_ASC_AUTO<float>() << endl;
				cout << "\tNHB_ASC_AUTO = " << NHB_ASC_AUTO<float>() << endl;
				cout << "\tHBW_ASC_PASS = " << HBW_ASC_PASS<float>() << endl;
				cout << "\tHBO_ASC_PASS = " << HBO_ASC_PASS<float>() << endl;
				cout << "\tNHB_ASC_PASS = " << NHB_ASC_PASS<float>() << endl;
				cout << "\tHBW_ASC_TAXI = " << HBW_ASC_TAXI<float>() << endl;
				cout << "\tHBO_ASC_TAXI = " << HBO_ASC_TAXI<float>() << endl;
				cout << "\tNHB_ASC_TAXI = " << NHB_ASC_TAXI<float>() << endl;
				cout << "\tHBW_ASC_WALK = " << HBW_ASC_WALK<float>() << endl;
				cout << "\tHBO_ASC_WALK = " << HBO_ASC_WALK<float>() << endl;
				cout << "\tNHB_ASC_WALK = " << NHB_ASC_WALK<float>() << endl;
				cout << "\tHBW_ASC_TRAN = " << HBW_ASC_TRAN<float>() << endl;
				cout << "\tHBO_ASC_TRAN = " << HBO_ASC_TRAN<float>() << endl;
				cout << "\tNHB_ASC_TRAN = " << NHB_ASC_TRAN<float>() << endl;
				cout << "\tHBW_ASC_BIKE = " << HBW_ASC_BIKE<float>() << endl;
				cout << "\tHBO_ASC_BIKE = " << HBO_ASC_BIKE<float>() << endl;
				cout << "\tNHB_ASC_BIKE = " << NHB_ASC_BIKE<float>() << endl;
				cout << "\tHBW_B_cbd_pa = " << HBW_B_cbd_pa<float>() << endl;
				cout << "\tHBO_B_cbd_pa = " << HBO_B_cbd_pa<float>() << endl;
				cout << "\tNHB_B_cbd_pa = " << NHB_B_cbd_pa<float>() << endl;
				cout << "\tHBW_B_cost = " << HBW_B_cost<float>() << endl;
				cout << "\tHBO_B_cost = " << HBO_B_cost<float>() << endl;
				cout << "\tNHB_B_cost = " << NHB_B_cost<float>() << endl;
				cout << "\tHBW_B_cost_hinc = " << HBW_B_cost_hinc<float>() << endl;
				cout << "\tHBO_B_cost_hinc = " << HBO_B_cost_hinc<float>() << endl;
				cout << "\tNHB_B_cost_hinc = " << NHB_B_cost_hinc<float>() << endl;
				cout << "\tHBW_B_cost_minc = " << HBW_B_cost_minc<float>() << endl;
				cout << "\tHBO_B_cost_minc = " << HBO_B_cost_minc<float>() << endl;
				cout << "\tNHB_B_cost_minc = " << NHB_B_cost_minc<float>() << endl;
				cout << "\tHBW_B_dens_bike = " << HBW_B_dens_bike<float>() << endl;
				cout << "\tHBO_B_dens_bike = " << HBO_B_dens_bike<float>() << endl;
				cout << "\tNHB_B_dens_bike = " << NHB_B_dens_bike<float>() << endl;
				cout << "\tHBW_B_dens_walk = " << HBW_B_dens_walk<float>() << endl;
				cout << "\tHBO_B_dens_walk = " << HBO_B_dens_walk<float>() << endl;
				cout << "\tNHB_B_dens_walk = " << NHB_B_dens_walk<float>() << endl;
				cout << "\tHBW_B_male_taxi = " << HBW_B_male_taxi<float>() << endl;
				cout << "\tHBO_B_male_taxi = " << HBO_B_male_taxi<float>() << endl;
				cout << "\tNHB_B_male_taxi = " << NHB_B_male_taxi<float>() << endl;
				cout << "\tHBW_B_notalone_pass = " << HBW_B_notalone_pass<float>() << endl;
				cout << "\tHBO_B_notalone_pass = " << HBO_B_notalone_pass<float>() << endl;
				cout << "\tNHB_B_notalone_pass = " << NHB_B_notalone_pass<float>() << endl;
				cout << "\tHBW_B_over65_pass = " << HBW_B_over65_pass<float>() << endl;
				cout << "\tHBO_B_over65_pass = " << HBO_B_over65_pass<float>() << endl;
				cout << "\tNHB_B_over65_pass = " << NHB_B_over65_pass<float>() << endl;
				cout << "\tHBW_B_over65_tran = " << HBW_B_over65_tran<float>() << endl;
				cout << "\tHBO_B_over65_tran = " << HBO_B_over65_tran<float>() << endl;
				cout << "\tNHB_B_over65_tran = " << NHB_B_over65_tran<float>() << endl;
				cout << "\tHBW_B_ovttime_tran = " << HBW_B_ovttime_tran<float>() << endl;
				cout << "\tHBO_B_ovttime_tran = " << HBO_B_ovttime_tran<float>() << endl;
				cout << "\tNHB_B_ovttime_tran = " << NHB_B_ovttime_tran<float>() << endl;
				cout << "\tHBW_B_peak_auto = " << HBW_B_peak_auto<float>() << endl;
				cout << "\tHBO_B_peak_auto = " << HBO_B_peak_auto<float>() << endl;
				cout << "\tNHB_B_peak_auto = " << NHB_B_peak_auto<float>() << endl;
				cout << "\tHBW_B_ttime_bike = " << HBW_B_ttime_bike<float>() << endl;
				cout << "\tHBO_B_ttime_bike = " << HBO_B_ttime_bike<float>() << endl;
				cout << "\tNHB_B_ttime_bike = " << NHB_B_ttime_bike<float>() << endl;
				cout << "\tHBW_B_ttime_tran = " << HBW_B_ttime_tran<float>() << endl;
				cout << "\tHBO_B_ttime_tran = " << HBO_B_ttime_tran<float>() << endl;
				cout << "\tNHB_B_ttime_tran = " << NHB_B_ttime_tran<float>() << endl;
				cout << "\tHBW_B_ttime_walk = " << HBW_B_ttime_walk<float>() << endl;
				cout << "\tHBO_B_ttime_walk = " << HBO_B_ttime_walk<float>() << endl;
				cout << "\tNHB_B_ttime_walk = " << NHB_B_ttime_walk<float>() << endl;
				cout << "\tHBW_B_u18_pass = " << HBW_B_u18_pass<float>() << endl;
				cout << "\tHBO_B_u18_pass = " << HBO_B_u18_pass<float>() << endl;
				cout << "\tNHB_B_u18_pass = " << NHB_B_u18_pass<float>() << endl;
				cout << "\tHBW_B_vehavail_pass = " << HBW_B_vehavail_pass<float>() << endl;
				cout << "\tHBO_B_vehavail_pass = " << HBO_B_vehavail_pass<float>() << endl;
				cout << "\tNHB_B_vehavail_pass = " << NHB_B_vehavail_pass<float>() << endl;
				cout << "\tHBW_B_vehavail_taxi = " << HBW_B_vehavail_taxi<float>() << endl;
				cout << "\tHBO_B_vehavail_taxi = " << HBO_B_vehavail_taxi<float>() << endl;
				cout << "\tNHB_B_vehavail_taxi = " << NHB_B_vehavail_taxi<float>() << endl;
				cout << "\tHBW_B_vehavail_tran = " << HBW_B_vehavail_tran<float>() << endl;
				cout << "\tHBO_B_vehavail_tran = " << HBO_B_vehavail_tran<float>() << endl;
				cout << "\tNHB_B_vehavail_tran = " << NHB_B_vehavail_tran<float>() << endl;
				cout << "\tHBW_B_waittime_tran = " << HBW_B_waittime_tran<float>() << endl;
				cout << "\tHBO_B_waittime_tran = " << HBO_B_waittime_tran<float>() << endl;
				cout << "\tNHB_B_waittime_tran = " << NHB_B_waittime_tran<float>() << endl;
				cout << "\tHBW_ASC_N_AUTO = " << HBW_ASC_N_AUTO<float>() << endl;
				cout << "\tHBO_ASC_N_AUTO = " << HBO_ASC_N_AUTO<float>() << endl;
				cout << "\tNHB_ASC_N_AUTO = " << NHB_ASC_N_AUTO<float>() << endl;
				cout << "\tHBW_ASC_N_NM = " << HBW_ASC_N_NM<float>() << endl;
				cout << "\tHBO_ASC_N_NM = " << HBO_ASC_N_NM<float>() << endl;
				cout << "\tNHB_ASC_N_NM = " << NHB_ASC_N_NM<float>() << endl;
				cout << "\tHBW_B_vehavail_nm = " << HBW_B_vehavail_nm<float>() << endl;
				cout << "\tHBO_B_vehavail_nm = " << HBO_B_vehavail_nm<float>() << endl;
				cout << "\tNHB_B_vehavail_nm = " << NHB_B_vehavail_nm<float>() << endl;
				cout << "\tHBW_B_male_nm = " << HBW_B_male_nm<float>() << endl;
				cout << "\tHBO_B_male_nm = " << HBO_B_male_nm<float>() << endl;
				cout << "\tNHB_B_male_nm = " << NHB_B_male_nm<float>() << endl;
				cout << "\tHBW_B_cbd_nm = " << HBW_B_cbd_nm<float>() << endl;
				cout << "\tHBO_B_cbd_nm = " << HBO_B_cbd_nm<float>() << endl;
				cout << "\tNHB_B_cbd_nm = " << NHB_B_cbd_nm<float>() << endl;
				cout << "\tHBW_NEST_AUTO = " << HBW_NEST_AUTO<float>() << endl;
				cout << "\tHBO_NEST_AUTO = " << HBO_NEST_AUTO<float>() << endl;
				cout << "\tNHB_NEST_AUTO = " << NHB_NEST_AUTO<float>() << endl;
				cout << "\tHBW_NEST_NM = " << HBW_NEST_NM<float>() << endl;
				cout << "\tHBO_NEST_NM = " << HBO_NEST_NM<float>() << endl;
				cout << "\tNHB_NEST_NM = " << NHB_NEST_NM<float>() << endl;
			}


			void copy(ComponentType* obj)
			{
				this->_Parent_Planner = obj->_Parent_Planner;
				this->_current_activity = obj->_current_activity;
				this->_previous_activity = obj->_previous_activity;
				this->_next_activity = obj->_next_activity;
				this->_destination = obj->_destination;
				this->_previous_location = obj->_previous_location;
				this->_next_location = obj->_next_location;
				this->_home_based = obj->_home_based;
				this->_auto_available = obj->_auto_available;
				this->_to_CBD = obj->_to_CBD;

			}

			// data members
			//m_prototype(Prototypes::Person_Planner, typename MasterType::person_planner_type, Parent_Planner, NONE, NONE);
			m_prototype(Prototypes::Mode_Chooser, typename MasterType::person_mode_chooser_type, Mode_Chooser, NONE, NONE);
			m_data(Vehicle_Components::Types::Vehicle_Type_Keys, mode_type, NONE, NONE);

			// PARAMETER DECLARATIONS
			#pragma region static parameters
			m_static_data(float, HBW_ASC_AUTO, NONE, NONE);	m_static_data(float, HBO_ASC_AUTO, NONE, NONE);	m_static_data(float, NHB_ASC_AUTO, NONE, NONE);
			m_static_data(float, HBW_ASC_PASS, NONE, NONE);	m_static_data(float, HBO_ASC_PASS, NONE, NONE);	m_static_data(float, NHB_ASC_PASS, NONE, NONE);
			m_static_data(float, HBW_ASC_TAXI, NONE, NONE);	m_static_data(float, HBO_ASC_TAXI, NONE, NONE);	m_static_data(float, NHB_ASC_TAXI, NONE, NONE);
			m_static_data(float, HBW_ASC_WALK, NONE, NONE);	m_static_data(float, HBO_ASC_WALK, NONE, NONE);	m_static_data(float, NHB_ASC_WALK, NONE, NONE);
			m_static_data(float, HBW_ASC_TRAN, NONE, NONE);	m_static_data(float, HBO_ASC_TRAN, NONE, NONE);	m_static_data(float, NHB_ASC_TRAN, NONE, NONE);
			m_static_data(float, HBW_ASC_BIKE, NONE, NONE);	m_static_data(float, HBO_ASC_BIKE, NONE, NONE);	m_static_data(float, NHB_ASC_BIKE, NONE, NONE);
			m_static_data(float, HBW_B_cbd_pa, NONE, NONE);	m_static_data(float, HBO_B_cbd_pa, NONE, NONE);	m_static_data(float, NHB_B_cbd_pa, NONE, NONE);
			m_static_data(float, HBW_B_cost, NONE, NONE);	m_static_data(float, HBO_B_cost, NONE, NONE);	m_static_data(float, NHB_B_cost, NONE, NONE);
			m_static_data(float, HBW_B_cost_hinc, NONE, NONE);	m_static_data(float, HBO_B_cost_hinc, NONE, NONE);	m_static_data(float, NHB_B_cost_hinc, NONE, NONE);
			m_static_data(float, HBW_B_cost_minc, NONE, NONE);	m_static_data(float, HBO_B_cost_minc, NONE, NONE);	m_static_data(float, NHB_B_cost_minc, NONE, NONE);
			m_static_data(float, HBW_B_dens_bike, NONE, NONE);	m_static_data(float, HBO_B_dens_bike, NONE, NONE);	m_static_data(float, NHB_B_dens_bike, NONE, NONE);
			m_static_data(float, HBW_B_dens_walk, NONE, NONE);	m_static_data(float, HBO_B_dens_walk, NONE, NONE);	m_static_data(float, NHB_B_dens_walk, NONE, NONE);
			m_static_data(float, HBW_B_male_taxi, NONE, NONE);	m_static_data(float, HBO_B_male_taxi, NONE, NONE);	m_static_data(float, NHB_B_male_taxi, NONE, NONE);
			m_static_data(float, HBW_B_notalone_pass, NONE, NONE);	m_static_data(float, HBO_B_notalone_pass, NONE, NONE);	m_static_data(float, NHB_B_notalone_pass, NONE, NONE);
			m_static_data(float, HBW_B_over65_pass, NONE, NONE);	m_static_data(float, HBO_B_over65_pass, NONE, NONE);	m_static_data(float, NHB_B_over65_pass, NONE, NONE);
			m_static_data(float, HBW_B_over65_tran, NONE, NONE);	m_static_data(float, HBO_B_over65_tran, NONE, NONE);	m_static_data(float, NHB_B_over65_tran, NONE, NONE);
			m_static_data(float, HBW_B_ovttime_tran, NONE, NONE);	m_static_data(float, HBO_B_ovttime_tran, NONE, NONE);	m_static_data(float, NHB_B_ovttime_tran, NONE, NONE);
			m_static_data(float, HBW_B_peak_auto, NONE, NONE);	m_static_data(float, HBO_B_peak_auto, NONE, NONE);	m_static_data(float, NHB_B_peak_auto, NONE, NONE);
			m_static_data(float, HBW_B_ttime_bike, NONE, NONE);	m_static_data(float, HBO_B_ttime_bike, NONE, NONE);	m_static_data(float, NHB_B_ttime_bike, NONE, NONE);
			m_static_data(float, HBW_B_ttime_tran, NONE, NONE);	m_static_data(float, HBO_B_ttime_tran, NONE, NONE);	m_static_data(float, NHB_B_ttime_tran, NONE, NONE);
			m_static_data(float, HBW_B_ttime_walk, NONE, NONE);	m_static_data(float, HBO_B_ttime_walk, NONE, NONE);	m_static_data(float, NHB_B_ttime_walk, NONE, NONE);
			m_static_data(float, HBW_B_u18_pass, NONE, NONE);	m_static_data(float, HBO_B_u18_pass, NONE, NONE);	m_static_data(float, NHB_B_u18_pass, NONE, NONE);
			m_static_data(float, HBW_B_vehavail_pass, NONE, NONE);	m_static_data(float, HBO_B_vehavail_pass, NONE, NONE);	m_static_data(float, NHB_B_vehavail_pass, NONE, NONE);
			m_static_data(float, HBW_B_vehavail_taxi, NONE, NONE);	m_static_data(float, HBO_B_vehavail_taxi, NONE, NONE);	m_static_data(float, NHB_B_vehavail_taxi, NONE, NONE);
			m_static_data(float, HBW_B_vehavail_tran, NONE, NONE);	m_static_data(float, HBO_B_vehavail_tran, NONE, NONE);	m_static_data(float, NHB_B_vehavail_tran, NONE, NONE);
			m_static_data(float, HBW_B_waittime_tran, NONE, NONE);	m_static_data(float, HBO_B_waittime_tran, NONE, NONE);	m_static_data(float, NHB_B_waittime_tran, NONE, NONE);
			m_static_data(float, HBW_ASC_N_AUTO, NONE, NONE);	m_static_data(float, HBO_ASC_N_AUTO, NONE, NONE);	m_static_data(float, NHB_ASC_N_AUTO, NONE, NONE);
			m_static_data(float, HBW_ASC_N_NM, NONE, NONE);	m_static_data(float, HBO_ASC_N_NM, NONE, NONE);	m_static_data(float, NHB_ASC_N_NM, NONE, NONE);
			m_static_data(float, HBW_B_vehavail_nm, NONE, NONE);	m_static_data(float, HBO_B_vehavail_nm, NONE, NONE);	m_static_data(float, NHB_B_vehavail_nm, NONE, NONE);
			m_static_data(float, HBW_B_male_nm, NONE, NONE);	m_static_data(float, HBO_B_male_nm, NONE, NONE);	m_static_data(float, NHB_B_male_nm, NONE, NONE);
			m_static_data(float, HBW_B_cbd_nm, NONE, NONE);	m_static_data(float, HBO_B_cbd_nm, NONE, NONE);	m_static_data(float, NHB_B_cbd_nm, NONE, NONE);
			m_static_data(float, HBW_NEST_AUTO, NONE, NONE);	m_static_data(float, HBO_NEST_AUTO, NONE, NONE);	m_static_data(float, NHB_NEST_AUTO, NONE, NONE);
			m_static_data(float, HBW_NEST_NM, NONE, NONE);	m_static_data(float, HBO_NEST_NM, NONE, NONE);	m_static_data(float, NHB_NEST_NM, NONE, NONE);
			#pragma endregion

			//====================================================================================================================================
			// Interface definitions
			typedef Prototypes::Person_Planner<typename type_of(Mode_Chooser)::type_of(Parent_Planner)> planner_itf;
			typedef Prototypes::Person<typename planner_itf::get_type_of(Parent_Person)> person_itf;
			typedef Prototypes::Person_Properties<typename person_itf::get_type_of(Static_Properties)> person_properties_itf;
			typedef Household_Components::Prototypes::Household<typename person_itf::get_type_of(Household)> household_itf;
			typedef Household_Components::Prototypes::Household_Properties<typename household_itf::get_type_of(Static_Properties)> household_properties_itf;
			typedef Vehicle_Components::Prototypes::Vehicle<typename person_itf::get_type_of(vehicle)> vehicle_itf;
			typedef Prototypes::Person_Scheduler<typename person_itf::get_type_of(Scheduling_Faculty)> scheduler_itf;
			typedef Scenario_Components::Prototypes::Scenario< typename person_itf::get_type_of(scenario_reference)> _Scenario_Interface;
			typedef Network_Components::Prototypes::Network< typename person_itf::get_type_of(network_reference)> _Network_Interface;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _Network_Interface::get_type_of(skimming_faculty)> _Skim_Interface;

			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(_Activity_Locations_Container_Interface)>  _Activity_Location_Interface;

			typedef Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links)> _Links_Container_Interface;
			typedef Link_Components::Prototypes::Link<get_component_type(_Links_Container_Interface)>  _Link_Interface;

			typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_Zones_Container_Interface)>  _Zone_Interface;

			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Activity_Container)> Activity_Plans;
			typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Plans)> Activity_Plan;

			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Movement_Plans_Container)> Movement_Plans;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan<get_component_type(Movement_Plans)> Movement_Plan;


			typedef Network_Skimming_Components::Prototypes::LOS<typename MasterType::los_value_type> los_itf;
			typedef Network_Skimming_Components::Prototypes::LOS<typename MasterType::los_invariant_value_type> los_invariant_itf;

			//------------------------------------------------------------------------------------------------------------------------------------

			// Feature called from prototype and by Choice_Model
			virtual double Calculate_Utility()
			{
				los_itf* los = _Mode_Chooser->los<los_itf*>();
			
				float utility = -1.0*FLT_MAX;

				person_itf* _Parent_Person = this->_Mode_Chooser->Parent_Planner<planner_itf*>()->template Parent_Person<person_itf*>();
				household_itf* household = _Parent_Person->Household<household_itf*>();
				household_properties_itf* hh_properties = household->Static_Properties<household_properties_itf*>();
				scheduler_itf* scheduler = _Parent_Person->template Scheduling_Faculty<scheduler_itf*>();
				person_properties_itf* properties = _Parent_Person->template Static_Properties<person_properties_itf*>();
				vehicle_itf* vehicle = _Parent_Person->template vehicle<vehicle_itf*>();
				_Zone_Interface* dest_zone = _Mode_Chooser->destination<_Activity_Location_Interface*>()->template zone<_Zone_Interface*>();
				_Zone_Interface* orig_zone = _Mode_Chooser->previous_location<_Activity_Location_Interface*>()->template zone<_Zone_Interface*>();
				Activity_Plan* current_activity = _Mode_Chooser->current_activity<Activity_Plan*>();
				Activity_Components::Types::ACTIVITY_TYPES activity_type = current_activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();

				// calculate utilty model variable requirements
				Time_Minutes start = current_activity->Start_Time<Time_Minutes>();
				double h_inc = hh_properties->Income<Basic_Units::Currency_Variables::Dollars>() > 100000 ? 1.0 : 0.0;
				double m_inc = hh_properties->Income<Basic_Units::Currency_Variables::Dollars>() > 50000 && h_inc == 0.0 ? 1.0 : 0.0;
				double PEAK = (start > 420 && start < 600) || (start > 900 && start < 1080) ? 1.0 : 0.0;
				double AUTO_TT = los->auto_ttime<Time_Minutes>();
				double AUTO_COST = los->auto_distance<Miles>() * 0.2;
				double AUTO_COST_M = AUTO_COST * m_inc;
				double AUTO_COST_H = AUTO_COST * h_inc;
				double PASS_COST = 0.5 * AUTO_COST;
				double PASS_COST_M = AUTO_COST_M;
				double PASS_COST_H = AUTO_COST_H;
				double TAXI_COST = los->auto_distance<Miles>() * 2.25 + 3.0;
				double TAXI_COST_M = AUTO_COST * m_inc;
				double TAXI_COST_H = AUTO_COST * h_inc;
				double POP_DENS = log(max(1.0, (dest_zone->population_density<Basic_Units::Area_Variables::Square_Kilometers, double>() + orig_zone->population_density<Basic_Units::Area_Variables::Square_Kilometers, double>()) / 2.0));
				double BIKE_TTr = los->auto_distance<Miles>() / 10.0 * 60.0 - AUTO_TT;
				double WALK_TTr = los->auto_distance<Miles>() / 3.0 * 60.0 - AUTO_TT;
				double TRAN_IVTTr = los->transit_ttime<Time_Minutes>() - AUTO_TT;
				double TRAN_WAIT = los->transit_wait_time<Time_Minutes>();
				double TRAN_COST = los->transit_fare<Dollars>();
				double TRAN_COST_M = TRAN_COST * m_inc;
				double TRAN_COST_H = TRAN_COST * h_inc;
				
				//TODO OMER JOSH
				Kilometers_Per_Hour walkSpeed_kph = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::walkSpeed<float>();
				Kilometers_Per_Minute walkSpeed_kpm = GLOBALS::Convert_Units<Kilometers_Per_Hour, Kilometers_Per_Minute>(walkSpeed_kph);

				double TRAN_OVTT = (_Mode_Chooser->template walk_distance_to_transit<Kilometers>() + _Mode_Chooser->template walk_distance_after_transit<Kilometers>() ) / walkSpeed_kpm; // meters/(meters per second *60)
				
				//double TRAN_OVTT = los->transit_walk_access_time<Time_Minutes>();
				
				if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::PARK_AND_RIDE)
				{
					TRAN_OVTT = _Mode_Chooser->template drive_fft_to_transit<Time_Minutes>() + _Mode_Chooser->template walk_distance_after_transit<Kilometers>() / walkSpeed_kpm; //seconds to minutes
				}

				double GENDER = properties->Gender<Person_Components::Types::GENDER>() == Person_Components::Types::GENDER::MALE ? 1.0 : 0.0;
				double VEHPERLIC = hh_properties->Number_of_workers<double>() > 0 ? hh_properties->Number_of_vehicles<double>() / hh_properties->Number_of_workers<double>() : 0.0;
				double AGE65 = properties->Age<int>() >= 65 ? 1.0 : 0.0;
				double IN_CBD = _Mode_Chooser->to_CBD<bool>() ? 1.0 : 0.0;
				double NOTALONE = 0.0;
				double AGEU18 = properties->Age<int>() <18 ? 1.0 : 0.0;
				double ONE = 1.0;

				// return 0 for nest level utilities (all parameters should be at the leaf level)
				if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::AUTO || this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::NONMOTORIZED) return 0.0;

				// Split mode choice model by Home-based Work, Home-based other and non-home based
				if (_Mode_Chooser->home_based<bool>())
				{
					// home based work/school
					if (_Mode_Chooser->to_work_school<bool>())
					{
						if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::SOV)		utility = this->_HBW_ASC_N_AUTO + this->_HBW_B_peak_auto * PEAK + this->_HBW_B_cost * AUTO_COST + this->_HBW_B_cost_minc * AUTO_COST_M + this->_HBW_B_cost_hinc * AUTO_COST_H;
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::BUS || this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::PARK_AND_RIDE)		utility = this->_HBW_ASC_TRAN + this->_HBW_B_waittime_tran * TRAN_WAIT + this->_HBW_B_over65_tran * AGE65 + this->_HBW_B_ttime_tran * TRAN_IVTTr + this->_HBW_B_vehavail_tran * VEHPERLIC + this->_HBW_B_cost * TRAN_COST + this->_HBW_B_cost_minc * TRAN_COST_M + this->_HBW_B_ovttime_tran * TRAN_OVTT + this->_HBW_B_cost_hinc * TRAN_COST_H;
						/*else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::BUS || this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::PARK_AND_RIDE)		utility = this->_HBW_ASC_TRAN + this->_HBW_B_waittime_tran * TRAN_WAIT + this->_HBW_B_over65_tran * AGE65 + this->_HBW_B_ttime_tran * TRAN_IVTTr + this->_HBW_B_vehavail_tran * VEHPERLIC + this->_HBW_B_cost * TRAN_COST + this->_HBW_B_cost_minc * TRAN_COST_M + this->_HBW_B_ovttime_tran * TRAN_OVTT + this->_HBW_B_cost_hinc * TRAN_COST_H;*/
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::TAXI)	utility = this->_HBW_ASC_TAXI + this->_HBW_ASC_N_AUTO + this->_HBW_B_cost * TAXI_COST + this->_HBW_B_vehavail_taxi * VEHPERLIC + this->_HBW_B_cost_minc * TAXI_COST_M + this->_HBW_B_male_taxi * GENDER + this->_HBW_B_cost_hinc * TAXI_COST_H;
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE) utility = this->_HBW_ASC_BIKE + this->_HBW_B_dens_bike * POP_DENS + this->_HBW_ASC_N_NM + this->_HBW_B_ttime_bike * BIKE_TTr + this->_HBW_B_male_nm * GENDER + this->_HBW_B_vehavail_nm * VEHPERLIC;
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::WALK)	utility = this->_HBW_ASC_N_NM + this->_HBW_B_dens_walk * POP_DENS + this->_HBW_B_ttime_walk * WALK_TTr + this->_HBW_ASC_WALK + this->_HBW_B_male_nm * GENDER + this->_HBW_B_vehavail_nm * VEHPERLIC;
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::HOV)		utility = this->_HBW_ASC_N_AUTO + this->_HBW_B_over65_pass * AGE65 + this->_HBW_B_cbd_pa * IN_CBD + this->_HBW_B_notalone_pass * NOTALONE + this->_HBW_ASC_PASS + this->_HBW_B_vehavail_pass * VEHPERLIC + this->_HBW_B_u18_pass * AGEU18 + this->_HBW_B_cost * PASS_COST + this->_HBW_B_cost_minc * PASS_COST_M + this->_HBW_B_cost_hinc * PASS_COST_H;
						else THROW_EXCEPTION("UNKNOWN MODE TYPE.")
					}
					// home based other
					else
					{
						if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::SOV)			utility = _HBO_ASC_N_AUTO * ONE + _HBO_B_peak_auto * PEAK + _HBO_B_cost * AUTO_COST + _HBO_ASC_AUTO * ONE + _HBO_B_cost_minc * AUTO_COST_M + _HBO_B_cost_hinc * AUTO_COST_H;
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE) utility = this->_HBO_ASC_BIKE + _HBO_B_dens_bike * POP_DENS + _HBO_ASC_N_NM * ONE + _HBO_B_ttime_bike * BIKE_TTr + _HBO_B_male_nm * GENDER + _HBO_B_vehavail_nm * VEHPERLIC;
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::HOV)		utility = _HBO_ASC_N_AUTO * ONE + _HBO_B_over65_pass * AGE65 + _HBO_B_cbd_pa * IN_CBD + _HBO_B_notalone_pass * NOTALONE + _HBO_B_vehavail_pass * VEHPERLIC + _HBO_B_u18_pass * AGEU18 + _HBO_B_cost * PASS_COST + _HBO_B_cost_minc * PASS_COST_M + _HBO_B_cost_hinc * PASS_COST_H;
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::TAXI)	utility = _HBO_ASC_TAXI * ONE + _HBO_ASC_N_AUTO * ONE + _HBO_B_cost * TAXI_COST + _HBO_B_vehavail_taxi * VEHPERLIC + _HBO_B_cost_minc * TAXI_COST_M + _HBO_B_male_taxi * GENDER + _HBO_B_cost_hinc * TAXI_COST_H;
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::BUS || this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::PARK_AND_RIDE)		utility = this->_HBO_ASC_TRAN + _HBO_B_waittime_tran * TRAN_WAIT + _HBO_B_over65_tran * AGE65 + _HBO_B_ttime_tran * TRAN_IVTTr + _HBO_B_vehavail_tran * VEHPERLIC + _HBO_B_cost * TRAN_COST + _HBO_B_cost_minc * TRAN_COST_M + _HBO_B_ovttime_tran * TRAN_OVTT + _HBO_B_cost_hinc * TRAN_COST_H;
						/*else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::BUS || this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::PARK_AND_RIDE)		utility = this->_HBO_ASC_TRAN + _HBO_B_waittime_tran * TRAN_WAIT + _HBO_B_over65_tran * AGE65 + _HBO_B_ttime_tran * TRAN_IVTTr + _HBO_B_vehavail_tran * VEHPERLIC + _HBO_B_cost * TRAN_COST + _HBO_B_cost_minc * TRAN_COST_M + _HBO_B_ovttime_tran * TRAN_OVTT + _HBO_B_cost_hinc * TRAN_COST_H;*/
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::WALK)	utility = _HBO_ASC_N_NM * ONE + _HBO_B_dens_walk * POP_DENS + _HBO_B_ttime_walk * WALK_TTr + _HBO_ASC_WALK * ONE + _HBO_B_male_nm * GENDER + _HBO_B_vehavail_nm * VEHPERLIC;
						else THROW_EXCEPTION("UNKNOWN MODE TYPE.")
					}
				}
				else // non-home based
				{
					// work/school based subtour:
					if (this->_Mode_Chooser->from_work_school<bool>() && this->_Mode_Chooser->to_work_school<bool>())
					{
						if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::SOV)			utility = _NHB_ASC_N_AUTO * ONE + _NHB_B_peak_auto * PEAK + _NHB_B_cost * AUTO_COST + _NHB_B_cost_minc * AUTO_COST_M + _NHB_B_cost_hinc * AUTO_COST_H;
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE) utility = this->_NHB_ASC_BIKE + _NHB_B_dens_bike * POP_DENS + _NHB_B_cbd_nm * IN_CBD + _NHB_ASC_N_NM * ONE + _NHB_B_ttime_bike * BIKE_TTr + _NHB_B_male_nm * GENDER + _NHB_B_vehavail_nm * VEHPERLIC;
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::HOV)		utility = _NHB_ASC_N_AUTO * ONE + _NHB_B_over65_pass * AGE65 + _NHB_B_cbd_pa * IN_CBD + _NHB_B_notalone_pass * NOTALONE + _NHB_ASC_PASS * ONE + _NHB_B_vehavail_pass * VEHPERLIC + _NHB_B_cost * PASS_COST + _NHB_B_cost_minc * PASS_COST_M + _NHB_B_cost_hinc * PASS_COST_H;
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::TAXI)	utility = _NHB_ASC_TAXI * ONE + _NHB_ASC_N_AUTO * ONE + _NHB_B_cost * TAXI_COST + _NHB_B_vehavail_taxi * VEHPERLIC + _NHB_B_cost_minc * TAXI_COST_M + _NHB_B_male_taxi * GENDER + _NHB_B_cost_hinc * TAXI_COST_H;
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::BUS || this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::PARK_AND_RIDE)		utility = this->_NHB_ASC_TRAN + _NHB_B_waittime_tran * TRAN_WAIT + _NHB_B_ttime_tran * TRAN_IVTTr + _NHB_B_cost * TRAN_COST + _NHB_B_cost_minc * TRAN_COST_M + _NHB_B_ovttime_tran * TRAN_OVTT + _NHB_B_cost_hinc * TRAN_COST_H;
						/*else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::BUS || this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::PARK_AND_RIDE)		utility = this->_NHB_ASC_TRAN + _NHB_B_waittime_tran * TRAN_WAIT + _NHB_B_ttime_tran * TRAN_IVTTr + _NHB_B_cost * TRAN_COST + _NHB_B_cost_minc * TRAN_COST_M + _NHB_B_ovttime_tran * TRAN_OVTT + _NHB_B_cost_hinc * TRAN_COST_H;*/
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::WALK)	utility = _NHB_B_cbd_nm * IN_CBD + _NHB_ASC_N_NM * ONE + _NHB_B_dens_walk * POP_DENS + _NHB_B_ttime_walk * WALK_TTr + ComponentType::_NHB_ASC_WALK * ONE + _NHB_B_male_nm * GENDER + _NHB_B_vehavail_nm * VEHPERLIC;
						else THROW_EXCEPTION("UNKNOWN MODE TYPE.")
					}
					//TODO: implement heuristics for tour-based restrictions on travel mode - placeholder here just reapplies the NHB model
					// use whatever the previous mode is - can't change mode mid-tour
					else
					{
						if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::SOV)			utility = _NHB_ASC_N_AUTO * ONE + _NHB_B_peak_auto * PEAK + _NHB_B_cost * AUTO_COST + _NHB_B_cost_minc * AUTO_COST_M + _NHB_B_cost_hinc * AUTO_COST_H;
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE) utility = this->_NHB_ASC_BIKE + _NHB_B_dens_bike * POP_DENS + _NHB_B_cbd_nm * IN_CBD + _NHB_ASC_N_NM * ONE + _NHB_B_ttime_bike * BIKE_TTr + _NHB_B_male_nm * GENDER + _NHB_B_vehavail_nm * VEHPERLIC;
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::HOV)		utility = _NHB_ASC_N_AUTO * ONE + _NHB_B_over65_pass * AGE65 + _NHB_B_cbd_pa * IN_CBD + _NHB_B_notalone_pass * NOTALONE + _NHB_ASC_PASS * ONE + _NHB_B_vehavail_pass * VEHPERLIC + _NHB_B_cost * PASS_COST + _NHB_B_cost_minc * PASS_COST_M + _NHB_B_cost_hinc * PASS_COST_H;
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::TAXI)	utility = _NHB_ASC_TAXI * ONE + _NHB_ASC_N_AUTO * ONE + _NHB_B_cost * TAXI_COST + _NHB_B_vehavail_taxi * VEHPERLIC + _NHB_B_cost_minc * TAXI_COST_M + _NHB_B_male_taxi * GENDER + _NHB_B_cost_hinc * TAXI_COST_H;
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::BUS || this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::PARK_AND_RIDE)		utility = this->_NHB_ASC_TRAN + _NHB_B_waittime_tran * TRAN_WAIT + _NHB_B_ttime_tran * TRAN_IVTTr + _NHB_B_cost * TRAN_COST + _NHB_B_cost_minc * TRAN_COST_M + _NHB_B_ovttime_tran * TRAN_OVTT + _NHB_B_cost_hinc * TRAN_COST_H;
						/*else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::BUS || this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::PARK_AND_RIDE)		utility = this->_NHB_ASC_TRAN + _NHB_B_waittime_tran * TRAN_WAIT + _NHB_B_ttime_tran * TRAN_IVTTr + _NHB_B_cost * TRAN_COST + _NHB_B_cost_minc * TRAN_COST_M + _NHB_B_ovttime_tran * TRAN_OVTT + _NHB_B_cost_hinc * TRAN_COST_H;*/
						else if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::WALK)	utility = _NHB_B_cbd_nm * IN_CBD + _NHB_ASC_N_NM * ONE + _NHB_B_dens_walk * POP_DENS + _NHB_B_ttime_walk * WALK_TTr + _NHB_ASC_WALK * ONE + _NHB_B_male_nm * GENDER + _NHB_B_vehavail_nm * VEHPERLIC;
						else THROW_EXCEPTION("UNKNOWN MODE TYPE.")
					}
				}

				if (utility < -999999.0) THROW_WARNING("WARNING: utility=-inf, (Tripd,Mode,AutoTT,AutoCost,TranTT,TranOVTT,TranWait,TranFare,WalkTT,O,D): " << household->uuid<int>() << "," << this->_mode_type << "," << AUTO_TT << "," << AUTO_COST << "," << TRAN_IVTTr << "," << TRAN_OVTT << "," << TRAN_WAIT << "," << TRAN_COST << "," << WALK_TTr<<","<<orig_zone->uuid<int>()<<","<<dest_zone->uuid<int>());
				if (utility > 100.0) THROW_WARNING("WARNING: utility=+inf (Tripd,Mode,AutoTT,AutoCost,TranTT,TranOVTT,TranWait,TranFare,WalkTT): " << household->uuid<int>() << "," << this->_mode_type << "," << AUTO_TT << "," << AUTO_COST << "," << TRAN_IVTTr << "," << TRAN_OVTT << "," << TRAN_WAIT << "," << TRAN_COST << "," << WALK_TTr << "," << orig_zone->uuid<int>() << "," << dest_zone->uuid<int>());
				if (ISNAN(utility) ) THROW_WARNING("WARNING: utility=NaN, (Tripd,Mode,AutoTT,AutoCost,TranTT,TranOVTT,TranWait,TranFare,WalkTT): "<< household->uuid<int>()<<","<<this->_mode_type<<","<< AUTO_TT << "," << AUTO_COST << "," << TRAN_IVTTr << "," << TRAN_OVTT << "," << TRAN_WAIT<<","<<TRAN_COST<<","<<WALK_TTr << "," << orig_zone->uuid<int>() << "," << dest_zone->uuid<int>());
				return utility;
			}

			virtual void Print_Utility()
			{
				//return 0.0;
			}

		};
		
		#pragma region Choice option parameters	
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_ASC_AUTO);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_ASC_PASS);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_ASC_TAXI);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_ASC_WALK);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_ASC_TRAN);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_ASC_BIKE);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_cbd_pa);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_cost);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_cost_hinc);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_cost_minc);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_dens_bike);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_dens_walk);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_male_taxi);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_notalone_pass);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_over65_pass);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_over65_tran);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_ovttime_tran);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_peak_auto);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_ttime_bike);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_ttime_tran);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_ttime_walk);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_u18_pass);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_vehavail_pass);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_vehavail_taxi);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_vehavail_tran);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_waittime_tran );
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_ASC_N_AUTO);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_ASC_N_NM);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_vehavail_nm);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_male_nm);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_B_cbd_nm);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_NEST_AUTO);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBW_NEST_NM);
	
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_ASC_AUTO			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_ASC_PASS			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_ASC_TAXI			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_ASC_WALK			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_ASC_TRAN			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_ASC_BIKE			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_cbd_pa			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_cost			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_cost_hinc		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_cost_minc		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_dens_bike		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_dens_walk		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_male_taxi		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_notalone_pass	);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_over65_pass		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_over65_tran		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_ovttime_tran	);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_peak_auto		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_ttime_bike		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_ttime_tran		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_ttime_walk		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_u18_pass		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_vehavail_pass	);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_vehavail_taxi	);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_vehavail_tran	);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_waittime_tran	);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_ASC_N_AUTO		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_ASC_N_NM			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_vehavail_nm		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_male_nm			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_B_cbd_nm			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_NEST_AUTO			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, HBO_NEST_NM			);

		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_ASC_AUTO			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_ASC_PASS			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_ASC_TAXI			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_ASC_WALK			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_ASC_TRAN			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_ASC_BIKE			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_cbd_pa			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_cost			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_cost_hinc		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_cost_minc		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_dens_bike		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_dens_walk		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_male_taxi		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_notalone_pass	);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_over65_tran		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_over65_pass		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_ovttime_tran	);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_peak_auto		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_ttime_bike		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_ttime_tran		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_ttime_walk		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_u18_pass		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_vehavail_pass	);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_vehavail_tran	);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_vehavail_taxi	);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_waittime_tran	);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_ASC_N_AUTO		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_ASC_N_NM			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_vehavail_nm		);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_male_nm			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_B_cbd_nm			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_NEST_AUTO			);
		define_static_member_variable(ADAPTS_Mode_Choice_Option, NHB_NEST_NM			);
		#pragma endregion

		implementation struct ADAPTS_Mode_Chooser_Implementation : public Polaris_Component<MasterType, INHERIT(ADAPTS_Mode_Chooser_Implementation), Data_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType, INHERIT(ADAPTS_Mode_Chooser_Implementation), Data_Object>::Component_Type ComponentType;

			// Pointer to the Parent class
			m_prototype(Prototypes::Person_Planner, typename MasterType::person_planner_type, Parent_Planner, NONE, NONE);
			m_prototype(Choice_Model_Components::Prototypes::Choice_Model, typename MasterType::nl_model_type, Choice_Model, NONE, NONE);
			m_prototype(Activity_Components::Prototypes::Activity_Planner, typename MasterType::activity_type, current_activity, NONE, NONE);
			m_prototype(Activity_Components::Prototypes::Activity_Planner, typename MasterType::activity_type, previous_activity, NONE, NONE);
			m_prototype(Activity_Components::Prototypes::Activity_Planner, typename MasterType::activity_type, next_activity, NONE, NONE);
			m_prototype(Activity_Location_Components::Prototypes::Activity_Location, typename MasterType::activity_location_type, destination, NONE, NONE);
			m_prototype(Activity_Location_Components::Prototypes::Activity_Location, typename MasterType::activity_location_type, previous_location, NONE, NONE);
			m_prototype(Activity_Location_Components::Prototypes::Activity_Location, typename MasterType::activity_location_type, next_location, NONE, NONE);
			m_prototype(Network_Skimming_Components::Prototypes::LOS, typename MasterType::los_value_type, los, NONE, NONE);
			m_data(bool, home_based, NONE, NONE);
			m_data(bool, to_work_school, NONE, NONE);
			m_data(bool, from_work_school, NONE, NONE);
			m_data(bool, auto_available, NONE, NONE);
			m_data(bool, to_CBD, NONE, NONE);
			member_component_and_feature_accessor(walk_distance_to_transit, Value, Basic_Units::Prototypes::Length, Basic_Units::Implementations::Length_Implementation<NT>);
			member_component_and_feature_accessor(walk_distance_after_transit, Value, Basic_Units::Prototypes::Length, Basic_Units::Implementations::Length_Implementation<NT>);
			member_component_and_feature_accessor(drive_fft_to_transit, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);

			m_data(bool, delete_los, NONE, NONE);
			m_data(_lock, update_lock, NONE, NONE);

			// Interface definitions	
			typedef Choice_Model_Components::Prototypes::Choice_Model<typename MasterType::nl_model_type > _Choice_Model_Interface;
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

			typedef Network_Skimming_Components::Prototypes::LOS<typename MasterType::los_value_type> los_itf;
			typedef Network_Skimming_Components::Prototypes::LOS<typename MasterType::los_invariant_value_type> los_invariant_itf;

			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(_Activity_Locations_Container_Interface)>  _Activity_Location_Interface;

			typedef Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links)> _Links_Container_Interface;
			typedef Link_Components::Prototypes::Link<get_component_type(_Links_Container_Interface)>  _Link_Interface;

			typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_Zones_Container_Interface)>  _Zone_Interface;

			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(zone_ids_container), int> _Zone_Ids_Interface;

			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Activity_Container)> Activity_Plans;
			typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Plans)> Activity_Plan;

			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Movement_Plans_Container)> Movement_Plans;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan<get_component_type(Movement_Plans)> Movement_Plan;


			template<typename TargetType> void Initialize()
			{
			}

			template<typename ActivityItfType> void Define_Travel_Context(ActivityItfType activity)
			{
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
				scheduler_itf* scheduler = _Parent_Person->template Scheduling_Faculty<scheduler_itf*>();
				household_itf* _Parent_Household = _Parent_Person->person_itf::template Household<household_itf*>();
				household_static_properties_itf* household_properties = _Parent_Household->template Static_Properties<household_static_properties_itf*>();
				_delete_los = false;

				// If no vehicles in the household
				_auto_available = true;
				if (household_properties->template Number_of_vehicles<int>() < 1) _auto_available = false;

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();
				_Skim_Interface* skim = network->template skimming_faculty<_Skim_Interface*>();

				_current_activity = (Activity_Plan*)activity;
				
				// If the start time is known, set the previous activity/location and the next activity/location to do mode choice planning
				if (_current_activity->Start_Is_Planned())
				{
					_previous_activity = _Parent_Person->template previous_activity_plan<Time_Seconds, Activity_Plan*>(_current_activity->template Start_Time<Time_Seconds>());
					_previous_location = scheduler->template previous_location<Activity_Plan*, _Activity_Location_Interface*>(_current_activity);
					if (_previous_location == nullptr) _previous_location = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
					_next_activity = _Parent_Person->template next_activity_plan<Time_Seconds, Activity_Plan*>(_current_activity->template Start_Time<Time_Seconds>());
					_next_location = scheduler->template next_location<Activity_Plan*, _Activity_Location_Interface*>(_current_activity);
					if (_next_location == nullptr) _next_location = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
					// check for auto availability constraints - make sure it is not a home/work based activity and see if previous mode is non-auto
					if (_previous_location != _Parent_Person->template Home_Location<_Activity_Location_Interface*>() && _previous_location != _Parent_Person->template Work_Location<_Activity_Location_Interface*>() && _previous_location != _Parent_Person->template School_Location<_Activity_Location_Interface*>())
					{
						if (_previous_activity->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() != Vehicle_Components::Types::Vehicle_Type_Keys::SOV) _auto_available = false;
					}
				}
				// Otherwise, next activities not known, assume start and end tour location is home
				else
				{
					_previous_activity = nullptr;
					_previous_location = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
					_next_activity = nullptr;
					_next_location = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
				}

				// See if this is a trip to/from CBD
				if (_current_activity->Location_Is_Planned())
				{
					_destination = _current_activity->template Location<_Activity_Location_Interface*>();
					int orig_zone = _previous_location->zone<_Zone_Interface*>()->uuid<int>();
					int dest_zone = _destination->zone<_Zone_Interface*>()->uuid<int>();
					if ((orig_zone > 159 && orig_zone < 216) || (dest_zone > 159 && dest_zone < 216)) _to_CBD = true;
					else _to_CBD = false;
				}
				else
				{
					_destination = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
				}

				//=========================================================
				// Find trip typology
				//---------------------------------------------------------
				// Determine if the trip is from home
				if (_previous_location == _Parent_Person->template Home_Location<_Activity_Location_Interface*>()) _home_based = true;
				else _home_based = false;

				// Determine if trip is to work
				Activity_Components::Types::ACTIVITY_TYPES activity_type = _current_activity->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();
				if (activity_type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY ||
					activity_type == Activity_Components::Types::ACTIVITY_TYPES::OTHER_WORK_ACTIVITY ||
					activity_type == Activity_Components::Types::ACTIVITY_TYPES::PART_TIME_WORK_ACTIVITY ||
					activity_type == Activity_Components::Types::ACTIVITY_TYPES::SCHOOL_ACTIVITY)
				{
					this->_to_work_school = true;
				}

				// Determine if trip is from work/school
				if (_previous_activity != nullptr)
				{
					activity_type = _previous_activity->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();
					if (activity_type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY ||
						activity_type == Activity_Components::Types::ACTIVITY_TYPES::OTHER_WORK_ACTIVITY ||
						activity_type == Activity_Components::Types::ACTIVITY_TYPES::PART_TIME_WORK_ACTIVITY ||
						activity_type == Activity_Components::Types::ACTIVITY_TYPES::SCHOOL_ACTIVITY)
					{
						this->_from_work_school = true;
					}
					else this->_from_work_school = false;
				}
				else this->_from_work_school = false;

				//=========================================================
				// set the level of service for this context
				//---------------------------------------------------------
				if (_current_activity->Location_Is_Planned())
				{
					if (_current_activity->Start_Is_Planned()) _los = skim->template Get_LOS<_Activity_Location_Interface*, Time_Seconds, los_itf*>(_previous_location, _destination, _current_activity->template Start_Time<Time_Seconds>());
					else _los = skim->template Get_LOS<_Activity_Location_Interface*, Time_Hours, los_itf*>(_previous_location, _destination, 12.0);
				}
				else
				{
					_delete_los = true;
					_Zone_Interface* origin_zone = _previous_location->zone<_Zone_Interface*>();
					los_itf* los = (los_itf*)Allocate<typename MasterType::los_value_type>();
					los_invariant_itf* ilos = (los_invariant_itf*)Allocate<typename MasterType::los_invariant_value_type>();
					los->LOS_time_invariant(ilos);
					los->auto_ttime(origin_zone->template avg_ttime_auto_ampeak<Time_Minutes>());
					los->transit_ttime(origin_zone->avg_ttime_transit<Time_Minutes>());
					los->transit_fare(origin_zone->avg_fare_transit<Dollars>());
					los->transit_walk_access_time(origin_zone->avg_ovtt_transit<Time_Minutes>());
					los->transit_wait_time(origin_zone->avg_wait_transit<Time_Minutes>());
					los->auto_distance(origin_zone->avg_distance<Miles>());
					this->_los = los;
				}

				//Calculating walking/driving distance from the origin to the closest transit, from the closests transit to destination 
				_Links_Container_Interface* origin_walk_links = _previous_location->template origin_walk_links<_Links_Container_Interface*>();
				float walk_distance_to_transit;
				if (origin_walk_links)
				{
					walk_distance_to_transit = 0;
				}
				else
				{
					walk_distance_to_transit = FLT_MAX / 2.0f;
				}
				int org_ctr = 0;
				for (auto itr = origin_walk_links->begin(); itr != origin_walk_links->end(); ++itr)
				{
					_Link_Interface* origin_walk_link = (_Link_Interface*)(*itr);
					float temp_dist = origin_walk_link->walk_distance_to_transit<float>();
					walk_distance_to_transit = walk_distance_to_transit + (temp_dist - walk_distance_to_transit) / (org_ctr + 1);
					org_ctr++;
				}

				_Links_Container_Interface* destination_walk_links = _destination->template destination_walk_links<_Links_Container_Interface*>();
				float walk_distance_after_transit;
				if (destination_walk_links)
				{
					walk_distance_after_transit = 0;
				}
				else
				{
					walk_distance_after_transit = FLT_MAX / 2.0f;
				}
				int dst_ctr = 0;
				for (auto itr = destination_walk_links->begin(); itr != destination_walk_links->end(); ++itr)
				{
					_Link_Interface* destination_walk_link = (_Link_Interface*)(*itr);
					float temp_dist = destination_walk_link->walk_distance_to_transit<float>();
					walk_distance_after_transit = walk_distance_after_transit + (temp_dist - walk_distance_after_transit) / (dst_ctr + 1);
					dst_ctr++;
				}

				_Links_Container_Interface* origin_links = _previous_location->template origin_links<_Links_Container_Interface*>();
				float drive_fft_to_transit;
				if (origin_links)
				{
					drive_fft_to_transit = 0;
				}
				else
				{
					drive_fft_to_transit = FLT_MAX / 2.0f;
				}
				org_ctr = 0;
				for (auto itr = origin_links->begin(); itr != origin_links->end(); ++itr)
				{
					_Link_Interface* origin_link = (_Link_Interface*)(*itr);
					float temp_time = origin_link->drive_fft_to_transit<float>();
					drive_fft_to_transit = drive_fft_to_transit + (temp_time - drive_fft_to_transit) / (org_ctr + 1);
					org_ctr++;
				}

				this->template walk_distance_to_transit<Meters>(walk_distance_to_transit);
				this->template walk_distance_after_transit<Meters>(walk_distance_after_transit);
				this->template drive_fft_to_transit<Time_Seconds>(drive_fft_to_transit);
				//END Calculating walking/driving distance from the origin to the closest transit, from the closests transit to destination


			}

			template<typename ActivityItfType, typename ReturnType> ReturnType Choose_Mode(ActivityItfType activity)
			{
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
				scheduler_itf* scheduler = _Parent_Person->template Scheduling_Faculty<scheduler_itf*>();
				household_itf* _Parent_Household = _Parent_Person->person_itf::template Household<household_itf*>();
				person_static_properties_itf* person_properties = _Parent_Person->template Static_Properties<person_static_properties_itf*>();
				household_static_properties_itf* household_properties = _Parent_Household->template Static_Properties<household_static_properties_itf*>();		

				this->Define_Travel_Context(activity);
				
				if (person_properties->Age<int>() < 16) return Choose_Mode_For_Child<ActivityItfType, ReturnType>(activity);
				
				// create local choice model
				_Choice_Model_Interface* choice_model = (_Choice_Model_Interface*)Allocate<typename MasterType::nl_model_type>();

				// Allocate choice options
				_Mode_Choice_Option_Interface* auto_nest = (_Mode_Choice_Option_Interface*)Allocate<typename MasterType::mode_choice_option_type>();
				_Mode_Choice_Option_Interface* sov_choice = (_Mode_Choice_Option_Interface*)Allocate<typename MasterType::mode_choice_option_type>();
				_Mode_Choice_Option_Interface* taxi_choice = (_Mode_Choice_Option_Interface*)Allocate<typename MasterType::mode_choice_option_type>();
				_Mode_Choice_Option_Interface* hov_choice = (_Mode_Choice_Option_Interface*)Allocate<typename MasterType::mode_choice_option_type>();
				_Mode_Choice_Option_Interface* transit_choice = (_Mode_Choice_Option_Interface*)Allocate<typename MasterType::mode_choice_option_type>();
				_Mode_Choice_Option_Interface* pnr_choice = (_Mode_Choice_Option_Interface*)Allocate<typename MasterType::mode_choice_option_type>();
				_Mode_Choice_Option_Interface* nm_nest = (_Mode_Choice_Option_Interface*)Allocate<typename MasterType::mode_choice_option_type>();
				_Mode_Choice_Option_Interface* walk_choice = (_Mode_Choice_Option_Interface*)Allocate<typename MasterType::mode_choice_option_type>();
				_Mode_Choice_Option_Interface* bike_choice = (_Mode_Choice_Option_Interface*)Allocate<typename MasterType::mode_choice_option_type>();

				//============================================================================================
				// AUTO NEST
				auto_nest->Mode_Chooser(this);
				auto_nest->template mode_type<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::AUTO);
				if (_home_based && _to_work_school) auto_nest->inclusive_value_parameter(auto_nest->HBW_NEST_AUTO<float>());
				else if (_home_based) auto_nest->inclusive_value_parameter(auto_nest->HBO_NEST_AUTO<float>());
				else auto_nest->inclusive_value_parameter(auto_nest->NHB_NEST_AUTO<float>());
				choice_model->template Add_Choice_Option<_Choice_Option_Interface*>((_Choice_Option_Interface*)auto_nest);
				// SOV option
				if (_auto_available)
				{ 		
					sov_choice->Mode_Chooser(this);
					sov_choice->template mode_type<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
					auto_nest->template Add_Sub_Choice_Option<_Choice_Option_Interface*>((_Choice_Option_Interface*)sov_choice);
				}
				// Taxi option
				taxi_choice->Mode_Chooser(this);
				taxi_choice->template mode_type<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::TAXI);
				auto_nest->template Add_Sub_Choice_Option<_Choice_Option_Interface*>((_Choice_Option_Interface*)taxi_choice);
				// HOV option
				hov_choice->Mode_Chooser(this);
				hov_choice->template mode_type<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::HOV);
				auto_nest->template Add_Sub_Choice_Option<_Choice_Option_Interface*>((_Choice_Option_Interface*)hov_choice);			
				//============================================================================================
				// TRANSIT NEST
				//TODO: OMER - replace this check with the djikstra-based feasibility check with reasonable cutoffs
				
				/*if (_los->transit_ttime<Time_Minutes>() < 1440.0)
				{
					transit_choice->Mode_Chooser(this);
					transit_choice->template mode_type<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::BUS);
					choice_model->template Add_Choice_Option<_Choice_Option_Interface*>((_Choice_Option_Interface*)transit_choice);
				}*/
				
				//Obtain walk threshold in meters
				Meters walkThreshold = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::walkThreshold<float>();
				Meters walkThreshold_init = walkThreshold / 5.0; //in meters
				//Obtain walking distance to the closest transit stop
								
				
				if (walk_distance_to_transit<Meters>() < walkThreshold_init && walk_distance_after_transit<Meters>() < walkThreshold_init)
				{
					transit_choice->Mode_Chooser(this);
					transit_choice->template mode_type<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::BUS);
					choice_model->template Add_Choice_Option<_Choice_Option_Interface*>((_Choice_Option_Interface*)transit_choice);
				}
				else if (drive_fft_to_transit<Time_Seconds>() < _los->auto_ttime<Time_Seconds>()*0.75 && walk_distance_after_transit<Meters>() < walkThreshold_init)
				{
					pnr_choice->Mode_Chooser(this);
					pnr_choice->template mode_type<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::PARK_AND_RIDE);
					choice_model->template Add_Choice_Option<_Choice_Option_Interface*>((_Choice_Option_Interface*)pnr_choice);
				}

				//============================================================================================
				// Nonmotorized nest
				if (_los->auto_distance<Miles>() < 10.0)
				{
					nm_nest->Mode_Chooser(this);
					nm_nest->template mode_type<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::NONMOTORIZED);
					if (_home_based && _to_work_school) nm_nest->inclusive_value_parameter(nm_nest->HBW_NEST_NM<float>());
					else if (_home_based) nm_nest->inclusive_value_parameter(nm_nest->HBO_NEST_NM<float>());
					else nm_nest->inclusive_value_parameter(nm_nest->NHB_NEST_NM<float>());
					choice_model->template Add_Choice_Option<_Choice_Option_Interface*>((_Choice_Option_Interface*)nm_nest);
					// Walk option					
					
					//if (_los->auto_distance<Miles>() < 3.0)
					if (_los->auto_distance<Meters>() < walkThreshold)
					{
						walk_choice->Mode_Chooser(this);
						walk_choice->template mode_type<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::WALK);
						nm_nest->template Add_Sub_Choice_Option<_Choice_Option_Interface*>((_Choice_Option_Interface*)walk_choice);
					}
					// Bike option
					bike_choice->Mode_Chooser(this);
					bike_choice->template mode_type<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE);
					nm_nest->template Add_Sub_Choice_Option<_Choice_Option_Interface*>((_Choice_Option_Interface*)bike_choice);
				}


				// Make choice
				int selected_index = 0;
				choice_model->template Evaluate_Choices<NT>();

				_Choice_Option_Interface* selected = choice_model->template Choose<_Choice_Option_Interface*>(selected_index);
				Vehicle_Components::Types::Vehicle_Type_Keys selected_mode = Vehicle_Components::Types::Vehicle_Type_Keys::SOV;

				if (selected == nullptr) { THROW_WARNING("WARNING: selected is null - no mode choice made, defaulted to auto mode." << selected_index);}
				else selected_mode = ((_Mode_Choice_Option_Interface*)selected)->template mode_type<ReturnType>();
				
				//TODO OMER: Delete when done
				if (selected_mode == Vehicle_Components::Types::Vehicle_Type_Keys::BUS)
				{
					char myLine[2000];
					std::string bus_mode_paragraph;
					stringstream bus_mode_stream;

					sprintf_s(myLine, "%s\t%f\t%f\t%d\t%d\t%f\t%s\n",
						"Adult",
						walkThreshold_init,
						walk_distance_to_transit<Meters>(),
						_previous_location->template uuid<unsigned int>(),
						_destination->template uuid<unsigned int>(),
						activity->template Start_Time<Time_Minutes>(),
						"BUS");
					bus_mode_paragraph.insert(0, myLine);
					bus_mode_stream << bus_mode_paragraph;
					fw_bus_mode.Write_NoDelim(bus_mode_stream);
				}

				//TURN ON TO VALIDATE UTILITY CALCULATION AND RESULTS
			/*	if (__thread_id == 0) Print_Calculation_Results(choice_model, selected_mode);
				if (__thread_id == 0) Print_Selection(selected_mode);*/

/*				//============================================================================================
				//Account for touring - if previous act is not at an anchor location and not using auto, then auto not available
				//-need to update to disable auto mode when not available at the anchor location as well
				if (prev_location != _Parent_Person->template Home_Location<_Activity_Location_Interface*>() &&
					prev_location != _Parent_Person->template Work_Location<_Activity_Location_Interface*>() &&
					prev_location != _Parent_Person->template School_Location<_Activity_Location_Interface*>() &&
					prev_act != nullptr)
				{
					//if (prev_act->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == BUS || prev_act->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == HOV)
					//{
					//	if (selected_mode == Vehicle_Components::Types::Vehicle_Type_Keys::SOV) selected_mode = Vehicle_Components::Types::Vehicle_Type_Keys::HOV;
					//}
				}
*/
				// free memory allocated locally
				Free<typename _Mode_Choice_Option_Interface::Component_Type>((typename _Mode_Choice_Option_Interface::Component_Type*)nm_nest);
				Free<typename _Mode_Choice_Option_Interface::Component_Type>((typename _Mode_Choice_Option_Interface::Component_Type*)auto_nest);
				Free<typename _Mode_Choice_Option_Interface::Component_Type>((typename _Mode_Choice_Option_Interface::Component_Type*)sov_choice);
				Free<typename _Mode_Choice_Option_Interface::Component_Type>((typename _Mode_Choice_Option_Interface::Component_Type*)taxi_choice);
				Free<typename _Mode_Choice_Option_Interface::Component_Type>((typename _Mode_Choice_Option_Interface::Component_Type*)hov_choice);
				Free<typename _Mode_Choice_Option_Interface::Component_Type>((typename _Mode_Choice_Option_Interface::Component_Type*)transit_choice);
				Free<typename _Mode_Choice_Option_Interface::Component_Type>((typename _Mode_Choice_Option_Interface::Component_Type*)walk_choice);
				Free<typename _Mode_Choice_Option_Interface::Component_Type>((typename _Mode_Choice_Option_Interface::Component_Type*)bike_choice);
				Free<typename _Choice_Model_Interface::Component_Type>((typename _Choice_Model_Interface::Component_Type*)choice_model);

				if (_delete_los)
				{
					Free<typename MasterType::los_invariant_value_type>((typename typename MasterType::los_invariant_value_type*)_los->LOS_time_invariant<typename MasterType::los_invariant_value_type*>());
					Free<typename MasterType::los_value_type>((typename typename MasterType::los_value_type*)_los);
				}
				// return the chosen mode
				return selected_mode;
			}

			template<typename ActivityItfType, typename ReturnType> ReturnType Choose_Mode_For_Child(ActivityItfType activity)
			{
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
				scheduler_itf* scheduler = _Parent_Person->template Scheduling_Faculty<scheduler_itf*>();
				household_itf* _Parent_Household = _Parent_Person->person_itf::template Household<household_itf*>();
				person_static_properties_itf* person_properties = _Parent_Person->template Static_Properties<person_static_properties_itf*>();
				household_static_properties_itf* household_properties = _Parent_Household->template Static_Properties<household_static_properties_itf*>();
				Activity_Plan* cur_act = (Activity_Plan*)activity;

				// non-licensed drivers always use HOV	
				Kilometers dist = _los->auto_distance<Kilometers>();

				//Obtain walk threshold in meters
				Meters walkThreshold = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::walkThreshold<float>();
				Meters walkThreshold_init = walkThreshold / 5.0;
				
				float p_hov = 1.0;
				float p_bike = 0.0;
				float p_walk = 0.0;
				float p_bus = 0.0;
				float p_school = 0.0;

				// going to school activity
				if (this->_to_work_school || this->_from_work_school)
				{
					if (dist < 0.5) p_walk = 1.0;
					else if (dist < 1.0)
					{
						p_walk = 0.5;
						p_bike = 0.1;
						p_school = 0.4;
					}
					else p_school = 1.0;
				}
				else
				{
					if (dist < 0.5)
					{
						p_walk = 1.0;
					}
					else if (dist > 2.0)
					{
						if (person_properties->Age<int>() < 5)
						{
							p_hov = 1.0;
							p_walk = 0.0;
							p_bike = 0.0;
							p_bus = 0.0;
						}
						else if (person_properties->Age<int>() < 11)
						{
							p_hov = 0.82;
							p_walk = 0.1;
							p_bike = 0.01;
							p_bus = 1.0 - p_hov - p_walk - p_bike;
						}
						else
						{
							p_hov = 0.72;
							p_walk = 0.004;
							p_bike = 0.034;
							p_bus = 1.0 - p_hov - p_walk - p_bike;
						}

					}
					else
					{
						if (person_properties->Age<int>() < 5)
						{
							p_hov = 0.90;
							p_walk = 0.02;
							p_bike = 0.08;
							p_bus = 1.0 - p_hov - p_walk - p_bike;
						}
						else if (person_properties->Age<int>() < 11)
						{
							p_hov = 0.71;
							p_walk = 0.13;
							p_bike = 0.16;
							p_bus = 1.0 - p_hov - p_walk - p_bike;
						}
						else
						{
							p_hov = 0.52;
							p_walk = 0.14;
							p_bike = 0.26;
							p_bus = 1.0 - p_hov - p_walk - p_bike;
						}

					}

					if (walk_distance_to_transit<Meters>() >= walkThreshold_init || walk_distance_after_transit<Meters>() >= walkThreshold_init)
					{
						float p_hov_walk_bike = p_hov + p_walk + p_bike;
						p_hov = p_hov / p_hov_walk_bike;
						p_walk = p_walk / p_hov_walk_bike;
						p_bike = p_bike / p_hov_walk_bike;
						p_bus = 0;
					}
					

					if (GLOBALS::Length_Converter.Convert_Value<Kilometers, Meters>(dist) >= walkThreshold)
					{
						p_hov = 1.0;
						p_walk = 0.0;
						p_bike = 0.0;
						p_bus = 0.0;
					}
					
				}

				Vehicle_Components::Types::Vehicle_Type_Keys selected_mode;

				float r = GLOBALS::Uniform_RNG.Next_Rand<float>();
				if (r < p_hov) selected_mode = Vehicle_Components::Types::Vehicle_Type_Keys::HOV;
				else if(r < p_hov + p_walk) selected_mode = Vehicle_Components::Types::Vehicle_Type_Keys::WALK;
				else if(r < p_hov + p_walk + p_bike) selected_mode = Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE;
				else if (r < p_hov + p_walk + p_bike + p_bus) selected_mode = Vehicle_Components::Types::Vehicle_Type_Keys::BUS;
				else selected_mode = Vehicle_Components::Types::Vehicle_Type_Keys::SCHOOLBUS;

				//TODO OMER: Delete when done
				if (selected_mode == Vehicle_Components::Types::Vehicle_Type_Keys::BUS)
				{
					char myLine[2000];
					std::string bus_mode_paragraph;
					stringstream bus_mode_stream;

					sprintf_s(myLine, "%s\t%f\t%f\t%d\t%d\t%f\t%s\n",
						"Child",
						walkThreshold_init,
						walk_distance_to_transit<Meters>(),
						_previous_location->template uuid<unsigned int>(),
						_destination->template uuid<unsigned int>(),
						activity->template Start_Time<Time_Minutes>(),
						"BUS");
					bus_mode_paragraph.insert(0, myLine);
					bus_mode_stream << bus_mode_paragraph;
					fw_bus_mode.Write_NoDelim(bus_mode_stream);
				}

				if (_delete_los)
				{
					Free<typename MasterType::los_invariant_value_type>((typename typename MasterType::los_invariant_value_type*)_los->LOS_time_invariant<typename MasterType::los_invariant_value_type*>());
					Free<typename MasterType::los_value_type>((typename typename MasterType::los_value_type*)_los);
				}

				// return the chosen mode
				return selected_mode;
			}

			template<typename ActivityItfType> bool Assign_To_HOV(ActivityItfType activity, Vehicle_Components::Types::Vehicle_Type_Keys& selected_mode)
			{
				// if the mode is not chosen as an auto mode, return false
				if (selected_mode != Vehicle_Components::Types::Vehicle_Type_Keys::SOV) return false;

				// get interfaces
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
				person_static_properties_itf* person_properties = _Parent_Person->template Static_Properties<person_static_properties_itf*>();
				Activity_Plan* cur_act = (Activity_Plan*)activity;

				// non-licensed drivers always use HOV	
				if (person_properties->template Age<float>() < 16.0) return true;

				// probability of HOV
				float p = 0.0;

				// get probabilty for specific activity type
				switch (cur_act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>())
				{
				case Activity_Components::Types::AT_HOME_ACTIVITY: { p = 0.154065262221031; break;}
				case Activity_Components::Types::CHANGE_TRANSPORTATION_ACTIVITY: { p = 0.427794057766015; break;}
				case Activity_Components::Types::EAT_OUT_ACTIVITY: { p = 0.249301559829059; break;}
				case Activity_Components::Types::ERRANDS_ACTIVITY: { p = 0.107109608324519; break;}
				case Activity_Components::Types::HEALTHCARE_ACTIVITY: { p = 0.214534967014411; break;}
				case Activity_Components::Types::LEISURE_ACTIVITY: { p = 0.209992348215725; break;}
				case Activity_Components::Types::MAJOR_SHOPPING_ACTIVITY: { p = 0.200815449555537; break;}
				case Activity_Components::Types::OTHER_ACTIVITY: { p = 0.144108249493225; break;}
				case Activity_Components::Types::OTHER_SHOPPING_ACTIVITY: { p = 0.16716082963978; break;}
				case Activity_Components::Types::OTHER_WORK_ACTIVITY: { p = 0.040719107742865; break;}
				case Activity_Components::Types::PERSONAL_BUSINESS_ACTIVITY: { p = 0.17837940715925; break;}
				case Activity_Components::Types::PICK_UP_OR_DROP_OFF_ACTIVITY: { p = 0.0739687591191153; break;}
				case Activity_Components::Types::PRIMARY_WORK_ACTIVITY: { p = 0.0637426142432775; break;}
				case Activity_Components::Types::PART_TIME_WORK_ACTIVITY: { p = 0.0637426142432775; break;}
				case Activity_Components::Types::RELIGIOUS_OR_CIVIC_ACTIVITY: { p = 0.300601437534384; break;}
				case Activity_Components::Types::SCHOOL_ACTIVITY: { p = 0.228655158483933; break;}
				case Activity_Components::Types::SERVICE_VEHICLE_ACTIVITY: { p = 0.145728864260415; break;}
				case Activity_Components::Types::SOCIAL_ACTIVITY: { p = 0.224093784698896; break;}
				case Activity_Components::Types::WORK_AT_HOME_ACTIVITY: { p = 0.0294387874193491; break;}
				default:
					p = 0.149;
				}

				// Return true if the person uses HOV mode according to random draw
				if (GLOBALS::Uniform_RNG.Next_Rand<float>() < p) return true;
				else return false;

			}
			template<typename LocItfType, typename ActivityItfType> bool Assign_To_Walk(LocItfType orig, LocItfType dest, ActivityItfType act)
			{
				_Activity_Location_Interface* O = (_Activity_Location_Interface*)orig;
				_Activity_Location_Interface* D = (_Activity_Location_Interface*)dest;

				Meters x1 = O->template x_position<Meters>();
				Meters y1 = O->template y_position<Meters>();

				Meters x2 = D->template x_position<Meters>();
				Meters y2 = D->template y_position<Meters>();

				_Zone_Interface* dest_zone = D->template zone<_Zone_Interface*>();

				// Set the maximum allowable walk distance based on area type - higher in CBD/downtown
				Kilometers max_walk_distance = 0;
				if (dest_zone->template areatype<int>() == 1) max_walk_distance = 1.0;			// CBD
				else if (dest_zone->template areatype<int>() == 2) max_walk_distance = 0.5;	// Downtown
				else if (dest_zone->template areatype<int>() == 3) max_walk_distance = 0.25;		// Rest of chicago
				else if (dest_zone->template areatype<int>() >= 4 && dest_zone->template areatype<int>() >= 6) max_walk_distance = 0.25; // Suburban
				else if (dest_zone->template areatype<int>() == 7) max_walk_distance = 0.0;	// Exurb
				else max_walk_distance = 0.0;											// Rural


				Meters dist = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
				Kilometers dist_km = GLOBALS::Length_Converter.Convert_Value<Meters, Kilometers>(dist);


				if (dist < max_walk_distance) return true;
				else return false;
			}

			void Print_Selection(Vehicle_Components::Types::Vehicle_Type_Keys selected_mode)
			{
				LOCK(this->_update_lock);
				MESSAGE("MODE_CHOICE," << selected_mode << ",ACTTYPE," << _current_activity->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>()<<endl<<endl);
				UNLOCK(this->_update_lock);
			}

			void Print_Calculation_Results(_Choice_Model_Interface* choice_model, Vehicle_Components::Types::Vehicle_Type_Keys selected_mode)
			{
				LOCK(this->_update_lock);


				float utility = -1.0*FLT_MAX;

				typedef Random_Access_Sequence<typename _Choice_Model_Interface::get_type_of(choice_options)> choice_options_itf;
				typedef Choice_Model_Components::Prototypes::Choice_Option<get_component_type(choice_options_itf)> choice_option_itf;
				typedef Random_Access_Sequence<typename choice_option_itf::get_type_of(sub_choice_options)> subchoice_options_itf;
				typedef Choice_Model_Components::Prototypes::Choice_Option<get_component_type(subchoice_options_itf)> subchoice_option_itf;

				person_itf* _Parent_Person = this->_Parent_Planner->template Parent_Person<person_itf*>();
				household_itf* household = _Parent_Person->Household<household_itf*>();
				household_static_properties_itf* hh_properties = household->Static_Properties<household_static_properties_itf*>();
				scheduler_itf* scheduler = _Parent_Person->template Scheduling_Faculty<scheduler_itf*>();
				person_static_properties_itf* properties = _Parent_Person->template Static_Properties<person_static_properties_itf*>();
				_Zone_Interface* dest_zone = this->destination<_Activity_Location_Interface*>()->template zone<_Zone_Interface*>();
				_Zone_Interface* orig_zone = this->previous_location<_Activity_Location_Interface*>()->template zone<_Zone_Interface*>();
				Activity_Plan* current_activity = this->current_activity<Activity_Plan*>();
				Activity_Components::Types::ACTIVITY_TYPES activity_type = current_activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();

				// calculate utilty model variable requirements
				Time_Minutes start = current_activity->Start_Time<Time_Minutes>();
				double h_inc = hh_properties->Income<Basic_Units::Currency_Variables::Dollars>() > 100000 ? 1.0 : 0.0;
				double m_inc = hh_properties->Income<Basic_Units::Currency_Variables::Dollars>() > 50000 && h_inc == 0.0 ? 1.0 : 0.0;
				double PEAK = (start > 420 && start < 600) || (start > 900 && start < 1080) ? 1.0 : 0.0;
				double AUTO_TT = _los->auto_ttime<Time_Minutes>();
				double AUTO_COST = _los->auto_distance<Miles>() * 0.2;
				double AUTO_COST_M = AUTO_COST * m_inc;
				double AUTO_COST_H = AUTO_COST * h_inc;
				double PASS_COST = 0.5 * AUTO_COST;
				double PASS_COST_M = AUTO_COST_M;
				double PASS_COST_H = AUTO_COST_H;
				double TAXI_COST = _los->auto_distance<Miles>() * 2.25 + 3.0;
				double TAXI_COST_M = AUTO_COST * m_inc;
				double TAXI_COST_H = AUTO_COST * h_inc;
				double POP_DENS = log(max(1.0, (dest_zone->population_density<Basic_Units::Area_Variables::Square_Kilometers, double>() + orig_zone->population_density<Basic_Units::Area_Variables::Square_Kilometers, double>()) / 2.0));
				double BIKE_TTr = _los->auto_distance<Miles>() / 10.0 * 60.0 - AUTO_TT;
				double WALK_TTr = _los->auto_distance<Miles>() / 3.0 * 60.0 - AUTO_TT;
				double TRAN_IVTTr = _los->transit_ttime<Time_Minutes>() - AUTO_TT;
				double TRAN_WAIT = _los->transit_wait_time<Time_Minutes>();
				double TRAN_COST = _los->transit_fare<Dollars>();
				double TRAN_COST_M = TRAN_COST * m_inc;
				double TRAN_COST_H = TRAN_COST * h_inc;
				double TRAN_OVTT = _los->transit_walk_access_time<Time_Minutes>();
				double GENDER = properties->Gender<Person_Components::Types::GENDER>() == Person_Components::Types::GENDER::MALE ? 1.0 : 0.0;
				double VEHPERLIC = hh_properties->Number_of_workers<double>() > 0 ? hh_properties->Number_of_vehicles<double>() / hh_properties->Number_of_workers<double>() : 0.0;
				double AGE65 = properties->Age<int>() >= 65 ? 1.0 : 0.0;
				double IN_CBD = _to_CBD ? 1.0 : 0.0;
				double NOTALONE = 0.0;
				double AGEU18 = properties->Age<int>() <18 ? 1.0 : 0.0;
				double ONE = 1.0;

				// print input values: hhid,mode,h_inc,m_inc,peak,auto_tt,auto_cost,taxi_cost,popdens,travivtt,tranovtt,tranwait,trancost,walkttr,gen,vehperlic,age65,incbd,notalone,ageu18,
				cout <<"DATA:,"<<household->uuid<int>() << ","<<orig_zone->uuid<int>()<<"-"<<dest_zone->uuid<int>()<<"," << selected_mode << "," << h_inc << "," <<m_inc << "," <<PEAK << "," << "," << AUTO_TT << "," << AUTO_COST << "," <<TAXI_COST << "," <<POP_DENS << "," << TRAN_IVTTr << "," << TRAN_OVTT << "," << TRAN_WAIT << "," << TRAN_COST << "," << WALK_TTr << "," << GENDER << "," << VEHPERLIC << "," << AGE65 << "," << IN_CBD << "," << NOTALONE << "," << AGEU18<<endl;
				// print available modes
				choice_options_itf* choices = choice_model->choice_options<choice_options_itf*>();
				choice_options_itf::iterator itr = choices->begin();
				cout << ":OPTIONS:,";
				for (itr; itr != choices->end(); ++itr)
				{
					_Mode_Choice_Option_Interface* choice = (_Mode_Choice_Option_Interface*)(*itr);
					cout << choice->mode_type<Vehicle_Components::Types::Vehicle_Type_Keys>() << ",";

					subchoice_options_itf* subchoices = choice->sub_choice_options<subchoice_options_itf*>();
					subchoice_options_itf::iterator sitr = subchoices->begin();
					for (sitr; sitr != subchoices->end(); ++sitr)
					{
						_Mode_Choice_Option_Interface* subchoice = (_Mode_Choice_Option_Interface*)(*sitr);
						cout << subchoice->mode_type<Vehicle_Components::Types::Vehicle_Type_Keys>() << ",";
					}
					cout << endl;
				}
				cout << ":Utility/Probability:,";
				// print utility/probability for each mode
				choice_model->Print_Values<NT>();
				cout << endl;

				UNLOCK(this->_update_lock);
			}
		};





//#pragma region	******************** OLD CMAP-version MODE CHOICE CODE - DEPRECATED **************************
//		//==================================================================================
//		/// Mode Choice Planning classes
//		//----------------------------------------------------------------------------------
//		implementation struct Mode_Choice_Option : public Choice_Model_Components::Implementations::Choice_Option_Base<MasterType, INHERIT(Mode_Choice_Option)>
//		{
//			// Tag as Implementation
//			typedef typename Polaris_Component<MasterType, INHERIT(Mode_Choice_Option), Data_Object>::Component_Type ComponentType;
//
//
//			// data members
//			m_prototype(Prototypes::Mode_Chooser, typename MasterType::person_mode_chooser_type, Mode_Chooser, NONE, NONE);
//			m_data(Vehicle_Components::Types::Vehicle_Type_Keys, mode_type, NONE, NONE);
//
//			// PARAMETER DECLARATIONS
//#pragma region static parameters
//			m_static_data(float, BHW_CBD_IVTT, NONE, NONE);
//			m_static_data(float, BHO_IVTT, NONE, NONE);
//			m_static_data(float, BHO_CBD_IVTT, NONE, NONE);
//			m_static_data(float, BNH_IVTT, NONE, NONE);
//			m_static_data(float, BNH_CBD_IVTT, NONE, NONE);
//			m_static_data(float, BHW_IVTT, NONE, NONE);
//
//			m_static_data(float, BHW_WAIT, NONE, NONE);
//			m_static_data(float, BHW_CBD_WAIT, NONE, NONE);
//			m_static_data(float, BHO_WAIT, NONE, NONE);
//			m_static_data(float, BHO_CBD_WAIT, NONE, NONE);
//			m_static_data(float, BNH_WAIT, NONE, NONE);
//			m_static_data(float, BNH_CBD_WAIT, NONE, NONE);
//
//			m_static_data(float, BHW_TRANSFER, NONE, NONE);
//			m_static_data(float, BHW_CBD_TRANSFER, NONE, NONE);
//			m_static_data(float, BHO_TRANSFER, NONE, NONE);
//			m_static_data(float, BHO_CBD_TRANSFER, NONE, NONE);
//			m_static_data(float, BNH_TRANSFER, NONE, NONE);
//			m_static_data(float, BNH_CBD_TRANSFER, NONE, NONE);
//
//			m_static_data(float, BHW_WALK, NONE, NONE);
//			m_static_data(float, BHW_CBD_WALK, NONE, NONE);
//			m_static_data(float, BHO_WALK, NONE, NONE);
//			m_static_data(float, BHO_CBD_WALK, NONE, NONE);
//			m_static_data(float, BNH_WALK, NONE, NONE);
//			m_static_data(float, BNH_CBD_WALK, NONE, NONE);
//
//			m_static_data(float, BHW_COST, NONE, NONE);
//			m_static_data(float, BHW_CBD_COST, NONE, NONE);
//			m_static_data(float, BHO_COST, NONE, NONE);
//			m_static_data(float, BHO_CBD_COST, NONE, NONE);
//			m_static_data(float, BNH_COST, NONE, NONE);
//			m_static_data(float, BNH_CBD_COST, NONE, NONE);
//
//			m_static_data(float, BHW_BIAS, NONE, NONE);
//			m_static_data(float, BHW_CBD_BIAS, NONE, NONE);
//			m_static_data(float, BHO_BIAS, NONE, NONE);
//			m_static_data(float, BHO_CBD_BIAS, NONE, NONE);
//			m_static_data(float, BNH_BIAS, NONE, NONE);
//			m_static_data(float, BNH_CBD_BIAS, NONE, NONE);
//#pragma endregion
//
//			//====================================================================================================================================
//			// Interface definitions
//			typedef Prototypes::Person_Planner<typename type_of(Mode_Chooser)::type_of(Parent_Planner)> planner_itf;
//			typedef Prototypes::Person<typename planner_itf::get_type_of(Parent_Person)> person_itf;
//			typedef Prototypes::Person_Properties<typename person_itf::get_type_of(Properties)> person_properties_itf;
//			typedef Household_Components::Prototypes::Household<typename person_itf::get_type_of(Household)> household_itf;
//			typedef Household_Components::Prototypes::Household_Properties<typename household_itf::get_type_of(Static_Properties)> household_static_properties_itf;
//			typedef Vehicle_Components::Prototypes::Vehicle<typename person_itf::get_type_of(vehicle)> vehicle_itf;
//			typedef Prototypes::Person_Scheduler<typename person_itf::get_type_of(Scheduling_Faculty)> scheduler_itf;
//			typedef Scenario_Components::Prototypes::Scenario< typename person_itf::get_type_of(scenario_reference)> _Scenario_Interface;
//			typedef Network_Components::Prototypes::Network< typename person_itf::get_type_of(network_reference)> _Network_Interface;
//			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _Network_Interface::get_type_of(skimming_faculty)> _Skim_Interface;
//
//			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
//			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(_Activity_Locations_Container_Interface)>  _Activity_Location_Interface;
//
//			typedef Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links)> _Links_Container_Interface;
//			typedef Link_Components::Prototypes::Link<get_component_type(_Links_Container_Interface)>  _Link_Interface;
//
//			typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
//			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_Zones_Container_Interface)>  _Zone_Interface;
//
//			//typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Activity_Container)> Activity_Plans;
//			//typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Plans)> Activity_Plan;
//
//			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Movement_Plans_Container)> Movement_Plans;
//			typedef Movement_Plan_Components::Prototypes::Movement_Plan<get_component_type(Movement_Plans)> Movement_Plan;
//
//
//			typedef Network_Skimming_Components::Prototypes::LOS<typename MasterType::los_value_type> los_itf;
//			//------------------------------------------------------------------------------------------------------------------------------------
//
//			static bool static_initialize(const string& option_file);
//			static void print_parameters();
//			static void default_static_initializer();
//
//			// Feature called from prototype and by Choice_Model
//			virtual double Calculate_Utility();
//
//			virtual void Print_Utility();
//
//			// Local features
//			template<typename TargetType> TargetType Calculate_Utility_For_Known_Location();
//
//			//template<typename TargetType>
//			//TargetType Calculate_Utility_For_Unknown_Location()
//			//{		
//			//	los_itf* los = _Mode_Chooser->los<los_itf*>();
//			//	person_itf* _Parent_Person = this->_Mode_Chooser->Parent_Planner<planner_itf*>()->template Parent_Person<person_itf*>();
//			//	household_itf* household = _Parent_Person->Household<household_itf*>();
//			//	household_properties_itf* hh_properties = household->Static_Properties<household_properties_itf*>();
//			//	scheduler_itf* scheduler = _Parent_Person->template Scheduling_Faculty<scheduler_itf*>();
//			//	person_properties_itf* properties = _Parent_Person->template Static_Properties<person_properties_itf*>();
//			//	vehicle_itf* vehicle = _Parent_Person->template vehicle<vehicle_itf*>();
//			//	_Zone_Interface* dest_zone = _Mode_Chooser->destination<_Activity_Location_Interface*>()->template zone<_Zone_Interface*>();
//			//	_Zone_Interface* origin_zone = _Mode_Chooser->previous_location<_Activity_Location_Interface*>()->template zone<_Zone_Interface*>();
//			//	Activity_Plan* current_activity = _Mode_Chooser->current_activity<Activity_Plan*>();
//			//	Activity_Components::Types::ACTIVITY_TYPES activity_type = current_activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();
//			//
//			//
//			//	// account for any VOT changes for this individual (i.e. from CAVS)
//			//	//person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
//			//	//person_properties_itf* properties = _Parent_Person->template Properties<person_properties_itf*>();
//			//	float vot_change = properties->template Value_of_Travel_Time_Adjustment<float>();
//			//
//			//	//_Zone_Interface* origin_zone = _previous_location->template zone<_Zone_Interface*>();
//			//	float ivtt_dif = origin_zone->template avg_ttime_auto_to_transit_accessible_zones<Time_Minutes>() * vot_change - origin_zone->template avg_ttime_transit<Time_Minutes>();
//			//	float wait_dif = -5.0; // assumed wait time of 5 minutes for transit trips
//			//	float transfer_dif = 0;
//			//	float walk_time_dif = -5.0; // assumed average walk time of 5 minutes for transit trips, given walk speed of 3 mph and max distance of 0.5 miles
//			//	float cost_dif = 0;
//			//	float utility = -1.0 * FLT_MAX;
//			//
//			//	// modify the values if no auto in the household (i.e. auto mode becomes like carpool with wait times, walk times, transfer time)
//			//	if (!_Mode_Chooser->auto_available<bool>())
//			//	{
//			//		wait_dif += 30.0;
//			//		transfer_dif += 2.0;
//			//		walk_time_dif += 2.0;
//			//	}
//			//
//			//	// If the transit mode is  accessible from the current zone, calculate utility, otherwise utility is flt_max		
//			//	if (origin_zone->template avg_ttime_transit<Time_Days>() < 1.0)
//			//		//%%%RLW utility = this->template Calculate_Utility_Value(ivtt_dif, wait_dif, transfer_dif, walk_time_dif, cost_dif);
//			//		utility = this->Calculate_Utility_Value(ivtt_dif, wait_dif, transfer_dif, walk_time_dif, cost_dif);
//			//	else
//			//		return utility;
//			//
//			//	if (utility > 100.0) THROW_WARNING("WARNING: utility > 200.0 will cause numeric overflow, possible misspecification in utility function for mode choice (ivtt,wait,transfer,walk,cost): "<<ivtt_dif<<","<<wait_dif<<","<<transfer_dif<<","<<walk_time_dif<<","<<cost_dif)
//			//	
//			//	//cout << "O/D=unknown:"<<_previous_location->zone<_Zone_Interface*>()->uuid<int>() <<"/xxx";
//			//	//cout << ",Auto_TT:"<<origin_zone->avg_ttime_auto_to_transit_accessible_zones<Time_Minutes>()<< ",Transit_TT:"<<origin_zone->avg_ttime_transit<Time_Minutes>();
//			//	//cout <<",Transit_wait:" << -1.0* wait_dif << ",transfer_time:"<<0;
//			//	//cout <<",Auto_cost:" << 0;
//			//	//cout <<",Transit_fare:" << 0 << ",utility:"<< utility << endl;
//			//
//			//	// Otherwise return the transit utility based on Zonal average characteristics
//			//	return utility;			
//			//}
//
//			float Calculate_Utility_Value(float ivtt_dif, float wait_dif, float transfer_dif, float walk_time_dif, float cost_dif);
//
//		};
//
//		template<typename MasterType, typename InheritanceList>
//		bool Mode_Choice_Option<MasterType, InheritanceList>::static_initialize(const string& option_file)
//		{
//			// set the base values
//			default_static_initializer();
//
//			// now see if there are config file changes
//			if (option_file.length() < 1)
//			{
//				cout << "Warning: option file for Mode_Choice_Option was not specified" << endl;
//				return true;
//			}
//
//			rapidjson::Document document;
//			_Scenario_Interface* scenario = static_cast<_Scenario_Interface*>(_global_scenario);
//			if (!scenario->parse_option_file(document, option_file))
//				return false;
//
//			string section = "Mode_Choice_Option";
//
//			scenario->set_parameter(document, section, "BHW_CBD_IVTT", _BHW_CBD_IVTT);
//			scenario->set_parameter(document, section, "BHO_IVTT", _BHO_IVTT);
//			scenario->set_parameter(document, section, "BHO_CBD_IVTT", _BHO_CBD_IVTT);
//			scenario->set_parameter(document, section, "BNH_IVTT", _BNH_IVTT);
//			scenario->set_parameter(document, section, "BNH_CBD_IVTT", _BNH_CBD_IVTT);
//			scenario->set_parameter(document, section, "BHW_IVTT", _BHW_IVTT);
//			scenario->set_parameter(document, section, "BHW_WAIT", _BHW_WAIT);
//			scenario->set_parameter(document, section, "BHW_CBD_WAIT", _BHW_CBD_WAIT);
//			scenario->set_parameter(document, section, "BHO_WAIT", _BHO_WAIT);
//			scenario->set_parameter(document, section, "BHO_CBD_WAIT", _BHO_CBD_WAIT);
//			scenario->set_parameter(document, section, "BNH_WAIT", _BNH_WAIT);
//			scenario->set_parameter(document, section, "BNH_CBD_WAIT", _BNH_CBD_WAIT);
//			scenario->set_parameter(document, section, "BHW_TRANSFER", _BHW_TRANSFER);
//			scenario->set_parameter(document, section, "BHW_CBD_TRANSFER", _BHW_CBD_TRANSFER);
//			scenario->set_parameter(document, section, "BHO_TRANSFER", _BHO_TRANSFER);
//			scenario->set_parameter(document, section, "BHO_CBD_TRANSFER", _BHO_CBD_TRANSFER);
//			scenario->set_parameter(document, section, "BNH_TRANSFER", _BNH_TRANSFER);
//			scenario->set_parameter(document, section, "BNH_CBD_TRANSFER", _BNH_CBD_TRANSFER);
//			scenario->set_parameter(document, section, "BHW_WALK", _BHW_WALK);
//			scenario->set_parameter(document, section, "BHW_CBD_WALK", _BHW_CBD_WALK);
//			scenario->set_parameter(document, section, "BHO_WALK", _BHO_WALK);
//			scenario->set_parameter(document, section, "BHO_CBD_WALK", _BHO_CBD_WALK);
//			scenario->set_parameter(document, section, "BNH_WALK", _BNH_WALK);
//			scenario->set_parameter(document, section, "BNH_CBD_WALK", _BNH_CBD_WALK);
//			scenario->set_parameter(document, section, "BHW_COST", _BHW_COST);
//			scenario->set_parameter(document, section, "BHW_CBD_COST", _BHW_CBD_COST);
//			scenario->set_parameter(document, section, "BHO_COST", _BHO_COST);
//			scenario->set_parameter(document, section, "BHO_CBD_COST", _BHO_CBD_COST);
//			scenario->set_parameter(document, section, "BNH_COST", _BNH_COST);
//			scenario->set_parameter(document, section, "BNH_CBD_COST", _BNH_CBD_COST);
//			scenario->set_parameter(document, section, "BHW_BIAS", _BHW_BIAS);
//			scenario->set_parameter(document, section, "BHW_CBD_BIAS", _BHW_CBD_BIAS);
//			scenario->set_parameter(document, section, "BHO_BIAS", _BHO_BIAS);
//			scenario->set_parameter(document, section, "BHO_CBD_BIAS", _BHO_CBD_BIAS);
//			scenario->set_parameter(document, section, "BNH_BIAS", _BNH_BIAS);
//			scenario->set_parameter(document, section, "BNH_CBD_BIAS", _BNH_CBD_BIAS);
//
//			return true;
//		}
//
//
//		template<typename MasterType, typename InheritanceList>
//		void Mode_Choice_Option<MasterType, InheritanceList>::print_parameters()
//		{
//			cout << "Mode_Choice_Option parameters" << endl;
//			cout << "\tBHW_CBD_IVTT = " << BHW_CBD_IVTT	<float>() << endl;
//			cout << "\tBHO_IVTT = " << BHO_IVTT		<float>() << endl;
//			cout << "\tBHO_CBD_IVTT = " << BHO_CBD_IVTT	<float>() << endl;
//			cout << "\tBNH_IVTT = " << BNH_IVTT		<float>() << endl;
//			cout << "\tBNH_CBD_IVTT = " << BNH_CBD_IVTT	<float>() << endl;
//			cout << "\tBHW_IVTT = " << BHW_IVTT		<float>() << endl;
//			cout << "\tBHW_WAIT = " << BHW_WAIT		<float>() << endl;
//			cout << "\tBHW_CBD_WAIT = " << BHW_CBD_WAIT	<float>() << endl;
//			cout << "\tBHO_WAIT = " << BHO_WAIT		<float>() << endl;
//			cout << "\tBHO_CBD_WAIT = " << BHO_CBD_WAIT	<float>() << endl;
//			cout << "\tBNH_WAIT = " << BNH_WAIT		<float>() << endl;
//			cout << "\tBNH_CBD_WAIT = " << BNH_CBD_WAIT	<float>() << endl;
//			cout << "\tBHW_TRANSFER = " << BHW_TRANSFER	<float>() << endl;
//			cout << "\tBHW_CBD_TRANSFER = " << BHW_CBD_TRANSFER<float>() << endl;
//			cout << "\tBHO_TRANSFER = " << BHO_TRANSFER	<float>() << endl;
//			cout << "\tBHO_CBD_TRANSFER = " << BHO_CBD_TRANSFER<float>() << endl;
//			cout << "\tBNH_TRANSFER = " << BNH_TRANSFER	<float>() << endl;
//			cout << "\tBNH_CBD_TRANSFER = " << BNH_CBD_TRANSFER<float>() << endl;
//			cout << "\tBHW_WALK = " << BHW_WALK		<float>() << endl;
//			cout << "\tBHW_CBD_WALK = " << BHW_CBD_WALK	<float>() << endl;
//			cout << "\tBHO_WALK = " << BHO_WALK		<float>() << endl;
//			cout << "\tBHO_CBD_WALK = " << BHO_CBD_WALK	<float>() << endl;
//			cout << "\tBNH_WALK = " << BNH_WALK		<float>() << endl;
//			cout << "\tBNH_CBD_WALK = " << BNH_CBD_WALK	<float>() << endl;
//			cout << "\tBHW_COST = " << BHW_COST		<float>() << endl;
//			cout << "\tBHW_CBD_COST = " << BHW_CBD_COST	<float>() << endl;
//			cout << "\tBHO_COST = " << BHO_COST		<float>() << endl;
//			cout << "\tBHO_CBD_COST = " << BHO_CBD_COST	<float>() << endl;
//			cout << "\tBNH_COST = " << BNH_COST		<float>() << endl;
//			cout << "\tBNH_CBD_COST = " << BNH_CBD_COST	<float>() << endl;
//			cout << "\tBHW_BIAS = " << BHW_BIAS		<float>() << endl;
//			cout << "\tBHW_CBD_BIAS = " << BHW_CBD_BIAS	<float>() << endl;
//			cout << "\tBHO_BIAS = " << BHO_BIAS		<float>() << endl;
//			cout << "\tBHO_CBD_BIAS = " << BHO_CBD_BIAS	<float>() << endl;
//			cout << "\tBNH_BIAS = " << BNH_BIAS		<float>() << endl;
//			cout << "\tBNH_CBD_BIAS = " << BNH_CBD_BIAS	<float>() << endl;
//		}
//
//		template<typename MasterType, typename InheritanceList>
//		double Mode_Choice_Option<MasterType, InheritanceList>::Calculate_Utility()
//		{
//			double utility;
//
//			// Since the model is bimodal, treat SOV as the reference class
//			if (this->_mode_type == Vehicle_Components::Types::Vehicle_Type_Keys::SOV)
//			{
//				utility = 0.0;
//			}
//			else
//			{
//				utility = this->template Calculate_Utility_For_Known_Location<double>();
//
//				//// Determine if the trip is from home
//				//person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
//				//if (_previous_location == _Parent_Person->template Home_Location<_Activity_Location_Interface*>()) _home_based=true;
//				//else _home_based = false;
//
//				//// Determine if the trip destination is known, and if it is destined for the CBD
//				//if (this->_current_activity->template Location_Is_Planned<bool>())
//				//{
//				//	_Zone_Interface* dest_zone = this->_destination->template zone<_Zone_Interface*>();
//				//	if (dest_zone->template uuid<int>() < 88) _to_CBD = true;
//				//	else _to_CBD = false;
//				//	
//				//	utility = this->template Calculate_Utility_For_Known_Location<double>();
//				//}
//				//else 
//				//{
//				//	_to_CBD = false;
//				//	utility = this->template Calculate_Utility_For_Unknown_Location<double>();
//				//}
//			}
//			return utility;
//		}
//
//		template<typename MasterType, typename InheritanceList>
//		void Mode_Choice_Option<MasterType, InheritanceList>::Print_Utility()
//		{
//			//return 0.0;
//		}
//
//		template<typename MasterType, typename InheritanceList>
//		template<typename TargetType>
//		TargetType Mode_Choice_Option<MasterType, InheritanceList>::Calculate_Utility_For_Known_Location()
//		{
//			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Activity_Container)> Activity_Plans;
//			typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Plans)> Activity_Plan;
//			//typedef std::list<Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type>*> Activity_Plans;
//			//typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type> Activity_Plan;
//
//			los_itf* los = _Mode_Chooser->los<los_itf*>();
//			person_itf* _Parent_Person = this->_Mode_Chooser->Parent_Planner<planner_itf*>()->template Parent_Person<person_itf*>();
//			household_itf* household = _Parent_Person->Household<household_itf*>();
//			household_static_properties_itf* hh_properties = household->Static_Properties<household_static_properties_itf*>();
//			scheduler_itf* scheduler = _Parent_Person->template Scheduling_Faculty<scheduler_itf*>();
//			person_properties_itf* properties = _Parent_Person->template Properties<person_properties_itf*>();
//			vehicle_itf* vehicle = _Parent_Person->template vehicle<vehicle_itf*>();
//			_Zone_Interface* dest_zone = _Mode_Chooser->destination<_Activity_Location_Interface*>()->template zone<_Zone_Interface*>();
//			_Zone_Interface* orig_zone = _Mode_Chooser->previous_location<_Activity_Location_Interface*>()->template zone<_Zone_Interface*>();
//			Activity_Plan* current_activity = _Mode_Chooser->current_activity<Activity_Plan*>();
//			Activity_Components::Types::ACTIVITY_TYPES activity_type = current_activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();
//
//
//			//person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
//			//scheduler_itf* scheduler = _Parent_Person->template Scheduling_Faculty<scheduler_itf*>();
//			//person_properties_itf* properties = _Parent_Person->template Properties<person_properties_itf*>();
//			//vehicle_itf* vehicle = _Parent_Person->template vehicle<vehicle_itf*>();
//
//			//// external knowledge references
//			//_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
//			//_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();
//			//_Activity_Locations_Container_Interface* locations = network->template activity_locations_container<_Activity_Locations_Container_Interface*>();
//			//_Skim_Interface* skim = network->template skimming_faculty<_Skim_Interface*>();
//			//_Zone_Interface* dest_zone = _destination->template zone<_Zone_Interface*>();
//
//			//// get the travel time to the destination
//			//los_itf* los;
//			//if (_current_activity->template Start_Is_Planned<bool>()) los = skim->template Get_LOS<_Activity_Location_Interface*, Time_Seconds,los_itf*>(_previous_location,_destination, _current_activity->template Start_Time<Time_Seconds>());
//			//else los = skim->template Get_LOS<_Activity_Location_Interface*, Time_Hours,los_itf*>(_previous_location,_destination, 12.0);
//
//			// account for any VOT changes for this individual (i.e. from CAVS)
//			float vot_change = properties->template Value_of_Travel_Time_Adjustment<float>();
//			float parking_cost_factor = 1.0;
//			//if (vehicle->template is_autonomous<bool>()) parking_cost_factor = 0.0;
//
//			// Get the differences in characteristics for transit compared to auto mode (CMAP model specified as difference)
//			float ivtt_dif = los->template auto_ttime<Time_Minutes>()*vot_change - los->template transit_ttime<Time_Minutes>();
//			float wait_dif = -1.0 * los->template transit_wait_time<Time_Minutes>();
//			float transfer_dif = -1.0 * los->template transit_walk_access_time<Time_Minutes>();
//			if (los->template transit_walk_access_time<Time_Minutes>() > 60.0) transfer_dif = 0;
//			float walk_time_dif = -4.0; // * los->transit_walk_access_time<Time_Minutes>();
//			float cost_dif = los->template auto_distance<Miles>()*20.0 + dest_zone->template Parking_Cost<Cents>()*parking_cost_factor + los->template auto_tolls<Cents>() - los->template transit_fare<Cents>();
//
//			// modify the values if no auto in the household (i.e. auto mode becomes like carpool with wait times, walk times, transfer time)
//			if (!_Mode_Chooser->auto_available<bool>())
//			{
//				wait_dif += 30.0;
//				transfer_dif += 2.0;
//				walk_time_dif += 2.0;
//			}
//
//			if (abs(ivtt_dif) > 1440) return -FLT_MAX;
//			float u = Calculate_Utility_Value(ivtt_dif, wait_dif, transfer_dif, walk_time_dif, cost_dif);
//			if (u > 100.0) THROW_WARNING("WARNING: utility > 200.0 will cause numeric overflow, possible misspecification in utility function for mode choice (ivtt,wait,transfer,walk,cost): " << ivtt_dif << "," << wait_dif << "," << transfer_dif << "," << walk_time_dif << "," << cost_dif);
//
//
//			//cout << "O/D:"<<_previous_location->zone<_Zone_Interface*>()->uuid<int>() <<"/"<<_destination->zone<_Zone_Interface*>()->uuid<int>();
//			//cout << ",Auto_TT:"<<los->auto_ttime<Time_Minutes>() << ",Transit_TT:"<<los->transit_ttime<Time_Minutes>();
//			//cout <<",Transit_wait:" << -1.0* wait_dif << ",transfer_time:"<<-1.0 *transfer_dif;
//			//cout <<",Auto_cost:" << los->auto_distance<Miles>()*15.0 + dest_zone->Parking_Cost<Cents>() + los->auto_tolls<Cents>();
//			//cout <<",Transit_fare:" << los->transit_fare<Cents>() << ",utility:"<< u << endl;
//
//			return (TargetType)u;
//		}
//
//		template<typename MasterType, typename InheritanceList>
//		float Mode_Choice_Option<MasterType, InheritanceList>::Calculate_Utility_Value(float ivtt_dif, float wait_dif, float transfer_dif, float walk_time_dif, float cost_dif)
//		{
//			typedef std::list<Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type>*> Activity_Plans;
//			typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type> Activity_Plan;
//
//			float utility;
//
//			Activity_Components::Types::ACTIVITY_TYPES activity_type = _Mode_Chooser->current_activity<Activity_Plan*>()->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();
//
//			// Split mode choice model by in/out of CBD for Home-based Work, Home-based other and non-home based
//			if (_Mode_Chooser->to_CBD<bool>())
//			{
//				if (_Mode_Chooser->home_based<bool>())
//				{
//					if (activity_type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY || activity_type == Activity_Components::Types::ACTIVITY_TYPES::OTHER_WORK_ACTIVITY || activity_type == Activity_Components::Types::ACTIVITY_TYPES::PART_TIME_WORK_ACTIVITY)
//					{
//						utility = ivtt_dif * this->_BHW_CBD_IVTT + wait_dif * this->_BHW_CBD_WAIT + transfer_dif * this->_BHW_CBD_TRANSFER + walk_time_dif * this->_BHW_CBD_WALK + cost_dif * this->_BHW_CBD_COST + this->_BHW_CBD_BIAS;
//					}
//					else
//					{
//						utility = ivtt_dif * this->_BHO_CBD_IVTT + wait_dif * this->_BHO_CBD_WAIT + transfer_dif * this->_BHO_CBD_TRANSFER + walk_time_dif * this->_BHO_CBD_WALK + cost_dif * this->_BHO_CBD_COST + this->_BHO_CBD_BIAS;
//					}
//				}
//				else
//				{
//					utility = ivtt_dif * this->_BNH_CBD_IVTT + wait_dif * this->_BNH_CBD_WAIT + transfer_dif * this->_BNH_CBD_TRANSFER + walk_time_dif * this->_BNH_CBD_WALK + cost_dif * this->_BNH_CBD_COST + this->_BNH_CBD_BIAS;
//				}
//			}
//			else
//			{
//				if (_Mode_Chooser->home_based<bool>())
//				{
//					if (activity_type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY || activity_type == Activity_Components::Types::ACTIVITY_TYPES::OTHER_WORK_ACTIVITY || activity_type == Activity_Components::Types::ACTIVITY_TYPES::PART_TIME_WORK_ACTIVITY)
//					{
//						utility = ivtt_dif * this->_BHW_IVTT + wait_dif * this->_BHW_WAIT + transfer_dif * this->_BHW_TRANSFER + walk_time_dif * this->_BHW_WALK + cost_dif * this->_BHW_COST + this->_BHW_BIAS;
//					}
//					else
//					{
//						utility = ivtt_dif * this->_BHO_IVTT + wait_dif * this->_BHO_WAIT + transfer_dif * this->_BHO_TRANSFER + walk_time_dif * this->_BHO_WALK + cost_dif * this->_BHO_COST + this->_BHO_BIAS;
//					}
//				}
//				else
//				{
//					utility = ivtt_dif * this->_BNH_IVTT + wait_dif * this->_BNH_WAIT + transfer_dif * this->_BNH_TRANSFER + walk_time_dif * this->_BNH_WALK + cost_dif * this->_BNH_COST + this->_BNH_BIAS;
//				}
//			}
//			return utility;
//		}
//
//		template<typename MasterType, typename InheritanceList>
//		void Mode_Choice_Option<MasterType, InheritanceList>::default_static_initializer()
//		{
//			_BHW_CBD_IVTT = 0.0159;
//			_BHO_IVTT = 0.0114;
//			_BHO_CBD_IVTT = 0.0159;
//			_BNH_IVTT = 0.0114;
//			_BNH_CBD_IVTT = 0.0159;
//			_BHW_IVTT = 0.0186;
//
//			_BHW_WAIT = 0.0811;
//			_BHW_CBD_WAIT = 0.0173;
//			_BHO_WAIT = 0.061;
//			_BHO_CBD_WAIT = 0.0173;
//			_BNH_WAIT = 0.061;
//			_BNH_CBD_WAIT = 0.0173;
//
//			_BHW_TRANSFER = 0.0399;
//			_BHW_CBD_TRANSFER = 0.029;
//			_BHO_TRANSFER = 0.0589;
//			_BHO_CBD_TRANSFER = 0.029;
//			_BNH_TRANSFER = 0.0589;
//			_BNH_CBD_TRANSFER = 0.029;
//
//			_BHW_WALK = 0.0584;
//			_BHW_CBD_WALK = 0.0486;
//			_BHO_WALK = 0.0663;
//			_BHO_CBD_WALK = 0.0486;
//			_BNH_WALK = 0.0663;
//			_BNH_CBD_WALK = 0.0486;
//
//			_BHW_COST = 0.00119;
//			_BHW_CBD_COST = 0.00141;
//			_BHO_COST = 0.00546;
//			_BHO_CBD_COST = 0.00141;
//			_BNH_COST = 0.00546;
//			_BNH_CBD_COST = 0.00141;
//
//			_BHW_BIAS = -2;
//			_BHW_CBD_BIAS = -1;
//			_BHO_BIAS = -1.9;
//			_BHO_CBD_BIAS = -1;
//			_BNH_BIAS = -2;
//			_BNH_CBD_BIAS = -1;
//		}
//
//		// INITIALIZE MODE CHOICE MODEL STATIC PARAMETERS
//#pragma region Choice option parameters	
//		//define_static_member_variable(Telecommute_Choice_Implementation, Z_CONSTANT);
//
//		define_static_member_variable(Mode_Choice_Option, BHW_IVTT);
//		define_static_member_variable(Mode_Choice_Option, BHW_CBD_IVTT);
//		define_static_member_variable(Mode_Choice_Option, BHO_IVTT);
//		define_static_member_variable(Mode_Choice_Option, BHO_CBD_IVTT);
//		define_static_member_variable(Mode_Choice_Option, BNH_IVTT);
//		define_static_member_variable(Mode_Choice_Option, BNH_CBD_IVTT);
//
//		define_static_member_variable(Mode_Choice_Option, BHW_WAIT);
//		define_static_member_variable(Mode_Choice_Option, BHW_CBD_WAIT);
//		define_static_member_variable(Mode_Choice_Option, BHO_WAIT);
//		define_static_member_variable(Mode_Choice_Option, BHO_CBD_WAIT);
//		define_static_member_variable(Mode_Choice_Option, BNH_WAIT);
//		define_static_member_variable(Mode_Choice_Option, BNH_CBD_WAIT);
//
//		define_static_member_variable(Mode_Choice_Option, BHW_TRANSFER);
//		define_static_member_variable(Mode_Choice_Option, BHW_CBD_TRANSFER);
//		define_static_member_variable(Mode_Choice_Option, BHO_TRANSFER);
//		define_static_member_variable(Mode_Choice_Option, BHO_CBD_TRANSFER);
//		define_static_member_variable(Mode_Choice_Option, BNH_TRANSFER);
//		define_static_member_variable(Mode_Choice_Option, BNH_CBD_TRANSFER);
//
//		define_static_member_variable(Mode_Choice_Option, BHW_WALK);
//		define_static_member_variable(Mode_Choice_Option, BHW_CBD_WALK);
//		define_static_member_variable(Mode_Choice_Option, BHO_WALK);
//		define_static_member_variable(Mode_Choice_Option, BHO_CBD_WALK);
//		define_static_member_variable(Mode_Choice_Option, BNH_WALK);
//		define_static_member_variable(Mode_Choice_Option, BNH_CBD_WALK);
//
//		define_static_member_variable(Mode_Choice_Option, BHW_COST);
//		define_static_member_variable(Mode_Choice_Option, BHW_CBD_COST);
//		define_static_member_variable(Mode_Choice_Option, BHO_COST);
//		define_static_member_variable(Mode_Choice_Option, BHO_CBD_COST);
//		define_static_member_variable(Mode_Choice_Option, BNH_COST);
//		define_static_member_variable(Mode_Choice_Option, BNH_CBD_COST);
//
//		define_static_member_variable(Mode_Choice_Option, BHW_BIAS);
//		define_static_member_variable(Mode_Choice_Option, BHW_CBD_BIAS);
//		define_static_member_variable(Mode_Choice_Option, BHO_BIAS);
//		define_static_member_variable(Mode_Choice_Option, BHO_CBD_BIAS);
//		define_static_member_variable(Mode_Choice_Option, BNH_BIAS);
//		define_static_member_variable(Mode_Choice_Option, BNH_CBD_BIAS);
//#pragma endregion
//
//
//		implementation struct Mode_Chooser_Implementation : public Polaris_Component<MasterType, INHERIT(Mode_Chooser_Implementation), Data_Object>
//		{
//			// Tag as Implementation
//			typedef typename Polaris_Component<MasterType, INHERIT(Mode_Chooser_Implementation), Data_Object>::Component_Type ComponentType;
//
//			// Pointer to the Parent class
//			m_prototype(Prototypes::Person_Planner, typename MasterType::person_planner_type, Parent_Planner, NONE, NONE);
//			m_prototype(Choice_Model_Components::Prototypes::Choice_Model, typename MasterType::mnl_model_type, Choice_Model, NONE, NONE);
//
//			m_prototype(Activity_Components::Prototypes::Activity_Planner, typename MasterType::activity_type, current_activity, NONE, NONE);
//			m_prototype(Activity_Components::Prototypes::Activity_Planner, typename MasterType::activity_type, previous_activity, NONE, NONE);
//			m_prototype(Activity_Components::Prototypes::Activity_Planner, typename MasterType::activity_type, next_activity, NONE, NONE);
//			m_prototype(Activity_Location_Components::Prototypes::Activity_Location, typename MasterType::activity_location_type, destination, NONE, NONE);
//			m_prototype(Activity_Location_Components::Prototypes::Activity_Location, typename MasterType::activity_location_type, previous_location, NONE, NONE);
//			m_prototype(Activity_Location_Components::Prototypes::Activity_Location, typename MasterType::activity_location_type, next_location, NONE, NONE);
//			m_prototype(Network_Skimming_Components::Prototypes::LOS, typename MasterType::los_value_type, los, NONE, NONE);
//			m_data(bool, home_based, NONE, NONE);
//			m_data(bool, to_work_school, NONE, NONE);
//			m_data(bool, from_work_school, NONE, NONE);
//			m_data(bool, auto_available, NONE, NONE);
//			m_data(bool, to_CBD, NONE, NONE);
//			m_data(bool, delete_los, NONE, NONE);
//
//			// Interface definitions	
//			typedef Choice_Model_Components::Prototypes::Choice_Model<typename MasterType::mnl_model_type > _Choice_Model_Interface;
//			typedef Prototypes::Mode_Choice_Option<typename MasterType::mode_choice_option_type> _Mode_Choice_Option_Interface;
//			typedef Choice_Model_Components::Prototypes::Choice_Option<typename MasterType::mode_choice_option_type> _Choice_Option_Interface;
//
//			typedef Network_Skimming_Components::Prototypes::LOS<typename MasterType::los_value_type> los_itf;
//			typedef Network_Skimming_Components::Prototypes::LOS<typename MasterType::los_invariant_value_type> los_invariant_itf;
//
//			typedef Prototypes::Person<typename type_of(Parent_Planner)::type_of(Parent_Person)> person_itf;
//			typedef Prototypes::Person_Properties<typename person_itf::get_type_of(Static_Properties)> person_static_properties_itf;
//			typedef Household_Components::Prototypes::Household<typename person_itf::get_type_of(Household)> household_itf;
//			typedef Household_Components::Prototypes::Household_Properties<typename household_itf::get_type_of(Static_Properties)> household_static_properties_itf;
//			typedef Prototypes::Person_Scheduler<typename person_itf::get_type_of(Scheduling_Faculty)> scheduler_itf;
//			typedef Scenario_Components::Prototypes::Scenario< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference)> _Scenario_Interface;
//			typedef Network_Components::Prototypes::Network< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference)> _Network_Interface;
//			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _Network_Interface::get_type_of(skimming_faculty)> _Skim_Interface;
//
//			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
//			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(_Activity_Locations_Container_Interface)>  _Activity_Location_Interface;
//
//			typedef Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links)> _Links_Container_Interface;
//			typedef Link_Components::Prototypes::Link<get_component_type(_Links_Container_Interface)>  _Link_Interface;
//
//			typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
//			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_Zones_Container_Interface)>  _Zone_Interface;
//
//			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(zone_ids_container), int> _Zone_Ids_Interface;
//
//			//RLW%%%
//			//typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Activity_Container)> Activity_Plans;
//			//typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Plans)> Activity_Plan;
//
//			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Movement_Plans_Container)> Movement_Plans;
//			typedef Movement_Plan_Components::Prototypes::Movement_Plan<get_component_type(Movement_Plans)> Movement_Plan;
//
//
//			template<typename TargetType> void Initialize(/*requires(TargetType,check(typename ComponentType::Parent_Type,Concepts::Is_Person))*/);
//
//			template<typename ActivityItfType> void Define_Travel_Context(ActivityItfType activity);
//
//			template<typename ActivityItfType, typename ReturnType> ReturnType Choose_Mode(ActivityItfType activity);
//
//			template<typename ActivityItfType> bool Assign_To_HOV(ActivityItfType activity, Vehicle_Components::Types::Vehicle_Type_Keys& selected_mode);
//			template<typename LocItfType, typename ActivityItfType> bool Assign_To_Walk(LocItfType orig, LocItfType dest, ActivityItfType act);
//		};
//
//		template<typename MasterType, typename InheritanceList>
//		template<typename TargetType>
//		void Mode_Chooser_Implementation<MasterType, InheritanceList>::Initialize(/*requires(TargetType,check(typename ComponentType::Parent_Type,Concepts::Is_Person))*/)
//		{
//		}
//
//		template<typename MasterType, typename InheritanceList>
//		template<typename ActivityItfType>
//		void Mode_Chooser_Implementation<MasterType, InheritanceList>::Define_Travel_Context(ActivityItfType activity)
//		{
//			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Activity_Container)> Activity_Plans;
//			typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Plans)> Activity_Plan;
//			//typedef std::list<Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type>*> Activity_Plans;
//			//typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type> Activity_Plan;
//
//			person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
//			scheduler_itf* scheduler = _Parent_Person->template Scheduling_Faculty<scheduler_itf*>();
//			household_itf* _Parent_Household = _Parent_Person->person_itf::template Household<household_itf*>();
//			household_static_properties_itf* household_properties = _Parent_Household->template Static_Properties<household_static_properties_itf*>();
//			_delete_los = false;
//
//			// If no vehicles in the household, automatically assume transit
//			_auto_available = true;
//			if (household_properties->template Number_of_vehicles<int>() < 1) _auto_available = false;
//
//			// external knowledge references
//			_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
//			_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();
//			_Skim_Interface* skim = network->template skimming_faculty<_Skim_Interface*>();
//
//			_current_activity = (Activity_Plan*)activity;
//
//			// If the start time is known, set the previous activity/location and the next activity/location to do mode choice planning
//			if (_current_activity->Start_Is_Planned())
//			{
//				_previous_activity = _Parent_Person->template previous_activity_plan<Time_Seconds, Activity_Plan*>(_current_activity->template Start_Time<Time_Seconds>());
//				_previous_location = scheduler->template previous_location<Activity_Plan*, _Activity_Location_Interface*>(_current_activity);
//				if (_previous_location == nullptr) _previous_location = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
//				_next_activity = _Parent_Person->template next_activity_plan<Time_Seconds, Activity_Plan*>(_current_activity->template Start_Time<Time_Seconds>());
//				_next_location = scheduler->template next_location<Activity_Plan*, _Activity_Location_Interface*>(_current_activity);
//				if (_next_location == nullptr) _next_location = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
//				// check for auto availability constraints - make sure it is not a home/work based activity and see if previous mode is non-auto
//				if (_previous_location != _Parent_Person->template Home_Location<_Activity_Location_Interface*>() && _previous_location != _Parent_Person->template Work_Location<_Activity_Location_Interface*>() && _previous_location != _Parent_Person->template School_Location<_Activity_Location_Interface*>())
//				{
//					if (_previous_activity->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() != Vehicle_Components::Types::Vehicle_Type_Keys::SOV) _auto_available = false;
//				}
//			}
//			// Otherwise, next activities not known, assume start and end tour location is home
//			else
//			{
//				_previous_activity = nullptr;
//				_previous_location = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
//				_next_activity = nullptr;
//				_next_location = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
//			}
//
//			// See if this is a trip to/from CBD
//			if (_current_activity->Location_Is_Planned())
//			{
//				_destination = _current_activity->template Location<_Activity_Location_Interface*>();
//				int orig_zone = _previous_location->zone<_Zone_Interface*>()->uuid<int>();
//				int dest_zone = _destination->zone<_Zone_Interface*>()->uuid<int>();
//				if ((orig_zone > 159 && orig_zone < 216) || (dest_zone > 159 && dest_zone < 216)) _to_CBD = true;
//				else _to_CBD = false;
//			}
//			else
//			{
//				_destination = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
//			}
//
//			//=========================================================
//			// Find trip typology
//			//---------------------------------------------------------
//			// Determine if the trip is from home
//			if (_previous_location == _Parent_Person->template Home_Location<_Activity_Location_Interface*>()) _home_based = true;
//			else _home_based = false;
//
//			// Determine if trip is to work
//			Activity_Components::Types::ACTIVITY_TYPES activity_type = _current_activity->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();
//			if (activity_type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY ||
//				activity_type == Activity_Components::Types::ACTIVITY_TYPES::OTHER_WORK_ACTIVITY ||
//				activity_type == Activity_Components::Types::ACTIVITY_TYPES::PART_TIME_WORK_ACTIVITY ||
//				activity_type == Activity_Components::Types::ACTIVITY_TYPES::SCHOOL_ACTIVITY)
//			{
//				this->_to_work_school = true;
//			}
//
//			// Determine if trip is from work/school
//			if (_previous_activity != nullptr)
//			{
//				activity_type = _previous_activity->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();
//				if (activity_type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY ||
//					activity_type == Activity_Components::Types::ACTIVITY_TYPES::OTHER_WORK_ACTIVITY ||
//					activity_type == Activity_Components::Types::ACTIVITY_TYPES::PART_TIME_WORK_ACTIVITY ||
//					activity_type == Activity_Components::Types::ACTIVITY_TYPES::SCHOOL_ACTIVITY)
//				{
//					this->_from_work_school = true;
//				}
//				else this->_from_work_school = false;
//			}
//			else this->_from_work_school = false;
//
//			//=========================================================
//			// set the level of service for this context
//			//---------------------------------------------------------
//			if (_current_activity->Location_Is_Planned())
//			{
//				if (_current_activity->Start_Is_Planned()) _los = skim->template Get_LOS<_Activity_Location_Interface*, Time_Seconds, los_itf*>(_previous_location, _destination, _current_activity->template Start_Time<Time_Seconds>());
//				else _los = skim->template Get_LOS<_Activity_Location_Interface*, Time_Hours, los_itf*>(_previous_location, _destination, 12.0);
//			}
//			else
//			{
//				_delete_los = true;
//				_Zone_Interface* origin_zone = _previous_location->zone<_Zone_Interface*>();
//				los_itf* los = (los_itf*)Allocate<typename MasterType::los_value_type>();
//				los_invariant_itf* ilos = (los_invariant_itf*)Allocate<typename MasterType::los_invariant_value_type>();
//				los->LOS_time_invariant(ilos);
//				los->auto_ttime(origin_zone->template avg_ttime_auto_ampeak<Time_Minutes>());
//				los->transit_ttime(origin_zone->avg_ttime_transit<Time_Minutes>());
//				los->transit_fare(origin_zone->avg_fare_transit<Dollars>());
//				los->transit_walk_access_time(origin_zone->avg_ovtt_transit<Time_Minutes>());
//				los->transit_wait_time(origin_zone->avg_wait_transit<Time_Minutes>());
//				los->auto_distance(origin_zone->avg_distance<Miles>());
//				this->_los = los;
//			}
//		}
//
//		template<typename MasterType, typename InheritanceList>
//		template<typename ActivityItfType, typename ReturnType>
//		ReturnType Mode_Chooser_Implementation<MasterType, InheritanceList>::Choose_Mode(ActivityItfType activity)
//		{
//			person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
//			scheduler_itf* scheduler = _Parent_Person->template Scheduling_Faculty<scheduler_itf*>();
//			household_itf* _Parent_Household = _Parent_Person->person_itf::template Household<household_itf*>();
//			household_static_properties_itf* household_properties = _Parent_Household->template Static_Properties<household_static_properties_itf*>();
//
//
//			this->Define_Travel_Context(activity);
//
//			// create local choice model
//			_Choice_Model_Interface* choice_model = (_Choice_Model_Interface*)Allocate<typename MasterType::mnl_model_type>();
//			std::vector<_Mode_Choice_Option_Interface*> mode_options;
//
//			//// external knowledge references
//			//_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
//			//_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();
//
//			//Activity_Plan* cur_act = (Activity_Plan*)activity;
//			//Activity_Plan* prev_act, *next_act;
//			//_Activity_Location_Interface* prev_location, *next_location, *dest_location;
//
//			//// If the start time is known, set the previous activity/location and the next activity/location to do mode choice planning
//			//if (cur_act->template Start_Is_Planned<bool>())
//			//{
//			//	prev_act = _Parent_Person->template previous_activity_plan<Time_Seconds, Activity_Plan*>(cur_act->template Start_Time<Time_Seconds>());
//			//	prev_location = scheduler->template previous_location<Activity_Plan*, _Activity_Location_Interface*>(cur_act);
//			//	if (prev_location == nullptr) prev_location = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
//			//	next_act = _Parent_Person->template next_activity_plan<Time_Seconds, Activity_Plan*>(cur_act->template Start_Time<Time_Seconds>());
//			//	next_location = scheduler->template next_location<Activity_Plan*, _Activity_Location_Interface*>(cur_act);
//			//	if (next_location == nullptr) next_location = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
//			//}
//			//// Otherwise, next activities not known, assume start and end tour location is home
//			//else
//			//{
//			//	prev_act = nullptr;
//			//	prev_location = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
//			//	next_act = nullptr;
//			//	next_location = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
//			//}
//			//if (cur_act->template Location_Is_Planned<bool>())
//			//{
//			//	dest_location = cur_act->template Location<_Activity_Location_Interface*>();
//			//}
//			//else
//			//{
//			//	dest_location = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
//			//}
//
//			//============================================================================================
//			// add the SOV choice option
//			_Mode_Choice_Option_Interface* choice = (_Mode_Choice_Option_Interface*)Allocate<typename MasterType::mode_choice_option_type>();
//			choice->Mode_Chooser(this);
//			//choice->Parent_Planner<Parent_Planner_type>(_Parent_Planner);
//			choice->mode_type<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
//			//choice->current_activity<ActivityItfType>(activity);
//			choice_model->Add_Choice_Option<_Choice_Option_Interface*>((_Choice_Option_Interface*)choice);
//			mode_options.push_back(choice);
//
//			// add the transit choice option
//			choice = (_Mode_Choice_Option_Interface*)Allocate<typename MasterType::mode_choice_option_type>();
//			//choice->Parent_Planner<Parent_Planner_type>(_Parent_Planner);
//			choice->Mode_Chooser(this);
//			choice->mode_type<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::BUS);
//
//			//choice->current_activity<ActivityItfType>(activity);
//			//choice->destination<_Activity_Location_Interface*>(dest_location);
//			//choice->previous_activity<Activity_Plan*>(prev_act);
//			//choice->previous_location<_Activity_Location_Interface*>(prev_location);
//			//choice->next_activity<Activity_Plan*>(next_act);
//			//choice->next_location<_Activity_Location_Interface*>(next_location);
//			//choice->auto_available<bool>(auto_available);
//			choice_model->Add_Choice_Option<_Choice_Option_Interface*>((_Choice_Option_Interface*)choice);
//			mode_options.push_back(choice);
//
//			choice = (_Mode_Choice_Option_Interface*)Allocate<typename MasterType::mode_choice_option_type>();
//			choice->Mode_Chooser(this);
//			choice->mode_type<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::PARK_AND_RIDE);
//			choice_model->Add_Choice_Option<_Choice_Option_Interface*>((_Choice_Option_Interface*)choice);
//			mode_options.push_back(choice);
//
//
//			// Make choice
//			int selected_index = 0;
//			choice_model->template Evaluate_Choices<NT>();
//			_Choice_Option_Interface* selected = choice_model->template Choose<_Choice_Option_Interface*>(selected_index);
//			Vehicle_Components::Types::Vehicle_Type_Keys selected_mode = Vehicle_Components::Types::Vehicle_Type_Keys::SOV;
//
//			if (selected == nullptr) { THROW_WARNING("WARNING: selected is null - no mode choice made, defaulted to auto mode." << selected_index); }
//			else selected_mode = ((_Mode_Choice_Option_Interface*)selected)->template mode_type<ReturnType>();
//
//			// Add the temporary HOV correction - this should eventually be replaced by including HOV as an option in the choice model
//			if (Assign_To_HOV<ActivityItfType>(activity, selected_mode)) selected_mode = Vehicle_Components::Types::Vehicle_Type_Keys::HOV;
//			//if (Assign_To_Walk<_Activity_Location_Interface*, ActivityItfType>(prev_location,dest_location, activity)) selected_mode = Vehicle_Components::Types::Vehicle_Type_Keys::WALK;
//
//			//============================================================================================
//			//Account for touring - if previous act is not at an anchor location and not using auto, then auto not available
//			//-need to update to disable auto mode when not available at the anchor location as well
//			//if (prev_location != _Parent_Person->template Home_Location<_Activity_Location_Interface*>() &&
//			//	prev_location != _Parent_Person->template Work_Location<_Activity_Location_Interface*>() &&
//			//	prev_location != _Parent_Person->template School_Location<_Activity_Location_Interface*>() &&
//			//	prev_act != nullptr)
//			//{
//			//	//if (prev_act->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == BUS || prev_act->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == HOV)
//			//	//{
//			//	//	if (selected_mode == Vehicle_Components::Types::Vehicle_Type_Keys::SOV) selected_mode = Vehicle_Components::Types::Vehicle_Type_Keys::HOV;
//			//	//}
//			//}
//
//			// free memory allocated locally
//			for (int i = 0; i < mode_options.size(); i++) Free<typename _Choice_Option_Interface::Component_Type>((typename _Choice_Option_Interface::Component_Type*)mode_options[i]);
//			Free<typename MasterType::mnl_model_type>((typename MasterType::mnl_model_type*)choice_model);
//
//			// return the chosen mode
//			return selected_mode;
//		}
//
//		template<typename MasterType, typename InheritanceList>
//		template<typename ActivityItfType>
//		bool Mode_Chooser_Implementation<MasterType, InheritanceList>::Assign_To_HOV(ActivityItfType activity, Vehicle_Components::Types::Vehicle_Type_Keys& selected_mode)
//		{
//			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Activity_Container)> Activity_Plans;
//			typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Plans)> Activity_Plan;
//			//typedef std::list<Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type>*> Activity_Plans;
//			//typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type> Activity_Plan;
//
//			// if the mode is not chosen as an auto mode, return false
//			if (selected_mode != Vehicle_Components::Types::Vehicle_Type_Keys::SOV) return false;
//
//			// get interfaces
//			person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
//			person_static_properties_itf* person_properties = _Parent_Person->template Static_Properties<person_static_properties_itf*>();
//			Activity_Plan* cur_act = (Activity_Plan*)activity;
//
//			// non-licensed drivers always use HOV	
//			if (person_properties->template Age<float>() < 16.0) return true;
//
//			// probability of HOV
//			float p = 0.0;
//
//			// get probabilty for specific activity type
//			switch (cur_act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>())
//			{
//			case Activity_Components::Types::AT_HOME_ACTIVITY: { p = 0.154065262221031; break; }
//			case Activity_Components::Types::CHANGE_TRANSPORTATION_ACTIVITY: { p = 0.427794057766015; break; }
//			case Activity_Components::Types::EAT_OUT_ACTIVITY: { p = 0.249301559829059; break; }
//			case Activity_Components::Types::ERRANDS_ACTIVITY: { p = 0.107109608324519; break; }
//			case Activity_Components::Types::HEALTHCARE_ACTIVITY: { p = 0.214534967014411; break; }
//			case Activity_Components::Types::LEISURE_ACTIVITY: { p = 0.209992348215725; break; }
//			case Activity_Components::Types::MAJOR_SHOPPING_ACTIVITY: { p = 0.200815449555537; break; }
//			case Activity_Components::Types::OTHER_ACTIVITY: { p = 0.144108249493225; break; }
//			case Activity_Components::Types::OTHER_SHOPPING_ACTIVITY: { p = 0.16716082963978; break; }
//			case Activity_Components::Types::OTHER_WORK_ACTIVITY: { p = 0.040719107742865; break; }
//			case Activity_Components::Types::PERSONAL_BUSINESS_ACTIVITY: { p = 0.17837940715925; break; }
//			case Activity_Components::Types::PICK_UP_OR_DROP_OFF_ACTIVITY: { p = 0.0739687591191153; break; }
//			case Activity_Components::Types::PRIMARY_WORK_ACTIVITY: { p = 0.0637426142432775; break; }
//			case Activity_Components::Types::PART_TIME_WORK_ACTIVITY: { p = 0.0637426142432775; break; }
//			case Activity_Components::Types::RELIGIOUS_OR_CIVIC_ACTIVITY: { p = 0.300601437534384; break; }
//			case Activity_Components::Types::SCHOOL_ACTIVITY: { p = 0.228655158483933; break; }
//			case Activity_Components::Types::SERVICE_VEHICLE_ACTIVITY: { p = 0.145728864260415; break; }
//			case Activity_Components::Types::SOCIAL_ACTIVITY: { p = 0.224093784698896; break; }
//			case Activity_Components::Types::WORK_AT_HOME_ACTIVITY: { p = 0.0294387874193491; break; }
//			default:
//				p = 0.149;
//			}
//
//			// Return true if the person uses HOV mode according to random draw
//			if (GLOBALS::Uniform_RNG.Next_Rand<float>() < p) return true;
//			else return false;
//
//		}
//
//		template<typename MasterType, typename InheritanceList>
//		template<typename LocItfType, typename ActivityItfType>
//		bool Mode_Chooser_Implementation<MasterType, InheritanceList>::Assign_To_Walk(LocItfType orig, LocItfType dest, ActivityItfType act)
//		{
//			_Activity_Location_Interface* O = (_Activity_Location_Interface*)orig;
//			_Activity_Location_Interface* D = (_Activity_Location_Interface*)dest;
//
//			Meters x1 = O->template x_position<Meters>();
//			Meters y1 = O->template y_position<Meters>();
//
//			Meters x2 = D->template x_position<Meters>();
//			Meters y2 = D->template y_position<Meters>();
//
//			_Zone_Interface* dest_zone = D->template zone<_Zone_Interface*>();
//
//			// Set the maximum allowable walk distance based on area type - higher in CBD/downtown
//			Kilometers max_walk_distance = 0;
//			if (dest_zone->template areatype<int>() == 1) max_walk_distance = 1.0;			// CBD
//			else if (dest_zone->template areatype<int>() == 2) max_walk_distance = 0.5;	// Downtown
//			else if (dest_zone->template areatype<int>() == 3) max_walk_distance = 0.25;		// Rest of chicago
//			else if (dest_zone->template areatype<int>() >= 4 && dest_zone->template areatype<int>() >= 6) max_walk_distance = 0.25; // Suburban
//			else if (dest_zone->template areatype<int>() == 7) max_walk_distance = 0.0;	// Exurb
//			else max_walk_distance = 0.0;											// Rural
//
//
//			Meters dist = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
//			Kilometers dist_km = GLOBALS::Length_Converter.Convert_Value<Meters, Kilometers>(dist);
//
//
//			if (dist < max_walk_distance) return true;
//			else return false;
//		}
//#pragma endregion
	}
}


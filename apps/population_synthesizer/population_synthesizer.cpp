#ifdef _DEBUG
#define SHOW_WARNINGS
#endif

#include "Polaris_PCH.h"
#include "Population_Synthesis.h"
#include "Scenario_Implementation.h"


struct MasterType
{
	typedef Scenario_Components::Implementations::Scenario_Implementation<MasterType> scenario_type;
	typedef polaris::Basic_Units::Implementations::Length_Implementation<MasterType> length_type;
	typedef polaris::Basic_Units::Implementations::Time_Implementation<MasterType> time_type;
	typedef PopSyn::Implementations::Synthesis_Zone_Implementation_Simple<MasterType> synthesis_zone_type;
	typedef PopSyn::Implementations::Synthesis_Region_Implementation_Simple<MasterType> synthesis_region_type;
	typedef PopSyn::Implementations::IPF_Solver_Settings_Implementation<MasterType> ipf_solver_settings_type;
	typedef PopSyn::Implementations::ADAPTS_Population_Synthesis_Implementation<MasterType> population_synthesis_type;
	typedef PopSyn::Implementations::Popsyn_File_Linker_Implementation<MasterType> popsyn_file_linker_type;
	typedef Person_Components::Implementations::ACS_Person_Static_Properties_Implementation<MasterType> person_static_properties_type;
	typedef Household_Components::Implementations::ACS_Household_Static_Properties_Implementation<MasterType> household_static_properties_type;
	typedef RNG_Components::Implementations::MT_Probability_Double<MasterType> rng_type;
	typedef NULLCOMPONENT household_type;
	typedef NULLTYPE person_type;
	typedef NULLTYPE network_type;
};




int main(int argc, char* argv[])
{
	Simulation_Configuration cfg;
	cfg.Single_Threaded_Setup(1000);
	INITIALIZE_SIMULATION(cfg);


	//==================================================================================================================================
	// Scenario initialization
	//----------------------------------------------------------------------------------------------------------------------------------
	char* scenario_filename = "scenario.json";
	if (argc >= 2) scenario_filename = argv[1];
	typedef Scenario_Components::Prototypes::Scenario<MasterType::scenario_type> _Scenario_Interface;
	_Scenario_Interface* scenario=(_Scenario_Interface*)Allocate<MasterType::scenario_type>();
	cout << "reading scenario data..." <<endl;
	scenario->read_scenario_data<Scenario_Components::Types::ODB_Scenario>(scenario_filename);


	//==================================================================================================================================
	// Initialize global randon number generators - if seed set to zero or left blank use system time
	//---------------------------------------------------------------------------------------------------------------------------------- 
	int seed = scenario->iseed<int>();
	GLOBALS::Normal_RNG.Initialize();
	GLOBALS::Uniform_RNG.Initialize();
	if (seed != 0)
	{
		GLOBALS::Normal_RNG.Set_Seed<int>(seed);
		GLOBALS::Uniform_RNG.Set_Seed<int>(seed);
	}
	else
	{
		GLOBALS::Normal_RNG.Set_Seed<int>();
		GLOBALS::Uniform_RNG.Set_Seed<int>();
	}

	//==================================================================================================================================
	// Start population synthesis
	//---------------------------------------------------------------------------------------------------------------------------------- 
	typedef PopSyn::Prototypes::Population_Synthesizer<MasterType::population_synthesis_type> popsyn_itf;
	popsyn_itf* popsyn = (popsyn_itf*)Allocate<MasterType::population_synthesis_type>();
	popsyn->Initialize<_Scenario_Interface*>(scenario);

	START();

	char ans;
	cout<<endl<<endl<<"done. Press 'any' key to exit.";
	cin >> ans;
}
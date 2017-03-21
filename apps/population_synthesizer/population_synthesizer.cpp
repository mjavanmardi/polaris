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
	typedef PopSyn::Implementations::Polaris_Synthesis_Zone_Implementation_Simple<MasterType> synthesis_zone_type;
	typedef PopSyn::Implementations::Polaris_Synthesis_Region_Implementation_Simple<MasterType> synthesis_region_type;
	typedef PopSyn::Implementations::IPF_Solver_Settings_Implementation<MasterType> solver_settings_type;
	typedef PopSyn::Implementations::ADAPTS_Population_Synthesis_Implementation<MasterType> population_synthesis_type;
	typedef PopSyn::Implementations::Popsyn_File_Linker_Implementation<MasterType> popsyn_file_linker_type;
	typedef Person_Components::Implementations::ACS_Person_Static_Properties_Implementation<MasterType> person_static_properties_type;
	typedef Household_Components::Implementations::ACS_Household_Static_Properties_Implementation<MasterType> household_static_properties_type;
	typedef polaris::io::Synthetic_Household hh_db_rec_type;
	typedef polaris::io::Synthetic_Person person_db_rec_type;
	typedef RNG_Components::Implementations::MT_Probability<MasterType> rng_type;
	typedef NULLCOMPONENT household_type;
	typedef NULLTYPE person_type;
	typedef NULLTYPE network_type;
};
void help();
void generate_scenario();


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

	if (strcmp(scenario_filename, "-c") == 0)
	{
		generate_scenario();
		char help;
		cout << "Default scenario file 'scenario_popsyn.json' created in working directory.  Press any key.";
		cin >> help;
		return 0;
	}
	else if (strcmp(scenario_filename, "-h") == 0)
	{
		help();
		char help;
		cout << "Please see help file created as 'popsyn_file_linker.txt'.";
		cin >> help;
		return 0;
	}

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
	popsyn_itf* popsyn = static_cast<popsyn_itf*>(Allocate<MasterType::population_synthesis_type>());
	popsyn->Initialize<_Scenario_Interface*>(scenario);

	START();

	char ans;
	cout<<endl<<endl<<"done. Press 'any' key to exit.";
	cin >> ans;
}

void help()
{
	ofstream outfile;
	outfile.open("popsyn_file_linker.txt");
	outfile << "#USE THIS FILE AS A TEMPLATE TO GENERATE A VALID LINKER FILE FOR POPSYN"<< endl;
	outfile << endl;
	outfile << "# Specify input files for sample and marginal variables"<< endl;
	outfile << "HHFILE	pums_file.txt"<< endl;
	outfile << "PERSONFILE	pums_person_file.txt"<< endl;
	outfile << "ZONEFILE	sf1_file.txt"<< endl;
	outfile << endl;
	outfile << "# enter the size of each dimension for the household and person marginal data - this should correspond exactly with what is defined using HHHHMARGVAR and PERSONHHMARGVAR"<< endl;
	outfile << "HHDIMS	12	7"<< endl;
	outfile << "PERSONDIMS	18	8"<< endl;
	outfile << endl;
	outfile << "# Specify the region id, hhid and weight columns from the HH Pums file"<< endl;
	outfile << "REGION	3	2	6"<< endl;
	outfile << "# Specify the zone id and region id columns in the SF3 file"<< endl;
	outfile << "ZONE	0	1"<< endl;
	outfile << "# Specify the region id, the sample id, and the weight from the Person Pums file"<< endl;
	outfile << "PERSON	0	1	4"<< endl;
	outfile << endl;
	outfile << "# Specify all columns (0-indexed) for the following variables: hhtype, hhsize, nveh, nworkers, income"<< endl;
	outfile << "HHDATA	10	9	43	84	57"<< endl;
	outfile << "# Specify all columns (0-indexed) for the following variables: age, COW, educ, empl_industry, emp_status, gender, income, work arrival time, work mode, work travel time, work_veh_occ, marital_status, race, school_enroll, school_level, work hours" << endl;
	outfile << "PERSONDATA	8	12	67	93	88	69	111	94	36	34	35	38	122	65	66	73"<< endl;
	outfile << endl;
	outfile << "# Specify the variable id (starting from 0) and column number in the pums files wher the variable is located"<< endl;
	outfile << "HHVAR	0	7"<< endl;
	outfile << "HHVAR	1	8"<< endl;
	outfile << "PERSONVAR	0	7"<< endl;
	outfile << "PERSONVAR	1	6"<< endl;
	outfile << endl;
	outfile << "# Specify the marginal variables giving the variable id, index of the marginal (starting from 0 for each variable), low value (inclusive), high value (exclusive), and column number in the SF3 File"<< endl;
	outfile << "# Use the keyword for each marginal value for each variable - making sure that the number added equals that specified using the HHDIMS and PERSONDIMS commands"<< endl;
	outfile << "HHMARGVAR	0	0	1	2	33"<< endl;
	outfile << "HHMARGVAR	0	1	2	3	34"<< endl;
	outfile << "HHMARGVAR	0	2	3	4	35"<< endl;
	outfile << "HHMARGVAR	0	3	4	5	36"<< endl;
	outfile << "HHMARGVAR	0	4	5	6	37"<< endl;
	outfile << "HHMARGVAR	0	5	6	7	38"<< endl;
	outfile << "HHMARGVAR	0	6	7	8	39"<< endl;
	outfile << "HHMARGVAR	0	7	8	9	40"<< endl;
	outfile << "HHMARGVAR	0	8	9	10	41"<< endl;
	outfile << "HHMARGVAR	0	9	10	11	42"<< endl;
	outfile << "HHMARGVAR	0	10	11	12	43"<< endl;
	outfile << "HHMARGVAR	0	11	12	99	44"<< endl;
	outfile << "HHMARGVAR	1	0	1	2	45"<< endl;
	outfile << "HHMARGVAR	1	1	2	3	46"<< endl;
	outfile << "HHMARGVAR	1	2	3	4	47"<< endl;
	outfile << "HHMARGVAR	1	3	4	5	48"<< endl;
	outfile << "HHMARGVAR	1	4	5	6	49"<< endl;
	outfile << "HHMARGVAR	1	5	6	7	50"<< endl;
	outfile << "HHMARGVAR	1	6	7	99	51"<< endl;
	outfile << "PERSONMARGVAR	0	0	0	5	4"<< endl;
	outfile << "PERSONMARGVAR	0	1	5	10	5"<< endl;
	outfile << "PERSONMARGVAR	0	2	10	15	6"<< endl;
	outfile << "PERSONMARGVAR	0	3	15	20	7"<< endl;
	outfile << "PERSONMARGVAR	0	4	20	25	8"<< endl;
	outfile << "PERSONMARGVAR	0	5	25	30	9"<< endl;
	outfile << "PERSONMARGVAR	0	6	30	35	10"<< endl;
	outfile << "PERSONMARGVAR	0	7	35	40	11"<< endl;
	outfile << "PERSONMARGVAR	0	8	40	45	12"<< endl;
	outfile << "PERSONMARGVAR	0	9	45	50	13"<< endl;
	outfile << "PERSONMARGVAR	0	10	50	55	14"<< endl;
	outfile << "PERSONMARGVAR	0	11	55	60	15"<< endl;
	outfile << "PERSONMARGVAR	0	12	60	65	16"<< endl;
	outfile << "PERSONMARGVAR	0	13	65	70	17"<< endl;
	outfile << "PERSONMARGVAR	0	14	70	75	18"<< endl;
	outfile << "PERSONMARGVAR	0	15	75	80	19"<< endl;
	outfile << "PERSONMARGVAR	0	16	80	85	20"<< endl;
	outfile << "PERSONMARGVAR	0	17	85	199	21"<< endl;
	outfile << "PERSONMARGVAR	1	0	1	2	24"<< endl;
	outfile << "PERSONMARGVAR	1	1	2	3	25"<< endl;
	outfile << "PERSONMARGVAR	1	2	3	4	26"<< endl;
	outfile << "PERSONMARGVAR	1	3	4	5	27"<< endl;
	outfile << "PERSONMARGVAR	1	4	5	6	28"<< endl;
	outfile << "PERSONMARGVAR	1	5	6	7	29"<< endl;
	outfile << "PERSONMARGVAR	1	6	7	8	30"<< endl;
	outfile << "PERSONMARGVAR	1	7	8	99	31"<< endl;
	outfile.close();
}

void generate_scenario()
{
	ofstream outfile;
	outfile.open("scenario_popsyn.json");
	outfile<<"{"<<endl;
	outfile<<"\t"<<"\"output_dir_name\" : \"[Specify directory name]\","<<endl;
	outfile<<"\t"<<"\"database_name\" : \"[database]\","<<endl;
	outfile<<"\t"<<"\"percent_to_synthesize\" : 1.0,"<<endl;
	outfile<<"\t"<<"\"demand_reduction_factor\" : 1.0,"<<endl;
	outfile<<"\t"<<"\"ipf_tolerance\" : 0.01,"<<endl;
	outfile<<"\t"<<"\"marginal_tolerance\" : 5,"<<endl;
	outfile<<"\t"<<"\"maximum_iterations\" : 100,"<<endl;
	outfile<<"\t"<<"\"write_marginal_output\" : false,"<<endl;
	outfile<<"\t"<<"\"write_full_output\" : false,"<<endl;
	outfile<<"\t"<<"\"popsyn_control_file\" : \"[linker file path: use -h for help]\","<<endl;
	outfile<<"\t"<<"\"write_demand_to_database\" : true"<<endl;
	outfile<<"}"<<endl;
}



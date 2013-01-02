
#include "Application_Includes.h"


// notice how components are no longer doubly defined, they are defined once in MasterType now
struct MasterType
{
	typedef Polaris_Component<RNG_Components::Implementations::RngStream_Implementation,MasterType,Data_Object> RNG;
	typedef m_array<int>	matrix_int;
	typedef m_array<double> matrix_double;
	typedef Polaris_Component<PopSyn::Implementations::Synthesis_Zone_Implementation, MasterType, Data_Object> zone;
	typedef Polaris_Component<PopSyn::Implementations::Synthesis_Region_Implementation, MasterType, Execution_Object> region;
	typedef Polaris_Component<PopSyn::Implementations::IPF_Solver_Settings_Implementation, MasterType, Data_Object> IPF_Solver_Settings;
	typedef Polaris_Component<PopSyn::Implementations::ADAPTS_Population_Unit_Implementation, MasterType, Data_Object> pop_unit;
	typedef Polaris_Component<PopSyn::Implementations::ADAPTS_Population_Synthesis_Implementation, MasterType, Execution_Object> popsyn_solver;
};




int main()
{
	ofstream out;
	out.open("full_population_chicag.xls",ios_base::out);
	ofstream marg_out;
	marg_out.open("marginals_and_distributions_chicago.xls",ios_base::out);
	//ostream& out = cout;

	// IPF Solver Settings
	define_component_interface(solver_itf,MasterType::IPF_Solver_Settings,PopSyn::Prototypes::Solver_Settings_Prototype,NULLTYPE);
	solver_itf* solver = (solver_itf*)Allocate<MasterType::IPF_Solver_Settings>();
	solver->Initialize<Target_Type<void,double,int>>(0.05,100);

	define_component_interface(popsyn_itf,MasterType::popsyn_solver,PopSyn::Prototypes::Population_Synthesizer_Prototype,NULLTYPE);
	popsyn_itf* popsyn = (popsyn_itf*)Allocate<MasterType::popsyn_solver>();
	popsyn->linker_file_path<string>(string("C:\\Users\\Jauld\\Desktop\\Popsyn_data\\LINK_chicago.txt"));
	popsyn->Solution_Settings<solver_itf*>(solver);
	popsyn->Output_Stream<ostream&>(out);
	popsyn->Marginal_Output_Stream<ostream&>(marg_out);
	popsyn->Initialize<NULLTYPE>();


	START();
	out.close();
	// Break Here
	bool pause=true;
	cout <<endl<<endl<<"Done, press a key.";
	//cin >> pause;
}

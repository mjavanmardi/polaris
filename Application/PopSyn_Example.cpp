#include "Model_Selection.h"

#ifdef PopSyn
#include "Application_Includes.h"


// notice how components are no longer doubly defined, they are defined once in MasterType now
struct MasterType
{
	typedef MasterType M;

	typedef Person_Components::Implementations::Person_Implementation<M>	Person;
	typedef Person_Components::Implementations::Person_Properties_Implementation<M,Person> Person_Properties;
	typedef Person_Components::Implementations::Person_Planner_Implementation<M,Person> Person_Planner;
	typedef Person_Components::Implementations::Activity_Plan_Implementation<M,Person> Activity_Plan;
	typedef Person_Components::Implementations::Movement_Plan_Implementation<M,Person> Movement_Plan;
	//typedef Polaris_Component<Routing_Components::Implementations::Polaris_Routing_Implementation, MasterType, Execution_Object> routing_type;

	//typedef Polaris_Component<Derived_Agent, M, E> Agent;
	//typedef Polaris_Component<Basic_Units::Implementations::Meters_Implementation, M, D, Agent> Agent_Length;
	//typedef Polaris_Component<Basic_Units::Implementations::Square_Meters_Implementation, M, D, Agent> Agent_Area;
	//typedef Polaris_Component<Basic_Units::Implementations::Hours_Implementation, M, D, Agent> Agent_Time;
	//typedef Polaris_Component<Basic_Units::Implementations::Units_Per_Second_Implementation, M, D, Agent> Agent_Rate;
	//typedef Polaris_Component<Basic_Units::Implementations::Feet_Per_Second_Implementation, M, D, Agent> Agent_Speed;
	//typedef Polaris_Component<Basic_Units::Implementations::Dollars_Implementation, M, D, Agent> Agent_Income;

	//typedef Polaris_Component<RNG_Components::Implementations::RngStream_Implementation, M, D> RNG;
	//typedef m_array<int>	matrix_int;
	//typedef m_array<double> matrix_double;
	//typedef Polaris_Component<PopSyn::Implementations::Synthesis_Zone_Implementation, M, D> zone;
	//typedef Polaris_Component<PopSyn::Implementations::Synthesis_Region_Implementation, M, E> region;
	//typedef Polaris_Component<PopSyn::Implementations::IPF_Solver_Settings_Implementation, M, D> IPF_Solver_Settings;
	//typedef Polaris_Component<PopSyn::Implementations::ADAPTS_Population_Unit_Implementation, M, D> pop_unit;
	//typedef Polaris_Component<PopSyn::Implementations::ADAPTS_Population_Synthesis_Implementation, M, E> popsyn_solver;
};


int main()
{
	define_component_interface(agent_itf,MasterType::Person,Person_Components::Prototypes::Person_Prototype,NULLTYPE);
	define_component_interface(agent_properties_itf,MasterType::Person_Properties,Person_Components::Prototypes::Person_Properties,NULLTYPE);
	agent_itf* agent = (agent_itf*)Allocate<MasterType::Person>();
	agent->Initialize<NULLTYPE>();
	agent_properties_itf* properties = agent->Properties<agent_properties_itf*>();
	//
	//properties->My_Length<Basic_Units::Length_Variables::Feet>(20.0);
	//cout << properties->My_Length<Basic_Units::Length_Variables::Centimeters>();

	//properties->My_Area<Basic_Units::Area_Variables::Square_Inches>(144.0);
	//cout <<endl<< properties->My_Area<Basic_Units::Area_Variables::Square_Feet>();

	//properties->My_Time<Basic_Units::Time_Variables::Time_Minutes>(1440.0);
	//cout <<endl<<properties->My_Time<Basic_Units::Time_Variables::Time_Days>();

	//properties->My_Speed<Basic_Units::Speed_Variables::Kilometers_Per_Hour>(100.0);
	//cout <<endl<<properties->My_Speed<Basic_Units::Speed_Variables::Feet_Per_Second>();

	//properties->HH_Income<Basic_Units::Currency_Variables::Dollars>(100000.0);
	//cout <<endl<<properties->HH_Income<Basic_Units::Currency_Variables::Thousand_Dollars>();
	
	//ofstream out;
	//out.open("full_population_chicag.xls",ios_base::out);
	//ofstream marg_out;
	//marg_out.open("marginals_and_distributions_chicago.xls",ios_base::out);
	////ostream& out = cout;

	//// IPF Solver Settings
	//define_component_interface(solver_itf,MasterType::IPF_Solver_Settings,PopSyn::Prototypes::Solver_Settings_Prototype,NULLTYPE);
	//solver_itf* solver = (solver_itf*)Allocate<MasterType::IPF_Solver_Settings>();
	//solver->Initialize<Target_Type<void,double,int>>(0.05,100);

	//define_component_interface(popsyn_itf,MasterType::popsyn_solver,PopSyn::Prototypes::Population_Synthesizer_Prototype,NULLTYPE);
	//popsyn_itf* popsyn = (popsyn_itf*)Allocate<MasterType::popsyn_solver>();
	//popsyn->linker_file_path<string>(string("C:\\Users\\Jauld\\Desktop\\Popsyn_data\\LINK_chicago.txt"));
	//popsyn->Solution_Settings<solver_itf*>(solver);
	//popsyn->Output_Stream<ostream&>(out);
	//popsyn->Marginal_Output_Stream<ostream&>(marg_out);
	//popsyn->Initialize<NULLTYPE>();


	START();
	//out.close();
	// Break Here
	bool pause=true;
	cout <<endl<<endl<<"Done, press a key.";
	cin >> pause;
}
#endif
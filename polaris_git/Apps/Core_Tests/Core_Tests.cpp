#include "Core.h"

#include "gtest\gtest.h"
#include <random>

using namespace polaris;



static volatile long long* net_agents;
std::default_random_engine* generator;
std::uniform_real_distribution<double>* distribution;

static const double death_chance = .1499;
static const double birth_chance = .15;

template<typename MasterType = NULLTYPE,typename InheritanceList = NULLTYPELIST,template<class,class,class> class InheritanceTemplate = NULLTEMPLATE_3>
struct Agent : public Polaris_Component<MasterType,INHERIT(Agent),Execution_Object>
{
	Agent(){}

	static void Do_Stuff(Agent* _this,Event_Response& resp)
	{
		resp.next._iteration = iteration() + 1;
		resp.next._sub_iteration = 0;

		double number = distribution[__thread_id]( generator[__thread_id] );

		if(number <= (birth_chance-death_chance))
		{
			++net_agents[__thread_id];
			Agent* child = Allocate<Agent>();
			child->Load_Event<MasterType::agent_type>(&MasterType::agent_type::Do_Stuff,iteration() + 1,0);
		}

		if(number > (1-(death_chance-birth_chance)))
		{
			--net_agents[__thread_id];
			Free<Agent>(_this);
		}
	}
};

struct MasterType{ typedef Agent<MasterType> agent_type; };

TEST(Core_Engine_Test, Population_Test_Single_Threaded)
{
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	LARGE_INTEGER frequency;

	QueryPerformanceFrequency(&frequency);

	Simulation_Configuration cfg;

	cfg.Single_Threaded_Setup(1000);

	//cfg.Multi_Threaded_Setup(1000,6);
	
	INITIALIZE_SIMULATION(cfg);

	double population = 100000;

	net_agents = new volatile long long[num_sim_threads()];
	generator = new std::default_random_engine[num_sim_threads()]();
	distribution = new std::uniform_real_distribution<double>[num_sim_threads()]();
	
	for(unsigned int i=0;i<num_sim_threads();i++)
	{
		net_agents[i] = 0;
		generator[i].seed(i+12345);
	}

	//Average_Execution_Objects_Hint<MasterType::agent_type>(1000);

	for(int i=0;i<population;i++)
	{
		MasterType::agent_type* bob = Allocate<MasterType::agent_type>();
		bob->Load_Event<MasterType::agent_type>(&MasterType::agent_type::Do_Stuff,0,0);
	}

	QueryPerformanceCounter(&start);
	START();
	QueryPerformanceCounter(&end);

	double expected_population = population*pow((1.0+birth_chance-death_chance),((double)(num_iterations())));

	for(unsigned int i=0;i<num_sim_threads();i++){ population+=net_agents[i]; }
	
	//MESSAGE("done: " << population << "," << expected_population << ": " << (population-expected_population)/expected_population);
	MESSAGE("time: " << ((double)end.QuadPart - (double)start.QuadPart)/((double)frequency.QuadPart) );

	EXPECT_NEAR((population-expected_population)/expected_population,0,.025);

	delete[] net_agents;
	delete[] generator;
	delete[] distribution;

	TERMINATE_SIMULATION();
}

TEST(Core_Engine_Test, Population_Test_Multi_Threaded)
{
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	LARGE_INTEGER frequency;

	QueryPerformanceFrequency(&frequency);

	Simulation_Configuration cfg;

	//cfg.Single_Threaded_Setup(1000);

	cfg.Multi_Threaded_Setup(1000,6);
	
	INITIALIZE_SIMULATION(cfg);

	double population = 100000;

	net_agents = new volatile long long[num_sim_threads()];
	generator = new std::default_random_engine[num_sim_threads()]();
	distribution = new std::uniform_real_distribution<double>[num_sim_threads()]();
	
	for(unsigned int i=0;i<num_sim_threads();i++)
	{
		net_agents[i] = 0;
		generator[i].seed(i+12345);
	}

	//Average_Execution_Objects_Hint<MasterType::agent_type>(1000);

	for(int i=0;i<population;i++)
	{
		MasterType::agent_type* bob = Allocate<MasterType::agent_type>();
		bob->Load_Event<MasterType::agent_type>(&MasterType::agent_type::Do_Stuff,0,0);
	}

	QueryPerformanceCounter(&start);
	START();
	QueryPerformanceCounter(&end);

	double expected_population = population*pow((1.0+birth_chance-death_chance),((double)(num_iterations())));

	for(unsigned int i=0;i<num_sim_threads();i++){ population+=net_agents[i]; }
	
	//MESSAGE("done: " << population << "," << expected_population << ": " << (population-expected_population)/expected_population);
	MESSAGE("time: " << ((double)end.QuadPart - (double)start.QuadPart)/((double)frequency.QuadPart) );

	EXPECT_NEAR((population-expected_population)/expected_population,0,.05);

	delete[] net_agents;
	delete[] generator;
	delete[] distribution;

	TERMINATE_SIMULATION();
}







int main(int argc,char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#include <boost\program_options.hpp>
#include <boost\format.hpp>
#include <boost\filesystem.hpp>
#include <boost\timer\timer.hpp>
#include "myAgent.h"
#include <algorithm>
#include "version.h"

// Most polaris functionality can be found contained within the polaris namespace
using namespace polaris;

// The MasterType structure acts as a global directory for all of the other agents to refer to
//    Use it to define one concrete form of your agent and give it a name

struct MasterType
{
//#if (_MSC_VER == 1600)
	typedef myAgent::Agent<MasterType> agent_type;
	typedef myAgent::Named_Agent<MasterType> agent_named_type;
	//typedef myAgent::myAgent<MasterType::agent_type> my_agent_itf;
	//typedef myAgent::myAgent<MasterType::agent_named_type> my_named_agent_itf;
//#else
//	//template<typename AgentType> using my_agent_itf = myAgent::myAgent<AgentType>;
//	using agent_type = myAgent::Agent<MasterType>;
//	using agent_named_type = myAgent::Named_Agent<MasterType>;
//	//using my_named_agent = myAgent::my_agent_itf<agent_named_type>;
//#endif
};

struct options
{
	int num_iterations;
	int num_agents;
	int num_threads;
	std::string app_name;
};

bool parse_commandline(int argc, char* argv[], options& opts);

int main(int argc, char* argv[])
{
	options opts;
	opts.num_iterations = 1000;
	opts.num_agents = 1;
	opts.num_threads = 1;

	bool bContinue = parse_commandline( argc, argv, opts);
	if (!bContinue)
		return 0;

    // There are several pre-set configurations
    //     for this case configure a single threaded simulation which runs for 10 iterations
    Simulation_Configuration cfg;
	cfg.Multi_Threaded_Setup(opts.num_iterations, opts.num_threads);

    // The INITIALIZE_SIMULATION macro informs the Core of what type of simulation to build
    INITIALIZE_SIMULATION(cfg);

#if (_MSC_VER == 1600)
	std::vector< MasterType::my_agent_itf* > agents;
#else
	std::vector< MasterType::agent_type*> agents;
#endif
	for (int i=0; i<opts.num_agents;++i)
	{
		// In order to place agents in the simulation engine
		//     they must be allocated using the POLARIS allocator
		MasterType::agent_type* pAgent = Allocate<MasterType::agent_type>(i);
		agents.push_back(pAgent);
		// Load the agent's event
		pAgent->Initialize(i+1, i+2, i+3, i);
	}

	std::vector<MasterType::agent_named_type*> named_agents;
	for (int i=0; i<opts.num_agents;++i)
	{
		// In order to place agents in the simulation engine
		//     they must be allocated using the POLARIS allocator
		MasterType::agent_named_type* pAgent = Allocate<MasterType::agent_named_type>(i);
		named_agents.push_back(pAgent);
		string name = boost::str(boost::format("bob-%d") % i);
		// Load the agent's event
		pAgent->Initialize(i+1, i+2, i+3, name);
	}

	MasterType::agent_type* implAgent = Allocate<MasterType::agent_type>(666);
	implAgent->Initialize(1, 2, 3, 666);

	MasterType::agent_named_type* protoAgent = Allocate<MasterType::agent_named_type>();
	protoAgent->Initialize(3, 4, 5, "Daisy");

	// Begin the simulation  
	{
		boost::timer::auto_cpu_timer t;
		START();
	}

	for (int i=0; i<agents.size();++i)
	{
		MasterType::agent_type* pAgent = agents[i];
		int id;
		pAgent->get_id(id);
		cout << boost::str(boost::format("After simulation Agent '%6d'(%d): x=%6.2f, y=%6.2f, z=%6.2f\n") % id % pAgent->uuid() % pAgent->x()   % pAgent->y() % pAgent->z());
		cout << boost::str(boost::format("After simulation Agent '%6d'(%d): x=%6.2f, y=%6.2f, z=%6.2f\n") % pAgent->id() % pAgent->uuid() % pAgent->x() % pAgent->y() % pAgent->z());
		cout << boost::str(boost::format("After simulation Agent '%6s'(%d): x=%6.2f, y=%6.2f, z=%6.2f\n") % pAgent->id_desc() % pAgent->uuid() % pAgent->x() % pAgent->y() % pAgent->z());
		auto uuid = pAgent->uuid();
	}

	for (int i=0; i<named_agents.size();++i)
	{
		MasterType::agent_named_type* pAgent = named_agents[i];
		string id;
		pAgent->get_id(id);
		cout << boost::str(boost::format("After simulation Agent '%6s'(%d): x=%6.2f, y=%6.2f, z=%6.2f\n") % id % pAgent->uuid() % pAgent->x() % pAgent->y() % pAgent->z());
		cout << boost::str(boost::format("After simulation Agent '%6s'(%d): x=%6.2f, y=%6.2f, z=%6.2f\n") % pAgent->id() % pAgent->uuid() % pAgent->x() % pAgent->y() % pAgent->z());
		cout << boost::str(boost::format("After simulation Agent '%6s'(%d): x=%6.2f, y=%6.2f, z=%6.2f\n") % pAgent->id_desc() % pAgent->uuid() % pAgent->x() % pAgent->y() % pAgent->z());
	}

	cout << boost::str(boost::format("After simulation Agent '%6d'(%d): x=%6.2f, y=%6.2f, z=%6.2f\n") % implAgent->id() % implAgent->uuid() % implAgent->x() % implAgent->y() % implAgent->z());
	auto uuid = implAgent->id();

	cout << boost::str(boost::format("After simulation Agent '%6s'(%d): x=%6.2f, y=%6.2f, z=%6.2f\n") % protoAgent->id<string>() % protoAgent->uuid() % protoAgent->x() % protoAgent->y() % protoAgent->z());
	auto uuid2 = protoAgent->id<string>();

	double val = implAgent->x();
	
	// The simulation has finished
    cout << "Done!" << endl;
}

bool parse_commandline(int argc, char* argv[], options& opts)
{
	//std::string app_name;
	boost::filesystem::path p(argv[0]);
	opts.app_name = p.filename().string();
	namespace po = boost::program_options;
	po::options_description general("General options");
	general.add_options()
		("help", "display help message")
		("version", "display version information")
	;

	po::options_description app("Application options");
	app.add_options()
		("num_iterations", po::value<int>(), "set number of interations")
		("num_agents", po::value<int>(), "set number of agents")
		("num_threads", po::value<int>(), "set number of threads")
	;

	po::options_description all("Allowed options");
	all.add(general).add(app);


	po::variables_map vm;
	try
	{
		po::store(po::parse_command_line(argc, argv, all), vm);
		po::notify(vm); 
	}
	catch(...)
	{
		std::cout << std::string("ERROR parsing command line arguments...");
		return false;
	}

	if (vm.count("help"))
	{
		cout << Version::get_version(opts.app_name) << std::endl;
		cout << all << "\n";
		return false;
	}

	if (vm.count("version"))
	{
		cout << Version::get_version(opts.app_name) << std::endl;
		return false;
	}

	if (vm.count("num_iterations"))
	{
		int iters = vm["num_iterations"].as<int>();
		if (iters > 0 && iters <= 10000)
		{
			opts.num_iterations = iters;
		}
		else
		{
			std::cout << boost::str(boost::format("ERROR: num_iterations (%d) is out of range.\n") % iters);
			return false;
		}
	}

	if (vm.count("num_agents"))
	{
		int agents = vm["num_agents"].as<int>();
		if (agents > 0 && agents <= 1000)
		{
			opts.num_agents = agents;
		}
		else
		{
			std::cout << boost::str(boost::format("ERROR: num_agents (%d) is out of range.\n") % agents);
			return false;
		}
	}

	if (vm.count("num_threads"))
	{
		int threads = vm["num_threads"].as<int>();
		if (threads > 0 && threads <= 100)
		{
			opts.num_threads = threads;
		}
		else
		{
			std::cout << boost::str(boost::format("ERROR: num_threads (%d) is out of range.\n") % threads);
			return false;
		}
	}

	std::cout << boost::str(boost::format("running %s with:\n\tnum_iterations=%d\n\tnum_agents=%d\n\tnum_threads=%d") % opts.app_name % opts.num_iterations % opts.num_agents % opts.num_threads) << std::endl;

	return true;
}
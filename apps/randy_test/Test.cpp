#ifdef COMMANDLINE_ARGS
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/timer/timer.hpp>
#endif
using namespace std;
#include <boost/format.hpp>
//#include <chrono>
#include "myAgent.h"
#include <algorithm>
#include "version.h"

// Most polaris functionality can be found contained within the polaris namespace
using namespace polaris;

// MasterType is not required any longer - if using the cloneable CRTP patterns
// The MasterType structure acts as a global directory for all of the other agents to refer to
//    Use it to define one concrete form of your agent and give it a name

struct options
{
	int num_iterations;
	int num_agents;
	int num_threads;
	std::string app_name;
};

#ifdef COMMANDLINE_ARGS
bool parse_commandline(int argc, char* argv[], options& opts);
#endif

int main(int argc, char* argv[])
{
	options opts;
	opts.num_iterations = 1000;
	opts.num_agents = 1;
	opts.num_threads = 1;

#ifdef COMMANDLINE_ARGS
	bool bContinue = parse_commandline( argc, argv, opts);
	if (!bContinue)
		return 0;
#endif
	std::cout << boost::str(boost::format("running %s with:\n\tnum_iterations=%d\n\tnum_agents=%d\n\tnum_threads=%d") % opts.app_name % opts.num_iterations % opts.num_agents % opts.num_threads) << std::endl;
	
	// There are several pre-set configurations
    //     for this case configure a single threaded simulation which runs for 10 iterations
    Simulation_Configuration cfg;
	cfg.Multi_Threaded_Setup(opts.num_iterations, opts.num_threads);

    // The INITIALIZE_SIMULATION macro informs the Core of what type of simulation to build
    INITIALIZE_SIMULATION(cfg);

	myAgent::ExtendedAgent* pAgent1 = myAgent::ExtendedAgent::Allocate(22);
	pAgent1->Initialize(1, 2, 3);
	pAgent1->num_stuff(38);
	pAgent1->stuff_name("Thurlow");
	myAgent::ExtendedAgent* pAgent2 = myAgent::ExtendedAgent::Allocate();
	*pAgent2 = *pAgent1;

	std::vector<myAgent::ExtendedAgent* > extended_agents;
	for (int i = 0; i < opts.num_agents;++i)
	{
		myAgent::ExtendedAgent* pAgent = myAgent::ExtendedAgent::Allocate(i);
		extended_agents.push_back(pAgent);
		pAgent->Initialize(i, i+1, i+2);
		pAgent->id(i);
		pAgent->num_stuff(38+i);
		pAgent->stuff_name("Thurlow");
	}

	std::vector<myAgent::NewCloneableAgent<>* > agents;
	for (int i=0; i<opts.num_agents;++i)
	{
		// In order to place agents in the simulation engine
		//     they must be allocated using the POLARIS allocator
		myAgent::NewCloneableAgent<>* pAgent = myAgent::NewCloneableAgent<>::Allocate(i);
		agents.push_back(pAgent);
		// Load the agent's event
		pAgent->Initialize(i+1, i+2, i+3);
		pAgent->id(i);
	}

	std::vector<myAgent::NewCloneableNamedAgent<>*> named_agents;
	for (int i=0; i<opts.num_agents;++i)
	{
		// In order to place agents in the simulation engine
		//     they must be allocated using the POLARIS allocator
		myAgent::NewCloneableNamedAgent<>* pAgent = myAgent::NewCloneableNamedAgent<>::Allocate(i);
		named_agents.push_back(pAgent);
		string name = boost::str(boost::format("bob-%d") % i);
		// Load the agent's event
		pAgent->Initialize(i+1, i+2, i+3);
		pAgent->id(name);
	}

	// Begin the simulation  
	{
#ifdef COMMANDLINE_ARGS
		boost::timer::auto_cpu_timer t;
#endif
		START();
	}
	string int_extended_output_format = "After simulation Agent '%5d'(%3d): x=%6.2f, y=%6.2f, z=%6.2f, num_stuff=%3d, stuff_name=%s\n";
	string int_output_format = "After simulation Agent '%5d'(%3d): x=%6.2f, y=%6.2f, z=%6.2f\n";
	string str_output_format = "After simulation Agent '%5s'(%3d): x=%6.2f, y=%6.2f, z=%6.2f\n";

	for (const auto& pAgent : extended_agents)
	{
		cout << boost::str(boost::format(int_extended_output_format) % pAgent->id() % pAgent->uuid() % pAgent->x() % pAgent->y() % pAgent->z() % pAgent->num_stuff() % pAgent->stuff_name());
	}

	for (const auto& pAgent : agents)
	{
		cout << boost::str(boost::format(int_output_format) % pAgent->id() % pAgent->uuid() % pAgent->x() % pAgent->y() % pAgent->z());
	}

	for (const auto& pAgent : named_agents)
	{
		cout << boost::str(boost::format(str_output_format) % pAgent->id() % pAgent->uuid() % pAgent->x() % pAgent->y() % pAgent->z());
	}

	// The simulation has finished
	cout << "Finished!" << endl;
	cout << "Press RETURN key to continue..." << endl;
	cin.ignore();
}

#ifdef COMMANDLINE_ARGS

const int MAX_ITERATIONS = 100000;
const int MAX_AGENTS = 1000;
const int MAX_THREADS = 100;

bool parse_commandline(int argc, char* argv[], options& opts)
{
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
		if (iters > 0 && iters <= MAX_ITERATIONS)
		{
			opts.num_iterations = iters;
		}
		else
		{
			std::cout << boost::str(boost::format("ERROR: num_iterations (%d) is out of range (1-%d).\n") % iters % MAX_ITERATIONS);
			return false;
		}
	}

	if (vm.count("num_agents"))
	{
		int agents = vm["num_agents"].as<int>();
		if (agents > 0 && agents <= MAX_AGENTS)
		{
			opts.num_agents = agents;
		}
		else
		{
			std::cout << boost::str(boost::format("ERROR: num_agents (%d) is out of range (1-%d).\n") % agents % MAX_AGENTS);
			return false;
		}
	}

	if (vm.count("num_threads"))
	{
		int threads = vm["num_threads"].as<int>();
		if (threads > 0 && threads <= MAX_THREADS)
		{
			opts.num_threads = threads;
		}
		else
		{
			std::cout << boost::str(boost::format("ERROR: num_threads (%d) is out of range (1-%d).\n") % threads % MAX_THREADS);
			return false;
		}
	}

	return true;
}
#endif // COMMANDLINE_ARGS
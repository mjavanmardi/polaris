using namespace std;

#include "Repository.h"

using namespace polaris;


implementation struct bus_implementation : public Polaris_Component<MasterType, INHERIT(bus_implementation), polaris::Execution_Object>
{
	int _start_time;

	void initialize(int start_time)
	{
		_start_time = start_time;
		Load_Event<bus_implementation>(&do_bus_stuff, start_time, 0);
	}
	static void do_bus_stuff(bus_implementation* _this, Event_Response& response)
	{
		cout << "I am a basic bus. My start time is " << _this->_start_time << endl;
		response.next._iteration = iteration() + 5;
		response.next._sub_iteration = 0;
	}
};
implementation struct cta_bus_implementation : public bus_implementation<MasterType, INHERIT(cta_bus_implementation)>
{
	void do_bus_stuff()
	{
		cout << "I am a CTA bus." << endl;
	}
};
implementation struct pace_bus_implementation : public bus_implementation<MasterType, INHERIT(pace_bus_implementation)>
{
	void do_bus_stuff()
	{
		cout << "I am a pace bus." << endl;
	}
};


implementation struct driver_implementation : public Polaris_Component<MasterType, INHERIT(driver_implementation), polaris::Execution_Object>
{
	vector<typename MasterType::bus_type*> bus_container;

	//typename MasterTyp::bus_type* bus;
	string _name;

	void initialize(string name)
	{
		_name = name;
		cout << "Initializing " << _name << endl;
		typename MasterType::bus_type* bus = Allocate < MasterType::bus_type>();
		bus->initialize(iteration() + 3);
		Load_Event<driver_implementation>(&Do_driver_stuff, 0, 0);
		bus_container.push_back(bus);
	}
	static void Do_driver_stuff(driver_implementation* _this, Event_Response& response)
	{
		response.next._iteration = iteration() + 2;
		response.next._sub_iteration = 0;
		cout << "I am " << _this->_name << ", the time is " << iteration() << endl;
	}

};

struct MasterType
{
	typedef pace_bus_implementation<MasterType> bus_type;
	typedef driver_implementation<MasterType> driver_type;
};



int main(int argc, char* argv[])
{
	// There are several pre-set configurations
	polaris::Simulation_Configuration cfg;
	cfg.Single_Threaded_Setup(10); // do either single threaded and give iterations, or multi-threaded and also give number of threads
	INITIALIZE_SIMULATION(cfg);


	// Add your own set up code - make sure all execution agents have an event loaded.
	MasterType::driver_type* the_driver = Allocate<MasterType::driver_type>();

	the_driver->initialize("John");

	typedef vector<typename MasterType::bus_type*> bus_container_type;

	bus_container_type* bus_vec = &the_driver->bus_container;

	bus_container_type& bus_ref = the_driver->bus_container;


	START();

	char done;
	cin >> done;
	return true;
}

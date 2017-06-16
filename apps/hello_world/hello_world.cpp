
using namespace std;

#include "Repository.h"

using namespace polaris;

prototype struct Driver;
prototype struct Bus;
prototype struct Agency;

prototype struct Bus : public ComponentType
{
	tag_as_prototype;

	void initialize(int start_time)
	{
		this_component()->initialize(start_time);
	}
	void do_bus_stuff()
	{
		this_component()->do_bus_stuff();
	}
	accessor(start_time, true, true);
	accessor(my_bus_number, true, true);
	accessor(my_driver, true, true);
};
implementation struct bus_implementation : public Polaris_Component<MasterType, INHERIT(bus_implementation), polaris::Execution_Object>
{
	m_data(int, start_time, NONE, NONE);
	m_data(int, my_bus_number, NONE, NONE);
	m_prototype(Driver, typename MasterType::driver_type, my_driver, NONE, NONE);

	void initialize(int start_time, requires(ComponentType,check_2(ComponentType,typename MasterType::bus_type,is_base_of)))
	{
		_start_time = start_time;
		Load_Event<typename MasterType::bus_type>(&do_bus_scheduling, start_time, 1);
	}
	static void do_bus_scheduling(typename MasterType::bus_type* _this, Event_Response& response)
	{	
		response.next._iteration = iteration() + 5;
		response.next._sub_iteration = 1;
		_this->do_bus_stuff();
	}
	void do_bus_stuff()
	{
		cout << "I am a basic bus. My start time is " << this->_start_time << endl;
	}
};
implementation struct cta_bus_implementation : public bus_implementation<MasterType,INHERIT(cta_bus_implementation)>
{
	typedef true_type cta_bus_type;
	void do_bus_stuff()
	{
		cout << "I am a CTA bus, my driver is "<<_my_driver->name<string>() << endl;
	}
};
implementation struct pace_bus_implementation : public bus_implementation<MasterType, INHERIT(pace_bus_implementation)>
{
	typedef true_type pace_bus_type;
	void do_bus_stuff()
	{
		cout << "I am a pace bus, my driver is " << _my_driver->name<string>() << endl;
	}
};


prototype struct Driver : public ComponentType
{
	tag_as_prototype;

	void initialize(string name, int start_time)
	{
		this->name(name);
		this_component()->initialize(start_time);
	}
	accessor(name, true, true);
	accessor(my_bus, true, true);
	accessor(my_agency, true, true);
};
implementation struct driver_implementation : public Polaris_Component<MasterType, INHERIT(driver_implementation), polaris::Execution_Object>
{
	m_data(string, name, NONE, NONE);
	m_prototype(Bus, typename MasterType::bus_type, my_bus, NONE, NONE);
	m_prototype(Agency, typename MasterType::agency_type, my_agency, NONE, NONE);

	void initialize(int start_time)
	{
		cout << "Initializing " << _name << endl;

		this->my_bus(Allocate <typename MasterType::bus_type>());
		_my_bus->initialize(start_time + 2);
		_my_bus->my_driver(this);
		_my_bus->my_bus_number(_my_agency->buses<typename type_of(my_agency)::get_type_of(buses)*>()->size()*100+101);
		_my_agency->buses<typename type_of(my_agency)::get_type_of(buses)*>()->push_back(_my_bus);

		Load_Event<typename MasterType::driver_type>(&Do_driver_event, start_time, 0);
	}
	static void Do_driver_event(typename MasterType::driver_type* _this, Event_Response& response)
	{
		response.next._iteration = iteration() + 2;
		response.next._sub_iteration = 0;
		_this->Do_driver_stuff<NT>();
	}
	template <typename T> void Do_driver_stuff()
	{
		cout << "I am basic driver " << _name << ", the time is " << iteration() << endl;
	}
	
};
implementation struct cta_driver_implementation : public driver_implementation<MasterType,INHERIT(cta_driver_implementation)>
{
	template <typename T> void Do_driver_stuff(requires(T, check_2(my_bus_component_type, cta_bus_implementation<MasterType>, is_same)))
	{
		cout << "I am CTA driver " << _name << ", the time is " << iteration() << endl;
	}
	template <typename T> void Do_driver_stuff(requires(T, !check_2(my_bus_component_type, cta_bus_implementation<MasterType>, is_same)))
	{
		assert_check_2(my_bus_component_type, cta_bus_implementation<MasterType>, !is_same, "Error - CTA drivers can only drive CTA buses.");
	}

};
implementation struct pace_driver_implementation : public driver_implementation<MasterType, INHERIT(pace_driver_implementation)>
{
	template <typename T> void Do_driver_stuff(requires(T, check_2(my_bus_component_type, pace_bus_implementation<MasterType>, is_same)))
	{
		cout << "I am Pace driver " << _name << ", the time is " << iteration() << endl;
	}
	template <typename T> void Do_driver_stuff(requires(T, !check_2(my_bus_component_type, pace_bus_implementation<MasterType>, is_same)))
	{
		assert_check_2(my_bus_component_type, cta_bus_implementation<MasterType>, !is_same, "Error - PACE drivers can only drive PACE buses.");
	}

};


prototype struct Agency : public ComponentType
{
	tag_as_prototype;
	void initialize(){this_component()->initialize();}
};
implementation struct agency_implementation : public Polaris_Component<MasterType, INHERIT(agency_implementation), polaris::Data_Object>
{
	m_container(std::vector<Driver<typename MasterType::driver_type>*>, drivers, NONE, NONE);
	m_container(std::vector<Bus<typename MasterType::bus_type>*>, buses, NONE, NONE);

	void initialize()
	{
		create_driver("John", 2);		
		create_driver("David", 3);
		list_my_drivers_and_buses();
	}
	void create_driver(string name, int start_time)
	{
		Driver<MasterType::driver_type>* driver = (Driver<MasterType::driver_type>*)Allocate<MasterType::driver_type>();
		driver->my_agency(this);
		driver->initialize(name, start_time);
		_drivers.push_back(driver);
	}

	void list_my_drivers_and_buses()
	{
		cout << "Agency drivers:" << endl;
		for (drivers_type::iterator itr = _drivers.begin(); itr != _drivers.end(); ++itr) cout << (*itr)->name<string>() << endl;
		cout << "Agency buses:" << endl;
		for (buses_type::iterator itr = _buses.begin(); itr != _buses.end(); ++itr) cout << (*itr)->my_bus_number<int>() << endl;
		cout << endl;
	}
};

struct MasterType
{
	typedef cta_bus_implementation<MasterType> bus_type;
	typedef cta_driver_implementation<MasterType> driver_type;
	typedef agency_implementation<MasterType> agency_type;
};


int main(int argc, char* argv[])
{
	// There are several pre-set configurations
    polaris::Simulation_Configuration cfg;
	cfg.Single_Threaded_Setup(10); // do either single threaded and give iterations, or multi-threaded and also give number of threads
    INITIALIZE_SIMULATION(cfg);

	// Add your own set up code - make sure all execution agents have an event loaded.
	Agency<typename MasterType::agency_type>* the_agency = (Agency<MasterType::agency_type>*)Allocate<MasterType::agency_type>();
	the_agency->initialize();

	START();

	return true;
}

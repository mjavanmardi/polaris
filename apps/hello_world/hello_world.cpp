
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
		cout << "I am a basic bus. My start time is " <<_this->_start_time<< endl;
		response.next._iteration = iteration() + 5;
		response.next._sub_iteration = 0;
	}
};
implementation struct cta_bus_implementation : public bus_implementation<MasterType,INHERIT(cta_bus_implementation)>
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
	typename MasterType::bus_type* bus;
	string _name;

	void initialize(string name)
	{
		_name = name;
		cout << "Initializing " << _name << endl;
		bus = Allocate < MasterType::bus_type>();
		bus->initialize(iteration() + 3);
		Load_Event<driver_implementation>(&Do_driver_stuff, 0, 0);
	}
	static void Do_driver_stuff(driver_implementation* _this, Event_Response& response)
	{
		response.next._iteration = iteration() + 2;
		response.next._sub_iteration = 0;
		cout << "I am " << _this->_name << ", the time is " << iteration() << endl;
	}
	
};





implementation struct Base : public Polaris_Component<MasterType, INHERIT(Base)>
{
	static int x;
	static int y;
	static int z;
	void print()
	{
		cout <<"Static value is: "<< Base::x<<", "<<Base::y<<", "<<Base::z<<endl;
	}
};
template <typename MasterType, typename InheritanceList> int Base<MasterType, InheritanceList>::x = 1;
template <typename MasterType, typename InheritanceList> int Base<MasterType, InheritanceList>::y = 2;
template <typename MasterType, typename InheritanceList> int Base<MasterType, InheritanceList>::z = 3;

implementation struct Child : public Base<MasterType, INHERIT(Child)>
{
	static void init()
	{
		Base<MasterType, INHERIT(Child)>::x = 4;
	}
};


//prototype struct Agent
//{
//	tag_as_prototype;
//
//	void initialize(int start_time, int start_subiteration)
//	{
//		this->my_subiteration(start_subiteration);
//
//		this_component()->initialize(start_time);
//	}
//	accessor(name, true, true);
//	accessor(my_subiteration,true,true);
//	accessor(money, true, true);
//	accessor(my_parent, true, true);
//};
//
//prototype struct Link
//{
//	tag_as_prototype;
//	accessor(name, NONE, NONE);
//	accessor(agent, NONE, NONE);
//};
//
//
//implementation struct base_agent_impl : public Polaris_Component<MasterType, INHERIT(base_agent_impl),Execution_Object>
//{
//	void initialize(int start_time)
//	{
//		_name = "Base";
//        this->template Load_Event<base_agent_impl>(&Do_stuff, start_time, this->my_subiteration<int>());
//	}
//	static void Do_stuff(base_agent_impl* _this, Event_Response& response)
//	{
//		response.next._iteration = iteration() + 2;
//		response.next._sub_iteration = _this->my_subiteration<int>();
//		_this->write_stuff();
//	}
//	void write_stuff()
//	{
//		cout << "Hello world, I am "<<this->name<string>()<<" this is iteration " << iteration()<<", subiteration "<<sub_iteration() << endl;
//	}
//
//	m_data(string, name, true, true);
//	m_data(int, my_subiteration, true, true);
//	//m_data(int, money,true,true);
//};
//implementation struct other_agent_impl : public base_agent_impl<MasterType, INHERIT(other_agent_impl)>
//{
//	m_data(float, money, true, true);
//	void initialize(int start_time)
//	{
//		_name = "Other";
//        this->template Load_Event<other_agent_impl>(&Do_stuff, start_time, this->template my_subiteration<int>());
//	}
//	static void Do_stuff(other_agent_impl* _this, Event_Response& response)
//	{
//		response.next._iteration = iteration() + 3;
//        response.next._sub_iteration = _this->template my_subiteration<int>();
//		_this->write_stuff();
//	}
//	void write_stuff()
//	{
//		this->_money += 1;
//        cout << "Hello world, I am " << this->template name<string>() << " this is iteration " << iteration() << ", subiteration " << sub_iteration() << endl;
//	}
//
//	m_prototype(Agent, typename MasterType::base_agent_type, my_parent, true, true);
//};
//
//implementation struct link_impl : public Polaris_Component<MasterType, INHERIT(link_impl), Execution_Object>
//{
//	void initialize(int start_time)
//	{
//		typedef Agent<MasterType::agent_type> agent_itf;
//		agent_itf* my_agent = (agent_itf*)Allocate<typename MasterType::agent_type>();
//		my_agent->initialize(2, 2);
//		this->the_agent(my_agent);
//		my_agent->money<float>(3.0f);
//
//        this->template Load_Event<link_impl>(&Do_stuff, start_time, 0);
//	}
//	static void Do_stuff(link_impl* _this, Event_Response& response)
//	{
//		response.next._iteration = iteration() + 1;
//		response.next._sub_iteration = 0;
//
//		_this->Do_the_stuff();
//	}
//	void Do_the_stuff()
//	{
//        cout << "I am link. My agent has " << this->_the_agent->template money<int>() << " money." << endl;
//		this->_the_agent->my_parent<void*>();
//	}
//
//	m_data(string, name, true, true);
//	m_prototype(Agent, typename MasterType::agent_type, the_agent, true, true);
//};

struct MasterType
{
	typedef Base<MasterType>  base_type;
	typedef Child<MasterType> child_type;
	typedef pace_bus_implementation<MasterType> bus_type;
	typedef driver_implementation<MasterType> driver_type;
};



int main(int argc, char* argv[])
{
	float n1 = GLOBALS::Normal_Distribution->Cumulative_Distribution_Value<float>(0.0);
	float n2 = GLOBALS::Bivariate_Normal_Distribution->Cumulative_Distribution_Value<float>(0.0, 0.0, 1.0);

	Child<MT>* c = Allocate<Child<MT>>();
	c->init();
	Base<MT>* b = Allocate<Base<MT>>();

	c->print();
	b->print();


	// There are several pre-set configurations
    polaris::Simulation_Configuration cfg;
	cfg.Single_Threaded_Setup(10); // do either single threaded and give iterations, or multi-threaded and also give number of threads
    INITIALIZE_SIMULATION(cfg);


	// Add your own set up code - make sure all execution agents have an event loaded.
	MasterType::driver_type* the_driver = Allocate<MasterType::driver_type>();

	the_driver->initialize("John");


	START();

	char done;
	cin >> done;
	return true;
}

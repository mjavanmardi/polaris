
using namespace std;

#include "Repository.h"

using namespace polaris;



prototype struct Agent
{
	tag_as_prototype;

	void initialize(int start_time, int start_subiteration)
	{
		this->my_subiteration(start_subiteration);

		this_component()->initialize(start_time);
	}
	accessor(name, true, true);
	accessor(my_subiteration,true,true);
	accessor(money, true, true);
	accessor(my_parent, true, true);
};

prototype struct Link
{
	tag_as_prototype;
	accessor(name, NONE, NONE);
	accessor(agent, NONE, NONE);
};

implementation struct base_agent_impl : public Polaris_Component<MasterType, INHERIT(base_agent_impl),Execution_Object>
{
	void initialize(int start_time)
	{
		_name = "Base";
        this->template Load_Event<base_agent_impl>(&Do_stuff, start_time, this->my_subiteration<int>());
	}
	static void Do_stuff(base_agent_impl* _this, Event_Response& response)
	{
		response.next._iteration = iteration() + 2;
		response.next._sub_iteration = _this->my_subiteration<int>();
		_this->write_stuff();
	}
	void write_stuff()
	{
		cout << "Hello world, I am "<<this->name<string>()<<" this is iteration " << iteration()<<", subiteration "<<sub_iteration() << endl;
	}

	m_data(string, name, true, true);
	m_data(int, my_subiteration, true, true);
	//m_data(int, money,true,true);
};
implementation struct other_agent_impl : public base_agent_impl<MasterType, INHERIT(other_agent_impl)>
{
	m_data(float, money, true, true);
	void initialize(int start_time)
	{
		_name = "Other";
        this->template Load_Event<other_agent_impl>(&Do_stuff, start_time, this->template my_subiteration<int>());
	}
	static void Do_stuff(other_agent_impl* _this, Event_Response& response)
	{
		response.next._iteration = iteration() + 3;
        response.next._sub_iteration = _this->template my_subiteration<int>();
		_this->write_stuff();
	}
	void write_stuff()
	{
		this->_money += 1;
        cout << "Hello world, I am " << this->template name<string>() << " this is iteration " << iteration() << ", subiteration " << sub_iteration() << endl;
	}

	m_prototype(Agent, typename MasterType::base_agent_type, my_parent, true, true);
};

implementation struct link_impl : public Polaris_Component<MasterType, INHERIT(link_impl), Execution_Object>
{
	void initialize(int start_time)
	{
		typedef Agent<MasterType::agent_type> agent_itf;
		agent_itf* my_agent = (agent_itf*)Allocate<typename MasterType::agent_type>();
		my_agent->initialize(2, 2);
		this->the_agent(my_agent);
		my_agent->money<float>(3.0f);

        this->template Load_Event<link_impl>(&Do_stuff, start_time, 0);
	}
	static void Do_stuff(link_impl* _this, Event_Response& response)
	{
		response.next._iteration = iteration() + 1;
		response.next._sub_iteration = 0;

		_this->Do_the_stuff();
	}
	void Do_the_stuff()
	{
        cout << "I am link. My agent has " << this->_the_agent->template money<int>() << " money." << endl;
		this->_the_agent->my_parent<void*>();
	}

	m_data(string, name, true, true);
	m_prototype(Agent, typename MasterType::agent_type, the_agent, true, true);
};

struct MasterType
{
	typedef base_agent_impl<MasterType> agent_type;
	typedef link_impl<MasterType> link_type;
	typedef base_agent_impl<MasterType> base_agent_type;
};

int main(int argc, char* argv[])
{
	// There are several pre-set configurations
    Simulation_Configuration cfg;
	cfg.Single_Threaded_Setup(10);

    // The INITIALIZE_SIMULATION macro informs the Core of what type of simulation to build
    INITIALIZE_SIMULATION(cfg);


	MasterType::link_type* my_link = (MasterType::link_type*)Allocate<MasterType::link_type>();
	my_link->initialize(1);


	START();

	char done;
	cin >> done;
	return true;
}

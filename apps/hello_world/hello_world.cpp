
using namespace std;

#include "Repository.h"

using namespace polaris;

prototype struct Agent
{
	tag_as_prototype;

	void initialize(int start_time, int start_subiteration, string myname)
	{
		this->name(myname);
		this->my_subiteration(start_subiteration);

		this_component()->initialize(start_time);
	}
	accessor(name, true, true);
	accessor(my_subiteration, true, true);
	accessor(is_active, true, true);
};

implementation struct my_agent_impl : public Polaris_Component<MasterType, INHERIT(my_agent_impl),Execution_Object>
{
	void initialize(int start_time)
	{
        this->template Load_Event<my_agent_impl>(&Do_stuff, start_time, this->my_subiteration<int>());
	}
	static void Do_stuff(my_agent_impl* _this, Event_Response& response)
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
	

};
implementation struct other_agent_impl : public my_agent_impl<MasterType, INHERIT(other_agent_impl)>
{
	void initialize(int start_time)
	{
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
		if (this->_is_active) this->_is_active = false;
		else this->_is_active = true;
        cout << "Hello world, I am " << this->template name<string>() << " this is iteration " << iteration() << ", subiteration " << sub_iteration() << endl;
	}

	m_data(bool, is_active, true, true);
};

implementation struct link_impl : public Polaris_Component<MasterType, INHERIT(link_impl), Execution_Object>
{
	void initialize(int start_time)
	{
        this->template Load_Event<link_impl>(&Do_stuff, start_time, 0);
	}
	static void Do_stuff(link_impl* _this, Event_Response& response)
	{
		response.next._iteration = iteration() + 3;
		response.next._sub_iteration = 0;

		_this->Do_the_stuff();
	}
	void Do_the_stuff()
	{
        if (this->_the_agent->template is_active<bool>()) cout << "My agent is currently active..." << endl;
		else cout << "My agent is sleeping..." << endl;
	}

	m_prototype(Agent, typename MasterType::agent_type, the_agent, true, true);
};

struct MasterType
{
	typedef other_agent_impl<MasterType> agent_type;
	typedef link_impl<MasterType> link_type;

};

int main(int argc, char* argv[])
{
	// There are several pre-set configurations
    Simulation_Configuration cfg;
	cfg.Single_Threaded_Setup(10);

    // The INITIALIZE_SIMULATION macro informs the Core of what type of simulation to build
    INITIALIZE_SIMULATION(cfg);

	typedef Agent<MasterType::agent_type> agent_itf;
	agent_itf* my_agent = (agent_itf*)Allocate<typename MasterType::agent_type>();
	my_agent->initialize(2, 2, "base");

	MasterType::link_type* my_link = (MasterType::link_type*)Allocate<MasterType::link_type>();
	my_link->the_agent(my_agent);
	my_link->initialize(1);

	RNG_Components::Implementations::MT_Normal<NT> nrm1;
	float p1 = nrm1.Cumulative_Distribution<float>(0.5);

	float p2 = GLOBALS::Bivariate_Normal_Distribution->Cumulative_Distribution_Value<float>(0, 0, 1);

	START();

	char done;
	cin >> done;
	return true;
}

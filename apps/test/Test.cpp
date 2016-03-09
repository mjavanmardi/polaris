#include "repository/Repository.h"
using namespace polaris;


prototype struct Agent
{
	void initialize()
	{
		((ComponentType*)this)->initialize();
	}

	accessor(my_agent,NONE,NONE);
	accessor(id,NONE,NONE);
};

implementation struct Agent_Impl : public Polaris_Component<MasterType,INHERIT(Agent_Impl), Execution_Object>
{
	void initialize()
	{
		Load_Event<Agent_Impl>(&Do_Stuff, 0, 0); //&event function address, first iteration, first subiteration
	}
	static void Do_Stuff(Agent_Impl* _this, Event_Response& response)
	{
		cout << "Hello world...this is iteration"<<iteration()<<" from agent id: " <<_this->_my_agent->id<int>()<<endl;
		response.next._iteration = iteration()+1;
		response.next._sub_iteration = 0;
	}
	m_data(int,id,NONE,NONE);
	m_prototype(Agent,typename MasterType::other_agent_type,my_agent,NONE,NONE);
};

implementation struct Other_Agent_Impl : public Polaris_Component<MasterType,INHERIT(Other_Agent_Impl), Execution_Object>
{
	void initialize()
	{
		Load_Event<Other_Agent_Impl>(&Do_Stuff, 0, 0);
	}
	static void Do_Stuff(Other_Agent_Impl* _this, Event_Response& response)
	{
		cout << "Goodbye world...this is iteration"<<iteration()<<endl;
		response.next._iteration = iteration()+1;
		response.next._sub_iteration = 0;
	}
	m_data(int,id,NONE,NONE);
	m_prototype(Agent,typename MasterType::agent_type,my_agent,NONE,NONE);
};

struct MasterType
{
	typedef Agent_Impl<MasterType> agent_type;
	typedef Agent_Impl<MasterType> other_agent_type;
};


int main(int argc, char *argv[])
{
	Simulation_Configuration cfg;
	cfg.Multi_Threaded_Setup(10, 1);
	INITIALIZE_SIMULATION(cfg);

	Agent<MasterType::agent_type>* base_agent = (Agent<MasterType::agent_type>*)Allocate<MasterType::agent_type>();
	base_agent->initialize();

	Agent<MasterType::other_agent_type>* other_agent = (Agent<MasterType::other_agent_type>*)Allocate<MasterType::other_agent_type>();
	other_agent->initialize();
	
	base_agent->id<double>(1.2);
	other_agent->id<float>(3.4f);

	base_agent->my_agent(other_agent);
	other_agent->my_agent(base_agent);
		
	START();

	char test;
	cin >> test;
}
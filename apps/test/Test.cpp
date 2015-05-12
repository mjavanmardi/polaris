
#include "Polaris_PCH.h"
using namespace polaris;


// Test agent
prototype struct Agent ADD_DEBUG_INFO
{
	tag_as_prototype;
	template<typename T> void Initialize()
	{
		this_component()->Initialize();
		this_component()->Load_Event<ComponentType>(&Agent_Event,1,0);
	}
	static void Agent_Event(ComponentType* _this,Event_Response& response)
	{
		if (iteration() != 20)
		{
			response.next._iteration = 20;
			response.next._sub_iteration = 0;
		}
		else
		{
			response.next._iteration = END;
			response.next._sub_iteration = END;
		}
		_this->Do_Event();
	}
	void Reschedule_Agent(int iter)
	{
		// Everything works fine until here - I think I need some way from RTTI instead of this_component() to make sure
		// the correct Execution_Component_Manager gets updated.
		this_component()->Reschedule<ComponentType>(iter, 0);
	}
};
implementation struct Base_Agent_Implementation : public Polaris_Component<MasterType,INHERIT(Base_Agent_Implementation),Execution_Object>
{
	void Initialize()
	{
		_data = 1;
	}
	void Do_Event()
	{
		cout <<"BASE_AGENT: event fired at " << iteration()<<endl;
	}
	m_data(int,data,NONE,NONE);
};
implementation struct Derived_Agent_Implementation : public Base_Agent_Implementation<MasterType,INHERIT(Derived_Agent_Implementation)>
{
	typedef Agent<ComponentType> this_itf;
	void Initialize()
	{
		_data = 5;
	}
	void Do_Event()
	{
		cout <<"DERIVED_AGENT: event fired at " << iteration()<<endl;
		this_itf* pthis = (this_itf*)this;
		pthis->Reschedule_Agent(26);
	}
};

prototype struct Helper
{
	tag_as_prototype;

	template<typename T> void Initialize()
	{
		this_component()->Initialize<T>();
	}
	accessor(agent_container,NONE,NONE);
};
implementation struct Helper_Implementation : public Polaris_Component<MasterType,INHERIT(Helper_Implementation),Execution_Object>
{
	template<typename T> void Initialize()
	{
		typedef Agent<typename MasterType::base_agent_type> base_itf;
		typedef Agent<typename MasterType::derived_agent_type> derived_itf;

		base_itf* base_agent=(base_itf*)Allocate<typename MasterType::base_agent_type>();
		base_agent->Initialize<NT>();

		derived_itf* derived_agent=(derived_itf*)Allocate<typename MasterType::derived_agent_type>();
		derived_agent->Initialize<NT>();

		this->_agent_container.push_back((base_itf*)derived_agent);
		this->_agent_container.push_back(base_agent);

		Load_Event<ComponentType>(&Helper_Event,0,0);
	}
	static void Helper_Event(ComponentType* _this,Event_Response& response)
	{
		// Pretending here that we don't know which type derived_agent is so using interface to base type (as in activity model)
		typedef Agent<typename MasterType::base_agent_type> base_itf;

		if (iteration() < 10)
		{
			response.next._iteration = iteration()+1;
			response.next._sub_iteration = 0;
		}
		else if (iteration() == 10)
		{
			base_itf* b = (base_itf*)_this->_agent_container[0];
			response.next._iteration = iteration()+1;
			response.next._sub_iteration = 0;
			b->Reschedule_Agent(iteration()+1);
		}
		else
		{
			base_itf* b = (base_itf*)_this->_agent_container[1];
			response.next._iteration = END;
			response.next._sub_iteration = END;
			b->Reschedule_Agent(iteration()+1);
		}
		cout <<"HELPER_AGENT: event fired at " << iteration() <<", next event to fire at " << response.next._iteration<<endl;
	}

	m_container(std::vector<Agent<typename MasterType::base_agent_type>*>,agent_container,NONE,NONE);
};

struct MasterType
{
	typedef MasterType M;
	typedef Base_Agent_Implementation<M> base_agent_type;
	typedef Derived_Agent_Implementation<M> derived_agent_type;
	
	// Add all of the types used in your code here
};


int main(int argc, char *argv[])
{
	//----------------------------------------------------------
	// Initialize basic simulation 
	Simulation_Configuration cfg;
	cfg.Multi_Threaded_Setup(100, 1);
	INITIALIZE_SIMULATION(cfg);


	//-------------------------------------------------------------
	// Initialize random variable generators
	GLOBALS::Normal_RNG.Initialize();
	GLOBALS::Uniform_RNG.Initialize();
	GLOBALS::Normal_RNG.Set_Seed<int>();
	GLOBALS::Uniform_RNG.Set_Seed<int>();


	//----------------------------------------------------------
	// Your code here
	typedef Agent<MasterType::base_agent_type> base_itf;
	typedef Agent<MasterType::derived_agent_type> derived_itf;

	base_itf* agent1 = (base_itf*)Allocate<MasterType::base_agent_type>();
	derived_itf* agent2 = (derived_itf*)Allocate<MasterType::derived_agent_type>();

	agent1->Initialize<NT>();
	agent2->Initialize<NT>();


	START();
	char test;
	cin >> test;
}

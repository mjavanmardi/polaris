#include "Core\Core.h"

using namespace polaris;

template<typename MasterType, typename InheritanceList = NULLTYPELIST>
class Agent : public Polaris_Component<MasterType,INHERIT(Agent),Execution_Object>
{
public:
	static void testReinit(Agent* _this, Event_Response& response)
	{
		_this->test = 0;
		cout << "Test value has been reinitialized to : " << _this->test << endl;
		response.next._iteration = iteration() + 1;
		response.next._sub_iteration = 0;
	}
	static void Talk(Agent* _this, Event_Response& response)
	{
		cout << "My id is : " << _this->id ;
		cout << " ; current iteration is : " << iteration() << endl;
		response.next._iteration = iteration() + 1;
		response.next._sub_iteration = 1;
	}
	void Setup(int _id)
	{
		id = _id;
		test = 0;
		Load_Event(&testReinit,0,0);
		Load_Event(&Talk,0,1);
	}
private:
	int id;
	int test;
};

struct MasterType
{
	typedef Agent<MasterType> agent_type;
};

int main()
{
	Simulation_Configuration cfg;
	//cfg.Multi_Threaded_Setup(10,4);
	cfg.Single_Threaded_Setup(10);
	INITIALIZE_SIMULATION(cfg);
	MT::agent_type* bob = Allocate< MT::agent_type >(1);
	bob->Setup(1);
	MT::agent_type* sally = Allocate< MT::agent_type >(2);
	sally->Setup(2);
	MT::agent_type* tom = Allocate< MT::agent_type >(3);
	tom->Setup(3);
	START();
	TERMINATE_SIMULATION();
	system("pause");
	return 0;
}
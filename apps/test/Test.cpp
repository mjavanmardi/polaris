#include "Core\Core.h"

using namespace polaris;

template<typename MasterType,typename InheritanceList = NULLTYPELIST>
struct Agent : public Polaris_Component<MasterType,INHERIT(Agent),Execution_Object>
{
    static void Talk(Agent* _this,Event_Response& response)
    {
        cout << "My name is: " << _this->_name << "; the current iteration is: " << iteration() << endl;

        response.next._iteration = iteration() + 1;
        response.next._sub_iteration = sub_iteration();

        if(_this->uuid() == 1)
        {
            Agent* sally = Object_Lookup<Agent>(2);
            cout << "Found: " << sally->_name << endl;
        }
    }

    void Setup(string& name)
    {
        _name = name;

        Load_Event(&Talk,0,0);
    }

    string _name;
};

struct MasterType
{
    typedef Agent<MasterType> agent_type;
};

int main()
{
    Simulation_Configuration cfg;
    cfg.Single_Threaded_Setup(10);
    INITIALIZE_SIMULATION(cfg);

    MT::agent_type* bob = Allocate< MT::agent_type >(1);

    bob->Setup(string("Bob"));

    MT::agent_type* sally = Allocate< MT::agent_type >(2);

    sally->Setup(string("Sally"));

    START();

    TERMINATE_SIMULATION();
}
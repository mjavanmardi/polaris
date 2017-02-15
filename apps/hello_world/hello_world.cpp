using namespace std;

#include "Repository.h"
#ifndef _MSC_VER
	#include <cxxabi.h>
#endif

using namespace polaris;

prototype struct Agent : public ComponentType
{
    tag_as_prototype;

    void initialize(int start_time, int start_subiteration, string myname)
    {
        this->name(myname);
        this->my_subiteration(start_subiteration);
        this->money(0.0);

        this_component()->initialize(start_time);
    }
    accessor(name, true, true);
    accessor(my_subiteration, true, true);
    accessor(is_active, true, true);
    accessor(money, true, true);
};

prototype struct Link_Prototype : public ComponentType
{
    tag_as_prototype;

    accessor(agent_container, NONE, NONE);
};

implementation struct my_agent_impl : public Polaris_Component<MasterType, INHERIT(my_agent_impl),Execution_Object>
{
    typedef Agent<typename MasterType::agent_type> agent_itf;

    void initialize(int start_time)
    {
        //this->template Load_Event<my_agent_impl>(&Do_stuff, start_time, this->my_subiteration<int>());
		_name = "Base";
        this->template Load_Event<base_agent_impl>(&Do_stuff, start_time, this->my_subiteration<int>());
    }

    static void Do_stuff(my_agent_impl* _this, Event_Response& response)
    {
        response.next._iteration = iteration() + 2;
        response.next._sub_iteration = _this->my_subiteration<int>();
        _this->write_stuff();
    }

    void write_stuff()
    {
        agent_itf agent;
        cout << typeid(this).name() << endl;
        agent_itf* pAgent = static_cast<agent_itf*>(this);
        cout << "Hello world, I am "<< pAgent->name() /*this->name<string>()*/ <<" this is iteration " << iteration()<<", subiteration "<<sub_iteration() << endl;
    }

    m_data(string, name, true, true);
    m_data(int, my_subiteration, true, true);
    m_data(double, money, true, true);
};

implementation struct other_agent_impl : public my_agent_impl<MasterType, INHERIT(other_agent_impl)>
{
    void initialize(int start_time)
    {
        this->template Load_Event<other_agent_impl>(&Do_stuff2, start_time, this->template my_subiteration<int>());
    }

    static void Do_stuff2(other_agent_impl* _this, Event_Response& response)
    {
        response.next._iteration = iteration() + 3;
        response.next._sub_iteration = _this->template my_subiteration<int>();
        _this->write_stuff();
    }

    typedef Agent<typename MasterType::agent_type> agent_itf;

    void write_stuff()
    {
        agent_itf agent;
        agent_itf* pAgent = static_cast<agent_itf*>(this);
#ifdef _MSC_VER
		const char* object_name = typeid(*this).name();
#else
		int status;
		char* object_name = abi::__cxa_demangle(typeid(*this).name(), 0,0,&status);
#endif

        if (this->_is_active) this->_is_active = false;
        else this->_is_active = true;
        cout << "Hello world, I am a " << object_name << " object and my name is '" << this->template name<string>() << "' this is iteration " << iteration() << ", subiteration " << sub_iteration() << endl;
        int test = this->template my_subiteration<int>();
    }

    m_data(bool, is_active, true, true);
    //m_data(double, money, true, true);
    agent_itf* anotherAgent;
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
        response.next._iteration = iteration() + 3;
        response.next._sub_iteration = 0;

        _this->Do_the_stuff();
    }
    void Do_the_stuff()
    {
        if (this->_the_agent->template is_active<bool>()) cout << "My agent is currently active..." << endl;
        else cout << "My agent is sleeping..." << endl;
    }

    //m_prototype(Agent, typename MasterType::agent_type, the_agent, true, true);
    //m_data(typename MasterType::agent_type, the_agent, true, true);
    typename MasterType::agent_type* _the_agent;

    m_container(std::vector<typename MasterType::agent_type*>, agents_container, NONE, NONE);

    //typedef  Link_Prototype<typename remove_pointer< typename Link_Prototype::get_type_of(agent_container)::value_type>::type>  _Agent_Interface;
    //typedef  Link_Prototype<typename decltype(_agent_container)::value_type>  _Agent_Interface;
    //typedef  Random_Access_Sequence< typename _Agent_Network_Interface::get_type_of(links_container), _Regular_Link_Interface3*> _Regular_Links_Container_Interface;
    //typedef  Random_Access_Sequence<typename remove_pointer<decltype(_agents_container)::value_type>::type> _Links_Agent_Container_Interface;

    //typedef Agent<typename type_of(Parent_Household)::get_type_of(network_reference)> agent_itf;
    //typedef Random_Access_Sequence<typename network_itf::get_type_of(activity_locations_container)> activity_locations_itf;

    void use_the_container()
    {
        for(int i=0; i<50;++i)
            _agents_container.push_back(Allocate<typename MasterType::agent_type>());
        cout << "Number of link agents: " << _agents_container.size() << endl;
    }
};

struct MasterType
{
    typedef other_agent_impl<MasterType> agent_type;
    typedef link_impl<MasterType> link_type;
	typedef base_agent_impl<MasterType> base_agent_type;
};

int main(int argc, char* argv[])
{
    //static_assert(is_same<decltype(declval<MasterType::agent_type>().name<int>(0)),void>::value, "no void.");
    //static_assert(is_same<decltype(declval<other_agent_impl<MasterType>>().name<int>(0)),void>::value, "WTF - no void.");
    // f;
    //typeof<f>;

    // There are several pre-set configurations
    Simulation_Configuration cfg;
    cfg.Single_Threaded_Setup(10);

    // The INITIALIZE_SIMULATION macro informs the Core of what type of simulation to build
    INITIALIZE_SIMULATION(cfg);

    typedef Agent<MasterType::agent_type> agent_itf;
    //typedef MasterType::agent_type agent_itf;
    agent_itf* my_agent = static_cast<agent_itf*>(Allocate<typename MasterType::agent_type>());
    my_agent->initialize(2, 2, "base");

    MasterType::link_type* my_link = Allocate<MasterType::link_type>();
    my_link->_the_agent = my_agent;
    my_link->initialize(1);
    my_link->use_the_container();

    RNG_Components::Implementations::MT_Normal<NT> nrm1;
    float p1 = nrm1.Cumulative_Distribution<float>(0.5);

//    float p2 = GLOBALS::Bivariate_Normal_Distribution->Cumulative_Distribution_Value<float>(0, 0, 1);

    START();

    // The simulation has finished
    cout << "Finished!" << endl;
    cout << "Press RETURN key to continue..." << endl;
    cin.ignore();

    return true;
}

#ifdef COMMANDLINE_ARGS
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/timer/timer.hpp>
#endif
using namespace std;
#include <iostream>
#include <iomanip>
#include <boost/format.hpp>
//#include <chrono>
#include <algorithm>
#include "version.h"
#include <tbb/tbb.h>
#include <tbb/scalable_allocator.h>
#include <cxxabi.h>
//#include "forms.h"
#include "myclock.h"


// Most polaris functionality can be found contained within the polaris namespace
//using namespace polaris;

struct options
{
	int num_iterations;
	int num_agents;
	int num_threads;
	std::string app_name;
};

#ifdef COMMANDLINE_ARGS
bool parse_commandline(int argc, char* argv[], options& opts);
#endif

///----------------------------------------------------------------------------------------------------
/// Revision - class for tracking event steps
///----------------------------------------------------------------------------------------------------

union Revision
{
    Revision():_revision(-1){};
    Revision(int revision,int sub_revision):_iteration(revision),_sub_iteration(sub_revision){};
    Revision(const Revision& copy):_revision(copy._revision){};
    Revision(const Revision&& copy):_revision(copy._revision){};

    inline bool operator == (const Revision& other) const{return _revision == other._revision;}
    inline bool operator != (const Revision& other) const{return _revision != other._revision;}
    inline bool operator < (const Revision& other) const{return _revision < other._revision;}
    inline bool operator > (const Revision& other) const{return _revision > other._revision;}
    inline bool operator <= (const Revision& other) const{return _revision <= other._revision;}
    inline bool operator >= (const Revision& other) const{return _revision >= other._revision;}

    inline void operator = (const long long val){_revision=val;}
    inline void operator = (const Revision& other){_revision=other._revision;}

    struct
    {
        int _sub_iteration;
        int _iteration;
    };

    long long _revision;
};

struct Event_Response
{
    Event_Response(){next._iteration=INT_MAX;next._sub_iteration=0;};

    Revision next;
};

typedef void (*Event)(void*,Event_Response&);

template<typename Derived>
struct ExecuteObject
{
    void handleEvent(Event_Response& response)
    {
        static_cast<Derived*>(this)->handleEvent_impl(response);
    }
};



template <typename Base, typename Derived>
class VehicleCloneable : public Base, public ExecuteObject<Derived>
{
public:
    using Base::Base;

    VehicleCloneable()
    {
        //std::cout << "Contructing " << static_cast<Derived const*>(this)->description() << std::endl;
    }

    virtual ~VehicleCloneable()
    {
        //std::cout << "Deleting " << static_cast<Derived const*>(this)->description() << std::endl;
    }

    virtual Base *clone() const
    {
        return new Derived(static_cast<Derived const &>(*this));
    }

    virtual void describe() const
    {
        std::cout << "I am a " << static_cast<Derived const*>(this)->description() << std::endl;
    }

    virtual void Initialize()
    {
        static_cast<Derived*>(this)->initialize_impl();
    }

//    virtual void handleEvent(Event_Response& response)
//    {
////        if(_event_callback != nullptr)
////            _event_callback(static_cast<Derived*>(this), response);
//        static_cast<Derived*>(this)->handleEvent_impl(response);
//    }


    void Load_Event(void (*p_event)(Derived*,Event_Response&),int start_iteration,int start_sub_iteration)
    {
        const Revision update_revision(start_iteration,start_sub_iteration);
        // Update the OPTEX
        _next_revision = update_revision;
        //_event_callback = (Event)p_event;
    }

private:
    Revision _next_revision;

    union
    {
        Event _event_callback;
        //Byte* _next_free_cell;
    };

};

class Vehicle
{
public:
    virtual ~Vehicle() {}

    virtual Vehicle *clone() const  = 0;

    virtual void describe() const = 0;

    virtual void Initialize() = 0;

    //virtual void handleEvent_impl(Event_Response& response) = 0;
};

class Car : public VehicleCloneable<Vehicle, Car>
{
public:\
    Car(){;}
    Car(const Car& car) : car_value(car.car_value) {;}
    std::string description() const
    {
        return "car";
    }

    void initialize_impl()
    {
        car_value=0;
    }

    void handleEvent_impl(Event_Response& response)
    {
        ++car_value;
        //std::cout << "Event handled by " << description() << " - " << car_value << std::endl;
    }

    int car_value;
};

class Plane : public VehicleCloneable<Vehicle, Plane>
{
public:
    std::string description() const
    {
        return "plane";
    }

    void initialize_impl()
    {
        plane_value = 0;
    }

    void handleEvent_impl(Event_Response& response)
    {
        ++plane_value;
        //std::cout << "Event handled by " << description() << " - " << plane_value << std::endl;
    }

    int plane_value;
};

class FighterPlane : public VehicleCloneable<Plane, FighterPlane>
{
public:
    std::string description() const
    {
        return "fighter plane";
    }

    void initialize_impl()
    {
        fighter_plane_value = 10;
        //std::cout << "Initialized " << description() << " - " << fighter_plane_value << std::endl;
    }

    void handleEvent_impl(Event_Response& response)
    {
        fighter_plane_value+=2;
        //std::cout << "Event handled by " << description() << " - " << fighter_plane_value << std::endl;
    }

    int fighter_plane_value;
};


class BaseComponentManager
{
public:
    virtual BaseComponentManager* getManager() = 0;
    virtual void Describe() = 0;

    virtual size_t get_size() = 0;
    virtual void handleEvent(size_t index, Event_Response& response) = 0;
};
static std::vector<BaseComponentManager*> ComponentManagerList;

template<typename Component>
class ComponentManager : public BaseComponentManager, public tbb::concurrent_vector<Component, tbb::scalable_allocator<Component> >
//class ComponentManager : public BaseComponentManager, public std::vector<Component,  tbb::scalable_allocator<Component> >
{
public:
    ComponentManager()
    {
        ComponentManagerList.push_back(this);
    }

    virtual BaseComponentManager* getManager() { return pManager; }
    virtual void Describe()
    {
        int status;
        char* manager_name = abi::__cxa_demangle(typeid(*this).name(), 0,0,&status);
        char* component_name = abi::__cxa_demangle(typeid(Component).name(), 0,0,&status);
        std::cout << manager_name << " has " << get_size() << " " << component_name << " elements."  << std::endl;
        free(manager_name);
        free(component_name);
    }

    static ComponentManager* pManager;

    virtual size_t get_size() { return this->size();}
    virtual void handleEvent(size_t index, Event_Response &response){ static_cast<Component&>(this->at(index)).handleEvent_impl(response); }
};
template<typename Component>
ComponentManager<Component>* ComponentManager<Component>::pManager = new ComponentManager<Component>();

template<typename Component>
Component* Allocate()
{
//    void* pMem = scalable_malloc(sizeof(Component));
//    Component* component =  new (pMem) Component();
    Component component;
    return &(*static_cast<ComponentManager<Component>*>(ComponentManager<Component>::pManager)->push_back(component));
    //return component;
}

template<typename Component>
Component* Emplace(Component& component)
{
    return static_cast<Component*>(static_cast<ComponentManager<Component>*>(ComponentManager<Component>::pManager)->emplace_back(component));
}

template<typename Component>
Component* Emplace()
{
    Component component;
    return &(*static_cast<ComponentManager<Component>*>(ComponentManager<Component>::pManager)->emplace_back(component));
}

void DescribeManagers()
{
    for (auto& manager : ComponentManagerList)
    {
        manager->getManager()->Describe();
    }
}

template <typename Base, typename Derived>
class AgentCloneable : public Base, public ExecuteObject<Derived>
{
public:
    using Base::Base;

    virtual Base *clone() const
    {
        return new Derived(static_cast<Derived const &>(*this));
    }

    void calculate(int x, int y) { /*std::cout << "AgentProto::Calculate called: x=" << x << " y=" << y << std::endl ;*/ static_cast<Derived*>(this)->calculate_impl(x, y); }
    template<typename DataType> DataType DoSomething(DataType input) { return static_cast<Derived*>(this)->DoSomething_impl(input); }

    int Answer()
    {
        //cout << boolalpha << check_it::has_receive<A,Ev, void>::value << endl;
        return static_cast<Derived*>(this)->Answer_impl();
    }

    void Answer(int ans)
    {
        static_cast<Derived*>(this)->Answer_impl(ans);
    }

    void describe() { static_cast<Derived*>(this)->describe_impl(); }

    //static Derived* Allocate(int uuid = -1)
    //{
    //	Derived* pAgent = polaris::Allocate<Derived>(uuid, false);
    //	Execution_Block* exec_block = pAgent->execution_block();
    //	new (pAgent) Derived();
    //	((Derived*)pAgent)->execution_block(exec_block);
    //	pAgent->_uuid = uuid;
    //	return pAgent;
    //}
};

template<typename ComponentType>
class AgentProto
{
//public:
//    void calculate(int x, int y) { /*std::cout << "AgentProto::Calculate called: x=" << x << " y=" << y << std::endl ;*/ static_cast<ComponentType*>(this)->calculate_impl(x, y); }
//    template<typename DataType> DataType DoSomething(DataType input) { return static_cast<ComponentType*>(this)->DoSomething_impl(input); }

//    int Answer()
//    {
//        //cout << boolalpha << check_it::has_receive<A,Ev, void>::value << endl;
//        return static_cast<ComponentType*>(this)->Answer_impl();
//    }

//    void Answer(int ans)
//    {
//        static_cast<ComponentType*>(this)->Answer_impl(ans);
//    }

//    void describe() { static_cast<ComponentType*>(this)->describe_impl(); }

//    //accessor(stuff, true, true);

};

//template<typename Derived = void>
class NewAgent : public AgentCloneable<AgentProto<NewAgent>, NewAgent >
{
public:
    NewAgent() : BaseClass() { ; }
    NewAgent(int _id) : id(_id), BaseClass() { ; }
    NewAgent(const NewAgent& agent) : BaseClass(agent), id(agent.id), _ans(agent._ans) { ; }
    NewAgent& operator=(const NewAgent& agent) { BaseClass::operator=(agent); _ans = agent._ans; return *this; }
    ~NewAgent() { ; }

    bool operator<(NewAgent a)
    {
        return a.id < id;
    }

    void calculate_impl(int x, int y)
    {
        //_ans = x + y;
        _ans++;
        //std::cout << "NewAgent::Calculate called: x=" << x << " y=" << y <<  " _ans=" << _ans << std::endl;
    }
    void handleEvent_impl(Event_Response& response)
    {
        //_ans = x + y;
        ++_ans;
        //std::cout << "NewAgent::Calculate called: x=" << x << " y=" << y <<  " _ans=" << _ans << std::endl;
    }
    template<typename DataType> DataType DoSomething_impl(DataType input) { return input * 2; }

    int Answer_impl()
    {
        return _ans;
    }

    void Answer_impl(int ans)
    {
        _ans = ans;
    }

    void describe_impl()
    {
        std::cout << "I am a New Agent" << std::endl;
    }

    //m_data(int, stuff, NONE, NONE);

private:
    typedef AgentCloneable<AgentProto<NewAgent>, NewAgent > BaseClass;
    friend class AgentProto< NewAgent >;
    int id;

    int _ans;
};

class DerivedAgent : public AgentCloneable<NewAgent, DerivedAgent>
{
public:
    void describe_impl()
    {
        std::cout << "I am a Derived Agent" << std::endl;

    }
};

struct MasterType
{
    typedef NewAgent tbb_agent_type;
};

class SimpleAgent : public ExecuteObject<SimpleAgent>
{
  public:
    void handleEvent_impl(Event_Response& response)
    {
        ++_ans;
        //std::cout << "Simple:: handleEvent_impl called" << std::endl;
    }

private:
    int _ans;
};

//template <typename Type>
//class has_foo
//{
//   class yes { char m;};
//   class no { yes m[2];};
//   struct BaseMixin
//   {
//     void foo(){}
//   };
//   struct Base : public Type, public BaseMixin {};
//   template <typename T, T t>  class Helper{};
//   template <typename U>
//   static no deduce(U*, Helper<void (BaseMixin::*)(), &U::foo>* = 0);
//   static yes deduce(...);
//public:
//   static const bool result = sizeof(yes) == sizeof(deduce((Base*)(0)));
//};

struct check_it
{
    template< typename C, typename Ev, typename = void >
    struct has_receive
      : std::false_type
    {};

    template< typename C, typename Ev >
    struct has_receive< C, Ev, typename std::enable_if<
        std::is_same<
            decltype( std::declval<C>().receive( std::declval<const Ev&>() ) ),
            void
        >::value
    >::type >
      : std::true_type
    {};
};

struct Ev{};
struct A
{
    void receive(const Ev& event);
};

struct B : A {};

struct C {};

// Helper macro to create traits to check if function exist.
// Note: template funcName should exist, see below for a work around.
#define HAS_TEMPLATED_FUNC(traitsName, funcName, Prototype)                          \
    template<typename U>                                                             \
    class traitsName                                                                 \
    {                                                                                \
        typedef std::uint8_t yes;                                                    \
        typedef std::uint16_t no;                                                    \
        template <typename T, T> struct type_check;                                  \
        template <typename T = U> static yes &chk(type_check<Prototype, &funcName>*); \
        template <typename > static no &chk(...);                                    \
    public:                                                                          \
        static bool const value = sizeof(chk<U>(0)) == sizeof(yes);                  \
    }

// namespace to test
namespace foo {
    template<typename T, typename U> T bar(U);
    // bar2 not present
}

// dummy class which should be never used
namespace detail {
    struct dummy;
}

// Trick, so the names exist.
// we use a specialization which should never happen
namespace foo {
    template <typename T, typename U>
    std::enable_if<std::is_same<detail::dummy, T>::value, T> bar(U);

    template <typename T, typename U>
    std::enable_if<std::is_same<detail::dummy, T>::value, T> bar2(U);
}

#define COMMA_ , // trick to be able to use ',' in macro

// Create the traits
HAS_TEMPLATED_FUNC(has_foo_bar, foo::bar<T COMMA_ int>, int(*)(int));
HAS_TEMPLATED_FUNC(has_foo_bar2, foo::bar2<T COMMA_ int>, int(*)(int));

// test them
static_assert(has_foo_bar<int>::value, "something went wrong");
static_assert(!has_foo_bar2<int>::value, "something went wrong");

template<typename ComponentType>
struct bowser : public ComponentType
{
public :
    void ans(int answer);
    int ans();

    std::enable_if<std::is_same<detail::dummy, ComponentType>::value, ComponentType> ans(int);
};
HAS_TEMPLATED_FUNC(has_ans, bowser::ans<ComponentType COMMA_ int>, int(*)(int));


int main(int argc, char* argv[])
{
    using namespace std;
    cout << boolalpha << check_it::has_receive<A,Ev, void>::value << endl;
    cout << boolalpha << check_it::has_receive<A,int, void>::value << endl;
    cout << boolalpha << check_it::has_receive<B,Ev, void>::value << endl;
    cout << boolalpha << check_it::has_receive<B,int, void>::value << endl;
    cout << boolalpha << check_it::has_receive<C,Ev, void>::value << endl;
    cout << boolalpha << check_it::has_receive<C,int, void>::value << endl;

    static_assert(check_it::has_receive<A,Ev, void>::value, "A.response(const Ev&) not found");
    //static_assert(check_it::has_receive<A,int, void>::value, "A.response(const int&) not found");

	options opts;
	opts.num_iterations = 1000;
	opts.num_agents = 1;
	opts.num_threads = 1;

#ifdef COMMANDLINE_ARGS
	bool bContinue = parse_commandline( argc, argv, opts);
	if (!bContinue)
		return 0;
#endif
	std::cout << boost::str(boost::format("running %s with:\n\tnum_iterations=%d\n\tnum_agents=%d\n\tnum_threads=%d") % opts.app_name % opts.num_iterations % opts.num_agents % opts.num_threads) << std::endl;
	
	// There are several pre-set configurations
    //     for this case configure a single threaded simulation which runs for 10 iterations

    Vehicle* car = new Car();
    car->Initialize();
    car->describe();
    Vehicle* car2 = car->clone();
    car2->describe();

    Vehicle* plane = new Plane();
    plane->describe();
    Vehicle* plane2 = plane->clone();
    plane2->describe();

    Vehicle* fighterplane = new FighterPlane();
    fighterplane->describe();
    Vehicle* fp2 = fighterplane->clone();
    fp2->describe();

    NewAgent* na = new NewAgent();
    na->describe();

    DerivedAgent* da = new DerivedAgent();
    da->describe();

    delete car;
    delete car2;
    delete plane;
    delete plane2;
    delete fighterplane;
    delete fp2;
    delete na;
    delete da;

    {
        myAutoClock timer("Allocate Part 1");
        for(int i=0;i<opts.num_agents/4;++i)
        {
            Car* car = Allocate<Car>();
            car->Initialize();
        }
        for(int i=0;i<opts.num_agents/4;++i)
        {
            Plane* plane = Allocate<Plane>();
            plane->Initialize();
        }
        for(int i=0;i<opts.num_agents/4;++i)
        {
            FighterPlane* fp = Allocate<FighterPlane>();
            fp->Initialize();
        }
        for(int i=0;i<opts.num_agents/4;++i)
        {
            MasterType::tbb_agent_type* pAgent = Allocate<MasterType::tbb_agent_type>();
            //pAgent->Initialize();
            pAgent->Answer(i);
        }
        for(int i=0;i<opts.num_agents/4;++i)
        {
            DerivedAgent* pAgent = Allocate<DerivedAgent>();
            //pAgent->Initialize();
            pAgent->Answer(i);
        }
//        for(int i=0;i<opts.num_agents/4;++i)
//        {
//            SimpleAgent* pAgent = Allocate<SimpleAgent>();
//            //pAgent->Initialize();
//            pAgent->Answer(i);
//        }
    }

    DescribeManagers();

    Event_Response response;

    {
        myAutoClock timer("Interface Execute Part 1");
        for(int iteration=0; iteration<opts.num_iterations;++iteration)
        {
            for(auto& manager : ComponentManagerList)
            {
                tbb::parallel_for(size_t(0), manager->getManager()->get_size(),
                    [&](size_t index) { manager->getManager()->handleEvent(index, response); });
            }
        }
    }

//    {
//        myAutoClock timer("Implementation Execute Part 1");
//        for(int iteration=0; iteration<opts.num_iterations;++iteration)
//        {
//            tbb::parallel_for(size_t(0), size_t(ComponentManager<Car>::pManager->size()),
//                [&](size_t index) { ComponentManager<Car>::pManager->at(index)->handleEvent_impl(response); });
//        }
//    }

//    std::vector < MasterType::tbb_agent_type*, tbb::scalable_allocator< MasterType::tbb_agent_type>> vec;
//    {
//        myAutoClock timer("Allocate Part 2");
//        for (int i = 0; i < opts.num_agents; ++i)
//        {
//            void* pMem = scalable_malloc(sizeof(MasterType::tbb_agent_type));
//            MasterType::tbb_agent_type* pAgent = new (pMem) MasterType::tbb_agent_type(i);
//            vec.push_back(pAgent);
//        }
//    }

//    {
//        myAutoClock timer("Execute Part 2");
//        for (int i = 0; i < opts.num_iterations; ++i)
//        {
//            tbb::parallel_for(size_t(0), size_t(vec.size()),
//                [&](size_t index) { vec[index]->calculate((int)index, (int)index + 1); });
//        }
//    }

    // The simulation has finished
	cout << "Finished!" << endl;
	cout << "Press RETURN key to continue..." << endl;
	cin.ignore();

	return 0;
}

#ifdef COMMANDLINE_ARGS

const int MAX_ITERATIONS = 100000;
const int MAX_AGENTS = 4000000;
const int MAX_THREADS = 100;

bool parse_commandline(int argc, char* argv[], options& opts)
{
	boost::filesystem::path p(argv[0]);
	opts.app_name = p.filename().string();
	namespace po = boost::program_options;
	po::options_description general("General options");
	general.add_options()
        ("help,h", "display help message")
        ("version,v", "display version information")
	;

	po::options_description app("Application options");
	app.add_options()
        ("num_iterations,i", po::value<int>(), "set number of interations")
        ("num_agents,a", po::value<int>(), "set number of agents")
        ("num_threads,t", po::value<int>(), "set number of threads")
	;

	po::options_description all("Allowed options");
	all.add(general).add(app);


	po::variables_map vm;
	try
	{
		po::store(po::parse_command_line(argc, argv, all), vm);
		po::notify(vm); 
	}
	catch(...)
	{
		std::cout << std::string("ERROR parsing command line arguments...");
		return false;
	}

	if (vm.count("help"))
	{
		cout << Version::get_version(opts.app_name) << std::endl;
		cout << all << "\n";
		return false;
	}

	if (vm.count("version"))
	{
		cout << Version::get_version(opts.app_name) << std::endl;
		return false;
	}

	if (vm.count("num_iterations"))
	{
		int iters = vm["num_iterations"].as<int>();
		if (iters > 0 && iters <= MAX_ITERATIONS)
		{
			opts.num_iterations = iters;
		}
		else
		{
			std::cout << boost::str(boost::format("ERROR: num_iterations (%d) is out of range (1-%d).\n") % iters % MAX_ITERATIONS);
			return false;
		}
	}

	if (vm.count("num_agents"))
	{
		int agents = vm["num_agents"].as<int>();
		if (agents > 0 && agents <= MAX_AGENTS)
		{
			opts.num_agents = agents;
		}
		else
		{
			std::cout << boost::str(boost::format("ERROR: num_agents (%d) is out of range (1-%d).\n") % agents % MAX_AGENTS);
			return false;
		}
	}

	if (vm.count("num_threads"))
	{
		int threads = vm["num_threads"].as<int>();
		if (threads > 0 && threads <= MAX_THREADS)
		{
			opts.num_threads = threads;
		}
		else
		{
			std::cout << boost::str(boost::format("ERROR: num_threads (%d) is out of range (1-%d).\n") % threads % MAX_THREADS);
			return false;
		}
	}

	return true;
}
#endif // COMMANDLINE_ARGS

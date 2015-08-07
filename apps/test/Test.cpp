#ifdef ANTARES
#include "Antares/Antares.h"
#endif
#include "repository/Repository.h"
using namespace polaris;


// Test agent
prototype struct Agent ADD_DEBUG_INFO
{
	tag_as_prototype;


	template<typename T> void Initialize(T i, requires(T,check(strip_modifiers(T),is_integral)))
	{
		this_component()->Initialize(i);
		this_component()->Load_Event<ComponentType>(&Agent_Event,1,0);
	}
	template<typename T> void Initialize(T i, requires(T,!check(strip_modifiers(T),is_integral)))
	{
		cout <<"OOPS, NOT AN INTEGRAL DATA TYPE.";
	}

	accessor(data,check(strip_modifiers(TargetType),is_integral),check(strip_modifiers(TargetType),is_integral));
	accessor(x,NONE,NONE);
	accessor(y,NONE,NONE);
	accessor(my_agent,NONE,NONE);
	accessor(something_else,NONE,NONE);

	static void Agent_Event(ComponentType* _this,Event_Response& response)
	{

		response.next._iteration = iteration() + 1;
		response.next._sub_iteration = 0;
		_this->Do_Event();
	}
};
#ifdef ANTARES
implementation struct Antares_Agent_Implementation : public Polaris_Component<MasterType,INHERIT(Antares_Agent_Implementation),Execution_Object>
{
	// Member data
	m_data(int,data,NONE,NONE);
	m_data(float,x,NONE,NONE);
	m_data(float,y,NONE,NONE);
	m_data(True_Color_RGB<NT>,color,NONE,NONE);
	static Antares_Layer<typename MasterType::antares_layer_type>* _base_agent_layer;

	// static initializer for the type - call once at program start to initialize static variables
	static void Initialize_Type()
	{
		_base_agent_layer = Allocate_New_Layer<MT>(string("Base Agent"));
		Antares_Layer_Configuration cfg;
		cfg.Configure_Dynamic_Points();
		cfg.Configure_Dynamic_Points();
		cfg.target_sub_iteration = 1;
		cfg.head_size_value = 20;
		cfg.primitive_color = true;
		cfg.draw=true;
		_base_agent_layer->Initialize<NT>(cfg);
	}

	// Member functions
	void Initialize(int i)
	{
		// set initial values for member variables
		_data = 1;
		_x = GLOBALS::Uniform_RNG.Next_Rand<float>()*1000;
		_y = GLOBALS::Uniform_RNG.Next_Rand<float>()*1000;

		int r = i % 255;
		_color.r(255-r); _color.g(0); _color.b(r);
	}
	void Do_Event(Event_Response& response)
	{
		// Code to execute when the event fires
		_x += 150.0 * (GLOBALS::Uniform_RNG.Next_Rand<float>() - 0.5);
		_y += 150.0 * (GLOBALS::Uniform_RNG.Next_Rand<float>()  - 0.5);
		Draw();
	}
	void Draw()
	{
		Point_Element_Colored p;
		p._point.x(_x);
		p._point.y(_y);
		p._color._r=_color._r;
		p._color._g=_color._g;
		p._color._b=_color._b;
		Scale_Coordinates<MT>(p._point);
		_base_agent_layer->Push_Element<Regular_Element>(&p);
	}
};
#endif
implementation struct Base_Agent_Implementation : public Polaris_Component<MasterType,INHERIT(Base_Agent_Implementation),Execution_Object>
{
	// Member data
	m_data(int,data,NONE,NONE);
	m_data(float,x,NONE,NONE);
	m_data(float,y,NONE,NONE);
	m_prototype(Agent, typename MasterType::other_agent_type, my_agent, NONE,NONE);
	m_data(True_Color_RGB<NT>,color,NONE,NONE);
	static Antares_Layer<typename MasterType::antares_layer_type>* _base_agent_layer;

	// static initializer for the type - call once at program start to initialize static variables
	static void Initialize_Type()
	{
		_base_agent_layer = Allocate_New_Layer<MT>(string("Base Agent"));
		Antares_Layer_Configuration cfg;
		cfg.Configure_Dynamic_Points();
		cfg.Configure_Dynamic_Points();
		cfg.target_sub_iteration = 1;
		cfg.head_size_value = 20;
		cfg.primitive_color = true;
		cfg.draw=true;
		_base_agent_layer->Initialize<NT>(cfg);
	}

	void Initialize(int i)
	{
		// set initial values for member variables
		_data = 1;
		_x = GLOBALS::Uniform_RNG.Next_Rand<float>()*1000;
		_y = GLOBALS::Uniform_RNG.Next_Rand<float>()*1000;
		_color.r(255); _color.g(0); _color.b(0);
	}
	void Do_Event()
	{
		// Code to execute when the event fires
		_x += 150.0 * (GLOBALS::Uniform_RNG.Next_Rand<float>() - 0.5);
		_y += 150.0 * (GLOBALS::Uniform_RNG.Next_Rand<float>()  - 0.5);
		if (_x >= 1000) _x = 1000;
		if (_x <= -1000) _x = -1000;
		if (_y >= 500) _y = 500;
		if (_y <= -500) _y = -500;
		
		Draw();
	}
	void Draw()
	{
		Point_Element_Colored p;
		p._point.x(_x);
		p._point.y(_y);
		p._color._r=_color._r;
		p._color._g=_color._g;
		p._color._b=_color._b;
		Scale_Coordinates<MT>(p._point);
		_base_agent_layer->Push_Element<Regular_Element>(&p);
	}
};
template<typename MasterType,typename InheritanceList> Antares_Layer<typename MasterType::antares_layer_type>* Base_Agent_Implementation<MasterType,InheritanceList>::_base_agent_layer;
implementation struct Other_Agent_Implementation : public Base_Agent_Implementation<MasterType,INHERIT(Other_Agent_Implementation)>
{
	typedef Base_Agent_Implementation<MasterType,INHERIT(Other_Agent_Implementation)> base_type;
	// Member data

	m_data(float,z,NONE,NONE);
	m_prototype(Agent, typename MasterType::agent_type, my_agent, NONE,NONE);
			
	member_component_and_feature_accessor(length, Value, Basic_Units::Prototypes::Length,Basic_Units::Implementations::Length_Implementation<NT>);

	m_data(True_Color_RGB<NT>,color,NONE,NONE);
	static Antares_Layer<typename MasterType::antares_layer_type>* _other_agent_layer;

	// static initializer for the type - call once at program start to initialize static variables
	static void Initialize_Type()
	{
		_base_agent_layer = Allocate_New_Layer<MT>(string("Other Agent"));
		Antares_Layer_Configuration cfg;
		cfg.Configure_Dynamic_Points();
		cfg.Configure_Dynamic_Points();
		cfg.target_sub_iteration = 1;
		cfg.head_size_value = 20;
		cfg.primitive_color = true;
		cfg.draw=true;
		_base_agent_layer->Initialize<NT>(cfg);
	}

	void Initialize(int i)
	{
		this->length<Basic_Units::Length_Variables::Feet>(10.0);

		base_type::Initialize(i);

		// set initial values for member variables
		_z = GLOBALS::Uniform_RNG.Next_Rand<float>()*10;

		_color.r(0); _color.g(0); _color.b(255);
	}
	void Do_Event()
	{
		// Code to execute when the event fires
		float d_x = base_type::_x - this->_my_agent->x<float>();
		float d_y = base_type::_y - this->_my_agent->y<float>();
		float len = sqrt(pow(d_x,2) + pow(d_y,2));

		float dd_x = -d_x/len*10;
		float dd_y = -d_y/len*10;
		base_type::_x += dd_x;
		base_type::_y += dd_y;

		cout << "Base Agent is at {"<<this->_my_agent->x<float>() << ", " <<this->_my_agent->y<float>() << "} ; Agent is at " <<base_type::_x << ", " <<base_type::_y<<endl;

		Draw();
	}
	void Draw()
	{
		Point_Element_Colored p;
		p._point.x(_x);
		p._point.y(_y);
		p._color._r=_color._r;
		p._color._g=_color._g;
		p._color._b=_color._b;
		Scale_Coordinates<MT>(p._point);
		_base_agent_layer->Push_Element<Regular_Element>(&p);
	}
};
template<typename MasterType,typename InheritanceList> Antares_Layer<typename MasterType::antares_layer_type>* Other_Agent_Implementation<MasterType,InheritanceList>::_other_agent_layer;

implementation struct Some_Other_Thing : public Polaris_Component<MasterType,INHERIT(Some_Other_Thing),Execution_Object>
{
	m_data(int,stuff,NONE,NONE);
	m_data(int,x,NONE,NONE);
	void Do_Event()
	{
		// Code to execute when the event fires
	}
};

struct MasterType
{
	//=================================================================================
	// REQUIRED ANTARES TYPES
	#ifdef ANTARES
	typedef Conductor_Implementation<MasterType> conductor_type;
	typedef Control_Panel_Implementation<MasterType> control_panel_type;
	typedef Time_Panel_Implementation<MasterType> time_panel_type;
	typedef Information_Panel_Implementation<MasterType> information_panel_type;
	typedef Canvas_Implementation<MasterType> canvas_type;
	typedef Antares_Layer_Implementation<MasterType> antares_layer_type;
	typedef Layer_Options_Implementation<MasterType> layer_options_type;
	typedef Attributes_Panel_Implementation<MasterType> attributes_panel_type;
	typedef Control_Dialog_Implementation<MasterType> control_dialog_type;
	typedef Information_Page_Implementation<MasterType> information_page_type;
	typedef Splash_Panel_Implementation<MasterType> splash_panel_type;
	//=================================================================================
	#endif
	typedef MasterType M;
	// Add all of the types used in your code here
	typedef Base_Agent_Implementation<M> agent_type;
	typedef Other_Agent_Implementation<M> other_agent_type;
	typedef Some_Other_Thing<M> some_other_type;
	
};


int main(int argc, char *argv[])
{
	//----------------------------------------------------------
	// Initialize basic simulation 
	Simulation_Configuration cfg;
	cfg.Multi_Threaded_Setup(10000, 1);
	INITIALIZE_SIMULATION(cfg);


	// initialize the visualizer environment
	#ifdef ANTARES
	START_UI(MasterType,0,0,1000,1000);
	#endif

	// Standard initialization of random number generator
	GLOBALS::Uniform_RNG.Initialize();
	GLOBALS::Uniform_RNG.Set_Seed<int>();



	//----------------------------------------------------------
	// Your code here

	// Initialize drawing layers - always need to call static initializer functions this way
	MasterType::agent_type::Initialize_Type();
	MasterType::other_agent_type::Initialize_Type();

	// define an interface to use
	typedef Agent<MasterType::agent_type> agent_itf;
	typedef Agent<MasterType::other_agent_type> other_agent_itf;
	
	// Create agents and initialize them
	for (int i = 0; i < 1; ++i)
	{
		agent_itf* base_agent = (agent_itf*)Allocate<MasterType::agent_type>();
		base_agent->Initialize<int>(i);
		

		other_agent_itf* other_agent = (other_agent_itf*)Allocate<MasterType::other_agent_type>();
		other_agent->Initialize<int>(i);
		other_agent->my_agent(base_agent);
		base_agent->my_agent(other_agent);

		Agent<MasterType::some_other_type>* other_thing = (Agent<MasterType::some_other_type>*)Allocate<MasterType::some_other_type>();
		other_thing->x<int>(1);
	}


	START();
	char test;
	cin >> test;
}

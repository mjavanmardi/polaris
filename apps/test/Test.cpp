#include "Antares/Antares.h"
#include "repository/Repository.h"
using namespace polaris;


// Test agent
prototype struct Agent ADD_DEBUG_INFO
{
	tag_as_prototype;
	template<typename T> void Initialize(int i)
	{
		this_component()->Initialize(i);
		this_component()->Load_Event<ComponentType>(&Agent_Event,1,0);
	}
	static void Agent_Event(ComponentType* _this,Event_Response& response)
	{
		response.next._iteration = iteration() + 1;
		response.next._sub_iteration = 0;
		_this->Do_Event(response);
	}
};
implementation struct Base_Agent_Implementation : public Polaris_Component<MasterType,INHERIT(Base_Agent_Implementation),Execution_Object>
{
	void Initialize(int i)
	{
		_data = 1;
		_x = GLOBALS::Uniform_RNG.Next_Rand<float>()*1000;
		_y = GLOBALS::Uniform_RNG.Next_Rand<float>()*1000;

		int r = i % 255;
		_color.r(255-r); _color.g(0); _color.b(r);
	}
	void Do_Event(Event_Response& response)
	{
		//cout <<"BASE_AGENT: event fired at " << iteration()<<", next event to fire at " << response.next._iteration<<endl;
		_x += 150.0 * (GLOBALS::Uniform_RNG.Next_Rand<float>() - 0.5);
		_y += 150.0 * (GLOBALS::Uniform_RNG.Next_Rand<float>()  - 0.5);
		Draw();
	}
	m_data(int,data,NONE,NONE);
	m_data(float,x,NONE,NONE);
	m_data(float,y,NONE,NONE);
	m_data(True_Color_RGB<NT>,color,NONE,NONE);

	// Functions
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
	// Drawing functions for 3D vehicle layer
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
	static Antares_Layer<typename MasterType::antares_layer_type>* _base_agent_layer;
};
// As antares layers are static they also need to be defined outside of the class body, NOTE* this is true for any static member variable
template<typename MasterType,typename InheritanceList> Antares_Layer<typename MasterType::antares_layer_type>* Base_Agent_Implementation<MasterType,InheritanceList>::_base_agent_layer;


struct MasterType
{
	//=================================================================================
	// REQUIRED ANTARES TYPES
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

	typedef MasterType M;
	typedef Base_Agent_Implementation<M> base_agent_type;
	// Add all of the types used in your code here
};


int main(int argc, char *argv[])
{
	//----------------------------------------------------------
	// Initialize basic simulation 
	Simulation_Configuration cfg;
	cfg.Multi_Threaded_Setup(10000, 1);
	INITIALIZE_SIMULATION(cfg);


	// initialize the visualizer environment
	START_UI(MasterType,0,0,1000,1000);


	// Standard initialization of random number generator
	GLOBALS::Uniform_RNG.Initialize();
	GLOBALS::Uniform_RNG.Set_Seed<int>();



	//----------------------------------------------------------
	// Your code here



	//typedef Helper<MasterType::helper_type> helper_itf;
	//helper_itf* main_agent = (helper_itf*)Allocate<MasterType::helper_type>();

	
	//---------------------------------------------------
	// Initialize drawing layers - always need to call static initializer functions this way
	MasterType::base_agent_type::Initialize_Type();

	typedef Agent<MasterType::base_agent_type> agent_itf;

	for (int i = 0; i < 1500; ++i)
	{
		agent_itf* main_agent = (agent_itf*)Allocate<MasterType::base_agent_type>();
		main_agent->Initialize<NT>(i);
	}


	START();
	char test;
	cin >> test;
}

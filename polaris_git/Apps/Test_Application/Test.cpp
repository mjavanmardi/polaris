#include "Antares.h"

using namespace polaris;


template<typename MasterType,typename InheritanceList = NULLTYPELIST>
struct Agent_Implementation : public Polaris_Component<MasterType,INHERIT(Agent_Implementation),Data_Object>
{
	static void Initialize_Type()
	{
		_vehicles_layer = Allocate_New_Layer<MT>(string("Vehicles"));

		Antares_Layer_Configuration cfg;

		cfg.Configure_Dynamic_Points();
		cfg.target_sub_iteration = 1;
		cfg.draw=true;

		_vehicles_layer->Initialize<NT>(cfg);
	}

	void Initialize()
	{
		_offset = 50.0f;

		//Load_Event<Agent_Implementation>(&Do_Stuff,0,0);
	}

	void Move()
	{
		cout << "Hiya" << endl;

		_offset += (rand()%10);
		
		if(_offset < 950.0f && _offset > 50.0f)
		{
			Point_3D<NT> position;
			position._x = 500;
			position._y = _offset;
			position._z = 5;

			Scale_Coordinates<MT>(position);

			_vehicles_layer->Push_Element<Regular_Element>(&position);
		}
		else if(_offset >= 950.0f)
		{
			_offset = -300.0f;
		}

		Sleep(100);
	}

	static void Do_Stuff(Agent_Implementation* _this,Event_Response& response)
	{
		response.next._iteration = iteration() + 1;
		response.next._sub_iteration = 0;

		_this->Move();
	}

	float _offset;

	static Antares_Layer<typename MasterType::antares_layer_type>* _vehicles_layer;
};

template<typename MasterType,typename InheritanceList>
Antares_Layer<typename MasterType::antares_layer_type>* Agent_Implementation<MasterType,InheritanceList>::_vehicles_layer;

struct MasterType
{
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

	typedef Agent_Implementation<MasterType> agent_type;
};

int main()
{
	Simulation_Configuration cfg;
	cfg.Multi_Threaded_Setup(100000,6);
	INITIALIZE_SIMULATION(cfg);
	
	//START_UI(MasterType,0,0,1000,1000);
	
	//MasterType::agent_type::Initialize_Type();

	MasterType::agent_type* agent = Allocate<MasterType::agent_type>(1);

	agent->Initialize();

	cout << Object_Lookup<MasterType::agent_type>(1)->_offset << endl;

	
	//START();
}
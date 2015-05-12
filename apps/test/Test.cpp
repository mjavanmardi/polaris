#include "Antares/Antares.h"
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
		if (iteration() < 20)
		{
			response.next._iteration = 20;
			response.next._sub_iteration = 0;
			_this->Do_Event(response);
		}
		else
		{
			response.next._iteration = END;
			response.next._sub_iteration = END;
		}
		
	}
	void Reschedule_Agent(int iter)
	{
		this_component()->Reschedule<ComponentType>(iter, 0);
	}
	void Reschedule_Agent(int iter, Event_Response& response)
	{
		this_component()->Reschedule<ComponentType>(iter, 0);
		response.next._iteration = iter;
		response.next._sub_iteration = 0;
	}
};
implementation struct Base_Agent_Implementation : public Polaris_Component<MasterType,INHERIT(Base_Agent_Implementation),Execution_Object>
{
	void Initialize()
	{
		_data = 1;
	}
	void Do_Event(Event_Response& response)
	{
		cout <<"BASE_AGENT: event fired at " << iteration()<<", next event to fire at " << response.next._iteration<<endl;
	}
	m_data(int,data,NONE,NONE);

	// Functions
	static void Initialize_Type()
	{
		_base_agent_layer = Allocate_New_Layer<MT>(string("Base Agent"));

		Antares_Layer_Configuration cfg =;
		cfg.Configure_Dynamic_Points();
		_base_agent_layer->Initialize<NT>(cfg);
	}
	// Drawing functions for 3D vehicle layer
	void Draw(float rotation_rad, True_Color_RGBA<NT> _color)
	{
		/// DRAW BOTTOM ///
		float x = _centroid._x;
		float y = _centroid._y;
		float z = 5.0f;
		Quad_Element_Colored bottom_elem;
		bottom_elem._color._r = _color._r;
		bottom_elem._color._g = _color._g;
		bottom_elem._color._b = _color._b;
		bottom_elem._color._a = _color._a;

		float cosa = cos(this->_current_link->heading());
		float sina = sin(this->_current_link->heading());

		bottom_elem._v1._x = x + _length/2.0 * cosa - _width/2.0 * sina ;
		bottom_elem._v1._y = y + _length/2.0 * sina + _width/2.0 * cosa ;
		bottom_elem._v1._z = z;
		bottom_elem._v2._x = x - _length/2.0 * cosa - _width/2.0  * sina;
		bottom_elem._v2._y = y - _length/2.0 * sina + _width/2.0  * cosa;
		bottom_elem._v2._z = z;
		bottom_elem._v3._x = x - _length/2.0 * cosa + _width/2.0 * sina;
		bottom_elem._v3._y = y - _length/2.0 * sina - _width/2.0 * cosa;
		bottom_elem._v3._z = z;
		bottom_elem._v4._x = x + _length/2.0 * cosa + _width/2.0 * sina;
		bottom_elem._v4._y = y + _length/2.0 * sina - _width/2.0 * cosa;
		bottom_elem._v4._z = z;

		Scale_Coordinates<MT>(bottom_elem._v1);
		Scale_Coordinates<MT>(bottom_elem._v2);
		Scale_Coordinates<MT>(bottom_elem._v3);
		Scale_Coordinates<MT>(bottom_elem._v4);

		_vehicles_layer->Push_Element<Regular_Element>(&bottom_elem);


		/// DRAW TOP ///
		Quad_Element_Colored top_elem;
		top_elem._color._r = _color._r;
		top_elem._color._g = _color._g;
		top_elem._color._b = _color._b;
		top_elem._color._a = _color._a;

		top_elem._v1._x = x + (_length/2.0- 30.0) * cosa - _width/2.0 * sina ;
		top_elem._v1._y = y + (_length/2.0- 30.0) * sina + _width/2.0 * cosa;
		top_elem._v1._z = z + _height;
		top_elem._v2._x = x - _length/2.0 * cosa - _width/2.0 * sina ;
		top_elem._v2._y = y - _length/2.0 * sina + _width/2.0 * cosa;
		top_elem._v2._z = z + _height;
		top_elem._v3._x = x - _length/2.0 * cosa + _width/2.0 * sina ;
		top_elem._v3._y = y - _length/2.0 * sina - _width/2.0 * cosa;
		top_elem._v3._z = z + _height;
		top_elem._v4._x = x + (_length/2.0 - 30.0) * cosa + _width/2.0 * sina ;
		top_elem._v4._y = y + (_length/2.0 - 30.0) * sina - _width/2.0 * cosa;
		top_elem._v4._z = z + _height;

		Scale_Coordinates<MT>(top_elem._v1);
		Scale_Coordinates<MT>(top_elem._v2);
		Scale_Coordinates<MT>(top_elem._v3);
		Scale_Coordinates<MT>(top_elem._v4);

		_vehicles_layer->Push_Element<Regular_Element>(&top_elem);

		/// DRAW FRONT ///
		Quad_Element_Colored front_elem;
		front_elem._color._r = _color._r;
		front_elem._color._g = _color._g;
		front_elem._color._b = _color._b;
		front_elem._color._a = _color._a;

		front_elem._v1._x = x + _length/2.0 * cosa - _width/2.0 * sina; //_length/2.0;
		front_elem._v1._y = y + _length/2.0 * sina + _width/2.0 * cosa; //_width/2.0;
		front_elem._v1._z = z;
		front_elem._v2._x = x + (_length/2.0 - 15.0) * cosa - _width/2.0 * sina; //(_length/2.0 - 15.0)
		front_elem._v2._y = y + (_length/2.0 - 15.0) * sina + _width/2.0 * cosa;//_width/2.0;
		front_elem._v2._z = z + _height/2.0;
		front_elem._v3._x = x + (_length/2.0 - 15.0) * cosa + _width/2.0 * sina;//_length/2.0 - 15.0;
		front_elem._v3._y = y + (_length/2.0 - 15.0) * sina - _width/2.0 * cosa;//-_width/2.0;
		front_elem._v3._z = z + _height/2.0;
		front_elem._v4._x = x + _length/2.0 * cosa + _width/2.0 * sina; //_length/2.0;
		front_elem._v4._y = y + _length/2.0 * sina - _width/2.0 * cosa; //-_width/2.0;
		front_elem._v4._z = z;

		Scale_Coordinates<MT>(front_elem._v1);
		Scale_Coordinates<MT>(front_elem._v2);
		Scale_Coordinates<MT>(front_elem._v3);
		Scale_Coordinates<MT>(front_elem._v4);

		_vehicles_layer->Push_Element<Regular_Element>(&front_elem);

		/// DRAW FRONT ///
		Quad_Element_Colored front_window_elem;
		front_window_elem._color._r = 170;
		front_window_elem._color._g = 170;
		front_window_elem._color._b = 255;
		front_window_elem._color._a = 255;

		front_window_elem._v1._x = x + (_length/2.0 - 15.0) * cosa - _width/2.0 * sina;//_length/2.0 - 15.0;
		front_window_elem._v1._y = y + (_length/2.0 - 15.0) * sina + _width/2.0 * cosa;//_width/2.0;
		front_window_elem._v1._z = z + _height/2.0;
		front_window_elem._v2._x = x + (_length/2.0 - 30.0) * cosa - _width/2.0 * sina;//_length/2.0 - 30.0;
		front_window_elem._v2._y = y + (_length/2.0 - 30.0) * sina + _width/2.0 * cosa;//_width/2.0;
		front_window_elem._v2._z = z + _height;
		front_window_elem._v3._x = x + (_length/2.0 - 30.0) * cosa + _width/2.0 * sina;//_length/2.0 - 30.0;
		front_window_elem._v3._y = y + (_length/2.0 - 30.0) * sina - _width/2.0 * cosa;//- _width/2.0;
		front_window_elem._v3._z = z + _height;
		front_window_elem._v4._x = x + (_length/2.0 - 15.0) * cosa + _width/2.0 * sina;//_length/2.0 - 15.0;
		front_window_elem._v4._y = y + (_length/2.0 - 15.0) * sina - _width/2.0 * cosa;//- _width/2.0;
		front_window_elem._v4._z = z + _height/2.0;

		Scale_Coordinates<MT>(front_window_elem._v1);
		Scale_Coordinates<MT>(front_window_elem._v2);
		Scale_Coordinates<MT>(front_window_elem._v3);
		Scale_Coordinates<MT>(front_window_elem._v4);

		_vehicles_layer->Push_Element<Regular_Element>(&front_window_elem);

		/// DRAW PASSENGER SIDE ///
		Quad_Element_Colored pass_elem;
		pass_elem._color._r = _color._r;
		pass_elem._color._g = _color._g;
		pass_elem._color._b = _color._b-20;
		pass_elem._color._a = _color._a;

		pass_elem._v1._x = x + _length/2.0 * cosa + _width/2.0 * sina; //_length/2.0;
		pass_elem._v1._y = y + _length/2.0 * sina - _width/2.0 * cosa;//- _width/2.0;
		pass_elem._v1._z = z;
		pass_elem._v2._x = x + (_length/2.0 - 30.0) * cosa + _width/2.0 * sina;//_length/2.0 - 30.0;
		pass_elem._v2._y = y + (_length/2.0 - 30.0) * sina - _width/2.0 * cosa;//- _width/2.0;
		pass_elem._v2._z = z + _height;
		pass_elem._v3._x = x - _length/2.0 * cosa + _width/2.0 * sina;// - _length/2.0;
		pass_elem._v3._y = y - _length/2.0 * sina - _width/2.0 * cosa;// - _width/2.0;
		pass_elem._v3._z = z + _height;
		pass_elem._v4._x = x - _length/2.0 * cosa + _width/2.0 * sina;//- _length/2.0;
		pass_elem._v4._y = y - _length/2.0 * sina - _width/2.0 * cosa;//- _width/2.0;
		pass_elem._v4._z = z;

		Scale_Coordinates<MT>(pass_elem._v1);
		Scale_Coordinates<MT>(pass_elem._v2);
		Scale_Coordinates<MT>(pass_elem._v3);
		Scale_Coordinates<MT>(pass_elem._v4);

		_vehicles_layer->Push_Element<Regular_Element>(&pass_elem);

		/// DRAW DRIVER SIDE ///
		Quad_Element_Colored driver_elem;
		driver_elem._color._r = _color._r;
		driver_elem._color._g = _color._g;
		driver_elem._color._b = _color._b-20;
		driver_elem._color._a = _color._a;

		driver_elem._v1._x = x + _length/2.0 * cosa - _width/2.0 * sina; //_length/2.0;
		driver_elem._v1._y = y + _length/2.0 * sina + _width/2.0 * cosa; //_width/2.0;
		driver_elem._v1._z = z;
		driver_elem._v2._x = x + (_length/2.0 - 30.0) * cosa - _width/2.0 * sina; //_length/2.0 - 30.0;
		driver_elem._v2._y = y + (_length/2.0 - 30.0) * sina + _width/2.0 * cosa; //_width/2.0;
		driver_elem._v2._z = z + _height;
		driver_elem._v3._x = x - _length/2.0 * cosa - _width/2.0 * sina; //- _length/2.0;
		driver_elem._v3._y = y - _length/2.0 * sina + _width/2.0 * cosa; //_width/2.0;
		driver_elem._v3._z = z + _height;
		driver_elem._v4._x = x - _length/2.0 * cosa - _width/2.0 * sina; //- _length/2.0;
		driver_elem._v4._y = y - _length/2.0 * sina + _width/2.0 * cosa; //_width/2.0;
		driver_elem._v4._z = z;

		Scale_Coordinates<MT>(driver_elem._v1);
		Scale_Coordinates<MT>(driver_elem._v2);
		Scale_Coordinates<MT>(driver_elem._v3);
		Scale_Coordinates<MT>(driver_elem._v4);

		_vehicles_layer->Push_Element<Regular_Element>(&driver_elem);

		/// DRAW BACK SIDE ///
		Quad_Element_Colored back_elem;
		back_elem._color._r = _color._r;
		back_elem._color._g = _color._g;
		back_elem._color._b = _color._b;
		back_elem._color._a = _color._a;

		back_elem._v1._x = x - _length/2.0 * cosa - _width/2.0 * sina; //- _length/2.0;
		back_elem._v1._y = y - _length/2.0 * sina + _width/2.0 * cosa; //_width/2.0;
		back_elem._v1._z = z;
		back_elem._v2._x = x - _length/2.0 * cosa + _width/2.0 * sina; //- _length/2.0;
		back_elem._v2._y = y - _length/2.0 * sina - _width/2.0 * cosa; //- _width/2.0;
		back_elem._v2._z = z;
		back_elem._v3._x = x - _length/2.0 * cosa + _width/2.0 * sina; //- _length/2.0;
		back_elem._v3._y = y - _length/2.0 * sina - _width/2.0 * cosa; //- _width/2.0;
		back_elem._v3._z = z + _height/2.0;
		back_elem._v4._x = x - _length/2.0 * cosa - _width/2.0 * sina; //- _length/2.0;
		back_elem._v4._y = y - _length/2.0 * sina + _width/2.0 * cosa;  //_width/2.0;
		back_elem._v4._z = z + _height/2.0;

		Scale_Coordinates<MT>(back_elem._v1);
		Scale_Coordinates<MT>(back_elem._v2);
		Scale_Coordinates<MT>(back_elem._v3);
		Scale_Coordinates<MT>(back_elem._v4);

		_vehicles_layer->Push_Element<Regular_Element>(&back_elem);

		/// DRAW BACK SIDE WINDOW ///
		Quad_Element_Colored back_window_elem;
		back_window_elem._color._r = 170;
		back_window_elem._color._g = 170;
		back_window_elem._color._b = 255;
		back_window_elem._color._a = 255;

		back_window_elem._v1._x = x - _length/2.0 * cosa - _width/2.0 * sina; //- _length/2.0;
		back_window_elem._v1._y = y - _length/2.0 * sina + _width/2.0 * cosa; //_width/2.0;
		back_window_elem._v1._z = z + _height/2.0;
		back_window_elem._v2._x = x - _length/2.0 * cosa + _width/2.0 * sina; //- _length/2.0;
		back_window_elem._v2._y = y - _length/2.0 * sina - _width/2.0 * cosa; //- _width/2.0;
		back_window_elem._v2._z = z + _height/2.0;
		back_window_elem._v3._x = x - _length/2.0 * cosa + _width/2.0 * sina; //- _length/2.0;
		back_window_elem._v3._y = y - _length/2.0 * sina - _width/2.0 * cosa; //- _width/2.0;
		back_window_elem._v3._z = z + _height;
		back_window_elem._v4._x = x - _length/2.0 * cosa - _width/2.0 * sina; //- _length/2.0;
		back_window_elem._v4._y = y - _length/2.0 * sina + _width/2.0 * cosa; //_width/2.0;
		back_window_elem._v4._z = z + _height;

		Scale_Coordinates<MT>(back_window_elem._v1);
		Scale_Coordinates<MT>(back_window_elem._v2);
		Scale_Coordinates<MT>(back_window_elem._v3);
		Scale_Coordinates<MT>(back_window_elem._v4);

		_vehicles_layer->Push_Element<Regular_Element>(&back_window_elem);
	}
	static Antares_Layer<typename MasterType::antares_layer_type>* _base_agent_layer;
};
implementation struct Derived_Agent_Implementation : public Base_Agent_Implementation<MasterType,INHERIT(Derived_Agent_Implementation)>
{
	typedef Agent<ComponentType> this_itf;
	void Initialize()
	{
		_data = 5;
	}
	void Do_Event(Event_Response& response)
	{
		cout <<"DERIVED_AGENT: event fired at " << iteration()<<", next event to fire at " << response.next._iteration<<endl;
		this_itf* pthis = (this_itf*)this;
		pthis->Reschedule_Agent(iteration()+5, response);
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
	typedef Derived_Agent_Implementation<M> derived_agent_type;
	typedef Helper_Implementation<M> helper_type;
	// Add all of the types used in your code here
};


int main(int argc, char *argv[])
{
	//----------------------------------------------------------
	// Initialize basic simulation 
	Simulation_Configuration cfg;
	cfg.Multi_Threaded_Setup(100, 1);
	INITIALIZE_SIMULATION(cfg);


	// initialize the visualizer environment
	START_UI(MasterType,0,0,1000,1000);


	//----------------------------------------------------------
	// Your code here
	typedef Helper<MasterType::helper_type> helper_itf;

	helper_itf* main_agent = (helper_itf*)Allocate<MasterType::helper_type>();

	main_agent->Initialize<NT>();


	START();
	char test;
	cin >> test;
}

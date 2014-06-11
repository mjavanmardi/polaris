#include "Antares/Antares.h"
#include <vector>
#define PI 3.14159265359

// include polaris namespace, everything written in a polaris prototype/implementation file should be inside the polaris namespace
using namespace polaris;

int link_count;
int node_count;


//---------------------------------------------------
// SCENARIO DEFINITIONS
//---------------------------------------------------
#define RANDOM_SIGNALS
//#define COORDINATED_SIGNALS



// forward declarations: use to declare a prototype which is used in a class before it is defined (i.e. used in node definition but defined later), this is needed when there are circular dependencies
prototype struct Link;


//---------------------------------------------------
// NODE DEFINITION
//---------------------------------------------------
// Prototype - generic interface to a Node element
prototype struct Node : ComponentType
{
	// required tag to identify this class as a polaris prototype
	tag_as_prototype;
	
	// simple accessors to implementation data members - only use these when the data member will be one specific type
	typed_accessor(bool,is_red);
	typed_accessor(int, cycle_length);
	typed_accessor(int, green_time);
	typed_accessor(int, counter);

	// general accessor to implementation data members - this form allows more generality in the underlying implementation accessed by the prototype
	accessor(coordinate,NONE,NONE);
	accessor(outbound_links,NONE,NONE);
	accessor(outbound_link_ids,NONE,NONE);

	// function call, passed through to implementation
	void Initialize(float x, float y, int cycle_length, int green_time, int start_time)
	{
		this_component()->Initialize(x,y,cycle_length,green_time,start_time);
	}
};
// Implementation, represents a concrete representation of an object with typed data members
implementation struct Node_Implementation : public Polaris_Component<MasterType,INHERIT(Node_Implementation),Execution_Object>
{	
	typedef Link<typename MasterType::link_type> link_itf;

	// Implementation member variables, describe the specific properties, characteristics, members, etc. of the implementation using concrete data types
	// m_data keyword is used to create the types, this gives
	// 1.) a variable of the given type with the given name preceded by an underscore, and
	// 2.) a getter and setter function, which can check the requirements specified by GETTER_REQUIREMENTS, SETTER_REQUIREMENTS
	// The requirements on Getter/Setter should be given in the form 'check(<type_name>, <concept>)' - 
	// LEAVE THE REQUIREMENATS AS 'NONE' FOR NOW, UNTIL CONCEPTS ARE USED
	m_data(Point_3D<NT>,coordinate,NONE,NONE);
	m_data(bool,is_red,NONE,NONE);
	m_data(int, cycle_length,NONE,NONE);
	m_data(int, green_time,NONE,NONE);
	m_data(int, counter,NONE,NONE);

	// this is a specialization of m_data that works specifically with container types
	m_container(std::vector<link_itf*>, outbound_links, NONE, NONE);
	m_container(std::vector<int>, outbound_link_ids, NONE, NONE);

	// Add this function to initialize the drawing or any other type level (static) information
	static void Initialize_Type()
	{
		// create new antares layer, with the given name
		_nodes_layer = Allocate_New_Layer<MT>(string("Nodes"));

		// create an antares layer configuration, used to set layer drawing properties, use in the layer initialize function
		Antares_Layer_Configuration cfg;
		True_Color_RGBA<NT> _color;
		_color.r(205); _color.g(205); _color.b(205); _color.a(255);
		cfg.Configure_Dynamic_Points();
		cfg.target_sub_iteration = 1;
		cfg.head_size_value = 20;
		cfg.primitive_color = true;
		cfg.draw=true;

		// intialize layer with the specified configuration
		_nodes_layer->Initialize<NT>(cfg);
	}

	// Use initializer functions to set up object instances, set up object execution, etc.
	void Initialize(float x, float y, int cycle_length, int green_time, int start_time)
	{
		// set member variables, *NOTE member variables should all be preceded with underscores - automatically enforeced by the m_data keyword
		_cycle_length = cycle_length;
		_green_time = green_time;
		_is_red = false;
		_counter = 0;
		_coordinate._x = x;
		_coordinate._y = y;

		Point_Element_Colored signal;
		signal._color.r(0);
		signal._color.g(255);
		signal._color.b(0);
		signal._color.a(255);
		signal._point._x = _coordinate._x;
		signal._point._y = _coordinate._y;
		signal._point._z = 1;

		// draw initial version of signal
		Scale_Coordinates<MT>(signal._point);
		_nodes_layer->Push_Element<Regular_Element>(&signal);


		// Load event function call sets up the object execution.  The event is set to be first called at the specified iteration,subiteration pair
		Load_Event<Node_Implementation>(&Do_Signal_Update,start_time,0);
	}

	// Functions of the format are used to create 'events' which are used by the Execution_Engine of core
	// The event member functions should always be of the form 'static void {Function_Name}({ComponentType}*, Event_Response&)'
	static void Do_Signal_Update(Node_Implementation* _this,Event_Response& response)
	{
		response.next._iteration = iteration() + 1;
		response.next._sub_iteration = 0;

		_this->Signal_Update();
	}
	// Extension of the event function
	void Signal_Update()
	{
		
		_counter++;

		// updae the signal state
		if (_counter > _green_time) 
		{
			_is_red = true;
		}
		if (_counter > _cycle_length )
		{
			_is_red = false;
			_counter = 0;
		}
		
		if(_is_red)
		{
			Draw(True_Color_RGBA<NT>(255,0,0,255));
		}
		else 
		{
			Draw(True_Color_RGBA<NT>(0,255,0,255));
		}
	}

	// Connection to antares used to draw layer
	void Draw(True_Color_RGBA<NT> _color)
	{
		/// DRAW BOTTOM ///
		float x = _coordinate._x;
		float y = _coordinate._y;
		Point_Element_Colored signal;
		signal._color._r = _color._r;
		signal._color._g = _color._g;
		signal._color._b = _color._b;
		signal._color._a = _color._a;

		signal._point._x = x;
		signal._point._y = y;
		signal._point._z = 20;

		// This is used to ensure the points draw at the proper screen location, call this for every point
		Scale_Coordinates<MT>(signal._point);

		// This sets the element to be drawn to the layer associated with the implementation
		_nodes_layer->Push_Element<Regular_Element>(&signal);
	}

	// Antares layers are specified as static member variables as they apply to the type as a whole - note that this alone does not define the layer variable, see next line
	static Antares_Layer<typename MasterType::antares_layer_type>* _nodes_layer;
};
// As antares layers are static they also need to be defined outside of the class body, NOTE* this is true for any static member variable
template<typename MasterType,typename InheritanceList> Antares_Layer<typename MasterType::antares_layer_type>* Node_Implementation<MasterType,InheritanceList>::_nodes_layer;



//---------------------------------------------------
// LINK DEFINITION
//---------------------------------------------------
prototype struct Link : ComponentType
{
	tag_as_prototype;
	accessor(anode,NONE,NONE);
	accessor(bnode,NONE,NONE);
	typed_accessor(int, anode_id);
	typed_accessor(int, bnode_id);
	typed_accessor(float, width);
	typed_accessor(float, speed_limit);
	typed_accessor(float, length);
	typed_accessor(float, heading);
	// this is an example of a templated member function, this allows more flexibility in the prototype, i.e. Node_Type is not restricted to any specific type
	// restrictions can be added later through the use of concepts
	template<typename Node_Type> void Initialize(Node_Type* anode, Node_Type* bnode, float width)
	{
		this_component()->Initialize<Node_Type>(anode,bnode,width);
	}
	template<typename ReturnType> void Travel_Time()
	{
		return length() / speed_limit();
	}
};
implementation struct Link_Implementation : public Polaris_Component<MasterType,INHERIT(Link_Implementation),Data_Object>
{
	typedef Node<typename MasterType::node_type> node_itf;

	static void Initialize_Type()
	{
		_links_layer = Allocate_New_Layer<MT>(string("Links"));
		
		Antares_Layer_Configuration cfg;
		True_Color_RGBA<NT> _color;
		_color.r(1);
		_color.g(1);
		_color.b(1);
		_color.a(255);
		cfg.Configure_Static_Quads(_color,10);
		cfg.primitive_color = true;
		cfg.grouped = false;
		cfg.target_sub_iteration = 1;
		cfg.draw=true;

		_links_layer->Initialize<NT>(cfg);
	}

	template<typename Node_Type> void Initialize(Node_Type* anode, Node_Type* bnode, float width)
	{
		// set the a/b nodes
		_anode = (node_itf*)anode;
		_bnode = (node_itf*)bnode;
		_width = width;
		_anode_id = _anode->uuid();
		_bnode_id = _bnode->uuid();

		// the link is an outbound link from anode, since that is the start, so add *this to the outbound links container of the anode
		typedef Prototype_Back_Insertion_Sequence<typename node_itf::get_type_of(outbound_links), Link> links_container_itf;
		links_container_itf* outbound_links = _anode->outbound_links<links_container_itf*>();
		outbound_links->push_back((Link<ComponentType>*)this);
		typedef Back_Insertion_Sequence<typename node_itf::get_type_of(outbound_link_ids), int> link_ids_container_itf;
		link_ids_container_itf* outbound_link_ids = _anode->outbound_link_ids<link_ids_container_itf*>();
		int id = ((Link<ComponentType>*)this)->uuid();
		outbound_link_ids->push_back(id);

		Point_3D<NT> a_point = _anode->coordinate<Point_3D<NT>>();
		Point_3D<NT> b_point = _bnode->coordinate<Point_3D<NT>>();

		float d_x = (b_point._x - a_point._x);
		float d_y = (b_point._y - a_point._y);
		
		// calculate and set length
		_length = sqrt( d_x*d_x + d_y*d_y);

		// assign random speed limit
		_speed_limit = 40.0 + (float)(rand() % 20);


		// drawing stuff
		_heading = abs(atan(d_y/d_x));
		if (d_x == 0 && d_y > 0) _heading = (float)PI/2.0;
		if (d_x == 0 && d_y <= 0) _heading = 3.0f*(float)PI/2.0f;
		if (d_x < 0 && d_y >= 0) _heading = (float)PI - _heading;
		if (d_x < 0 && d_y < 0) _heading = (float)PI + _heading;
		if (d_x > 0 && d_y < 0) _heading = 2.0f*(float)PI - _heading;

		//float angle = PI / 2.0 - atan(d_y/d_x);
		float angle = PI/2.0 - _heading;
		if (d_x == 0 && d_y > 0) angle = 0.0;
		else if (d_x == 0 && d_y < 0) angle = (float)PI;
		float d_x_disp = (_width/* / 2.0*/) * (float)cos(angle);
		float d_y_disp = (_width/* / 2.0*/) * (float)sin(angle);

		Quad_Element_Colored elem;
		int shade = (_speed_limit - 40)*10;
		elem._color.r(shade); elem._color.g(shade); elem._color.b(shade); elem._color.a(255);
		elem._v1._x = anode->coordinate<Point_3D<NT>>()._x + d_x_disp;
		elem._v1._y = anode->coordinate<Point_3D<NT>>()._y - d_y_disp;
		elem._v1._z = 0;
		elem._v2._x = bnode->coordinate<Point_3D<NT>>()._x + d_x_disp;
		elem._v2._y = bnode->coordinate<Point_3D<NT>>()._y - d_y_disp;
		elem._v2._z = 0;
		elem._v3._x = bnode->coordinate<Point_3D<NT>>()._x /*- d_x_disp*/;
		elem._v3._y = bnode->coordinate<Point_3D<NT>>()._y /*+ d_y_disp*/;
		elem._v3._z = 0;
		elem._v4._x = anode->coordinate<Point_3D<NT>>()._x /*- d_x_disp*/;
		elem._v4._y = anode->coordinate<Point_3D<NT>>()._y /*+ d_y_disp*/;
		elem._v4._z = 0;

		Scale_Coordinates<MT>(elem._v1);
		Scale_Coordinates<MT>(elem._v2);
		Scale_Coordinates<MT>(elem._v3);
		Scale_Coordinates<MT>(elem._v4);

		_links_layer->Push_Element<Regular_Element>(&elem);
	}

	// m_prototype is a special version of m_data, where the typed data member is another polaris_component, and is reference through an associated prototype
	// all connections to other components should be handled in this way.
	// This keyword creates a local 'prototype<member_type>* pointer variable which can be referenced using the given name preceded by an underscore
	m_prototype(Node,typename MasterType::node_type,anode,NONE,NONE); // i.e. _anode, which is of type Node<typename MasterType::node_type>*
	// Note here the use of MasterType::node_type, which at this point is not defined, meaning that node_type is not a concrete type at this point, which is a KEY point of the Polaris framework
	m_prototype(Node,typename MasterType::node_type,bnode,NONE,NONE);
	m_data(int,anode_id,NONE,NONE);
	m_data(int,bnode_id,NONE,NONE);
	m_data(float,width,NONE,NONE);
	m_data(float,length,NONE,NONE);
	m_data(float,speed_limit,NONE,NONE);
	m_data(float,heading, NONE,NONE);

	static Antares_Layer<typename MasterType::antares_layer_type>* _links_layer;
};
template<typename MasterType,typename InheritanceList> Antares_Layer<typename MasterType::antares_layer_type>* Link_Implementation<MasterType,InheritanceList>::_links_layer;



//---------------------------------------------------
// NETWORK DEFINITION
//---------------------------------------------------
prototype struct Network
{
	tag_as_prototype;

	typedef Node<typename ComponentType::Master_Type::node_type> node_itf;
	typedef Link<typename ComponentType::Master_Type::link_type> link_itf;

	accessor(links,NONE,NONE);

	// This is a passthrough to the implementation version of the function, i.e. the implementation function does the actual work	
	pair<node_itf*,float> find_closest_node(Point_3D<NT>& point)
	{
		return this_component()->find_closest_node(point);
	}

	// helper function to calculate distance
	float calculate_distance(Point_3D<NT>& point_a, Point_3D<NT>& point_b)
	{
		float d_x = point_a._x - point_b._x;
		float d_y = point_a._y - point_b._y;
		return sqrt(d_x*d_x + d_y*d_y);
	}

	// helper function to calculate heading in radians
	float calculate_heading(Point_3D<NT>& point_a, Point_3D<NT>& point_b)
	{
		float _heading;

		float d_x = point_b._x - point_a._x;
		float d_y = point_b._y - point_a._y;

		_heading = abs(atan(d_y/d_x));
		if (d_x == 0 && d_y > 0) _heading = (float)PI/2.0f;
		if (d_x == 0 && d_y <= 0) _heading = 3.0f*(float)PI/2.0f;
		if (d_x < 0 && d_y >= 0) _heading = (float)PI - _heading;
		if (d_x < 0 && d_y < 0) _heading = (float)PI + _heading;
		if (d_x > 0 && d_y < 0) _heading = 2.0f*(float)PI - _heading;

		return _heading;
	}

	template <typename LinkType> LinkType Get_Link(int id)
	{
		return this_component()->Get_Link<LinkType>(id);
	}
};
implementation struct Network_Implementation : public Polaris_Component<MasterType,INHERIT(Network_Implementation),Data_Object>
{
	// Typedefs can be useful to create shorter names for the prototype interfaces to components
	typedef Node<typename MasterType::node_type> node_itf;
	typedef Link<typename MasterType::link_type> link_itf;

	// this is a specialization of m_data that works specifically with container types
	m_container(std::vector<link_itf*>, links, NONE, NONE);

	template <typename LinkType> LinkType Get_Link(int id)
	{
		typedef typename std::vector<link_itf*>::iterator link_iterator;

		for (link_iterator link_itr = this->_links.begin(); link_itr != this->_links.end(); ++link_itr)
		{
			link_itf* link = *link_itr;
			if (link->uuid() == id) return link;
		}
		return NULL;
	}

	// implementation version called by prototype
	pair<node_itf*,float> find_closest_node(Point_3D<NT>& point)
	{
		std::vector<link_itf*>::iterator link_itr;

		float x_dist_min = FLT_MAX;
		node_itf* closest_node;

		for (link_itr=_links.begin(); link_itr != _links.end(); ++link_itr)
		{
			link_itf* link_ptr = (link_itf*)(*link_itr);
			node_itf* node_ptr = link_ptr->bnode<node_itf*>();

			float dist = node_ptr->coordinate<Point_3D<NT>>()._x - point._x;

			if (dist >= 0 && dist < x_dist_min)
			{
				x_dist_min = dist;
				closest_node = node_ptr;
			}

		}
		
		return pair<node_itf*,float>(closest_node,x_dist_min);
	}
};



//---------------------------------------------------
// ROUTER DEFINITION
//---------------------------------------------------
prototype struct Router
{
	tag_as_prototype;
	accessor(network_reference,NONE,NONE);
	template <typename LinkType, typename PathRefType> void Compute_Route(LinkType origin_link, LinkType destination_link, PathRefType path)
	{
		this_component()->Compute_Route<LinkType, PathRefType>(origin_link, destination_link, path);
	}
};
implementation struct Random_Walk_Router_Implementation : public Polaris_Component<MasterType,INHERIT(Random_Walk_Router_Implementation),Data_Object>
{
	m_prototype(Network, typename MasterType::network_type, network_reference, NONE, NONE);

	typedef Node<typename MasterType::node_type> node_itf;
	typedef Link<typename MasterType::link_type> link_itf;
	typedef Prototype_Back_Insertion_Sequence<typename get_type_of(network_reference)::get_type_of(links), Link> links_container_itf;
	typedef typename links_container_itf::iterator link_iterator;


	template <typename LinkType, typename PathRefType> void Compute_Route(LinkType origin_link, LinkType destination_link, PathRefType path)
	{
		links_container_itf* links = _network_reference->links<links_container_itf*>();

		path.push_back(origin_link);
		

		link_itf* current_link = (link_itf*)origin_link;
		link_itf* next_link;
		while (true)
		{
			node_itf* end_node = current_link->bnode<node_itf*>();
			links_container_itf* outbound_links = end_node->outbound_links<links_container_itf*>();

			// check if destination link is in outbound links, if so end trip
			for (link_iterator link_itr = outbound_links->begin(); link_itr != outbound_links->end(); link_itr++)
			{
				if (*link_itr == destination_link) 
				{
					path.push_back(destination_link);
					return;
				}
			}

			// otherwise go to random outbound link
			int link_index = rand() % outbound_links->size();
			int i=0;
			for (link_iterator link_itr = outbound_links->begin(); link_itr != outbound_links->end(); link_itr++, i++)
			{
				if (i == link_index) 
				{
					next_link = *link_itr;
					break;
				}
			}
			path.push_back(next_link);
			current_link = next_link;
		}
		
	}
};
implementation struct Dead_Reckoning_Router_Implementation : public Polaris_Component<MasterType,INHERIT(Dead_Reckoning_Router_Implementation),Data_Object>
{
	m_prototype(Network, typename MasterType::network_type, network_reference, NONE, NONE);

	typedef Node<typename MasterType::node_type> node_itf;
	typedef Link<typename MasterType::link_type> link_itf;
	typedef Prototype_Back_Insertion_Sequence<typename get_type_of(network_reference)::get_type_of(links), Link> links_container_itf;
	typedef typename links_container_itf::iterator link_iterator;


	template <typename LinkType, typename PathRefType> void Compute_Route(LinkType origin_link, LinkType destination_link, PathRefType path)
	{
		links_container_itf* links = _network_reference->links<links_container_itf*>();

		path.push_back(origin_link);
		

		link_itf* current_link = (link_itf*)origin_link;
		link_itf* next_link;
		while (true)
		{
			node_itf* end_node = current_link->bnode<node_itf*>();
			links_container_itf* outbound_links = end_node->outbound_links<links_container_itf*>();


			//calculate the target heading from the b-node of current link to the destination node
			float heading = _network_reference->calculate_heading(end_node->coordinate<Point_3D<NT>>(), destination_link->anode<node_itf*>()->coordinate<Point_3D<NT>>());


			// check if destination link is in outbound links, if so end trip
			for (link_iterator link_itr = outbound_links->begin(); link_itr != outbound_links->end(); link_itr++)
			{
				if (*link_itr == destination_link) 
				{
					path.push_back(destination_link);
					return;
				}
			}

		

			// otherwise go to outbound link with smallest difference from target heading
			float min_d_heading = (float)3.0*(float)PI;
			link_itf* temp_link;
			for (link_iterator link_itr = outbound_links->begin(); link_itr != outbound_links->end(); link_itr++)
			{
				link_itf* temp_link = (*link_itr);
				float d_heading = abs(temp_link->heading() - heading);
				if (d_heading < min_d_heading)
				{
					min_d_heading = d_heading;
					next_link = temp_link;
				}
			}
			path.push_back(next_link);
			current_link = next_link;
		}
		
	}
};


//---------------------------------------------------
// AGENT DEFINITION
//---------------------------------------------------
prototype struct Agent
{
	tag_as_prototype;
	typed_accessor(float, width);
	typed_accessor(float, height);
	typed_accessor(float, length);
	typed_accessor(float, speed);
	typed_accessor(Point_3D<NT>, centroid);
	accessor(network_reference, NONE, NONE);
	accessor(current_link, NONE, NONE);
	accessor(router,NONE,NONE);
	
	template<typename NetworkType, typename LinkType> void Initialize(NetworkType network_ref, LinkType starting_link, LinkType destination_link)
	{
		this_component()->Initialize<NetworkType,LinkType>(network_ref, starting_link, destination_link);
	}

	template<typename LinkType> void Set_Path(LinkType origin_link, LinkType destination_link)
	{
		this_component()->Set_Path<LinkType>(origin_link, destination_link);
	}
};
implementation struct Agent_Implementation : public Polaris_Component<MasterType,INHERIT(Agent_Implementation),Execution_Object>
{
	// General typedefs
	typedef Node<typename MasterType::node_type> node_itf;
	typedef Link<typename MasterType::link_type> link_itf;

	// class data members
	m_data(float, width, NONE,NONE);
	m_data(float, height, NONE,NONE);
	m_data(float, length, NONE,NONE);
	m_data(float, speed, NONE,NONE);
	m_data(Point_3D<NT>, centroid, NONE,NONE);
	m_data(int, path_location_index, NONE,NONE);
	m_prototype(Network, typename MasterType::network_type, network_reference, NONE, NONE);
	m_prototype(Link, typename MasterType::link_type, current_link, NONE, NONE);
	m_prototype(Router, typename MasterType::router_type, router, NONE, NONE);
	m_container(std::vector<link_itf*>, path, NONE, NONE);

	// typedefs dependent on data members - make sure to place this after the m_data statement which creates the referenced member
	typedef Prototype_Back_Insertion_Sequence<typename get_type_of(network_reference)::get_type_of(links), Link> links_container_itf;
	typedef typename links_container_itf::iterator link_iterator;


	// Functions
	static void Initialize_Type()
	{
		_vehicles_layer = Allocate_New_Layer<MT>(string("Vehicles"));

		Antares_Layer_Configuration cfg;
		True_Color_RGBA<NT> _color;
		_color.r(1); _color.g(1); _color.b(255); _color.a(255);
		cfg.Configure_Dynamic_Quads(_color,4);
		cfg.target_sub_iteration = 1;
		cfg.primitive_color = true;
		cfg.draw=true;

		_vehicles_layer->Initialize<NT>(cfg);
	}

	// Agent initializer - creates and draws agent at starting position, loads the starting event
	template<typename NetworkType, typename LinkType> void Initialize(NetworkType network_ref, LinkType starting_link, LinkType destination_link)
	{
		typedef Node<typename MasterType::node_type> node_interface;

		_width = 50.0f;
		_height = 20.0f;
		_length = 100.0f;

		_speed = 43.0f;

		Load_Event<Agent_Implementation>(&Do_Move,0,0);
		_current_link = starting_link;
		_network_reference = network_ref;
		Set_Path(starting_link, destination_link);
		_path_location_index = 0;

		node_interface* a_node = _current_link->anode<node_interface*>();

		_centroid._x = a_node->coordinate<Point_3D<NT>>()._x;
		_centroid._y = a_node->coordinate<Point_3D<NT>>()._y;
	}

	// Agent router, calls the internal router feature to create a new route
	template<typename LinkType> void Set_Path(LinkType origin_link, LinkType destination_link)
	{
		_path.clear();
		_path_location_index = 0;
		_router->Compute_Route<link_itf*,std::vector<link_itf*>&>(origin_link, destination_link, _path);
		
		cout << "Selected Path: ";

		link_iterator link_itr = _path.begin(); 
		for (;link_itr != _path.end(); ++link_itr)
		{
			cout << (*link_itr)->uuid() << ", ";
		}
		cout << endl;
	}

	// Movement event, and associated event function
	// Check the conditions for movement in the next movement timestep (i.e. is there a node within the distance that will be moved in the next iteration based on current travel speed?
	// If no node, move distance = speed/_iteration_length
	// If node, check if a) trip is ended b) light is green b.1) go straight or turn c) stop at red in which case load the Stop event
	static void Do_Move(Agent_Implementation* _this,Event_Response& response)
	{
		response.next._iteration = iteration() + 1;
		response.next._sub_iteration = 0;

		_this->Move();
	}
	void Move()
	{
		node_itf* next_node = _current_link->bnode<node_itf*>();
		
		float dist = _network_reference->calculate_distance(next_node->coordinate<Point_3D<NT>>(), _centroid);

		// far away from intersection, move at current speed
		if (dist - _length/2.0 > _speed)
		{
			_centroid._x += _current_link->speed_limit()*cos(_current_link->heading());
			_centroid._y += _current_link->speed_limit()*sin(_current_link->heading());
		}
		// close to intersection, check for stops otherwise turn to next link
		else
		{
			// the light is green, finish movement
			if (!next_node->is_red())
			{
				// choose outbound link to move to
				links_container_itf* outbound_links = next_node->outbound_links<links_container_itf*>();
				link_itf* next_link;

				if (outbound_links->size() == 0)
				{
					cout << "Car cannot move further as there are no outbound links from current node."<<endl;
					return;
				}


				int i = 0;
				_path_location_index++;
				if (_path_location_index >= _path.size())
				{
					cout << "Trip over.  Starting new trip" << endl;
					Sleep(1000);

					// get a new link ID to route to, make sure it is not same as current link ID, or out of bounds
					int size = _network_reference->links<links_container_itf*>()->size();
					int new_dest = rand()%size;
					if (new_dest == _current_link->uuid()) new_dest++;
					if (new_dest >= size) new_dest = 0;

					Set_Path(_current_link,_network_reference->Get_Link<link_itf*>(new_dest) );
					return;
				}
				for (link_iterator link_itr = _path.begin(); link_itr != _path.end(); link_itr++, i++)
				{
					if (i == this->_path_location_index) 
					{
						next_link = *link_itr;
						break;
					}
				}
				
				
				// approach intersection
				_centroid._x += dist*cos(_current_link->heading());
				_centroid._y += dist*sin(_current_link->heading());

				// move to next link
				_current_link = next_link;
				_centroid._x = _current_link->anode<node_itf*>()->coordinate<Point_3D<NT>>()._x;
				_centroid._y = _current_link->anode<node_itf*>()->coordinate<Point_3D<NT>>()._y;
			}

			// the light is red, do stop
			else
			{
				_centroid._x += (dist - _length/2.0)*cos(_current_link->heading());
				_centroid._y += (dist - _length/2.0)*sin(_current_link->heading());

				Swap_Event((Event)&Do_Stop);
			}
		}
		Draw(0,True_Color_RGBA<NT>(50,155,175,255));

		
		//if(_centroid._x < 1750.0f && _centroid._x > -1350.0f)
		//{
		//	Draw(0,True_Color_RGBA<NT>(50,155,175,255));
		//}
		//else if(_centroid._x >= 1750.0f)
		//{
		//	_centroid._x = -1350.0f;
		//}

		Sleep(50);
	}

	// Stop event and associated stop function, this draws the stopped vehicle
	// Continues to draw the stopped vehicle until the signal at next_node changes to green
	static void Do_Stop(Agent_Implementation* _this,Event_Response& response)
	{
		response.next._iteration = iteration() + 1;
		response.next._sub_iteration = 0;

		_this->Stop();
	}
	void Stop()
	{
		pair<node_itf*,float> closest_node = _network_reference->find_closest_node(_centroid);
		float dist = closest_node.second;
		node_itf* next_node = closest_node.first;
		if (!next_node->is_red())
		{
			Swap_Event((Event)&Do_Move);
		}

		Draw(0, True_Color_RGBA<NT>(50,155,175,255));
		//if(_centroid._x < 1750.0f && _centroid._x > -1350.0f)
		//{
		//	Draw(0, True_Color_RGBA<NT>(50,155,175,255));
		//}
		//else if(_centroid._x >= 1750.0f)
		//{
		//	_centroid._x = -1350.0f;
		//}


		Sleep(50);
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
	static Antares_Layer<typename MasterType::antares_layer_type>* _vehicles_layer;
};
template<typename MasterType,typename InheritanceList> Antares_Layer<typename MasterType::antares_layer_type>* Agent_Implementation<MasterType,InheritanceList>::_vehicles_layer;



//---------------------------------------------------
// TYPES COLLECTION - this structure is used to collect all of the types available to the current simulation
// When using different types for different simulations, change the types here but keep the same typedef names
// i.e. if you want to use a different link implementation, change to the new 'Link_Implementation<MasterType> to your new class, but leave 'link_type' defined.
// this is the strength of polaris, as all code that depends on 'link_type' will still work, as long as your new implementation matches the API defined in the 'link' prototype
//---------------------------------------------------
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
	typedef Link_Implementation<MasterType> link_type;
	typedef Node_Implementation<MasterType> node_type;
	typedef Network_Implementation<MasterType> network_type;

	// Change the router here to whatever router you want to implement, 
	// as long as router_type matches the simple router prototype the agent will still function properly
	// for now you can use 'Random_Walk_Router_Implementation' or 'Dead_Reckoning_Router_Implementation'
	typedef Dead_Reckoning_Router_Implementation<MasterType> router_type;
};



//---------------------------------------------------
// MAIN SIMULATION CODE - simulation entry point - make sure only one is defined
//---------------------------------------------------
int main()
{
	srand (time(NULL));
	link_count = 0;
	node_count = 0;

	//---------------------------------------------------
	// Define easy to use names for the prototype interfaces to the MasterType objects - use these in later calls instead of the full name for readability
	// usually helps to do this for all prototype-componenttype combinations used
	// note: may need to make multiple versions if different componenttypes share the same prototype
	typedef Network<MasterType::network_type> network_itf;
	typedef Agent<MasterType::agent_type> agent_itf;
	typedef Node<MasterType::node_type> node_itf;
	typedef Link<MasterType::link_type> link_itf;
	typedef Router<MasterType::router_type> router_itf;
	// This creates an interface to a container, using generalized container prototypes
	// Here, the prototype works for any 'back_insertion' container, i.e. containers with 'push_back' defined - examples: vector, deque, list
	// To create interface to a back insertion sequence, specify the container type, and give the prototype in contains as template arguments, here 'Link' is the prototype held in the vector
	// 'get_type_of()' macro is used to extract types of member variables
	typedef Prototype_Back_Insertion_Sequence<typename network_itf::get_type_of(links), Link> links_container_itf;



	//---------------------------------------------------
	// Simulation configuration is used to set the run parameters for the program
	Simulation_Configuration cfg;
	// use this to set up a multi-threaded run, specifying the number of iterations and threads to use
	cfg.Multi_Threaded_Setup(100000,6);
	// initialize the run environment
	INITIALIZE_SIMULATION(cfg);
	// initialize the visualizer environment
	START_UI(MasterType,0,0,1000,1000);
	

	//---------------------------------------------------
	// Initialize drawing layers - always need to call static initializer functions this way
	MasterType::agent_type::Initialize_Type();
	MasterType::link_type::Initialize_Type();
	MasterType::node_type::Initialize_Type();


	//---------------------------------------------------
	// Allocate the network and push the links to the network links container
	// *note here that allocation returns a pointer to a MasterType::network_type object, which is then cast to an interface to that object for ease of use
	network_itf* network = (network_itf*)Allocate<MasterType::network_type>();
	// call an accessor from the network interface to get the links container
	links_container_itf* links = network->links<links_container_itf*>();


	//=================================================================================================
	// Create grid network automatically, based on specified number of rows/columns 
	// - adjust net_width , net_height, start_x and start_y if the grid does not fit on your screen
	// - the following code allocates a rowsXcols grid of nodes, then allocates and initializes links between those nodes
	//-------------------------------------------------------------------------------------------------
	int rows = 5;
	int cols = 6;
	float start_x = -1500.0;
	float start_y = -400;
	float net_width = 4800.0;
	float net_height = 2200.0;
	float link_width = 20.0;

	// creates node at each grid intersection point
	std::vector<std::vector<node_itf*>> nodes;
	for (int i = 0; i < rows; i++)
	{
		nodes.push_back(std::vector<node_itf*>());
		for (int j=0; j < cols; j++)
		{
			node_itf* n = (node_itf*)Allocate<MasterType::node_type>(node_count); ++node_count;
			float x = start_x + net_width/cols * j;
			float y = start_y + net_height/rows * i;
		// Modify the signal starting time, based on signal coordination type
		#ifdef RANDOM_SIGNALS
			int start = rand()%60;
			n->Initialize(x,y,60,40,start);
		#else
			n->Initialize(x,y,60,40,0);
		#endif
			nodes[i].push_back(n);
		}
	}

	// create horizontal links between nodes
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols-1; j++)
		{
			link_itf* link = (link_itf*)Allocate<MasterType::link_type>(link_count); ++link_count;
			links->push_back(link);
			link->Initialize<node_itf>(nodes[i][j], nodes[i][j+1],link_width);
			link_itf* rev_link = (link_itf*)Allocate<MasterType::link_type>(link_count); ++link_count;
			rev_link->Initialize<node_itf>(nodes[i][j+1], nodes[i][j],link_width);
			links->push_back(rev_link);
		}
	}

	// create vertical links between nodes
	for (int i = 0; i < rows-1; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			link_itf* link = (link_itf*)Allocate<MasterType::link_type>(link_count); ++link_count;
			links->push_back(link);
			link->Initialize<node_itf>(nodes[i][j], nodes[i+1][j],link_width);
			link_itf* rev_link = (link_itf*)Allocate<MasterType::link_type>(link_count); ++link_count;
			rev_link->Initialize<node_itf>(nodes[i+1][j], nodes[i][j],link_width);
			links->push_back(rev_link);
		}
	}

	// create diagonal through network
	for (int i=0; i < min(rows-1,cols-1); i++)
	{
		link_itf* link = (link_itf*)Allocate<MasterType::link_type>(link_count); ++link_count;
		links->push_back(link);
		link->Initialize<node_itf>(nodes[i][i], nodes[i+1][i+1],link_width);
		link_itf* rev_link = (link_itf*)Allocate<MasterType::link_type>(link_count); ++link_count;
		rev_link->Initialize<node_itf>(nodes[i+1][i+1], nodes[i][i],link_width);
		links->push_back(rev_link);
	}
	//-------------------------------------------------------------------------------------------------


	//=================================================================================================
	// allocate the traveler agent, and assign it a router
	//-------------------------------------------------------------------------------------------------
	agent_itf* agent = (agent_itf*)Allocate<MasterType::agent_type>();
	router_itf* router = (router_itf*)Allocate<MasterType::router_type>();
	router->network_reference<network_itf*>(network);
	agent->router<router_itf*>(router);

	// initialize the agent with a reference to the network, and a trip orig/dest
	agent->Initialize<network_itf*,link_itf*>(network, network->Get_Link<link_itf*>(0),network->Get_Link<link_itf*>(48));

	// Start the simulation
	START();
}
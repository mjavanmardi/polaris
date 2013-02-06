//*********************************************************
//	Canvas_Implementation.h - Initialization Functions
//*********************************************************

#pragma once
#include "Canvas.h"

//---------------------------------------------------------
//	Canvas_Implementation - canvas class definition
//---------------------------------------------------------

implementation class Canvas_Implementation : public Polaris_Component_Class<Canvas_Implementation,MasterType,NULLTYPE>, public wxGLCanvas
{
public:
	Canvas_Implementation(wxFrame* parent, int* args);
	virtual ~Canvas_Implementation(void) {delete _glcontext;}

	feature_implementation void Initialize();
	void Initialize_GLCanvas();
	void Calculate_Bounds();
	void Draw_Network();
	void Draw_Vehicles(int current_iteration);

	void Render(wxPaintEvent& event);
	void OnResize(wxSizeEvent& event);
	void OnLeftDown(wxMouseEvent& event);
    void OnLeftUp(wxMouseEvent& event);
    void OnRightDown(wxMouseEvent& event);
    void OnRightUp(wxMouseEvent& event);
    void OnMotion(wxMouseEvent& event);
    void OnRotationMotion(wxMouseEvent& event);
    void OnWheel(wxMouseEvent& event);
	void OnLeave(wxMouseEvent& event);
	void Zoom(wxIdleEvent& event);

	member_pointer(wxGLContext,glcontext,none,none);

	member_data(Rectangle_XY<MasterType>,canvas_bounds,none,none);

	member_data(int,cached_iteration,none,none);

	member_data(float,near_plane,none,none);
	member_data(float,far_plane,none,none);
	
	member_data(float,win_z_mid,none,none);

	member_data(float,panel_width,none,none);
	member_data(float,panel_height,none,none);
	member_data(float,scale,none,none);

	member_data(float,x_rotation,none,none);
	member_data(float,z_rotation,none,none);
	member_data(float,x_translation,none,none);
	member_data(float,y_translation,none,none);

	member_data(float,meters_per_pixel_max,none,none);
	member_data(float,meters_per_pixel_mid,none,none);
	member_data(float,meters_per_pixel_min,none,none);

	member_data(Rectangle_XY<MasterType>,view_bounds,none,none);

	member_data(bool,spatial_change,none,none);
	member_data(bool,temporal_change,none,none);

	member_data(bool,left_down,none,none);
	member_data(bool,right_down,none,none);

	member_data(float,x_start_utm,none,none);
	member_data(float,y_start_utm,none,none);

	member_data(int,x_start_win,none,none);
	member_data(int,y_start_win,none,none);
	
	member_data(int,wheel_dir,none,none);

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];

	member_prototype(Network_Prototype,graphical_network,typename MasterType::graphical_network_type,Canvas_Implementation,none,none);
};

//---------------------------------------------------------
//	Canvas_Implementation - canvas initialization
//---------------------------------------------------------

template<typename MasterType,typename ParentType>
Canvas_Implementation<MasterType,ParentType>::Canvas_Implementation(wxFrame* parent, int* args) : wxGLCanvas(parent,-1,args,wxDefaultPosition,wxDefaultSize,wxFULL_REPAINT_ON_RESIZE)
{
	//antares_ptr=((Antares_Implementation<MasterType>*)GetParent());

	_glcontext = new wxGLContext(this);

	//---- initialize image handling ----

	wxInitAllImageHandlers();

	Connect(wxEVT_SIZE,wxSizeEventHandler(Canvas_Implementation::OnResize));
	Connect(wxEVT_LEFT_DOWN,wxMouseEventHandler(Canvas_Implementation::OnLeftDown));
	Connect(wxEVT_LEFT_UP,wxMouseEventHandler(Canvas_Implementation::OnLeftUp));
	Connect(wxEVT_RIGHT_DOWN,wxMouseEventHandler(Canvas_Implementation::OnRightDown));
	Connect(wxEVT_RIGHT_UP,wxMouseEventHandler(Canvas_Implementation::OnRightUp));
	Connect(wxEVT_MOUSEWHEEL,wxMouseEventHandler(Canvas_Implementation::OnWheel));
	Connect(wxEVT_LEAVE_WINDOW,wxMouseEventHandler(Canvas_Implementation::OnLeave));

	//---- navigation ----
	
	_left_down=_right_down=false;
	_x_translation=_y_translation=0;
	_x_rotation=_z_rotation=0;
	_x_start_utm=_y_start_utm=0;
	_x_start_win=_y_start_win=0;
	_scale=5;

	//---- visibility ----

	_near_plane=_far_plane=0;
	_win_z_mid=-1;

	//---- drawing ----

	_spatial_change=true;
	_temporal_change=false;
	_cached_iteration=-1;
}

//---------------------------------------------------------
//	Initialize - general initialization of canvas
//---------------------------------------------------------

template<typename MasterType,typename ParentType>
template<typename ComponentType,typename CallerType,typename TargetType>
void Canvas_Implementation<MasterType,ParentType>::Initialize()
{
	Initialize_GLCanvas();

	_graphical_network = (Network_Prototype<type_of(graphical_network),Canvas_Implementation>*) Allocate<type_of(graphical_network)>();
	
	// make the vital connection between graphical network and graphical vehicle
	Graphical_Vehicle_Implementation<MasterType>::_graphical_network=(Network_Prototype<type_of(graphical_network),Graphical_Vehicle_Implementation<MasterType>>*)_graphical_network;

	_graphical_network->read_network_data<Network_Components::Types::Network_Initialization_Type<Network_Components::Types::Graphical_Network,string&>>( ((Antares_Implementation<MasterType>*)GetParent())->_db_name );

	// set canvas bounds as network bounds
	Rectangle_Prototype<typename type_of(graphical_network)::type_of(network_bounds)>* net_bounds=_graphical_network->network_bounds<Rectangle_Prototype<typename type_of(graphical_network)::type_of(network_bounds)>*>();

	_canvas_bounds._xmin = net_bounds->xmin<float>();
	_canvas_bounds._xmax = net_bounds->xmax<float>();

	_canvas_bounds._ymin = net_bounds->ymin<float>();
	_canvas_bounds._ymax = net_bounds->ymax<float>();

	// compute near and far planes with these bounds
	if(_canvas_bounds._ymax-_canvas_bounds._ymin>_canvas_bounds._xmax-_canvas_bounds._xmin)
	{
		_near_plane=(_canvas_bounds._ymax-_canvas_bounds._ymin)/(tan(25.0*.0174532925)*4.0)*10;
		_far_plane=(_canvas_bounds._ymax-_canvas_bounds._ymin)/(tan(25.0*.0174532925)*4.0)*10+2.0*(_canvas_bounds._ymax-_canvas_bounds._ymin);
	}
	else
	{
		_near_plane=(_canvas_bounds._xmax-_canvas_bounds._xmin)/(tan(25.0*.0174532925)*4.0)*10;
		_far_plane=(_canvas_bounds._xmax-_canvas_bounds._xmin)/(tan(25.0*.0174532925)*4.0)*10+2.0*(_canvas_bounds._xmax-_canvas_bounds._xmin);
	}

	// connect the paint handler
	Connect(wxEVT_PAINT,wxPaintEventHandler(Canvas_Implementation::Render));
}

//---------------------------------------------------------
//	OnResize - handles when canvas is resized
//---------------------------------------------------------

template<typename MasterType,typename ParentType>
void Canvas_Implementation<MasterType,ParentType>::OnResize(wxSizeEvent& event)
{
	_panel_width=(float)GetSize().x;
	_panel_height=(float)GetSize().y;
	
	_spatial_change=true;
	Refresh();
}

//---------------------------------------------------------
//	Initialize_Canvas - initialization of GL Canvas
//---------------------------------------------------------

template<typename MasterType,typename ParentType>
void Canvas_Implementation<MasterType,ParentType>::Initialize_GLCanvas()
{
	//---- initialize the opengl canvas settings ----

	wxGLCanvas::SetCurrent(*_glcontext);
	
	glewInit();

	glClearColor(1,1,1,1);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);

	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	SetFocus();
}
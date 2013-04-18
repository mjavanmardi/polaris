//*********************************************************
//	Canvas_Implementation.h - Initialization Functions
//*********************************************************

#pragma once
#include "Canvas.h"

//---------------------------------------------------------
//	Canvas_Implementation - canvas class definition
//---------------------------------------------------------

implementation class Canvas_Implementation : public Polaris_Component<APPEND_CHILD(Canvas_Implementation),MasterType,NULLTYPE>, public wxGLCanvas
{
public:
	Canvas_Implementation(wxFrame* parent, int* args);
	virtual ~Canvas_Implementation(void) {delete _glcontext;}

	feature_implementation void Initialize(float xmin,float ymin,float xmax,float ymax);
	void Initialize_GLCanvas();
	void Calculate_Bounds();

	typedef Antares_Layer<typename type_of(MasterType::antares_layer),Canvas_Implementation> Antares_Layer_Interface;
	
	feature_implementation void Scale_Coordinates(Point_3D<MasterType>& coordinates)
	{
		coordinates._x += _input_offset._x;
		coordinates._y += _input_offset._y;
	}

	feature_implementation Antares_Layer_Interface* Allocate_New_Layer(string& name);
	feature_implementation void Toggle_Layer(int identifier,bool checked);
	feature_implementation void Select_Layer(int identifier);
	//feature_implementation void Set_Mode(ANTARES_MODE mode);

	void Draw_Layer(int start_iteration, int end_iteration, Antares_Layer_Interface* layer);
	
	void Draw_Network();
	void Draw_Vehicles(int current_iteration);

	void Render(wxPaintEvent& event);
	void OnResize(wxSizeEvent& event);
	void OnLeftDown(wxMouseEvent& event);
    void OnLeftUp(wxMouseEvent& event);
    void OnRightDown(wxMouseEvent& event);
    void OnRightUp(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnMotion(wxMouseEvent& event);
    void OnRotationMotion(wxMouseEvent& event);
    void OnWheel(wxMouseEvent& event);
	void OnLeave(wxMouseEvent& event);
	void OnEnter(wxMouseEvent& event);
	void OnDClick(wxMouseEvent& event);
	void Zoom(wxIdleEvent& event);

	member_pointer(wxGLContext,glcontext,none,none);

	member_data(Rectangle_XY<MasterType>,canvas_bounds,none,none);
	member_data(Point_2D<MasterType>,input_offset,none,none);

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

	//member_data(bool,left_down,none,none);
	//member_data(bool,right_down,none,none);
	
	member_data(bool,alt_down,none,none);
	member_data(bool,shift_down,none,none);
	member_data(bool,ctrl_down,none,none);
	
	member_data(float,x_start_utm,none,none);
	member_data(float,y_start_utm,none,none);

	member_data(int,x_start_win,none,none);
	member_data(int,y_start_win,none,none);
	
	member_data(int,wheel_dir,none,none);

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];

	//member_prototype(Network_Prototype,graphical_network,typename MasterType::graphical_network_type,none,none);

	member_prototype(Time_Panel,time_panel,typename MasterType::type_of(time_panel),none,none);
	member_prototype(Information_Panel,information_panel,typename MasterType::type_of(information_panel),none,none);
	member_prototype(Control_Panel,control_panel,typename MasterType::type_of(control_panel),none,none);
	member_prototype(Layer_Options,layer_options,typename MasterType::type_of(layer_options),none,none);
	member_prototype(Attributes_Panel,attributes_panel,typename MasterType::type_of(attributes_panel),none,none);

	list<Antares_Layer_Interface*> _layers;
	member_pointer(Antares_Layer_Interface,selected_layer,none,none);

	//member_data(ANTARES_MODE,interaction_mode,none,none);	
};

static _lock _canvas_lock;

//---------------------------------------------------------
//	Canvas_Implementation - canvas initialization
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
Canvas_Implementation<MasterType,ParentType,InheritanceList>::Canvas_Implementation(wxFrame* parent, int* args) : wxGLCanvas(parent,-1,args,wxDefaultPosition,wxDefaultSize,wxFULL_REPAINT_ON_RESIZE|wxWANTS_CHARS)
{
	UNLOCK(_canvas_lock);

	//antares_ptr=((Antares_Implementation<MasterType>*)GetParent());

	_glcontext = new wxGLContext(this);

	//---- initialize image handling ----

	wxInitAllImageHandlers();

	Connect(wxEVT_SIZE,wxSizeEventHandler(Canvas_Implementation::OnResize));
	Connect(wxEVT_LEFT_DOWN,wxMouseEventHandler(Canvas_Implementation::OnLeftDown));
	Connect(wxEVT_KEY_DOWN,wxKeyEventHandler(Canvas_Implementation::OnKeyDown));
	Connect(wxEVT_KEY_UP,wxKeyEventHandler(Canvas_Implementation::OnKeyUp));
	Connect(wxEVT_LEFT_UP,wxMouseEventHandler(Canvas_Implementation::OnLeftUp));
	Connect(wxEVT_RIGHT_DOWN,wxMouseEventHandler(Canvas_Implementation::OnRightDown));
	Connect(wxEVT_RIGHT_UP,wxMouseEventHandler(Canvas_Implementation::OnRightUp));
	Connect(wxEVT_MOUSEWHEEL,wxMouseEventHandler(Canvas_Implementation::OnWheel));
	Connect(wxEVT_LEAVE_WINDOW,wxMouseEventHandler(Canvas_Implementation::OnLeave));
	Connect(wxEVT_ENTER_WINDOW,wxMouseEventHandler(Canvas_Implementation::OnEnter));
	Connect(wxEVT_LEFT_DCLICK,wxMouseEventHandler(Canvas_Implementation::OnDClick));

	//---- navigation ----
	
	//_left_down=_right_down=false;
	_alt_down=_shift_down=_ctrl_down=false;
	_x_translation=_y_translation=0;
	_x_rotation=_z_rotation=0;
	_x_start_utm=_y_start_utm=0;
	_x_start_win=_y_start_win=0;
	_scale=5;
	//_interaction_mode=NAVIGATE;

	//---- visibility ----

	_near_plane=_far_plane=0;
	_win_z_mid=-1;

	//---- drawing ----

	_spatial_change=true;
	_temporal_change=false;
	_cached_iteration=-1;
	
	//---- construct layers ----

	_selected_layer=nullptr;
}

//---------------------------------------------------------
//	Initialize - general initialization of canvas
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
template<typename ComponentType,typename CallerType,typename TargetType>
void Canvas_Implementation<MasterType,ParentType,InheritanceList>::Initialize(float xmin,float ymin,float xmax,float ymax)
{
	Initialize_GLCanvas();

	float xmid=(xmin+xmax)/2;
	float ymid=(ymin+ymax)/2;
	
	_input_offset._x = -xmid;
	_input_offset._y = -ymid;

	_canvas_bounds._xmin = xmin - xmid;
	_canvas_bounds._ymin = ymin - ymid;

	_canvas_bounds._xmax = xmax - xmid;
	_canvas_bounds._ymax = ymax - ymid;

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
//	Initialize_Canvas - initialization of GL Canvas
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
void Canvas_Implementation<MasterType,ParentType,InheritanceList>::Initialize_GLCanvas()
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
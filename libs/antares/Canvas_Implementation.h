//*********************************************************
//	Canvas_Implementation.h - Initialization Functions
//*********************************************************

#pragma once
#include "Canvas.h"

//---------------------------------------------------------
//	Canvas_Implementation - canvas class definition - Inherited from wxGLCanvas which process GL commands to draw
//---------------------------------------------------------

implementation class Canvas_Implementation : public Polaris_Component<MasterType,INHERIT(Canvas_Implementation),NULLTYPE>, public wxGLCanvas
{
public:
	Canvas_Implementation(wxFrame* parent, int* args);
	virtual ~Canvas_Implementation(void) {delete _glcontext;}

	void Initialize(float xmin,float ymin,float xmax,float ymax);
	void Initialize_GLCanvas();
	void Calculate_Bounds();

	typedef Antares_Layer<typename MasterType::antares_layer_type> Antares_Layer_Interface;

	//void Custom_Draw();
	//void Custom_Init();

	template<typename TargetType> Point_3D<MasterType>& Scale_Coordinates(Point_3D<MasterType>& coordinates)
	{
		coordinates._x += _input_offset._x;
		coordinates._y += _input_offset._y;

		return coordinates;
	}
	
	template<typename TargetType> Point_3D<NT>& Scale_Coordinates(Point_3D<NT>& coordinates)
	{
		coordinates._x += _input_offset._x;
		coordinates._y += _input_offset._y;

		return coordinates;
	}

	template<typename TargetType> bool Clip_Coordinates(Point_3D<MasterType>& coordinates)
	{
		return ((coordinates._x < _canvas_bounds._xmin) || (coordinates._x > _canvas_bounds._xmax) || (coordinates._y < _canvas_bounds._ymin) || (coordinates._y > _canvas_bounds._ymax));
	}

	template<typename TargetType> int Build_Texture(int width, int height, unsigned char* data);
	template<typename TargetType> Antares_Layer_Interface* Allocate_New_Layer(string& name);
	void Toggle_Layer(int identifier,bool checked);
	void Select_Layer(int identifier);
	//feature_implementation void Set_Mode(ANTARES_MODE mode);

	void Draw_Layer(int start_iteration, int end_iteration, Antares_Layer_Interface* layer);

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

	void Add_Textures();

	void Initialize_Recorder();
	void Start_Recording();
	void Stop_Recording();
	void Record_Frame();

	m_data(unsigned int*,texture_ids, NONE, NONE);
	m_data(unsigned int,current_texture_index, NONE, NONE);

	struct Queued_Texture
	{
		int tex_id;
		int width;
		int height;
		unsigned char* data;
	};
	m_data(std::vector<Queued_Texture>,queued_textures, NONE, NONE);

	m_data(wxGLContext*,glcontext, NONE, NONE);

	m_data(Rectangle_XY<MasterType>,canvas_bounds, NONE, NONE);
	m_data(Point_2D<MasterType>,input_offset, NONE, NONE);

	m_data(int,cached_iteration, NONE, NONE);

	m_data(double,near_plane, NONE, NONE);
	m_data(double,far_plane, NONE, NONE);
	
	m_data(double,win_z_mid, NONE, NONE);

	m_data(float,panel_width, NONE, NONE);
	m_data(float,panel_height, NONE, NONE);
	m_data(double,scale, NONE, NONE);

	m_data(double,x_rotation, NONE, NONE);
	m_data(double,z_rotation, NONE, NONE);
	m_data(double,x_translation, NONE, NONE);
	m_data(double,y_translation, NONE, NONE);

	m_data(double,meters_per_pixel_max, NONE, NONE);
	m_data(double,meters_per_pixel_mid, NONE, NONE);
	m_data(double,meters_per_pixel_min, NONE, NONE);

	m_data(Rectangle_XY<MasterType>,view_bounds, NONE, NONE);

	m_data(bool,spatial_change, NONE, NONE);
	m_data(bool,temporal_change, NONE, NONE);

	m_data(bool,recording, NONE, NONE);
	m_data(int,recording_index, NONE, NONE);
	m_data(int,frame_counter, NONE, NONE);

	//member_data(bool,left_down,none,none);
	//member_data(bool,right_down,none,none);
	
	m_data(bool,alt_down, NONE, NONE);
	m_data(bool,shift_down, NONE, NONE);
	m_data(bool,ctrl_down, NONE, NONE);
	
	m_data(double,x_start_utm, NONE, NONE);
	m_data(double,y_start_utm, NONE, NONE);

	m_data(int,x_start_win, NONE, NONE);
	m_data(int,y_start_win, NONE, NONE);
	
	m_data(int,wheel_dir, NONE, NONE);
	m_data(float,initial_scale, NONE, NONE);

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];

	//member_prototype(Network_Prototype,graphical_network,typename MasterType::graphical_network_type,none,none);

	m_prototype(Time_Panel,typename MasterType::time_panel_type,time_panel, NONE, NONE);
	m_prototype(Information_Panel,typename MasterType::information_panel_type,information_panel, NONE, NONE);
	m_prototype(Control_Panel,typename MasterType::control_panel_type,control_panel, NONE, NONE);
	m_prototype(Layer_Options,typename MasterType::layer_options_type,layer_options, NONE, NONE);
	m_prototype(Attributes_Panel,typename MasterType::attributes_panel_type,attributes_panel, NONE, NONE);

	std::list<Antares_Layer_Interface*> _layers;
	m_data(Antares_Layer_Interface*,selected_layer, NONE, NONE);

	//member_data(ANTARES_MODE,interaction_mode,none,none);

	HANDLE in_read;
	HANDLE in_write;

	STARTUPINFOA start_info;
	PROCESS_INFORMATION process_info;
};

static _lock _canvas_lock;

//---------------------------------------------------------
//	Canvas_Implementation - canvas initialization
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
Canvas_Implementation<MasterType,InheritanceList>::Canvas_Implementation(wxFrame* parent, int* args) : wxGLCanvas(parent,-1,args,wxDefaultPosition,wxDefaultSize,wxFULL_REPAINT_ON_RESIZE|wxWANTS_CHARS)
{
	UNLOCK(_canvas_lock);

	//antares_ptr=((Antares_Implementation<MasterType>*)GetParent());

	_glcontext = new wxGLContext(this);

	//---- initialize image handling ----

	wxInitAllImageHandlers();

	// Assign callback for navigation commands mouse/button events
	Connect(wxEVT_SIZE,wxSizeEventHandler(Canvas_Implementation::OnResize));
	Connect(wxEVT_LEFT_DOWN,wxMouseEventHandler(Canvas_Implementation::OnLeftDown));
	Connect(wxEVT_KEY_DOWN,wxKeyEventHandler(Canvas_Implementation::OnKeyDown));
	//Connect(wxEVT_KEY_UP,wxKeyEventHandler(Canvas_Implementation::OnKeyUp));
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
	_initial_scale=.01f;
	_scale=.01f;
	//_interaction_mode=NAVIGATE;

	//---- visibility ----

	_near_plane=_far_plane=0;
	_win_z_mid=-1;

	//---- drawing ----

	_spatial_change=true;		// indicates the user has changed the view
	_temporal_change=false;		// indicates whether simulation has advanced in time
	_cached_iteration=-1;


	//---- construct layers ----

	_selected_layer=nullptr;
}

//---------------------------------------------------------
//	Initialize - general initialization of canvas
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::Initialize(float xmin,float ymin,float xmax,float ymax)
{
	Initialize_GLCanvas();

	// Set canvas bounds
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
		_near_plane=(_canvas_bounds._ymax-_canvas_bounds._ymin)/(tan(25.0*.0174532925)*4.0)*10000.0;
		_far_plane=(_canvas_bounds._ymax-_canvas_bounds._ymin)/(tan(25.0*.0174532925)*4.0)*10000.0+200.0*(_canvas_bounds._ymax-_canvas_bounds._ymin);
	}
	else
	{
		_near_plane=(_canvas_bounds._xmax-_canvas_bounds._xmin)/(tan(25.0*.0174532925)*4.0)*10000.0;
		_far_plane=(_canvas_bounds._xmax-_canvas_bounds._xmin)/(tan(25.0*.0174532925)*4.0)*10000.0+200.0*(_canvas_bounds._xmax-_canvas_bounds._xmin);
	}

	//Custom_Init();

	// connect the paint handler
	Connect(wxEVT_PAINT,wxPaintEventHandler(Canvas_Implementation::Render));






	Initialize_Recorder();



}

//---------------------------------------------------------
//	Initialize_Canvas - initialization of GL Canvas
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::Initialize_GLCanvas()
{
	//---- initialize the opengl canvas settings ----

	wxGLCanvas::SetCurrent(*_glcontext);
	
	glewInit();

	glClearColor(.95f,.95f,.95f,1); // background color
	glClearDepth(1.0f);				

	glEnable(GL_DEPTH_TEST);		// test for depth before drawing overlapping objects
	glDepthFunc(GL_LESS);

	glEnable(GL_POINT_SMOOTH);		
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);

	//glShadeModel(GL_SMOOTH);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT,GL_FASTEST);

	//glEnable(GL_POLYGON_SMOOTH);
	//glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);

	float anisotropic_level;		// Anisotropic filtering - look up documentation
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropic_level);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_BLEND);		// makes GL process transparency properly
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glPixelStorei(GL_PACK_ALIGNMENT,1);

	unsigned int null_texture;

	glGenTextures(1,&null_texture);	

	_texture_ids = new unsigned int[100];
	_current_texture_index = 0;
	
	glGenTextures(100,_texture_ids);

	SetFocus();
}


template<typename MasterType,typename InheritanceList>
template<typename TargetType>
int Canvas_Implementation<MasterType,InheritanceList>::Build_Texture(int width,int height,unsigned char* data)
{
	unsigned char* data_copy = new unsigned char[ width*height*4 ];
	memcpy(data_copy, data, width*height*4);

	LOCK(_canvas_lock);

		_queued_textures.push_back(Queued_Texture());
	
		_queued_textures.back().tex_id = _texture_ids[_current_texture_index];
		_queued_textures.back().width = width;
		_queued_textures.back().height = height;
		_queued_textures.back().data = data_copy; 

		++_current_texture_index;

	UNLOCK(_canvas_lock);

	return _texture_ids[_current_texture_index-1];

	//LOCK(_canvas_lock);

	//unsigned int tex_id = 0;

	//glGenTextures(1, &tex_id);

	//float anisotropic_level;
	//glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropic_level);

	//glBindTexture(GL_TEXTURE_2D, tex_id);

	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropic_level);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	////const GLubyte* errString = gluErrorString(glGetError());

	//UNLOCK(_canvas_lock);

	//return tex_id;
}

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::Add_Textures()
{
	float anisotropic_level;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropic_level);

	anisotropic_level = min(anisotropic_level,2.0f);

	//float border_color[3] = {0.0f,0.0f,0.0f};

	for(std::vector<Queued_Texture>::iterator itr = _queued_textures.begin();itr!=_queued_textures.end();itr++)
	{
		glBindTexture(GL_TEXTURE_2D, itr->tex_id);

		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropic_level);

		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &border_color[0]);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, itr->width, itr->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, itr->data);

		delete[] itr->data;
		//const GLubyte* errString = gluErrorString(glGetError());
	}
	
	_queued_textures.clear();
}

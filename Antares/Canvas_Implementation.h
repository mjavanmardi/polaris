//*********************************************************
//	Canvas_Implementation.h - Initialization Functions
//*********************************************************

#pragma once
#include "Canvas.h"
#include "Graphical_Network_Implementation.h"

//---------------------------------------------------------
//	Canvas_Implementation - canvas class definition
//---------------------------------------------------------

implementation class Canvas_Implementation : public wxGLCanvas
{
public:
	Canvas_Implementation(wxFrame* parent, int* args);
	virtual ~Canvas_Implementation(void) {delete glcontext;}

	void Initialize();
	void Initialize_GLCanvas();

	wxGLContext* glcontext;
	
	Antares_Implementation<MasterType,NULLTYPE>* antares_ptr;

	Network_Prototype<typename MasterType::graphical_network_type,Canvas_Implementation>* graphical_network;
	typedef typename MasterType::graphical_network_type graphical_network_type;
};

//---------------------------------------------------------
//	Canvas_Implementation - canvas initialization
//---------------------------------------------------------

template<typename MasterType,typename ParentType>
Canvas_Implementation<MasterType,ParentType>::Canvas_Implementation(wxFrame* parent, int* args) : wxGLCanvas(parent,-1,args,wxDefaultPosition,wxDefaultSize,wxFULL_REPAINT_ON_RESIZE)
{
	//antares_ptr=((Antares_Implementation<MasterType>*)GetParent());

	glcontext = new wxGLContext(this);

	//---- initialize image handling ----

	wxInitAllImageHandlers();
}

//---------------------------------------------------------
//	Initialize - general initialization of canvas
//---------------------------------------------------------

template<typename MasterType,typename ParentType>
void Canvas_Implementation<MasterType,ParentType>::Initialize()
{
	graphical_network = (Network_Prototype<type_of(graphical_network),Canvas_Implementation>*) Allocate<type_of(graphical_network)>();

	graphical_network->read_network_data<Network_Components::Types::Network_Initialization_Type<Network_Components::Types::Graphical_Network,string&>>(((Antares_Implementation<MasterType>*)GetParent())->db_name);

	Initialize_GLCanvas();
}

//---------------------------------------------------------
//	Initialize_Canvas - initialization of GL Canvas
//---------------------------------------------------------

template<typename MasterType,typename ParentType>
void Canvas_Implementation<MasterType,ParentType>::Initialize_GLCanvas()
{
	//---- initialize the opengl canvas settings ----

	wxGLCanvas::SetCurrent(*glcontext);
	
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
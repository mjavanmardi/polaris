//*********************************************************
//	Canvas_Implementation.h - Initialization Functions
//*********************************************************

#pragma once
#include "Canvas.h"

//---------------------------------------------------------
//	Canvas - canvas initialization
//---------------------------------------------------------

Canvas::Canvas(wxFrame* parent, int* args) : wxGLCanvas(parent,-1,args,wxDefaultPosition,wxDefaultSize,wxFULL_REPAINT_ON_RESIZE)
{
	glcontext = new wxGLContext(this);

	//---- initialize image handling ----

	wxInitAllImageHandlers();
}

//---------------------------------------------------------
//	Initialize_Canvas - initialization of GL Canvas
//---------------------------------------------------------

void Canvas::Initialize_GLCanvas()
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
//*********************************************************
//	Canvas.h - Visualization Panel
//*********************************************************

#pragma once
#include "Antares_Includes.h"

//---------------------------------------------------------
//	Canvas - canvas class definition
//---------------------------------------------------------

class Canvas : public wxGLCanvas
{
public:
	Canvas(wxFrame* parent, int* args);
	virtual ~Canvas(void) {delete glcontext;}

	void Initialize_GLCanvas();

	wxGLContext* glcontext;
};
//*********************************************************
//	Canvas.h - Visualization Panel
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "Graphical_Network_Implementation.h"

//---------------------------------------------------------
//	Canvas - canvas class definition
//---------------------------------------------------------

implementation class Canvas : public wxGLCanvas
{
public:
	Canvas(wxFrame* parent, int* args);
	virtual ~Canvas(void) {delete glcontext;}

	void Initialize();
	void Initialize_GLCanvas();

	wxGLContext* glcontext;
	
	//Antares<MasterType,NULLTYPE>* antares_ptr;

	Network_Prototype<typename MasterType::graphical_network_type,Canvas>* graphical_network;
	typedef typename MasterType::graphical_network_type graphical_network_type;
};
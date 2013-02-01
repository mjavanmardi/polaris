//*********************************************************
//	Information_Panel.h - Information and Plotting
//*********************************************************

#pragma once
#include "Antares_Includes.h"

//---------------------------------------------------------
//	Information_Panel - information panel class definition
//---------------------------------------------------------

class Information_Panel : public wxPanel
{
public:
	Information_Panel(wxFrame* parent);
	virtual ~Information_Panel(void){};

	Canvas* canvas_ptr;
};
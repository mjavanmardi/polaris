//*********************************************************
//	Antares_Frame.h - Parent Frame
//*********************************************************

#pragma once
#include "Canvas.h"
#include "Time_Panel.h"
#include "Information_Panel.h"
#include "Control_Panel.h"

//---------------------------------------------------------
//	Antares - aui initialization
//---------------------------------------------------------

class Antares : public wxFrame
{
public:
	Antares::Antares(wxFrame* parent);

	virtual ~Antares(void){mgr.UnInit();}

	Control_Panel* control_panel;
	Time_Panel* time_panel;
	Information_Panel* information_panel;
	Canvas* canvas;
	wxAuiManager mgr;
};
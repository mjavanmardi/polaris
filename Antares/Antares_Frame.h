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

implementation class Antares : public wxFrame
{
public:
	Antares(wxFrame* parent,void* _simulation_ptr,string& _db_name);

	virtual ~Antares(void){mgr.UnInit();}

	Control_Panel* control_panel;
	Time_Panel* time_panel;
	Information_Panel* information_panel;
	Canvas<MasterType,Antares>* canvas;
	
	wxAuiManager mgr;
	
	string db_name;
	void* simulation_ptr;
};
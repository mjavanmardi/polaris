//*********************************************************
//	Control_Panel.h - Container Panel for Controls
//*********************************************************

#pragma once
#include "Antares_Includes.h"

//---------------------------------------------------------
//	Control_Panel - control panel class definition
//---------------------------------------------------------

class Control_Panel : public wxPanel
{
public:
	Control_Panel(wxFrame* parent);
	virtual ~Control_Panel(void){};

	wxAuiNotebook* control_book;

	wxBoxSizer* sizer;
};
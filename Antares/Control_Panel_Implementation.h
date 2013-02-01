//*********************************************************
//	Control_Panel_Implementation.cpp - Container Panel for Controls
//*********************************************************

#pragma once
#include "Control_Panel.h"

//---------------------------------------------------------
//	Control_Panel - control_panel initialization
//---------------------------------------------------------

Control_Panel::Control_Panel(wxFrame* parent) : wxPanel(parent,-1,wxDefaultPosition,wxDefaultSize)
{
	SetBackgroundColour(wxColor(255,255,255));

	//---- initialize the sizer and container notebook ----
	
	sizer=new wxBoxSizer(wxVERTICAL);

	control_book=new wxAuiNotebook(this,-1,wxDefaultPosition,wxDefaultSize,wxAUI_NB_TOP);
	sizer->Add(control_book,1,wxEXPAND);
	
	//---- initialize and add the components ----

	//---- set the sizer ----

	SetSizerAndFit(sizer);
}
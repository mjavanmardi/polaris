//*********************************************************
//	Control_Panel_Implementation.cpp - Container Panel for Controls
//*********************************************************

#pragma once
#include "Control_Panel.h"

//---------------------------------------------------------
//	Control_Panel_Implementation - control panel class definition
//---------------------------------------------------------

implementation class Control_Panel_Implementation : public wxPanel
{
public:
	Control_Panel_Implementation(wxFrame* parent);
	virtual ~Control_Panel_Implementation(void){};

	wxAuiNotebook* control_book;

	wxBoxSizer* sizer;

};

//---------------------------------------------------------
//	Control_Panel_Implementation - control_panel initialization
//---------------------------------------------------------

template<typename MasterType,typename ParentType>
Control_Panel_Implementation<MasterType,ParentType>::Control_Panel_Implementation(wxFrame* parent) : wxPanel(parent,-1,wxDefaultPosition,wxDefaultSize)
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
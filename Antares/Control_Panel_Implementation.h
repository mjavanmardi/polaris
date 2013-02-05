//*********************************************************
//	Control_Panel_Implementation.cpp - Container Panel for Controls
//*********************************************************

#pragma once
#include "Control_Panel.h"

//---------------------------------------------------------
//	Control_Panel_Implementation - control panel class definition
//---------------------------------------------------------

implementation class Control_Panel_Implementation : public Polaris_Component_Class<Control_Panel_Implementation,MasterType,NULLTYPE>,public wxPanel
{
public:
	Control_Panel_Implementation(wxFrame* parent);
	virtual ~Control_Panel_Implementation(void){};

	member_pointer(wxAuiNotebook,control_book,none,none);
	member_pointer(wxBoxSizer,sizer,none,none);
};

//---------------------------------------------------------
//	Control_Panel_Implementation - control_panel initialization
//---------------------------------------------------------

template<typename MasterType,typename ParentType>
Control_Panel_Implementation<MasterType,ParentType>::Control_Panel_Implementation(wxFrame* parent) : wxPanel(parent,-1,wxDefaultPosition,wxDefaultSize)
{
	SetBackgroundColour(wxColor(255,255,255));

	//---- initialize the sizer and container notebook ----
	
	_sizer=new wxBoxSizer(wxVERTICAL);

	_control_book=new wxAuiNotebook(this,-1,wxDefaultPosition,wxDefaultSize,wxAUI_NB_TOP);
	_sizer->Add(_control_book,1,wxEXPAND);
	
	//---- initialize and add the components ----

	//---- set the sizer ----

	SetSizerAndFit(_sizer);
}
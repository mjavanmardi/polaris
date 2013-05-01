//*********************************************************
//	Control_Panel_Implementation.cpp - Container Panel for Controls
//*********************************************************

#pragma once
#include "Control_Panel.h"
#include "Layer_Options.h"
#include "Splash_Panel.h"

//---------------------------------------------------------
//	Control_Panel_Implementation - control panel class definition
//---------------------------------------------------------

implementation class Control_Panel_Implementation : public Polaris_Component<APPEND_CHILD(Control_Panel_Implementation),MasterType,NULLTYPE>,public wxPanel
{
public:
	Control_Panel_Implementation(wxFrame* parent);
	virtual ~Control_Panel_Implementation(void){};

	member_prototype(Layer_Options,layer_options,typename MasterType::type_of(layer_options),none,none);
	member_prototype(Canvas,canvas,typename MasterType::type_of(canvas),none,none);
	member_prototype(Splash_Panel,splash_panel,typename MasterType::type_of(splash_panel),none,none);

	member_pointer(wxAuiNotebook,control_book,none,none);
	member_pointer(wxBoxSizer,sizer,none,none);
};

//---------------------------------------------------------
//	Control_Panel_Implementation - control_panel initialization
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
Control_Panel_Implementation<MasterType,ParentType,InheritanceList>::Control_Panel_Implementation(wxFrame* parent) : wxPanel(parent,-1,wxDefaultPosition,wxDefaultSize)
{
	SetBackgroundColour(wxColor(255,255,255));

	//---- initialize the sizer and container notebook ----
	
	_sizer=new wxBoxSizer(wxVERTICAL);
	_splash_panel=(splash_panel_interface*)new type_of(splash_panel)(this);

	_sizer->Add((wxPanel*)_splash_panel);

	_control_book=new wxAuiNotebook(this,-1,wxDefaultPosition,wxDefaultSize,wxAUI_NB_TOP);
	_sizer->Add(_control_book,1,wxEXPAND);
	
	//---- initialize and add the components ----

	_layer_options = (Layer_Options<type_of(layer_options),ComponentType>*)new type_of(layer_options)(this);
	_control_book->AddPage((wxPanel*)_layer_options,"Layer");

	//---- set the sizer ----

	SetSizerAndFit(_sizer);
}
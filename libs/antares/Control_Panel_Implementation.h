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

implementation class Control_Panel_Implementation : public Polaris_Component<MasterType,INHERIT(Control_Panel_Implementation),NULLTYPE>,public wxPanel
{
public:
	Control_Panel_Implementation(wxFrame* parent);
	virtual ~Control_Panel_Implementation(void){};

	m_prototype(Layer_Options,typename MasterType::layer_options_type,layer_options, NONE, NONE);
	m_prototype(Canvas,typename MasterType::canvas_type,canvas, NONE, NONE);
	m_prototype(Splash_Panel,typename MasterType::splash_panel_type,splash_panel, NONE, NONE);

	m_data(wxAuiNotebook*,control_book, NONE, NONE);
	m_data(wxBoxSizer*,sizer, NONE, NONE);
};

//---------------------------------------------------------
//	Control_Panel_Implementation - control_panel initialization
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
Control_Panel_Implementation<MasterType,InheritanceList>::Control_Panel_Implementation(wxFrame* parent) : wxPanel(parent,-1,wxDefaultPosition,wxDefaultSize)
{
	SetBackgroundColour(wxColor(255,255,255));

	//---- initialize the sizer and container notebook ----
	
	_sizer=new wxBoxSizer(wxVERTICAL);
	_splash_panel=(splash_panel_type)new type_of(splash_panel)(this);

	_sizer->Add((wxPanel*)_splash_panel);

	_control_book=new wxAuiNotebook(this,-1,wxDefaultPosition,wxDefaultSize,wxAUI_NB_TOP);
	_sizer->Add(_control_book,1,wxEXPAND);
	
	//---- initialize and add the components ----

	_layer_options = (Layer_Options<type_of(layer_options)>*)new type_of(layer_options)(this);
	_control_book->AddPage((wxPanel*)_layer_options,"Layer");

	//---- set the sizer ----

	SetSizerAndFit(_sizer);
}

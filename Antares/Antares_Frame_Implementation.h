//*********************************************************
//	Antares_Frame_Implementation.h - Parent Frame
//*********************************************************

#pragma once
#include "Antares_Frame.h"

//---------------------------------------------------------
//	Antares_Implementation - aui initialization
//---------------------------------------------------------

implementation class Antares_Implementation : public Polaris_Component<APPEND_CHILD(Antares_Implementation),MasterType,NULLTYPE>,public wxFrame
{
public:
	Antares_Implementation(wxFrame* parent);
	virtual ~Antares_Implementation(void){_aui_manager.UnInit();}

	member_data(wxAuiManager,aui_manager,none,none);

	member_prototype(Conductor,conductor,typename MasterType::type_of(conductor),none,none);
	member_prototype(Control_Panel,control_panel,typename MasterType::type_of(control_panel),none,none);
	member_prototype(Time_Panel,time_panel,typename MasterType::type_of(time_panel),none,none);
	member_prototype(Information_Panel,information_panel,typename MasterType::type_of(information_panel),none,none);
	member_prototype(Canvas,canvas,typename MasterType::type_of(canvas),none,none);
	member_prototype(Attributes_Panel,attributes_panel,typename MasterType::type_of(attributes_panel),none,none);

	define_component_interface(layer_options_interface,typename MasterType::type_of(layer_options),Layer_Options,Antares_Implementation);
};

template<typename MasterType,typename ParentType,typename InheritanceList>
Antares_Implementation<MasterType,ParentType,InheritanceList>::Antares_Implementation(wxFrame* parent):wxFrame(parent,-1,"Antares")
{
	//---- initialize conductor ----

	_conductor=(conductor_interface*)Allocate<typename MasterType::type_of(conductor)>();

	_conductor->Initialize<NULLTYPE>();

	//---- initialize aui ----

	_aui_manager.SetManagedWindow(this);

	_aui_manager.GetArtProvider()->SetMetric(wxAUI_DOCKART_GRADIENT_TYPE,2);
	_aui_manager.GetArtProvider()->SetColor(wxAUI_DOCKART_BORDER_COLOUR,wxColor(255,255,255));
	_aui_manager.GetArtProvider()->SetColor(wxAUI_DOCKART_SASH_COLOUR,wxColor(255,255,255));
	_aui_manager.GetArtProvider()->SetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE,0);
	_aui_manager.GetArtProvider()->SetMetric(wxAUI_DOCKART_SASH_SIZE,5);
	_aui_manager.GetArtProvider()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR,wxColor(200,15,15));
	_aui_manager.GetArtProvider()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR,wxColor(255,255,255));

	//---- initialize and add the primary widgets ----

	int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 32, 0};

	_canvas=(canvas_interface*)new type_of(canvas)(this,args);

	_aui_manager.AddPane((type_of(canvas)*)_canvas,wxAuiPaneInfo().Name("Canvas").CenterPane());

	_control_panel=(control_panel_interface*)new type_of(control_panel)(this);
	
	_aui_manager.AddPane((type_of(control_panel)*)_control_panel,wxAuiPaneInfo().Name("Control_Panel").Left().MinSize(300,100));

	_information_panel=(information_panel_interface*)new type_of(information_panel)(this);
	
	_aui_manager.AddPane((type_of(information_panel)*)_information_panel,wxAuiPaneInfo().Name("Information_Panel").Bottom().MinSize(1920-300,300));
	
	_time_panel=(time_panel_interface*)new type_of(time_panel)(this);

	_aui_manager.InsertPane((type_of(time_panel)*)_time_panel,wxAuiPaneInfo().Name("Time_Panel").Bottom());

	_attributes_panel=(attributes_panel_interface*)new type_of(attributes_panel)(this);
	
	_aui_manager.AddPane((type_of(attributes_panel)*)_attributes_panel,wxAuiPaneInfo().Name("Attributes_Panel").Right().MinSize(200,100));

	_aui_manager.Update();
	
	//---- connect conductor to other panels ----

	_conductor->canvas<canvas_interface*>(_canvas);
	_conductor->information_panel<information_panel_interface*>(_information_panel);
	
	//---- connect canvas to other panels ----

	_canvas->time_panel<time_panel_interface*>(_time_panel);
	_canvas->information_panel<information_panel_interface*>(_information_panel);
	_canvas->attributes_panel<attributes_panel_interface*>(_attributes_panel);
	_canvas->control_panel<control_panel_interface*>(_control_panel);
	_canvas->layer_options<layer_options_interface*>( _control_panel->layer_options<layer_options_interface*>() );

	//---- connect control_panel to other panels ----

	_control_panel->canvas<canvas_interface*>(_canvas);
	_control_panel->layer_options<layer_options_interface*>()->canvas<canvas_interface*>(_canvas);
}
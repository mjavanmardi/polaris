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
	Antares_Implementation(wxFrame* parent,string& _db_name);
	virtual ~Antares_Implementation(void){_aui_manager.UnInit();}
	void Render(wxPaintEvent& event);

	member_data(wxAuiManager,aui_manager,none,none);
	member_data(string,db_name,none,none);

	member_prototype(Conductor,conductor,typename MasterType::type_of(conductor),none,none);
	member_prototype(Control_Panel,control_panel,typename MasterType::type_of(control_panel),none,none);
	member_prototype(Time_Panel,time_panel,typename MasterType::type_of(time_panel),none,none);
	member_prototype(Information_Panel,information_panel,typename MasterType::type_of(information_panel),none,none);
	member_prototype(Canvas,canvas,typename MasterType::type_of(canvas),none,none);

	define_component_interface(Conductor_Interface,type_of(conductor),Conductor,Antares_Implementation);	
	define_component_interface(Control_Panel_Interface,type_of(control_panel),Control_Panel,Antares_Implementation);
	define_component_interface(Time_Panel_Interface,type_of(time_panel),Time_Panel,Antares_Implementation);
	define_component_interface(Information_Panel_Interface,type_of(information_panel),Information_Panel,Antares_Implementation);
	define_component_interface(Canvas_Interface,type_of(canvas),Canvas,Antares_Implementation);
	define_component_interface(Layer_Options_Interface,typename MasterType::type_of(layer_options),Layer_Options,Antares_Implementation);
};

template<typename MasterType,typename ParentType,typename InheritanceList>
Antares_Implementation<MasterType,ParentType,InheritanceList>::Antares_Implementation(wxFrame* parent,string& __db_name):wxFrame(parent,-1,"Antares")
{
	//---- initialize conductor ----

	_conductor=(Conductor_Interface*)Allocate<typename MasterType::type_of(conductor)>();

	_conductor->Initialize<NULLTYPE>();
	
	//---- set references ----

	_db_name=__db_name;

	//---- initialize aui ----

	_aui_manager.SetManagedWindow(this);

	_aui_manager.GetArtProvider()->SetMetric(wxAUI_DOCKART_GRADIENT_TYPE,2);
	_aui_manager.GetArtProvider()->SetColor(wxAUI_DOCKART_BORDER_COLOUR,wxColor(255,255,255));
	_aui_manager.GetArtProvider()->SetColor(wxAUI_DOCKART_SASH_COLOUR,wxColor(255,255,255));
	_aui_manager.GetArtProvider()->SetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE,0);
	_aui_manager.GetArtProvider()->SetMetric(wxAUI_DOCKART_SASH_SIZE,5);
	_aui_manager.GetArtProvider()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR,wxColor(50,150,255));
	_aui_manager.GetArtProvider()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR,wxColor(255,255,255));

	//---- initialize and add the primary widgets ----

	int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 32, 0};

	_canvas=(Canvas_Interface*)new type_of(canvas)(this,args);

	_aui_manager.AddPane((type_of(canvas)*)_canvas,wxAuiPaneInfo().Name("Canvas").CenterPane());

	_control_panel=(Control_Panel_Interface*)new type_of(control_panel)(this);
	
	_aui_manager.AddPane((type_of(control_panel)*)_control_panel,wxAuiPaneInfo().Name("Control_Panel").Left().MinSize(300,200));
	
	_time_panel=(Time_Panel_Interface*)new type_of(time_panel)(this);
	
	_aui_manager.AddPane((type_of(time_panel)*)_time_panel,wxAuiPaneInfo().Name("Time_Panel").Bottom());

	_information_panel=(Information_Panel_Interface*)new type_of(information_panel)(this);
	
	_aui_manager.AddPane((type_of(information_panel)*)_information_panel,wxAuiPaneInfo().Name("Information_Panel").Bottom().MinSize(800,200));

	_aui_manager.Update();
	
	//---- connect conductor to other panels ----

	_conductor->canvas<Canvas_Interface*>(_canvas);
	_conductor->information_panel<Information_Panel_Interface*>(_information_panel);
	
	//---- connect canvas to other panels ----

	_canvas->time_panel<Time_Panel_Interface*>(_time_panel);
	_canvas->information_panel<Information_Panel_Interface*>(_information_panel);
	_canvas->control_panel<Control_Panel_Interface*>(_control_panel);
	_canvas->layer_options<Layer_Options_Interface*>( _control_panel->layer_options<Layer_Options_Interface*>() );

	//---- connect control_panel to other panels ----

	_control_panel->canvas<Canvas_Interface*>(_canvas);
	_control_panel->layer_options<Layer_Options_Interface*>()->canvas<Canvas_Interface*>(_canvas);
}
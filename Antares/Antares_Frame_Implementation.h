//*********************************************************
//	Antares_Frame_Implementation.h - Parent Frame
//*********************************************************

#pragma once
#include "Antares_Frame.h"

//---------------------------------------------------------
//	Antares_Implementation - aui initialization
//---------------------------------------------------------

template<typename MasterType, typename ParentType=NULLTYPE>
class Antares_Implementation : public Polaris_Component_Class<Antares_Implementation,MasterType,NULLTYPE>,public wxFrame
{
public:
	Antares_Implementation(wxFrame* parent,void* _simulation_ptr,string& _db_name);
	virtual ~Antares_Implementation(void){_aui_manager.UnInit();}
	void Render(wxPaintEvent& event);

	member_data(wxAuiManager,aui_manager,none,none);
	member_data(string,db_name,none,none);
	member_pointer(void,simulation_ptr,none,none);

	member_prototype(Conductor,conductor,typename MasterType::type_of(conductor),Antares_Implementation,none,none);
	member_prototype(Control_Panel,control_panel,typename MasterType::type_of(control_panel),Antares_Implementation,none,none);
	member_prototype(Time_Panel,time_panel,typename MasterType::type_of(time_panel),Antares_Implementation,none,none);
	member_prototype(Information_Panel,information_panel,typename MasterType::type_of(information_panel),Antares_Implementation,none,none);
	member_prototype(Canvas,canvas,typename MasterType::type_of(canvas),Antares_Implementation,none,none);


	define_component_interface(Conductor_Interface,type_of(conductor),Conductor,Antares_Implementation);	
	define_component_interface(Control_Panel_Interface,type_of(control_panel),Control_Panel,Antares_Implementation);
	define_component_interface(Time_Panel_Interface,type_of(time_panel),Time_Panel,Antares_Implementation);
	define_component_interface(Information_Panel_Interface,type_of(information_panel),Information_Panel,Antares_Implementation);
	define_component_interface(Canvas_Interface,type_of(canvas),Canvas,Antares_Implementation);
};

template<typename MasterType,typename ParentType>
Antares_Implementation<MasterType,ParentType>::Antares_Implementation(wxFrame* parent,void* __simulation_ptr,string& __db_name):wxFrame(parent,-1,"Antares")
{
	//---- initialize conductor ----

	_conductor=(Conductor_Interface*)Allocate<Conductor_Implementation<MasterType,NULLTYPE>>();

	_conductor->Initialize<NULLTYPE>();
	
	//---- set references ----

	_simulation_ptr=__simulation_ptr;
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

	_canvas=(Canvas_Interface*)new Canvas_Implementation<MasterType,Antares_Implementation>(this,args);

	_aui_manager.AddPane((type_of(canvas)*)_canvas,wxAuiPaneInfo().Name("Canvas").CenterPane());

	_control_panel=(Control_Panel_Interface*)new Control_Panel_Implementation<MasterType,Antares_Implementation>(this);
	
	_aui_manager.AddPane((type_of(control_panel)*)_control_panel,wxAuiPaneInfo().Name("Control_Panel").Left());
	
	_time_panel=(Time_Panel_Interface*)new Time_Panel_Implementation<MasterType,Antares_Implementation>(this);
	
	_aui_manager.AddPane((type_of(time_panel)*)_time_panel,wxAuiPaneInfo().Name("Time_Panel").Bottom());

	_information_panel=(Information_Panel_Interface*)new Information_Panel_Implementation<MasterType,Antares_Implementation>(this);
	
	_aui_manager.AddPane((type_of(information_panel)*)_information_panel,wxAuiPaneInfo().Name("Information_Panel").Bottom().MinSize(800,200));

	_aui_manager.Update();

}
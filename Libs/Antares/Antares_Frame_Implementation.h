//*********************************************************
//	Antares_Frame_Implementation.h - Parent Frame
//*********************************************************

#pragma once
#include "Antares_Frame.h"

//---------------------------------------------------------
//	Antares_Implementation - aui initialization
//---------------------------------------------------------

implementation class Antares_Implementation : public Polaris_Component<MasterType,INHERIT(Antares_Implementation),NULLTYPE>,public wxFrame
{
public:
	Antares_Implementation(wxFrame* parent);
	virtual ~Antares_Implementation(void){_aui_manager.UnInit();}

	m_data(wxAuiManager,aui_manager,NONE,NONE);

	m_data(wxMenuBar*,menu_bar,NONE,NONE);
	m_data(wxMenu*,file_menu,NONE,NONE);
	m_data(wxMenu*,edit_menu,NONE,NONE);
	m_data(wxMenu*,view_menu,NONE,NONE);
	m_data(wxMenu*,window_menu,NONE,NONE);
	m_data(wxMenu*,help_menu,NONE,NONE);

	m_data(wxStatusBar*,status_bar,NONE,NONE);

	m_prototype(Conductor<typename MasterType::type_of(conductor)>,conductor,NONE,NONE);
	m_prototype(Control_Panel<typename MasterType::type_of(control_panel)>,control_panel,NONE,NONE);
	m_prototype(Time_Panel<typename MasterType::type_of(time_panel)>,time_panel,NONE,NONE);
	m_prototype(Information_Panel<typename MasterType::type_of(information_panel)>,information_panel,NONE,NONE);
	m_prototype(Canvas<typename MasterType::type_of(canvas)>,canvas,NONE,NONE);
	m_prototype(Attributes_Panel<typename MasterType::type_of(attributes_panel)>,attributes_panel,NONE,NONE);

	typedef Layer_Options<typename MasterType::type_of(layer_options)> layer_options_interface_type;

	//define_component_interface(layer_options_interface,typename MasterType::type_of(layer_options),Layer_Options,Antares_Implementation);
};

template<typename MasterType,typename InheritanceList>
Antares_Implementation<MasterType,InheritanceList>::Antares_Implementation(wxFrame* parent):wxFrame(parent,-1,"POLARIS")
{
	//---- initialize conductor ----

	_conductor=(conductor_interface_type*)Allocate<typename MasterType::type_of(conductor)>();

	_conductor->Initialize();
	
	//---- initialize icon ----

	SetIcon( wxIcon("C:\\opt\\polarisdeps\\antares\\POLARIS_Icon.ico",wxBITMAP_TYPE_ICO) );

	//---- initialize aui ----

	_aui_manager.SetManagedWindow(this);

	_aui_manager.GetArtProvider()->SetMetric(wxAUI_DOCKART_GRADIENT_TYPE,2);
	_aui_manager.GetArtProvider()->SetColor(wxAUI_DOCKART_BORDER_COLOUR,wxColor(255,255,255));
	_aui_manager.GetArtProvider()->SetColor(wxAUI_DOCKART_SASH_COLOUR,wxColor(0,0,0));
	_aui_manager.GetArtProvider()->SetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE,0);
	_aui_manager.GetArtProvider()->SetMetric(wxAUI_DOCKART_SASH_SIZE,5);
	_aui_manager.GetArtProvider()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR,wxColor(15,15,200));
	_aui_manager.GetArtProvider()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR,wxColor(0,0,0));
	
	//---- initialize the menu bar ----

	_menu_bar= new wxMenuBar();

	_file_menu = new wxMenu();
	_menu_bar->Append(_file_menu,"File");

	_edit_menu = new wxMenu();
	_menu_bar->Append(_edit_menu,"Edit");

	_view_menu = new wxMenu();
	_menu_bar->Append(_view_menu,"View");

	_window_menu = new wxMenu();
	_menu_bar->Append(_window_menu,"Window");

	_help_menu = new wxMenu();
	_menu_bar->Append(_help_menu,"Help");

	SetMenuBar(_menu_bar);

	//---- initialize the menu bar ----
	
	_status_bar= CreateStatusBar();
	
	_status_bar->SetStatusText("Ready");

	SetStatusBar(_status_bar);


	//---- initialize and add the primary widgets ----

	int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 32, 0};

	_canvas=(canvas_interface_type*)new type_of(canvas)(this,args);

	_aui_manager.AddPane((type_of(canvas)*)_canvas,wxAuiPaneInfo().Name("Canvas").CenterPane().CloseButton(false));

	_control_panel=(control_panel_interface_type*)new type_of(control_panel)(this);
	
	_aui_manager.AddPane((type_of(control_panel)*)_control_panel,wxAuiPaneInfo().Name("Control_Panel").Left().MinSize(250,100).CloseButton(false));
	


	_time_panel=(time_panel_interface_type*)new type_of(time_panel)(this);

	_aui_manager.AddPane((type_of(time_panel)*)_time_panel,wxAuiPaneInfo().Name("Time_Panel").Bottom().MinSize(250,300).CloseButton(false).Position(0));
	
	_information_panel=(information_panel_interface_type*)new type_of(information_panel)(this);

	_aui_manager.AddPane((type_of(information_panel)*)_information_panel,wxAuiPaneInfo().Name("Information_Panel").Bottom().MinSize(1920-250,300).CloseButton(false).Position(1));
	


	_attributes_panel=(attributes_panel_interface_type*)new type_of(attributes_panel)(this);
	
	_aui_manager.AddPane((type_of(attributes_panel)*)_attributes_panel,wxAuiPaneInfo().Name("Attributes_Panel").Right().MinSize(250,100).CloseButton(false));

	_aui_manager.Update();
	
	//---- connect conductor to other panels ----

	_conductor->canvas<canvas_interface_type*>(_canvas);
	_conductor->information_panel<information_panel_interface_type*>(_information_panel);
	
	//---- connect canvas to other panels ----

	_canvas->time_panel<time_panel_interface_type*>(_time_panel);
	_canvas->information_panel<information_panel_interface_type*>(_information_panel);
	_canvas->attributes_panel<attributes_panel_interface_type*>(_attributes_panel);
	_canvas->control_panel<control_panel_interface_type*>(_control_panel);
	_canvas->layer_options<layer_options_interface_type*>( _control_panel->layer_options<layer_options_interface_type*>() );

	//---- connect control_panel to other panels ----

	_control_panel->canvas<canvas_interface_type*>(_canvas);
	_control_panel->layer_options<layer_options_interface_type*>()->canvas<canvas_interface_type*>(_canvas);
}
//*********************************************************
//	Antares_Frame_Implementation.h - Parent Frame
//*********************************************************

#pragma once
#include "Antares_Frame.h"
#include <fstream>

//---------------------------------------------------------
//	Antares_Implementation - aui initialization
//---------------------------------------------------------

implementation class Antares_Implementation : public Polaris_Component<MasterType,INHERIT(Antares_Implementation)>,public wxFrame
{
public:
	Antares_Implementation(wxFrame* parent);
	virtual ~Antares_Implementation(void){_aui_manager.UnInit();}

	m_data(wxAuiManager,aui_manager, NONE, NONE);
    
	void OnHidePlot(wxCommandEvent& event);
    void OnShowPlot(wxCommandEvent& event);
	
	void OnLoadView(wxCommandEvent& event);
    void OnSaveView(wxCommandEvent& event);

	m_data(wxMenuBar*,menu_bar, NONE, NONE);
	m_data(wxMenu*,file_menu, NONE, NONE);
	m_data(wxMenu*,edit_menu, NONE, NONE);
	m_data(wxMenu*,view_menu, NONE, NONE);
	m_data(wxMenu*,window_menu, NONE, NONE);
	m_data(wxMenu*,help_menu, NONE, NONE);
	m_data(wxMenuItem*,hide_plot, NONE, NONE);
	m_data(wxMenuItem*,show_plot, NONE, NONE);
	
	m_data(wxMenuItem*,load_view, NONE, NONE);
	m_data(wxMenuItem*,save_view, NONE, NONE);

	m_data(wxStatusBar*,status_bar, NONE, NONE);

	m_prototype(Conductor,typename MasterType::conductor_type,conductor, NONE, NONE);
	m_prototype(Control_Panel,typename MasterType::control_panel_type,control_panel, NONE, NONE);
	m_prototype(Time_Panel,typename MasterType::time_panel_type,time_panel, NONE, NONE);
	m_prototype(Information_Panel,typename MasterType::information_panel_type,information_panel, NONE, NONE);
	m_prototype(Canvas,typename MasterType::canvas_type,canvas, NONE, NONE);
	m_prototype(Attributes_Panel,typename MasterType::attributes_panel_type,attributes_panel, NONE, NONE);

	typedef Layer_Options<typename MasterType::layer_options_type>* layer_options_type;
};

template<typename MasterType,typename InheritanceList>
void Antares_Implementation<MasterType,InheritanceList>::OnHidePlot(wxCommandEvent& event)
{
	//((type_of(MasterType::information_panel)*)_information_panel)->Hide();

	//cout << "Got Information Panel " << _aui_manager.GetPane("Information_Panel").IsOk() << endl;

	_aui_manager.GetPane("Information_Panel").Hide();
	
	//cout << "Updated Information Panel " << _aui_manager.GetPane("Information_Panel").IsShown() << endl;

	_aui_manager.Update();
	
	//cout << "Updated AUI Manager " << _aui_manager.GetPane("Information_Panel").IsShown() << endl;

	//cout << "Got Time Panel " << _aui_manager.GetPane("Time_Panel").IsOk() << endl;

	_aui_manager.GetPane("Time_Panel").Float();

	_aui_manager.Update();

	_aui_manager.GetPane("Time_Panel").Dock().Left().Position(1);

	_aui_manager.Update();
}

template<typename MasterType,typename InheritanceList>
void Antares_Implementation<MasterType,InheritanceList>::OnShowPlot(wxCommandEvent& event)
{
	//((type_of(MasterType::information_panel)*)_information_panel)->Show();

	_aui_manager.GetPane("Information_Panel").Show(true);
	
	_aui_manager.Update();

	_aui_manager.GetPane("Time_Panel").Bottom().Position(0);

	_aui_manager.Update();
}

struct View_File
{
	double scale;
	double x_rotation;
	double z_rotation;
	double x_translation;
	double y_translation;
};

template<typename MasterType,typename InheritanceList>
void Antares_Implementation<MasterType,InheritanceList>::OnLoadView(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, _("Open View File"), "", "", "View files (*.vis)|*.vis", wxFD_OPEN|wxFD_FILE_MUST_EXIST);

	if(openFileDialog.ShowModal() == wxID_OK)
	{
		bool loading_successful = false;
		
		ifstream view_file((char*)openFileDialog.GetPath().ToStdString().c_str(),ios::binary);
		
		if(view_file.is_open())
		{
			view_file.seekg(0,ios::end);
			size_t file_size = view_file.tellg();
			view_file.seekg(0,ios::beg);
			
			if(file_size == sizeof(View_File))
			{
				char* view = new char[file_size];

				view_file.read(view,file_size);

				View_File* view_data = (View_File*)view;

				_canvas->scale<double>(view_data->scale);
				_canvas->x_rotation<double>(view_data->x_rotation);
				_canvas->z_rotation<double>(view_data->z_rotation);
				_canvas->x_translation<double>(view_data->x_translation);
				_canvas->y_translation<double>(view_data->y_translation);
				_canvas->spatial_change<bool>(true);

				_canvas->Refresh<NT>();

				loading_successful = true;
			}
		}

		if(!loading_successful)
		{
			wxMessageDialog(this,"Error Opening View File!");
		}
	}
} 

template<typename MasterType,typename InheritanceList>
void Antares_Implementation<MasterType,InheritanceList>::OnSaveView(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(this, _("Save View File"), "", "", "View files (*.vis)|*.vis", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

	if(saveFileDialog.ShowModal() == wxID_OK)
	{
		bool saving_successful = false;

		ofstream view_file((char*)saveFileDialog.GetPath().ToStdString().c_str(),ios::binary);
		
		if(view_file.is_open())
		{
			View_File view_data;

			view_data.scale = _canvas->scale<double>();
			view_data.x_rotation = _canvas->x_rotation<double>();
			view_data.z_rotation = _canvas->z_rotation<double>();
			view_data.x_translation = _canvas->x_translation<double>();
			view_data.y_translation = _canvas->y_translation<double>();

			view_file.write((char*)&view_data,sizeof(View_File));

			saving_successful = true;
		}

		if(!saving_successful)
		{
			wxMessageDialog(this,"Error Saving View File!");
		}
	}
}

template<typename MasterType,typename InheritanceList>
Antares_Implementation<MasterType,InheritanceList>::Antares_Implementation(wxFrame* parent):wxFrame(parent,-1,"POLARIS")
{
	//---- initialize conductor ---- Controls the execution timing

	_conductor=(conductor_type)Allocate<typename MasterType::conductor_type>();

	_conductor->Initialize<NULLTYPE>();
	
	//---- initialize icon ----

	SetIcon( wxIcon("C:\\opt\\polarisdeps\\antares\\POLARIS_Icon.ico",wxBITMAP_TYPE_ICO) );

	//---- initialize aui ---- - Part of the wxFrame object

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
	_load_view = _file_menu->Append(0,"Load View");
	_save_view = _file_menu->Append(1,"Save View");

	Connect(_load_view->GetId(),wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(Antares_Implementation::OnLoadView));
	Connect(_save_view->GetId(),wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(Antares_Implementation::OnSaveView));


	_edit_menu = new wxMenu();
	_menu_bar->Append(_edit_menu,"Edit");

	_view_menu = new wxMenu();
	_menu_bar->Append(_view_menu,"View");
	_hide_plot = _view_menu->Append(2,"Hide Plot");
	_show_plot = _view_menu->Append(3,"Show Plot");

	Connect(_hide_plot->GetId(),wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(Antares_Implementation::OnHidePlot));
	Connect(_show_plot->GetId(),wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(Antares_Implementation::OnShowPlot));

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

	int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 32, 0}; // Arguments passed to GL canvas (color type, double buffer property, depth size, some other stuff...)

	_canvas=(canvas_type)new type_of(canvas)(this,args);

	_aui_manager.AddPane((type_of(canvas)*)_canvas,wxAuiPaneInfo().Name("Canvas").CenterPane().CloseButton(false));

	_control_panel=(control_panel_type)new type_of(control_panel)(this);
	
	_aui_manager.AddPane((type_of(control_panel)*)_control_panel,wxAuiPaneInfo().Name("Control_Panel").Left().MinSize(250,100).CloseButton(false).Position(0));
	


	_time_panel=(time_panel_type)new type_of(time_panel)(this);

	_aui_manager.AddPane((type_of(time_panel)*)_time_panel,wxAuiPaneInfo().Name("Time_Panel").Bottom().MinSize(250,100).CloseButton(false).Position(0));
	
	_information_panel=(information_panel_type)new type_of(information_panel)(this);

	//1920-250
	_aui_manager.AddPane((type_of(information_panel)*)_information_panel,wxAuiPaneInfo().Name("Information_Panel").Bottom().MinSize(1920-250,300).CloseButton(true).Position(1));
	

	_attributes_panel=(attributes_panel_type)new type_of(attributes_panel)(this);
	
	_aui_manager.AddPane((type_of(attributes_panel)*)_attributes_panel,wxAuiPaneInfo().Name("Attributes_Panel").Right().MinSize(250,100).CloseButton(false));

	_aui_manager.Update();
	
	//---- connect conductor to other panels ----

	_conductor->canvas<canvas_type>(_canvas);
	_conductor->information_panel<information_panel_type>(_information_panel);
	_conductor->time_panel<time_panel_type>(_time_panel);

	//---- connect canvas to other panels ----

	_canvas->time_panel<time_panel_type>(_time_panel);
	_canvas->information_panel<information_panel_type>(_information_panel);
	_canvas->attributes_panel<attributes_panel_type>(_attributes_panel);
	_canvas->control_panel<control_panel_type>(_control_panel);
	_canvas->layer_options<layer_options_type>( _control_panel->layer_options<layer_options_type>() );

	//---- connect control_panel to other panels ----

	_control_panel->canvas<canvas_type>(_canvas);
	_control_panel->layer_options<layer_options_type>()->canvas<canvas_type>(_canvas);

	//---- connect time_panel to canvas ----

	_time_panel->canvas<canvas_type>(_canvas);
}

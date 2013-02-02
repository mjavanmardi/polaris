//*********************************************************
//	Antares_Frame_Implementation.h - Parent Frame
//*********************************************************

#pragma once
#include "Antares_Frame.h"
#include "Control_Panel_Implementation.h"
#include "Time_Panel_Implementation.h"
#include "Information_Panel_Implementation.h"
#include "Canvas_Implementation.h"

//---------------------------------------------------------
//	Antares_Implementation - aui initialization
//---------------------------------------------------------

template<typename MasterType, typename ParentType>
class Antares_Implementation : public wxFrame
{
public:
	Antares_Implementation(wxFrame* parent,void* _simulation_ptr,string& _db_name);

	virtual ~Antares_Implementation(void){mgr.UnInit();}

	Control_Panel_Implementation<MasterType,Antares_Implementation>* control_panel;
	Time_Panel_Implementation<MasterType,Antares_Implementation>* time_panel;
	Information_Panel_Implementation<MasterType,Antares_Implementation>* information_panel;
	Canvas_Implementation<MasterType,Antares_Implementation>* canvas;
	
	wxAuiManager mgr;
	
	string db_name;
	void* simulation_ptr;
};

template<typename MasterType,typename ParentType>
Antares_Implementation<MasterType,ParentType>::Antares_Implementation(wxFrame* parent,void* _simulation_ptr,string& _db_name):wxFrame(parent,-1,"Antares")
{
	simulation_ptr=_simulation_ptr;
	db_name=_db_name;

	//---- initialize aui ----

	mgr.SetManagedWindow(this);

	mgr.GetArtProvider()->SetMetric(wxAUI_DOCKART_GRADIENT_TYPE,2);
	mgr.GetArtProvider()->SetColor(wxAUI_DOCKART_BORDER_COLOUR,wxColor(255,255,255));
	mgr.GetArtProvider()->SetColor(wxAUI_DOCKART_SASH_COLOUR,wxColor(255,255,255));
	mgr.GetArtProvider()->SetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE,0);
	mgr.GetArtProvider()->SetMetric(wxAUI_DOCKART_SASH_SIZE,5);
	mgr.GetArtProvider()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR,wxColor(50,150,255));
	mgr.GetArtProvider()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR,wxColor(255,255,255));

	//---- initialize and add the primary widgets ----

	int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 32, 0};

	canvas=new Canvas_Implementation<MasterType,Antares_Implementation>(this,args);

	mgr.AddPane(canvas,wxAuiPaneInfo().Name("Canvas").CenterPane());

	control_panel=new Control_Panel_Implementation<MasterType,Antares_Implementation>(this);
	
	mgr.AddPane(control_panel,wxAuiPaneInfo().Name("Control_Panel").Left());
	
	time_panel=new Time_Panel_Implementation<MasterType,Antares_Implementation>(this);
	
	mgr.AddPane(time_panel,wxAuiPaneInfo().Name("Time_Panel").Bottom());

	information_panel=new Information_Panel_Implementation<MasterType,Antares_Implementation>(this);
	
	mgr.AddPane(information_panel,wxAuiPaneInfo().Name("Information_Panel").Bottom().MinSize(800,200));

	mgr.Update();
}
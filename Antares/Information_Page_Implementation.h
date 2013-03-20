//*********************************************************
//	Information_Page_Implementation.h - Plotting Canvas
//*********************************************************

#pragma once
#include "Information_Page.h"

//---------------------------------------------------------
//	Information_Page_Implementation - information page class definition
//---------------------------------------------------------

implementation class Information_Page_Implementation : public Polaris_Component<APPEND_CHILD(Information_Page_Implementation),MasterType,NULLTYPE>,public wxPanel
{
public:
	Information_Page_Implementation(wxWindow* parent);
	virtual ~Information_Page_Implementation(void){};
	
	feature_implementation void Draw_Layer(const int start_iteration, const int end_iteration);

	member_pointer(wxPLplotwindow,plotwindow,none,none);
	member_pointer(wxBoxSizer,sizer,none,none);

	member_data(int,cached_iteration,none,none);
	member_prototype(Antares_Layer,layer,typename MasterType::type_of(antares_layer),none,none);
};

//---------------------------------------------------------
//	Information_Page_Implementation - information page initialization
//---------------------------------------------------------

//wxPanel(parent,-1,wxDefaultPosition,wxDefaultSize,wxCLIP_CHILDREN )
template<typename MasterType,typename ParentType,typename InheritanceList>
Information_Page_Implementation<MasterType,ParentType,InheritanceList>::Information_Page_Implementation(wxWindow* parent) : wxPanel(parent, -1)
{
	//---- miscellaneous initialization ----

	_sizer = new wxBoxSizer( wxVERTICAL );
	//wxPLPLOT_BACKEND_GC
	_plotwindow = new wxPLplotwindow( this, -1, wxDefaultPosition, wxSize(1920,1080), wxWANTS_CHARS, wxPLPLOT_DRAW_TEXT );
	_plotwindow->SetMaxSize( wxSize(1920,1080) );

	_sizer->Add( _plotwindow, 1, wxEXPAND );
	
	SetSizer(_sizer);

	wxPLplotstream* pls = _plotwindow->GetStream();

	pls->adv( 0 );
	pls->scol0( 0, 255, 255, 255 );
	pls->scol0( 15, 0, 0, 0 );

	_layer=nullptr;
}

//template<typename MasterType,typename ParentType,typename InheritanceList>
//void Information_Panel_Implementation<MasterType,ParentType,InheritanceList>::OnResize(wxSizeEvent& event)
//{
//	_box->SetDimension(wxPoint(0,0),GetSize());
//}

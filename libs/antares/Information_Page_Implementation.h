//*********************************************************
//	Information_Page_Implementation.h - Plotting Canvas
//*********************************************************

#pragma once
#include "Information_Page.h"

//---------------------------------------------------------
//	Information_Page_Implementation - information page class definition
//---------------------------------------------------------

implementation class Information_Page_Implementation : public Polaris_Component<MasterType,INHERIT(Information_Page_Implementation),NULLTYPE>,public wxPanel
{
public:
	Information_Page_Implementation(wxWindow* parent);
	virtual ~Information_Page_Implementation(void){};
	
	template<typename ComponentType,typename TargetType> void Draw_Layer(const int start_iteration, const int end_iteration);

	m_data(wxPLplotwindow*,plotwindow,NONE,NONE);
	m_data(wxBoxSizer*,sizer,NONE,NONE);

	m_data(int,cached_iteration,NONE,NONE);
	m_prototype(Antares_Layer<typename MasterType::type_of(antares_layer)>,layer,NONE,NONE);
};

//---------------------------------------------------------
//	Information_Page_Implementation - information page initialization
//---------------------------------------------------------

//wxPanel(parent,-1,wxDefaultPosition,wxDefaultSize,wxCLIP_CHILDREN )
template<typename MasterType,typename InheritanceList>
Information_Page_Implementation<MasterType,InheritanceList>::Information_Page_Implementation(wxWindow* parent) : wxPanel(parent, -1)
{
	//---- miscellaneous initialization ----

	_sizer = new wxBoxSizer( wxVERTICAL );
	//wxPLPLOT_BACKEND_GC
	_plotwindow = new wxPLplotwindow( this, -1, wxDefaultPosition, wxSize(1920,1080), wxWANTS_CHARS, wxPLPLOT_DRAW_TEXT|wxPLPLOT_BACKEND_GC );
	_plotwindow->SetMaxSize( wxSize(1920,1080) );

	_sizer->Add( _plotwindow, 1, wxEXPAND );
	
	SetSizer(_sizer);

	wxPLplotstream* pls = _plotwindow->GetStream();

	pls->adv( 0 );
	// background color
	pls->scol0( 0, 255, 255, 255 ); // white
	// environment color
	pls->scol0( 1, 0, 0, 0 ); // black

	_layer=nullptr;
}

//template<typename MasterType,typename InheritanceList>
//void Information_Panel_Implementation<MasterType,InheritanceList>::OnResize(wxSizeEvent& event)
//{
//	_box->SetDimension(wxPoint(0,0),GetSize());
//}

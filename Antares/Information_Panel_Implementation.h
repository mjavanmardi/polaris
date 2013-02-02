//*********************************************************
//	Information_Panel_Implementation.h - Information and Plotting
//*********************************************************

#pragma once
#include "Information_Panel.h"

//---------------------------------------------------------
//	Information_Panel_Implementation - information panel class definition
//---------------------------------------------------------

class MyPlotwindow : public wxPLplotwindow
{
public:
    MyPlotwindow( wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize, long style = 0,
                  int pl_style = wxPLPLOT_NONE ) :
	wxPLplotwindow( parent, id, pos, size, style, pl_style ){};
};

implementation class Information_Panel_Implementation : public wxPanel
{
public:
	Information_Panel_Implementation(wxFrame* parent);
	virtual ~Information_Panel_Implementation(void){};

	void Plot();
	void OnResize(wxSizeEvent& event);

	MyPlotwindow* plotwindow;
    bool bgcolor;

	//Canvas_Implementation* canvas_ptr;
};

//---------------------------------------------------------
//	Information_Panel - information initialization
//---------------------------------------------------------

template<typename MasterType,typename ParentType>
Information_Panel_Implementation<MasterType,ParentType>::Information_Panel_Implementation(wxFrame* parent) : wxPanel(parent,-1,wxDefaultPosition,wxDefaultSize,wxCLIP_CHILDREN )
{
	//---- miscellaneous initialization ----

	//canvas_ptr=((Antares_Implementation *) GetParent())->canvas;

	bgcolor = true;

    //wxPanel* panel = new wxPanel( this ,-1,wxDefaultPosition,wxDefaultSize);
    wxBoxSizer* box = new wxBoxSizer( wxVERTICAL );
//#define wxUSE_GRAPHICS_CONTEXT 1

    plotwindow = new MyPlotwindow( this, -1, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS,
#if wxUSE_GRAPHICS_CONTEXT
        wxPLPLOT_BACKEND_GC | wxPLPLOT_DRAW_TEXT );
#else
        wxPLPLOT_BACKEND_AGG | wxPLPLOT_DRAW_TEXT );
#endif

    box->Add( plotwindow, 1, wxALL | wxEXPAND, 0 );
    //panel->SetSizer( box );

	SetSizer(box);
	SetSize( 800, 200 );      // set frame size
    //SetSizeHints( 220, 150 ); // set minimum frame size

	Connect(wxEVT_SIZE,wxSizeEventHandler(Information_Panel_Implementation::OnResize));
}

template<typename MasterType,typename ParentType>
void Information_Panel_Implementation<MasterType,ParentType>::OnResize(wxSizeEvent& event)
{
	Plot();
}

template<typename MasterType,typename ParentType>
void Information_Panel_Implementation<MasterType,ParentType>::Plot()
{
    const size_t np = 500;
    PLFLT x[np], y[np];
    PLFLT xmin, xmax;
    PLFLT ymin = 1e30, ymax = 1e-30;

    xmin = -2.0;
    xmax = 10.0;
    for ( size_t i = 0; i < np; i++ )
    {
        x[i] = ( xmax - xmin ) * i / np + xmin;
        y[i] = 1.0;
        if ( x[i] != 0.0 ) y[i] = sin( x[i] ) / x[i];

		ymin = ( ( ymin ) < ( y[i] ) ? ( ymin ) : ( y[i] ) );
		ymax = ( ( ymax ) < ( y[i] ) ? ( y[i] ) : ( ymax ) );
    }
    
	wxPLplotstream* pls = plotwindow->GetStream();

    pls->adv( 0 );

    pls->scol0( 0, 255, 255, 255 );
    pls->scol0( 15, 0, 0, 0 );

    pls->col0( 1 );
    pls->env( xmin, xmax, ymin, ymax, 0, 0 );
    pls->col0( 2 );
    pls->lab( "x", "y", "sin(x)/x" );

    pls->col0( 3 );
    pls->width( 2 );
    pls->line( np, x, y );

    plotwindow->RenewPlot();
}
//*********************************************************
//	Splash_Panel_Implementation.h - Plotting Canvas
//*********************************************************

#pragma once
#include "Splash_Panel.h"

//---------------------------------------------------------
//	Splash_Panel_Implementation - information page class definition
//---------------------------------------------------------

implementation class Splash_Panel_Implementation : public Polaris_Component<MasterType,INHERIT(Splash_Panel_Implementation),NULLTYPE>,public wxPanel
{
public:
	Splash_Panel_Implementation(wxWindow* parent) : wxPanel(parent,-1,wxDefaultPosition,wxSize(300,200))
	{
		Connect(wxEVT_PAINT,wxPaintEventHandler(Splash_Panel_Implementation::Render));
		
		wxInitAllImageHandlers();

		SetBackgroundColour(wxColour(0,0,0));

		_image.LoadFile("C:\\opt\\polarisdeps\\antares\\Logo.png",wxBITMAP_TYPE_PNG);
	}

	virtual ~Splash_Panel_Implementation(void){};

	void Render(wxPaintEvent& event)
	{
		wxPaintDC dc(this);
		dc.DrawBitmap(_image, 30, 0, false);
	}

	m_data(wxBitmap,image, NONE, NONE);
};

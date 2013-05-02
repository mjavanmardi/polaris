//*********************************************************
//	Splash_Panel_Implementation.h - Plotting Canvas
//*********************************************************

#pragma once
#include "Splash_Panel.h"

//---------------------------------------------------------
//	Splash_Panel_Implementation - information page class definition
//---------------------------------------------------------

implementation class Splash_Panel_Implementation : public Polaris_Component<APPEND_CHILD(Splash_Panel_Implementation),MasterType,NULLTYPE>,public wxPanel
{
public:
	Splash_Panel_Implementation(wxWindow* parent) : wxPanel(parent,-1,wxDefaultPosition,wxSize(300,200))
	{
		Connect(wxEVT_PAINT,wxPaintEventHandler(Splash_Panel_Implementation::Render));
		
		wxInitAllImageHandlers();

		SetBackgroundColour(wxColour(0,0,0));

		_image.LoadFile("Logo.png",wxBITMAP_TYPE_PNG);
	}

	virtual ~Splash_Panel_Implementation(void){};

	void Render(wxPaintEvent& event)
	{
		wxPaintDC dc(this);
		dc.DrawBitmap(_image, 30, 0, false);
	}

	member_data(wxBitmap,image,none,none);
};
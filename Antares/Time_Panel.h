//*********************************************************
//	Time_Panel.h - Time Navigation Control
//*********************************************************

#pragma once
#include "Antares_Includes.h"

//---------------------------------------------------------
//	Time_Panel - time panel class definition
//---------------------------------------------------------

class Time_Panel : public wxPanel
{
public:
	Time_Panel(wxFrame* parent);
	virtual ~Time_Panel(void){};
	
	void Update_Time(int updated_time);
	void OnPlay(wxCommandEvent& event);
    void OnStop(wxCommandEvent& event);

	wxBitmapButton* play;
	wxBitmap play_button;
	wxBitmap pause_button;
	
	wxBoxSizer* sizer;

	wxTextCtrl* time_display;

	//Canvas* canvas_ptr;
};
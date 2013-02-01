//*********************************************************
//	Time_Panel_Implementation.h - Time Navigation Control
//*********************************************************

#pragma once
#include "Time_Panel.h"

//---------------------------------------------------------
//	Time_Panel - time panel class definition
//---------------------------------------------------------

implementation class Time_Panel_Implementation : public wxPanel
{
public:
	Time_Panel_Implementation(wxFrame* parent);
	virtual ~Time_Panel_Implementation(void){};
	
	void Update_Time(int updated_time);
	void OnPlay(wxCommandEvent& event);
    void OnStop(wxCommandEvent& event);

	wxBitmapButton* play;
	wxBitmap play_button;
	wxBitmap pause_button;
	
	wxBoxSizer* sizer;

	wxTextCtrl* time_display;

	//Canvas_Implementation* canvas_ptr;
};

//---------------------------------------------------------
//	Time_Panel - time_panel initialization
//---------------------------------------------------------

template<typename MasterType,typename ParentType>
Time_Panel_Implementation<MasterType,ParentType>::Time_Panel_Implementation(wxFrame* parent) : wxPanel(parent,-1,wxDefaultPosition,wxDefaultSize,wxCLIP_CHILDREN )
{
	//---- miscellaneous initialization ----

	//canvas_ptr=((Antares_Implementation *) GetParent())->canvas;

	//---- initialize the sizers ----
	
	sizer=new wxBoxSizer(wxHORIZONTAL);

	//---- initialize and add the components ----
	
	play_button=wxBitmap("Play.png",wxBITMAP_TYPE_PNG);
	pause_button=wxBitmap("Pause.png",wxBITMAP_TYPE_PNG);

	play=new wxBitmapButton(this,wxID_ANY,play_button,wxDefaultPosition,wxSize(62,50));
	Connect(play->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(Time_Panel_Implementation::OnPlay));
	play->SetToolTip("Play");
	sizer->Add(play);

	time_display=new wxTextCtrl(this,wxID_ANY,"00:00:00",wxDefaultPosition,wxSize(100,-1),wxTE_READONLY);

	time_display->SetFont(wxFont(14,wxSWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD));
	time_display->SetBackgroundStyle(wxBG_STYLE_COLOUR);
	time_display->SetBackgroundColour(wxColour(152.5,202.5,255,0));
	time_display->Disable();

	sizer->Add(time_display);

	//---- set the sizer ----
	
	SetSizerAndFit(sizer);
}

//---------------------------------------------------------
//	OnPlay - user presses the play button
//---------------------------------------------------------

template<typename MasterType,typename ParentType>
void Time_Panel_Implementation<MasterType,ParentType>::OnPlay(wxCommandEvent& event)
{
	play->SetBitmapLabel(pause_button);
	Refresh();
	Connect(play->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(Time_Panel_Implementation::OnStop));
}

//---------------------------------------------------------
//	OnStop - user presses the stop button
//---------------------------------------------------------

template<typename MasterType,typename ParentType>
void Time_Panel_Implementation<MasterType,ParentType>::OnStop(wxCommandEvent& event)
{
	play->SetBitmapLabel(play_button);
	Refresh();
	Connect(play->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(Time_Panel_Implementation::OnPlay));
}

//---------------------------------------------------------
//	Update_Time - convert seconds/base_step_length to a 24:00 clock display
//---------------------------------------------------------

template<typename MasterType,typename ParentType>
void Time_Panel_Implementation<MasterType,ParentType>::Update_Time(int updated_time)
{
	//---- extract hours, minutes, and seconds from time value given ----

	//int hours=updated_time/(3600*((Canvas *)canvas_ptr)->base_step_factor);
	//int minutes=(updated_time-hours*(3600*((Canvas *)canvas_ptr)->base_step_factor))/(60*((Canvas *)canvas_ptr)->base_step_factor);
	//int seconds=(updated_time-hours*3600*((Canvas *)canvas_ptr)->base_step_factor-minutes*60*((Canvas *)canvas_ptr)->base_step_factor)/((Canvas *)canvas_ptr)->base_step_factor;

	int hours=0;
	int minutes=0;
	int seconds=0;

	//---- assemble time string and input ----

	stringstream s;
	if(hours<10) s << "0";
	s << hours;
	s << ":";
	if(minutes<10) s << "0";
	s << minutes;
	s << ":";
	if(seconds<10) s << "0";
	s << seconds;
	
	time_display->SetValue(s.str());

	time_display->Refresh();
}
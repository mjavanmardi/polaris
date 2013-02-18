//*********************************************************
//	Time_Panel_Implementation.h - Time Navigation Control
//*********************************************************

#pragma once
#include "Time_Panel.h"

//---------------------------------------------------------
//	Time_Panel - time panel class definition
//---------------------------------------------------------

implementation class Time_Panel_Implementation : public Polaris_Component<APPEND_CHILD(Time_Panel_Implementation),MasterType,NULLTYPE>, public wxPanel
{
public:
	Time_Panel_Implementation(wxFrame* parent);
	virtual ~Time_Panel_Implementation(void){};
	
	void Update_Time(int updated_time);
	void OnPlay(wxCommandEvent& event);
    void OnStop(wxCommandEvent& event);

	member_pointer(wxBitmapToggleButton,play,none,none);
	member_data(wxBitmap,play_button,none,none);
	member_data(wxBitmap,pause_button,none,none);

	member_pointer(wxBoxSizer,sizer,none,none);
	member_pointer(wxTextCtrl,time_display,none,none);

	member_prototype(Conductor,conductor,typename MasterType::conductor_type,none,none);
	define_component_interface(Conductor_Interface, type_of(conductor), Conductor, Time_Panel_Implementation);

	//Canvas_Implementation* canvas_ptr;
};

//---------------------------------------------------------
//	Time_Panel - time_panel initialization
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
Time_Panel_Implementation<MasterType,ParentType,InheritanceList>::Time_Panel_Implementation(wxFrame* parent) : wxPanel(parent,-1,wxDefaultPosition,wxDefaultSize,wxCLIP_CHILDREN )
{
	//---- miscellaneous initialization ----

	_conductor=(Conductor_Interface*)((Antares_Implementation<MasterType>*)GetParent())->_conductor;

	//---- initialize the sizers ----
	
	_sizer=new wxBoxSizer(wxHORIZONTAL);

	//---- initialize and add the components ----
	
	_play_button=wxBitmap("Play.png",wxBITMAP_TYPE_PNG);
	_pause_button=wxBitmap("Pause.png",wxBITMAP_TYPE_PNG);

	_play=new wxBitmapToggleButton(this,wxID_ANY,_play_button,wxDefaultPosition,wxSize(62,50));
	Connect(_play->GetId(),wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,wxCommandEventHandler(Time_Panel_Implementation::OnPlay));
	_play->SetToolTip("Play");
	_sizer->Add(_play);

	_time_display=new wxTextCtrl(this,wxID_ANY,"00:00:00",wxDefaultPosition,wxSize(100,-1),wxTE_READONLY);

	_time_display->SetFont(wxFont(14,wxSWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD));
	_time_display->SetBackgroundStyle(wxBG_STYLE_COLOUR);
	_time_display->SetBackgroundColour(wxColour(152.5,202.5,255,0));
	_time_display->Disable();

	_sizer->Add(_time_display);

	//---- set the sizer ----
	
	SetSizerAndFit(_sizer);
}

//---------------------------------------------------------
//	OnPlay - user presses the play button
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
void Time_Panel_Implementation<MasterType,ParentType,InheritanceList>::OnPlay(wxCommandEvent& event)
{
	_play->SetBitmapLabel(_pause_button);
	_play->SetValue(false);
	Refresh();
	Connect(_play->GetId(),wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,wxCommandEventHandler(Time_Panel_Implementation::OnStop));
	_conductor->pause<bool>(false);
}

//---------------------------------------------------------
//	OnStop - user presses the stop button
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
void Time_Panel_Implementation<MasterType,ParentType,InheritanceList>::OnStop(wxCommandEvent& event)
{
	_play->SetBitmapLabel(_play_button);
	_play->SetValue(true);
	Refresh();
	Connect(_play->GetId(),wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,wxCommandEventHandler(Time_Panel_Implementation::OnPlay));
	_conductor->pause<bool>(true);
}

//---------------------------------------------------------
//	Update_Time - convert seconds/base_step_length to a 24:00 clock display
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
void Time_Panel_Implementation<MasterType,ParentType,InheritanceList>::Update_Time(int updated_time)
{
	//---- extract hours, minutes, and seconds from time value given ----

	//int hours=updated_time/(3600*((Canvas *)canvas_ptr)->base_step_factor);
	//int minutes=(updated_time-hours*(3600*((Canvas *)canvas_ptr)->base_step_factor))/(60*((Canvas *)canvas_ptr)->base_step_factor);
	//int seconds=(updated_time-hours*3600*((Canvas *)canvas_ptr)->base_step_factor-minutes*60*((Canvas *)canvas_ptr)->base_step_factor)/((Canvas *)canvas_ptr)->base_step_factor;

	int hours=updated_time/3600;
	int minutes=(updated_time-hours*3600)/60;
	int seconds=updated_time-hours*3600-minutes*60;

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
	
	_time_display->SetValue(s.str());

	_time_display->Refresh();
}
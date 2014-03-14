//*********************************************************
//	Time_Panel_Implementation.h - Time Navigation Control
//*********************************************************

#pragma once
#include "Time_Panel.h"

//---------------------------------------------------------
//	Time_Panel - time panel class definition
//---------------------------------------------------------

implementation class Time_Panel_Implementation : public Polaris_Component<MasterType,INHERIT(Time_Panel_Implementation),NULLTYPE>, public wxPanel
{
public:
	Time_Panel_Implementation(wxFrame* parent);
	virtual ~Time_Panel_Implementation(void){};
	
	template<typename TargetType> bool Is_Running()
	{
		return _play->GetValue();
	}

	void Update_Time(int updated_time);
	void OnPlay(wxCommandEvent& event);
    void OnStop(wxCommandEvent& event);
	void OnSpeed(wxCommandEvent& event);
	
	void OnRecord(wxCommandEvent& event);
	void OnStopRecord(wxCommandEvent& event);

	m_data(wxBitmapToggleButton*,play, NONE, NONE);
	m_data(wxBitmap,play_button, NONE, NONE);
	m_data(wxBitmap,pause_button, NONE, NONE);
	m_data(wxBitmap,record_button, NONE, NONE);
	m_data(wxBitmap,stop_button, NONE, NONE);

	m_data(wxBitmapToggleButton*,record, NONE, NONE);

	m_data(wxChoice*,speed_control, NONE, NONE);

	m_data(wxBoxSizer*,sizer, NONE, NONE);
	m_data(wxBoxSizer*,vertical_sizer, NONE, NONE);
	m_data(wxBoxSizer*,record_sizer, NONE, NONE);
	m_data(wxTextCtrl*,time_display, NONE, NONE);

	m_prototype(Conductor,typename MasterType::conductor_type,conductor, NONE, NONE);

	m_prototype(Canvas,typename MasterType::canvas_type,canvas, NONE, NONE);

	m_data(int,delay, NONE, NONE);
};

//---------------------------------------------------------
//	Time_Panel - time_panel initialization
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
Time_Panel_Implementation<MasterType,InheritanceList>::Time_Panel_Implementation(wxFrame* parent) : wxPanel(parent,-1,wxDefaultPosition,wxDefaultSize,wxCLIP_CHILDREN )
{


	//---- miscellaneous initialization ----

	_conductor=(conductor_type)((Antares_Implementation<MasterType>*)GetParent())->_conductor;

	//---- initialize the sizers ----
	
	_sizer=new wxBoxSizer(wxHORIZONTAL);
	_vertical_sizer=new wxBoxSizer(wxVERTICAL);
	//_record_sizer=new wxBoxSizer(wxVERTICAL);

	//---- initialize and add the components ----
	

	_record_button=wxBitmap("C:\\opt\\polarisdeps\\antares\\Record.png",wxBITMAP_TYPE_PNG);
	
	_stop_button=wxBitmap("C:\\opt\\polarisdeps\\antares\\Stop.png",wxBITMAP_TYPE_PNG);

	_record=new wxBitmapToggleButton(this,wxID_ANY,_record_button,wxDefaultPosition,wxSize(62,52));
	Connect(_record->GetId(),wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,wxCommandEventHandler(Time_Panel_Implementation::OnRecord));
	_record->SetToolTip("Record");

	_sizer->Add(_record);



	_time_display=new wxTextCtrl(this,wxID_ANY,"00:00:00",wxDefaultPosition,wxSize(106,30),wxTE_READONLY);

	_time_display->SetFont(wxFont(14,wxSWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD));
	_time_display->SetBackgroundStyle(wxBG_STYLE_COLOUR);
	_time_display->SetBackgroundColour(wxColour(152.5,202.5,255,0));
	_time_display->Disable();

	_vertical_sizer->Add(_time_display);




	_delay = -1;

	const unsigned int num_choices = 9;

	wxString choices[num_choices];

	stringstream s;

	s << "MAX";
	choices[0] = s.str();
	s.str("");

	s << "AUTO";
	choices[1] = s.str();
	s.str("");

	s << 100;
	choices[2] = s.str();
	s.str("");

	s << 60;
	choices[3] = s.str();
	s.str("");

	s << 30;
	choices[4] = s.str();
	s.str("");

	s << 20;
	choices[5] = s.str();
	s.str("");
	
	s << 10;
	choices[6] = s.str();
	s.str("");

	s << 5;
	choices[7] = s.str();
	s.str("");

	s << 1;
	choices[8] = s.str();
	s.str("");


	_speed_control = new wxChoice(this,wxID_ANY,wxDefaultPosition,wxSize(106,21),num_choices,choices );
	
	_speed_control->SetSelection(0);

	Connect(_speed_control->GetId(),wxEVT_COMMAND_CHOICE_SELECTED,wxCommandEventHandler(Time_Panel_Implementation::OnSpeed));

	_vertical_sizer->Add(_speed_control);

	_sizer->Add(_vertical_sizer,0,wxLEFT,10);

	//_record_sizer->Add(_sizer);
	


	_play_button=wxBitmap("C:\\opt\\polarisdeps\\antares\\Play.png",wxBITMAP_TYPE_PNG);
	_pause_button=wxBitmap("C:\\opt\\polarisdeps\\antares\\Pause.png",wxBITMAP_TYPE_PNG);

	_play=new wxBitmapToggleButton(this,wxID_ANY,_play_button,wxDefaultPosition,wxSize(62,52));
	Connect(_play->GetId(),wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,wxCommandEventHandler(Time_Panel_Implementation::OnPlay));
	_play->SetToolTip("Play");
	_sizer->Add(_play,0,wxLEFT,10);

	//_record_sizer->Add(_record);

	//---- set the sizer ----
	
	SetSizerAndFit(_sizer);
}

//---------------------------------------------------------
//	OnPlay - user presses the play button
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Time_Panel_Implementation<MasterType,InheritanceList>::OnPlay(wxCommandEvent& event)
{
	_play->SetBitmapLabel(_pause_button);
	_play->SetValue(true);
	Refresh();
	Connect(_play->GetId(),wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,wxCommandEventHandler(Time_Panel_Implementation::OnStop));
	_conductor->pause<bool>(false);
}

//---------------------------------------------------------
//	OnStop - user presses the stop button
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Time_Panel_Implementation<MasterType,InheritanceList>::OnStop(wxCommandEvent& event)
{
	_play->SetBitmapLabel(_play_button);
	_play->SetValue(false);
	Refresh();
	Connect(_play->GetId(),wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,wxCommandEventHandler(Time_Panel_Implementation::OnPlay));
	_conductor->pause<bool>(true);
}

//---------------------------------------------------------
//	OnRecord - user presses the record button
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Time_Panel_Implementation<MasterType,InheritanceList>::OnRecord(wxCommandEvent& event)
{
	if(!Is_Running<NT>())
	{
		_speed_control->SetSelection(6);
		_delay = (int)(1.0f/15.0f * 1000.0f + .5f);
		_record->SetBitmapLabel(_stop_button);
		_record->SetValue(true);
		Refresh();
		Connect(_record->GetId(),wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,wxCommandEventHandler(Time_Panel_Implementation::OnStopRecord));
		_canvas->Start_Recording<NT>();
	}
	else
	{
		_record->SetValue(false);
	}
}

//---------------------------------------------------------
//	OnStopRecord - user presses the stop button
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Time_Panel_Implementation<MasterType,InheritanceList>::OnStopRecord(wxCommandEvent& event)
{
	_canvas->Stop_Recording<NT>();
	_record->SetBitmapLabel(_record_button);
	_record->SetValue(false);
	Refresh();
	Connect(_record->GetId(),wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,wxCommandEventHandler(Time_Panel_Implementation::OnRecord));
}

//---------------------------------------------------------
//	OnSpeed - user uses the speed control
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Time_Panel_Implementation<MasterType,InheritanceList>::OnSpeed(wxCommandEvent& event)
{
	if(event.GetString() == "MAX")
	{
		_delay = -1;
	}
	else if(event.GetString() == "AUTO")
	{
		_delay = 0;
	}
	else
	{
		float fps = atof(event.GetString().c_str());
		_delay = (int)(1.0f/fps * 1000.0f + .5f);
	}
}

//---------------------------------------------------------
//	Update_Time - convert seconds/base_step_length to a 24:00 clock display
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Time_Panel_Implementation<MasterType,InheritanceList>::Update_Time(int updated_time)
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

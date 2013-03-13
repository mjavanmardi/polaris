//*********************************************************
//	Control_Dialog_Implementation.cpp - Dialog used for Model Control
//*********************************************************

#pragma once
#include "Control_Dialog.h"

//---------------------------------------------------------
//	Control_Dialog_Implementation - control dialog class definition
//---------------------------------------------------------

implementation class Control_Dialog_Implementation : public Polaris_Component<APPEND_CHILD(Control_Dialog_Implementation),MasterType,NULLTYPE>,public wxDialog
{
public:

	Control_Dialog_Implementation(string& name);
	virtual ~Control_Dialog_Implementation(void){};

	feature_implementation void Push_Schema(string& schema);
	feature_implementation void Push_Attributes(vector<string>& attributes);

	void OnApply(wxCommandEvent& event);
	void OnOk(wxCommandEvent& event);

	member_pointer(wxListCtrl,attributes_list,none,none);
	
	member_pointer(wxButton,ok_button,none,none);
	member_pointer(wxButton,apply_button,none,none);
	//member_pointer(wxButton,cancel_button,none,none);

	member_pointer(wxBoxSizer,sizer,none,none);
	member_pointer(wxBoxSizer,button_sizer,none,none);

	member_pointer(void,selected_object,none,check_2(CallerType,typename MasterType::type_of(antares_layer),is_same));
	member_data(attributes_callback_type,submission_callback,none,check_2(CallerType,typename MasterType::type_of(antares_layer),is_same));
};

//---------------------------------------------------------
//	Control_Dialog_Implementation - control_dialog initialization
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
Control_Dialog_Implementation<MasterType,ParentType,InheritanceList>::Control_Dialog_Implementation(string& name) : wxDialog(NULL,-1,"",wxDefaultPosition,wxSize(350,550),wxRESIZE_BORDER|wxCAPTION)
{
	//|wxSYSTEM_MENU|wxMAXIMIZE_BOX|wxCLOSE_BOX|wxMINIMIZE_BOX

	//---- initialize the sizers ----
	
	_sizer=new wxBoxSizer(wxVERTICAL);
	_button_sizer=new wxBoxSizer(wxHORIZONTAL);

	//---- initialize and add the components ----

	_attributes_list=new wxListCtrl(this,wxID_ANY,wxDefaultPosition,wxSize(500,400),wxLC_REPORT|wxLC_HRULES|wxLC_VRULES|wxLC_EDIT_LABELS);
	
	wxListItem columns[2];

	columns[0].SetId(0);
	columns[0].SetText("Value");
	_attributes_list->InsertColumn(0, columns[0]);

	columns[1].SetId(1);
	columns[1].SetText("Attribute");
	_attributes_list->InsertColumn(1, columns[1]);

	wxListItem atts_rows[20];
	for(int i=0;i<20;i++)
	{
		atts_rows[i].SetId(i);
		_attributes_list->InsertItem(atts_rows[i]);
		_attributes_list->SetItem(i,0,"");
		_attributes_list->SetItem(i,1,"");
	}
	
	_sizer->Add(_attributes_list,0,wxTOP,10);

	_ok_button=new wxButton(this,wxID_ANY,"Ok");
	Connect(_ok_button->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(Control_Dialog_Implementation::OnOk));
	_button_sizer->Add(_ok_button,0,wxLEFT,10);

	_apply_button=new wxButton(this,wxID_ANY,"Apply");
	Connect(_apply_button->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(Control_Dialog_Implementation::OnApply));
	_button_sizer->Add(_apply_button,0,wxLEFT,10);

	//_cancel_button=new wxButton(this,wxID_ANY,"Cancel");
	//_button_sizer->Add(_cancel_button,0,wxLEFT,10);

	_sizer->Add(_button_sizer,0,wxTOP,20);

	//---- set the sizer ----

	SetSizer(_sizer);

	_selected_object=nullptr;
	_submission_callback=nullptr;

	string title=name;

	title.append("_Control_Dialog");

	SetTitle(title);

	Show(false);
}

//---------------------------------------------------------
//	Push_Schema
//--------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
template<typename ComponentType,typename CallerType,typename TargetType>
void Control_Dialog_Implementation<MasterType,ParentType,InheritanceList>::Push_Schema(string& schema)
{
	for(int i=0;i<20;i++)
	{
		_attributes_list->SetItem(i,0,"");
		_attributes_list->SetItem(i,1,"");
	}

	const char* schema_itr = schema.c_str();
	const char* const schema_end = schema_itr + schema.size();

	int atts_row_counter = 0;
	string new_token("");

	while( schema_itr != schema_end )
	{
		if((*schema_itr) == ',')
		{
			_attributes_list->SetItem(atts_row_counter,1,new_token.c_str());
			new_token.clear();
			++atts_row_counter;
		}
		else
		{
			new_token.push_back((*schema_itr));
		}

		++schema_itr;
	}

	_attributes_list->SetItem(atts_row_counter,1,new_token.c_str());
	
	_attributes_list->SetColumnWidth(0,wxLIST_AUTOSIZE);
	_attributes_list->SetColumnWidth(1,wxLIST_AUTOSIZE);

	Refresh();
}

//---------------------------------------------------------
//	Push_Attributes
//--------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
template<typename ComponentType,typename CallerType,typename TargetType>
void Control_Dialog_Implementation<MasterType,ParentType,InheritanceList>::Push_Attributes(vector<string>& attributes)
{
	for(int i=0;i<20;i++)
	{
		_attributes_list->SetItem(i,0,"");
	}

	int atts_row_counter = 0;

	vector<string>::iterator itr;

	for(itr=attributes.begin();itr!=attributes.end();itr++,atts_row_counter++)
	{
		_attributes_list->SetItem(atts_row_counter,0,(*itr).c_str());
	}

	_attributes_list->SetColumnWidth(0,wxLIST_AUTOSIZE);
	_attributes_list->SetColumnWidth(1,wxLIST_AUTOSIZE);

	Refresh();
}

//---------------------------------------------------------
//	OnApply
//--------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
void Control_Dialog_Implementation<MasterType,ParentType,InheritanceList>::OnApply(wxCommandEvent& event)
{
	if(_selected_object!=nullptr && _submission_callback!=nullptr)
	{
		vector<string> new_attributes;
		wxListItem itr;
		string text;

		for(int i=0;i<20;i++)
		{
			itr.SetId(i);
			itr.SetColumn(0);
			itr.SetMask(wxLIST_MASK_TEXT);

			_attributes_list->GetItem(itr);
			text=itr.GetText();

			if(text=="") break;
			else new_attributes.push_back(text);
		}

		if(_submission_callback(_selected_object,new_attributes))
		{
			_apply_button->SetBackgroundColour(wxColour(75,200,75));
		}
		else
		{
			_apply_button->SetBackgroundColour(wxColour(200,75,75));
		}
		
		_attributes_list->SetColumnWidth(0,wxLIST_AUTOSIZE);
		_attributes_list->SetColumnWidth(1,wxLIST_AUTOSIZE);

		Refresh();
	}
}

//---------------------------------------------------------
//	OnOk
//--------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
void Control_Dialog_Implementation<MasterType,ParentType,InheritanceList>::OnOk(wxCommandEvent& event)
{
	_apply_button->SetBackgroundColour(wxNullColour);
	Refresh();

	EndModal(true);
}
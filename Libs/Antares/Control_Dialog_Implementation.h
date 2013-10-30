//*********************************************************
//	Control_Dialog_Implementation.cpp - Dialog used for Model Control
//*********************************************************

#pragma once
#include "Control_Dialog.h"

//---------------------------------------------------------
//	Control_Dialog_Implementation - control dialog class definition
//---------------------------------------------------------

implementation class Control_Dialog_Implementation : public Polaris_Component<MasterType,INHERIT(Control_Dialog_Implementation),NULLTYPE>,public wxDialog
{
public:
	Control_Dialog_Implementation(string&,const boost::container::list<void*>&,boost::container::vector<pair<string,string>>&,boost::container::vector<boost::container::vector<string>>&,submission_callback_type);
	virtual ~Control_Dialog_Implementation(void){};

	//template<typename ComponentType,typename TargetType> void Push_Schema(boost::container::vector<string>& attributes_schema,boost::container::vector<boost::container::vector<string>>& dropdown_schema);
	//template<typename ComponentType,typename TargetType> void Push_Attributes(boost::container::vector<string>& attributes);

	//void OnApply(wxCommandEvent& event);
	void OnOk(wxCommandEvent& event);

	m_data(wxListCtrl*,attributes_list,NONE,NONE);
	
	m_data(wxButton*,ok_button,NONE,NONE);
	//m_data(wxButton*,apply_button,NONE,NONE);
	//m_data(wxButton*,cancel_button,NONE,NONE);

	m_data(wxBoxSizer*,sizer,NONE,NONE);
	m_data(wxBoxSizer*,button_sizer,NONE,NONE);
	m_data(wxBoxSizer*,table_sizer,NONE,NONE);
	m_data(wxBoxSizer*,dropdown_sizer,NONE,NONE);

	m_data(wxChoice**,dropdown_menus,NONE,NONE);

	m_data(boost::container::list<void*>,selected_elements,NONE,NONE);
	m_data(submission_callback_type,submission_callback,NONE,NONE);
	
	m_data(int,num_attributes,NONE,NONE);
	m_data(int,num_dropdowns,NONE,NONE);
};

//---------------------------------------------------------
//	Control_Dialog_Implementation - control_dialog initialization
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
Control_Dialog_Implementation<MasterType,InheritanceList>::Control_Dialog_Implementation(string& layer_name,const boost::container::list<void*>& layer_selected_elements,boost::container::vector<pair<string,string>>& attributes,boost::container::vector<boost::container::vector<string>>& dropdown_schema,submission_callback_type layer_submission_callback) : wxDialog(NULL,-1,"",wxDefaultPosition,wxSize(500,550),wxRESIZE_BORDER|wxCAPTION)
{
	_submission_callback = layer_submission_callback;

	for(boost::container::list<void*>::const_iterator itr=layer_selected_elements.begin();itr!=layer_selected_elements.end();itr++) _selected_elements.push_back( *itr );

	string title=layer_name;

	title.append("_Configuration_Dialog");
	
	SetTitle(title);

	_sizer=new wxBoxSizer(wxVERTICAL);
	_button_sizer=new wxBoxSizer(wxHORIZONTAL);
	_table_sizer=new wxBoxSizer(wxHORIZONTAL);
	
	_num_attributes=attributes.size();

	if(_num_attributes)
	{
		_attributes_list=new wxListCtrl(this,wxID_ANY,wxDefaultPosition,wxSize(250,400),wxLC_REPORT|wxLC_HRULES|wxLC_VRULES|wxLC_EDIT_LABELS);

		wxListItem columns[2];

		columns[0].SetId(0);
		columns[0].SetText("Value");
		_attributes_list->InsertColumn(0, columns[0]);

		columns[1].SetId(1);
		columns[1].SetText("Attribute");
		_attributes_list->InsertColumn(1, columns[1]);

		wxListItem atts_rows[30];

		for(int i=0;i<_num_attributes;i++)
		{
			atts_rows[i].SetId(i);
			_attributes_list->InsertItem(atts_rows[i]);
			_attributes_list->SetItem(i,0,"");
			_attributes_list->SetItem(i,1,"");
		}

		int atts_row_counter = 0;

		for(boost::container::vector<pair<string,string>>::iterator itr=attributes.begin();itr!=attributes.end();itr++,atts_row_counter++)
		{
			if(atts_row_counter == 30)
			{
				cout << "exceeded attribute capacity" << endl;
				exit(0);
			}

			_attributes_list->SetItem(atts_row_counter,0,itr->first);
			_attributes_list->SetItem(atts_row_counter,1,itr->second);
		}
	
		_attributes_list->SetColumnWidth(0,wxLIST_AUTOSIZE);
		_attributes_list->SetColumnWidth(1,wxLIST_AUTOSIZE);

		_table_sizer->Add(_attributes_list,0,wxTOP|wxLEFT,10);
	}
	else
	{
		_attributes_list=nullptr;
	}

	_num_dropdowns = dropdown_schema.size();

	if( _num_dropdowns )
	{
		_dropdown_sizer=new wxBoxSizer(wxVERTICAL);

		wxString choices[20];

		boost::container::vector<boost::container::vector<string>>::iterator vitr;
		boost::container::vector<string>::iterator itr;

		_dropdown_menus = new wxChoice* [ _num_dropdowns ];
		
		int i=0;

		for(vitr=dropdown_schema.begin();vitr!=dropdown_schema.end();vitr++,i++)
		{
			int j=0;

			for(itr=(*vitr).begin();itr!=(*vitr).end();itr++,j++)
			{
				choices[j] = (*itr);
			}

			_dropdown_menus[i]=new wxChoice(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,(*vitr).size(),choices );
			_dropdown_sizer->Add(_dropdown_menus[i],0,wxTOP,10);
		}

		if(_attributes_list == nullptr)
		{
			_table_sizer->Add(_dropdown_sizer,0,wxTOP|wxLEFT,10);
		}
		else
		{
			_table_sizer->Add(_dropdown_sizer,0,wxLEFT,20);
		}
	}
	else
	{
		_dropdown_menus=nullptr;
	}

	_sizer->Add(_table_sizer);

	_ok_button=new wxButton(this,wxID_ANY,"Ok");
	Connect(_ok_button->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(Control_Dialog_Implementation::OnOk));
	_button_sizer->Add(_ok_button,0,wxLEFT,10);

	//_apply_button=new wxButton(this,wxID_ANY,"Apply");
	//Connect(_apply_button->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(Control_Dialog_Implementation::OnApply));
	//_button_sizer->Add(_apply_button,0,wxLEFT,10);

	_sizer->Add(_button_sizer,0,wxTOP,20);

	//---- set the sizer ----

	SetSizer(_sizer);


	Show(true);
}

////---------------------------------------------------------
////	Push_Schema
////--------------------------------------------------------
//
//template<typename MasterType,typename InheritanceList>
//template<typename ComponentType,typename TargetType>
//void Control_Dialog_Implementation<MasterType,InheritanceList>::Push_Schema(boost::container::vector<string>& attributes_schema,boost::container::vector<boost::container::vector<string>>& dropdown_schema)
//{
//	//|wxSYSTEM_MENU|wxMAXIMIZE_BOX|wxCLOSE_BOX|wxMINIMIZE_BOX
//
//	//---- initialize the sizers ----
//	
//	_sizer=new wxBoxSizer(wxVERTICAL);
//	_button_sizer=new wxBoxSizer(wxHORIZONTAL);
//	_table_sizer=new wxBoxSizer(wxHORIZONTAL);
//	_dropdown_sizer=new wxBoxSizer(wxVERTICAL);
//
//	//---- initialize and add the components ----
//	
//	_attributes_list=new wxListCtrl(this,wxID_ANY,wxDefaultPosition,wxSize(250,400),wxLC_REPORT|wxLC_HRULES|wxLC_VRULES|wxLC_EDIT_LABELS);
//	
//	wxListItem columns[2];
//
//	columns[0].SetId(0);
//	columns[0].SetText("Value");
//	_attributes_list->InsertColumn(0, columns[0]);
//
//	columns[1].SetId(1);
//	columns[1].SetText("Attribute");
//	_attributes_list->InsertColumn(1, columns[1]);
//
//	_num_attributes=attributes_schema.size();
//
//	wxListItem atts_rows[20];
//
//
//	for(int i=0;i<_num_attributes;i++)
//	{
//		atts_rows[i].SetId(i);
//		_attributes_list->InsertItem(atts_rows[i]);
//		_attributes_list->SetItem(i,0,"");
//		_attributes_list->SetItem(i,1,"");
//	}
//	
//	_table_sizer->Add(_attributes_list,0,wxTOP,10);
//
//	_num_dropdowns = dropdown_schema.size();
//
//
//	if( _num_dropdowns )
//	{
//		wxString choices[20];
//
//		boost::container::vector<boost::container::vector<string>>::iterator vitr;
//		boost::container::vector<string>::iterator itr;
//
//		_dropdown_menus = new wxChoice* [ _num_dropdowns ];
//		
//		int i=0;
//
//		for(vitr=dropdown_schema.begin();vitr!=dropdown_schema.end();vitr++,i++)
//		{
//			int j=0;
//
//			for(itr=(*vitr).begin();itr!=(*vitr).end();itr++,j++)
//			{
//				choices[j] = (*itr);
//			}
//
//			_dropdown_menus[i]=new wxChoice(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,(*vitr).size(),choices );
//			_dropdown_sizer->Add(_dropdown_menus[i],0,wxTOP,10);
//		}
//
//		_table_sizer->Add(_dropdown_sizer,0,wxLEFT,20);
//	}
//	else
//	{
//		_dropdown_menus=nullptr;
//	}
//
//
//
//
//
//	_sizer->Add(_table_sizer);
//
//
//	_ok_button=new wxButton(this,wxID_ANY,"Ok");
//	Connect(_ok_button->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(Control_Dialog_Implementation::OnOk));
//	_button_sizer->Add(_ok_button,0,wxLEFT,10);
//
//	_apply_button=new wxButton(this,wxID_ANY,"Apply");
//	Connect(_apply_button->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(Control_Dialog_Implementation::OnApply));
//	_button_sizer->Add(_apply_button,0,wxLEFT,10);
//
//	//_cancel_button=new wxButton(this,wxID_ANY,"Cancel");
//	//_button_sizer->Add(_cancel_button,0,wxLEFT,10);
//
//	_sizer->Add(_button_sizer,0,wxTOP,20);
//
//	//---- set the sizer ----
//
//	SetSizer(_sizer);
//
//	_selected_object=nullptr;
//	_submission_callback=nullptr;
//

//
//	for(int i=0;i<_num_attributes;i++)
//	{
//		_attributes_list->SetItem(i,0,"");
//		_attributes_list->SetItem(i,1,"");
//	}
//	
//	int atts_row_counter = 0;
//
//
//	boost::container::vector<string>::iterator itr;
//
//	for(itr=attributes_schema.begin();itr!=attributes_schema.end();itr++,atts_row_counter++)
//	{
//		_attributes_list->SetItem( atts_row_counter,1,(*itr).c_str() );
//	}
//
//	//const char* schema_itr = schema.c_str();
//	//const char* const schema_end = schema_itr + schema.size();
//
//	//string new_token("");
//
//	//while( schema_itr != schema_end )
//	//{
//	//	if((*schema_itr) == ',')
//	//	{
//	//		_attributes_list->SetItem(atts_row_counter,1,new_token.c_str());
//	//		new_token.clear();
//	//		++atts_row_counter;
//	//	}
//	//	else
//	//	{
//	//		new_token.push_back((*schema_itr));
//	//	}
//
//	//	++schema_itr;
//	//}
//
//	//_attributes_list->SetItem(atts_row_counter,1,new_token.c_str());
//
//
//
//	
//	_attributes_list->SetColumnWidth(0,wxLIST_AUTOSIZE);
//	_attributes_list->SetColumnWidth(1,wxLIST_AUTOSIZE);
//
//	Refresh();
//}
//
////---------------------------------------------------------
////	Push_Attributes
////--------------------------------------------------------
//
//template<typename MasterType,typename InheritanceList>
//template<typename ComponentType,typename TargetType>
//void Control_Dialog_Implementation<MasterType,InheritanceList>::Push_Attributes(boost::container::vector<string>& attributes)
//{
//	for(int i=0;i<_num_attributes;i++)
//	{
//		_attributes_list->SetItem(i,0,"");
//	}
//
//	int atts_row_counter = 0;
//
//	boost::container::vector<string>::iterator itr;
//
//	for(itr=attributes.begin();itr!=attributes.end();itr++,atts_row_counter++)
//	{
//		_attributes_list->SetItem(atts_row_counter,0,(*itr).c_str());
//	}
//
//	_attributes_list->SetColumnWidth(0,wxLIST_AUTOSIZE);
//	_attributes_list->SetColumnWidth(1,wxLIST_AUTOSIZE);
//
//	Refresh();
//}

////---------------------------------------------------------
////	OnApply
////--------------------------------------------------------
//
//template<typename MasterType,typename InheritanceList>
//void Control_Dialog_Implementation<MasterType,InheritanceList>::OnApply(wxCommandEvent& event)
//{
//	boost::container::vector<string> attributes_update;
//	string text;
//
//	if(_num_attributes)
//	{
//		wxListItem itr;
//
//		
//		for(int i=0;i<_num_attributes;i++)
//		{
//			itr.SetId(i);
//			itr.SetColumn(0);
//			itr.SetMask(wxLIST_MASK_TEXT);
//
//			_attributes_list->GetItem(itr);
//			text=itr.GetText();
//
//			if(text=="") break;
//			else attributes_update.push_back(text);
//		}
//	}
//	
//	boost::container::vector<string> dropdowns_update;
//
//	if(_num_dropdowns)
//	{
//		for(int i=0;i<_num_dropdowns;i++)
//		{
//			text=_dropdown_menus[i]->GetStringSelection();
//
//			dropdowns_update.push_back(text);
//		}
//	}
//
//	if(_submission_callback(_selected_elements,attributes_update,dropdowns_update))
//	{
//		_apply_button->SetBackgroundColour(wxColour(75,200,75));
//	}
//	else
//	{
//		_apply_button->SetBackgroundColour(wxColour(200,75,75));
//	}
//
//	Refresh();
//}

//---------------------------------------------------------
//	OnOk
//--------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Control_Dialog_Implementation<MasterType,InheritanceList>::OnOk(wxCommandEvent& event)
{
	boost::container::vector<string> attributes_update;
	string text;

	if(_num_attributes)
	{
		wxListItem itr;


		for(int i=0;i<_num_attributes;i++)
		{
			itr.SetId(i);
			itr.SetColumn(0);
			itr.SetMask(wxLIST_MASK_TEXT);

			_attributes_list->GetItem(itr);
			text=itr.GetText();

			if(text=="") break;
			else attributes_update.push_back(text);
		}
	}
		
	boost::container::vector<string> dropdowns_update;

	if(_num_dropdowns)
	{
		for(int i=0;i<_num_dropdowns;i++)
		{
			text=_dropdown_menus[i]->GetStringSelection();

			dropdowns_update.push_back(text);
		}
	}

	if(_submission_callback(_selected_elements,attributes_update,dropdowns_update))
	{
		_ok_button->SetBackgroundColour(wxNullColour);
		EndModal(true);
	}
	else
	{
		_ok_button->SetBackgroundColour(wxColour(200,75,75));
	}

	Refresh();
}
//*********************************************************
//	Attributes_Panel_Implementation.cpp - Container Panel for Attributes
//*********************************************************

#pragma once
#include "Attributes_Panel.h"

//---------------------------------------------------------
//	Attributes_Panel_Implementation - attribute_panel class definition
//---------------------------------------------------------

implementation class Attributes_Panel_Implementation : public Polaris_Component<MasterType,INHERIT(Attributes_Panel_Implementation),NULLTYPE>,public wxPanel
{
public:
	Attributes_Panel_Implementation(wxFrame* parent);
	virtual ~Attributes_Panel_Implementation(void){};

	//feature_implementation void Push_Schema(vector<string>& attributes_schema);
	void Push_Attributes(std::vector<pair<string,string>>& attributes);

	m_data(wxListCtrl*,attributes_list, NONE, NONE);

	m_data(wxBoxSizer*,sizer, NONE, NONE);
};

//---------------------------------------------------------
//	Attributes_Panel_Implementation - attribute_panel initialization
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
Attributes_Panel_Implementation<MasterType,InheritanceList>::Attributes_Panel_Implementation(wxFrame* parent) : wxPanel(parent,-1,wxDefaultPosition,wxDefaultSize)
{
	//---- initialize the sizers ----
	
	_sizer=new wxBoxSizer(wxVERTICAL);

	//---- initialize and add the components ----

	_attributes_list=new wxListCtrl(this,wxID_ANY,wxDefaultPosition,wxSize(-1,700),wxLC_REPORT|wxLC_HRULES|wxLC_VRULES);

	
	wxListItem columns[2];

	columns[0].SetId(0);
	columns[0].SetText("Attribute");
	_attributes_list->InsertColumn(0, columns[0]);

	columns[1].SetId(1);
	columns[1].SetText("Value");
	_attributes_list->InsertColumn(1, columns[1]);

	wxListItem atts_rows[100];
	for(int i=0;i<100;i++)
	{
		atts_rows[i].SetId(i);
		_attributes_list->InsertItem(atts_rows[i]);
		_attributes_list->SetItem(i,0,"");
		_attributes_list->SetItem(i,1,"");
	}
	
	_sizer->Add(_attributes_list,0,wxEXPAND|wxALL,10);

	//---- set the sizer ----

	SetSizer(_sizer);
}

//---------------------------------------------------------
//	Push_Schema
//--------------------------------------------------------

//template<typename MasterType,typename ParentType,typename InheritanceList>
//template<typename ComponentType,typename CallerType,typename TargetType>
//void Attributes_Panel_Implementation<MasterType,ParentType,InheritanceList>::Push_Schema(vector<string>& attributes_schema)
//{
//	for(int i=0;i<20;i++)
//	{
//		_attributes_list->SetItem(i,0,"");
//		_attributes_list->SetItem(i,1,"");
//	}
//
//	int atts_row_counter = 0;
//
//	vector<string>::iterator itr;
//
//	for(itr=attributes_schema.begin();itr!=attributes_schema.end();itr++,atts_row_counter++)
//	{
//		_attributes_list->SetItem( atts_row_counter,0,(*itr).c_str() );
//	}
//
//	//const char* schema_itr = schema.c_str();
//	//const char* const schema_end = schema_itr + schema.size();
//
//	//int atts_row_counter = 0;
//	//string new_token("");
//
//	//while( schema_itr != schema_end )
//	//{
//	//	if((*schema_itr) == ',')
//	//	{
//	//		_attributes_list->SetItem(atts_row_counter,0,new_token.c_str());
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
//	//_attributes_list->SetItem(atts_row_counter,0,new_token.c_str());
//	
//
//
//
//
//	_attributes_list->SetColumnWidth(0,wxLIST_AUTOSIZE);
//	_attributes_list->SetColumnWidth(1,wxLIST_AUTOSIZE);
//
//	Refresh();
//}


//---------------------------------------------------------
//	Push_Attributes
//--------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Attributes_Panel_Implementation<MasterType,InheritanceList>::Push_Attributes(std::vector<pair<string,string>>& attributes)
{
	for(int i=0;i<100;i++)
	{
		_attributes_list->SetItem(i,0,"");
		_attributes_list->SetItem(i,1,"");
	}

	int atts_row_counter = 0;

	std::vector<pair<string,string>>::iterator itr;

	for(itr=attributes.begin();itr!=attributes.end();itr++,atts_row_counter++)
	{
		if(atts_row_counter == 100) break;

		_attributes_list->SetItem(atts_row_counter,0,itr->first);
		_attributes_list->SetItem(atts_row_counter,1,itr->second);
	}
	
	_attributes_list->SetColumnWidth(0,wxLIST_AUTOSIZE);
	_attributes_list->SetColumnWidth(1,wxLIST_AUTOSIZE);

	Refresh();
}

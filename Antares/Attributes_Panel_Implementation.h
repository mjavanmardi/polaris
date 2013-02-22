//*********************************************************
//	Attributes_Panel_Implementation.cpp - Container Panel for Attributes
//*********************************************************

#pragma once
#include "Attributes_Panel.h"

//---------------------------------------------------------
//	Attributes_Panel_Implementation - attribute_panel class definition
//---------------------------------------------------------

implementation class Attributes_Panel_Implementation : public Polaris_Component<APPEND_CHILD(Attributes_Panel_Implementation),MasterType,NULLTYPE>,public wxPanel
{
public:
	Attributes_Panel_Implementation(wxFrame* parent);
	virtual ~Attributes_Panel_Implementation(void){};

	feature_implementation void Push_Schema(string& schema);
	feature_implementation void Push_Attributes(string& attributes);

	member_pointer(wxListCtrl,attributes_list,none,none);

	member_pointer(wxBoxSizer,sizer,none,none);
};

//---------------------------------------------------------
//	Attributes_Panel_Implementation - attribute_panel initialization
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
Attributes_Panel_Implementation<MasterType,ParentType,InheritanceList>::Attributes_Panel_Implementation(wxFrame* parent) : wxPanel(parent,-1,wxDefaultPosition,wxDefaultSize)
{
	//---- initialize the sizers ----
	
	_sizer=new wxBoxSizer(wxVERTICAL);

	//---- initialize and add the components ----

	_attributes_list=new wxListCtrl(this,wxID_ANY,wxDefaultPosition,wxSize(-1,500),wxLC_REPORT|wxLC_HRULES|wxLC_VRULES);

	
	wxListItem columns[2];

	columns[0].SetId(0);
	columns[0].SetText("Attribute");
	_attributes_list->InsertColumn(0, columns[0]);

	columns[1].SetId(1);
	columns[1].SetText("Value");
	_attributes_list->InsertColumn(1, columns[1]);

	wxListItem atts_rows[20];
	for(int i=0;i<20;i++)
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

template<typename MasterType,typename ParentType,typename InheritanceList>
template<typename ComponentType,typename CallerType,typename TargetType>
void Attributes_Panel_Implementation<MasterType,ParentType,InheritanceList>::Push_Schema(string& schema)
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
			_attributes_list->SetItem(atts_row_counter,0,new_token.c_str());
			new_token.clear();
			++atts_row_counter;
		}
		else
		{
			new_token.push_back((*schema_itr));
		}

		++schema_itr;		
	}

	_attributes_list->SetItem(atts_row_counter,0,new_token.c_str());

	Refresh();
}


//---------------------------------------------------------
//	Push_Attributes
//--------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
template<typename ComponentType,typename CallerType,typename TargetType>
void Attributes_Panel_Implementation<MasterType,ParentType,InheritanceList>::Push_Attributes(string& attributes)
{
	for(int i=0;i<20;i++)
	{
		_attributes_list->SetItem(i,1,"");
	}

	const char* attributes_itr = attributes.c_str();
	const char* const attributes_end = attributes_itr + attributes.size();

	int atts_row_counter = 0;
	string new_token("");

	while( attributes_itr != attributes_end )
	{
		if((*attributes_itr) == ',')
		{
			_attributes_list->SetItem(atts_row_counter,1,new_token.c_str());
			new_token.clear();
			++atts_row_counter;
		}
		else
		{
			new_token.push_back((*attributes_itr));
		}

		++attributes_itr;		
	}

	_attributes_list->SetItem(atts_row_counter,1,new_token.c_str());

	Refresh();
}
//*********************************************************
//	Layer_Options_Implementation.h - Graphical Layer Options
//*********************************************************

#pragma once
#include "Layer_Options.h"

//---------------------------------------------------------
//	Layer_Options_Implementation - layer definition
//---------------------------------------------------------

implementation struct Layer_Options_Implementation:public Polaris_Component<APPEND_CHILD(Layer_Options_Implementation),MasterType>,public wxScrolledWindow
{
	Layer_Options_Implementation(wxPanel* parent);
	virtual ~Layer_Options_Implementation(void){};

	void OnSelectLayer(wxCommandEvent& event);
	void OnToggleLayer(wxCommandEvent& event);
	void OnNavigate(wxCommandEvent& event);
    void OnIdentify(wxCommandEvent& event);

	member_pointer(wxBitmapToggleButton,navigate,none,none);
	member_pointer(wxBitmapToggleButton,identify,none,none);

	member_data(wxBitmap,navigate_bitmap,none,none);
	member_data(wxBitmap,identify_bitmap,none,none);
	member_pointer(wxBoxSizer,button_sizer,none,none);

	member_pointer(wxCheckListBox,layers,none,none);

	member_pointer(wxBoxSizer,sizer,none,none);
	
	feature_implementation void Allocate_New_Layer(string& name);

	member_prototype(Canvas,canvas,typename MasterType::type_of(canvas),none,none);
};

//---------------------------------------------------------
//	Layer_Options_Implementation - layer_options initialization
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
Layer_Options_Implementation<MasterType,ParentType,InheritanceList>::Layer_Options_Implementation(wxPanel* parent) : wxScrolledWindow(parent,-1,wxDefaultPosition,wxDefaultSize)
{
	//---- initialize the sizers ----
	
	_sizer=new wxBoxSizer(wxVERTICAL);
	_button_sizer= new wxBoxSizer(wxHORIZONTAL);

	//---- initialize and add the components ----
	
	_navigate_bitmap=wxBitmap("Navigate.png",wxBITMAP_TYPE_PNG);
	_navigate=new wxBitmapToggleButton(this,wxID_ANY,_navigate_bitmap,wxDefaultPosition,wxSize(50,50));
	_button_sizer->Add(_navigate);
	_navigate->SetToolTip("Toggle Navigate");
	_navigate->SetValue(true);
	Connect(_navigate->GetId(),wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,wxCommandEventHandler(Layer_Options_Implementation::OnNavigate));
	

	_identify_bitmap=wxBitmap("Identify.png",wxBITMAP_TYPE_PNG);
	_identify=new wxBitmapToggleButton(this,wxID_ANY,_identify_bitmap,wxDefaultPosition,wxSize(50,50));
	_button_sizer->Add(_identify,0,wxLEFT,10);
	_identify->SetToolTip("Toggle Identify");
	_identify->SetValue(false);
	Connect(_identify->GetId(),wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,wxCommandEventHandler(Layer_Options_Implementation::OnIdentify));

	_sizer->Add(_button_sizer,0,wxLEFT|wxTOP|wxBOTTOM,10);

	_layers=new wxCheckListBox(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,0,NULL,wxLB_SINGLE|wxLB_NEEDED_SB);
	Connect(_layers->GetId(),wxEVT_COMMAND_LISTBOX_SELECTED,wxCommandEventHandler(Layer_Options_Implementation::OnSelectLayer));
	Connect(_layers->GetId(),wxEVT_COMMAND_CHECKLISTBOX_TOGGLED,wxCommandEventHandler(Layer_Options_Implementation::OnToggleLayer));

	_sizer->Add(_layers,1,wxEXPAND);

	_layers->SetBackgroundColour(wxColour(255,255,255));
	
	//---- set the sizer ----

	SetSizer(_sizer);
}

//---------------------------------------------------------
//	Allocate_New_Layer - handle request to add a layer
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
template<typename ComponentType,typename CallerType,typename TargetType>
void Layer_Options_Implementation<MasterType,ParentType,InheritanceList>::Allocate_New_Layer(string& name)
{
	int layer_id=_layers->Append(name);

	_layers->Check(layer_id,false);

	Refresh();
}

//---------------------------------------------------------
//	OnNavigate - navigate button pressed
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
void Layer_Options_Implementation<MasterType,ParentType,InheritanceList>::OnNavigate(wxCommandEvent& event)
{
	_canvas->Set_Mode<NULLTYPE>(NAVIGATE);
	
	_identify->SetValue(false);
}

//---------------------------------------------------------
//	OnIdentify - identify button pressed
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
void Layer_Options_Implementation<MasterType,ParentType,InheritanceList>::OnIdentify(wxCommandEvent& event)
{
	_canvas->Set_Mode<NULLTYPE>(IDENTIFY);

	_navigate->SetValue(false);
}

//---------------------------------------------------------
//	OnSelectLayer - layer selected
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
void Layer_Options_Implementation<MasterType,ParentType,InheritanceList>::OnSelectLayer(wxCommandEvent& event)
{
	int i=_layers->GetSelection();

	_canvas->Select_Layer<Target_Type<NULLTYPE,void,int>>(i);
}

//---------------------------------------------------------
//	OnToggleLayer - layer toggled
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
void Layer_Options_Implementation<MasterType,ParentType,InheritanceList>::OnToggleLayer(wxCommandEvent& event)
{
	int i=event.GetInt();

	bool checked=_layers->IsChecked(i);

	_canvas->Toggle_Layer<Target_Type<NULLTYPE,void,int,bool>>(i,checked);
}
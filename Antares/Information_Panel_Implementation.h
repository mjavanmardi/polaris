//*********************************************************
//	Information_Panel_Implementation.h - Information and Plotting
//*********************************************************

#pragma once
#include "Information_Panel.h"

//---------------------------------------------------------
//	Information_Panel_Implementation - information panel class definition
//---------------------------------------------------------

implementation class Information_Panel_Implementation : public Polaris_Component<APPEND_CHILD(Information_Panel_Implementation),MasterType,NULLTYPE>,public wxPanel
{
public:
	Information_Panel_Implementation(wxFrame* parent);
	virtual ~Information_Panel_Implementation(void){};
	
	typedef Antares_Layer<typename type_of(MasterType::antares_layer),Information_Panel_Implementation> Antares_Layer_Interface;

	feature_implementation Antares_Layer_Interface* Allocate_New_Layer(string& name)
	{
		Antares_Layer_Interface* new_layer=nullptr;

		list< Information_Page<typename MasterType::type_of(information_page),ComponentType>* >::iterator itr;

		for(itr=_2D_layers.begin();itr!=_2D_layers.end();itr++)
		{
			if((*itr)->layer<Antares_Layer_Interface*>()==nullptr)
			{
				new_layer=(Antares_Layer_Interface*)Allocate<typename type_of(MasterType::antares_layer)>();

				(*itr)->layer<Antares_Layer_Interface*>(new_layer);

				new_layer->list_index<int>(_2D_layers.size() - 1);
				new_layer->name<string&>(name);

				int idx=_information_book->GetPageIndex((wxWindow*)(*itr));

				_information_book->SetPageText(idx,name);

				break;
			}
		}

		return new_layer;
	}
	
	feature_implementation void Render();
	
	void OnSelect(wxAuiNotebookEvent& event);

	//void OnResize(wxSizeEvent& event);

	member_pointer(wxAuiNotebook,information_book,none,none);
	member_pointer(wxBoxSizer,sizer,none,none);
	
	member_data(int,cached_iteration,none,none);

	list< Information_Page<typename MasterType::type_of(information_page),ComponentType>* > _2D_layers;

	member_data(bool,initialized,none,none);
};

static _lock _plot_lock;

//---------------------------------------------------------
//	Information_Panel - information initialization
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
Information_Panel_Implementation<MasterType,ParentType,InheritanceList>::Information_Panel_Implementation(wxFrame* parent) : wxPanel(parent,-1,wxDefaultPosition,wxDefaultSize,wxCLIP_CHILDREN )
{
	UNLOCK(_plot_lock);

	SetBackgroundColour(wxColor(255,255,255));

	//---- initialize the sizer and container notebook ----
	
	_sizer=new wxBoxSizer(wxVERTICAL);

	_information_book=new wxAuiNotebook(this,-1,wxDefaultPosition,wxDefaultSize,wxAUI_NB_TOP);

	for(int i=0;i<20;i++)
	{
		Information_Page<typename MasterType::type_of(information_page),ComponentType>* layer = (Information_Page<typename MasterType::type_of(information_page),ComponentType>*) new typename MasterType::type_of(information_page)(_information_book);
		
		_2D_layers.push_back(layer);
		
		_information_book->AddPage((wxWindow*)layer,"");
	}

	_sizer->Add(_information_book,1,wxEXPAND);

	//---- set the sizer ----

	SetSizerAndFit(_sizer);
	
	Connect(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED,wxAuiNotebookEventHandler(Information_Panel_Implementation::OnSelect));

	_initialized = false;
}


template<typename MasterType,typename ParentType,typename InheritanceList>
void Information_Panel_Implementation<MasterType,ParentType,InheritanceList>::OnSelect(wxAuiNotebookEvent& event)
{
	Render<ComponentType,ComponentType,NT>();
}

//template<typename MasterType,typename ParentType,typename InheritanceList>
//void Information_Panel_Implementation<MasterType,ParentType,InheritanceList>::OnResize(wxSizeEvent& event)
//{
//	_box->SetDimension(wxPoint(0,0),GetSize());
//}
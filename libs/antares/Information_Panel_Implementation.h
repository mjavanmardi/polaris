//*********************************************************
//	Information_Panel_Implementation.h - Information and Plotting
//*********************************************************

#pragma once
#include "Information_Panel.h"

//---------------------------------------------------------
//	Information_Panel_Implementation - information panel class definition
//---------------------------------------------------------

implementation class Information_Panel_Implementation : public Polaris_Component<MasterType,INHERIT(Information_Panel_Implementation),NULLTYPE>,public wxPanel
{
public:
	Information_Panel_Implementation(wxFrame* parent);
	virtual ~Information_Panel_Implementation(void){};
	
	typedef Antares_Layer<typename MasterType::antares_layer_type> Antares_Layer_Interface;

	template<typename TargetType> Antares_Layer_Interface* Allocate_New_Layer(string& name)
	{
		Antares_Layer_Interface* new_layer=nullptr;

		std::list< Information_Page<typename MasterType::information_page_type>* >::iterator itr;

		bool blank_page_skipped=false;

		for(itr=_2D_layers.begin();itr!=_2D_layers.end();itr++)
		{
			if((*itr)->layer<Antares_Layer_Interface*>()==nullptr && blank_page_skipped)
			{
				new_layer=(Antares_Layer_Interface*)Allocate<typename MasterType::antares_layer_type>();

				(*itr)->layer<Antares_Layer_Interface*>(new_layer);

				new_layer->list_index<int>(_2D_layers.size() - 1);
				new_layer->name<string&>(name);

				int idx=_information_book->GetPageIndex((wxWindow*)(*itr));

				_information_book->SetPageText(idx,name);

				break;
			}

			blank_page_skipped=true;
		}

		return new_layer;
	}
	
	template<typename TargetType> void Render();
	
	void OnSelect(wxAuiNotebookEvent& event);

	//void OnResize(wxSizeEvent& event);

	m_data(wxAuiNotebook*,information_book, NONE, NONE);
	m_data(wxBoxSizer*,sizer, NONE, NONE);
	
	m_data(int,cached_iteration, NONE, NONE);

	std::list< Information_Page<typename MasterType::information_page_type>* > _2D_layers;

	m_data(bool,initialized, NONE, NONE);
};

static _lock _plot_lock;

//---------------------------------------------------------
//	Information_Panel - information initialization
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
Information_Panel_Implementation<MasterType,InheritanceList>::Information_Panel_Implementation(wxFrame* parent) : wxPanel(parent,-1,wxDefaultPosition,wxDefaultSize,wxCLIP_CHILDREN )
{
	UNLOCK(_plot_lock);

	SetBackgroundColour(wxColor(255,255,255));

	//---- initialize the sizer and container notebook ----
	
	_sizer=new wxBoxSizer(wxVERTICAL);

	_information_book=new wxAuiNotebook(this,-1,wxDefaultPosition,wxDefaultSize,wxAUI_NB_TOP|wxAUI_NB_SCROLL_BUTTONS);

	//RLW%%%
	for(int i=0;i<25;i++)
	{
		Information_Page<typename MasterType::information_page_type>* layer = (Information_Page<typename MasterType::information_page_type>*) new typename MasterType::information_page_type(_information_book);
		
		_2D_layers.push_back(layer);
		
		_information_book->AddPage((wxWindow*)layer,"");
	}

	_sizer->Add(_information_book,1,wxEXPAND);

	//---- set the sizer ----

	SetSizerAndFit(_sizer);
	
	Connect(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED,wxAuiNotebookEventHandler(Information_Panel_Implementation::OnSelect));

	_initialized = false;
}


template<typename MasterType,typename InheritanceList>
void Information_Panel_Implementation<MasterType,InheritanceList>::OnSelect(wxAuiNotebookEvent& event)
{
	Render<NT>();
}

//template<typename MasterType,typename ParentType,typename InheritanceList>
//void Information_Panel_Implementation<MasterType,ParentType,InheritanceList>::OnResize(wxSizeEvent& event)
//{
//	_box->SetDimension(wxPoint(0,0),GetSize());
//}

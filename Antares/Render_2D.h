//*********************************************************
//	Render.h - Primary Render Function for Plotting
//*********************************************************

#pragma once
#include "Information_Panel_Implementation.h"

//---------------------------------------------------------
//	Render
//--------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
template<typename ComponentType,typename CallerType,typename TargetType>
void Information_Panel_Implementation<MasterType,ParentType,InheritanceList>::Render()
{
	return;
	if(!_initialized)
	{
		Connect(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED,wxAuiNotebookEventHandler(Information_Panel_Implementation::OnSelect));
		_initialized = true;
	}

	int current_iteration = _iteration - 1;

	if(current_iteration!=_cached_iteration)
	{
		_cached_iteration=current_iteration;
	}

	//---- loop over and draw all layers ----

	((Information_Page<typename MasterType::type_of(information_page),ComponentType>*)_information_book->GetPage(_information_book->GetSelection()))->Draw_Layer<NULLTYPE>(current_iteration,current_iteration);
	
	//list< Information_Page<typename MasterType::type_of(information_page),ComponentType>* >::iterator itr;

	//for(itr=_2D_layers.begin();itr!=_2D_layers.end();itr++)
	//{
	//	(*itr)->Draw_Layer<NULLTYPE>(current_iteration,current_iteration);
	//}

	//---- flush and display ----

	//_plotwindow->RenewPlot();
}
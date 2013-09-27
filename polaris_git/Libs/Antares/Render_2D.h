//*********************************************************
//	Render.h - Primary Render Function for Plotting
//*********************************************************

#pragma once
#include "Information_Panel_Implementation.h"

//---------------------------------------------------------
//	Render
//--------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Information_Panel_Implementation<MasterType,InheritanceList>::Render()
{
	LOCK(_plot_lock);

	int current_iteration = iteration() - 1;

	if(current_iteration!=_cached_iteration)
	{
		_cached_iteration=current_iteration;
	}

	//---- loop over and draw all layers ----

	Information_Page<typename MasterType::type_of(information_page)>* active_page = (Information_Page<typename MasterType::type_of(information_page)>*)_information_book->GetPage(_information_book->GetSelection());
	
	if(active_page->layer<void*>()!=nullptr)
	{
		active_page->Draw_Layer<NULLTYPE>(current_iteration,current_iteration);
	}
	
	UNLOCK(_plot_lock);

	//boost::container::list< Information_Page<typename MasterType::type_of(information_page),ComponentType>* >::iterator itr;

	//for(itr=_2D_layers.begin();itr!=_2D_layers.end();itr++)
	//{
	//	(*itr)->Draw_Layer<NULLTYPE>(current_iteration,current_iteration);
	//}
}
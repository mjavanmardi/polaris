//*********************************************************
//	Layer_Management.h - Functions which manage layer options
//*********************************************************

#pragma once
#include "Canvas_Implementation.h"

//---------------------------------------------------------
//	Allocate_New_Layer - allocate a new antares layer
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
template<typename CallerType,typename TargetType>
typename Canvas_Implementation<MasterType,ParentType,InheritanceList>::Antares_Layer_Interface* Canvas_Implementation<MasterType,ParentType,InheritanceList>::Allocate_New_Layer(string& name)
{
	Antares_Layer_Interface* new_layer=(Antares_Layer_Interface*)Allocate<typename type_of(MasterType::antares_layer)>();

	_3D_Layers.push_back(new_layer);
	
	new_layer->list_index<int>(_3D_Layers.size() - 1);
	new_layer->name<string&>(name);

	_layer_options->Allocate_New_Layer<Target_Type<NULLTYPE,void,string&>>(name);

	return new_layer;
}

//---------------------------------------------------------
//	Toggle_Layer - toggle the selected layer
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
template<typename CallerType,typename TargetType>
void Canvas_Implementation<MasterType,ParentType,InheritanceList>::Toggle_Layer(int identifier)
{
	int counter=0;

	list<Antares_Layer_Interface*>::iterator itr;

	for(itr=_3D_Layers.begin();itr!=_3D_Layers.end();++itr,++counter)
	{
		if(counter==identifier)
		{
			bool current=(*itr)->draw<bool>();
			(*itr)->draw<bool>(!current);
			break;
		}
	}

	Refresh();
}

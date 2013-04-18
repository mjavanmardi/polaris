//*********************************************************
//	Layer_Management.h - Functions which manage layer options
//*********************************************************

#pragma once
#include "Canvas_Implementation.h"

//---------------------------------------------------------
//	Allocate_New_Layer - allocate a new antares layer
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
template<typename ComponentType,typename CallerType,typename TargetType>
typename Canvas_Implementation<MasterType,ParentType,InheritanceList>::Antares_Layer_Interface* Canvas_Implementation<MasterType,ParentType,InheritanceList>::Allocate_New_Layer(string& name)
{
	SetFocus();

	Antares_Layer_Interface* new_layer=(Antares_Layer_Interface*)Allocate<typename type_of(MasterType::antares_layer)>();

	_layers.push_back(new_layer);
	
	new_layer->list_index<int>(_layers.size() - 1);

	new_layer->name<string&>(name);
	new_layer->attributes_panel<attributes_panel_interface*>(_attributes_panel);

	_layer_options->Allocate_New_Layer<Target_Type<NULLTYPE,void,string&>>(name);

	return new_layer;
}

//---------------------------------------------------------
//	Toggle_Layer - toggle the selected layer
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
template<typename ComponentType,typename CallerType,typename TargetType>
void Canvas_Implementation<MasterType,ParentType,InheritanceList>::Toggle_Layer(int identifier,bool checked)
{
	SetFocus();

	int counter=0;

	list<Antares_Layer_Interface*>::iterator itr;

	for(itr=_layers.begin();itr!=_layers.end();++itr,++counter)
	{
		if(counter==identifier)
		{
			(*itr)->draw<bool>(checked);
			break;
		}
	}

	Refresh();
}

//---------------------------------------------------------
//	Select_Layer - select a layer
//---------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
template<typename ComponentType,typename CallerType,typename TargetType>
void Canvas_Implementation<MasterType,ParentType,InheritanceList>::Select_Layer(int identifier)
{
	SetFocus();

	int counter=0;

	list<Antares_Layer_Interface*>::iterator itr;

	for(itr=_layers.begin();itr!=_layers.end();++itr,++counter)
	{
		if(counter==identifier)
		{
			if(_selected_layer != nullptr) _selected_layer->Deselect_All<NULLTYPE>();

			_selected_layer=(*itr);

			_selected_layer->Select<NULLTYPE>();
			
			break;
		}
	}

	Refresh();
}

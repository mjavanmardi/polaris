//*********************************************************
//	Antares_Depot_Implementation.h - Graphical Depot
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Depot_Prototype.h"

namespace Depot_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{

	}
	
	namespace Implementations
	{
		template<typename MasterType,typename ParentType=NULLTYPE,typename InheritanceList=NULLTYPELIST,template<class,class,class> class InheritanceTemplate=NULLTEMPLATE_3>
		struct Antares_Depot : public InheritanceTemplate<MasterType,ParentType,APPEND_CHILD(Antares_Depot)>
		{
			typedef typename InheritanceTemplate<MasterType,NT,APPEND_CHILD(Antares_Depot)>::ComponentType ComponentType;
			
			feature_implementation static void Initialize_Type(void* obj,string& name)
			{
				InheritanceTemplate<MasterType,NT,APPEND_CHILD(Antares_Depot)>::Initialize_Type<ComponentType,CallerType,NT>(obj);

				//_event_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(event_layer),ComponentType>*, string& > >(name);

				//Antares_Layer_Configuration cfg;
				//cfg.Configure_Dynamic_Lines();
				//cfg.grouped=true;
				////cfg.group_color=true;
				//cfg.head_size_value=4;
				//cfg.storage_period=1;
				//
				//for(vector<string>::iterator itr=ComponentType::_event_keys.begin();itr!=ComponentType::_event_keys.end();itr++)
				//{
				//	cfg.attributes_schema.push_back(*itr);
				//}

				//cfg.head_color._r = 0;
				//cfg.head_color._g = 0;
				//cfg.head_color._b = 255;
				//cfg.head_color._a = 255;

				//_event_layer->Initialize<NULLTYPE>(cfg);
			}

			static member_prototype(Antares_Layer,its_component_layer,typename type_of(MasterType::antares_layer),none,none);
		};
		
		template<typename MasterType,typename ParentType,typename InheritanceList,template<class,class,class> class InheritanceTemplate>
		Antares_Layer<typename type_of(MasterType::antares_layer),typename Antares_Depot<MasterType,ParentType,InheritanceList,InheritanceTemplate>::ComponentType>* Antares_Depot<MasterType,ParentType,InheritanceList,InheritanceTemplate>::_its_component_layer;

		implementation struct Antares_Tow_Truck_Depot : public Antares_Depot<MasterType,NT,APPEND_CHILD(Antares_Tow_Truck_Depot),Tow_Truck_Depot>
		{
			feature_implementation static void Initialize_Type(void* obj)
			{
				Antares_Depot::Initialize_Type<ComponentType,CallerType,NT>(obj,string("Tow Truck Depots"));
			}
		};
	}
}
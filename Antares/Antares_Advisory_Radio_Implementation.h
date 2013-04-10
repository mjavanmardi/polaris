//*********************************************************
//	Antares_Advisory_Radio_Implementation.h - Graphical Advisory Radio
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Advisory_Radio_Prototype.h"

namespace Advisory_Radio_Components
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
		struct Antares_Advisory_Radio : public InheritanceTemplate<MasterType,ParentType,APPEND_CHILD(Antares_Advisory_Radio)>
		{
			typedef typename InheritanceTemplate<MasterType,NT,APPEND_CHILD(Antares_Advisory_Radio)>::ComponentType ComponentType;

			static member_prototype(Antares_Layer,its_component_layer,typename type_of(MasterType::antares_layer),none,none);

			feature_implementation static void Initialize_Type(void* obj,string& name)
			{
				InheritanceTemplate<MasterType,NT,APPEND_CHILD(Antares_Advisory_Radio)>::Initialize_Type<ComponentType,CallerType,NT>(obj);

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
		};
		
		template<typename MasterType,typename ParentType,typename InheritanceList,template<class,class,class> class InheritanceTemplate>
		Antares_Layer<typename type_of(MasterType::antares_layer),typename Antares_Advisory_Radio<MasterType,ParentType,InheritanceList,InheritanceTemplate>::ComponentType>* Antares_Advisory_Radio<MasterType,ParentType,InheritanceList,InheritanceTemplate>::_its_component_layer;

		implementation struct Antares_Highway_Advisory_Radio : public Antares_Advisory_Radio<MasterType,NT,APPEND_CHILD(Antares_Highway_Advisory_Radio),Highway_Advisory_Radio>
		{
			feature_implementation static void Initialize_Type(void* obj)
			{
				Antares_Advisory_Radio::Initialize_Type<ComponentType,CallerType,NT>(obj,string("Highway Advisory Radios"));
			}
		};
	}
}
//*********************************************************
//	Graphical_Vehicle_Implementation.h - Graphical Vehicle Variant
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Network_Event_Prototype.h"

namespace Network_Event_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{

	}
	
	namespace Implementations
	{

		implementation struct Antares_Weather_Network_Event : public Weather_Network_Event<MasterType,NT,APPEND_CHILD(Antares_Weather_Network_Event)>
		{
			feature_implementation static void Initialize_Type()
			{
				_event_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(event_layer),ComponentType>*, string& > >(string("Weather_Events"));

				Antares_Layer_Configuration cfg;
				cfg.Configure_Lines();
				cfg.dynamic_data=true;
				cfg.grouped=true;
				cfg.group_color=true;
				cfg.head_size_value=4;

				_event_layer->Initialize<NULLTYPE>(cfg);
			}
			
			feature_implementation void Initialize()
			{
				Weather_Network_Event::Initialize();
			}

			static member_prototype(Antares_Layer,event_layer,typename type_of(MasterType::antares_layer),none,none);
		};
		
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),Antares_Weather_Network_Event<MasterType,ParentType,InheritanceList>>* Antares_Weather_Network_Event<MasterType,ParentType,InheritanceList>::_event_layer;
		
		implementation struct Antares_Accident_Network_Event : public Accident_Network_Event<MasterType,NT,APPEND_CHILD(Antares_Accident_Network_Event)>
		{
			feature_implementation static void Initialize_Type()
			{
				_event_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(event_layer),ComponentType>*, string& > >(string("Weather_Events"));

				Antares_Layer_Configuration cfg;
				cfg.Configure_Lines();
				cfg.dynamic_data=true;
				cfg.grouped=true;
				cfg.group_color=true;
				cfg.head_size_value=4;

				_event_layer->Initialize<NULLTYPE>(cfg);
			}
			
			feature_implementation void Initialize()
			{
				Accident_Network_Event::Initialize();
			}
			static member_prototype(Antares_Layer,event_layer,typename type_of(MasterType::antares_layer),none,none);
		};
		
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),Antares_Accident_Network_Event<MasterType,ParentType,InheritanceList>>* Antares_Accident_Network_Event<MasterType,ParentType,InheritanceList>::_event_layer;


		implementation struct Antares_Congestion_Network_Event : public Congestion_Network_Event<MasterType,NT,APPEND_CHILD(Antares_Congestion_Network_Event)>
		{
			feature_implementation static void Initialize_Type()
			{
				_event_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(event_layer),ComponentType>*, string& > >(string("Weather_Events"));

				Antares_Layer_Configuration cfg;
				cfg.Configure_Lines();
				cfg.dynamic_data=true;
				cfg.grouped=true;
				cfg.group_color=true;
				cfg.head_size_value=4;

				_event_layer->Initialize<NULLTYPE>(cfg);
			}
			
			feature_implementation void Initialize()
			{
				Congestion_Network_Event::Initialize();
			}
			static member_prototype(Antares_Layer,event_layer,typename type_of(MasterType::antares_layer),none,none);
		};
		
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),Antares_Congestion_Network_Event<MasterType,ParentType,InheritanceList>>* Antares_Congestion_Network_Event<MasterType,ParentType,InheritanceList>::_event_layer;


		implementation struct Antares_Lane_Closure_Network_Event : public Lane_Closure_Network_Event<MasterType,NT,APPEND_CHILD(Antares_Lane_Closure_Network_Event)>
		{
			feature_implementation static void Initialize_Type()
			{
				_event_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(event_layer),ComponentType>*, string& > >(string("Weather_Events"));

				Antares_Layer_Configuration cfg;
				cfg.Configure_Lines();
				cfg.dynamic_data=true;
				cfg.grouped=true;
				cfg.group_color=true;
				cfg.head_size_value=4;

				_event_layer->Initialize<NULLTYPE>(cfg);
			}
			
			feature_implementation void Initialize()
			{
				Lane_Closure_Network_Event::Initialize();
			}
			static member_prototype(Antares_Layer,event_layer,typename type_of(MasterType::antares_layer),none,none);
		};

		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),Antares_Lane_Closure_Network_Event<MasterType,ParentType,InheritanceList>>* Antares_Lane_Closure_Network_Event<MasterType,ParentType,InheritanceList>::_event_layer;

	}

}

using namespace Network_Event_Components::Implementations;
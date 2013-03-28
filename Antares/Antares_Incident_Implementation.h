//*********************************************************
//	Graphical_Vehicle_Implementation.h - Graphical Vehicle Variant
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Incident_Prototype.h"

namespace Incident_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{

	}
	
	namespace Implementations
	{

		//implementation struct Antares_Base_Incident : public Polaris_Component<APPEND_CHILD(Base_Incident),MasterType,Data_Object,ParentType>
		//{
		//	static member_prototype(Antares_Layer,incident_layer,typename type_of(MasterType::antares_layer),none,none);
		//};

		implementation struct Antares_Weather_Incident : public Weather_Incident<MasterType,NT,APPEND_CHILD(Antares_Weather_Incident)>
		{
			//static void Initialize() - Allocate the layer and configure for this geometry

				//_vehicle_points=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(vehicle_points),Graphical_Vehicle_Implementation>*, string& > >(string("Vehicles"));
				//Antares_Layer_Configuration cfg;
				//cfg.Configure_Points();
				//cfg.draw=true;
				//cfg.primitive_color=true;

				//cfg.attributes_schema.push_back("ID");
				//cfg.attributes_schema.push_back("Status");
				//cfg.attributes_schema.push_back("Current_Link");

				//cfg.attributes_callback = (attributes_callback_type)&fetch_attributes;
				//cfg.submission_callback = (attributes_callback_type)&submit_attributes;

				//_vehicle_points->Initialize<NULLTYPE>(cfg);
		};
		
		implementation struct Antares_Accident_Incident : public Accident_Incident<MasterType,NT,APPEND_CHILD(Antares_Accident_Incident)>
		{
			//static void Initialize() - Allocate the layer and configure for this geometry
		};

		implementation struct Antares_Congestion_Incident : public Congestion_Incident<MasterType,NT,APPEND_CHILD(Antares_Congestion_Incident)>
		{
			//static void Initialize() - Allocate the layer and configure for this geometry
		};
		
		implementation struct Antares_Lane_Closure_Incident : public Lane_Closure_Incident<MasterType,NT,APPEND_CHILD(Antares_Lane_Closure_Incident)>
		{
			//static void Initialize() - Allocate the layer and configure for this geometry
		};

		implementation struct Antares_Incident_Manager:public Incident_Manager_Implementation<MasterType,ParentType,APPEND_CHILD(Antares_Incident_Manager)>
		{
			typedef Incident_Manager_Implementation<MasterType,ParentType,APPEND_CHILD(Antares_Incident_Manager)> Parent;

			void Initialize()
			{
				// Call Parent::Initialize();

				//Swap_Event() to Antares Event;

				//Use metafunction to loop over incident types and initialize each
				//Alternatively, create a switch over types in the prototype to initialize based off of component_index

			}

			declare_feature_event(compute_vehicle_position)
			{
				//Call Parent Event()

#pragma pack(push,1)
				struct attribute_coordinate
				{
					void* ptr;
					True_Color_RGBA<NT> color;
					Point_3D<MasterType> vertex;
				} coordinate;
#pragma pack(pop)

				coordinate.ptr = _this;
				
				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>(coordinate.vertex);

				_vehicle_points->Push_Element<Regular_Element>(&coordinate);
			}

			//static bool fetch_attributes(Antares_Incident_Manager* _this,vector<string>& bucket)
			//{
			//}
			//
			//static bool submit_attributes(Antares_Incident_Manager* _this,vector<string>& bucket)
			//{
			//}


		};
	}

}

using namespace Incident_Components::Implementations;
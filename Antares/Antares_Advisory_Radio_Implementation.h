//*********************************************************
//	Antares_Advisory_Radio_Implementation.h - Graphical Advisory Radio
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Advisory_Radio_Prototype.h"
#include "Antares_Advisory_ITS_Implementation.h"

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
		implementation struct Antares_Advisory_Radio : public Advisory_ITS_Components::Implementations::Antares_Advisory_ITS<MasterType,ParentType,APPEND_CHILD(Antares_Advisory_Radio),Highway_Advisory_Radio>
		{
			typedef typename Antares_Advisory_ITS::ComponentType ComponentType;

			feature_implementation static void Initialize_Type()
			{
				Antares_Advisory_ITS::Initialize_Type<ComponentType,CallerType,NT>();
				Antares_Layer_Configuration cfg;
				_its_coverage_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(its_component_layer),ComponentType>*, string& > >(string("HAR Coverage"));

				cfg.Configure_Static_Lines();
				cfg.grouped=true;
				cfg.head_size_value=6;
				cfg.head_accent_size_value=6;
				cfg.draw=true;

				cfg.head_color._r = 255;
				cfg.head_color._g = 0;
				cfg.head_color._b = 0;
				cfg.head_color._a = 200;

				_its_coverage_layer->Initialize<NULLTYPE>(cfg);

				
				_its_component_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(its_component_layer),ComponentType>*, string& > >(string("Highway Advisory Radios"));

				
				cfg.Configure_Static_Points();
				cfg.head_texture = cfg.Add_Texture(string("HAR.jpg"));
				cfg.grouped=false;
				cfg.head_size_value=32;
				cfg.head_accent_size_value=64;
				cfg.selection_callback=&on_select;
				cfg.double_click_callback=&on_double_click;
				cfg.submission_callback=&on_submit;

				cfg.head_color._r = 150;
				cfg.head_color._g = 255;
				cfg.head_color._b = 150;
				cfg.head_color._a = 255;

				_its_component_layer->Initialize<NULLTYPE>(cfg);



			}
		};
	}
}
//*********************************************************
//	Antares_Variable_Message_Sign_Implementation.h - Graphical VMS
//*********************************************************

#pragma once
#include "Dependencies.h"
#include "Traffic_Simulator\Variable_Message_Sign_Implementation.h"
#include "Antares_Advisory_ITS_Implementation.h"

namespace Variable_Message_Sign_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{

	}
	
	namespace Implementations
	{
		implementation struct Antares_Variable_Word_Sign : public Advisory_ITS_Components::Implementations::Antares_Advisory_ITS<MasterType,INHERIT(Antares_Variable_Word_Sign),Variable_Word_Sign>
		{
			static float pixel_size_callback(float meter_per_pixel_mid)
			{
				float size = floor((1.57777/meter_per_pixel_mid)*54.0f+24.0f);
				return size;
			}
			
			static float accented_pixel_size_callback(float meter_per_pixel_mid)
			{
				float size = floor((1.57777/meter_per_pixel_mid)*70.0f+32.0f);
				return size;
			}

			template<typename TargetType> static void Initialize_Type()
			{
				using namespace Advisory_ITS_Components::Implementations;
				Antares_Layer_Configuration cfg;
				
//TODO
//				//_its_coverage_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(its_component_layer)>*, string& > >(string("VMS Coverage"));

				//cfg.Configure_Static_Lines();
				//cfg.grouped=true;
				//cfg.head_size_value=6;
				//cfg.head_accent_size_value=6;
				//cfg.draw=true;

				//cfg.head_color._r = 255;
				//cfg.head_color._g = 0;
				//cfg.head_color._b = 0;
				//cfg.head_color._a = 200;

				//_its_coverage_layer->Initialize<NULLTYPE>(cfg);


				_its_component_layer=Allocate_New_Layer<MT>(string("Variable Message Signs"));

				
				cfg.Configure_Static_Points();
				cfg.head_texture = cfg.Add_Texture(string("C:\\opt\\polarisdeps\\antares\\VMS.png"));
				cfg.grouped=false;
				cfg.head_size_value=24;
				cfg.head_accent_size_value=48;
				cfg.selection_callback=&on_select;
				cfg.double_click_callback=&on_double_click;
				cfg.submission_callback=&on_submit;
				cfg.pixel_size_callback = &pixel_size_callback;
				cfg.accent_pixel_size_callback = &accented_pixel_size_callback;

				cfg.head_color._r = 255;
				cfg.head_color._g = 255;
				cfg.head_color._b = 255;
				cfg.head_color._a = 255;

				_its_component_layer->Initialize<NULLTYPE>(cfg);



			}
		};
	}
}

//*********************************************************
//	Antares_Variable_Speed_Sign_Implementation.h - Graphical VSS
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
		implementation struct Antares_Variable_Speed_Sign : public Advisory_ITS_Components::Implementations::Antares_Advisory_ITS<MasterType,INHERIT(Antares_Variable_Speed_Sign),Variable_Speed_Sign>
		{

			static void on_double_click(const std::list<void*>& selected,std::vector<pair<string,string>>& attributes,std::vector<std::vector<string>>& dropdowns)
			{
				ComponentType* its_component=(ComponentType*)selected.back();

				dropdowns.resize(1);

				Link_Interface* link = its_component->_covered_links[0];
				
				stringstream s;

				for(float i=0.0f;i<link->original_speed_limit<float>()*2.0;i+=5.0f)
				{
					s << i;
					dropdowns[0].push_back( s.str() );
					s.str("");
				}

				pair<string,string> atts;

				
				
				s << link->speed_limit<float>();

				atts.first = "Current Speed Limit";
				atts.second = s.str();
				
				s.str("");
				
				attributes.push_back(atts);

				
				
				s << link->original_speed_limit<float>();

				atts.first = "Original Speed Limit";
				atts.second = s.str();

				attributes.push_back(atts);
			}

			template<typename TargetType> void Display_Attributes(std::vector<pair<string,string>>& bucket)
			{
				pair<string,string> key_value_pair;
				
				Link_Interface* link = _covered_links[0];

				key_value_pair.first="Current Limit";
				
				stringstream s;

				s << link->speed_limit<float>();

				key_value_pair.second=s.str();
				
				bucket.push_back(key_value_pair);
			}

			static bool on_submit(const std::list<void*>& selected,const std::vector<string>& attribute_choices,const std::vector<string>& dropdown_choices)
			{
				ComponentType* its_component=(ComponentType*)selected.back();

				string user_event_choice = dropdown_choices[0];

				Link_Interface* link = its_component->_covered_links[0];
				
				link->change_speed_limit<float>( atof(user_event_choice.c_str()) );

				return true;
			}
			
			static float pixel_size_callback(float meter_per_pixel_mid)
			{
				float size = floor((1.57777/meter_per_pixel_mid)*48.0f+24.0f);
				return size;
			}
			
			static float accented_pixel_size_callback(float meter_per_pixel_mid)
			{
				float size = floor((1.57777/meter_per_pixel_mid)*64.0f+32.0f);
				return size;
			}

			template<typename TargetType> static void Initialize_Type()
			{
				using namespace Advisory_ITS_Components::Implementations;
				Antares_Layer_Configuration cfg;
				
//TODO
//				//_its_coverage_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(its_component_layer)>*, string& > >(string("VSS Coverage"));
				
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


				_its_component_layer=Allocate_New_Layer<MT>(string("Variable Speed Signs"));

				
				cfg.Configure_Static_Points();
				cfg.head_texture = cfg.Add_Texture(string("C:\\opt\\polarisdeps\\antares\\VSS.png"));
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

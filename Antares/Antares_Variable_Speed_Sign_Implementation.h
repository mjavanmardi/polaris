//*********************************************************
//	Antares_Variable_Speed_Sign_Implementation.h - Graphical VSS
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Variable_Message_Sign_Prototype.h"
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
		implementation struct Antares_Variable_Speed_Sign : public Advisory_ITS_Components::Implementations::Antares_Advisory_ITS<MasterType,ParentType,APPEND_CHILD(Antares_Variable_Speed_Sign),Variable_Speed_Sign>
		{

			static void on_double_click(const list<void*>& selected,vector<pair<string,string>>& attributes,vector<vector<string>>& dropdowns)
			{
				ComponentType* its_component=(ComponentType*)selected.back();

				dropdowns.resize(1);

				Link_Interface* link = its_component->_covered_links[0];
				
				stringstream s;

				for(float i=5.0f;i<link->speed_limit<float>()*1.2;i+=5.0f)
				{
					s << i;
					dropdowns[0].push_back( s.str() );
					s.str("");
				}
			}

			feature_implementation void Display_Attributes(vector<pair<string,string>>& bucket)
			{
				pair<string,string> key_value_pair;
				
				Link_Interface* link = _covered_links[0];

				key_value_pair.first="Current Limit";
				
				stringstream s;

				s << link->speed_limit<float>();

				key_value_pair.second=s.str();
				
				bucket.push_back(key_value_pair);
			}

			static bool on_submit(const list<void*>& selected,const vector<string>& attribute_choices,const vector<string>& dropdown_choices)
			{
				ComponentType* its_component=(ComponentType*)selected.back();

				string user_event_choice = dropdown_choices[0];

				Link_Interface* link = its_component->_covered_links[0];
				
				link->change_speed_limit<float>( atof(user_event_choice.c_str()) );

				return true;
			}

			feature_implementation static void Initialize_Type()
			{
				using namespace Advisory_ITS_Components::Implementations;
				Antares_Layer_Configuration cfg;
				
				//_its_coverage_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(its_component_layer),ComponentType>*, string& > >(string("VSS Coverage"));
				
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


				_its_component_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(its_component_layer),ComponentType>*, string& > >(string("Variable Speed Signs"));

				
				cfg.Configure_Static_Points();
				cfg.head_texture = cfg.Add_Texture(string("VSS.png"));
				cfg.grouped=false;
				cfg.head_size_value=24;
				cfg.head_accent_size_value=48;
				cfg.selection_callback=&on_select;
				cfg.double_click_callback=&on_double_click;
				cfg.submission_callback=&on_submit;

				cfg.head_color._r = 255;
				cfg.head_color._g = 255;
				cfg.head_color._b = 255;
				cfg.head_color._a = 255;

				_its_component_layer->Initialize<NULLTYPE>(cfg);



			}
		};
	}
}
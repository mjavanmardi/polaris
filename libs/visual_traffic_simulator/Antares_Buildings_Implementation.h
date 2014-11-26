//*********************************************************
//	Antares_Buildings_Implementation.h - Draw Buildings
//*********************************************************

#pragma once
#include "Dependencies.h"

namespace Buildings_Components
{
	namespace Types
	{
	}
	
	namespace Concepts
	{
	}
	
	namespace Implementations
	{
//#pragma pack(push,1)
//		struct Building_Primitive
//		{
//			Point_3D<NT> a;
//			Point_3D<NT> b;
//			Point_3D<NT> c;
//		};
//#pragma pack(pop)

#pragma pack(push,1)
		struct Building_Group
		{
			//Vehicle_Attribute_Shape()
			//{
			//	data = new Vehicle_Attribute_Primitive[10];
			//	num_primitives = 10;
			//}

			//~Vehicle_Attribute_Shape()
			//{
			//	delete[] data;
			//}
			True_Color_RGBA<NT> color;
			int num_primitives;
			Point_3D<NT>* data;
		};
#pragma pack(pop)

		implementation struct Antares_Buildings_Implementation : public Polaris_Component<MasterType,INHERIT(Antares_Buildings_Implementation),Data_Object>
		{
			typedef typename Antares_Buildings_Implementation::ComponentType ComponentType;

			void Initialize(string& buildings_filename)
			{
				Building_Group* building_group = new Building_Group();
				//Allocate plenty of vertices to re-use
				building_group->data = new Point_3D<NT>[2000];

				ifstream buildings_file;
				//buildings_file.open("Building_Min_5000_sqft_Simplified.bdg",ios::binary);
				buildings_file.open(buildings_filename.c_str(),ios::binary);

				char* buffer;
				
				unsigned int begin=buildings_file.tellg();
				buildings_file.seekg(0,ios::end);
				unsigned int end=buildings_file.tellg();
				buildings_file.seekg(0,ios::beg);

				buffer=new char[end-begin];

				buildings_file.read(buffer,end-begin);

				int triangle_counter = 0;

				unsigned int i=0;
				unsigned int j=0;
				unsigned int k=0;
				unsigned int stories=0;
				unsigned int floats=0;
				float coord=0;
				float height=0;
				unsigned char r=0;
				unsigned char g=0;
				unsigned char b=0;

				unsigned char name_len=0;

				bool push_coordinates_flag=true;

				//---- skip the bvis ----

				i+=8;

				bool print_flag = true;

				while(i<(end-begin-4))
				{
					Point_3D<NT>* current_vertex = building_group->data;
					triangle_counter = 0;
					push_coordinates_flag = true;


					int random_num = rand()%4;

					if(random_num == 0)
					{
						building_group->color._r = 0;
						building_group->color._g = 0;
						building_group->color._b = 25;
						building_group->color._a = 110;
					}
					else if(random_num == 1)
					{
						building_group->color._r = 50;
						building_group->color._g = 50;
						building_group->color._b = 75;
						building_group->color._a = 110;
					}
					else if(random_num == 2)
					{
						building_group->color._r = 100;
						building_group->color._g = 100;
						building_group->color._b = 125;
						building_group->color._a = 110;
					}
					else
					{
						building_group->color._r = 150;
						building_group->color._g = 150;
						building_group->color._b = 175;
						building_group->color._a = 110;
					}

					//---- skip the bbdg ----
					i+=4;

					//---- skip the counter and number fields----
					
					i+=(2*4);


					// --- this skips buildings in a complicated fashion (get stories
					if((*((unsigned int *)&buffer[i])) < 0) push_coordinates_flag = false;
					height = ((*((unsigned int *)&buffer[i]))*3.5f + 5.0f/*+10.0f*/)*3.28084f;
					i+=4;

					// get units field to do coloring (temp solution) - skip alpha stored at the first position (argb)
					r = (unsigned char)buffer[i];
					g = (unsigned char)buffer[i+1];
					b = (unsigned char)buffer[i+2];
					if (r > 0) building_group->color._r = r;
					if (g > 0) building_group->color._g = g;
					if (b > 0) building_group->color._b = b;

					//---- skip to name field ----

					i+=(2*4+4*4);

					name_len=(unsigned char)buffer[i];

					i++;

					//---- skip name field ----

					i+=name_len;
		
					//---- skip tag ----

					i+=4;

					//---- read floats for top ----

					floats=(*((unsigned int *)&buffer[i]));

					i+=4;

					for(j=0;j<floats;j++,i+=4)
					{
						coord=(*((float *)&buffer[i]));

						if(j%2==0)
						{
							current_vertex->_x = coord*3.28084f;
						}
						else
						{
							current_vertex->_y = coord*3.28084f;
							current_vertex->_z = height;

							Scale_Coordinates<MT>(*current_vertex);
							push_coordinates_flag = push_coordinates_flag && !Clip_Coordinates<typename MasterType::canvas_type>(*current_vertex);
							++current_vertex;

							if(j%6==5)
							{
								++triangle_counter;
							}
						}
					}

					//---- skip tags ----

					i+=4;
					i+=4;

					//---- read floats for sides ----

					floats=(*((unsigned int *)&buffer[i]));

					i+=4;

					for(j=0;j<floats;j++,i+=4)
					{
						coord=(*((float *)&buffer[i]));

						if(j%3==0)
						{
							current_vertex->_x = coord*3.28084f;
							
						}
						else if(j%3==1)
						{
							current_vertex->_y = coord*3.28084f;
						}
						else
						{
							current_vertex->_z = coord*3.28084f;

							Scale_Coordinates<MT>(*current_vertex);

							push_coordinates_flag = push_coordinates_flag && !Clip_Coordinates<typename MasterType::canvas_type>(*current_vertex);

							++current_vertex;

							if(j%9==8)
							{
								++triangle_counter;
							}
						}
					}

					//---- skip tags ----

					i+=4;
					i+=4;

					//---- read floats for poly ----

					floats=(*((unsigned int *)&buffer[i]));

					i+=4;

					//---- skip poly ----

					i+=(4*floats);

					//---- skip tags ----

					i+=4;
					i+=4;

					building_group->num_primitives = triangle_counter;
					print_flag = false;
					
					if(push_coordinates_flag)
					{
						_buildings_layer->Push_Element<Regular_Element>(building_group);
					}
				}

				delete[] buffer;
		
				delete[] building_group->data;
				delete building_group;
			}

			static void Initialize_Type()
			{
				Antares_Layer_Configuration cfg;

				_buildings_layer=Allocate_New_Layer<MT>(string("Buildings"));
				
				True_Color_RGBA<NT> color;
				color._r = 0;
				color._g = 125;
				color._b = 255;
				color._a = 120;

				cfg.Configure_Static_Triangles(color,0);
				cfg.primitive_normal = true;
				cfg.draw = true;
				cfg.group_color = true;

				//cfg.head_texture = cfg.Add_Texture(string("C:\\opt\\polarisdeps\\antares\\HAR.png"));
				//cfg.grouped=false;
				//cfg.head_size_value=32;
				//cfg.head_accent_size_value=64;
				//cfg.selection_callback=&on_select;
				//cfg.double_click_callback=&on_double_click;
				//cfg.submission_callback=&on_submit;

				//cfg.head_color._r = 255;
				//cfg.head_color._g = 255;
				//cfg.head_color._b = 255;
				//cfg.head_color._a = 255;

				_buildings_layer->Initialize<NULLTYPE>(cfg);
			}

			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,buildings_layer, NONE, NONE);
		};

		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Buildings_Implementation<MasterType,InheritanceList>::_buildings_layer;
	}
}


using namespace Buildings_Components::Implementations;

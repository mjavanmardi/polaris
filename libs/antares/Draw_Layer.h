//*********************************************************
//	Draw_Layer.h - Function for drawing a general layer
//*********************************************************

#pragma once
#include "Canvas_Implementation.h"

//---------------------------------------------------------
//	Draw_Layer
//--------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::Draw_Layer(int start_iteration, int end_iteration, Antares_Layer_Interface* layer)
{
	

	const bool draw=layer->draw<bool>();

	// skip hidden layers
	if(!draw) return;

	glPushAttrib ( GL_ALL_ATTRIB_BITS );


	const PrimitiveType primitive_type=layer->primitive_type<PrimitiveType>();	


	const True_Color_RGBA<MasterType> head_color=layer->head_color<True_Color_RGBA<MasterType>>();
	const Point_3D<MasterType> head_normal=layer->head_normal<Point_3D<MasterType>>();
	const int head_size_value=layer->head_size_value<int>();
	
	const pixel_size_callback_type pixel_size_callback = layer->pixel_size_callback<pixel_size_callback_type>();
	const pixel_size_callback_type accent_pixel_size_callback = layer->accent_pixel_size_callback<pixel_size_callback_type>();

	const int head_accent_size_value=layer->head_accent_size_value<int>();
	
	const int head_texture=layer->head_texture<int>();

	const bool grouped=layer->grouped<bool>();
	const bool group_color=layer->group_color<bool>();
	const bool group_normal=layer->group_normal<bool>();
	const bool group_texture=layer->group_texture<bool>();

	const bool poly=layer->poly<bool>();

	const bool primitive_color=layer->primitive_color<bool>();
	const bool primitive_normal=layer->primitive_normal<bool>();
	const bool primitive_texture=layer->primitive_texture<bool>();
	const int primitive_stride = layer->primitive_stride<int>();

	const int vert_size = layer->vert_size<int>();
	const int vert_stride = layer->vert_stride<int>();

	const int data_stride=layer->data_stride<int>();


	const bool enable_textures = head_texture || primitive_texture || group_texture;
	const bool texture_coordinates = enable_textures && (primitive_type != _POINT && primitive_type != _LINE);

	const std::vector<int>& texture_map=layer->texture_map< const std::vector<int>& > ();
	const float texture_coordinates_map_x[4] = {0.0f,1.0f,1.0f,0.0f};
	const float texture_coordinates_map_y[4] = {1.0f,1.0f,0.0f,0.0f};


	const float* texture_counter_x = &texture_coordinates_map_x[0];
	const float* texture_counter_y = &texture_coordinates_map_y[0];

	if(enable_textures)
	{
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
		glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	}

	if(head_texture)
	{
		glBindTexture(GL_TEXTURE_2D, texture_map[ head_texture ] );
	}

	if(primitive_normal)
	{
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		/*if(!grouped) */glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	}

	const Dynamic_Multi_Buffer< std::vector<int>* >& storage=layer->storage<Dynamic_Multi_Buffer< std::vector<int>* >&>();

	//---- draw main layer ----

	switch(primitive_type)
	{
	case _POINT:
		if(enable_textures)
		{
			glDepthFunc(GL_ALWAYS);
			glEnable(GL_POINT_SPRITE);
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
		}
		if(pixel_size_callback!=nullptr) glPointSize((*pixel_size_callback)(_meters_per_pixel_mid));
		else glPointSize(head_size_value);
		glBegin(GL_POINTS);
		break;
	case _LINE:
		if(enable_textures)
		{
			glEnable(GL_POINT_SPRITE);
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
		}
		if(pixel_size_callback!=nullptr) glLineWidth((*pixel_size_callback)(_meters_per_pixel_mid));
		else glLineWidth(head_size_value);
		glBegin(GL_LINES);
		break;
	case _TRIANGLE:
		glBegin(GL_TRIANGLES);
		break;
	case _QUAD:
		glBegin(GL_QUADS);
		break;
	case _POLYGON:
		break;
	default:
		assert(false);
		break;
	};

	if(!poly)
	{
		glColor4ubv((GLubyte*)&head_color);
		glNormal3fv((GLfloat*)&head_normal);
	}

	int current_iteration=start_iteration;

	while(current_iteration <= end_iteration)
	{
		const std::vector<int>* geometry_by_thread = storage[current_iteration];

		for(unsigned int i=0;i<num_antares_threads() && i < geometry_by_thread->size() ;i++)
		{
			const unsigned char* geometry_itr = (const unsigned char*)&geometry_by_thread[i].front();
			const unsigned char* const geometry_end = geometry_itr+geometry_by_thread[i].size()*sizeof(int);

			while(geometry_itr != geometry_end)
			{
				geometry_itr += data_stride;

				if(grouped)
				{
					if(poly)
					{
						glBegin(GL_POLYGON);
						glColor4ubv((GLubyte*)&head_color);
						glNormal3fv((GLfloat*)&head_normal);
					}

					if(group_color)
					{
						glColor4ubv((GLubyte*)geometry_itr);
						geometry_itr += sizeof(True_Color_RGBA<MasterType>);
					}

					if(group_normal)
					{
						glNormal3fv((GLfloat*)geometry_itr);
						geometry_itr += sizeof(Point_3D<MasterType>);
					}

					//if(group_texture)
					//{
					//	glBindTexture(GL_TEXTURE_2D, texture_map[ *((int*)geometry_itr) ] );
					//	geometry_itr += sizeof(int);
					//}

					const int num_group_primitives=*((int*)geometry_itr);

					geometry_itr += sizeof(int);

					const unsigned char* const group_end = geometry_itr + primitive_stride * num_group_primitives;

					while( geometry_itr != group_end )
					{
						if(primitive_color)
						{
							glColor4ubv((GLubyte*)geometry_itr);
							geometry_itr += sizeof(True_Color_RGBA<MasterType>);
						}

						if(primitive_normal)
						{
							glNormal3fv((GLfloat*)geometry_itr);
							geometry_itr += sizeof(Point_3D<MasterType>);
						}

						//if(primitive_texture)
						//{
						//	glBindTexture(GL_TEXTURE_2D, texture_map[ *((int*)geometry_itr) ] );
						//	geometry_itr += sizeof(int);
						//}

						const unsigned char* const geometry_vert_end = geometry_itr + vert_stride;

						while( geometry_itr != geometry_vert_end )
						{
							if(texture_coordinates)
							{
								glTexCoord2f( *texture_counter_x, *texture_counter_y );
								++texture_counter_x; ++texture_counter_y;
							}

							glVertex3fv((GLfloat*)geometry_itr);
							geometry_itr += vert_size;
						}

						if(texture_coordinates)
						{
							texture_counter_x = &texture_coordinates_map_x[0];
							texture_counter_y = &texture_coordinates_map_y[0];
						}
					}

					if(poly)
					{
						glEnd();
					}
				}
				else
				{
					if(primitive_color)
					{
						glColor4ubv((GLubyte*)geometry_itr);
						geometry_itr += sizeof(True_Color_RGBA<MasterType>);
					}

					if(primitive_normal)
					{
						glNormal3fv((GLfloat*)geometry_itr);
						geometry_itr += sizeof(Point_3D<MasterType>);
					}
					//TODO: uncommented this to see if I could get textures working
					if(primitive_texture)
					{
						glEnd();
						glDisable (GL_DEPTH_TEST);
						glBindTexture(GL_TEXTURE_2D,texture_map[ *((int*)geometry_itr) ]); // geometry_itr here is the tex_id stored with the object
						glColor4f ( 1.0, 1.0, 1.0, 1.0 );
						glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
						glEnable(GL_BLEND);
						//glEnable (GL_DEPTH_TEST);
						glBegin(GL_QUADS);
						//cout << "Drawing for texture id="<<texture_map[ *((int*)geometry_itr) ]<<endl;
						geometry_itr += sizeof(int);
					}

					const unsigned char* const geometry_vert_end = geometry_itr + vert_stride;

					while( geometry_itr != geometry_vert_end )
					{
						if(texture_coordinates)
						{
							glTexCoord2f( *texture_counter_x, *texture_counter_y );
							//cout <<"Texture counters: "<<*texture_counter_x <<", " <<*texture_counter_y<<endl;
							++texture_counter_x; ++texture_counter_y;
						}

						glVertex3fv((GLfloat*)geometry_itr);
						//if(primitive_texture) cout <<"Vertex(x,y,z): "<<*((float*)&geometry_itr[0])<<","<<*((float*)&geometry_itr[4])<<","<<*((float*)&geometry_itr[8])<<endl;
						geometry_itr += vert_size;
					}

					if(texture_coordinates)
					{
						texture_counter_x = &texture_coordinates_map_x[0];
						texture_counter_y = &texture_coordinates_map_y[0];
					}
				}

			}
		}

		current_iteration++;
	}

	if(!poly)
	{
		glEnd();
	}

	glDepthFunc(GL_LEQUAL);

	//---- draw layer accents in the second pass ----

	const Dynamic_Multi_Buffer< std::vector<int>* >& accent_storage=layer->accent_storage<Dynamic_Multi_Buffer< std::vector<int>* >&>();
	
	switch(primitive_type)
	{
	case _POINT:
		//glPointSize( ceil(5.0f*(float)head_size_value) );
		if(accent_pixel_size_callback!=nullptr) glPointSize((*accent_pixel_size_callback)(_meters_per_pixel_mid));
		else glPointSize(head_accent_size_value);
		glBegin(GL_POINTS);
		break;
	case _LINE:
		//glLineWidth( ceil(5.0f*(float)head_size_value) );
		if(accent_pixel_size_callback!=nullptr) glLineWidth((*accent_pixel_size_callback)(_meters_per_pixel_mid));
		else glLineWidth(head_accent_size_value);
		glBegin(GL_LINES);
		break;
	case _TRIANGLE:
		glBegin(GL_TRIANGLES);
		break;
	case _QUAD:
		glBegin(GL_QUADS);
		break;
	case _POLYGON:
		break;
	default:
		assert(false);
		break;
	};

	True_Color_RGBA<MasterType> accent_color;
	accent_color = head_color;
	//accent_color._r = 255 - accent_color._r;
	//accent_color._g = 255 - accent_color._g;
	//accent_color._b = 255 - accent_color._b;

	if(!poly)
	{
		glColor4ubv((GLubyte*)&accent_color);
		glNormal3fv((GLfloat*)&head_normal);
	}
	
	current_iteration = start_iteration;
	
	while(current_iteration <= end_iteration)
	{
		const std::vector<int>* geometry_by_thread = accent_storage[current_iteration];

		for(unsigned int i=0;i<num_antares_threads() && i<geometry_by_thread->size();i++)
		{
			const unsigned char* geometry_itr = (const unsigned char*)&geometry_by_thread[i].front();
			const unsigned char* const geometry_end = geometry_itr+geometry_by_thread[i].size()*sizeof(int);

			while(geometry_itr != geometry_end)
			{
				geometry_itr += data_stride;

				if(grouped)
				{
					if(poly)
					{
						glBegin(GL_POLYGON);
						glColor4ubv((GLubyte*)&head_color);
						glNormal3fv((GLfloat*)&head_normal);
					}

					if(group_color)
					{
						glColor4ubv((GLubyte*)geometry_itr);
						geometry_itr += sizeof(True_Color_RGBA<MasterType>);
					}

					if(group_normal)
					{
						glNormal3fv((GLfloat*)geometry_itr);
						geometry_itr += sizeof(Point_3D<MasterType>);
					}
					
					//if(group_texture)
					//{
					//	glBindTexture(GL_TEXTURE_2D, texture_map[ *((int*)geometry_itr) ] );
					//	geometry_itr += sizeof(int);
					//}

					const int num_group_primitives=*((int*)geometry_itr);
					geometry_itr += sizeof(int);

					const unsigned char* const group_end = geometry_itr + primitive_stride * num_group_primitives;

					while( geometry_itr != group_end )
					{
						if(primitive_color)
						{
							glColor4ubv((GLubyte*)geometry_itr);
							geometry_itr += sizeof(True_Color_RGBA<MasterType>);
						}

						if(primitive_normal)
						{
							glNormal3fv((GLfloat*)geometry_itr);
							geometry_itr += sizeof(Point_3D<MasterType>);
						}

						//if(primitive_texture)
						//{
						//	glBindTexture(GL_TEXTURE_2D, texture_map[ *((int*)geometry_itr) ] );
						//	geometry_itr += sizeof(int);
						//}

						const unsigned char* const geometry_vert_end = geometry_itr + vert_stride;

						while( geometry_itr != geometry_vert_end )
						{
							if(texture_coordinates)
							{
								glTexCoord2f( *texture_counter_x, *texture_counter_y );

								++texture_counter_x; ++texture_counter_y;
							}

							glVertex3fv((GLfloat*)geometry_itr);
							geometry_itr += vert_size;
						}

						if(texture_coordinates)
						{
							texture_counter_x = &texture_coordinates_map_x[0];
							texture_counter_y = &texture_coordinates_map_y[0];
						}
					}

					if(poly)
					{
						glEnd();
					}
				}
				else
				{
					if(primitive_color)
					{
						glColor4ubv((GLubyte*)geometry_itr);
						geometry_itr += sizeof(True_Color_RGBA<MasterType>);
					}

					if(primitive_normal)
					{
						glNormal3fv((GLfloat*)geometry_itr);
						geometry_itr += sizeof(Point_3D<MasterType>);
					}
					
					//if(primitive_texture)
					//{
					//	glBindTexture(GL_TEXTURE_2D, texture_map[ *((int*)geometry_itr) ] );
					//	geometry_itr += sizeof(int);
					//}

					const unsigned char* const geometry_vert_end = geometry_itr + vert_stride;

					while( geometry_itr != geometry_vert_end )
					{
						if(texture_coordinates)
						{
							glTexCoord2f( *texture_counter_x, *texture_counter_y );

							++texture_counter_x; ++texture_counter_y;
						}

						glVertex3fv((GLfloat*)geometry_itr);
						geometry_itr += vert_size;
					}

					if(texture_coordinates)
					{
						texture_counter_x = &texture_coordinates_map_x[0];
						texture_counter_y = &texture_coordinates_map_y[0];
					}
				}
			}
		}

		current_iteration++;
	}

	if(!poly)
	{
		glEnd();
	}

	if(primitive_normal)
	{
		/*if(!grouped) */glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}
	
	if(enable_textures)
	{
		glDisable(GL_TEXTURE_2D);

		if(primitive_type == _POINT)
		{
			glDepthFunc(GL_LESS);
			glDisable(GL_POINT_SPRITE);
		}
	}

	glDepthFunc(GL_LESS);

	glPopAttrib ();
}

//*********************************************************
//	Draw_Layer.h - Function for drawing a general layer
//*********************************************************

#pragma once
#include "Canvas_Implementation.h"

//---------------------------------------------------------
//	Draw_Layer
//--------------------------------------------------------

template<typename MasterType,typename ParentType>
void Canvas_Implementation<MasterType,ParentType>::Draw_Layer(int start_iteration, int end_iteration, Antares_Layer_Interface* layer)
{
	const bool draw=layer->draw<bool>();

	// skip hidden layers
	if(!draw) return;

	const PrimitiveType primitive_type=layer->primitive_type<PrimitiveType>();	

	const bool color=layer->color<bool>();
	const True_Color_RGBA<MasterType> head_color=layer->head_color<True_Color_RGBA<MasterType>>();

	const bool normal=layer->normal<bool>();
	const Point_3D<MasterType> head_normal=layer->head_normal<Point_3D<MasterType>>();

	const int head_size_value=layer->head_size_value<int>();

	const int vert_stride=layer->num_vertices<int>() * sizeof(Point_3D<MasterType>);
	
	const int data_stride=layer->data_stride<int>();
	
	const Dynamic_Multi_Buffer< vector<unsigned char>[_num_threads] >& storage=layer->storage<Dynamic_Multi_Buffer< vector<unsigned char>[_num_threads] >&>();
	
	switch(primitive_type)
	{
	case _POINT:
		glPointSize(head_size_value);
		glBegin(GL_POINTS);
		break;
	case _LINE:
		glLineWidth(head_size_value);
		glBegin(GL_LINES);
		break;
	case _TRIANGLE:
		glBegin(GL_TRIANGLES);
		break;
	case _QUAD:
		glBegin(GL_QUADS);
		break;
	default:
		assert(false);
		break;
	};

	glColor4ubv((GLubyte*)&head_color);
	glNormal3fv((GLfloat*)&head_normal);

	int current_iteration=start_iteration;
	
	while(current_iteration <= end_iteration)
	{
		const vector<unsigned char> (&geometry_by_thread)[_num_threads] = storage[current_iteration];

		for(int i=0;i<_num_threads;i++)
		{
			const unsigned char* geometry_itr = &geometry_by_thread[i].front();
			const unsigned char* const geometry_end = geometry_itr+geometry_by_thread[i].size();

			while(geometry_itr != geometry_end)
			{
				if(color)
				{
					glColor4ubv((GLubyte*)geometry_itr);
					geometry_itr += sizeof(True_Color_RGBA<MasterType>);
				}

				if(normal)
				{
					glNormal3fv((GLfloat*)geometry_itr);
					geometry_itr += sizeof(Point_3D<MasterType>);
				}

				const unsigned char* const geometry_vert_end = geometry_itr + vert_stride;

				while( geometry_itr != geometry_vert_end )
				{
					glVertex3fv((GLfloat*)geometry_itr);
					geometry_itr += sizeof(Point_3D<MasterType>);
				}

				geometry_itr += data_stride;
			}
		}

		current_iteration++;
	}

	glEnd();
}
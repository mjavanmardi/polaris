//*********************************************************
//	Identify.h - Primary Identification Functions
//*********************************************************

#pragma once
#include "Antares_Layer_Implementation.h"

//---------------------------------------------------------
//	Identify
//--------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
template<typename ComponentType,typename CallerType,typename TargetType>
void Antares_Layer_Implementation<MasterType,ParentType,InheritanceList>::Identify(const Point_3D<MasterType>& point, int start_iteration, int end_iteration)
{
	if(_primitive_type==_POINT)
	{
		const bool color = _color;
		const bool normal = _normal;
		const int vert_stride = _num_vertices * sizeof(Point_3D<MasterType>);
		const int data_stride = _data_stride;
		int current_iteration = start_iteration;
		const PrimitiveType primitive_type = _primitive_type;	
		float best_dist = FLT_MAX;
		unsigned char* best_element = nullptr;

		while(current_iteration <= end_iteration)
		{
			const vector<unsigned char> (&geometry_by_thread)[_num_antares_threads] = _storage[current_iteration];

			for(int i=0;i<_num_antares_threads;i++)
			{
				const unsigned char* geometry_itr = &geometry_by_thread[i].front();
				const unsigned char* const geometry_end = geometry_itr+geometry_by_thread[i].size();

				while(geometry_itr != geometry_end)
				{
					const unsigned char* const geometry_head = geometry_itr;

					if(color)
					{
						geometry_itr += sizeof(True_Color_RGBA<MasterType>);
					}

					if(normal)
					{
						geometry_itr += sizeof(Point_3D<MasterType>);
					}

					const unsigned char* const geometry_vert_end = geometry_itr + vert_stride;

					while( geometry_itr != geometry_vert_end )
					{
						Point_3D<MasterType>* current = (Point_3D<MasterType>*) geometry_itr;

						float dist = (current->_x - point._x)*(current->_x - point._x) + (current->_y - point._y)*(current->_y - point._y);

						if(dist < best_dist)
						{
							best_dist = dist;
							best_element = (unsigned char*)geometry_head;
						}

						geometry_itr += sizeof(Point_3D<MasterType>);
					}

					geometry_itr += data_stride;
				}
			}

			current_iteration++;
		}

		if(best_element!=nullptr)
		{
			current_iteration = start_iteration;

			while(current_iteration <= end_iteration)
			{
				vector<unsigned char> (&geometry_by_thread)[_num_antares_threads] = _accent_storage[current_iteration];

				for(int i=0;i<_num_antares_threads;i++)
				{
					geometry_by_thread[i].clear();
				}

				current_iteration++;
			}

			Push_Element<ComponentType,CallerType,Accented_Element>(best_element,_element_size,start_iteration);
		}
	}
}

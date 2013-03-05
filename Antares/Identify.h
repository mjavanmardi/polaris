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
		const bool grouped = _grouped;
		const bool group_color = _group_color;
		const bool group_normal = _group_normal;
		
		const bool primitive_color = _primitive_color;
		const bool primitive_normal = _primitive_normal;
		const int primitive_stride = _primitive_stride;
		
		const int vert_stride = _vert_stride;
		const int data_stride = _data_stride;

		float best_dist = FLT_MAX;
		unsigned char* best_element = nullptr;

		

		int current_iteration=start_iteration;
		
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
					
					geometry_itr += data_stride;

					if(grouped)
					{
						if(group_color)
						{
							geometry_itr += sizeof(True_Color_RGBA<MasterType>);
						}

						if(group_normal)
						{
							geometry_itr += sizeof(Point_3D<MasterType>);
						}

						const int num_group_primitives=*((int*)geometry_itr);
						geometry_itr += sizeof(int);

						const unsigned char* const group_end = geometry_itr + primitive_stride * num_group_primitives;

						while( geometry_itr != group_end )
						{
							if(primitive_color)
							{
								geometry_itr += sizeof(True_Color_RGBA<MasterType>);
							}

							if(primitive_normal)
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
						}
					}
					else
					{
						if(primitive_color)
						{
							geometry_itr += sizeof(True_Color_RGBA<MasterType>);
						}

						if(primitive_normal)
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
					}
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

			Push_Element<ComponentType,CallerType,Accented_Element>(best_element,start_iteration);
			
			_selected_element = *((void**)best_element);

			if(_attributes_callback != nullptr)
			{
				vector<string> bucket;

				_attributes_callback( *((void**)best_element), bucket );
				
				_attributes_panel->Push_Attributes<Target_Type<NT,NT,vector<string>&>>(bucket);
			}
		}
	}
	else if(_primitive_type==_LINE)
	{
		const bool grouped = _grouped;
		const bool group_color = _group_color;
		const bool group_normal = _group_normal;
		
		const bool primitive_color = _primitive_color;
		const bool primitive_normal = _primitive_normal;
		const int primitive_stride = _primitive_stride;
		
		const int vert_stride = _vert_stride;
		const int data_stride = _data_stride;
		
		float best_dist = FLT_MAX;
		unsigned char* best_element = nullptr;

		int current_iteration=start_iteration;
		
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
					
					geometry_itr += data_stride;

					if(grouped)
					{
						//Note: untested code

						if(group_color)
						{
							geometry_itr += sizeof(True_Color_RGBA<MasterType>);
						}

						if(group_normal)
						{
							geometry_itr += sizeof(Point_3D<MasterType>);
						}

						const int num_group_primitives=*((int*)geometry_itr);
						geometry_itr += sizeof(int);

						const unsigned char* const group_end = geometry_itr + primitive_stride * num_group_primitives;

						while( geometry_itr != group_end )
						{
							if(primitive_color)
							{
								geometry_itr += sizeof(True_Color_RGBA<MasterType>);
							}

							if(primitive_normal)
							{
								geometry_itr += sizeof(Point_3D<MasterType>);
							}

							const unsigned char* const geometry_vert_end = geometry_itr + vert_stride;

							while( geometry_itr != geometry_vert_end )
							{
								float dist=FLT_MAX;
							
								Point_Segment_Distance( *((Point_3D<NULLTYPE>*) geometry_itr), *((Point_3D<NULLTYPE>*) (geometry_itr+sizeof(Point_3D<MasterType>))), (Point_3D<NULLTYPE>&)point, dist);

								if(dist < best_dist)
								{
									best_dist = dist;
									best_element = (unsigned char*)geometry_head;
								}

								geometry_itr += (sizeof(Point_3D<MasterType>)*2);
							}
						}
					}
					else
					{
						if(primitive_color)
						{
							geometry_itr += sizeof(True_Color_RGBA<MasterType>);
						}

						if(primitive_normal)
						{
							geometry_itr += sizeof(Point_3D<MasterType>);
						}
						
						const unsigned char* const geometry_vert_end = geometry_itr + vert_stride;

						while( geometry_itr != geometry_vert_end )
						{
							float dist=FLT_MAX;
							
							Point_Segment_Distance( *((Point_3D<NULLTYPE>*) geometry_itr), *((Point_3D<NULLTYPE>*) (geometry_itr+sizeof(Point_3D<MasterType>))), (Point_3D<NULLTYPE>&)point, dist);

							if(dist < best_dist)
							{
								best_dist = dist;
								best_element = (unsigned char*)geometry_head;
							}

							geometry_itr += (sizeof(Point_3D<MasterType>)*2);
						}
					}
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

			Push_Element<ComponentType,CallerType,Accented_Element>(best_element,start_iteration);
			
			_selected_element = *((void**)best_element);

			if(_attributes_callback != nullptr)
			{
				vector<string> bucket;

				_attributes_callback( *((void**)best_element), bucket );

				_attributes_panel->Push_Attributes<Target_Type<NT,NT,vector<string>&>>(bucket);
			}
		}
	}
	else if(_primitive_type==_QUAD)
	{
		const bool grouped = _grouped;
		const bool group_color = _group_color;
		const bool group_normal = _group_normal;
		
		const bool primitive_color = _primitive_color;
		const bool primitive_normal = _primitive_normal;
		const int primitive_stride = _primitive_stride;
		
		const int vert_stride = _vert_stride;
		const int data_stride = _data_stride;

		unsigned char* best_element = nullptr;
		bool found=false;
		
		vector<Point_3D<MasterType>> polygon;

		int current_iteration=start_iteration;
		
		while(current_iteration <= end_iteration)
		{
			if(found) break;

			const vector<unsigned char> (&geometry_by_thread)[_num_antares_threads] = _storage[current_iteration];

			for(int i=0;i<_num_antares_threads;i++)
			{
				if(found) break;

				const unsigned char* geometry_itr = &geometry_by_thread[i].front();
				const unsigned char* const geometry_end = geometry_itr+geometry_by_thread[i].size();

				while(geometry_itr != geometry_end)
				{
					if(found) break;

					const unsigned char* const geometry_head = geometry_itr;
					
					geometry_itr += data_stride;

					if(grouped)
					{
						if(group_color)
						{
							geometry_itr += sizeof(True_Color_RGBA<MasterType>);
						}

						if(group_normal)
						{
							geometry_itr += sizeof(Point_3D<MasterType>);
						}

						const int num_group_primitives=*((int*)geometry_itr);
						geometry_itr += sizeof(int);

						const unsigned char* const group_end = geometry_itr + primitive_stride * num_group_primitives;

						while( geometry_itr != group_end )
						{
							if(primitive_color)
							{
								geometry_itr += sizeof(True_Color_RGBA<MasterType>);
							}

							if(primitive_normal)
							{
								geometry_itr += sizeof(Point_3D<MasterType>);
							}

							const unsigned char* const geometry_vert_end = geometry_itr + vert_stride;

							while( geometry_itr != geometry_vert_end )
							{
								Point_3D<MasterType>* current = (Point_3D<MasterType>*) geometry_itr;

								polygon.push_back(*current);

								geometry_itr += sizeof(Point_3D<MasterType>);
							}

							if(In_Polygon((vector<Point_3D<NULLTYPE>>&)polygon,point._x,point._y))
							{
								best_element = (unsigned char*)geometry_head;
								found = true;
								break;
							}

							polygon.clear();
						}
					}
					else
					{
						if(primitive_color)
						{
							geometry_itr += sizeof(True_Color_RGBA<MasterType>);
						}

						if(primitive_normal)
						{
							geometry_itr += sizeof(Point_3D<MasterType>);
						}

						const unsigned char* const geometry_vert_end = geometry_itr + vert_stride;

						while( geometry_itr != geometry_vert_end )
						{
							Point_3D<MasterType>* current = (Point_3D<MasterType>*) geometry_itr;

							polygon.push_back(*current);

							geometry_itr += sizeof(Point_3D<MasterType>);
						}

						if(In_Polygon((vector<Point_3D<NULLTYPE>>&)polygon,point._x,point._y))
						{
							best_element = (unsigned char*)geometry_head;
							found = true;
							break;
						}

						polygon.clear();
					}
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

			Push_Element<ComponentType,CallerType,Accented_Element>(best_element,start_iteration);
			
			_selected_element = *((void**)best_element);

			if(_attributes_callback != nullptr)
			{
				vector<string> bucket;

				_attributes_callback( *((void**)best_element), bucket );
				
				_attributes_panel->Push_Attributes<Target_Type<NT,NT,vector<string>&>>(bucket);
			}
		}
	}
	else if(_primitive_type==_POLYGON)
	{
		const bool grouped = _grouped;
		const bool group_color = _group_color;
		const bool group_normal = _group_normal;
		
		const bool primitive_color = _primitive_color;
		const bool primitive_normal = _primitive_normal;
		const int primitive_stride = _primitive_stride;
		
		const int vert_stride = _vert_stride;
		const int data_stride = _data_stride;

		unsigned char* best_element = nullptr;
		bool found=false;
		
		vector<Point_3D<MasterType>> polygon;

		int current_iteration=start_iteration;
		
		while(current_iteration <= end_iteration)
		{
			if(found) break;

			const vector<unsigned char> (&geometry_by_thread)[_num_antares_threads] = _storage[current_iteration];

			for(int i=0;i<_num_antares_threads;i++)
			{
				if(found) break;

				const unsigned char* geometry_itr = &geometry_by_thread[i].front();
				const unsigned char* const geometry_end = geometry_itr+geometry_by_thread[i].size();

				while(geometry_itr != geometry_end)
				{
					if(found) break;

					// assume grouped for polygon
					
					const unsigned char* const geometry_head = geometry_itr;
					
					geometry_itr += data_stride;

					if(group_color)
					{
						geometry_itr += sizeof(True_Color_RGBA<MasterType>);
					}

					if(group_normal)
					{
						geometry_itr += sizeof(Point_3D<MasterType>);
					}

					const int num_group_primitives=*((int*)geometry_itr);
					geometry_itr += sizeof(int);

					const unsigned char* const group_end = geometry_itr + primitive_stride * num_group_primitives;

					while( geometry_itr != group_end )
					{
						if(primitive_color)
						{
							geometry_itr += sizeof(True_Color_RGBA<MasterType>);
						}

						if(primitive_normal)
						{
							geometry_itr += sizeof(Point_3D<MasterType>);
						}

						const unsigned char* const geometry_vert_end = geometry_itr + vert_stride;

						while( geometry_itr != geometry_vert_end )
						{
							Point_3D<MasterType>* current = (Point_3D<MasterType>*) geometry_itr;

							polygon.push_back(*current);

							geometry_itr += sizeof(Point_3D<MasterType>);
						}
					}

					if(In_Polygon((vector<Point_3D<NULLTYPE>>&)polygon,point._x,point._y))
					{
						best_element = (unsigned char*)geometry_head;
						found = true;
						break;
					}

					polygon.clear();
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

			Push_Element<ComponentType,CallerType,Accented_Element>(best_element,start_iteration);
			
			_selected_element = *((void**)best_element);

			if(_attributes_callback != nullptr)
			{
				vector<string> bucket;

				_attributes_callback( *((void**)best_element), bucket );

				_attributes_panel->Push_Attributes<Target_Type<NT,NT,vector<string>&>>(bucket);
			}
		}
	}
}
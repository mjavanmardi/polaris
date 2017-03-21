//*********************************************************
//	Identify.h - Primary Identification Functions
//*********************************************************

#pragma once
#include "Antares_Layer_Implementation.h"

//---------------------------------------------------------
//	Identify
//--------------------------------------------------------

template<typename MasterType,typename InheritanceList>
bool Antares_Layer_Implementation<MasterType,InheritanceList>::Identify_One(const Point_3D<MasterType>& point, int start_iteration, int end_iteration,ANTARES_SELECTION_MODE mode)
{
	unsigned char* best_element = nullptr;

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

		int current_iteration=start_iteration;
		
		while(current_iteration <= end_iteration)
		{
			const std::vector<int>* geometry_by_thread = _storage[current_iteration];

			for(int i=0;i<num_antares_threads();i++)
			{
				const unsigned char* geometry_itr = (const unsigned char*)&geometry_by_thread[i].front();
				const unsigned char* const geometry_end = geometry_itr+geometry_by_thread[i].size()*sizeof(int);

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

		int current_iteration=start_iteration;
		
		while(current_iteration <= end_iteration)
		{
			const std::vector<int>* geometry_by_thread = _storage[current_iteration];

			for(int i=0;i<num_antares_threads();i++)
			{
				const unsigned char* geometry_itr = (const unsigned char*)&geometry_by_thread[i].front();
				const unsigned char* const geometry_end = geometry_itr+geometry_by_thread[i].size()*sizeof(int);

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

		bool found=false;
		
		std::vector<Point_3D<MasterType>> polygon;

		int current_iteration=start_iteration;
		
		while(current_iteration <= end_iteration)
		{
			if(found) break;

			const std::vector<int>* geometry_by_thread = _storage[current_iteration];

			for(unsigned int i=0;i<num_antares_threads();i++)
			{
				if(found) break;

				const unsigned char* geometry_itr = (const unsigned char*)&geometry_by_thread[i].front();
				const unsigned char* const geometry_end = geometry_itr+geometry_by_thread[i].size()*sizeof(int);

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

							if(In_Polygon((std::vector<Point_3D<NULLTYPE>>&)polygon,point._x,point._y))
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

						if(In_Polygon((std::vector<Point_3D<NULLTYPE>>&)polygon,point._x,point._y))
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

		bool found=false;
		
		std::vector<Point_3D<MasterType>> polygon;

		int current_iteration=start_iteration;
		
		while(current_iteration <= end_iteration)
		{
			if(found) break;

			const std::vector<int>* geometry_by_thread = _storage[current_iteration];

			for(unsigned int i=0;i<num_antares_threads();i++)
			{
				if(found) break;

				const unsigned char* geometry_itr = (const unsigned char*)&geometry_by_thread[i].front();
				const unsigned char* const geometry_end = geometry_itr+geometry_by_thread[i].size()*sizeof(int);

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

					if(In_Polygon((std::vector<Point_3D<NULLTYPE>>&)polygon,point._x,point._y))
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
	}

	if(best_element!=nullptr/* && (void*)best_element!=_selected_element*/)
	{
		if(mode==ALT_DOWN)
		{
			for(std::list<void*>::iterator itr=_selected_elements.begin();itr!=_selected_elements.end();itr++)
			{
				if((*itr) == *((void**)best_element))
				{
					return false;
				}
			}

			_deselected_elements.clear();
			for(std::list<void*>::iterator itr=_selected_elements.begin();itr!=_selected_elements.end();itr++){ _deselected_elements.push_back( *itr ); }

			_added_elements.clear();
			_added_elements.push_back(*((void**)best_element));

			_selected_elements.clear();
			_selected_elements.push_back(*((void**)best_element));

			if(_selection_callback != nullptr)
			{
				std::vector<pair<string,string>> bucket;

				_selection_callback( _deselected_elements, _added_elements, _selected_elements, bucket );
				
				_attributes_panel->Push_Attributes(bucket);
			}
		}
		else if(mode==CTRL_DOWN)
		{
			_deselected_elements.clear();
			_added_elements.clear();

			for(std::list<void*>::iterator itr=_selected_elements.begin();itr!=_selected_elements.end();itr++)
			{
				if((*itr) == *((void**)best_element))
				{
					_deselected_elements.push_back( *itr );
					_selected_elements.erase(itr);
					break;
				}
			}
			
			if(!_deselected_elements.size())
			{
				_added_elements.push_back( *((void**)best_element) );
				_selected_elements.push_back( *((void**)best_element) );
			}

			if(_selection_callback != nullptr)
			{
				std::vector<pair<string,string>> bucket;

				_selection_callback( _deselected_elements, _added_elements, _selected_elements, bucket );


				_attributes_panel->Push_Attributes(bucket);
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}

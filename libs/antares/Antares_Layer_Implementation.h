//*********************************************************
//	Antares_Layer_Implementation.h - Graphical Layer
//*********************************************************

#pragma once
#include "Antares_Layer.h"

//---------------------------------------------------------
//	Antares_Layer_Implementation - layer definition
//---------------------------------------------------------

implementation struct Antares_Layer_Implementation:public Polaris_Component<MasterType,INHERIT(Antares_Layer_Implementation),Execution_Object>
{
	Antares_Layer_Implementation()
	{
		_draw=false;
		_layer_options=nullptr;
		_attributes_panel=nullptr;
		_pixel_size_callback=nullptr;
		_accent_pixel_size_callback=nullptr;
	}

	template<typename TargetType> void Initialize(Antares_Layer_Configuration& cfg)
	{
		_dynamic_data=cfg.dynamic_data;
		_target_sub_iteration=cfg.target_sub_iteration;

		//Load_Event<Antares_Layer_Implementation>(&Update_Condition<NULLTYPE>, &Update<NULLTYPE>, cfg.storage_offset, _target_sub_iteration);
		Load_Event<Antares_Layer_Implementation>(&Update, cfg.storage_offset, _target_sub_iteration);


		_storage.Initialize(cfg.storage_offset, cfg.storage_period, cfg.storage_size);

		for(unsigned int i=0;i<_storage.buffer_size;i++)
		{
			_storage.multi_buffer[i] = new std::vector<int>[num_antares_threads()];
		}

		_accent_storage.Initialize(cfg.storage_offset, cfg.storage_period, 1);
		
		for(unsigned int i=0;i<_accent_storage.buffer_size;i++)
		{
			_accent_storage.multi_buffer[i] = new std::vector<int>[num_antares_threads()];
		}

		_draw=cfg.draw;
		_primitive_type=cfg.primitive_type;

		_head_texture = cfg.head_texture;

		_default_texture_alpha = cfg.default_texture_alpha;

		_grouped=cfg.grouped;
			_group_color=cfg.group_color;
			_group_normal=cfg.group_normal;
			_group_texture=cfg.group_texture;

		_head_color._r = cfg.head_color._r;
		_head_color._g = cfg.head_color._g;
		_head_color._b = cfg.head_color._b;
		_head_color._a = cfg.head_color._a;
		
		_head_normal._x = cfg.head_normal._x;
		_head_normal._y = cfg.head_normal._y;
		_head_normal._z = cfg.head_normal._z;
		
		_head_size_value=cfg.head_size_value;
		
		_head_accent_size_value=cfg.head_accent_size_value;

		//_head_accent_color=*((True_Color_RGBA<MasterType>*)&cfg.head_accent_color);

		_primitive_color=cfg.primitive_color;
		_primitive_normal=cfg.primitive_normal;
		_primitive_texture=cfg.primitive_texture;

		_vert_size = sizeof(Point_3D<MasterType>);
		_poly = false;

		switch(_primitive_type)
		{
		case _PLOT:
			_x_label = cfg.x_label;
			_y_label = cfg.y_label;

			_primitive_normal = false;
			_primitive_color = false;
			assert(grouped == true);
			break;
		case _POINT:
			_vert_stride = _vert_size*1;
			_layer_options->Toggle_Named_Layer<NT>(_name,_draw);
			break;
		case _LINE:
			_vert_stride = _vert_size*2;
			_layer_options->Toggle_Named_Layer<NT>(_name,_draw);
			break;
		case _TRIANGLE:
			_vert_stride = _vert_size*3;
			_layer_options->Toggle_Named_Layer<NT>(_name,_draw);
			break;
		case _QUAD:
			_vert_stride = _vert_size*4;
			_layer_options->Toggle_Named_Layer<NT>(_name,_draw);
			break;
		case _POLYGON:
			_vert_stride = _vert_size*1;
			_layer_options->Toggle_Named_Layer<NT>(_name,_draw);
			_poly = true;
			assert(grouped == true);
			break;
		default:
			assert(false);
			break;
		};

		_primitive_stride = _vert_stride + _primitive_color*sizeof(True_Color_RGBA<MasterType>) + _primitive_normal*sizeof(Point_3D<MasterType>) + _primitive_texture*sizeof(int);

		_submission_callback=cfg.submission_callback;
		_selection_callback=cfg.selection_callback;
		_double_click_callback=cfg.double_click_callback;

		

		_accent_pixel_size_callback=cfg.accent_pixel_size_callback;
		_pixel_size_callback=cfg.pixel_size_callback;


		if(_selection_callback != nullptr || _submission_callback != nullptr || _double_click_callback) _data_stride=sizeof(void*);
		else _data_stride=0;

		_selected_elements.clear();
		_control_dialog = nullptr;


		Canvas<typename MasterType::canvas_type>* _canvas = (Canvas<typename MasterType::canvas_type>*)canvas;

		//Null texture
		_texture_map.push_back(1);

		for(std::vector<string>::iterator itr = cfg.textures.begin();itr!=cfg.textures.end();itr++)
		{
			wxImage tex_image;

			if(tex_image.LoadFile(*itr))
			{
				const bool has_alpha=tex_image.HasAlpha();

				const unsigned char* rgb_data = tex_image.GetData();
				const unsigned char* alpha_data = nullptr;
				if(has_alpha) alpha_data = tex_image.GetAlpha();

				const unsigned char* const rgb_end = rgb_data + tex_image.GetHeight()*tex_image.GetWidth()*3;

				unsigned char* rgba_data = new unsigned char[tex_image.GetHeight()*tex_image.GetWidth()*4];
				const unsigned char* const rgba_end = rgba_data + tex_image.GetHeight()*tex_image.GetWidth()*4;

				while(rgb_data!=rgb_end)
				{
					(*rgba_data)=(*rgb_data); ++rgb_data; ++rgba_data;
					(*rgba_data)=(*rgb_data); ++rgb_data; ++rgba_data;
					(*rgba_data)=(*rgb_data); ++rgb_data; ++rgba_data;
					if(has_alpha) {(*rgba_data)=(*alpha_data); ++alpha_data; ++rgba_data;}
					//TODO: change back to 255 - testing higher transparency for layers
					else {(*rgba_data)=_default_texture_alpha; ++rgba_data;}
				}
				
				rgba_data -= (tex_image.GetHeight()*tex_image.GetWidth()*4);

				int tex_id = _canvas->Build_Texture<NT>(tex_image.GetWidth(),tex_image.GetHeight(),rgba_data);

				delete[] rgba_data;

				_texture_map.push_back(tex_id);
				//TODO: remove when done testing
				//cout <<"Tex_id when reading layer "<<_name<<": "<<tex_id<<endl;
			}
			else
			{
				cout << "Unable to load texture: " << *itr << endl;

				_texture_map.push_back( -1 );

				exit(0);
			}
		}
	}

	template<typename TargetType> void Reschedule_Execution(int new_iteration)
	{
		Reschedule<Antares_Layer_Implementation>(new_iteration, _target_sub_iteration);
	}

	// Push user data to a thread specific buffer in OpenGL compatible format - used in draw_layer function
	template<typename TargetType> void Push_Element(void* data, int iteration, requires(TargetType,check_2(TargetType,Regular_Element,is_same)))
	{
		const int* geometry_itr=(const int*)data;

		const bool grouped = _grouped;
		const bool group_color = _group_color;
		const bool group_normal = _group_normal;
		const bool group_texture = _group_texture;

		const PrimitiveType primitive_type = _primitive_type;

		const bool primitive_color = _primitive_color;
		const bool primitive_normal = _primitive_normal;
		const bool primitive_texture = _primitive_texture;
		const int primitive_stride = (_vert_stride + _primitive_color*sizeof(True_Color_RGBA<MasterType>) + _primitive_texture*sizeof(int))/sizeof(int);
		
		const int vert_size = _vert_size/sizeof(int);
		const int vert_stride = _vert_stride/sizeof(int);
		const int data_stride = _data_stride/sizeof(int);

		Point_3D<NT> a;
		Point_3D<NT> b;
		Point_3D<NT> c;
		Point_3D<NT> result;

		int* const beg_write_itr = (int*)&result;
		const int* const end_write_itr = (int*)&result + sizeof(Point_3D<NT>)/sizeof(int);

		std::vector<int>* geometry_by_thread = _storage[iteration];

		std::vector<int>* const storage_reference=& (geometry_by_thread[thread_id()]);

		//std::vector<int>& storage_reference=_accent_storage[iteration][__thread_id];

		if(primitive_type == _PLOT)
		{
			PLFLT point_value;
			const int* end_data_itr = (const int*) ( ((int*)&point_value) + sizeof(PLFLT)/sizeof(int) );

			const int* const geometry_data_end = geometry_itr + data_stride;

			while( geometry_itr != geometry_data_end )
			{
				storage_reference->push_back(*geometry_itr);
				++geometry_itr;
			}

			if(group_color)
			{
				const int* end_data_itr = geometry_itr+sizeof(True_Color_RGBA<MasterType>)/sizeof(int);

				while(geometry_itr!=end_data_itr)
				{
					storage_reference->push_back(*geometry_itr);
					++geometry_itr;
				}
			}

			const int num_group_primitives=*((int*)geometry_itr);
			
			const int* end_size_itr = geometry_itr+sizeof(int)/sizeof(int);

			while(geometry_itr!=end_size_itr)
			{
				storage_reference->push_back(*geometry_itr);
				++geometry_itr;
			}

			geometry_itr=(*((int**)geometry_itr));
			const int* const group_begin = geometry_itr;
			const int* const group_end = geometry_itr + (sizeof(Point_2D<MasterType>)/sizeof(int)) * num_group_primitives;

			// Collect x points
			while( geometry_itr != group_end )
			{
				const int* const geometry_vert_end = geometry_itr + sizeof(Point_2D<MasterType>)/sizeof(int);

				point_value=(PLFLT)(*((float*)geometry_itr));
				
				geometry_itr = (int*)&point_value;

				while( geometry_itr!=end_data_itr )
				{
					storage_reference->push_back(*geometry_itr);
					++geometry_itr;
				}

				geometry_itr = (const int*)geometry_vert_end;
			}

			geometry_itr = group_begin;

			// Collect y points
			while( geometry_itr != group_end )
			{
				const int* const geometry_vert_end = geometry_itr + sizeof(Point_2D<MasterType>)/sizeof(int);

				geometry_itr += (sizeof(Point_2D<MasterType>)/sizeof(int))/2;

				point_value=(PLFLT)(*((float*)geometry_itr));

				geometry_itr = (int*)&point_value;

				while( geometry_itr!=end_data_itr )
				{
					storage_reference->push_back(*geometry_itr);
					++geometry_itr;
				}

				geometry_itr = (const int*)geometry_vert_end;
			}
		}
		else
		{
			if(grouped)
			{
				if(data_stride)
				{
					// Pushing back pointer to the polaris object that the graphical element stores
					// *Note - pushing 8-byte address as two ints
					storage_reference->push_back(*geometry_itr);
					++geometry_itr;
					storage_reference->push_back(*geometry_itr);
					++geometry_itr;

					//const int* const geometry_data_end = geometry_itr + data_stride;

					//while( geometry_itr != geometry_data_end )
					//{
					//	storage_reference->push_back(*geometry_itr);
					//	++geometry_itr;
					//}
				}

				if(group_color)
				{
					const int* end_data_itr = geometry_itr+sizeof(True_Color_RGBA<MasterType>)/sizeof(int);

					while(geometry_itr!=end_data_itr)
					{
						storage_reference->push_back(*geometry_itr);
						++geometry_itr;
					}
				}

				if(group_normal)
				{
					const int* end_data_itr = geometry_itr+sizeof(Point_3D<MasterType>)/sizeof(int);
					
					while(geometry_itr!=end_data_itr)
					{
						storage_reference->push_back(*geometry_itr);
						++geometry_itr;
					}
				}

				if(group_texture)
				{
					storage_reference->push_back(*geometry_itr);
					++geometry_itr;
				}

				const int num_group_primitives=*((int*)geometry_itr);

				storage_reference->push_back(*geometry_itr);
				++geometry_itr;

				//const int* end_size_itr = geometry_itr+sizeof(int)/sizeof(int);

				//while(geometry_itr!=end_size_itr)
				//{
				//	storage_reference->push_back(*geometry_itr);
				//	++geometry_itr;
				//}

				const int* const group_end = (*((int**)geometry_itr)) + primitive_stride * num_group_primitives;
				geometry_itr=(*((int**)geometry_itr));

				while( geometry_itr != group_end )
				{
					if(primitive_color)
					{
						storage_reference->push_back(*geometry_itr);
						++geometry_itr;

						//const int* end_data_itr = geometry_itr+sizeof(True_Color_RGBA<MasterType>)/sizeof(int);

						//while(geometry_itr!=end_data_itr)
						//{
						//	storage_reference->push_back(*geometry_itr);
						//	++geometry_itr;
						//}
					}

					if(primitive_normal)
					{
						const int* read_itr = geometry_itr;

						a = *((Point_3D<NT>*)read_itr);
						read_itr += sizeof(Point_3D<NT>)/sizeof(int);
						b = *((Point_3D<NT>*)read_itr);						
						read_itr += sizeof(Point_3D<NT>)/sizeof(int);
						c = *((Point_3D<NT>*)read_itr);

						Compute_Fast_Normal(a,b,c,result);
						
						const int* write_itr = (const int*)beg_write_itr;

						//while(write_itr!=end_write_itr)
						//{
						//	storage_reference->push_back(*write_itr);
						//	++write_itr;
						//}

						storage_reference->push_back(*write_itr);
						++write_itr;
						storage_reference->push_back(*write_itr);
						++write_itr;
						storage_reference->push_back(*write_itr);
						++write_itr;
					}

					if(primitive_texture)
					{
						storage_reference->push_back(*geometry_itr);
						++geometry_itr;
					}

					int siz = storage_reference->size();

					storage_reference->resize( siz + vert_stride );

					memcpy( &( storage_reference->at(siz) ), geometry_itr, vert_stride*sizeof(int) );

					geometry_itr += vert_stride;

					//const int* const geometry_vert_end = geometry_itr + vert_stride;

					//while( geometry_itr != geometry_vert_end )
					//{
					//	const int* end_data_itr = geometry_itr + vert_size;

					//	while(geometry_itr!=end_data_itr)
					//	{
					//		storage_reference->push_back(*geometry_itr);
					//		++geometry_itr;
					//	}
					//}
				}
			}
			else
			{
				if(data_stride)
				{
					storage_reference->push_back(*geometry_itr);
					++geometry_itr;
					storage_reference->push_back(*geometry_itr);
					++geometry_itr;

					//const int* const geometry_data_end = geometry_itr + data_stride;

					//while( geometry_itr != geometry_data_end )
					//{
					//	storage_reference->push_back(*geometry_itr);
					//	++geometry_itr;
					//}
				}

				const int* const group_end = geometry_itr + primitive_stride;

				while( geometry_itr != group_end )
				{
					if(primitive_color)
					{
						storage_reference->push_back(*geometry_itr);
						++geometry_itr;

						//const int* end_data_itr = geometry_itr+sizeof(True_Color_RGBA<MasterType>)/sizeof(int);

						//while(geometry_itr!=end_data_itr)
						//{
						//	storage_reference->push_back(*geometry_itr);
						//	++geometry_itr;
						//}
					}

					if(primitive_normal)
					{
						const int* read_itr = geometry_itr;

						a = *((Point_3D<NT>*)read_itr);
						read_itr += sizeof(Point_3D<NT>)/sizeof(int);
						b = *((Point_3D<NT>*)read_itr);						
						read_itr += sizeof(Point_3D<NT>)/sizeof(int);
						c = *((Point_3D<NT>*)read_itr);

						Compute_Fast_Normal(a,b,c,result);

						const int* write_itr = (const int*)beg_write_itr;

						//while(write_itr!=end_write_itr)
						//{
						//	storage_reference->push_back(*write_itr);
						//	++write_itr;
						//}

						storage_reference->push_back(*write_itr);
						++write_itr;
						storage_reference->push_back(*write_itr);
						++write_itr;
						storage_reference->push_back(*write_itr);
						++write_itr;
					}
					
					if(primitive_texture)
					{
						storage_reference->push_back(*geometry_itr);
						++geometry_itr;
					}

					int siz = storage_reference->size();

					storage_reference->resize( siz + vert_stride );

					memcpy( &( storage_reference->at(siz) ), geometry_itr, vert_stride*sizeof(int) );

					geometry_itr += vert_stride;

					//const int* const geometry_vert_end = geometry_itr + vert_stride;

					//while( geometry_itr != geometry_vert_end )
					//{
					//	const int* end_data_itr = geometry_itr+vert_size;

					//	while(geometry_itr!=end_data_itr)
					//	{
					//		storage_reference->push_back(*geometry_itr);
					//		++geometry_itr;
					//	}
					//}
				}
			}
		}
	}

	template<typename TargetType> void Push_Element(void* data, int iteration, requires(TargetType,check_2(TargetType,Accented_Element,is_same)))
	{
		const int* geometry_itr=(const int*)data;

		const bool grouped = _grouped;
		const bool group_color = _group_color;
		const bool group_normal = _group_normal;
		const bool group_texture = _group_texture;

		const PrimitiveType primitive_type = _primitive_type;

		const bool primitive_color = _primitive_color;
		const bool primitive_normal = _primitive_normal;
		const bool primitive_texture = _primitive_texture;

		const int primitive_stride = (_vert_stride + _primitive_color*sizeof(True_Color_RGBA<MasterType>) + _primitive_texture*sizeof(int))/sizeof(int);
		
		const int vert_size = _vert_size/sizeof(int);
		const int vert_stride = _vert_stride/sizeof(int);
		const int data_stride = _data_stride/sizeof(int);

		Point_3D<NT> a;
		Point_3D<NT> b;
		Point_3D<NT> c;
		Point_3D<NT> result;

		std::vector<int>& storage_reference=_accent_storage[iteration][__thread_id];

		if(primitive_type == _PLOT)
		{
			PLFLT point_value;
			const int* end_data_itr = (const int*) ( ((int*)&point_value) + sizeof(PLFLT)/sizeof(int) );

			const int* const geometry_data_end = geometry_itr + data_stride;

			while( geometry_itr != geometry_data_end )
			{
				storage_reference.push_back(*geometry_itr);
				++geometry_itr;
			}

			if(group_color)
			{
				const int* end_data_itr = geometry_itr+sizeof(True_Color_RGBA<MasterType>)/sizeof(int);

				while(geometry_itr!=end_data_itr)
				{
					storage_reference.push_back(*geometry_itr);
					++geometry_itr;
				}
			}

			const int num_group_primitives=*((int*)geometry_itr);
			
			const int* end_size_itr = geometry_itr+sizeof(int)/sizeof(int);

			while(geometry_itr!=end_size_itr)
			{
				storage_reference.push_back(*geometry_itr);
				++geometry_itr;
			}

			geometry_itr=(*((int**)geometry_itr));
			const int* const group_begin = geometry_itr;
			const int* const group_end = geometry_itr + sizeof(Point_2D<MasterType>)/sizeof(int) * num_group_primitives;

			// Collect x points
			while( geometry_itr != group_end )
			{
				const int* const geometry_vert_end = geometry_itr + sizeof(Point_2D<MasterType>)/sizeof(int);

				point_value=(PLFLT)(*((float*)geometry_itr));
				
				geometry_itr = (int*)&point_value;

				while( geometry_itr!=end_data_itr )
				{
					storage_reference.push_back(*geometry_itr);
					++geometry_itr;
				}

				geometry_itr = (const int*)geometry_vert_end;
			}

			geometry_itr = group_begin;

			// Collect y points
			while( geometry_itr != group_end )
			{
				const int* const geometry_vert_end = geometry_itr + sizeof(Point_2D<MasterType>)/sizeof(int);

				geometry_itr += (sizeof(Point_2D<MasterType>)/sizeof(int))/2;

				point_value=(PLFLT)(*((float*)geometry_itr));

				geometry_itr = (int*)&point_value;

				while( geometry_itr!=end_data_itr )
				{
					storage_reference.push_back(*geometry_itr);
					++geometry_itr;
				}

				geometry_itr = (const int*)geometry_vert_end;
			}
		}
		else
		{
			if(grouped)
			{
				const int* const geometry_data_end = geometry_itr + data_stride;

				while( geometry_itr != geometry_data_end )
				{
					storage_reference.push_back(*geometry_itr);
					++geometry_itr;
				}

				if(group_color)
				{
					const int* end_data_itr = geometry_itr+sizeof(True_Color_RGBA<MasterType>)/sizeof(int);

					while(geometry_itr!=end_data_itr)
					{
						storage_reference.push_back(*geometry_itr);
						++geometry_itr;
					}
				}

				if(group_normal)
				{
					const int* end_data_itr = geometry_itr+sizeof(Point_3D<MasterType>)/sizeof(int);
					
					while(geometry_itr!=end_data_itr)
					{
						storage_reference.push_back(*geometry_itr);
						++geometry_itr;
					}
				}
				
				if(group_texture)
				{
					storage_reference.push_back(*geometry_itr);
					++geometry_itr;
				}

				const int num_group_primitives=*((int*)geometry_itr);
				
				const int* end_size_itr = geometry_itr+sizeof(int)/sizeof(int);

				while(geometry_itr!=end_size_itr)
				{
					storage_reference.push_back(*geometry_itr);
					++geometry_itr;
				}

				const int* const group_end = (*((int**)geometry_itr)) + primitive_stride * num_group_primitives;
				geometry_itr=(*((int**)geometry_itr));

				while( geometry_itr != group_end )
				{
					if(primitive_color)
					{
						const int* end_data_itr = geometry_itr+sizeof(True_Color_RGBA<MasterType>)/sizeof(int);

						while(geometry_itr!=end_data_itr)
						{
							storage_reference.push_back(*geometry_itr);
							++geometry_itr;
						}
					}

					if(primitive_normal)
					{
						const int* read_itr = geometry_itr;

						a = *((Point_3D<NT>*)read_itr);
						read_itr += sizeof(Point_3D<NT>)/sizeof(int);
						b = *((Point_3D<NT>*)read_itr);						
						read_itr += sizeof(Point_3D<NT>)/sizeof(int);
						c = *((Point_3D<NT>*)read_itr);

						Compute_Fast_Normal(a,b,c,result);

						const int* write_itr = (const int*)&result;

						const int* end_data_itr = write_itr + sizeof(Point_3D<NT>)/sizeof(int);

						while(write_itr!=end_data_itr)
						{
							storage_reference.push_back(*write_itr);
							++write_itr;
						}
					}
					
					if(primitive_texture)
					{
						storage_reference.push_back(*geometry_itr);
						++geometry_itr;
					}

					const int* const geometry_vert_end = geometry_itr + vert_stride;

					while( geometry_itr != geometry_vert_end )
					{
						const int* end_data_itr = geometry_itr + vert_size;

						while(geometry_itr!=end_data_itr)
						{
							storage_reference.push_back(*geometry_itr);
							++geometry_itr;
						}
					}
				}
			}
			else
			{
				const int* const geometry_data_end = geometry_itr + data_stride;

				while( geometry_itr != geometry_data_end )
				{
					storage_reference.push_back(*geometry_itr);
					++geometry_itr;
				}

				const int* const group_end = geometry_itr + primitive_stride;

				while( geometry_itr != group_end )
				{
					if(primitive_color)
					{
						const int* end_data_itr = geometry_itr+sizeof(True_Color_RGBA<MasterType>)/sizeof(int);

						while(geometry_itr!=end_data_itr)
						{
							storage_reference.push_back(*geometry_itr);
							++geometry_itr;
						}
					}

					if(primitive_normal)
					{
						const int* read_itr = geometry_itr;

						a = *((Point_3D<NT>*)read_itr);
						read_itr += sizeof(Point_3D<NT>)/sizeof(int);
						b = *((Point_3D<NT>*)read_itr);						
						read_itr += sizeof(Point_3D<NT>)/sizeof(int);
						c = *((Point_3D<NT>*)read_itr);

						Compute_Fast_Normal(a,b,c,result);

						const int* write_itr = (const int*)&result;

						const int* end_data_itr = write_itr + sizeof(Point_3D<NT>)/sizeof(int);

						while(write_itr!=end_data_itr)
						{
							storage_reference.push_back(*write_itr);
							++write_itr;
						}
					}
					
					if(primitive_texture)
					{
						storage_reference.push_back(*geometry_itr);
						++geometry_itr;
					}

					const int* const geometry_vert_end = geometry_itr + vert_stride;

					while( geometry_itr != geometry_vert_end )
					{
						const int* end_data_itr = geometry_itr+vert_size;

						while(geometry_itr!=end_data_itr)
						{
							storage_reference.push_back(*geometry_itr);
							++geometry_itr;
						}
					}
				}
			}
		}
	}

	template<typename TargetType> void Push_Element(void* data, int size, int iteration,requires(TargetType,!check_2(TargetType,Regular_Element,is_same) && !check_2(TargetType,Accented_Element,is_same)) && !check_2(TargetType,Internal_Element,is_same))
	{
		assert_check_2(TargetType,Regular_Element,is_same,"Not a recognizable element category! Available options are Regular_Element, Internal_Element or Accented_Element.");
		assert_check_2(TargetType,Accented_Element,is_same,"Not a recognizable element category! Available options are Regular_Element, Internal_Element or Accented_Element.");
		assert_check_2(TargetType,Internal_Element,is_same,"Not a recognizable element category! Available options are Regular_Element, Internal_Element or Accented_Element.");
	}

	template<typename TargetType> void Select(/*requires(TargetType,check_2(typename MasterType::canvas_type,is_same))*/)
	{
//TODO
//		//_attributes_panel->Push_Schema<Target_Type<NT,NT,std::vector<string>&>>(_attributes_schema);
	}
	
	//template<typename TargetType> void Select(requires(TargetType,!check_2(typename MasterType::canvas_type,is_same))){static_assert(false,"Caller Not a Canvas Object");}

	template<typename TargetType> void Deselect_All(/*requires(TargetType,check_2(typename MasterType::canvas_type,is_same) || check_2(ComponentType,is_same))*/)
	{
		if( _selection_callback != nullptr)
		{
			_added_elements.clear();
			
			_deselected_elements.clear();
			for(std::list<void*>::iterator itr=_selected_elements.begin();itr!=_selected_elements.end();itr++){ _deselected_elements.push_back( *itr ); }

			_selected_elements.clear();

			std::vector<pair<string,string>> bucket;

			(*_selection_callback)( _deselected_elements, _added_elements, _selected_elements, bucket );
		}
		
		Clear_Accented<NT>();
		
		_added_elements.clear();
		_deselected_elements.clear();
		_selected_elements.clear();
	}

	//template<typename TargetType> void Deselect_All( requires(TargetType,!check_2(typename MasterType::canvas_type,is_same) && !check_2(ComponentType,is_same)) ){static_assert(false,"Caller Not a Canvas Object");}

	template<typename TargetType> void Double_Click(/*requires(TargetType,check_2(typename MasterType::canvas_type,is_same))*/)
	{
		if(_selected_elements.size() && _double_click_callback!=nullptr/* && _submission_callback!=nullptr*/)
		{
			std::vector<pair<string,string>> dialog_key_value;
			std::vector<std::vector<string>> dialog_dropdown;

			(*_double_click_callback)(_selected_elements,dialog_key_value,dialog_dropdown);

			if(_control_dialog != nullptr)
			{
				delete _control_dialog;
				_control_dialog = nullptr;
			}

			_control_dialog = (control_dialog_type)new type_of(control_dialog)(_name,_selected_elements,dialog_key_value,dialog_dropdown,_submission_callback);

			_control_dialog->ShowModal<NULLTYPE>();
		}

		//if( _selected_elements.size()==0 ) return;

		//if(_control_dialog == nullptr)
		//{
		//	_control_dialog = (control_dialog_interface*)new type_of(control_dialog)(_name);

//TODO
//		//	_control_dialog->Push_Schema<Target_Type<NT,NT,std::vector<string>&,vector<vector<string>>&>>(_attributes_schema,_dropdown_schema);

		//	if(_submission_callback != nullptr)
		//	{
		//		_control_dialog->submission_callback<selection_callback_type>(_submission_callback);
		//	}
		//}

		//if(_selected_elements.size() != 0)
		//{
		//	if(_selection_callback != nullptr)
		//	{
		//		std::vector<string> bucket;
		//		_selection_callback( *((void**)_selected_elements[0]), bucket );
//TODO
//		//		_control_dialog->Push_Attributes<Target_Type<NT,NT,std::vector<string>&>>(bucket);

		//		if(_submission_callback != nullptr)
		//		{
		//			_control_dialog->selected_object<void*>( *((void**)_selected_elements[0]) );
		//		}
		//	}
		//}

		//_control_dialog->ShowModal<NULLTYPE>();

		//_control_dialog->selected_object<void*>(nullptr);
	}
	
	//template<typename TargetType> void Double_Click(requires(TargetType,!check_2(typename MasterType::canvas_type,is_same))){static_assert(false,"Caller Not a Canvas Object");}

	bool Identify_One(const Point_3D<MasterType>& point, int start_iteration, int end_iteration,ANTARES_SELECTION_MODE mode);

	template<typename TargetType> void Clear_Accented()
	{
		std::vector<int>* geometry_by_thread = _accent_storage[iteration()];

		for(int i=0;i<num_antares_threads();i++)
		{
			geometry_by_thread[i].clear();
		}
	}

	template<typename TargetType> void Refresh_Selection()
	{
		if( _selection_callback != nullptr && _selected_elements.size() )
		{
			_added_elements.clear();
			_deselected_elements.clear();
			
			std::vector<pair<string,string>> bucket;
			_selection_callback( _deselected_elements, _added_elements, _selected_elements, bucket );
			
//TODO
//			_attributes_panel->Push_Attributes<Target_Type<NT,NT,std::vector<pair<string,string>>&>>(bucket);
		}
		else
		{
			//Deselect_All<NT>();
		}
	}
	
	m_data(bool,dynamic_data, NONE, NONE);
	m_data(int,target_sub_iteration, NONE, NONE);

	m_data(Dynamic_Multi_Buffer< std::vector<int>* >,storage,NONE,NONE);
	m_data(Dynamic_Multi_Buffer< std::vector<int>* >,accent_storage,NONE,NONE);

	// Identification values
	m_data(string,name, NONE, NONE);
	m_data(int,list_index, NONE, NONE);
	
	// Drawing related values
	m_data(bool,draw, NONE, NONE);

	m_data(PrimitiveType,primitive_type, NONE, NONE);
	m_data(bool,poly, NONE, NONE);	

	m_data(True_Color_RGBA<MasterType>,head_color, NONE, NONE);
	m_data(Point_3D<MasterType>,head_normal, NONE, NONE);
	m_data(int,head_size_value, NONE, NONE);
	m_data(int,head_accent_size_value, NONE, NONE);
	//member_data(True_Color_RGBA<MasterType>,head_accent_color,none,none);
	m_data(int,head_texture, NONE, NONE);

	m_data(std::vector<unsigned int>,texture_map, NONE, NONE);

	m_data(bool,grouped, NONE, NONE);
	m_data(bool,group_color, NONE, NONE);
	m_data(bool,group_normal, NONE, NONE);
	m_data(bool,group_texture, NONE, NONE);

	m_data(int,primitive_stride, NONE, NONE);

	m_data(int,vert_stride, NONE, NONE);
	m_data(bool,primitive_color, NONE, NONE);
	m_data(bool,primitive_normal, NONE, NONE);
	m_data(bool,primitive_texture, NONE, NONE);

	m_data(unsigned char, default_texture_alpha, NONE, NONE);

	m_data(int,vert_size, NONE, NONE);	

	
	m_data(int,data_stride, NONE, NONE);

	//member_data(pixel_size_callback_type,pixel_size_callback,none,none);
	//member_data(pixel_size_callback_type,accent_pixel_size_callback,none,none);

	pixel_size_callback_type _pixel_size_callback;
	template<typename TargetType> TargetType pixel_size_callback(){return (TargetType)_pixel_size_callback;}
	tag_getter_as_available(pixel_size_callback);
	
	pixel_size_callback_type _accent_pixel_size_callback;
	template<typename TargetType> TargetType accent_pixel_size_callback(){return (TargetType)_accent_pixel_size_callback;}
	tag_getter_as_available(accent_pixel_size_callback);

	m_data(submission_callback_type,submission_callback, NONE, NONE);
	m_data(selection_callback_type,selection_callback, NONE, NONE);
	m_data(double_click_callback_type,double_click_callback, NONE, NONE);

	m_prototype(Layer_Options,typename MasterType::layer_options_type,layer_options, NONE, NONE);
	m_prototype(Attributes_Panel,typename MasterType::attributes_panel_type,attributes_panel, NONE, NONE);
	m_prototype(Control_Dialog,typename MasterType::control_dialog_type,control_dialog, NONE, NONE);

	m_data(std::list<void*>,selected_elements, NONE, NONE);
	m_data(std::list<void*>,deselected_elements, NONE, NONE);
	m_data(std::list<void*>,added_elements, NONE, NONE);

	m_data(string,x_label, NONE, NONE);
	m_data(string,y_label, NONE, NONE);
	
	// Agent behavior
	//declare_feature_conditional(Update_Condition)
	//{
	//	Antares_Layer_Implementation* pthis=(Antares_Layer_Implementation*)_this;

	//	response.result = true;
	//	response.next._iteration = iteration() + pthis->_storage.period;
	//	response.next._sub_iteration = pthis->_target_sub_iteration;
	//}

	static void Update(Antares_Layer_Implementation* _this,Event_Response& response)
	{
		Antares_Layer_Implementation* pthis=(Antares_Layer_Implementation*)_this;

		response.next._iteration = iteration() + pthis->_storage.period;
		response.next._sub_iteration = pthis->_target_sub_iteration;
			
		if(pthis->_dynamic_data)
		{
			int reserve;

			LOCK(_canvas_lock);

			for(unsigned int i=0;i<num_antares_threads();i++)
			{
				reserve=pthis->_storage[iteration() + pthis->_storage.period][i].size();
				pthis->_storage[iteration() + pthis->_storage.period][i].clear();
				pthis->_storage[iteration() + pthis->_storage.period][i].reserve(reserve);

				//reserve=pthis->_accent_storage[iteration() + pthis->_accent_storage.period][i].size();
				//pthis->_accent_storage[iteration() + pthis->_accent_storage.period][i].clear();
				//pthis->_accent_storage[iteration() + pthis->_accent_storage.period][i].reserve(reserve);
			}

			UNLOCK(_canvas_lock);
		}

		pthis->Refresh_Selection<ComponentType>();
	}
};

	//feature_implementation void Push_Element(void* data, int iteration, requires(check_2(TargetType,Internal_Element,is_same)))
	//{
	//	//Clear_Accented<ComponentType,ComponentType,NT>();

	//	True_Color_RGBA<MasterType> accent_color;
	//	
	//	const int* geometry_itr=(const int*)data;

	//	const bool grouped = _grouped;
	//	const bool group_color = _group_color;
	//	const bool group_normal = _group_normal;

	//	const PrimitiveType primitive_type = _primitive_type;

	//	const bool primitive_color = _primitive_color;
	//	const bool primitive_normal = _primitive_normal;
	//	const int primitive_stride = _primitive_stride/sizeof(int);
	//	
	//	const int vert_size = _vert_size/sizeof(int);
	//	const int vert_stride = _vert_stride/sizeof(int);
	//	const int data_stride = _data_stride/sizeof(int);

	//	vector<int>& storage_reference=_accent_storage[iteration][_thread_id];

	//	if(primitive_type == _PLOT)
	//	{
	//		//this does not reflect what pushing an internal plot looks like
	//		exit(0);

	//		PLFLT point_value;
	//		const int* end_data_itr = (const int*) ( ((int*)&point_value) + sizeof(PLFLT)/sizeof(int) );

	//		const int* const geometry_data_end = geometry_itr + data_stride;

	//		while( geometry_itr != geometry_data_end )
	//		{
	//			storage_reference.push_back(*geometry_itr);
	//			++geometry_itr;
	//		}

	//		if(group_color)
	//		{
	//			const int* end_data_itr = geometry_itr+sizeof(True_Color_RGBA<MasterType>)/sizeof(int);

	//			while(geometry_itr!=end_data_itr)
	//			{
	//				storage_reference.push_back(*geometry_itr);
	//				++geometry_itr;
	//			}
	//		}

	//		const int num_group_primitives=*((int*)geometry_itr);
	//		
	//		const int* end_size_itr = geometry_itr+sizeof(int)/sizeof(int);

	//		while(geometry_itr!=end_size_itr)
	//		{
	//			storage_reference.push_back(*geometry_itr);
	//			++geometry_itr;
	//		}

	//		geometry_itr=(*((int**)geometry_itr));
	//		const int* const group_begin = geometry_itr;
	//		const int* const group_end = geometry_itr + sizeof(Point_2D<MasterType>)/sizeof(int) * num_group_primitives;

	//		// Collect x points
	//		while( geometry_itr != group_end )
	//		{
	//			const int* const geometry_vert_end = geometry_itr + sizeof(Point_2D<MasterType>)/sizeof(int);

	//			point_value=(PLFLT)(*((float*)geometry_itr));
	//			
	//			geometry_itr = (int*)&point_value;

	//			while( geometry_itr!=end_data_itr )
	//			{
	//				storage_reference.push_back(*geometry_itr);
	//				++geometry_itr;
	//			}

	//			geometry_itr = (const int*)geometry_vert_end;
	//		}

	//		geometry_itr = group_begin;

	//		// Collect y points
	//		while( geometry_itr != group_end )
	//		{
	//			const int* const geometry_vert_end = geometry_itr + sizeof(Point_2D<MasterType>)/sizeof(int);

	//			geometry_itr += (sizeof(Point_2D<MasterType>)/sizeof(int))/2;

	//			point_value=(PLFLT)(*((float*)geometry_itr));

	//			geometry_itr = (int*)&point_value;

	//			while( geometry_itr!=end_data_itr )
	//			{
	//				storage_reference.push_back(*geometry_itr);
	//				++geometry_itr;
	//			}

	//			geometry_itr = (const int*)geometry_vert_end;
	//		}
	//	}
	//	else
	//	{
	//		if(grouped)
	//		{
	//			const int* const geometry_data_end = geometry_itr + data_stride;

	//			while( geometry_itr != geometry_data_end )
	//			{
	//				storage_reference.push_back(*geometry_itr);
	//				++geometry_itr;
	//			}

	//			if(group_color)
	//			{
	//				accent_color = *((True_Color_RGBA<MasterType>*)geometry_itr);
	//				accent_color._r = 255 - accent_color._r;
	//				accent_color._g = 255 - accent_color._g;
	//				accent_color._b = 255 - accent_color._b;

	//				int* accent_itr = (int*)&accent_color;
	//				const int* end_data_itr = geometry_itr+sizeof(True_Color_RGBA<MasterType>)/sizeof(int);

	//				while(geometry_itr!=end_data_itr)
	//				{
	//					storage_reference.push_back(*accent_itr);
	//					++accent_itr;
	//					++geometry_itr;
	//				}
	//			}

	//			if(group_normal)
	//			{
	//				const int* end_data_itr = geometry_itr+sizeof(Point_3D<MasterType>)/sizeof(int);
	//				
	//				while(geometry_itr!=end_data_itr)
	//				{
	//					storage_reference.push_back(*geometry_itr);
	//					++geometry_itr;
	//				}
	//			}

	//			const int num_group_primitives=*((int*)geometry_itr);

	//			const int* end_size_itr = geometry_itr+sizeof(int)/sizeof(int);

	//			while(geometry_itr!=end_size_itr)
	//			{
	//				storage_reference.push_back(*geometry_itr);
	//				++geometry_itr;
	//			}

	//			const int* const group_end = geometry_itr + primitive_stride * num_group_primitives;

	//			while( geometry_itr != group_end )
	//			{
	//				if(primitive_color)
	//				{
	//					accent_color = *((True_Color_RGBA<MasterType>*)geometry_itr);
	//					accent_color._r = 255 - accent_color._r;
	//					accent_color._g = 255 - accent_color._g;
	//					accent_color._b = 255 - accent_color._b;

	//					int* accent_itr = (int*)&accent_color;
	//					const int* end_data_itr = geometry_itr+sizeof(True_Color_RGBA<MasterType>)/sizeof(int);

	//					while(geometry_itr!=end_data_itr)
	//					{
	//						storage_reference.push_back(*accent_itr);
	//						++accent_itr;
	//						++geometry_itr;
	//					}
	//				}

	//				if(primitive_normal)
	//				{
	//					const int* end_data_itr = geometry_itr+sizeof(Point_3D<MasterType>)/sizeof(int);

	//					while(geometry_itr!=end_data_itr)
	//					{
	//						storage_reference.push_back(*geometry_itr);
	//						++geometry_itr;
	//					}
	//				}
	//			
	//				const int* const geometry_vert_end = geometry_itr + vert_stride;

	//				while( geometry_itr != geometry_vert_end )
	//				{
	//					const int* end_data_itr = geometry_itr+vert_size;

	//					while(geometry_itr!=end_data_itr)
	//					{
	//						storage_reference.push_back(*geometry_itr);
	//						++geometry_itr;
	//					}
	//				}
	//			}
	//		}
	//		else
	//		{
	//			const int* const geometry_data_end = geometry_itr + data_stride;

	//			while( geometry_itr != geometry_data_end )
	//			{
	//				storage_reference.push_back(*geometry_itr);
	//				++geometry_itr;
	//			}

	//			const int* const group_end = geometry_itr + primitive_stride;

	//			while( geometry_itr != group_end )
	//			{
	//				if(primitive_color)
	//				{
	//					accent_color = *((True_Color_RGBA<MasterType>*)geometry_itr);
	//					accent_color._r = 255 - accent_color._r;
	//					accent_color._g = 255 - accent_color._g;
	//					accent_color._b = 255 - accent_color._b;

	//					int* accent_itr = (int*)&accent_color;
	//					const int* end_data_itr = geometry_itr+sizeof(True_Color_RGBA<MasterType>)/sizeof(int);

	//					while(geometry_itr!=end_data_itr)
	//					{
	//						storage_reference.push_back(*accent_itr);
	//						++accent_itr;
	//						++geometry_itr;
	//					}
	//				}

	//				if(primitive_normal)
	//				{
	//					const int* end_data_itr = geometry_itr+sizeof(Point_3D<MasterType>)/sizeof(int);

	//					while(geometry_itr!=end_data_itr)
	//					{
	//						storage_reference.push_back(*geometry_itr);
	//						++geometry_itr;
	//					}
	//				}

	//				const int* const geometry_vert_end = geometry_itr + vert_stride;

	//				while( geometry_itr != geometry_vert_end )
	//				{
	//					const int* end_data_itr = geometry_itr+vert_size;

	//					while(geometry_itr!=end_data_itr)
	//					{
	//						storage_reference.push_back(*geometry_itr);
	//						++geometry_itr;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

// Notes


//enum PrimitiveType
//{
//	2D_PLOT,
//	POINT,
//	LINE,
//	TRIANGLE,
//	QUAD
//};
//
//// Unrelated note: to aid in output visualization, update execution engine to not select the next iteration / sub_iteration by incrementing, but rather by assigning the cached next value
//// similarly instead of having a time slider (which sucks), have time be a text entry + jump
//
//
//// Dynamic plotting can be handled much the same way, less complex multi buffer, can do moving measurement or historical plot depending on multi_buffer size and range input to draw
//// Color is established and drawn much the same way
//// Static plotting is a special case, but so in the same way is Static drawing
//
//implementation struct Antares_Layer_Implementation:public Polaris_Component_Class<Antares_Layer_Implementation,MasterType,Execution_Object>
//{
//	typedef unsigned char buffer_unit;
//
//	Dynamic_Multi_Buffer<vector<buffer_unit>> storage; //Multi_Buffer needs to be dynamic on buffer_size and buffer_period
//	string identifier; // for labeling
//	int std::list_index; // for ordering
//	PrimitiveType primitive_type;
//	bool draw; // check whether to draw or not, probably better to have a conditional callback here to determine, more flexible
//
//	// alternatively, string indicating vertex pattern: cnvvvv => color normal vertex x 4 ... primitive type is too far
//	// best to resolve this somehow at compile time so that gratuitous if statements do not occur in the draw loop
//	// though maybe compiler can figure out const loops: i.e. for(int i=0;i<(const)num_colors;i++) glColor();vertex++;
//	// this will work best for traditional progressions cnv -> cnv -> cnv , but not for cnvvcnvvv for instance
//	string data_pattern;
//
//	//int color_period;// indicates stride for each unique color, 0 => head only
//	//Color head_color; // color for head
//	//int normal_period;// indicates stride for each unique normal, 0 => head only
//	//Point head_normal;
//
//	//grander stride structures to put in vector
//	//color, normal, vert, vert, vert, vert - for instance
//	//seamless submission / conversion
//	
//	void Draw(int iteration_start, int iteration_end)
//	{
//		// perhaps load callbacks into a vector and call; member type, i.e. void (Canvas_Implementation::*)(void)
//		// this function is actually inside Canvas because it must issue gl commands
//		// one function for each geometry type: gl_point, gl_quad, gl_line, gl_triangle
//		// one function for mono-colored, one for multi-colored
//		
//		// loop over iteration range
//		// extract data vector at iteration
//		// glBegin, loop over threads, draw appropriate pattern, glEnd
//	}
//	
//	declare_feature_conditional(Update_Condition)
//	{
//		// activate on buffer_period
//		response.result = true;
//		response.next._iteration = _iteration + buffer_period;
//		response.next._sub_iteration = 0; // should be end iteration
//	}
//
//	declare_feature_event(Update)
//	{
//		// clear iteration + period
//		storage[_iteration + buffer_period].clear();
//
//		// perhaps have option to copy the data onward
//	}
//};
//
//// additional note, it may be helpful to have an API which accepts standardized Line structure + Offset as an alternative for position

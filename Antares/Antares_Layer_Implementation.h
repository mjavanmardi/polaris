//*********************************************************
//	Antares_Layer_Implementation.h - Graphical Layer
//*********************************************************

#pragma once
#include "Antares_Layer.h"

//---------------------------------------------------------
//	Antares_Layer_Implementation - layer definition
//---------------------------------------------------------

implementation struct Antares_Layer_Implementation:public Polaris_Component<APPEND_CHILD(Antares_Layer_Implementation),MasterType,Execution_Object>
{
	feature_implementation void Push_Element(void* data, int size, int iteration,requires(check_2(TargetType,Regular_Element,is_same)))
	{
		unsigned char* data_itr=(unsigned char*)data;

		vector<unsigned char>& storage_reference=_storage[iteration][_thread_id];

		unsigned char* end_data_itr=data_itr+size;
		
		while(data_itr!=end_data_itr)
		{
			storage_reference.push_back(*data_itr);
			data_itr++;
		}
	}
	
	feature_implementation void Push_Element(void* data, int size, int iteration,requires(check_2(TargetType,Accented_Element,is_same)))
	{
		unsigned char* data_itr=(unsigned char*)data;

		vector<unsigned char>& storage_reference=_accent_storage[iteration][_thread_id];

		unsigned char* end_data_itr=data_itr+size;
		
		while(data_itr!=end_data_itr)
		{
			storage_reference.push_back(*data_itr);
			data_itr++;
		}
	}
	
	feature_implementation void Push_Element(void* data, int size, int iteration,requires(!check_2(TargetType,Regular_Element,is_same) && !check_2(TargetType,Accented_Element,is_same)))
	{
		assert_check_2(TargetType,Regular_Element,is_same,"Not a recognizable element category! Available options are Regular_Element or Accented_Element.");
	}

	feature_implementation void Initialize(Antares_Layer_Configuration& cfg)
	{
		_dynamic_data=cfg.dynamic_data;
		_target_sub_iteration=cfg.target_sub_iteration;

		if(_dynamic_data)
		{
			Load_Register<Antares_Layer_Implementation>(&Update_Condition<NULLTYPE>, &Update<NULLTYPE>, _iteration + 1 ,_target_sub_iteration);
		}

		_storage.Initialize(cfg.storage_offset, cfg.storage_period, cfg.storage_size);
		_accent_storage.Initialize(cfg.storage_offset, cfg.storage_period, cfg.storage_size);

		_draw=cfg.draw;

		_primitive_type=cfg.primitive_type;

		_grouped=cfg.grouped;
			_group_color=cfg.group_color;
			_group_normal=cfg.group_normal;



		_head_color=*((True_Color_RGBA<MasterType>*)&cfg.head_color);
		_head_normal=*((Point_3D<MasterType>*)&cfg.head_normal);
		_head_size_value=cfg.head_size_value;
		
		
		
		_primitive_color=cfg.primitive_color;
		_primitive_normal=cfg.primitive_normal;

		switch(_primitive_type)
		{
		case _POINT:
			_vert_stride = sizeof(Point_3D<MasterType>)*1;
			break;
		case _LINE:
			_vert_stride = sizeof(Point_3D<MasterType>)*2;
			break;
		case _TRIANGLE:
			_vert_stride = sizeof(Point_3D<MasterType>)*3;
			break;
		case _QUAD:
			_vert_stride = sizeof(Point_3D<MasterType>)*4;
			break;
		default:
			assert(false);
			break;
		};

		_primitive_stride = _vert_stride + _primitive_color*sizeof(True_Color_RGBA<MasterType>) + _primitive_normal*sizeof(Point_3D<MasterType>);

		_attributes_schema=cfg.attributes_schema;
		_submission_callback=cfg.submission_callback;
		_attributes_callback=cfg.attributes_callback;

		_data_stride=cfg.data_stride;
	}

	feature_implementation void Identify(const Point_3D<MasterType>& point, int start_iteration, int end_iteration);

	member_data(bool,dynamic_data,none,none);
	member_data(int,target_sub_iteration,none,none);

	member_data(Dynamic_Multi_Buffer< vector<unsigned char>[_num_antares_threads] >,storage,none,none);
	member_data(Dynamic_Multi_Buffer< vector<unsigned char>[_num_antares_threads] >,accent_storage,none,none);

	// Identification values
	member_data(string,name,none,none);
	member_data(int,list_index,none,none);
	
	// Drawing related values
	member_data(bool,draw,none,none);

	member_data(PrimitiveType,primitive_type,none,none);
	

	member_data(True_Color_RGBA<MasterType>,head_color,none,none);
	member_data(Point_3D<MasterType>,head_normal,none,none);
	member_data(int,head_size_value,none,none);
	
	member_data(bool,grouped,none,none);
	member_data(bool,group_color,none,none);
	member_data(bool,group_normal,none,none);

	member_data(int,primitive_stride,none,none);

	member_data(int,vert_stride,none,none);
	member_data(bool,primitive_color,none,none);
	member_data(bool,primitive_normal,none,none);

	

	
	member_data(int,data_stride,none,none);

	member_data(string,attributes_schema,none,none);

	typedef bool (*attributes_callback_type)(void*,string&);
	
	member_data(attributes_callback_type,submission_callback,none,none);
	member_data(attributes_callback_type,attributes_callback,none,none);

	member_prototype(Attributes_Panel,attributes_panel,typename MasterType::type_of(attributes_panel),none,none);

	// Agent behavior

	declare_feature_conditional(Update_Condition)
	{
		Antares_Layer_Implementation* pthis=(Antares_Layer_Implementation*)_this;

		response.result = true;
		response.next._iteration = _iteration + pthis->_storage.period;
		response.next._sub_iteration = pthis->_target_sub_iteration;
	}

	declare_feature_event(Update)
	{
		Antares_Layer_Implementation* pthis=(Antares_Layer_Implementation*)_this;

		for(int i=0;i<_num_antares_threads;i++)
		{
			pthis->_storage[_iteration + pthis->_storage.period][i].clear();
			pthis->_accent_storage[_iteration + pthis->_accent_storage.period][i].clear();
		}
	}

};


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
//	int list_index; // for ordering
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
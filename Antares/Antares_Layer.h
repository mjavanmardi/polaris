//*********************************************************
//	Antares_Layer.h - Graphical Layer
//*********************************************************

#pragma once
#include "Antares_Includes.h"

struct Antares_Layer_Configuration;

//---------------------------------------------------------
//	Antares_Layer - layer definition
//---------------------------------------------------------

struct Accented_Element{};
struct Regular_Element{};

prototype struct Antares_Layer
{
	tag_as_prototype;
	
	feature_prototype void Push_Element(void* data, int size, int iteration = _iteration)
	{
		this_component()->Push_Element<ComponentType,CallerType,TargetType>(data,size,iteration);
	}

	feature_prototype void Initialize(Antares_Layer_Configuration& cfg)
	{
		this_component()->Initialize<ComponentType,CallerType,TargetType>(cfg);
	}
	
	feature_prototype void Identify(typename TargetType::ParamType point, int start_iteration, int end_iteration)
	{
		this_component()->Identify<ComponentType,CallerType,TargetType>(point,start_iteration,end_iteration);
	}

	feature_accessor(list_index,none,none);
	feature_accessor(name,none,none);
	
	feature_accessor(storage,none,none);
	feature_accessor(accent_storage,none,none);

	feature_accessor(primitive_type,none,none);

	feature_accessor(draw,none,none);

	feature_accessor(head_color,none,none);
	feature_accessor(color,none,none);
	
	feature_accessor(head_normal,none,none);	
	feature_accessor(normal,none,none);

	feature_accessor(head_size_value,none,none);

	feature_accessor(num_vertices,none,none);
	
	feature_accessor(data_stride,none,none);
	
	feature_accessor(element_size,none,none);
};

//---------------------------------------------------------
//	PrimitiveType - primitive type options
//---------------------------------------------------------

enum PrimitiveType
{
	_PLOT,
	_POINT,
	_LINE,
	_TRIANGLE,
	_QUAD
};

//---------------------------------------------------------
//	Antares_Layer_Configuration - layer configuration struct
//---------------------------------------------------------

struct Antares_Layer_Configuration
{
	void Configure_Lines()
	{
		dynamic_data=false;
		target_sub_iteration=-1;

		storage_offset=_iteration;
		storage_size=1;
		storage_period=1;

		draw=true;

		primitive_type=_LINE;
		
		color=false;
		head_color._r=0;
		head_color._g=0;
		head_color._b=0;
		head_color._a=200;
		
		normal=false;
		head_normal._x=0;
		head_normal._y=0;
		head_normal._z=1;
		
		head_size_value=1;

		num_vertices=2;

		data_stride=0;
	}
	
	void Configure_Points()
	{
		dynamic_data=true;
		target_sub_iteration=Scenario_Components::Types::END_OF_ITERATION+1;

		storage_offset=_iteration;
		storage_size=3;
		storage_period=1;

		draw=true;

		primitive_type=_POINT;

		color=false;
		head_color._r=255;
		head_color._g=0;
		head_color._b=0;
		head_color._a=255;

		normal=false;
		head_normal._x=0;
		head_normal._y=0;
		head_normal._z=1;
		
		head_size_value=4;

		num_vertices=1;

		data_stride=0;
	}

	void Configure_Static_Points(True_Color_RGBA<NULLTYPE>& Color, int size)
	{
		dynamic_data=false;
		target_sub_iteration=-1;

		storage_offset=_iteration;
		storage_size=1;
		storage_period=1;

		draw=true;

		primitive_type=_POINT;

		color=false;
		head_color._r=Color._r;
		head_color._g=Color._g;
		head_color._b=Color._b;
		head_color._a=Color._a;

		normal=false;
		head_normal._x=0;
		head_normal._y=0;
		head_normal._z=1;
		
		head_size_value=size;

		num_vertices=1;

		data_stride=0;
	}
	
	void Configure_Plot()
	{
		dynamic_data=false;
		target_sub_iteration=-1;

		storage_offset=_iteration;
		storage_size=_num_iterations;
		storage_period=1;

		draw=true;

		primitive_type=_PLOT;

		color=false;
		head_color._r=255;
		head_color._g=0;
		head_color._b=0;
		head_color._a=255;

		normal=false;
		head_normal._x=0;
		head_normal._y=0;
		head_normal._z=1;

		head_size_value=0;

		num_vertices=0;

		data_stride=sizeof(int);
	}

	bool dynamic_data;
	int target_sub_iteration;

	unsigned int storage_offset;
	unsigned int storage_size;
	unsigned int storage_period;

	bool draw;

	PrimitiveType primitive_type;
	
	bool color;
	True_Color_RGBA<NULLTYPE> head_color;

	bool normal;
	Point_3D<NULLTYPE> head_normal;

	int head_size_value;

	int num_vertices;
	
	int data_stride;
};
//*********************************************************
//	Antares_Layer.h - Graphical Layer
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "Control_Dialog.h"

struct Antares_Layer_Configuration;

//---------------------------------------------------------
//	Antares_Layer - layer definition
//---------------------------------------------------------

struct Accented_Element{};
struct Regular_Element{};
struct Internal_Element{};

prototype struct Antares_Layer
{
	tag_as_prototype;
	
	feature_prototype void Push_Element(void* data, int iteration = _iteration)
	{
		this_component()->Push_Element<ComponentType,CallerType,TargetType>(data,iteration);
	}

	feature_prototype void Initialize(Antares_Layer_Configuration& cfg)
	{
		this_component()->Initialize<ComponentType,CallerType,TargetType>(cfg);
	}
	
	feature_prototype bool Identify(typename TargetType::ParamType point, int start_iteration, int end_iteration)
	{
		return this_component()->Identify<ComponentType,CallerType,TargetType>(point,start_iteration,end_iteration);
	}
	
	feature_prototype void Select()
	{
		this_component()->Select<ComponentType,CallerType,TargetType>();
	}
	
	feature_prototype void Deselect()
	{
		this_component()->Deselect<ComponentType,CallerType,TargetType>();
	}
	
	feature_prototype void Double_Click()
	{
		this_component()->Double_Click<ComponentType,CallerType,TargetType>();
	}
	
	feature_prototype void Clear_Accented()
	{
		this_component()->Clear_Accented<ComponentType,CallerType,TargetType>();
	}

	feature_accessor(list_index,none,none);
	feature_accessor(name,none,none);
	
	feature_accessor(storage,none,none);
	feature_accessor(accent_storage,none,none);
	
	feature_accessor(draw,none,none);
	
	feature_accessor(poly,none,none);

	feature_accessor(primitive_type,none,none);
	feature_accessor(head_size_value,none,none);
	feature_accessor(head_normal,none,none);	
	
	
	feature_accessor(grouped,none,none);
	feature_accessor(group_color,none,none);
	feature_accessor(group_normal,none,none);
	feature_accessor(head_color,none,none);


	feature_accessor(primitive_color,none,none);
	feature_accessor(primitive_normal,none,none);
	feature_accessor(primitive_stride,none,none);
	feature_accessor(vert_stride,none,none);	
	feature_accessor(vert_size,none,none);	
	
	feature_accessor(attributes_schema,none,none);
	feature_accessor(attributes_callback,none,none);
	feature_accessor(submission_callback,none,none);

	feature_accessor(data_stride,none,none);

	feature_accessor(attributes_panel,none,none);
};

template<typename ComponentType,typename CallerType,typename TargetType>
typename TargetType::ReturnType Allocate_New_Layer(typename TargetType::ParamType name)
{
	return (TargetType::ReturnType)((ComponentType*)canvas)->Allocate_New_Layer<ComponentType,CallerType,TargetType>(name);
}

template<typename ComponentType,typename CallerType,typename TargetType>
typename TargetType::ReturnType Scale_Coordinates(typename TargetType::ParamType coordinates)
{
	return (TargetType::ReturnType)((ComponentType*)canvas)->Scale_Coordinates<ComponentType,CallerType,TargetType>(coordinates);
}

template<typename ComponentType,typename CallerType,typename TargetType>
typename TargetType::ReturnType Allocate_New_Plot_Layer(typename TargetType::ParamType name)
{
	return (TargetType::ReturnType)((ComponentType*)information_panel)->Allocate_New_Layer<ComponentType,CallerType,TargetType>(name);
}

//---------------------------------------------------------
//	PrimitiveType - primitive type options
//---------------------------------------------------------

enum PrimitiveType
{
	_PLOT,
	_POINT,
	_LINE,
	_TRIANGLE,
	_QUAD,
	_POLYGON
};

//---------------------------------------------------------
//	Antares_Layer_Configuration - layer configuration struct
//---------------------------------------------------------

struct Antares_Layer_Configuration
{
	// Default Configuration
	Antares_Layer_Configuration()
	{
		draw=false;
		dynamic_data=false;
		target_sub_iteration=-1;

		storage_offset=_iteration;
		storage_size=1;
		storage_period=1;
		
		primitive_color=false;
		head_color._r=0;
		head_color._g=0;
		head_color._b=0;
		head_color._a=255;
		
		primitive_normal=false;
		head_normal._x=0;
		head_normal._y=0;
		head_normal._z=1;
		
		grouped=false;

		head_size_value=1;

		attributes_schema="";

		submission_callback=nullptr;
		attributes_callback=nullptr;
	}

	void Configure_Lines()
	{
		primitive_type=_LINE;
		
		primitive_color=false;
		head_color._r=0;
		head_color._g=125;
		head_color._b=0;
		head_color._a=255;
	}
	
	void Configure_Points()
	{
		dynamic_data=true;
		target_sub_iteration=Scenario_Components::Types::END_OF_ITERATION+1;

		storage_offset=_iteration;
		storage_size=3;
		storage_period=1;

		primitive_type=_POINT;

		head_color._r=255;
		head_color._g=0;
		head_color._b=0;
		head_color._a=255;
		
		head_size_value=4;
	}

	void Configure_Static_Points(True_Color_RGBA<NULLTYPE>& Color, int size)
	{
		primitive_type=_POINT;

		head_color._r=Color._r;
		head_color._g=Color._g;
		head_color._b=Color._b;
		head_color._a=Color._a;
		
		head_size_value=size;
	}
	
	void Configure_Static_Quads(True_Color_RGBA<NULLTYPE>& Color, int size)
	{
		primitive_type=_QUAD;

		head_color._r=Color._r;
		head_color._g=Color._g;
		head_color._b=Color._b;
		head_color._a=Color._a;
		
		grouped=true;
		group_color=false;
		group_normal=false;
	}
	
	void Configure_Dynamic_Quads(True_Color_RGBA<NULLTYPE>& Color, int size)
	{
		dynamic_data=true;
		storage_size=3;
		storage_period=1;

		target_sub_iteration=Scenario_Components::Types::END_OF_ITERATION+1;

		primitive_type=_QUAD;

		head_color._r=Color._r;
		head_color._g=Color._g;
		head_color._b=Color._b;
		head_color._a=Color._a;
	}

	void Configure_Plot()
	{
		//storage_size=_num_iterations;
		
		draw=true;
		dynamic_data=true;
		target_sub_iteration=Scenario_Components::Types::END_OF_ITERATION+1;

		storage_size=3;
		storage_period=1;

		primitive_type=_PLOT;

		head_color._r=255;
		head_color._g=0;
		head_color._b=0;
		head_color._a=255;
	}
	
	bool draw;
	bool dynamic_data;
	int target_sub_iteration;

	unsigned int storage_offset;
	unsigned int storage_size;
	unsigned int storage_period;


	PrimitiveType primitive_type;

	True_Color_RGBA<NULLTYPE> head_color;
	Point_3D<NULLTYPE> head_normal;
	int head_size_value;


	bool grouped;
		bool group_color;
			// 1 x True_Color_RGBA<NULLTYPE> group_color;
		bool group_normal;
			// 1 x Point_3D<NULLTYPE> group_normal;
	
	
	bool primitive_color;
		// 1 x True_Color_RGBA<NULLTYPE> primitive_color;
	bool primitive_normal;
		// 1 x Point_3D<NULLTYPE> primitive_normal;
	

	string attributes_schema;

	attributes_callback_type submission_callback;
	attributes_callback_type attributes_callback;
};
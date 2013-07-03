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

enum ANTARES_SELECTION_MODE {ALT_DOWN,CTRL_DOWN};

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
	
	feature_prototype bool Identify_One(typename TargetType::ParamType point, int start_iteration, int end_iteration, ANTARES_SELECTION_MODE mode)
	{
		return this_component()->Identify_One<ComponentType,CallerType,TargetType>(point,start_iteration,end_iteration,mode);
	}
	
	feature_prototype void Select()
	{
		this_component()->Select<ComponentType,CallerType,TargetType>();
	}
	
	feature_prototype void Deselect_All()
	{
		this_component()->Deselect_All<ComponentType,CallerType,TargetType>();
	}
	
	feature_prototype void Double_Click()
	{
		this_component()->Double_Click<ComponentType,CallerType,TargetType>();
	}
	
	feature_prototype void Clear_Accented()
	{
		this_component()->Clear_Accented<ComponentType,CallerType,TargetType>();
	}

	feature_prototype void Refresh_Selection()
	{
		this_component()->Refresh_Selection<ComponentType,CallerType,TargetType>();
	}

	feature_accessor(list_index,none,none);
	feature_accessor(name,none,none);
	
	feature_accessor(storage,none,none);
	feature_accessor(accent_storage,none,none);
	
	feature_accessor(draw,none,none);
	
	feature_accessor(poly,none,none);

	feature_accessor(primitive_type,none,none);
	feature_accessor(head_size_value,none,none);
	feature_accessor(head_accent_size_value,none,none);

	feature_accessor(head_normal,none,none);

	feature_accessor(head_texture,none,none);	
	feature_accessor(texture_map,none,none);

	feature_accessor(grouped,none,none);
	feature_accessor(group_color,none,none);
	feature_accessor(group_normal,none,none);
	feature_accessor(group_texture,none,none);
	feature_accessor(head_color,none,none);


	feature_accessor(primitive_color,none,none);
	feature_accessor(primitive_normal,none,none);
	feature_accessor(primitive_texture,none,none);
	feature_accessor(primitive_stride,none,none);
	feature_accessor(vert_stride,none,none);	
	feature_accessor(vert_size,none,none);	
	
	//feature_accessor(attributes_schema,none,none);
	//feature_accessor(selection_callback,none,none);
	//feature_accessor(submission_callback,none,none);

	feature_accessor(data_stride,none,none);

	feature_accessor(attributes_panel,none,none);
	feature_accessor(layer_options,none,none);

	feature_accessor(selected_elements,none,none);

	feature_accessor(x_label,none,none);
	feature_accessor(y_label,none,none);
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
		Configure_Default();
	}

	int Add_Texture(string& filename)
	{
		textures.push_back(filename);

		return textures.size();
	}

	void Configure_Default()
	{
		draw=false;
		dynamic_data=false;

		storage_offset=_iteration;
		storage_size=1;
		storage_period=END;
		
		head_texture = 0;

		primitive_color=false;
		head_color._r=0;
		head_color._g=0;
		head_color._b=0;
		head_color._a=255;
		
		//head_accent_color._r=255;
		//head_accent_color._g=255;
		//head_accent_color._b=255;
		//head_accent_color._a=255;

		primitive_normal=false;
		head_normal._x=0;
		head_normal._y=0;
		head_normal._z=1;
		
		primitive_texture=false;

		grouped=false;
		group_color=false;
		group_normal=false;
		group_texture=false;

		head_size_value=1;
		head_accent_size_value=1;

		submission_callback=nullptr;
		selection_callback=nullptr;
		double_click_callback=nullptr;
	}
	
	void Configure_Static_Lines()
	{
		Configure_Default();
		primitive_type=_LINE;

		head_color._r=128;
		head_color._g=128;
		head_color._b=128;
		head_color._a=255;
	}
	
	void Configure_Dynamic_Lines()
	{
		Configure_Default();
		dynamic_data=true;
		storage_size=4;
		storage_period=1;

		primitive_type=_LINE;

		head_color._r=128;
		head_color._g=128;
		head_color._b=128;
		head_color._a=255;
	}

	void Configure_Dynamic_Points()
	{
		Configure_Default();
		dynamic_data=true;

		storage_offset=_iteration;
		storage_size=4;
		storage_period=1;

		primitive_type=_POINT;

		head_color._r=90;
		head_color._g=124;
		head_color._b=194;
		head_color._a=255;
		
		head_size_value=4;
	}

	void Configure_Static_Points()
	{
		Configure_Default();
		primitive_type=_POINT;
	}

	void Configure_Static_Points(True_Color_RGBA<NULLTYPE>& Color, int size)
	{
		Configure_Default();
		primitive_type=_POINT;

		head_color._r=Color._r;
		head_color._g=Color._g;
		head_color._b=Color._b;
		head_color._a=Color._a;
		
		head_size_value=size;
	}
	
	void Configure_Static_Quads(True_Color_RGBA<NULLTYPE>& Color, int size)
	{
		Configure_Default();
		primitive_type=_QUAD;

		head_color._r=Color._r;
		head_color._g=Color._g;
		head_color._b=Color._b;
		head_color._a=Color._a;
		
		grouped=true;
		group_color=false;
		group_normal=false;
	}
	
	void Configure_Static_Polygons()
	{
		Configure_Default();
		primitive_type = _POLYGON;
		grouped = true;
		group_color=true;
		group_normal=false;
	}

	void Configure_Dynamic_Quads(True_Color_RGBA<NULLTYPE>& Color, int size)
	{
		Configure_Default();
		dynamic_data=true;
		storage_size=4;
		storage_period=1;

		primitive_type=_QUAD;

		head_color._r=Color._r;
		head_color._g=Color._g;
		head_color._b=Color._b;
		head_color._a=Color._a;
	}

	void Configure_Plot()
	{
		Configure_Default();
		
		draw=true;
		dynamic_data=true;

		storage_size=4;
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
	int head_texture;

	//True_Color_RGBA<NULLTYPE> head_accent_color;
	int head_size_value;
	int head_accent_size_value;

	bool grouped;
		bool group_color;
			// 1 x True_Color_RGBA<NULLTYPE> group_color;
		bool group_normal;
			// 1 x Point_3D<NULLTYPE> group_normal;
		bool group_texture;
			// 1 x int group_texture;	
	
	bool primitive_color;
		// 1 x True_Color_RGBA<NULLTYPE> primitive_color;
	bool primitive_normal;
		// 1 x Point_3D<NULLTYPE> primitive_normal;
	bool primitive_texture;
		// 1 x int primitive_texture;	

	submission_callback_type submission_callback;
	selection_callback_type selection_callback;
	double_click_callback_type double_click_callback;

	string x_label;
	string y_label;

	vector<string> textures;
};
//*********************************************************
//	Antares_Layer.h - Graphical Layer
//*********************************************************

#pragma once
#include "Dependencies.h"
#include "Control_Dialog.h"
#include "Geometry_Implementation.h"

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
	
	template<typename TargetType> void Push_Element(void* data, int iteration = iteration())
	{
		this_component()->Push_Element<TargetType>(data,iteration);
	}

	template<typename TargetType> void Initialize(Antares_Layer_Configuration& cfg)
	{
		this_component()->Initialize<TargetType>(cfg);
	}

	template<typename TargetType> void Reschedule_Execution(int new_iteration)
	{
		this_component()->Reschedule_Execution<TargetType>(new_iteration);
	}
	
	bool Identify_One(Point_3D<typename Component_Type::Master_Type>& point, int start_iteration, int end_iteration, ANTARES_SELECTION_MODE mode)
	{
		return this_component()->Identify_One(point,start_iteration,end_iteration,mode);
	}
	
	template<typename TargetType> void Select()
	{
		this_component()->Select<TargetType>();
	}
	
	template<typename TargetType> void Deselect_All()
	{
		this_component()->Deselect_All<TargetType>();
	}
	
	template<typename TargetType> void Double_Click()
	{
		this_component()->Double_Click<TargetType>();
	}
	
	template<typename TargetType> void Clear_Accented()
	{
		this_component()->Clear_Accented<TargetType>();
	}

	template<typename TargetType> void Refresh_Selection()
	{
		this_component()->Refresh_Selection<TargetType>();
	}

	accessor(list_index, NONE, NONE);
	accessor(name, NONE, NONE);
	
	accessor(storage, NONE, NONE);
	accessor(accent_storage, NONE, NONE);
	
	accessor(draw, NONE, NONE);
	
	accessor(poly, NONE, NONE);

	accessor(primitive_type, NONE, NONE);
	accessor(head_size_value, NONE, NONE);
	accessor(head_accent_size_value, NONE, NONE);

	accessor(head_normal, NONE, NONE);

	accessor(head_texture, NONE, NONE);	
	accessor(texture_map, NONE, NONE);

	accessor(grouped, NONE, NONE);
	accessor(group_color, NONE, NONE);
	accessor(group_normal, NONE, NONE);
	accessor(group_texture, NONE, NONE);
	accessor(head_color, NONE, NONE);


	accessor(primitive_color, NONE, NONE);
	accessor(primitive_normal, NONE, NONE);
	accessor(primitive_texture, NONE, NONE);
	accessor(primitive_stride, NONE, NONE);
	accessor(vert_stride, NONE, NONE);	
	accessor(vert_size, NONE, NONE);	
	
	accessor(pixel_size_callback, NONE, NONE);
	accessor(accent_pixel_size_callback, NONE, NONE);

	//feature_accessor(attributes_schema,none,none);
	//feature_accessor(selection_callback,none,none);
	//feature_accessor(submission_callback,none,none);

	accessor(data_stride, NONE, NONE);

	accessor(attributes_panel, NONE, NONE);
	accessor(layer_options, NONE, NONE);

	accessor(selected_elements, NONE, NONE);

	accessor(x_label, NONE, NONE);
	accessor(y_label, NONE, NONE);

	accessor(default_texture_alpha, NONE, NONE);
};

template<typename MasterType>
Antares_Layer<typename MasterType::antares_layer_type>* Allocate_New_Layer(string& name)
{
	return (Antares_Layer<typename MasterType::antares_layer_type>*)((typename MasterType::canvas_type*)canvas)->Allocate_New_Layer<MasterType>(name);
}

template<typename ComponentType>
bool Clip_Coordinates(Point_3D<typename ComponentType::Master_Type>& coordinates)
{
	return ((ComponentType*)canvas)->Clip_Coordinates<NT,NT>(coordinates);
}

template<typename ComponentType>
bool Clip_Coordinates(Point_3D<NT>& coordinates)
{
	return ((ComponentType*)canvas)->Clip_Coordinates<NT>((Point_3D<ComponentType::Master_Type>&)coordinates);
}

template<typename MasterType>
typename Point_3D<MasterType>& Scale_Coordinates(Point_3D<MasterType>& coordinates)
{
	return (Point_3D<MasterType>&)((typename MasterType::canvas_type*)canvas)->Scale_Coordinates<MasterType>(coordinates);
}

template<typename MasterType>
typename Point_3D<NT>& Scale_Coordinates(Point_3D<NT>& coordinates)
{
	return (Point_3D<NT>&)((typename MasterType::canvas_type*)canvas)->Scale_Coordinates<NT>(coordinates);
}

template<typename MasterType>
Antares_Layer<typename MasterType::antares_layer_type>* Allocate_New_Plot_Layer(string& name)
{
	return (Antares_Layer<typename MasterType::antares_layer_type>*)((typename MasterType::information_panel_type*)information_panel)->Allocate_New_Layer<MasterType>(name);
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

		storage_offset=iteration();
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

		pixel_size_callback=nullptr;
		accent_pixel_size_callback=nullptr;

		submission_callback=nullptr;
		selection_callback=nullptr;
		double_click_callback=nullptr;

		default_texture_alpha=255;
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

		storage_offset=iteration();
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
	
	void Configure_Static_Quads()
	{
		Configure_Default();
		primitive_type=_QUAD;
		group_color=false;
		group_normal=false;
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
	
	void Configure_Static_Triangles(True_Color_RGBA<NULLTYPE>& Color, int size)
	{
		Configure_Default();
		primitive_type=_TRIANGLE;

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
	unsigned char default_texture_alpha;

	bool primitive_color;
		// 1 x True_Color_RGBA<NULLTYPE> primitive_color;
	bool primitive_normal;
		// 1 x Point_3D<NULLTYPE> primitive_normal;
	bool primitive_texture;
		// 1 x int primitive_texture;	

	submission_callback_type submission_callback;
	selection_callback_type selection_callback;
	double_click_callback_type double_click_callback;

	pixel_size_callback_type pixel_size_callback;
	pixel_size_callback_type accent_pixel_size_callback;

	string x_label;
	string y_label;

	std::vector<string> textures;
};

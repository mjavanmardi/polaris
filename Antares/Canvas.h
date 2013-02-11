//*********************************************************
//	Canvas.h - Visualization Panel
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "Antares_Layer.h"

//---------------------------------------------------------
//	Canvas - canvas class definition
//---------------------------------------------------------

prototype struct Canvas
{
	tag_as_prototype;

	feature_prototype void Initialize()
	{
		this_component()->Initialize<ComponentType,CallerType,TargetType>();
	}

	feature_prototype void Refresh()
	{
		this_component()->Refresh();
	}
	
	feature_prototype typename TargetType::ReturnType Allocate_New_Layer(typename TargetType::ParamType name)
	{
		return (TargetType::ReturnType)this_component()->Allocate_New_Layer<ComponentType,CallerType,TargetType>(name);
	}

	//feature_accessor(cached_iteration,none,none);
	
	//feature_accessor(glcontext,none,none);

	//feature_accessor(canvas_bounds,none,none);
	//feature_accessor(near_plane,none,none);
	//feature_accessor(far_plane,none,none);

	//feature_accessor(win_z_mid,none,none);

	//feature_accessor(panel_width,none,none);
	//feature_accessor(panel_height,none,none);
	//feature_accessor(scale,none,none);

	//feature_accessor(x_rotation,none,none);
	//feature_accessor(z_rotation,none,none);
	//feature_accessor(x_translation,none,none);
	//feature_accessor(y_translation,none,none);
	
	//feature_accessor(meters_per_pixel_max,none,none);
	//feature_accessor(meters_per_pixel_mid,none,none);
	//feature_accessor(meters_per_pixel_min,none,none);

	//feature_accessor(view_bounds,none,none);

	//feature_accessor(spatial_change,none,none);
	//feature_accessor(temporal_change,none,none);

	//feature_accessor(left_down,none,none);
	//feature_accessor(right_down,none,none);

	//feature_accessor(x_start_utm,none,none);
	//feature_accessor(y_start_utm,none,none);

	//feature_accessor(x_start_win,none,none);
	//feature_accessor(y_start_win,none,none);

	//feature_accessor(wheel_dir,none,none);

	//feature_accessor(viewport,none,none);
	//feature_accessor(modelview,none,none);
	//feature_accessor(projection,none,none);

	feature_accessor(time_panel,none,none);
	feature_accessor(information_panel,none,none);

	feature_accessor(graphical_network,none,none);
};
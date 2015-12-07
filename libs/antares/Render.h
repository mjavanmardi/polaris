//*********************************************************
//	Render.h - Primary Render Function
//*********************************************************

#pragma once
#include "Canvas_Implementation.h"

//---------------------------------------------------------
//	Render
//--------------------------------------------------------

static int image_counter = 0;

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::Render(wxPaintEvent& event)
{
	LOCK(_canvas_lock);

	Add_Textures();

	//---- clear and initialize drawing ----

	int current_iteration = iteration() - 1;

	if(current_iteration != _cached_iteration)
	{
		_temporal_change=true;
		_cached_iteration=current_iteration;

		if(current_iteration>=0)
		{
			_time_panel->Update_Time<NULLTYPE>(current_iteration);
		}

		//if(_selected_layer != nullptr)
		//{
		//	_selected_layer->Refresh_Selection<NT>();
		//}
	}

	wxGLCanvas::SetCurrent(*_glcontext);
	wxPaintDC(this);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//---- set up the current canvas viewing settings ----

	glViewport(0,0,_panel_width,_panel_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(2.0*_scale,_panel_width/_panel_height,_near_plane,_far_plane);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-(_far_plane+_near_plane)/2.0);

	glRotatef(_x_rotation,1,0,0);
	glRotatef(_z_rotation,0,0,1);
	glTranslated(_x_translation,_y_translation,0);

	//---- determine viewing trapezoid, warning: clears current canvas if pixel reading is enabled ----

	if(_spatial_change) Calculate_Bounds();

	//---- loop over and draw all layers ----

	std::list<Antares_Layer_Interface*>::iterator itr;

	for(itr=_layers.begin();itr!=_layers.end();itr++)
	{
		Draw_Layer(current_iteration,current_iteration,(*itr));
	}
	
	UNLOCK(_canvas_lock);
	
	//---- flush and display ----

	glFlush();
	SwapBuffers();

	if(_recording)
	{
		Record_Frame();
	}


	//---- indicate the spatial and temporal aspects of the rendering have been updated ----

	_spatial_change=_temporal_change=false;
}

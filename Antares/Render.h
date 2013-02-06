//*********************************************************
//	Render.h - Primary Render Function
//*********************************************************

#pragma once
#include "Canvas_Implementation.h"

//---------------------------------------------------------
//	Render
//--------------------------------------------------------

template<typename MasterType,typename ParentType>
void Canvas_Implementation<MasterType,ParentType>::Render(wxPaintEvent& event)
{
	//wxGLCanvas::SetCurrent(*_glcontext);
	//wxPaintDC(this);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	////Test Code Works
	//glViewport(0,0,_panel_width,_panel_height);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//
	//gluPerspective(45.0f,(GLfloat)_panel_width/(GLfloat)_panel_height,0.1f,100.0f);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//
	//glColor4f(1,0,0,1);

	//glTranslatef(-1.5f,0.0f,-6.0f);

	//glBegin(GL_TRIANGLES);                      // Drawing Using Triangles
	//glVertex3f( 0.0f, 1.0f, 0.0f);              // Top
	//glVertex3f(-1.0f,-1.0f, 0.0f);              // Bottom Left
	//glVertex3f( 1.0f,-1.0f, 0.0f);              // Bottom Right
	//glEnd();                            // Finished Drawing The Triangle
	//	
	//glTranslatef(3.0f,0.0f,0.0f);                   // Move Right 3 Units

	//glBegin(GL_QUADS);                      // Draw A Quad
	//glVertex3f(-1.0f, 1.0f, 0.0f);              // Top Left
	//glVertex3f( 1.0f, 1.0f, 0.0f);              // Top Right
	//glVertex3f( 1.0f,-1.0f, 0.0f);              // Bottom Right
	//glVertex3f(-1.0f,-1.0f, 0.0f);              // Bottom Left
	//glEnd();                            // Done Drawing The Quad
	//
	//glFlush();
	//SwapBuffers();

	//---- clear and initialize drawing ----

	int current_iteration = _iteration - 1;

	if(current_iteration!=_cached_iteration)
	{
		_temporal_change=true;
		_cached_iteration=current_iteration;

		if(current_iteration>=0) _time_panel->Update_Time<NULLTYPE>(current_iteration);
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

	//---- detailed network is opaque, draw front to back ----

	Draw_Network();
	
	//---- detailed network is opaque, draw front to back ----

	Draw_Vehicles(current_iteration);
	
	//---- flush and display ----

	glFlush();
	SwapBuffers();

	//---- indicate the spatial and temporal aspects of the rendering have been updated ----

	_spatial_change=_temporal_change=false;
}
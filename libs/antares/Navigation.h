//*********************************************************
//	Navigation.h - Navigation Functions
//*********************************************************

#pragma once
#include "Canvas_Implementation.h"

//---------------------------------------------------------
//	Set_Mode - handles mode change requests
//---------------------------------------------------------

//template<typename MasterType,typename ParentType,typename InheritanceList>
//template<typename ComponentType,typename CallerType,typename TargetType>
//void Canvas_Implementation<MasterType,ParentType,InheritanceList>::Set_Mode(ANTARES_MODE mode)
//{
//	_interaction_mode=mode;
//
//	if(mode==IDENTIFY)
//	{
//		Connect(wxEVT_MOTION,wxMouseEventHandler(Canvas_Implementation::OnMotion));
//	}
//	else
//	{
//		Disconnect(wxEVT_MOTION,wxMouseEventHandler(Canvas_Implementation::OnMotion));
//		_selected_layer->Deselect<NULLTYPE>();
//	}
//
//	Refresh();
//}

//---------------------------------------------------------
//	OnResize - handles when canvas is resized
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::OnResize(wxSizeEvent& event)
{
	_panel_width=(float)GetSize().x;
	_panel_height=(float)GetSize().y;
	
	_spatial_change=true;
	Refresh();
}

//---------------------------------------------------------
//	OnKeyDown - initialize one of several modes
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::OnKeyDown(wxKeyEvent& event)
{
	if(event.GetModifiers() == wxMOD_ALT)
	{
		if(_selected_layer != nullptr) _selected_layer->Deselect_All<NULLTYPE>();
		_alt_down=true;
		Connect(wxEVT_MOTION,wxMouseEventHandler(Canvas_Implementation::OnMotion));
		Connect(wxEVT_KEY_UP,wxKeyEventHandler(Canvas_Implementation::OnKeyUp));
		Disconnect(wxEVT_KEY_DOWN,wxKeyEventHandler(Canvas_Implementation::OnKeyDown));
	}
	else if(event.GetModifiers() == wxMOD_CONTROL)
	{
		_ctrl_down=true;
		//Connect(wxEVT_MOTION,wxMouseEventHandler(Canvas_Implementation::OnMotion));
		Connect(wxEVT_KEY_UP,wxKeyEventHandler(Canvas_Implementation::OnKeyUp));
		Disconnect(wxEVT_KEY_DOWN,wxKeyEventHandler(Canvas_Implementation::OnKeyDown));
	}
}

//---------------------------------------------------------
//	OnKeyUp - terminate one of several modes
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::OnKeyUp(wxKeyEvent& event)
{
	if(_alt_down)
	{
		_alt_down=false;
		Disconnect(wxEVT_MOTION,wxMouseEventHandler(Canvas_Implementation::OnMotion));
		Disconnect(wxEVT_KEY_UP,wxKeyEventHandler(Canvas_Implementation::OnKeyUp));
		Connect(wxEVT_KEY_DOWN,wxKeyEventHandler(Canvas_Implementation::OnKeyDown));
		if(_selected_layer != nullptr) _selected_layer->Deselect_All<NULLTYPE>();
		Refresh();
	}
	else if(_ctrl_down)
	{
		_ctrl_down=false;
		//Disconnect(wxEVT_MOTION,wxMouseEventHandler(Canvas_Implementation::OnMotion));
		//if(_selected_layer != nullptr) _selected_layer->Deselect_All<NULLTYPE>();
		Disconnect(wxEVT_KEY_UP,wxKeyEventHandler(Canvas_Implementation::OnKeyUp));
		Connect(wxEVT_KEY_DOWN,wxKeyEventHandler(Canvas_Implementation::OnKeyDown));
		Refresh();
	}
}

//---------------------------------------------------------
//	OnLeftDown - initialize panning procedure or selection
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::OnLeftDown(wxMouseEvent& event)
{
	//_left_down=true;

	SetFocus();

	int x_pos,y_pos;

	GLdouble winX, winY, winZ;
	GLdouble posX, posY, posZ;

	//---- load the current opengl matrices ----
	
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	//---- get the mouse position and figure out the z coordinate ----
	
	event.GetPosition(&x_pos, &y_pos);

	winX = (double)x_pos;
	winY = (double)viewport[3] - (double)y_pos;
	
	double tangent=tan((2.0*_scale)/2.0*(3.14159265/180.0));
	double near_height=_near_plane*tangent;
	//double near_width=_far_plane*(_panel_width/_panel_height);
	double far_height=(_far_plane/_near_plane)*near_height;
	double theta=-_x_rotation*(3.14159265/180.0);
	
	double ymax_screen=((far_height-near_height)/(_far_plane-_near_plane)*((_near_plane+_far_plane)/2.0-_near_plane)+near_height)/(1.0-(far_height-near_height)/(_far_plane-_near_plane)*sin(theta)/cos(theta));
	double ymin_screen=((near_height-far_height)/(_far_plane-_near_plane)*((_near_plane+_far_plane)/2.0-_near_plane)-near_height)/(1.0-(near_height-far_height)/(_far_plane-_near_plane)*sin(theta)/cos(theta));

	double glY=((ymin_screen-ymax_screen)/_panel_height)*(double)y_pos+ymax_screen;
	
	double z_dist=sin(theta)/cos(theta)*glY+(_near_plane+_far_plane)/2.0;

	winZ=(1.0/_near_plane-1.0/z_dist)/(1.0/_near_plane-1.0/_far_plane);

	//---- convert to world coordinates ----

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	_x_start_utm=(float)posX;
	_y_start_utm=(float)posY;
	
	
	if(_selected_layer != nullptr && _ctrl_down && !_time_panel->Is_Running<NT>())
	{
		Point_3D<MasterType> location;
		location._x=(float)posX;
		location._y=(float)posY;
		location._z=(float)posZ;

		if(_selected_layer->Identify_One(location,_cached_iteration,_cached_iteration,CTRL_DOWN))
		{
			Refresh();
			_information_panel->Render<NT>();
		}
	}

	Connect(wxEVT_MOTION,wxMouseEventHandler(Canvas_Implementation::OnMotion));
}

//---------------------------------------------------------
//	OnMotion - mouse motion calculations
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::OnMotion(wxMouseEvent& event)
{
	SetFocus();

	int x_pos, y_pos;
	event.GetPosition(&x_pos,&y_pos);

	GLdouble winX, winY, winZ;
	GLdouble posX, posY, posZ;

	//---- load the current opengl matrices ----
	
	if(!event.LeftIsDown())
	{
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		glGetIntegerv(GL_VIEWPORT, viewport);
	}

	//---- get the mouse position and figure out the z coordinate ----

	winX = (double)x_pos;
	winY = (double)viewport[3] - (double)y_pos;

	//---- manually calculate near and far plane properties ----

	double tangent=tan((2.0*_scale)/2.0*(3.14159265/180.0));
	double near_height=_near_plane*tangent;
	//double near_width=_far_plane*(_panel_width/_panel_height);
	double far_height=(_far_plane/_near_plane)*near_height;
	double theta=-_x_rotation*(3.14159265/180.0);

	

	//---- figure out the viewport's maximum and minimum object coordinates (not window, not utm) ----

	double ymax_screen=((far_height-near_height)/(_far_plane-_near_plane)*((_near_plane+_far_plane)/2.0-_near_plane)+near_height)/(1.0-(far_height-near_height)/(_far_plane-_near_plane)*sin(theta)/cos(theta));
	double ymin_screen=((near_height-far_height)/(_far_plane-_near_plane)*((_near_plane+_far_plane)/2.0-_near_plane)-near_height)/(1.0-(near_height-far_height)/(_far_plane-_near_plane)*sin(theta)/cos(theta));

	//---- convert window Y position to object y position ----

	double glY=((ymin_screen-ymax_screen)/_panel_height)*(double)y_pos+ymax_screen;

	//---- convert window Y position to object y position ----
	
	double z_dist=sin(theta)/cos(theta)*glY+(_near_plane+_far_plane)/2.0;

	winZ=(1.0/_near_plane-1.0/z_dist)/(1.0/_near_plane-1.0/_far_plane);
	
	//cout << _scale << "," << theta << "," << y_pos << "," << ymax_screen << "," << ymin_screen << "," << glY << "," << z_dist << "," << winZ << endl;

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	if(event.LeftIsDown())
	{
		//---- change the "world" translation by the amount moved ----

		_x_translation += posX - _x_start_utm;
		_y_translation += posY - _y_start_utm;

		//---- update start position to current position ----

		_x_start_utm=posX;
		_y_start_utm=posY;

		_spatial_change=true;

		Refresh();
	}
	else
	{
		if(_selected_layer != nullptr && _alt_down && !_time_panel->Is_Running<NT>())
		{
			Point_3D<MasterType> location;
			location._x=(float)posX;
			location._y=(float)posY;
			location._z=(float)posZ;

			if(_selected_layer->Identify_One(location,_cached_iteration,_cached_iteration,ALT_DOWN))
			{
				Refresh();
				_information_panel->Render<NT>();
			}
		}
	}
}

//---------------------------------------------------------
//	OnLeftUp - stop panning
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::OnLeftUp(wxMouseEvent& event)
{
	//_left_down=false;

	Disconnect(wxEVT_MOTION,wxMouseEventHandler(Canvas_Implementation::OnMotion));
}

//---------------------------------------------------------
//	OnRightDown - initialize rotation procedure
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::OnRightDown(wxMouseEvent& event)
{
	//_right_down=true;

	SetFocus();

	event.GetPosition(&_x_start_win,&_y_start_win);
	Connect(wxEVT_MOTION,wxMouseEventHandler(Canvas_Implementation::OnRotationMotion));
}

//---------------------------------------------------------
//	OnRotationMotion - rotation calculations
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::OnRotationMotion(wxMouseEvent& event)
{
	SetFocus();

	int x_end;
	int y_end;
	event.GetPosition(&x_end,&y_end);
	
	float zScale= 135.0/_panel_width;
	float yScale= 135.0/_panel_height;

	_x_rotation=_x_rotation+(y_end-_y_start_win)*yScale;

	//---- keep the rotation between 0, 360 ----

	if(_x_rotation>=360.0) _x_rotation-=360.0;
	else if(_x_rotation<0) _x_rotation+=360.0;

	//---- cap the maximum tilt ----

	if(_x_rotation<90.0) _x_rotation=360.0;
	else if(_x_rotation<270.0) _x_rotation=270.0;

	_z_rotation=_z_rotation+(x_end-_x_start_win)*zScale;
	
	//---- keep the rotation between 0, 360 ----

	if(_z_rotation>=360.0) _z_rotation-=360.0;
	else if(_z_rotation<0) _z_rotation+=360.0;

	_x_start_win=x_end;
	_y_start_win=y_end;

	_spatial_change=true;
	Refresh();
}

//---------------------------------------------------------
//	OnRightUp - stop rotating
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::OnRightUp(wxMouseEvent& event)
{
	//_right_down=false;
	Disconnect(wxEVT_MOTION,wxMouseEventHandler(Canvas_Implementation::OnRotationMotion));
}

//---------------------------------------------------------
//	OnWheel - zoom calculations
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::OnWheel(wxMouseEvent& event)
{
	SetFocus();

	_wheel_dir=event.GetWheelRotation();
		
	if(_wheel_dir>0)
	{
		_scale=_scale*.9;
		_spatial_change=true;
		Refresh();
	}
	else if(_scale*1.0/.9 < _initial_scale*1.2)
	{
		_scale=_scale*(1.0/.9);
		_spatial_change=true;
		Refresh();
	}
}

//---------------------------------------------------------
//	OnLeave - stop motion
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::OnLeave(wxMouseEvent& event)
{
	//_left_down=false;
	//_right_down=false;

	Disconnect(wxEVT_MOTION,wxMouseEventHandler(Canvas_Implementation::OnMotion));
	Disconnect(wxEVT_MOTION,wxMouseEventHandler(Canvas_Implementation::OnRotationMotion));
}

//---------------------------------------------------------
//	OnEnter - resume motion
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::OnEnter(wxMouseEvent& event)
{
	//if(_interaction_mode == IDENTIFY)
	//{
	//	Connect(wxEVT_MOTION,wxMouseEventHandler(Canvas_Implementation::OnMotion));
	//}
}

//---------------------------------------------------------
//	OnDClick - various double clicking actions
//---------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::OnDClick(wxMouseEvent& event)
{
	//if(_interaction_mode == IDENTIFY)
	//{
		if(_selected_layer != nullptr && !_time_panel->Is_Running<NT>())
		{
			_selected_layer->Double_Click<NULLTYPE>();
		}
	//}
}

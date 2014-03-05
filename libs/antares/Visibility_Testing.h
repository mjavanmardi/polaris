//*********************************************************
//	Visibility_Testing.h - Functions for testing visibility
//*********************************************************

#pragma once
#include "Canvas_Implementation.h"

//---------------------------------------------------------
//	Calculate_Bounds
//--------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::Calculate_Bounds()
{
	GLdouble modelview_matrix[16], projection_matrix[16];
	GLint viewport_matrix[4];
	
	GLdouble top_x_left, top_x_right, mid_x_left, mid_x_right, bottom_x_left, bottom_x_right;
	GLdouble top_y_left, top_y_right, mid_y_left, mid_y_right, bottom_y_left, bottom_y_right;

	GLdouble winX, winY, winZ;
	GLdouble posZ;
	
	double posY;
	double glY;
	double z_dist;

	//---- load the current opengl matrices ----

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);
	glGetIntegerv(GL_VIEWPORT, viewport_matrix);

	//---- compute near and far plane heights ----
	
    double tangent=tan((2.0*_scale)/2.0*(3.14159265/180.0));
    double near_height=_near_plane*tangent;
    //double near_width=far_plane*(panel_width/panel_height);
	double far_height=(_far_plane/_near_plane)*near_height;
	double theta=-_x_rotation*(3.14159265/180.0);
	
	double ymax_screen=((far_height-near_height)/(_far_plane-_near_plane)*((_near_plane+_far_plane)/2.0-_near_plane)+near_height)/(1.0-(far_height-near_height)/(_far_plane-_near_plane)*sin(theta)/cos(theta));
	double ymin_screen=((near_height-far_height)/(_far_plane-_near_plane)*((_near_plane+_far_plane)/2.0-_near_plane)-near_height)/(1.0-(near_height-far_height)/(_far_plane-_near_plane)*sin(theta)/cos(theta));

	//---- figure out the z coordinate for the mid-line, left ----

	winX = (double)0;
	winY = (double)viewport_matrix[3]*.5;
	
	if(_win_z_mid==-1)
	{
		posY=(double)viewport_matrix[3]*.5;

		glY=((ymin_screen-ymax_screen)/_panel_height)*posY+ymax_screen;
	
		z_dist=sin(theta)/cos(theta)*glY+(_near_plane+_far_plane)/2.0;

		_win_z_mid=(1.0/_near_plane-1.0/z_dist)/(1.0/_near_plane-1.0/_far_plane);
	}
	
	winZ = (double)_win_z_mid;
	
	gluUnProject(winX, winY, winZ, modelview_matrix, projection_matrix, viewport_matrix, &mid_x_left, &mid_y_left, &posZ);
	
	//---- figure out the z coordinate for the mid-line, right ----

	winX = (double)viewport_matrix[2];
	winY = (double)viewport_matrix[3]*.5;
	winZ = (double)_win_z_mid;

	gluUnProject(winX, winY, winZ, modelview_matrix, projection_matrix, viewport_matrix, &mid_x_right, &mid_y_right, &posZ);

	//---- figure out the z coordinate for the bottom-line, left ----

	winX = (double)0;
	winY = (double)0;

	posY=_panel_height;

	glY=((ymin_screen-ymax_screen)/_panel_height)*posY+ymax_screen;
	
	z_dist=sin(theta)/cos(theta)*glY+(_near_plane+_far_plane)/2.0;

	winZ=(1.0/_near_plane-1.0/z_dist)/(1.0/_near_plane-1.0/_far_plane);
	
	gluUnProject(winX, winY, winZ, modelview_matrix, projection_matrix, viewport_matrix, &bottom_x_left, &bottom_y_left, &posZ);

	//---- figure out the z coordinate for the top-line, left ----

	winX = (double)0;
	winY = (double)viewport_matrix[3];

	posY=0;

	glY=((ymin_screen-ymax_screen)/_panel_height)*posY+ymax_screen;
	
	z_dist=sin(theta)/cos(theta)*glY+(_near_plane+_far_plane)/2.0;

	winZ=(1.0/_near_plane-1.0/z_dist)/(1.0/_near_plane-1.0/_far_plane);

	gluUnProject(winX, winY, winZ, modelview_matrix, projection_matrix, viewport_matrix, &top_x_left, &top_y_left, &posZ);

	//---- calculate remaining trapezoid points ----

	double dist=(double)sqrt(pow((double)mid_x_left-(double)bottom_x_left,2.0)+pow((double)mid_y_left-(double)bottom_y_left,2.0));

	double diff=atan2(bottom_y_left-mid_y_left,bottom_x_left-mid_x_left)-atan2(mid_y_right-mid_y_left,mid_x_right-mid_x_left);

	bottom_x_right=dist*cos(atan2(mid_y_left-mid_y_right,mid_x_left-mid_x_right)-diff)+mid_x_right;
	bottom_y_right=dist*sin(atan2(mid_y_left-mid_y_right,mid_x_left-mid_x_right)-diff)+mid_y_right;

	dist=(double)sqrt(pow((double)mid_x_left-(double)top_x_left,2.0)+pow((double)mid_y_left-(double)top_y_left,2.0));

	top_x_right=dist*cos(atan2(mid_y_left-mid_y_right,mid_x_left-mid_x_right)-diff+M_PI)+mid_x_right;
	top_y_right=dist*sin(atan2(mid_y_left-mid_y_right,mid_x_left-mid_x_right)-diff+M_PI)+mid_y_right;

	//---- compute ratio of utm to pixels and bounding box ----

	double top_width=(double)sqrt(pow((double)top_x_right-(double)top_x_left,2.0)+pow((double)top_y_right-(double)top_y_left,2.0));
	double left_height=(double)sqrt(pow((double)top_x_left-(double)bottom_x_left,2.0)+pow((double)top_y_right-(double)bottom_y_right,2.0));
	double bottom_width=(double)sqrt(pow((double)bottom_x_right-(double)bottom_x_left,2.0)+pow((double)bottom_y_right-(double)bottom_y_left,2.0));
	double mid_width=(double)sqrt(pow((double)mid_x_right-(double)mid_x_left,2.0)+pow((double)mid_y_right-(double)mid_y_left,2.0));

	_meters_per_pixel_max=max((double)(top_width/(double)viewport_matrix[2]),(double)(left_height/(double)viewport_matrix[3]));
	_meters_per_pixel_mid=(double)(mid_width/(double)viewport_matrix[2]);
	_meters_per_pixel_min=min((double)(bottom_width/(double)viewport_matrix[2]),(double)(left_height/(double)viewport_matrix[3]));

	_view_bounds._xmin=min(min(min(min(min(mid_x_left,mid_x_right),bottom_x_left),bottom_x_right),top_x_left),top_x_right);
	_view_bounds._xmax=max(max(max(max(max(mid_x_left,mid_x_right),bottom_x_left),bottom_x_right),top_x_left),top_x_right);

	_view_bounds._ymin=min(min(min(min(min(mid_y_left,mid_y_right),bottom_y_left),bottom_y_right),top_y_left),top_y_right);
	_view_bounds._ymax=max(max(max(max(max(mid_y_left,mid_y_right),bottom_y_left),bottom_y_right),top_y_left),top_y_right);
	
	//bounds.clear();

	//Vertex vert;

	//vert.z=0;

	//vert.x=top_x_right;
	//vert.y=top_y_right;
	//
	//bounds.push_back(vert);

	//vert.x=top_x_left;
	//vert.y=top_y_left;
	//
	//bounds.push_back(vert);

	//vert.x=bottom_x_left;
	//vert.y=bottom_y_left;
	//
	//bounds.push_back(vert);

	//vert.x=bottom_x_right;
	//vert.y=bottom_y_right;
	//
	//bounds.push_back(vert);
}

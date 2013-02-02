//*********************************************************
//	Draw_Network.h - Function for drawing the network
//*********************************************************

#pragma once
#include "Canvas_Implementation.h"

//---------------------------------------------------------
//	Draw_Network
//--------------------------------------------------------

template<typename MasterType,typename ParentType>
void Canvas_Implementation<MasterType,ParentType>::Draw_Network()
{
	vector<Point_2D<MasterType>>* links=_graphical_network->link_lines<vector<Point_2D<MasterType>>*>();
	vector<Point_2D<MasterType>>::iterator itr;
	
	glBegin(GL_LINES);
	
	glColor4ub(255,0,0,255);

	for(itr=links->begin();itr!=links->end();itr++) glVertex2fv((GLfloat*)&(*itr));

	glEnd();
}
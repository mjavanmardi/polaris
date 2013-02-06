//*********************************************************
//	Draw_Vehicles.h - Function for drawing the vehicles
//*********************************************************

#pragma once
#include "Canvas_Implementation.h"

//---------------------------------------------------------
//	Draw_Network
//--------------------------------------------------------

template<typename MasterType,typename ParentType>
void Canvas_Implementation<MasterType,ParentType>::Draw_Vehicles(int current_iteration)
{
	if(current_iteration >= 0)
	{
		Double_Buffer< vector< Point_3D<MasterType> >[_num_threads] >& vehs_db=_graphical_network->vehicle_coordinates< Double_Buffer< vector< Point_3D<MasterType> >[_num_threads] >& >();

		vector<Point_3D<MasterType>> (&vehicles)[_num_threads] = vehs_db[current_iteration];

		vector<Point_3D<MasterType>>::iterator itr;
		
		
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);
		glPointSize(3);
		
		glBegin(GL_POINTS);

		glColor4ub(0,255,0,255);

		for(int i=0;i<_num_threads;i++)
		{
			for(itr=vehicles[i].begin();itr!=vehicles[i].end();itr++) glVertex3fv((GLfloat*)&(*itr));
		}

		glEnd();

		glDisable(GL_DEPTH_TEST);
	}
}
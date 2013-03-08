//*********************************************************
//	Draw_Layer.h - Function for drawing a general layer
//*********************************************************

#pragma once
#include "Information_Panel_Implementation.h"

//---------------------------------------------------------
//	Draw_Layer
//--------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
void Information_Panel_Implementation<MasterType,ParentType,InheritanceList>::Draw_Layer(const int start_iteration, const int end_iteration, Antares_Layer_Interface* layer)
{
	const bool draw=layer->draw<bool>();

	// skip hidden layers
	if(!draw) return;

	const PrimitiveType primitive_type=layer->primitive_type<PrimitiveType>();	
	
	assert(primitive_type==_PLOT);


	PLFLT xmin, xmax, ymin, ymax;
	xmin = ymin = FLT_MAX;
	xmax = ymax = -FLT_MAX;

	const True_Color_RGBA<MasterType> head_color=layer->head_color<True_Color_RGBA<MasterType>>();
	const int head_size_value=layer->head_size_value<int>();
	
	const bool grouped=layer->grouped<bool>();
	const bool group_color=layer->group_color<bool>();

	const int primitive_stride = layer->primitive_stride<int>();

	const int vert_size = layer->vert_size<int>();
	const int vert_stride = layer->vert_stride<int>();

	const int data_stride=layer->data_stride<int>();
	
	const Dynamic_Multi_Buffer< vector<unsigned char>[_num_antares_threads] >& storage=layer->storage<Dynamic_Multi_Buffer< vector<unsigned char>[_num_antares_threads] >&>();

	//---- determine bounds ----

	int current_iteration=start_iteration;
	
	PLFLT point_value;

	while(current_iteration <= end_iteration)
	{
		const vector<unsigned char> (&geometry_by_thread)[_num_antares_threads] = storage[current_iteration];

		for(int i=0;i<_num_antares_threads;i++)
		{
			const unsigned char* geometry_itr = &geometry_by_thread[i].front();
			const unsigned char* const geometry_end = geometry_itr+geometry_by_thread[i].size();

			while(geometry_itr != geometry_end)
			{
				geometry_itr += data_stride;

				if(group_color)
				{
					glColor4ubv((GLubyte*)geometry_itr);
					geometry_itr += sizeof(True_Color_RGBA<MasterType>);
				}

				const int num_group_primitives=*((int*)geometry_itr);

				geometry_itr += sizeof(int);

				const unsigned char* const group_x_end = geometry_itr + (sizeof(PLFLT) * num_group_primitives);

				//Scan x points
				while( geometry_itr != group_x_end )
				{
					point_value = *((PLFLT*)geometry_itr);

					xmin = ( ( xmin ) < ( point_value ) ? ( xmin ) : ( point_value ) );
					xmax = ( ( xmax ) < ( point_value ) ? ( point_value ) : ( xmax ) );

					geometry_itr += sizeof(PLFLT);
				}

				const unsigned char* const group_y_end = geometry_itr + (sizeof(PLFLT) * num_group_primitives);

				//Scan y points
				while( geometry_itr != group_y_end )
				{
					point_value = *((PLFLT*)geometry_itr);

					ymin = ( ( ymin ) < ( point_value ) ? ( ymin ) : ( point_value ) );
					ymax = ( ( ymax ) < ( point_value ) ? ( point_value ) : ( ymax ) );

					geometry_itr += sizeof(PLFLT);
				}
			}
		}

		current_iteration++;
	}

	wxPLplotstream* pls = _plotwindow->GetStream();

	pls->adv( 0 );
	pls->scol0( 0, 255, 255, 255 );
	pls->scol0( 15, 0, 0, 0 );

	pls->col0( 1 );
	
	xmax = max(xmax,1.0);

	ymax += 1;
	ymax *= 1.2;

	pls->env( xmin, xmax, ymin, ymax, 0, 0 );
	pls->col0( 15 );
	pls->lab( "Iteration", "Value", layer->name<string&>().c_str() );


	pls->scol0( 3, head_color._r, head_color._g, head_color._b );
	pls->col0( 3 );
	
	pls->width( 2 );

	current_iteration=start_iteration;

	//---- draw main layer ----

	while(current_iteration <= end_iteration)
	{
		const vector<unsigned char> (&geometry_by_thread)[_num_antares_threads] = storage[current_iteration];

		for(int i=0;i<_num_antares_threads;i++)
		{
			const unsigned char* geometry_itr = &geometry_by_thread[i].front();
			const unsigned char* const geometry_end = geometry_itr+geometry_by_thread[i].size();

			while(geometry_itr != geometry_end)
			{
				geometry_itr += data_stride;

				if(group_color)
				{
					glColor4ubv((GLubyte*)geometry_itr);
					geometry_itr += sizeof(True_Color_RGBA<MasterType>);
				}

				const int num_group_primitives=*((int*)geometry_itr);

				geometry_itr += sizeof(int);

				const PLFLT* const x_points = (const PLFLT* const)geometry_itr;
				geometry_itr+=(sizeof(PLFLT) * num_group_primitives);

				const PLFLT* const y_points = (const PLFLT* const)geometry_itr;
				geometry_itr+=(sizeof(PLFLT) * num_group_primitives);

				pls->line( num_group_primitives, x_points, y_points );
			}
		}

		current_iteration++;
	}




	//int np = end_iteration - start_iteration + 1;

	//if(np <= 1) return;

	//PLFLT* x = new PLFLT[np];
	//PLFLT* y = new PLFLT[np];
	//PLFLT xmin, xmax;
	//xmin = start_iteration;
	//xmax = end_iteration;
	//PLFLT ymin = 1e30, ymax = 1e-30;

	//const bool draw=layer->draw<bool>();

	//// skip hidden layers
	//if(!draw) return;

	//const PrimitiveType primitive_type=layer->primitive_type<PrimitiveType>();	
	//assert(primitive_type==_PLOT);

	//const True_Color_RGBA<MasterType> head_color=layer->head_color<True_Color_RGBA<MasterType>>();

	//const int head_size_value=layer->head_size_value<int>();
	//
	//const Dynamic_Multi_Buffer< vector<unsigned char>[_num_antares_threads] >& storage=layer->storage<Dynamic_Multi_Buffer< vector<unsigned char>[_num_antares_threads] >&>();

	//int current_iteration=start_iteration;
	//
	//while(current_iteration <= end_iteration)
	//{
	//	const vector<unsigned char> (&geometry_by_thread)[_num_antares_threads] = storage[current_iteration];

	//	x[current_iteration - start_iteration]=current_iteration;
	//	y[current_iteration - start_iteration]=0;

	//	for(int i=0;i<_num_antares_threads;i++)
	//	{
	//		if(geometry_by_thread[i].size())
	//		{
	//			const unsigned char* geometry_itr = &geometry_by_thread[i].front();
	//			y[current_iteration - start_iteration] += *((int*)(geometry_itr));
	//		}
	//	}

	//	ymin = ( ( ymin ) < ( y[current_iteration - start_iteration] ) ? ( ymin ) : ( y[current_iteration - start_iteration] ) );
	//	ymax = ( ( ymax ) < ( y[current_iteration - start_iteration] ) ? ( y[current_iteration - start_iteration] ) : ( ymax ) );

	//	current_iteration++;
	//}

	//wxPLplotstream* pls = _plotwindow->GetStream();

	//pls->adv( 0 );
	//pls->scol0( 0, 255, 255, 255 );
	//pls->scol0( 15, 0, 0, 0 );

	//pls->col0( 1 );
	//
	//ymax+=1;
	//ymax*=1.2;

	//pls->env( xmin, xmax, ymin, ymax, 0, 0 );
	//pls->col0( 15 );
	//pls->lab( "Iteration", "Value", layer->name<string&>().c_str() );
	//
	//pls->scol0( 3, head_color._r, head_color._g, head_color._b );
	//pls->col0( 3 );
	//pls->width( 2 );

	//pls->line( np, x, y );

	//delete[] x;
	//delete[] y;
}
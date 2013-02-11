//*********************************************************
//	Draw_Layer.h - Function for drawing a general layer
//*********************************************************

#pragma once
#include "Information_Panel_Implementation.h"

//---------------------------------------------------------
//	Draw_Layer
//--------------------------------------------------------

template<typename MasterType,typename ParentType>
void Information_Panel_Implementation<MasterType,ParentType>::Draw_Layer(const int start_iteration, const int end_iteration, Antares_Layer_Interface* layer)
{
	int np = end_iteration - start_iteration + 1;

	if(np <= 1) return;

	PLFLT* x = new PLFLT[np];
	PLFLT* y = new PLFLT[np];
	PLFLT xmin, xmax;
	xmin = start_iteration;
	xmax = end_iteration;
	PLFLT ymin = 1e30, ymax = 1e-30;

	const bool draw=layer->draw<bool>();

	// skip hidden layers
	if(!draw) return;

	const PrimitiveType primitive_type=layer->primitive_type<PrimitiveType>();	
	assert(primitive_type==_PLOT);

	const True_Color_RGBA<MasterType> head_color=layer->head_color<True_Color_RGBA<MasterType>>();

	const int head_size_value=layer->head_size_value<int>();
	
	const Dynamic_Multi_Buffer< vector<unsigned char>[_num_threads] >& storage=layer->storage<Dynamic_Multi_Buffer< vector<unsigned char>[_num_threads] >&>();

	int current_iteration=start_iteration;
	
	while(current_iteration <= end_iteration)
	{
		const vector<unsigned char> (&geometry_by_thread)[_num_threads] = storage[current_iteration];

		x[current_iteration - start_iteration]=current_iteration;
		y[current_iteration - start_iteration]=0;

		for(int i=0;i<_num_threads;i++)
		{
			if(geometry_by_thread[i].size())
			{
				const unsigned char* geometry_itr = &geometry_by_thread[i].front();
				y[current_iteration - start_iteration] += *((int*)(geometry_itr));
			}
		}

		ymin = ( ( ymin ) < ( y[current_iteration - start_iteration] ) ? ( ymin ) : ( y[current_iteration - start_iteration] ) );
		ymax = ( ( ymax ) < ( y[current_iteration - start_iteration] ) ? ( y[current_iteration - start_iteration] ) : ( ymax ) );

		current_iteration++;
	}

	wxPLplotstream* pls = _plotwindow->GetStream();

	pls->adv( 0 );
	pls->scol0( 0, 255, 255, 255 );
	pls->scol0( 15, 0, 0, 0 );

	pls->col0( 1 );
	pls->env( xmin, xmax, ymin, ymax + 1, 0, 0 );
	pls->col0( 15 );
	pls->lab( "Iteration", "Value", layer->name<string&>().c_str() );
	
	pls->scol0( 3, head_color._r, head_color._g, head_color._b );
	pls->col0( 3 );
	pls->width( 2 );

	pls->line( np, x, y );

	delete[] x;
	delete[] y;
}
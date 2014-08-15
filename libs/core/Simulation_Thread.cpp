///----------------------------------------------------------------------------------------------------
/// Simulation_Thread.cpp - General Purpose Threads for the Discrete Event Engine
///----------------------------------------------------------------------------------------------------

#include "Simulation_Thread.h"

namespace polaris
{

///----------------------------------------------------------------------------------------------------
/// Simulation_Loop - Primary simulation loop for discrete event engine threads
///----------------------------------------------------------------------------------------------------

#ifdef _MSC_VER
DWORD WINAPI Simulation_Loop(LPVOID package_ptr)
#else
void* Simulation_Loop(void* package_ptr)
#endif
{
	World* world=((Thread_Information*)package_ptr)->_world_ptr;
	int id=((Thread_Information*)package_ptr)->_id;

	__thread_id=id;

	Simulation_Engine* const simulation_engine = world->simulation_engine();

	// indicate thread is ready to start the simulation
	world->Send_Signal_To_World();

	// wait until all threads are ready - make sure no threads go into simulation before all have been created
	world->Wait_For_Signal_From_World();

	Revision next_revision = __revision_omega;

	// continue to loop until world gives the stop order
	while( world->Is_Running() )
	{
	
		// take one step forward in the simulation
		simulation_engine->Step(next_revision);

		// indicate thread is finished with iteration
		world->Send_Signal_To_World();

		// wait until all threads finished and world updated
		world->Wait_For_Signal_From_World();
	}

	// let the world know that this thread is completely finished
	world->Send_Finished_Signal_To_World();
	
	return 0;
}

}
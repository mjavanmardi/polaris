#pragma once
///----------------------------------------------------------------------------------------------------
/// Simulation_Thread.h - General Purpose Threads for the Discrete Event Engine
///----------------------------------------------------------------------------------------------------

#include "World.h"

namespace polaris
{

	///----------------------------------------------------------------------------------------------------
	/// Thread_Information - Information to link thread to the global memory space
	///----------------------------------------------------------------------------------------------------

	struct Thread_Information
	{
		World* _world_ptr;
		int _id;
	};

	///----------------------------------------------------------------------------------------------------
	/// Simulation_Loop - Primary simulation loop for discrete event engine _threads
	///----------------------------------------------------------------------------------------------------

	#ifdef _MSC_VER
	extern DWORD WINAPI Simulation_Loop(LPVOID package_ptr);
	#else
	extern void* Simulation_Loop(void* package_ptr);
	#endif

}
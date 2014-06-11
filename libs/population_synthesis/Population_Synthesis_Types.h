#pragma once

using namespace std;

//---------------------------------------------------------
//	SUMMARY FILE CLASS
//---------------------------------------------------------
namespace PopSyn
{
	namespace Types
	{
		enum POPSYN_ITERATIONS
		{
			MAIN_INITIALIZE,
			MAIN_PROCESS
		};
		enum POPSYN_SUBITERATIONS
		{
			INITIALIZE=23,
			START_TIMING,
			PROCESS,
			STOP_TIMING,
			OUTPUT
		};
	} using namespace Types;
}
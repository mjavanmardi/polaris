#pragma once
#include "linker.h"
#include "File_Reader.h"
#include "File_Writer.h"


namespace PopSyn
{
	enum POPSYN_ITERATIONS
	{
		MAIN_INITIALIZE,
		MAIN_PROCESS
	};
	enum POPSYN_SUBITERATIONS
	{
		INITIALIZE,
		START_TIMING,
		PROCESS,
		STOP_TIMING,
		OUTPUT
	};
}
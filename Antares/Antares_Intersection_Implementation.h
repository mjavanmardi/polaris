//*********************************************************
//	Graphical_Intersection_Implementation.h - Graphical Intersection Variant
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Polaris_Intersection_Implementation.h"

namespace Intersection_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Antares_Intersection_Implementation:public Polaris_Intersection_Implementation<MasterType,ParentType,APPEND_CHILD(Antares_Intersection_Implementation)>
		{

		};
	}
}

using namespace Intersection_Components::Implementations;

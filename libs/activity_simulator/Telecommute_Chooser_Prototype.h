#pragma once
#include "Activity_Simulator_Includes.h"


namespace Person_Components
{
namespace Prototypes
{
	prototype struct Telecommute_Chooser ADD_DEBUG_INFO
	{
		tag_as_prototype;

		// accessor to parent class
		accessor(Parent_Planner, NONE, NONE);

		template<typename ReturnType> ReturnType Telecommute_Choice()
		{
			static_assert("Error: no handler defined for general ReturnType in Telecommute_Choice function");
		}
		template<> Person_Components::Types::TELECOMMUTE_FREQUENCY Telecommute_Choice()
		{
			return this_component()->Telecommute_Choice();
		}
	};
}
}

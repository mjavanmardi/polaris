#pragma once
#include "Operation_Prototype.h"

namespace Operation_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Operation_Implementation:public Polaris_Component<MasterType,INHERIT(Operation_Implementation),Data_Object>
		{
			m_prototype(Null_Prototype,typename MasterType::network_type, network_reference, NONE, NONE);
		};
	}

}

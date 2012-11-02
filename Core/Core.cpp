#include "Core.h"



prototype struct Agent_Proto
{
	tag_as_prototype;

	feature_accessor(mead,none,none);

	feature_prototype void drink()
	{
		mead<int>();
	}
};

implementation struct Agent_Implementation
{
	member_data(int,mead,none,none);
};


struct MasterType
{
	typedef Polaris_Component<Agent_Implementation,MasterType> Agent;
};



void main()
{
	MasterType::Agent yo;

	((Agent_Proto<MasterType::Agent,NULLTYPE>&)yo).drink<NULLTYPE>();
}
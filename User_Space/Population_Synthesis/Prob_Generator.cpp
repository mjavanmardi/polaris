#include "Prob_Generator.h"

namespace MISC
{
	MT_Prob_Generator::MT_Prob_Generator(int threadnum)
	{
		for (int i=0; i<threadnum; i++) this->_generators.push_back(Prob_Generator());
	}
	
	MT_Prob_Generator::MT_Prob_Generator(vector<unsigned int>& seeds)
	{
		for (int i=0; i<seeds.size(); i++) this->_generators.push_back(Prob_Generator(seeds[i]));
	}
	void MT_Prob_Generator::Set_Seeds(vector<unsigned int>& seeds)
	{
		for (int i=0; i<_generators.size(); i++)
		{
			if (i>= seeds.size()) return;
			_generators[i].Set_Seed(seeds[i]);
		}
	}
	double MT_Prob_Generator::Get_Probability(int threadnum)
	{
		if (threadnum >= _generators.size())
		{
			const char* mess = "ERROR: 'threadnum' specified is greater than number of declared threads for MT_Prob_Generator;";
			THROW_EXCEPTION(mess);
		}

	}
	
}
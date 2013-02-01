#include "Model_Selection.h"

#ifdef ANTARES_EXAMPLE

#include "Application_Includes.h"

struct MasterType
{
	typedef Graphical_Network_Implementation<MasterType> graphical_network_type;
};

void main(int argc,char** argv)
{
	START_UI(argc,argv,MasterType,nullptr);

	for(int i=0;i<10000;i++)
	{
		cout << i << endl;
		Sleep(1000);
	}
}

#endif
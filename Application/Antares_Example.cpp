#include "Model_Selection.h"

#ifdef ANTARES_EXAMPLE

#include "Application_Includes.h"

void main(int argc,char** argv)
{
	START_UI(argc,argv,nullptr);

	for(int i=0;i<1000;i++)
	{
		cout << i << endl;
		Sleep(1000);
	}
}

#endif
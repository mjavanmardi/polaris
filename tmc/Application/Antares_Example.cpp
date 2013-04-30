#ifdef ANTARES_EXAMPLE
#include "Model_Selection.h"
#include "Application_Includes.h"

struct MasterType
{
	typedef Conductor_Implementation<MasterType> conductor_type;
	typedef Control_Panel_Implementation<MasterType> control_panel_type;
	typedef Time_Panel_Implementation<MasterType> time_panel_type;
	typedef Information_Panel_Implementation<MasterType> information_panel_type;
	typedef Canvas_Implementation<MasterType> canvas_type;

	typedef Graphical_Network_Implementation<MasterType> graphical_network_type;
	typedef Graphical_Link_Implementation<MasterType> graphical_link_type;
	typedef Graphical_Intersection_Implementation<MasterType> graphical_intersection_type;
};

void main(int argc,char** argv)
{
	START_UI(argc,argv,MasterType,nullptr);

	Sleep(100);

	//((Antares_App<MasterType>*)antares)->main->_conductor->Initialize<NULLTYPE>();

	//for(int i=0;i<10000;i++)
	//{
	//	cout << i << endl;
	//	Sleep(1000);
	//}

	START();
}

#endif
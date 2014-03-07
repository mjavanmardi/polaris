//*********************************************************
//	Antares_App.h - Antares Application Initialization
//*********************************************************

#pragma once
#include "Antares_Frame_Implementation.h"

//---------------------------------------------------------
//	antares - build global antares singleton
//---------------------------------------------------------

extern void* antares;
extern void* canvas;
extern void* information_panel;


//---------------------------------------------------------
//	START_UI - macro to start the UI
//---------------------------------------------------------

#define START_UI(MASTER_TYPE,XMIN,YMIN,XMAX,YMAX) \
	antares=(void*)new Antares_App<MASTER_TYPE>();\
	canvas=nullptr;\
	information_panel=nullptr;\
	((Antares_App<MASTER_TYPE>*)antares)->Start_UI(XMIN,YMIN,XMAX,YMAX);\

//---------------------------------------------------------
//	Wx_Loop - declare wx loop function
//---------------------------------------------------------

DWORD WINAPI Wx_Loop(LPVOID _app);

//---------------------------------------------------------
//	Antares_App - antares application class
//---------------------------------------------------------

implementation class Antares_App: public wxApp
{
public:
	//---------------------------------------------------------
	//	OnInit - build Antares frame, called after Start_UI
	//---------------------------------------------------------
	
	virtual bool OnInit()
	{
		//if(argc!=2)
		//{
		//	wxMessageBox("Database File Not Provided!");
		//	exit(0);
		//}
		//else
		//{
		//	db_name=argv[1];

		//	ifstream test;
		//	test.open(db_name);

		//	if(!test.is_open())
		//	{
		//		wxMessageBox("Invalid Database File Provided!");
		//		exit(0);
		//	}

		//	test.close();
		//}

		__thread_id = num_sim_threads();

		main=new Antares_Implementation<MasterType>(nullptr);

		canvas = main->_canvas;
		information_panel = main->_information_panel;

		main->Maximize();
		main->Show();

		main->_canvas->Initialize(xmin,ymin,xmax,ymax);

		main->_information_panel->Render<NULLTYPE>();

		initialized=true;

		return true;
	}
	
	//---------------------------------------------------------
	//	Start_UI - Initialize wx, spin thread to handle events
	//---------------------------------------------------------
	
	void Start_UI(float _xmin,float _ymin,float _xmax,float _ymax)
	{
		initialized=false;
		int _argc=0;
		char** _argv=nullptr;

		xmin=_xmin;
		ymin=_ymin;
		xmax=_xmax;
		ymax=_ymax;

		wxDISABLE_DEBUG_SUPPORT();

		wxEntryStart(_argc,_argv);

		CreateThread(NULL,0,Wx_Loop<MasterType>,this,0,NULL);

		while(!initialized) Sleep(100);
	}

	Antares_Implementation<MasterType>* main;
	float xmin;
	float ymin;
	float xmax;
	float ymax;

	volatile bool initialized;
};

//---------------------------------------------------------
//	Wx_Loop - define wx loop function
//---------------------------------------------------------

template<typename MasterType>
DWORD WINAPI Wx_Loop(LPVOID _app)
{
	Antares_App<MasterType>* app=(Antares_App<MasterType>*)_app;

	app->CallOnInit();
	app->MainLoop();
	return 1;
}

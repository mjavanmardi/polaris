//*********************************************************
//	Antares_App.h - Antares Application Initialization
//*********************************************************

#pragma once
#include "Antares_Frame.h"

//---------------------------------------------------------
//	START_UI - macro to start the UI
//---------------------------------------------------------

#define START_UI(ARGC,ARGV,SIMULATION_PTR) antares->Start_UI(ARGC,ARGV,SIMULATION_PTR)

//---------------------------------------------------------
//	Wx_Loop - declare wx loop function
//---------------------------------------------------------

DWORD WINAPI Wx_Loop(LPVOID _app);

//---------------------------------------------------------
//	Antares_App - antares application class
//---------------------------------------------------------

class Antares_App: public wxApp
{
public:
	//---------------------------------------------------------
	//	OnInit - build Antares frame, called after Start_UI
	//---------------------------------------------------------
	
	virtual bool OnInit()
	{
		main=new Antares(nullptr);

		main->Maximize();

		main->Show();

		main->canvas->Initialize_GLCanvas();

		return true;
	}
	
	//---------------------------------------------------------
	//	Start_UI - Initialize wx, spin thread to handle events
	//---------------------------------------------------------

	void Start_UI(int argc, char** argv, void* ptr)
	{
		simulation_pointer=ptr;

		wxEntryStart(argc,argv);

		CreateThread(NULL,0,Wx_Loop,this,0,NULL);
	}

	Antares* main;
	void* simulation_pointer;
};

//---------------------------------------------------------
//	Wx_Loop - define wx loop function
//---------------------------------------------------------

DWORD WINAPI Wx_Loop(LPVOID _app)
{
	Antares_App* app=(Antares_App*) _app;

	app->CallOnInit();
	app->MainLoop();
	return 1;
}

//---------------------------------------------------------
//	antares - build global antares singleton
//---------------------------------------------------------

static Antares_App* antares=new Antares_App();
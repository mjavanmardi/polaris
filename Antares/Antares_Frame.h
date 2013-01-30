#pragma once

#include "Antares_Includes.h"

#define START_UI(ARGC,ARGV,SIMULATION_PTR) antares->Start_UI(ARGC,ARGV,SIMULATION_PTR)

class Antares : public wxFrame
{
public:
	wxMenu* file_menu;
	wxMenuBar* menu_bar;

	Antares(wxFrame* parent) : wxFrame(parent,-1,"Antares")
	{
		file_menu=new wxMenu();
		file_menu->Append(wxID_OPEN,"Open");
		file_menu->Append(wxID_SAVE,"Save");
		file_menu->AppendSeparator();
		file_menu->Append(wxID_EXIT,"Exit");

		menu_bar=new wxMenuBar();

		menu_bar->Append(file_menu,"File");
		
		SetMenuBar(menu_bar);
	}

};

class Antares_App: public wxApp
{
public:
	virtual bool OnInit();
	wxFrame* main;
	void* simulation_pointer;

	void Start_UI(int argc, char** argv, void* ptr);
};

bool Antares_App::OnInit()
{
	main=new Antares((wxFrame *)NULL);

	main->Maximize();
	main->Show();

	return true;
}

DWORD WINAPI Wx_Loop(LPVOID _app)
{
	Antares_App* app=(Antares_App*) _app;

	app->CallOnInit();
	app->MainLoop();
	return 1;
}

void Antares_App::Start_UI(int argc, char** argv, void* ptr)
{
	simulation_pointer=ptr;

	wxEntryStart(argc,argv);

	CreateThread(NULL,0,Wx_Loop,this,0,NULL);
}

static Antares_App* antares=new Antares_App();

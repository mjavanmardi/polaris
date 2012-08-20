#pragma once
#include "Polaris_Component.h"

DWORD WINAPI Thread_Loop(LPVOID package_ptr);

class World;

struct Thread_Package
{
	World* world_ptr;
	int id;
};

class World
{
public:
	World()
	{
		memory_root_ptr=new Memory_Root();
		execution_root_ptr=new Execution_Root();
		data_root_ptr=new Data_Root();
		world_ptr=this;
		run=false;

		resume_event=CreateEvent(NULL,FALSE,FALSE,NULL);

		for(int i=0;i<num_threads;i++)
		{
			packages[i].id=i;
			packages[i].world_ptr=this;

			finished_event[i]=CreateEvent(NULL,TRUE,FALSE,NULL);

			threads[i]=CreateThread(NULL,0,Thread_Loop,&packages[i],CREATE_SUSPENDED,NULL);
		}
	}
	
	void Start_Turning()
	{
		run=true;

		for(int i=0;i<num_threads;i++){ResumeThread(threads[i]);}

		while(true)
		{
			WaitForMultipleObjects(num_threads,finished_event,TRUE,INFINITE);
			

			tick_tock=!tick_tock;

			iteration++;

			sub_iteration=0;

			if(iteration<num_iterations)
			{
				for(int i=0;i<num_threads;i++) ResetEvent(finished_event[i]);
			}
			else
			{
				_InterlockedDecrement(&run);
				break;
			}

			for(int i=0;i<num_threads;i++)
			{
				SetEvent(resume_event);
			}
		}
	}
	
	LONG volatile run;

	LARGE_INTEGER frequency;
	LARGE_INTEGER start;
	LARGE_INTEGER end;

	HANDLE resume_event;
	HANDLE finished_event[num_threads];
	Thread_Package packages[num_threads];
	HANDLE threads[num_threads];
};

static World* world=new World();

DWORD WINAPI Thread_Loop(LPVOID package_ptr)
{
	World* world=((Thread_Package*)package_ptr)->world_ptr;
	int id=((Thread_Package*)package_ptr)->id;

	Execution_Root* const exe=execution_root_ptr;

	while(world->run)
	{
		exe->Process();
		SignalObjectAndWait(world->finished_event[id],world->resume_event,INFINITE,TRUE);
	}

	return 0;
}
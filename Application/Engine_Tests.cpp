
//#define ENGINE_TESTS

#ifdef ENGINE_TESTS
#include "Application_Includes.h"

static volatile long long visited=0;

implementation struct Test:public Polaris_Component_Class<Test,MasterType,Execution_Object>
{
public:
	void Initialize()
	{
		Load_Register<Test>(&When_Do_Stuff<NULLTYPE>,&Do_Stuff<NULLTYPE>,0);
		x=0;
	}

	declare_feature_conditional(When_Do_Stuff)
	{
		response.result = (_iteration%2==0);
		response.next = _iteration + 1;
	}

	declare_feature_event(Do_Stuff)
	{
		visited=visited+2+((Test*)_this)->x;
	}

	int x;
	int y;
	int z;
};

struct MasterType{};

void main()
{
	LARGE_INTEGER start_timer, stop_timer, frequency;

	double allocation_time,run_time;

	const unsigned int num_agents=1000000;

	const int pages=(num_agents*sizeof(Test<MasterType*>))/_Page_Size;

	QueryPerformanceFrequency(&frequency);

	QueryPerformanceCounter(&start_timer);

	for(int i=0;i<num_agents;i++)
	{
		Test<MasterType>* test=Allocate<Test<MasterType>>();
		test->Initialize();
	}
	
	QueryPerformanceCounter(&stop_timer);

	allocation_time=((double)stop_timer.QuadPart-(double)start_timer.QuadPart)/((double)frequency.QuadPart);

	QueryPerformanceCounter(&start_timer);
	
	START();

	QueryPerformanceCounter(&stop_timer);

	run_time=((double)stop_timer.QuadPart-(double)start_timer.QuadPart)/((double)frequency.QuadPart);

	cout << "accuracy results: " << endl;
	cout << "\t" << visited << ", " << num_agents*_num_iterations << ": " << (bool)(visited==(num_agents*_num_iterations)) << endl;

	cout << "performance results: " << endl;
	cout << "\tallocation time: " << allocation_time << endl;
	cout << "\trun time: " << run_time << endl;

	bool pause=true;
}

#endif
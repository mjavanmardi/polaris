#include "Model_Selection.h"

#ifdef ENGINE_TESTS
#include "Application_Includes.h"
#define TEST_1

#ifdef TEST_1
static volatile unsigned long visited=0;

implementation struct Test:public Polaris_Component<APPEND_CHILD(Test),MasterType,Execution_Object>
{
public:
	void Initialize()
	{
		Load_Register<Test>(&When_Do_Stuff<NULLTYPE>,&Do_Stuff<NULLTYPE>,0,0);
		x=0;
	}

	declare_feature_conditional(When_Do_Stuff)
	{
		response.result = (_iteration%2==0);
		response.next._iteration = _iteration+1;
		response.next._sub_iteration = 0;
	}

	declare_feature_event(Do_Stuff)
	{
		if(!((Test*)_this)->x)
		{
			AtomicIncrement(&visited);
			AtomicIncrement(&visited);
		}
	}

	int x;
	int y;
	int z;
};

struct MasterType{};



void main()
{
	_num_iterations=1000;

	LARGE_INTEGER start_timer, stop_timer, frequency;

	double allocation_time,run_time;

	const unsigned int num_agents=30000;
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

#ifdef TEST_2

static volatile unsigned long visited=0;

implementation struct Test:public Polaris_Component_Class<Test,MasterType,Execution_Object>
{
public:
	void Initialize()
	{
		Load_Register<Test>(&When_Do_Stuff<NULLTYPE>,&Do_Stuff<NULLTYPE>,0,0);
	}

	declare_feature_conditional(When_Do_Stuff)
	{
		response.result = true;
		response.next._iteration = _iteration+1;
		response.next._sub_iteration = 0;
	}

	declare_feature_event(Do_Stuff)
	{
		Test* pthis=(Test*)_this;

		if(pthis->id==_iteration)
		{
			Free<Test>(pthis);
		}
		else
		{
			AtomicIncrement(&visited);
		}
	}

	int id;
};

struct MasterType{};

void main()
{
	const unsigned int num_agents=1000;
	const int pages=(num_agents*sizeof(Test<MasterType*>))/_Page_Size;

	for(int i=0;i<num_agents;i++)
	{
		Test<MasterType>* test=Allocate<Test<MasterType>>();
		test->Initialize();
		test->id=i;
	}
	
	START();

	int agents=num_agents;
	int ideal_visited=0;

	for(int i=0;i<_num_iterations;i++)
	{
		--agents;
		ideal_visited+=agents;
	}

	cout << "accuracy results: " << endl;
	cout << "\t" << visited << ", " << ideal_visited << ": " << (bool)(visited==ideal_visited) << endl;

	bool pause=true;
}
#endif

#ifdef TEST_3

struct MasterType{};

implementation class Agent:public Polaris_Component_Class<Agent,MasterType>
{
	feature_implementation void Test_1(typename TargetType::ParamType x, typename TargetType::Param2Type y, requires(check(typename TargetType::ReturnType, Basic_Units::Concepts::Is_Time_Value)))
	{
	}
};

void main()
{
	Agent<MasterType> yo;

	yo.Test_1<Agent<MasterType>,NULLTYPE,Target_Type<_Time<MasterType>,int,int>>(3,4);
}

#endif

#endif
#include "Polaris_PCH.h"

prototype struct My_Prototype
{
	feature_prototype void Call()
	{
		this_component()->Call<ComponentType,CallerType,TargetType>();
	}
	feature_prototype void Call_Only_In_Base()
	{
		this_component()->Call_Only_In_Base<ComponentType,CallerType,TargetType>();
	}
	feature_prototype void Tester()
	{
		this_component()->Tester<ComponentType,CallerType,TargetType>();
	}
};

implementation struct Base : public Polaris_Component<APPEND_CHILD(Base),MasterType,Data_Object>
{
	feature_implementation void Call()
	{
		cout << endl << "Called Base Version";
	}
	feature_implementation void Call_Only_In_Base()
	{
		cout << endl << "Called Version only in Base";
	}
};


implementation struct Inherited : public Base<MasterType,ParentType,APPEND_CHILD(Inherited)>
{
	typedef Base<MasterType,ParentType,APPEND_CHILD(Inherited)> BaseType;
	typedef typename BaseType::Component_Type ComponentType;

	typedef My_Prototype<BaseType> base_itf;
	typedef My_Prototype<ComponentType> this_itf;

	feature_implementation void Call()
	{
		cout << endl << "Called Inherited Version";
	}
	feature_implementation void Tester()
	{
		base_itf* bthis = (base_itf*)this;
		bthis->Call<NT>();
		bthis->Call_Only_In_Base<NT>();
		this_itf* pthis = (this_itf*)this;
		pthis->Call<NT>();
		pthis->Call_Only_In_Base<NT>();
	}
};

int main()
{
	typedef Inherited<NT> test_type;
	typedef My_Prototype<test_type> test_itf;

	test_itf* tester = (test_itf*)Allocate<test_type>();
	tester->Tester<NT>();

	int test;
	cin >> test;
}
#include "Model_Selection.h"

#ifdef EXAMPLE

#include "Application_Includes.h"


//implementation struct Network
//{
//	feature_implementation void Do_Stuff(int a, requires(check(TargetType, is_arithmetic)));
//	//feature_implementation void Do_Stuff(int a);
//};
//
//
//template<typename MasterType,typename ParentType>
//template<typename ComponentType, typename CallerType, typename TargetType>
//feature_implementation_definition
//void Network<MasterType,ParentType>::Do_Stuff(int a, requires(check(TargetType, is_arithmetic)))
//{
//	cout << "hooray" << endl;
//}



template<typename MyType>
struct Network
{
	template<typename MyOtherType>
	void Do_Stuff(char param[MyOtherType::size]);
};

template<typename MyType>
template<typename MyOtherType>
void Network<MyType>::Do_Stuff(char param[MyOtherType::size])
{
};

struct Array_Size
{
	static const int size;
};

const int Array_Size::size=3;

int main()
{
}
#endif

#ifdef EXAMPLE

#include "Polaris_PCH.h"

#include "Model_Selection.h"
//
//#include "../Repository/Repository.h"
using namespace Basic_Units::Implementations;



template <class TList> struct DISPATCH_ALIAS;
template <>
struct DISPATCH_ALIAS<NULLTYPE>
{
	template<class HeadComponentType, typename TargetType>
	static inline typename TargetType::ReturnType Start_Dispatch(void* obj)
	{
		THROW_EXCEPTION("ERROR: type id: " << ((generic_implementation<NT>*)obj)->Identify() << " not found in typelist");
	}
};
template <class Head, class Tail>
struct DISPATCH_ALIAS<TypeList<Head, Tail> >
{
	define_feature_exists_check(Print_Type_ID, Print_Type_ID_exists)
	template<class HeadType_As_Given, typename TargetType>
	static inline typename TargetType::ReturnType Start_Dispatch(void* obj, requires(check(HeadType_As_Given, Is_Polaris_Prototype) || check(HeadType_As_Given, Is_Polaris_Component)))
	{
		return DISPATCH_ALIAS<TypeList<HeadType_As_Given, Tail> >::Dispatch<HeadType_As_Given::Component_Type,HeadType_As_Given, TargetType>(obj);
	}
	template<class HeadType_As_Given, typename TargetType>
	static inline typename TargetType::ReturnType Start_Dispatch(void* obj, requires(!check(HeadType_As_Given, Is_Polaris_Prototype) && !check(HeadType_As_Given, Is_Polaris_Component)))
	{
		assert_check(HeadType_As_Given, Is_Polaris_Prototype, "Type is not a valid polaris prototype or component." );
	}
	template<class HeadComponentType, class HeadType_As_Given, typename TargetType>
	static inline typename TargetType::ReturnType Dispatch(void* obj, requires(check(HeadType_As_Given, Is_Polaris_Prototype) && check(HeadComponentType,Print_Type_ID_exists)))
	{
		if(((generic_implementation<NT>*)obj)->Identify() == Head::Component_Type::component_index)
		{
			return (TargetType::ReturnType)((Head*)obj)->Print_Type_ID<TargetType::ControlType>();
		}
		else
		{
			return DISPATCH_ALIAS<Tail>::Start_Dispatch<TypeAt<Tail,0>::Result, TargetType>(obj);
		}
	}
	template<class HeadComponentType, class HeadType_As_Given, class TargetType>
	static inline typename TargetType::ReturnType Dispatch(void* obj, requires(check(HeadType_As_Given, Is_Polaris_Component) && check(HeadComponentType,Print_Type_ID_exists)))
	{
		if(((generic_implementation<NT>*)obj)->Identify() == Head::Component_Type::component_index)
		{
			return (TargetType::ReturnType)((Head*)obj)->Print_Type_ID<HeadType_As_Given, HeadType_As_Given, TargetType::ControlType>();
		}
		else
		{
			return DISPATCH_ALIAS<Tail>::Start_Dispatch<TypeAt<Tail,0>::Result, TargetType>(obj);
		}
	}
	template<class HeadComponentType, class HeadType_As_Given, class TargetType>
	static inline typename TargetType::ReturnType  Dispatch(void* obj, requires( (!check(HeadType_As_Given, Is_Polaris_Component) && !check(HeadType_As_Given, Is_Polaris_Prototype))))
	{
		assert_check(HeadType_As_Given, Is_Polaris_Prototype, "Type is not a valid polaris prototype or component." );
	}
	template<class HeadComponentType, class HeadType_As_Given, class TargetType>
	static inline typename TargetType::ReturnType  Dispatch(void* obj, requires( (check(HeadType_As_Given, Is_Polaris_Component) || check(HeadType_As_Given, Is_Polaris_Prototype)) && !check(HeadComponentType,Print_Type_ID_exists)))
	{
		assert_check(HeadComponentType, Print_Type_ID_exists, "Print_Type_ID does not exist.  Make sure to 'tag_feature_as_available' if the feature does exist in the component." );
	}
};






struct tester0
{
	void Print_Type_ID(){cout<<"typeid";}
};
//-----------------------------------------------------------------
// The test classes which are combined into a anonymous vector
prototype struct tester1
{
	tag_as_prototype;
	feature_accessor(my_id,none,none);
	feature_prototype int Print_Type_ID(){cout<<"Prototype 1 called: ";return this_component()->Print_Type_ID<ComponentType,CallerType,TargetType>();}
	feature_prototype float Calculate_Utility(){return this_component()->Calculate_Utility<ComponentType,CallerType,TargetType>();}
};
implementation struct tester1_imp : public Polaris_Component<APPEND_CHILD(tester1_imp),NT,Data_Object,NT,true>
{
	member_data(int, my_id,none,none);
	feature_implementation int Print_Type_ID(){ cout << "tester1: component_index="<< this->component_index<< ", id="<<_my_id << endl ; return _my_id;} tag_feature_as_available(Print_Type_ID);
	feature_implementation float Calculate_Utility(){return _my_id;} tag_feature_as_available(Calculate_Utility);
};

prototype struct tester2
{
	tag_as_prototype;
	feature_accessor(my_id,none,none);
	feature_prototype int Print_Type_ID(){cout<<"Prototype 2 called: ";return this_component()->Print_Type_ID<ComponentType,CallerType,TargetType>();}
	feature_prototype float Calculate_Utility(){return this_component()->Calculate_Utility<ComponentType,CallerType,TargetType>();}
};
implementation struct tester2_imp : public Polaris_Component<APPEND_CHILD(tester2_imp),NT,Data_Object,NT,true>
{
	member_data(int, my_id,none,none);
	feature_implementation int Print_Type_ID(){ cout << "tester2: component_index="<< this->component_index << ", id="<<_my_id <<endl; return _my_id;}tag_feature_as_available(Print_Type_ID);
	feature_implementation float Calculate_Utility(){return _my_id;} tag_feature_as_available(Calculate_Utility);
};

prototype struct tester3
{
	tag_as_prototype;
	feature_accessor(my_id,none,none);
	feature_prototype int Print_Type_ID(){cout<<"Prototype 3 called: "; return this_component()->Print_Type_ID<ComponentType,CallerType,TargetType>();}
	feature_prototype float Calculate_Utility(){return this_component()->Calculate_Utility<ComponentType,CallerType,TargetType>();} 
};
implementation struct tester3_imp : public Polaris_Component<APPEND_CHILD(tester3_imp),NT,Data_Object,NT,true>
{
	member_data(int, my_id,none,none);
	feature_implementation int Print_Type_ID(){ cout << "tester3: component_index="<< this->component_index << ", id="<<_my_id <<endl; return _my_id;}tag_feature_as_available(Print_Type_ID);
	feature_implementation float Calculate_Utility(){return _my_id;} tag_feature_as_available(Calculate_Utility);
};


//-----------------------------------------------------------------
// Class which holds the anonymous vector and does conditional printing
prototype struct holder
{
	tag_as_prototype;

	define_feature_dispatcher(Print_Type_ID,Print_ID_Dispatcher);

	feature_accessor(test_vector, none, none);
	feature_prototype_generic void Add(TargetType item, requires(check(TargetType, Is_Polaris_Prototype)))
	{
		if (IndexOf<TList,strip_modifiers(TargetType)>::value < 0) THROW_EXCEPTION("Invalid TargetType, not found in TList.");

		// Push item into vector as anonymous
		define_simple_container_interface(test_vector_itf,typename get_type_of(test_vector),Containers::Random_Access_Sequence_Prototype,void*,ComponentType);
		test_vector_itf* test_vec = this->test_vector<test_vector_itf*>();
		test_vec->push_back((void*)item);
	}
	feature_prototype_generic typename TargetType print(requires(check(TargetType,is_arithmetic)))
	{
		define_simple_container_interface(test_vector_itf,typename get_type_of(test_vector),Containers::Random_Access_Sequence_Prototype,void*,ComponentType);
		test_vector_itf* test_vec = this->test_vector<test_vector_itf*>();
		for (test_vector_itf::iterator itr = test_vec->begin(); itr!=test_vec->end(); ++itr)
		{
			int test;
			// Generic interface to do type testing
			test = dispatch_to_feature(Print_ID_Dispatcher,TList,*itr,Target_Type<int,TargetType>);
			//DISPATCH_ALIAS<AvailableTypes>::Start_Dispatch<TypeAt<AvailableTypes,0>::Result,Target_Type<TargetType,void>>(*itr);
			cout << endl << endl << "MY int test= " << test<< endl << endl;
		}
		return (TargetType)1.0;
	}
	feature_prototype_generic typename TargetType print(requires(!check(TargetType,is_arithmetic)))
	{
		define_simple_container_interface(test_vector_itf,typename get_type_of(test_vector),Containers::Random_Access_Sequence_Prototype,void*,ComponentType);
		test_vector_itf* test_vec = this->test_vector<test_vector_itf*>();
		for (test_vector_itf::iterator itr = test_vec->begin(); itr!=test_vec->end(); ++itr)
		{
			dispatch_to_feature(Print_ID_Dispatcher,TList,*itr,Target_Type<int,TargetType>);
		}
	}
};

implementation struct holder_imp : public Polaris_Component<APPEND_CHILD(holder_imp),NT,Data_Object,NT,true>
{
	member_container(vector<void*>,test_vector, none, none);
};

struct MasterType
{
	typedef generic_implementation<MasterType> generic_component_type;
	typedef tester1_imp<MasterType> tester1_type;
	typedef tester2_imp<MasterType> tester2_type;
	typedef tester3_imp<MasterType> tester3_type;
	typedef holder_imp<MasterType> holder_type;
	typedef Choice_Model_Components::Implementations::MNL_Model_Implementation<MasterType> choice_model_type;

};

int main()
{
	// Type_List type thing to use in anonymous class
	typedef tester1<MasterType::tester1_type> test1_type;
	typedef tester2<MasterType::tester2_type> test2_type;
	typedef tester3<MasterType::tester3_type> test3_type;
	typedef Choice_Model_Components::Prototypes::Choice_Model<MasterType::choice_model_type> choice_mode_itf;

	typedef TypeList<test1_type, TypeList<test2_type> > Holder_1_List;
	typedef TypeList<test1_type> Holder_2_List;
	typedef TypeList<test1_type,TypeList<test2_type, TypeList<test3_type> > > Holder_3_List;
	

	cout << "Do Stuff " << endl;

	Time_Prototype<Time_Implementation<NT>>* time = (Time_Prototype<Time_Implementation<NT>>*)(new Time_Implementation<NT>());
	time->Value<Time_Seconds>(2);

	cout << endl << time->Value<Time_Minutes>();


	char pause;	cin >> pause;
}
#endif
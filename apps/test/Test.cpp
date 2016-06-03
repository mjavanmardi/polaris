#ifdef ANTARES
#include "Antares/Antares.h"
#endif



#define CONCEPTS

#ifdef CONCEPTS
#include "core.h"
//#include "Network_Prototype.h"
#include <type_traits>
using namespace std;

// ((NT (_V::*)())&_V::NAME<NT>)

//concept struct Is_Basic_Link_Prototype
//{
//	check_accessor_name(has_upstream_intersection, Component_Type::template upstream_intersection);
//	check_accessor_name(has_downstream_intersection, Component_Type::template downstream_intersection);
//	check_accessor_name(has_network_reference, Component_Type::template network_reference);
//	check_accessor_name(has_link_type, Component_Type::template link_type);
//	check_accessor_name(has_uuid, Component_Type::template uuid);
//	check_accessor_name(has_internal_id, Component_Type::template internal_id);
//	define_default_check(has_upstream_intersection && has_downstream_intersection && has_network_reference && has_link_type  && has_uuid && has_internal_id);
//};

//concept struct Is_Basic_Link
//{
//	check_accessor_name(has_upstream_intersection, template upstream_intersection);
////	check_accessor_name(has_downstream_intersection, template downstream_intersection);
////	check_accessor_name(has_network_reference, template network_reference);
////	check_accessor_name(has_link_type, template link_type);
////	check_accessor_name(has_uuid, template uuid);
////	check_accessor_name(has_internal_id, template internal_id);
////
////	check_concept(is_basic_link_prototype, Is_Basic_Link_Prototype, T, V);
////	define_sub_check(is_basic_link, has_upstream_intersection && has_downstream_intersection && has_network_reference && has_link_type  && has_uuid && has_internal_id);
////	define_default_check(is_basic_link || is_basic_link_prototype);
//	define_default_check(has_upstream_intersection);
//};

prototype struct Link
{
	tag_as_prototype;

	//==================================================================================================================
	/// basic link
	//------------------------------------------------------------------------------------------------------------------
//	accessor(uuid, NONE, NONE);
//	accessor(internal_id, NONE, NONE);
//	accessor(network_reference, NONE, NONE);
//	accessor(link_type, NONE, NONE);
//	accessor(turn_movements_container, NONE, NONE);
//	accessor(upstream_intersection, NONE, NONE);
//	accessor(downstream_intersection, NONE, NONE);
};

struct MasterType
{
	typedef float turn_movement_type;
	typedef int intersection_type;
	typedef int link_type;
};

implementation struct Link_Implementation:public Polaris_Component<MasterType,INHERIT(Link_Implementation),Execution_Object>
{
	typedef typename Polaris_Component<MasterType, INHERIT(Link_Implementation), Execution_Object>::Component_Type ComponentType;

//	m_data(int, uuid, NONE, NONE);
//	m_data(int, internal_id, NONE, NONE);
//	m_data(int, network_reference, NONE, NONE);
//	m_data(int, link_type, NONE, NONE);
//	m_container(boost::container::vector<typename MasterType::turn_movement_type*>, turn_movements_container, NONE, NONE);

	m_container(boost::container::vector<typename MasterType::intersection_type*>, intersections_container, NONE, NONE);
	m_container(boost::container::vector<typename MasterType::link_type*>, links_container, NONE, NONE);

	m_data(int, upstream_intersection, NONE, NONE);
//	m_data(int, downstream_intersection, NONE, NONE);

	//void read_data(Network_Components::Types::Network_IO_Maps& net_io_maps) {};
	void read_data() {};
	int read_data(const string& in) {};

	///*template<typename TargetType>*/ void read_network_data(Network_Components::Types::Network_IO_Maps& net_io_maps)
	/*template<typename TargetType>*/ void read_network_data()
	{
		//_db_reader = Allocate<typename remove_pointer<type_of(db_reader)>::type>();
		//typedef Prototypes::Network_DB_Reader<typename remove_pointer<type_of(db_reader)>::type> _DB_Interface;
		//_DB_Interface* db = (_DB_Interface*)_db_reader;
		//db->template network_reference<ComponentType*>((ComponentType*)this);

		//db->template read_network_data<Network_Components::Types::Network_IO_Maps&>(net_io_maps);
	}
};

template<typename, typename T>
struct has_read_data {
	static_assert(
		std::integral_constant<T, false>::value,
		"Second template parameter needs to be of function type.");
};

// specialization that does the checking

template<typename C, typename Ret, typename... Args>
struct has_read_data<C, Ret(Args...)> {
private:
	template<typename T>
	static constexpr auto check_it(T*)
		-> typename
		std::is_same<
		decltype(std::declval<T>().read_data(std::declval<Args>()...)),
		Ret    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		>::type;  // attempt to call it and see if the return type is correct

	template<typename>
	static constexpr std::false_type check_it(...);

	typedef decltype(check_it<C>(0)) type;

public:
	static constexpr bool value = type::value;
};

#define check_accessor_name_new(CHECK_ALIAS,NAME)\
		template<typename TypeChecked>\
		struct CHECK_ALIAS##_procedure\
		{\
			template<typename _V>\
			constexpr static bool has_matching_named_member(typename enable_if<is_member_function_pointer<decltype((NT* (_V::Component_Type::*)(void*))&_V::Component_Type::template NAME<NT*>)>::value>::type* = nullptr)\
			{return 1;}\
			\
			template<typename _V>\
			constexpr static bool has_matching_named_member(...)\
			{\
				return 0;\
			}\
			\
			static const bool value = has_matching_named_member<TypeChecked>(0);\
		};\
		static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure<T>::value;

#define check_method_name_new(CHECK_ALIAS,NAME)\
	template<typename TypeChecked>\
	struct CHECK_ALIAS##_procedure\
	{\
		template<typename _U>\
		struct function_check\
		{\
			template<typename _V> static small_type has_matching_named_member(typename is_member_function_pointer<decltype(&_V::NAME)>::type);\
			template<typename _V> static large_type has_matching_named_member(...);\
			\
			template<typename _V,bool _P>\
			struct form_check{\
				__pragma( message("check_method_name_new can't find " #CHECK_ALIAS "::" #NAME ))\
				static const bool value = false;\
			};\
			\
			template<typename _V>\
			struct form_check<_V,true>{ static const bool value = true; };\
			\
			static const bool performcheck = (sizeof(has_matching_named_member<_U>(true_val))==success);\
			static const bool value = form_check<_U,performcheck>::value;\
		};\
		\
		static const bool value = function_check<TypeChecked>::value;\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure<T>::value;

#define check_method_decl(CHECK_ALIAS,NAME, TEMPL_TYPE, Ret, Args)\
	template<typename, typename TypeChecked>\
	struct CHECK_ALIAS##_procedure {\
		static_assert(\
			std::integral_constant<TypeChecked, false>::value,\
				"Second template parameter needs to be of function type.");\
	};\
	template<typename TypeChecked, typename Ret, typename... Args>\
	struct CHECK_ALIAS##_procedure<TypeChecked, Ret(Args...)> {\
	private:\
		template<typename T>\
		static constexpr auto checkit(T*)\
			-> typename\
			std::is_same<\
			decltype(std::declval<T>().NAME<TEMPL_TYPE>(std::declval<Args>()...)),\
			Ret\
			>::type;\
		template<typename>\
		static constexpr std::false_type checkit(...);\
		typedef decltype(checkit<TypeChecked>(0)) type;\
	public:\
		static constexpr bool value = type::value;\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure<T>::value;

//template<typename TypeChecked>\
//	struct CHECK_ALIAS##_procedure\
//	{\
//		template<typename _U>\
//		struct function_check\
//		{\
//			template<typename _V> static small_type has_matching_named_member(typename is_member_function_pointer<decltype(&_V::NAME)>::type);\
//			template<typename _V> static large_type has_matching_named_member(...);\
//			\
//			template<typename _V,bool _P>\
//			struct form_check{\
//				__pragma( message("check_method_name_new can't find " #CHECK_ALIAS "::" #NAME ))\
//				static const bool value = false;\
//			};\
//			\
//			template<typename _V>\
//			struct form_check<_V,true>{ static const bool value = true; };\
//			\
//			static const bool performcheck = (sizeof(has_matching_named_member<_U>(true_val))==success);\
//			static const bool value = form_check<_U,performcheck>::value;\
//		};\
//		\
//		static const bool value = function_check<TypeChecked>::value;\
//	};\
//	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure<T>::value;


concept struct Is_Network_Compliant
{
	check_component_accessor_name(has_intersections, intersections_container);
	check_component_accessor_name(has_links, links_container);
	check_component_accessor_name(has_read_function, read_network_data);
	check_component_accessor_name(has_read_data, read_data);
	check_component_accessor_name(has_upstream_intersection, upstream_intersection);
	//check_method_decl(has_read_member, read_data, int, void, const std::string&);
	define_sub_check(is_basic_network, has_intersections && has_links /*&& has_read_member && has_read_function*/);
	define_default_check(is_basic_network);
//	define_default_check(has_upstream_intersection);
};

//	template<typename TypeChecked>
//	struct has_upstream_intersection_procedure
//	{
//		//	template<typename _V>
//		//	constexpr static bool has_matching_named_member(typename enable_if<is_member_function_pointer<decltype((NT* (_V::*)())&_V::template upstream_intersection<NT*>)>::value>::type* = nullptr)
//		//	{return 1;}
//		//
//		//	template<typename _V>
//		//	constexpr static bool has_matching_named_member(typename enable_if<is_member_function_pointer<decltype((NT* (_V::Component_Type::*)(void*))&_V::Component_Type::template upstream_intersection<NT*>)>::value>::type* = nullptr)
//		//	{return 1;}
//
//		//	template<typename _V>
//		//	constexpr static bool has_matching_named_member(typename enable_if<is_same<decltype(((_V*)nullptr)->template upstream_intersection<NT*>()),NT*>::value>::type* = nullptr)
//		//	{return 1;}
//
//		template<typename _V>
//		constexpr static bool has_matching_named_member(typename enable_if<is_same<decltype(((typename _V::Component_Type*)nullptr)->template upstream_intersection<NT*>()), NT*>::value>::type* = nullptr)
//		{
//			return 1;
//		}
//
//		//	template<typename _V>
//		//	constexpr static bool has_matching_named_member(typename enable_if<is_member_function_pointer<decltype((NT* (_V::Component_Type::*)(void*))&_V::Component_Type::template upstream_intersection<NT*>)>::value>::type* = nullptr)
//		//	{return 1;}
//
//		template<typename _V>
//		constexpr static bool has_matching_named_member(...)
//		{
//#pragma message("What? Something is missing...")
//			return 0;
//		}
//
//		static const bool value = has_matching_named_member<TypeChecked>(0);
//
//	};
//
//	//define_default_check(has_upstream_intersection_procedure);
//
//};

//decltype((NT* (_V::Component_Type::*)())&_V::Component_Type::NAME<NT*>)

//#define check_accessor_name(CHECK_ALIAS,NESTED_TYPE,NAME)\
//		template<typename TypeChecked>\
//		struct CHECK_ALIAS##_procedure\
//		{\
//			template<typename _V>\
//			constexpr static bool has_matching_named_member(typename enable_if<is_same<((_V*)nullptr)->template NAME<NT*>(),NT*>::value>::type* = nullptr)\
//			{return 1;}\
//			\
//			template<typename _V>\
//			constexpr static bool has_matching_named_member(...)\
//			{return 0;}\
//			\
//			static const bool value = has_matching_named_member<TypeChecked>(0);\
//		};\
//		static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure<T>::value;
//
//#define check_component_accessor_name(CHECK_ALIAS,NESTED_TYPE,NAME)\
//		template<typename TypeChecked>\
//		struct CHECK_ALIAS##_procedure\
//		{\
//			template<typename _V>\
//			constexpr static bool has_matching_named_member(typename enable_if<is_same<((typename _V::Component_Type*)nullptr)->template NAME<NT*>(),NT*>::value>::type* = nullptr)\
//			{return 1;}\
//			\
//			template<typename _V>\
//			constexpr static bool has_matching_named_member(...)\
//			{return 0;}\
//			\
//			static const bool value = has_matching_named_member<TypeChecked>(0);\
//		};\
//		static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure<T>::value;

template<typename, typename T>
struct has_serialize {
	static_assert(
		std::integral_constant<T, false>::value,
		"Second template parameter needs to be of function type.");
};

// specialization that does the checking

template<typename C, typename Ret, typename... Args>
struct has_serialize<C, Ret(Args...)> {
private:
	template<typename T>
	static constexpr auto checkit(T*)
		-> typename
		std::is_same<
		decltype(std::declval<T>().serialize(std::declval<Args>()...)),
		Ret    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		>::type;  // attempt to call it and see if the return type is correct

	template<typename>
	static constexpr std::false_type checkit(...);

	typedef decltype(checkit<C>(0)) type;

public:
	static constexpr bool value = type::value;
};

template <typename T>
struct has_serialize_method
{
	struct dummy { /* something */ };

	template <typename C, typename P>
	static auto test(P * p) -> decltype(std::declval<C>().serialize(*p), std::true_type());

	template <typename, typename>
	static std::false_type test(...);

	typedef decltype(test<T, dummy>(nullptr)) type;
	static const bool value = std::is_same<std::true_type, decltype(test<T, dummy>(nullptr))>::value;
};

template<typename T>
struct X {
	int serialize(T t) { return 42; }
};

template<typename T>
struct Y : X<T> {};

template<typename Whatever>
struct boom
{
	template <typename T>
	void bam(const T& t)
	{
		/* something */
	}
};

template <typename T>
struct has_bam_method
{
	struct dummy { /* something */ };

	template <typename C, typename P>
	static auto test(P * p) -> decltype(std::declval<C>().bam(*p), std::true_type());

	template <typename, typename>
	static std::false_type test(...);

	typedef decltype(test<T, dummy>(nullptr)) type;
	static const bool value = std::is_same<std::true_type, decltype(test<T, dummy>(nullptr))>::value;
};


int main(int argc, char *argv[])
{
	static_assert(has_bam_method<boom<int>>::value, "has_bam_method not found!");
	assert_check(boom<int>, has_bam_method, "has_bam_method not found again!");
 	static_assert(has_serialize<Y<string>, int(const std::string&)>::value, "has_serialize not found!");
	//static_assert(has_serialize_method<Y<string>>::value, "has_serialize_method not found!");

	typedef Link<Link_Implementation<MasterType>>* link_itf_ptr;
	typedef Link<Link_Implementation<MasterType>> link_itf;
	//using link_itf = Link<Link_Implementation<MasterType>>;
	//	typedef typename link_itf::Component_Type CType;
//	typedef NT* (CType::* bob_t)(void*);

	//static_assert(has_read_data<Link_Implementation<MasterType>, void(Network_Components::Types::Network_IO_Maps&)>::value, "has_read_data not found!");
	//static_assert(has_read_data<link_itf, void(Network_Components::Types::Network_IO_Maps& net_io_maps)>::value, "has_read_data not found using typedef!");
	//static_assert(check_method_decl(has_read_data, read_data, int, (const std::string&)));

	//NT (CType::* bob)();
	//bob_t bob = &CType::template turn_movements_container<NT*>;

	//decltype((NT (link_itf::*)())&strip_modifiers(link_itf_ptr)::Component_Type::template turn_movements_container<NT>) bob;

	//static_assert(is_same<decltype(judge->template upstream_intersection<NT>()),NT>::value,"Fail!");
	assert_check(link_itf, Is_Network_Compliant<link_itf>::has_upstream_intersection_procedure, "Fail! - 'has_upstream_intersection_procedure' not found");
	//assert_check(link_itf, Is_Network_Compliant, "Wait! What?");
	static_assert(check_stripped_type(link_itf, Is_Network_Compliant), "Wait! What?");
	//static_assert(check_stripped_type(link_itf, Is_Network_Compliant), "Fail! - 'has_upstream_intersection_procedure' not found");
	//static_assert(check(strip_modifiers(link_itf_ptr),has_upstream_intersection_procedure),"Fail! - 'has_upstream_intersection_procedure' not found");

	//bob_t bob = &link_itf::Component_Type::template upstream_intersection<NT*>;
	//decltype((NT* (link_itf::Component_Type*)(void*))&link_itf::Component_Type::template upstream_intersection<NT*>) bob;
	//static_assert(check(strip_modifiers(link_itf_ptr),Is_Basic_Link),"Fail!");
}
#endif

#ifdef ROUTER
#include <iostream>
#include <chrono>
#include <vector>
// can we get away with only storing 1 function pointer for an entire graph?
// what if we store the pointer type with the pointer in the priority queue, instead of in the graph?
// if we store like an index into to a function pointer table, it is at least cheaper

// "real edge" passed into the function pointer resolution

//template<typename This_Edge,typename Adjacent,typename Attributes>
//struct ConnectionGroup
//{
//
//};
//
//template<typename Edge_t,typename ConnectionGroup1,typename ConnectionGroup2>
//struct Graph
//{
//
//};
//
//struct Edge_A;
//
//struct Edge_B;
//
//template<typename From,typename To>
//struct Connection
//{
//
//};
//
//
//template<int x>
//struct Executor
//{
//	static void DoStuff(int pmap)
//	{
//		if(pmap & x)
//		{
//			std::cout << "Doing Stuff" << std::endl;
//
//
//			if(pmap == 0) return;
//		}
//
//		next::DoStuff(pmap-2);
//	}
//
//	typedef Executor<x-2> next;
//};
//
//
//template<typename This_Edge,typename ConnectionGroup1,typename ConnectionGroup2>
//struct ConnectionTypes
//{
//	void Visit_Neighbors(void* edge)
//	{
//		void* pthis = this;
//
//		// switch to combination of neighbors present
//		switch(_pmap)
//		{
//			case 0x01:
//				pthis = ((ConnectionGroup1*)pthis)->Visit_Neighbors(edge);
//				break;
//			case 0x10:
//				pthis = ((ConnectionGroup2*)pthis)->Visit_Neighbors(edge);
//				break;
//			case 0x11:
//			{
//				pthis = ((ConnectionGroup1*)pthis)->Visit_Neighbors(edge);
//				pthis = ((ConnectionGroup2*)pthis)->Visit_Neighbors(edge);
//				break;
//			}
//		};
//	}
//
//	static void Visit_Connections(void* pthis,void* edge)
//	{
//		((ConnectionTypes*)pthis)->Visit_Neighbors(edge);
//	}
//
//	int _pmap;
//};
//
//template<typename Adjacent_1,typename Adjacent_2>
//struct EdgeConn : public Connection<EdgeConn,Adjacent_1,Adjacent_2>
//{
//
//};


struct ConnectionC
{
	static void DoStuff(uint64_t& sum)
	{
		sum += 3;
	}

	static void DoStuff(int pmap,uint64_t& sum)
	{
		if(pmap & 0b100) sum += 3;
		//if(pmap <= 0b100) return;
	}


	template<int pmap>
	inline static void DoStuff(uint64_t& sum)
	{
		if (pmap & 0b100) sum += 3;
	}
};

struct ConnectionB
{
	static void DoStuff(uint64_t& sum)
	{
		sum += 2;
	}

	static void DoStuff(int pmap,uint64_t& sum)
	{
		if (pmap & 0b010)
		{
			sum += 2;

			if(pmap <= 0b011) return;
		}

		ConnectionC::DoStuff(pmap, sum);
	}

	template<int pmap>
	inline static void DoStuff(uint64_t& sum)
	{
		if (pmap & 0b010) sum += 2;

		ConnectionC::template DoStuff<pmap>(sum);
	}
};



struct ConnectionA
{
	static void DoStuff(uint64_t& sum)
	{
		sum += 1;
	}

	static void DoStuff(int pmap,uint64_t& sum)
	{
		if(pmap & 0b001)
		{
			sum += 1;

			if(pmap <= 0b001) return;
		}

		 ConnectionB::DoStuff(pmap, sum);
	}

	template<int pmap>
	inline static void DoStuff(uint64_t& sum)
	{
		if(pmap & 0b001) sum += 1;

		ConnectionB::template DoStuff<pmap>(sum);
	}
};

// Note, things get a lot cheaper if you can minimize parameter passing

int pmap;
int sum = 0;

template<int x = 0>
struct Executor
{
	inline static void DoStuff(uint64_t& sum)
	{
		if(pmap==x) ConnectionA::template DoStuff<x>(sum);
		else Executor<x-1>::DoStuff(sum);
	}
};

template<>
struct Executor<0>
{
	inline static void DoStuff(uint64_t&){}

//	{
//		ConnectionA::template DoStuff<1>(sum);
//	}
};

//template<>
//struct Executor<0>
//{
//
//}
int main(int argc, char *argv[])
{
	using namespace std::chrono;

	std::vector<int> edges;

	for(int i=0;i<100000000;i++)
	{
		edges.push_back((rand()%7+1));
	}

	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	uint64_t sum = 0;
	uint64_t sum2 = 0;

	for(auto& edge : edges)
	{
//		switch(edge)
//		{
//			case 0b001:
//				ConnectionA::DoStuff(sum);
//				break;
//			case 0b010:
//				ConnectionB::DoStuff(sum);
//				break;
//			case 0b011:
//				ConnectionA::DoStuff(sum);
//				ConnectionB::DoStuff(sum);
//				break;
//			case 0b100:
//				ConnectionC::DoStuff(sum);
//				break;
//			case 0b101:
//				ConnectionA::DoStuff(sum);
//				ConnectionC::DoStuff(sum);
//				break;
//			case 0b110:
//				ConnectionB::DoStuff(sum);
//				ConnectionC::DoStuff(sum);
//				break;
//			case 0b111:
//				ConnectionA::DoStuff(sum);
//				ConnectionB::DoStuff(sum);
//				ConnectionC::DoStuff(sum);
//				break;
//		}

//		if(edge == 0b001)
//		{
//			ConnectionA::DoStuff(sum);
//		}
//		else if(edge == 0b010)
//		{
//			ConnectionB::DoStuff(sum);
//		}
//		else if(edge == 0b011)
//		{
//			ConnectionA::DoStuff(sum);
//			ConnectionB::DoStuff(sum);
//		}
//		else if(edge == 0b100)
//		{
//			ConnectionC::DoStuff(sum);
//		}
//		else if(edge == 0b101)
//		{
//			ConnectionA::DoStuff(sum);
//			ConnectionC::DoStuff(sum);
//		}
//		else if(edge == 0b110)
//		{
//			ConnectionB::DoStuff(sum);
//			ConnectionC::DoStuff(sum);
//		}
//		else// if(edge == 0b111)
//		{
//			ConnectionA::DoStuff(sum);
//			ConnectionB::DoStuff(sum);
//			ConnectionC::DoStuff(sum);
//		}

//		ConnectionA::DoStuff(edge,sum);

		pmap = edge;
		Executor<7>::DoStuff(sum);
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	auto time_span = t2 - t1;

	std::cout << "It took me " << time_span.count() << " nanos." << std::endl;

	std::cout << sum << std::endl;
}


#endif

#ifdef STUFF
#include "core/Core.h"
using namespace polaris;


//template<typename T>
//struct AgentPrototype
//{
//	void DoStuff()
//	{
//		((T*)this)->DoStuff();
//	}
//
//	void DoOtherStuff()
//	{
//		std::cout << "yay";
//	}
//};
//
//struct Bob{};
//
//int main(int argc, char *argv[])
//{
//	AgentPrototype<Bob> hellion;
//	hellion.DoOtherStuff();
//	//hellion.DoStuff<int>();
//}

// Test agent
prototype struct Agent ADD_DEBUG_INFO
{
	tag_as_prototype;

	accessor(data,check(strip_modifiers(TargetType),is_integral),check(strip_modifiers(TargetType),is_integral));
	accessor(x,NONE,NONE);
	accessor(y,NONE,NONE);
	accessor(something_else,NONE,NONE);
	accessor(extra_data,NONE,NONE);
	accessor(thing_i_care_about,NONE,NONE);
};

prototype struct Simple_Agent ADD_DEBUG_INFO
{
	tag_as_prototype;
	accessor(extra_data,NONE,NONE);
	accessor(thing_i_care_about,NONE,NONE);
	accessor(other_stuff,NONE,NONE);
};

prototype struct My_Code ADD_DEBUG_INFO
{
	tag_as_prototype;

	accessor(my_agent,NONE,NONE);

	template<typename TargetType> void Do_stuff_with_agent()
	{
		//typedef Agent<MasterType::agent_type> agent_itf;
		//typedef Agent<MasterType::other_agent_type> other_agent_itf;
		this->my_agent<>();
	}
};
implementation struct My_Code_Implementation : public Polaris_Component<MasterType,INHERIT(My_Code_Implementation)>
{
	// Member data
	m_prototype(Agent, typename MasterType::other_agent_type, my_agent, NONE,NONE);
};

implementation struct Base_Agent_Implementation : public Polaris_Component<MasterType,INHERIT(Base_Agent_Implementation)>
{
	// Member data
	m_data(int,data,NONE,NONE);
	m_data(float,x,NONE,NONE);
	m_data(float,y,NONE,NONE);
	m_data(int, thing_i_care_about,NONE,NONE);
};

implementation struct Other_Agent_Implementation : public Polaris_Component<MasterType,INHERIT(Base_Agent_Implementation)>
{
	// Member data
	m_data(int,data,NONE,NONE);
	m_data(int,extra_data,NONE,NONE);
	m_data(int, thing_i_care_about,NONE,NONE);
	m_data(int,other_stuff,NONE,NONE);
};


struct MasterType
{
	//=================================================================================
	typedef MasterType M;
	// Add all of the types used in your code here
	typedef Base_Agent_Implementation<M> agent_type;
	typedef Other_Agent_Implementation<M> other_agent_type;
};


int main(int argc, char *argv[])
{
	//----------------------------------------------------------
	// Initialize basic simulation
	Simulation_Configuration cfg;
	cfg.Multi_Threaded_Setup(10000, 1);
	INITIALIZE_SIMULATION(cfg);


	// Standard initialization of random number generator
//	GLOBALS::Uniform_RNG.Initialize();
//	GLOBALS::Uniform_RNG.Set_Seed<int>();



	//----------------------------------------------------------
	// Your code here

	// define an interface to use
	typedef Agent<MasterType::agent_type> agent_itf;
	typedef Agent<MasterType::other_agent_type> other_agent_itf;

	// Create agents and initialize them
	agent_itf* base_agent = (agent_itf*)Allocate<MasterType::agent_type>();
	base_agent->thing_i_care_about(1);
	base_agent->x(5.0);


	other_agent_itf* other_agent = (other_agent_itf*)Allocate<MasterType::other_agent_type>();
	other_agent->data(6);
	//other_agent->other_stuff(7);

	other_agent->thing_i_care_about(2);

	agent_itf* new_agent = (agent_itf*)other_agent;
	cout << new_agent->template thing_i_care_about<int>();
	cout << new_agent->template x<float>();

	START();
	char test;
	cin >> test;
}
#endif


//// Test agent
//prototype struct Agent ADD_DEBUG_INFO
//{
//	tag_as_prototype;
//
//
//	template<typename T> void Initialize(T i, requires(T,check(strip_modifiers(T),is_integral)))
//	{
//		this_component()->Initialize(i);
//		this_component()->Load_Event<ComponentType>(&Agent_Event,1,0);
//	}
//	template<typename T> void Initialize(T i, requires(T,!check(strip_modifiers(T),is_integral)))
//	{
//		cout <<"OOPS, NOT AN INTEGRAL DATA TYPE.";
//	}
//
//	accessor(data,check(strip_modifiers(TargetType),is_integral),check(strip_modifiers(TargetType),is_integral));
//	accessor(x,NONE,NONE);
//	accessor(y,NONE,NONE);
//	accessor(my_agent,NONE,NONE);
//	accessor(something_else,NONE,NONE);
//
//	static void Agent_Event(ComponentType* _this,Event_Response& response)
//	{
//
//		response.next._iteration = iteration() + 1;
//		response.next._sub_iteration = 0;
//		_this->Do_Event();
//	}
//};
//#ifdef ANTARES
//implementation struct Antares_Agent_Implementation : public Polaris_Component<MasterType,INHERIT(Antares_Agent_Implementation),Execution_Object>
//{
//	// Member data
//	m_data(int,data,NONE,NONE);
//	m_data(float,x,NONE,NONE);
//	m_data(float,y,NONE,NONE);
//	m_data(True_Color_RGB<NT>,color,NONE,NONE);
//	static Antares_Layer<typename MasterType::antares_layer_type>* _base_agent_layer;
//
//	// static initializer for the type - call once at program start to initialize static variables
//	static void Initialize_Type()
//	{
//		_base_agent_layer = Allocate_New_Layer<MT>(string("Base Agent"));
//		Antares_Layer_Configuration cfg;
//		cfg.Configure_Dynamic_Points();
//		cfg.Configure_Dynamic_Points();
//		cfg.target_sub_iteration = 1;
//		cfg.head_size_value = 20;
//		cfg.primitive_color = true;
//		cfg.draw=true;
//		_base_agent_layer->Initialize<NT>(cfg);
//	}
//
//	// Member functions
//	void Initialize(int i)
//	{
//		// set initial values for member variables
//		_data = 1;
//		_x = GLOBALS::Uniform_RNG.Next_Rand<float>()*1000;
//		_y = GLOBALS::Uniform_RNG.Next_Rand<float>()*1000;
//
//		int r = i % 255;
//		_color.r(255-r); _color.g(0); _color.b(r);
//	}
//	void Do_Event(Event_Response& response)
//	{
//		// Code to execute when the event fires
//		_x += 150.0 * (GLOBALS::Uniform_RNG.Next_Rand<float>() - 0.5);
//		_y += 150.0 * (GLOBALS::Uniform_RNG.Next_Rand<float>()  - 0.5);
//		Draw();
//	}
//	void Draw()
//	{
//		Point_Element_Colored p;
//		p._point.x(_x);
//		p._point.y(_y);
//		p._color._r=_color._r;
//		p._color._g=_color._g;
//		p._color._b=_color._b;
//		Scale_Coordinates<MT>(p._point);
//		_base_agent_layer->Push_Element<Regular_Element>(&p);
//	}
//};
//#endif
//implementation struct Base_Agent_Implementation : public Polaris_Component<MasterType,INHERIT(Base_Agent_Implementation),Execution_Object>
//{
//	// Member data
//	m_data(int,data,NONE,NONE);
//	m_data(float,x,NONE,NONE);
//	m_data(float,y,NONE,NONE);
//	m_prototype(Agent, typename MasterType::other_agent_type, my_agent, NONE,NONE);
//	m_data(True_Color_RGB<NT>,color,NONE,NONE);
//	static Antares_Layer<typename MasterType::antares_layer_type>* _base_agent_layer;
//
//	// static initializer for the type - call once at program start to initialize static variables
//	static void Initialize_Type()
//	{
//		_base_agent_layer = Allocate_New_Layer<MT>(string("Base Agent"));
//		Antares_Layer_Configuration cfg;
//		cfg.Configure_Dynamic_Points();
//		cfg.Configure_Dynamic_Points();
//		cfg.target_sub_iteration = 1;
//		cfg.head_size_value = 20;
//		cfg.primitive_color = true;
//		cfg.draw=true;
//		_base_agent_layer->Initialize<NT>(cfg);
//	}
//
//	void Initialize(int i)
//	{
//		// set initial values for member variables
//		_data = 1;
//		_x = GLOBALS::Uniform_RNG.Next_Rand<float>()*1000;
//		_y = GLOBALS::Uniform_RNG.Next_Rand<float>()*1000;
//		_color.r(255); _color.g(0); _color.b(0);
//	}
//	void Do_Event()
//	{
//		// Code to execute when the event fires
//		_x += 150.0 * (GLOBALS::Uniform_RNG.Next_Rand<float>() - 0.5);
//		_y += 150.0 * (GLOBALS::Uniform_RNG.Next_Rand<float>()  - 0.5);
//		if (_x >= 1000) _x = 1000;
//		if (_x <= -1000) _x = -1000;
//		if (_y >= 500) _y = 500;
//		if (_y <= -500) _y = -500;
//		
//		Draw();
//	}
//	void Draw()
//	{
//		Point_Element_Colored p;
//		p._point.x(_x);
//		p._point.y(_y);
//		p._color._r=_color._r;
//		p._color._g=_color._g;
//		p._color._b=_color._b;
//		Scale_Coordinates<MT>(p._point);
//		_base_agent_layer->Push_Element<Regular_Element>(&p);
//	}
//};
//template<typename MasterType,typename InheritanceList> Antares_Layer<typename MasterType::antares_layer_type>* Base_Agent_Implementation<MasterType,InheritanceList>::_base_agent_layer;
//implementation struct Other_Agent_Implementation : public Base_Agent_Implementation<MasterType,INHERIT(Other_Agent_Implementation)>
//{
//	typedef Base_Agent_Implementation<MasterType,INHERIT(Other_Agent_Implementation)> base_type;
//	// Member data
//
//	m_data(float,z,NONE,NONE);
//	m_prototype(Agent, typename MasterType::agent_type, my_agent, NONE,NONE);
//			
//	member_component_and_feature_accessor(length, Value, Basic_Units::Prototypes::Length,Basic_Units::Implementations::Length_Implementation<NT>);
//
//	m_data(True_Color_RGB<NT>,color,NONE,NONE);
//	static Antares_Layer<typename MasterType::antares_layer_type>* _other_agent_layer;
//
//	// static initializer for the type - call once at program start to initialize static variables
//	static void Initialize_Type()
//	{
//		_base_agent_layer = Allocate_New_Layer<MT>(string("Other Agent"));
//		Antares_Layer_Configuration cfg;
//		cfg.Configure_Dynamic_Points();
//		cfg.Configure_Dynamic_Points();
//		cfg.target_sub_iteration = 1;
//		cfg.head_size_value = 20;
//		cfg.primitive_color = true;
//		cfg.draw=true;
//		_base_agent_layer->Initialize<NT>(cfg);
//	}
//
//	void Initialize(int i)
//	{
//		this->length<Basic_Units::Length_Variables::Feet>(10.0);
//
//		base_type::Initialize(i);
//
//		// set initial values for member variables
//		_z = GLOBALS::Uniform_RNG.Next_Rand<float>()*10;
//
//		_color.r(0); _color.g(0); _color.b(255);
//	}
//	void Do_Event()
//	{
//		// Code to execute when the event fires
//		float d_x = base_type::_x - this->_my_agent->x<float>();
//		float d_y = base_type::_y - this->_my_agent->y<float>();
//		float len = sqrt(pow(d_x,2) + pow(d_y,2));
//
//		float dd_x = -d_x/len*10;
//		float dd_y = -d_y/len*10;
//		base_type::_x += dd_x;
//		base_type::_y += dd_y;
//
//		cout << "Base Agent is at {"<<this->_my_agent->x<float>() << ", " <<this->_my_agent->y<float>() << "} ; Agent is at " <<base_type::_x << ", " <<base_type::_y<<endl;
//
//		Draw();
//	}
//	void Draw()
//	{
//		Point_Element_Colored p;
//		p._point.x(_x);
//		p._point.y(_y);
//		p._color._r=_color._r;
//		p._color._g=_color._g;
//		p._color._b=_color._b;
//		Scale_Coordinates<MT>(p._point);
//		_base_agent_layer->Push_Element<Regular_Element>(&p);
//	}
//};
//template<typename MasterType,typename InheritanceList> Antares_Layer<typename MasterType::antares_layer_type>* Other_Agent_Implementation<MasterType,InheritanceList>::_other_agent_layer;
//
//implementation struct Some_Other_Thing : public Polaris_Component<MasterType,INHERIT(Some_Other_Thing),Execution_Object>
//{
//	m_data(int,stuff,NONE,NONE);
//	m_data(int,x,NONE,NONE);
//	void Do_Event()
//	{
//		// Code to execute when the event fires
//	}
//};
//
//struct MasterType
//{
//	//=================================================================================
//	// REQUIRED ANTARES TYPES
//	#ifdef ANTARES
//	typedef Conductor_Implementation<MasterType> conductor_type;
//	typedef Control_Panel_Implementation<MasterType> control_panel_type;
//	typedef Time_Panel_Implementation<MasterType> time_panel_type;
//	typedef Information_Panel_Implementation<MasterType> information_panel_type;
//	typedef Canvas_Implementation<MasterType> canvas_type;
//	typedef Antares_Layer_Implementation<MasterType> antares_layer_type;
//	typedef Layer_Options_Implementation<MasterType> layer_options_type;
//	typedef Attributes_Panel_Implementation<MasterType> attributes_panel_type;
//	typedef Control_Dialog_Implementation<MasterType> control_dialog_type;
//	typedef Information_Page_Implementation<MasterType> information_page_type;
//	typedef Splash_Panel_Implementation<MasterType> splash_panel_type;
//	//=================================================================================
//	#endif
//	typedef MasterType M;
//	// Add all of the types used in your code here
//	typedef Base_Agent_Implementation<M> agent_type;
//	typedef Other_Agent_Implementation<M> other_agent_type;
//	typedef Some_Other_Thing<M> some_other_type;
//	
//};
//
//
//int main(int argc, char *argv[])
//{
//	//----------------------------------------------------------
//	// Initialize basic simulation 
//	Simulation_Configuration cfg;
//	cfg.Multi_Threaded_Setup(10000, 1);
//	INITIALIZE_SIMULATION(cfg);
//
//
//	// initialize the visualizer environment
//	#ifdef ANTARES
//	START_UI(MasterType,0,0,1000,1000);
//	#endif
//
//	// Standard initialization of random number generator
//	GLOBALS::Uniform_RNG.Initialize();
//	GLOBALS::Uniform_RNG.Set_Seed<int>();
//
//
//
//	//----------------------------------------------------------
//	// Your code here
//
//	// Initialize drawing layers - always need to call static initializer functions this way
//	MasterType::agent_type::Initialize_Type();
//	MasterType::other_agent_type::Initialize_Type();
//
//	// define an interface to use
//	typedef Agent<MasterType::agent_type> agent_itf;
//	typedef Agent<MasterType::other_agent_type> other_agent_itf;
//	
//	// Create agents and initialize them
//	for (int i = 0; i < 1; ++i)
//	{
//		agent_itf* base_agent = (agent_itf*)Allocate<MasterType::agent_type>();
//		base_agent->Initialize<int>(i);
//		
//
//		other_agent_itf* other_agent = (other_agent_itf*)Allocate<MasterType::other_agent_type>();
//		other_agent->Initialize<int>(i);
//		other_agent->my_agent(base_agent);
//		base_agent->my_agent(other_agent);
//
//		Agent<MasterType::some_other_type>* other_thing = (Agent<MasterType::some_other_type>*)Allocate<MasterType::some_other_type>();
//		other_thing->x<int>(1);
//	}
//
//
//	START();
//	char test;
//	cin >> test;
//}

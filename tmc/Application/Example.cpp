
#include "Application_Includes.h"
//struct Link_Implementation_For_GIS
//{
//	feature_implementation Get_Anode(){};
//};
//struct Link_Implementation_For_Simulation{};
//struct Node_Index{};
//
//template<typename ComponentType, typename CallerType>
//struct Link_Prototype
//{
//	template<typename TargetType>
//	void Do_Stuff(requires(check_2(ComponentType,Link_Implementation_For_GIS,is_same)))
//	{
//		// ok!
//		int anode_id=Get_Anode<Node_Index>();
//		// ok!
//		Node* anode=Get_Anode<Node*>();
//	}
//
//	template<typename TargetType>
//	void Do_Stuff(requires(check_2(ComponentType,Link_Implementation_For_Simulation,is_same)))
//	{
//		// compilation assertion:
//		int anode_id=Get_Anode<Node_Index>();
//		// ok!
//		Node* anode=Get_Anode<Node*>();
//	}
//
//	template<typename TargetType>
//	TargetType Get_Anode()
//	{
//		return ((ComponentType*)this)->Get_Anode<TargetType>();
//	}
//};



concept struct Is_In_Units
{
	// checks are now laid out like this
	check_typename_state(in_meters,In_Meters,true_type);
	check_typename_state(in_feet,In_Feet,true_type);
	
	// obligation for concept writer is to define one default check and any number of sub-checks
	define_sub_check(in_some_other_units,!(in_meters || in_feet));

	define_default_check(in_meters || in_feet);
};


prototype struct Node_Prototype
{	tag_as_prototype; // tags are required in all prototype defintions

	feature_accessor(id,check(ReturnValueType,is_arithmetic),check_2(ComponentType,CallerType,Is_Same_Entity)); // available slots for adding getter/setter checks, use none if not checking anything
};

implementation struct Node_Implementation
{
	member_data(int,id,none,none); // available slots for adding getter/setter checks, use none if not checking anything
};

prototype struct Link_Prototype
{	tag_as_prototype;


	feature_accessor(id,none,check_2(ComponentType,CallerType,Is_Same_Entity));
	feature_accessor(a_node,none,none);
	feature_accessor(b_node,none,none);
	feature_accessor(length,none,none);
};

implementation struct Link_Implementation
{
	polaris_variable(Length_In_Feet,float,In_Feet);

	member_data(int,id,none,none);
	member_data(Length_In_Feet,length,none,none); // for member data
	member_component(typename MasterType::Node,a_node,none,none); // for member components
	member_component(typename MasterType::Node,b_node,none,none);
};

prototype struct Network_Prototype
{	tag_as_prototype;
	
	
	feature_accessor(links,none,none);
};

implementation struct Network_Implementation
{
	member_container(vector<typename MasterType::Link*>,links,none,none); // for member containers
};


class YoYo
{
};

prototype struct Agent_Prototype
{	tag_as_prototype;

	feature_accessor(id,none,none);

	feature_accessor(network,none,none);

	feature_accessor(my_int_matrix,none,none);

	declare_feature_conditional(Do_Stuff_Condition)
	{
		// approve every other iteration
		response.next=_iteration+2;
		response.result=true;
	}

	declare_feature_event(Do_Stuff)
	{
		cout << "current iteration is: " << _iteration << endl;
	}

	feature_prototype void Initialize_Agent(requires(!check(TargetType,is_arithmetic)))
	{
		assert_default_check(TargetType,is_arithmetic,"TargetType is not an arithmetic type!");
	}

	feature_prototype void Initialize_Agent(requires(check(TargetType,is_arithmetic)))
	{
		id<int>();		

		// load event function
		load_event(ComponentType,Do_Stuff_Condition,Do_Stuff,0,NULLTYPE);
		
		// define your interface to network
		define_component_interface(Network,get_type_of(network),::Network_Prototype,ComponentType);

		Network& net=network<Network&>();

		// define your interface to the links container and to the links value inside
		define_container_and_value_interface(Links,Link,Network::get_type_of(links),Random_Access_Sequence_Prototype,::Link_Prototype,ComponentType);

		Links& links=net.links<Links&>();
	

		// access with IDE support
		size_t siz=access(Links,net.links).size();

		if(siz>0)
		{
			Link* link=links[0];
			
			// link monitor variable for debugging
			Link::Component_Type* link_monitor=(Link::Component_Type*)link;

			// Notice how polaris variables have type

			// Default check
			bool default_check=check(typename Link::get_type_of(length),Is_In_Units);
			
			// Sub check
			bool in_some_other_units=sub_check(typename Link::get_type_of(length),Is_In_Units,in_some_other_units);

			// Individual checks
			bool feet=sub_check(typename Link::get_type_of(length),Is_In_Units,in_feet);
			bool meters_check=sub_check(typename Link::get_type_of(length),Is_In_Units,in_meters);
			


			float len=link->length<float>();

			// nested access here, notice how the type is now extracted through the Link prototype
			define_component_interface(Node,Link::get_type_of(a_node),Node_Prototype,ComponentType);

			Node& a_node=link->a_node<Node&>();

			cout << "a_node id: " << a_node.id<int>() << endl;;
		}
	}
};

implementation struct Agent_Implementation
{
	member_component(typename MasterType::Network,network,none,none);
	member_data(int,id,check(ReturnValueType,is_arithmetic),none);
	member_container(m_array<int>,my_int_matrix,none,none);
};


// notice how components are no longer doubly defined, they are defined once in MasterType now
struct MasterType
{
	typedef Polaris_Component<Link_Implementation,MasterType,Data_Object> Link;
	typedef Polaris_Component<Node_Implementation,MasterType,Data_Object,Link> Node;
	typedef Polaris_Component<Agent_Implementation,MasterType,Execution_Object> Agent;
	typedef Polaris_Component<Network_Implementation,MasterType,Data_Object> Network;
	typedef Polaris_Component<RNG_Components::Implementations::RngStream_Implementation,MasterType,Data_Object> RNG;

	typedef m_array<int>	matrix_int;
	typedef m_array<double> matrix_double;
	typedef Polaris_Component<PopSyn::Implementations::Synthesis_Zone_Implementation, MasterType, Data_Object> zone;
	typedef Polaris_Component<PopSyn::Implementations::ADAPTS_Population_Unit_Implementation, MasterType, Data_Object> pop_unit;
	typedef Polaris_Container<hash_multimap<int,Node*>> node_map;
};




int main()
{

	define_component_interface(zone_itf,MasterType::zone,PopSyn::Prototypes::Synthesis_Zone_Prototype,NULLTYPE);
	zone_itf* my_zone = (zone_itf*)Allocate<MasterType::zone>();

	define_simple_container_interface(distribution_itf,MasterType::zone::Target_Joint_Distribution_type,Multidimensional_Random_Access_Array_Prototype, MasterType::zone::Target_Joint_Distribution_type::unqualified_value_type ,NULLTYPE);
	distribution_itf* distribution = my_zone->Target_Joint_Distribution<distribution_itf*>();
	distribution_itf::index_type index;
	index.push_back(3); index.push_back(4); index.push_back(4);
	distribution->resize(index,1.0);
	cout << distribution->size();
	cout << endl<<distribution->size(0)<<","<<distribution->size(1)<<","<<distribution->size(2);

	my_zone->Fit_Joint_Distribution_To_Marginal_Data<MasterType::zone::Target_Joint_Distribution_type::unqualified_value_type>();
	distribution_itf::iterator itr = distribution->begin();

	for (; itr != distribution->end(); ++itr)
	{
		
	}


	define_component_interface(Rand_Interface,MasterType::RNG,RNG_Prototype,NULLTYPE);
	Rand_Interface* r = (Rand_Interface*)Allocate<MasterType::RNG>();
	r->Initialize<double>(2.0);

	

	// Initialize Node
	define_component_interface(Node_Interface,MasterType::Node,Node_Prototype,MasterType::Link);

	// Allocate version with casting dictated by 2nd template parameter
	Node_Interface* node_itf=(Node_Interface*)Allocate<MasterType::Node>();
	node_itf->id<int>(3);
	


	// Initialize Link
	define_component_interface(Link_Interface,MasterType::Link,::Link_Prototype,MasterType::Node);
	Link_Interface* link_itf=(Link_Interface*)Allocate<MasterType::Link>();
	link_itf->id<int>(4);
	
	// Notice how polaris variables can be accessed like regular variables
	link_itf->length<int>(4);
	
	// Notice flexibility of passing in length
	int len=8;
	link_itf->length<int*>(&len);
	
	// Add reference to node, notice flexibility of passing in node
	link_itf->a_node<Node_Interface&>(*node_itf);
	link_itf->b_node<Node_Interface*>(node_itf);

	
	
	// Initialize Network
	define_component_interface(Network_Interface,MasterType::Network,::Network_Prototype,NULLTYPE);
	Network_Interface* net_itf=(Network_Interface*)Allocate<MasterType::Network>();
	

	// Add reference to link, notice 2 ways of accessing container
	define_container_and_value_interface(Links,Link,Network_Interface::get_type_of(links),Random_Access_Sequence_Prototype,::Link_Prototype,NULLTYPE);
	access(Links,net_itf->links).push_back(link_itf); // supports IDE
	net_itf->links<Links*>()->push_back(link_itf);



	// Initialize Agent
	define_component_interface(Agent_Interface,MasterType::Agent,Agent_Prototype,NULLTYPE);
	Agent_Interface* agent_itf=(Agent_Interface*)Allocate<MasterType::Agent>();
	// Add reference to network
	agent_itf->network<Network_Interface*>(net_itf);

	
	
	//agent_itf->Initialize_Agent<NULLTYPE>(); this will give a compile time error
	agent_itf->Initialize_Agent<int>();


	// resize the agents int array
	define_simple_container_interface(my_array,MasterType::matrix_int,Multidimensional_Random_Access_Array_Prototype,int,NULLTYPE);
	my_array& arr = agent_itf->my_int_matrix<my_array&>();
	my_array::index_type idx;
	my_array::iterator itr = arr.begin();

	for (itr; itr != arr.end(); ++itr) cout << endl <<*itr;

	idx.push_back(2); idx.push_back(3); idx.push_back(2);
	arr.resize(idx,3);

	for (itr=arr.begin(); itr != arr.end(); ++itr) 
	{
		cout << endl <<*itr;
	}

	idx[0]=1; idx[1]=1; idx[2]=1;
	arr[idx] = 123;


	// Use START() instead of world->Start_Turning()
	START();

	// Break Here
	bool pause=true;
}


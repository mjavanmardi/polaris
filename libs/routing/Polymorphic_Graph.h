#pragma once
#include "boost\intrusive\set.hpp"
#include <fstream>
#include "Polymorphic_Graph_Pool.h"

using namespace boost::intrusive;

namespace polaris
{
	//prototype struct Input_Polymorphic_Connected_Edge
	//{
	//	//tag_as_prototype;

	//	void Copy(Input_Polymorphic_Connected_Edge<ComponentType>& copy) { this_component()->Copy((ComponentType&)copy); }

	//	long long id() { return this_component()->id<long long>(); }

	//	float pos_x() { return this_component()->pos_x<float>(); }
	//	float pos_y() { return this_component()->pos_y<float>(); }

	//	/*
	//	Several options to convey Neighbor_Set<CurrentType,NeighborType>

	//	- CurrentType should be known and will be able to be statically referenced

	//	- NeighborType can be conveyed by one of these methods:
	//		* User places a typelist including the neighbor types and establishes different input edges for different connection types
	//		* User pre-allocates Neighbor_Set<CurrentType,NeighborType> or asks graph to do so, this will be stored and copied into the correct space later
	//			- This option is probably the most realistic, force them to create at least some factory object which has knowledge of these types which can be used by the graph compiler
	//			- If I set it up right, they might not even have to allocate it, simply by virtue of which sub-structure (within Input_Connected_Edge) is passed in, I can allocate it based on template deduction
	//		* User stores some type identifier for the neighbor type (via virtual function or rtti variable) - graph runs through types and deduces correct constructor
	//			- This would be aided by some global typelist of possible neighbor types which can be run through
	//	*/
	//	
	//	template<typename Neighbor_Edge_Type>
	//	Sequence<typename ComponentType::forward_edge_ids_type,global_edge_id>* forward_edge_ids(){ return this_component()->forward_edge_ids< Sequence<typename ComponentType::forward_edge_ids_type,global_edge_id>* >(); }
	//};

	//prototype struct Polymorphic_Graph_Assembler_Connected_Edge
	//{
	//	tag_as_prototype;

	//	typedef typename ComponentType::edge_type edge_type;
	//	typedef typename ComponentType::output_graph_type output_graph_type;
	//	
	//	bool compiled(){ return this_component()->compiled<bool>(); }
	//	void compiled(bool value){ this_component()->compiled<bool>(value); }

	//	template<typename Input_Edge_Type>
	//	void Add_Edge(Input_Polymorphic_Connected_Edge<Input_Edge_Type>* new_edge)
	//	{
	//		this_component()->Add_Edge(new_edge);
	//	}

	//	template<typename Input_Edge_Type>
	//	Interactive_Graph< output_graph_type >* Compile_Graph(typename edge_construction_callback<Input_Edge_Type, edge_type>::type construction_callback)
	//	{
	//		if(!compiled())
	//		{
	//			Interactive_Graph< output_graph_type >* return_value = this_component()->Compile_Graph<Input_Edge_Type>(construction_callback);

	//			if(return_value != nullptr) compiled(true);

	//			return return_value;
	//		}
	//		
	//		return nullptr;
	//	}
	//};


	/*
	Question to Answer - is this a comfortable space for users to overload?
	- They can create a new Neighbor_Set class which is an explicit specialization of Neighbor_Set
	- They can create new Visit_Neighbors / Evaluate_Neighbor functions within this body which utilizes concept checks to partially specialize the functionality

	Somehow it feels like these functions should be global template functions ...

	What may be comfortable, but not minimalistic is to enumerate all possible combinations for Current_Edge_Type, Neighbor_Edge_Type
	- Once there is dissatisfaction over this, tag-based overloads can be used
	*/


	struct global_id
	{
		unsigned char graph_id;
		unsigned long long edge_id : 56;
	};

	prototype struct Polymorphic_Graph_Pool
	{
		tag_as_prototype;

		typedef typename ComponentType::base_edge_type base_edge_type;

		bool Link_Graphs()
		{
			return this_component()->Link_Graphs();
		}

		unsigned char Add_Graph(Polymorphic_Graph<graph_type>* graph)
		{
			return this_component()->Add_Graph(graph);
		}

		Polymorphic_Graph<graph_type>* Get_Graph(unsigned char graph_id)
		{
			return this_component()->Get_Graph(graph);
		}

		Edge<base_edge_type>* Get_Edge(global_edge_id& edge_id)
		{
			return this_component()->Get_Edge(edge_id);
		}
	};


	prototype struct Interactive_Graph
	{
		tag_as_prototype;
		
		typedef typename ComponentType::edge_type edge_type;
		typedef typename ComponentType::edges_type edges_type;

		void Copy_Graph(Interactive_Graph<ComponentType>* io_copy, typename edge_update_callback<edge_type>::type callback = nullptr)
		{
			return this_component()->Copy_Graph((ComponentType*)io_copy,callback);
		}

		void Update_Edge(long long id, typename edge_update_callback<edge_type>::type callback = nullptr)
		{
			this_component()->Update_Edge(id,callback);
		}

		void Update_Edges(boost::container::vector<int>* edge_set = nullptr, typename edge_update_callback<edge_type>::type callback = nullptr)
		{
			this_component()->Update_Edges(edge_set,callback);
		}

		template<typename ExtendedInterface>
		Packed_Edge<edge_type,ExtendedInterface>* Get_Edge(long long id)
		{
			return this_component()->Get_Edge< Packed_Edge<edge_type,ExtendedInterface>* >(id);
		}
		
		template<typename ExtendedInterface>
		Sequence<edges_type,Packed_Edge<edge_type,ExtendedInterface>*>* edges()
		{
			return this_component()->edges< Sequence<edges_type,Packed_Edge<edge_type,ExtendedInterface>*>* >();
		}

		Sequence<edges_type,Packed_Edge<edge_type,NT>*>* edges()
		{
			return this_component()->edges< Sequence<edges_type,Packed_Edge<edge_type,NT>*>* >();
		}
	};



	prototype struct Routable_Agent
	{
		typedef typename ComponentType::agent_state_type agent_state_type;

		template<typename CurrentEdgeType, typename DestinationEdgeType>
		bool at_destination(CurrentEdgeType* current, DestinationEdgeType* destination){ this_component()->at_destination(current, destination); }

		template<typename CurrentEdgeType, typename DestinationEdgeType>
		float estimated_cost_between(CurrentEdgeType* current, DestinationEdgeType* destination){ this_component()->estimated_cost_between(current, destination) ;}

		template<typename CurrentEdgeType, typename NeighborEdgeType>
		float cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor){ this_component()->cost_between(current, neighbor); }	

		template<typename CurrentEdgeType,typename NeighborEdgeType>
		float time_cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor){ this_component()->time_cost_between(current, neighbor); }	

		template<typename CurrentEdgeType,typename NeighborEdgeType>
		float update_label(CurrentEdgeType* current, NeighborEdgeType* neighbor){ this_component()->update_label(current, neighbor); }	
	};


	struct Base_Edge_Attributes : public set_base_hook<optimize_size<false>>
	{
		typedef Base_Edge_Attributes base_attribute_type;

		void reset()
		{
			_cost_from_origin = 0.0f;
			_time_from_origin = 0.0f;
			_estimated_cost_origin_destination = 0.0f;

			_in_closed_set = false;
			_marked_for_reset = false;
			_in_open_set = false;
		}

		t_data(float, cost_from_origin);
		t_data(float, time_from_origin);
		t_data(float, estimated_cost_origin_destination);
		t_data(bool, marked_for_reset);
		t_data(bool, in_closed_set);
		t_data(bool, in_open_set);

		friend __forceinline bool operator< (const Base_Edge_Attributes &lhs,const Base_Edge_Attributes &rhs)
		{  return lhs._estimated_cost_origin_destination < rhs._estimated_cost_origin_destination;  }
		friend __forceinline bool operator> (const Base_Edge_Attributes &lhs,const Base_Edge_Attributes &rhs)
		{  return lhs._estimated_cost_origin_destination > rhs._estimated_cost_origin_destination;  }
		friend __forceinline bool operator== (const Base_Edge_Attributes &lhs,const Base_Edge_Attributes &rhs)
		{  return lhs._estimated_cost_origin_destination == rhs._estimated_cost_origin_destination;  }
	};

	template<typename Attribute_Type = NT,typename Neighbor_Set_Type = NT>
	struct Polymorphic_Edge_Implementation : public Attribute_Set
	{
		typedef Attribute_Type attribute_type;
		typedef Neighbor_Attribute_Type neighbor_attribute_type;

		long long _id;
		const long long id(){ return _id; }

		Anonymous_Neighbor_Set* begin_neighbor_set(){ return ((char*)this) + sizeof(Polymorphic_Edge_Implementation); }
		
		Anonymous_Neighbor_Set* _end_neighbor_set;
		const Anonymous_Neighbor_Set* const end_neighbor_set(){ return _end_neighbor_set; }
	};










	/*

	Hypothetical Construction Flow



	struct global_id
	{
		unsigned char graph_id;
		unsigned long long edge_id : 56;
	};

	struct Anonymous_Neighbor_Edge_Factory
	{
		virtual void Construct_Neighbor_Edge_Set( void* memory ) = 0;
		virtual Anonymous_Neighbor_Edge_Factory* Create_Copy() = 0;

		virtual deque<global_id>& neighbors() = 0;
	};




	// User Creates a Unique Version of this Class which Matches the Above API

	template<typename Current_Edge_Type,typename Neighbor_Attribute_Type,typename Neighbor_Edge_Type>
	struct Neighbor_Edge_Factory : public Anonymous_Neighbor_Edge_Factory
	{
	private:
		virtual void Construct_Neighbor_Edge_Set(void* memory)
		{
			new (memory) Polymorphic_Edge<Current_Edge_Type,Neighbor_Attribute_Type,Neighbor_Edge_Type>();
		}


	public:
		deque<global_id> _neighbors;

		virtual deque<global_id>& neighbors(){ return _neighbors; };


		virtual Anonymous_Neighbor_Edge_Factory* Create_Copy()
		{
			Neighbor_Edge_Factory* copy = new Neighbor_Edge_Factory();

			for(deque<global_id>::iterator neighbor_itr = _neighbors.begin();neighbor_itr!=_neighbors.end();neighbor_itr++)
			{
				copy->_neighbors.push_back( *neighbor_itr );
			}

			return (Anonymous_Neighbor_Edge_Factory*)copy;
		}
	};


	template<typename Input_Edge_Type>
	struct Input_Edge
	{
		void Copy(Input_Edge_Type& copy)
		{
			((Input_Edge_Type*)this)->Copy(copy);
		}

		long long edge_id(){((Input_Edge_Type*)this)->edge_id();}

		deque<Anonymous_Neighbor_Edge_Factory*>& connection_types(){((Input_Edge_Type*)this)->connection_types());}
	};
	
	// User Creates a Unique Version of this Class which Matches the Above API

	struct Input_Edge_Implementation
	{	
		void Copy(Input_Edge_Implementation& copy)
		{
			copy.edge_id = edge_id;

			for(deque<Anonymous_Neighbor_Edge_Factory*>::iterator itr = connection_types.begin();itr!=connection_types.end();itr++)
			{
				Anonymous_Neighbor_Edge_Factory* neighbor_set_copy = (*itr)->Create_Copy();

				copy.connection_types.push_back( neighbor_set_copy );
			}
		}

		t_data(long long,edge_id);

		t_data(deque<Anonymous_Neighbor_Edge_Factory*>,connection_types);
	};

	template<typename Input_Edge_Type = NT, typename Edge_Type = NT>
	struct edge_construction_callback
	{
		typedef void(*type)(Input_Edge_Type* input_edge_ptr,Edge_Type* edge_ptr);
	};

	struct Graph
	{
		template<typename Input_Edge_Type>
		void Push_Edge(Input_Edge<Input_Edge_Type>* input_edge);

		template<typename Input_Edge_Type, typename Stored_Edge_Type>
		void Compile(edge_construction_callback<Input_Edge_Type,Stored_Edge_Type>::type construction_callback);
	};


	struct Graph_Pool
	{
		typedef unsigned char graph_id;

		graph_id Push_Graph(Graph* input_graph);

		void Link_Graphs();
	};

	*/








	struct Agent_A{};
	struct Agent_B{};

	template<typename Base_Attribute_Type>
	struct Anonymous_Neighbor_Set
	{
		virtual Anonymous_Neighbor_Set* Visit_Neighbors(Routable_Agent<Agent_A>* agent, void* current, boost::intrusive::multiset< Base_Attribute_Type >& open_set, boost::container::deque< A_Star_Edge<Base_Attribute_Type>* >& modified_edges) = 0;
		virtual Anonymous_Neighbor_Set* Visit_Neighbors(Routable_Agent<Agent_B>* agent, void* current, boost::intrusive::multiset< Base_Attribute_Type >& open_set, boost::container::deque< A_Star_Edge<Base_Attribute_Type>* >& modified_edges) = 0;
	};

	template<typename Current_Edge_Type,typename Neighbor_Attribute_Type,typename Neighbor_Edge_Type>
	struct Neighbor_Set_Base : public Anonymous_Neighbor_Set<typename ComponentType::current_edge_type::base_attribute_type>
	{
		typedef Neighbor_Set<Current_Edge_Type,Neighbor_Attribute_Type,Neighbor_Edge_Type> ComponentType;
		typedef Current_Edge_Type current_edge_type;
		typedef Current_Edge_Type::base_attribute_type base_attribute_type;
		typedef Neighbor_Attribute_Type neighbor_attribute_type;
		typedef Neighbor_Edge_Type neighbor_edge_type;

		virtual Anonymous_Neighbor_Set<typename ComponentType::base_attribute_type>* Visit_Neighbors(Routable_Agent<Agent_A>* agent, void* current, boost::intrusive::multiset< typename ComponentType::base_attribute_type >& open_set, boost::container::deque< base_attribute_type* >& modified_edges)
		{
			this_component()->Visit_Neighbors(agent,current,open_set,modified_edges);
		};
		
		virtual Anonymous_Neighbor_Set<typename ComponentType::base_attribute_type>* Visit_Neighbors(Routable_Agent<Agent_B>* agent, void* current, boost::intrusive::multiset< typename ComponentType::base_attribute_type >& open_set, boost::container::deque< base_attribute_type* >& modified_edges)
		{
			this_component()->Visit_Neighbors(agent,current,open_set,modified_edges);
		};

		t_data(unsigned int, num_forward_edges);

		struct Neighbor_Edge_Reference : public neighbor_attribute_type
		{
			t_pointer(neighbor_edge_type*,neighbor);
		};

		Neighbor_Edge_Reference* forward_edges(){ return (Neighbor_Edge_Reference*)((char*)this + sizeof(ComponentType)); }
		const Neighbor_Edge_Reference* const end_forward_edges(){ return (const Neighbor_Edge_Reference* const)(((char*)this) + sizeof(Neighbor_Edge_Reference) * _num_forward_edges); }
	};


	template<typename Current_Edge_Type,typename Neighbor_Attribute_Type,typename Neighbor_Edge_Type>
	struct Neighbor_Set : public Neighbor_Set_Base< Current_Edge_Type,Neighbor_Attribute_Type,Neighbor_Edge_Type >
	{
		template<typename AgentType>
		Anonymous_Neighbor_Set<base_attribute_type>* Visit_Neighbors(Routable_Agent<AgentType>* agent, Current_Edge_Type* current, boost::intrusive::multiset< base_attribute_type >& open_set, boost::container::deque< A_Star_Edge<base_attribute_type>* >& modified_edges)
		{
			const Neighbor_Edge_Reference* const end_neighbor_itr = end_forward_edges();		

			for(Neighbor_Edge_Reference* neighbor_itr = forward_edges(); neighbor_itr != end_neighbor_itr; ++neighbor_itr)
			{
				Evaluate_Neighbor<AgentType>(agent,current,neighbor_itr,open_set,modified_edges);
			}

			return (Anonymous_Neighbor_Set<base_attribute_type>*)end_neighbor_itr;
		}

		template<typename AgentType>
		void Evaluate_Neighbor(Routable_Agent<AgentType>* agent, Current_Edge_Type* current, Neighbor_Edge_Reference* neighbor, boost::intrusive::multiset< base_attribute_type >& open_set, boost::container::deque< A_Star_Edge<base_attribute_type>* >& modified_edges)
		{
			A_Star_Edge<Neighbor_Edge_Type>* current_neighbor = (A_Star_Edge<Neighbor_Edge_Type>*)neighbor->neighbor();

			if(current_neighbor->in_closed_set()) return;

			float cost_from_origin = current->cost_from_origin() + agent->cost_between(current,neighbor);

			if( cost_from_origin < current_neighbor->cost_from_origin() )
			{
				if( current_neighbor->in_open_set() ) open_set.erase( open_set.iterator_to( *((base_edge_type*)current_neighbor)  );

				float time_from_origin = current->time_from_origin() + agent->time_cost_between(current, neighbor);

				if(!current_neighbor->marked_for_reset())
				{
					modified_edges.push_back(current_neighbor);
					current_neighbor->marked_for_reset(true);
				}

				current_neighbor->came_from( current );

				current_neighbor->cost_from_origin( cost_from_origin );
				current_neighbor->time_from_origin( time_from_origin );

				float neighbor_estimated_cost_origin_destination = cost_from_origin + agent->estimated_cost_between(current,neighbor);

				current_neighbor->estimated_cost_origin_destination( neighbor_estimated_cost_origin_destination );
				
				// update the label
				open_set.insert( *((base_edge_type*)current_neighbor) );

				current_neighbor->in_open_set(true);

				agent->update_label(current,neighbor);
			}
		}
	};

	struct Highway_Edge{};
	struct Transit_Edge{};
	struct Highway_Transit_Attributes{};

	template<>
	struct Neighbor_Set<Highway_Edge,Transit_Edge,Highway_Transit_Attributes> : public Neighbor_Set_Base<Highway_Edge,Transit_Edge,Highway_Transit_Attributes>
	{
		template<typename AgentType>
		Anonymous_Neighbor_Set<base_attribute_type>* Visit_Neighbors(Routable_Agent<AgentType>* agent, Current_Edge_Type* current, boost::intrusive::multiset< base_attribute_type >& open_set, boost::container::deque< A_Star_Edge<base_attribute_type>* >& modified_edges)
		{
			/* Visit Different Neighbors for Highway-Transit Connection Case */
		}

		template<typename AgentType>
		void Evaluate_Neighbor(Routable_Agent<AgentType>* agent, Neighbor_Set_Base::current_edge_type* current, Neighbor_Set_Base::Neighbor_Edge_Reference* neighbor, boost::intrusive::multiset< base_attribute_type >& open_set, boost::container::deque< base_attribute_type* >& modified_edges)
		{
			/* Evaluate Labels Differently for Highway-Transit Connection */
		}

		template<>
		void Evaluate_Neighbor<Agent_B>(Routable_Agent<Agent_B>* agent, Highway_Edge* current, Neighbor_Set_Base::Neighbor_Edge_Reference* neighbor, boost::intrusive::multiset< base_attribute_type >& open_set, boost::container::deque< base_attribute_type* >& modified_edges)
		{
			/* Evaluate Differently for Agent_B Case */
		}
	};

	template<typename AgentType,typename GraphPoolType>
	static void Polymorphic_A_Star(Routable_Agent<AgentType>* agent, Polymorphic_Graph_Pool<GraphPoolType>* graph_pool, global_edge_id& start_id, global_edge_id& end_id, boost::container::deque< global_edge_id >& out_path)
	{
		typedef typename Polymorphic_Graph_Pool<GraphPoolType>::base_edge_type base_edge_type;
		typedef typename Agent<AgentType>::agent_state_type agent_state_type;

		boost::container::deque< A_Star_Edge<base_edge_type>* > modified_edges;
		
		boost::intrusive::multiset< base_edge_type > open_set;

		A_Star_Edge<base_edge_type>* start = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(start_id);
		if(start == nullptr){ THROW_WARNING("Origin: " << start_id.edge_id << " not found in graph pool!"); return nullptr; }

		A_Star_Edge<base_edge_type>* end = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(end_id);
		if(end == nullptr){ THROW_WARNING("Destination: " << end_id.edge_id << " not found in graph!"); return nullptr; }

		start->cost_from_origin(0.0f);
		
		float initial_estimated_cost_origin_destination = start->cost_from_origin() + agent->estimated_cost_between(start,end);

		start->estimated_cost_origin_destination( initial_estimated_cost_origin_destination );
		
		open_set.insert( *((base_edge_type*)start) );

		if( !start->marked_for_reset() )
		{
			modified_edges.push_back(start);
			start->marked_for_reset(true);
		}
		
		bool success = false;
		
		while( open_set.size() )
		{
			A_Star_Edge<base_edge_type>* current = (A_Star_Edge<base_edge_type>*)&(*open_set.begin());

			if( agent->at_destination(current, end) )
			{
				success = true;
				break;
			}
			
			open_set.erase( open_set.iterator_to( *((base_edge_type*)current) ) );

			current->in_closed_set(true);

			Anonymous_Neighbor_Set* neighbor_set_iterator = current->begin_neighbor_set();
			const Anonymous_Neighbor_Set* const neighbor_set_end = current->end_neighbor_set();

			while( neighbor_set_iterator ! = neighbor_set_iterator )
			{
				neighbor_set_iterator = neighbor_set_iterator->Visit(agent, current, open_set);
			}
		}

		for(boost::container::deque< A_Star_Edge<base_edge_type>* >::iterator itr = modified_edges.begin();itr!=modified_edges.end();itr++)
		{
			(*itr)->reset();
		}

		if(success)
		{
			A_Star_Edge<base_edge_type>* current = end;
			A_Star_Edge<base_edge_type>* cached_current = current;

			while(current != nullptr)
			{
				out_path.push_back(current->id());

				current = current->came_from();
				
				cached_current->came_from(nullptr);

				cached_current = current;
			}

			std::reverse(out_path.begin(),out_path.end());

			return end;
		}
		else return nullptr;
	}
}
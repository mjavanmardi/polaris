#pragma once
#include "Routable_Agent.h"

//contains all of the parts necessary to construct a routable graph later 
namespace polaris
{
	typedef unsigned long long p_edge_id_type;
	typedef unsigned int p_graph_id_type;

	struct global_edge_id
	{
		p_graph_id_type graph_id;
		p_edge_id_type edge_id;
	};

	struct Input_Connection_Group
	{
		virtual void Construct_Connection_Group( void* memory ) = 0;
		virtual size_t Connection_Group_Size() = 0;

		virtual Input_Connection_Group* Create_Copy() = 0;

		virtual p_graph_id_type neighbor_graph_id() = 0;
		virtual std::deque<p_edge_id_type>& neighbors() = 0;
	};

	prototype struct Connection;
	prototype struct Connection_Group;

	template<typename Connection_Group_Type = NT>
	struct Input_Connection_Group_Implementation : public Input_Connection_Group
	{
		typedef Connection_Group_Type connection_group_type;
		typedef typename connection_group_type::connection_type connection_type;
		typedef typename connection_group_type::connection_attributes_type connection_attributes_type;

		virtual void Construct_Connection_Group(void* memory)
		{
			new (memory) Connection_Group_Type();

			Connection_Group<Connection_Group_Type>* connection_group = (Connection_Group<Connection_Group_Type>*)memory;

			connection_group->num_forward_edges((uint) _neighbors.size() );

			Connection<connection_type>* fwd_edge_itr = connection_group->forward_edges();
			auto connection_attributes_itr = _neighbor_attributes.begin();

			connection_group->linked_graph( _neighbor_graph_id );

			for(auto itr = _neighbors.begin();itr!=_neighbors.end();itr++)
			{
				fwd_edge_itr->edge_id( *itr );

				connection_attributes_type* attributes = fwd_edge_itr->connection_attributes();

				(*attributes) = (*connection_attributes_itr);

				connection_attributes_itr++;
				fwd_edge_itr = fwd_edge_itr->next_connection();
			}
		}

		virtual size_t Connection_Group_Size( ){ return ( sizeof(connection_group_type) + sizeof(connection_type)*_neighbors.size() ); }

		std::deque<connection_attributes_type>& neighbor_attributes(){ return _neighbor_attributes; }

		virtual Input_Connection_Group* Create_Copy()
		{
			Input_Connection_Group_Implementation* copy = new Input_Connection_Group_Implementation();
			copy->_neighbor_graph_id = _neighbor_graph_id;

			for(auto neighbor_itr = _neighbors.begin();neighbor_itr!=_neighbors.end();neighbor_itr++)
			{
				copy->_neighbors.push_back( *neighbor_itr );
			}
		
			for(auto neighbor_itr = _neighbor_attributes.begin();neighbor_itr!=_neighbor_attributes.end();neighbor_itr++)
			{
				copy->_neighbor_attributes.push_back( *neighbor_itr );
			}

			return (Input_Connection_Group*)copy;
		}

		virtual p_graph_id_type neighbor_graph_id(){ return _neighbor_graph_id; }
		
		virtual std::deque<p_edge_id_type>& neighbors(){ return _neighbors; }

		static p_graph_id_type _neighbor_graph_id;
		std::deque<p_edge_id_type> _neighbors;
		std::deque<connection_attributes_type> _neighbor_attributes;
	};
	//ADGE ATTRIBUTES TYPES HAS TO BE AN PLAIN old data (POD) so that the copy operation at line 116 works, otherwise need to define a copy operator
	template<typename Connection_Group_Type>
	p_graph_id_type Input_Connection_Group_Implementation<Connection_Group_Type>::_neighbor_graph_id;

	template<typename Edge_Attributes_Type = NT>
	struct Input_Edge : public Edge_Attributes_Type
	{
		Input_Edge():Edge_Attributes_Type(),_connection_groups(){}

		Input_Edge(const Input_Edge& original)
		{
			Edge_Attributes_Type* these_attributes = this;
			const Edge_Attributes_Type* original_attributes = &original;

			(*these_attributes) = (*original_attributes);

			for(std::deque<Input_Connection_Group*>::const_iterator itr = original._connection_groups.begin();itr!=original._connection_groups.end();itr++)
			{
				Input_Connection_Group* neighbor_set_copy = (*itr)->Create_Copy();

				_connection_groups.push_back( neighbor_set_copy );
			}
		}

		void Construct_Edge_Attributes( Edge_Attributes_Type* edge_attributes )
		{
			Edge_Attributes_Type* these_attributes = (Edge_Attributes_Type*)this;

			(*edge_attributes) = (*these_attributes);
		}

		t_object(std::deque<Input_Connection_Group*>,connection_groups);
	};


}
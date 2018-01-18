#pragma once
#include "Connection_Group.h"

namespace polaris
{
	template<typename MasterType, typename Current_Graph_Type, typename Neighbor_Graph_Type, typename Connection_Attributes_Type>
	struct Custom_Connection_Group;

	template<typename MasterType, typename Current_Graph_Type, typename Neighbor_Graph_Type, typename Connection_Attributes_Type>
	struct Connection_Group_Base : public Anonymous_Connection_Group< MasterType, typename Current_Graph_Type::base_edge_type >
	{
		// %%%RLW
		// http://stackoverflow.com/questions/282800/c-odd-compile-error-error-changes-meaning-of-object-from-class-object
		//
		typedef Anonymous_Connection_Group< MasterType, typename Current_Graph_Type::base_edge_type > mAnonymous_Connection_Group;

		Connection_Group_Base():_num_forward_edges(0),_num_backward_edges(0){}
		
		typedef Custom_Connection_Group<MasterType,Current_Graph_Type,Neighbor_Graph_Type,Connection_Attributes_Type> ComponentType;

		typedef Current_Graph_Type current_graph_type;
		typedef Neighbor_Graph_Type neighbor_graph_type;

		typedef typename current_graph_type::graph_pool_type graph_pool_type;
		typedef typename current_graph_type::stored_edge_type current_edge_type;
		typedef typename neighbor_graph_type::stored_edge_type neighbor_edge_type;
		
		typedef typename current_edge_type::base_edge_type base_edge_type;
		typedef Connection_Attributes_Type connection_attributes_type;

		virtual mAnonymous_Connection_Group* Next_Connection_Group()
		{
			return (mAnonymous_Connection_Group*)end_forward_edges();
		}

		virtual mAnonymous_Connection_Group* Unlink_Edges()
		{
			Connection_Implementation* forward_edge_itr = forward_edges();
			const Connection_Implementation* const end = end_forward_edges();

			while(forward_edge_itr != end)
			{
				forward_edge_itr->_edge_id = forward_edge_itr->_neighbor->edge_id();

				forward_edge_itr = forward_edge_itr->next_connection();
			}

			return (mAnonymous_Connection_Group*)end_forward_edges();
		}

		virtual mAnonymous_Connection_Group* Link_Edges(void* graph_pool)
		{
			Graph_Pool<graph_pool_type>* current_graph_pool = (Graph_Pool<graph_pool_type>*)graph_pool;

			Connection_Implementation* forward_edge_itr = forward_edges();
			const Connection_Implementation* const end = end_forward_edges();

			global_edge_id linked_edge_id;
			linked_edge_id.graph_id = _linked_graph;

			while(forward_edge_itr != end)
			{
				linked_edge_id.edge_id = forward_edge_itr->edge_id();
				forward_edge_itr->_neighbor = (neighbor_edge_type*)( current_graph_pool->template Get_Edge<Neighbor_Graph_Type>(linked_edge_id) );

				if(forward_edge_itr->_neighbor == nullptr)
				{
					THROW_EXCEPTION("Failed to create a link from graph:\n\t" <<  typeid(current_graph_type).name() << "\n\tto graph: " << typeid(neighbor_graph_type).name() << "\n\tusing link: " << linked_edge_id.edge_id);
				}

				forward_edge_itr = forward_edge_itr->next_connection();
			}

			return (mAnonymous_Connection_Group*)end_forward_edges();
		}

		virtual mAnonymous_Connection_Group* Visit_Neighbors(Routable_Agent<typename MasterType::routable_agent_type>* agent, void* current, Routing_Data<base_edge_type>& routing_data)
		{
			return this_component()->Visit_Neighbors(agent,(current_edge_type*)current,routing_data);
		};

		virtual mAnonymous_Connection_Group* Visit_Neighbors(Routable_Agent<typename MasterType::tree_agent_type>* agent, void* current, Routing_Data<base_edge_type>& routing_data)
		{
			return this_component()->Visit_Neighbors((Routable_Agent<typename MasterType::tree_agent_type>*)agent,(current_edge_type*)current,routing_data);
		};

		virtual mAnonymous_Connection_Group* Visit_Neighbors(Routable_Agent<typename MasterType::walk_to_transit_tree_agent_type>* agent, void* current, Routing_Data<base_edge_type>& routing_data)
		{
			return this_component()->Visit_Neighbors((Routable_Agent<typename MasterType::walk_to_transit_tree_agent_type>*)agent, (current_edge_type*)current, routing_data);
		};

		virtual mAnonymous_Connection_Group* Visit_Neighbors(Routable_Agent<typename MasterType::drive_to_transit_tree_agent_type>* agent, void* current, Routing_Data<base_edge_type>& routing_data)
		{
			return this_component()->Visit_Neighbors((Routable_Agent<typename MasterType::drive_to_transit_tree_agent_type>*)agent, (current_edge_type*)current, routing_data);
		};

		virtual mAnonymous_Connection_Group* Visit_Neighbors(Routable_Agent<typename MasterType::multi_modal_tree_agent_type>* agent, void* current, Routing_Data<base_edge_type>& routing_data)
		{
			return this_component()->Visit_Neighbors((Routable_Agent<typename MasterType::multi_modal_tree_agent_type>*)agent, (current_edge_type*)current, routing_data);
		};

		virtual mAnonymous_Connection_Group* Visit_Multimodal_Neighbors(Routable_Agent<typename MasterType::routable_agent_type>* agent, void* current, Routing_Data<base_edge_type>& routing_data, void* graph_pool)
		{
			return this_component()->Visit_Multimodal_Neighbors(agent, (current_edge_type*)current, routing_data, (Graph_Pool<graph_pool_type>*)graph_pool);
		};
		
		//virtual Anonymous_Connection_Group* Visit_Neighbors(Routable_Agent<typename MasterType::time_dependent_agent_type>* agent, void* current, Routing_Data<base_edge_type>& routing_data)
		//{
		//	return this_component()->Visit_Neighbors((Routable_Agent<typename MasterType::time_dependent_agent_type>*)agent,(current_edge_type*)current,routing_data);
		//};

		t_static_data(p_graph_id_type, linked_graph);

		t_data(unsigned int, num_forward_edges);
		t_data(unsigned int, num_backward_edges);

		struct Connection_Implementation : public connection_attributes_type
		{
			typedef neighbor_edge_type ci_neighbor_edge_type;
			typedef Connection_Implementation connection_type;
			typedef connection_attributes_type ci_connection_attributes_type;

			ci_connection_attributes_type* connection_attributes(){ return (ci_connection_attributes_type*)this; }

			ci_neighbor_edge_type* neighbor(){return _neighbor;}
			void neighbor(neighbor_edge_type* value){_neighbor = value;}

			p_edge_id_type edge_id(){return _edge_id;}
			void edge_id(p_edge_id_type value){_edge_id = value;}

			connection_type* next_connection(){return (connection_type*)(this + 1);}

			union
			{
				ci_neighbor_edge_type* _neighbor;
				p_edge_id_type _edge_id;
			};
		};

		typedef Connection_Implementation connection_type;

		Connection_Implementation* forward_edges(){ return (Connection_Implementation*)(this_component() + 1); }
		const Connection_Implementation* const end_forward_edges(){ return (const Connection_Implementation* const)((char*)forward_edges() + sizeof(Connection_Implementation) * _num_forward_edges); }

		//Connection_Implementation* backward_edges(){ return (Connection_Implementation*) end_forward_edges(); }
		//const Connection_Implementation* const end_backward_edges(){ return (const Connection_Implementation* const)((char*)backward_edges() + sizeof(Connection_Implementation) * _num_backward_edges); }
	};
	
	template<typename MasterType, typename Current_Graph_Type, typename Neighbor_Graph_Type, typename Connection_Attributes_Type>
	p_graph_id_type Connection_Group_Base<MasterType,Current_Graph_Type,Neighbor_Graph_Type,Connection_Attributes_Type>::_linked_graph;
}
#pragma once
#include "Interactive_Graph.h"

namespace polaris
{
	template<typename Attribute_Set = NT,typename Neighbor_Attribute_Set = NT>
	struct Packed_Edge_Implementation : public Attribute_Set
	{
		typedef Attribute_Set attributes_type;

		struct Neighbor_Edge : public Neighbor_Attribute_Set
		{
			typedef Packed_Edge_Implementation edge_type;
			
			float weight(){ return 0.0f; }

			Neighbor_Edge* next(){ return (Neighbor_Edge*)(this + 1); }

			Packed_Edge_Implementation* edge(){ return _edge; }
			void edge(Packed_Edge_Implementation* value){ _edge = value; }
			
			Packed_Edge_Implementation* _edge;
		};
		
		typedef Neighbor_Edge neighbor_type;
		typedef Neighbor_Attribute_Set neighbor_attributes_type;

		Packed_Edge_Implementation(int num_forward_edges,int num_backward_edges):_num_forward_edges(num_forward_edges),_num_backward_edges(num_backward_edges){};

		int _num_forward_edges;
		const int num_forward_edges(){ return _num_forward_edges; }
		void num_forward_edges(const int value){ _num_forward_edges = value; }

		int _num_backward_edges;
		const int num_backward_edges(){ return _num_backward_edges; }
		void num_backward_edges(const int value){ _num_backward_edges = value; }

		int _id;
		const int id(){ return _id; }

		Neighbor_Edge* forward_edges(){ return (Neighbor_Edge*)(((char*)this) + sizeof(Packed_Edge_Implementation)); }
		const Neighbor_Edge* const end_forward_edges(){ return (const Neighbor_Edge* const)(((char*)this) + sizeof(Packed_Edge_Implementation) + sizeof(Neighbor_Edge) * _num_forward_edges); }

		Neighbor_Edge* backward_edges(){ return (Neighbor_Edge*)(((char*)this) + sizeof(Packed_Edge_Implementation) + sizeof(Neighbor_Edge) * _num_forward_edges); }
		const Neighbor_Edge* const end_backward_edges(){ return (const Neighbor_Edge* const)(((char*)this) + sizeof(Packed_Edge_Implementation) + sizeof(Neighbor_Edge) * _num_forward_edges + sizeof(Neighbor_Edge) * _num_backward_edges); }
		
		inline const unsigned int edge_size_bytes()
		{
			return ((sizeof(Packed_Edge_Implementation) + sizeof(Neighbor_Edge) * _num_forward_edges + sizeof(Neighbor_Edge) * _num_backward_edges)/__cache_line_size + 1)*__cache_line_size;
		}

		Packed_Edge_Implementation* next()
		{
			return (Packed_Edge_Implementation*)(((char*)this) + edge_size_bytes());
		}
	};



	template<typename MasterType, typename InheritanceList = NULLTYPELIST, typename Edge_Type = NT>
	struct Packed_Graph_Implementation : public Polaris_Component<MasterType,INHERIT(Packed_Graph_Implementation)>
	{
	public:
		Packed_Graph_Implementation():_ordered_input_edge_reference(nullptr),_input_edge_reference(nullptr),
			_ordered_input_edge_node_reference(nullptr),_input_edge_node_reference(nullptr),
			_ordered_input_node_reference(nullptr),_input_node_reference(nullptr),
			_compiled(false){}

		implementation struct Input_Edge_Proxy : public Polaris_Component<MasterType,INHERIT(Input_Edge_Proxy),NT>
		{
			m_data(int,id,NONE,NONE);

			m_container(boost::container::vector<int>,forward_edge_ids,NONE,NONE);
			m_container(boost::container::vector<int>,backward_edge_ids,NONE,NONE);

			m_data(float,pos_x,NONE,NONE);
			m_data(float,pos_y,NONE,NONE);
		};

		typedef Edge_Type edge_type;
		typedef typename Edge_Type::neighbor_type neighbor_type;
		typedef Packed_Graph_Implementation output_graph_type;

		void Update_Edge(int id, typename edge_update_callback<edge_type>::type callback = nullptr)
		{
			if(_packed_edge_reference.count(id))
			{
				callback(_packed_edge_reference[id]);
			}
			else
			{
				THROW_WARNING("Edge: " << id << " does not exist in graph!");
			}
		}

		void Update_Edges(boost::container::vector<int>* edge_set = nullptr, typename edge_update_callback<edge_type>::type callback = nullptr)
		{
			if(edge_set == nullptr)
			{
				for(boost::container::vector<packed_edge_type*>::iterator itr = _edges.begin();itr != _edges.end();itr++)
				{
					callback( *itr );
				}
			}
			else
			{
				for(boost::container::vector<int>::iterator itr = edge_set->begin();itr != edge_set->end();itr++)
				{
					if(_packed_edge_reference.count( *itr ))
					{
						callback(_packed_edge_reference[*itr]);
					}
					else
					{
						THROW_WARNING("Edge: " << *itr << " does not exist in graph!");
					}
				}
			}
		}

		void Copy_Graph(Packed_Graph_Implementation* copy, typename edge_update_callback<edge_type>::type callback)
		{
			const char* const old_graph_data = _graph_data;

			copy->_graph_data = new char[_graph_size];

			const char* const new_graph_data = copy->_graph_data;

			memcpy((void*)new_graph_data,old_graph_data,_graph_size);


			packed_edge_type* current_edge = (packed_edge_type*)new_graph_data;

			if(callback == nullptr)
			{
				while(current_edge != (packed_edge_type*)(new_graph_data + _graph_size))
				{
					copy->_edges.push_back(current_edge);
					copy->_packed_edge_reference[current_edge->id()] = current_edge;

					const packed_edge_type::Neighbor_Edge* const end_forward_neighbor_itr = current_edge->end_forward_edges();
				
					for(packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->forward_edges(); neighbor_itr != end_forward_neighbor_itr; neighbor_itr = neighbor_itr->next())
					{
						packed_edge_type* updated_edge_pointer = (packed_edge_type*)(new_graph_data + ((char*)_packed_edge_reference[neighbor_itr->edge()->id()] - old_graph_data));
						neighbor_itr->edge(updated_edge_pointer);
					}

					const packed_edge_type::Neighbor_Edge* const end_backward_neighbor_itr = current_edge->end_backward_edges();
				
					for(packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->backward_edges(); neighbor_itr != end_backward_neighbor_itr; neighbor_itr = neighbor_itr->next())
					{
						packed_edge_type* updated_edge_pointer = (packed_edge_type*)(new_graph_data + ((char*)_packed_edge_reference[neighbor_itr->edge()->id()] - old_graph_data));
						neighbor_itr->edge(updated_edge_pointer);
					}

					current_edge = current_edge->next();
				}
			}
			else
			{
				while(current_edge != (packed_edge_type*)(new_graph_data + _graph_size))
				{
					copy->_edges.push_back(current_edge);
					copy->_packed_edge_reference[current_edge->id()] = current_edge;

					const packed_edge_type::Neighbor_Edge* const end_forward_neighbor_itr = current_edge->end_forward_edges();
				
					for(packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->forward_edges(); neighbor_itr != end_forward_neighbor_itr; neighbor_itr = neighbor_itr->next())
					{
						packed_edge_type* updated_edge_pointer = (packed_edge_type*)(new_graph_data + ((char*)_packed_edge_reference[neighbor_itr->edge()->id()] - old_graph_data));
						neighbor_itr->edge(updated_edge_pointer);
					}

					const packed_edge_type::Neighbor_Edge* const end_backward_neighbor_itr = current_edge->end_backward_edges();
				
					for(packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->backward_edges(); neighbor_itr != end_backward_neighbor_itr; neighbor_itr = neighbor_itr->next())
					{
						packed_edge_type* updated_edge_pointer = (packed_edge_type*)(new_graph_data + ((char*)_packed_edge_reference[neighbor_itr->edge()->id()] - old_graph_data));
						neighbor_itr->edge(updated_edge_pointer);
					}

					callback(current_edge);

					current_edge = current_edge->next();
				}
			}
		}

		void Create_Sub_Graph_Positive(Packed_Graph_Implementation* copy, boost::unordered::unordered_set<int>& positive_edges, typename edge_update_callback<edge_type>::type callback)
		{
			unsigned int sub_graph_size = 0;

			for(boost::container::vector<packed_edge_type*>::iterator itr = _edges.begin();itr != _edges.end();itr++)
			{
				if( positive_edges.count((*itr)->id()) )
				{
					packed_edge_type* current_edge = *itr;

					unsigned int active_edges = 0;

					const packed_edge_type::Neighbor_Edge* const end_forward_neighbor_itr = current_edge->end_forward_edges();
					
					for( packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->forward_edges(); neighbor_itr != end_forward_neighbor_itr; neighbor_itr = neighbor_itr->next() )
					{
						if( positive_edges.count(neighbor_itr->edge()->id()) ) ++active_edges;
					}

					const packed_edge_type::Neighbor_Edge* const end_backward_neighbor_itr = current_edge->end_backward_edges();
					
					for( packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->backward_edges(); neighbor_itr != end_backward_neighbor_itr; neighbor_itr = neighbor_itr->next() )
					{
						if( positive_edges.count(neighbor_itr->edge()->id()) ) ++active_edges;
					}

					unsigned int edge_size = ((sizeof(edge_type) + sizeof(neighbor_type)*active_edges)/__cache_line_size + 1) * __cache_line_size;

					//cout << "Visiting: " << (*itr)->id() << ", active edges = " << active_edges << endl;

					sub_graph_size += edge_size;
				}
			}

			//cout << "Sub Graph Size: " << sub_graph_size << ", original size: " << _graph_size << endl;

			copy->_graph_data = new char[sub_graph_size];
			char* graph_itr = copy->_graph_data;

			if(callback == nullptr)
			{
				for(boost::container::vector<packed_edge_type*>::iterator itr = _edges.begin(); itr != _edges.end(); itr++)
				{
					if( positive_edges.count((*itr)->id()) )
					{
						packed_edge_type* current_edge = *itr;

						//cout << "Visting edge: " << current_edge->id() << endl;

						copy->_packed_edge_reference[current_edge->id()] = (packed_edge_type*)graph_itr;
						copy->_edges.push_back( (packed_edge_type*)graph_itr );

						// assemble the body of the edge
						memcpy(graph_itr,current_edge,sizeof(packed_edge_type));

						char* edge_itr = graph_itr + sizeof(packed_edge_type);

						unsigned int forward_edges = 0;

						const packed_edge_type::Neighbor_Edge* const end_forward_neighbor_itr = current_edge->end_forward_edges();
						
						for( packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->forward_edges(); neighbor_itr != end_forward_neighbor_itr; neighbor_itr = neighbor_itr->next() )
						{
							if( positive_edges.count(neighbor_itr->edge()->id()) )
							{
								++forward_edges;
								
								// assemble a forward edge

								//cout << "Copying neighbor: " << neighbor_itr->edge()->id() << endl;

								memcpy( edge_itr, (char*) neighbor_itr, sizeof(neighbor_type) );

								edge_itr += sizeof(neighbor_type);
							}
						}

						((packed_edge_type*)graph_itr)->num_forward_edges(forward_edges);

						unsigned int backward_edges = 0;

						const packed_edge_type::Neighbor_Edge* const end_backward_neighbor_itr = current_edge->end_backward_edges();
						
						for( packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->backward_edges(); neighbor_itr != end_backward_neighbor_itr; neighbor_itr = neighbor_itr->next() )
						{
							if( positive_edges.count(neighbor_itr->edge()->id()) )
							{
								++backward_edges;

								// assemble a backward edge

								memcpy( edge_itr, (char*) neighbor_itr, sizeof(neighbor_type) );

								edge_itr += sizeof(neighbor_type);
							}
						}

						((packed_edge_type*)graph_itr)->num_backward_edges(backward_edges);

						unsigned int edge_size = ((sizeof(edge_type) + sizeof(neighbor_type)*(forward_edges + backward_edges))/__cache_line_size + 1) * __cache_line_size;

						graph_itr += edge_size;
					}
				}
			}
			else
			{
				for(boost::container::vector<packed_edge_type*>::iterator itr = _edges.begin(); itr != _edges.end(); itr++)
				{
					if( positive_edges.count((*itr)->id()) )
					{
						packed_edge_type* current_edge = *itr;

						//cout << "Visting edge: " << current_edge->id() << endl;

						copy->_packed_edge_reference[current_edge->id()] = (packed_edge_type*)graph_itr;
						copy->_edges.push_back( (packed_edge_type*)graph_itr );

						// assemble the body of the edge
						memcpy(graph_itr,current_edge,sizeof(packed_edge_type));

						char* edge_itr = graph_itr + sizeof(packed_edge_type);

						unsigned int forward_edges = 0;

						const packed_edge_type::Neighbor_Edge* const end_forward_neighbor_itr = current_edge->end_forward_edges();
						
						for( packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->forward_edges(); neighbor_itr != end_forward_neighbor_itr; neighbor_itr = neighbor_itr->next() )
						{
							if( positive_edges.count(neighbor_itr->edge()->id()) )
							{
								++forward_edges;
								
								// assemble a forward edge

								//cout << "Copying neighbor: " << neighbor_itr->edge()->id() << endl;

								memcpy( edge_itr, (char*) neighbor_itr, sizeof(neighbor_type) );

								edge_itr += sizeof(neighbor_type);
							}
						}

						((packed_edge_type*)graph_itr)->num_forward_edges(forward_edges);

						unsigned int backward_edges = 0;

						const packed_edge_type::Neighbor_Edge* const end_backward_neighbor_itr = current_edge->end_backward_edges();
						
						for( packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->backward_edges(); neighbor_itr != end_backward_neighbor_itr; neighbor_itr = neighbor_itr->next() )
						{
							if( positive_edges.count(neighbor_itr->edge()->id()) )
							{
								++backward_edges;

								// assemble a backward edge

								memcpy( edge_itr, (char*) neighbor_itr, sizeof(neighbor_type) );

								edge_itr += sizeof(neighbor_type);
							}
						}

						((packed_edge_type*)graph_itr)->num_backward_edges(backward_edges);

						callback( (packed_edge_type*)graph_itr );
						unsigned int edge_size = ((sizeof(edge_type) + sizeof(neighbor_type)*(forward_edges + backward_edges))/__cache_line_size + 1) * __cache_line_size;

						graph_itr += edge_size;
					}
				}
			}
			
			//cout << "Constructed" << endl;

			for(boost::container::vector<packed_edge_type*>::iterator itr = copy->_edges.begin(); itr != copy->_edges.end(); itr++)
			{
				packed_edge_type* current_edge = *itr;

				//cout << "Visting edge: " << current_edge->id() << "," << current_edge->num_forward_edges() << endl;

				const packed_edge_type::Neighbor_Edge* const end_forward_neighbor_itr = current_edge->end_forward_edges();
				
				for( packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->forward_edges(); neighbor_itr != end_forward_neighbor_itr; neighbor_itr = neighbor_itr->next() )
				{
					

					// contains the original pointer, can use this to establish internal pointer linkage
					int id = neighbor_itr->edge()->id();

					//cout << "Updating neighbor: " << id << endl;

					packed_edge_type* new_neighbor_edge = copy->_packed_edge_reference[id];
					neighbor_itr->edge(new_neighbor_edge);
				}


				const packed_edge_type::Neighbor_Edge* const end_backward_neighbor_itr = current_edge->end_backward_edges();
						
				for( packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->backward_edges(); neighbor_itr != end_backward_neighbor_itr; neighbor_itr = neighbor_itr->next() )
				{
					// contains the original pointer, can use this to establish internal pointer linkage
					int id = neighbor_itr->edge()->id();

					packed_edge_type* new_neighbor_edge = copy->_packed_edge_reference[id];
					neighbor_itr->edge(new_neighbor_edge);
				}
			}

			//cout << "References Updated" << endl;
		}

		void Create_Sub_Graph_Negative(Packed_Graph_Implementation* copy, boost::unordered::unordered_set<int>& negative_edges, typename edge_update_callback<edge_type>::type callback)
		{
			unsigned int sub_graph_size = 0;

			for(boost::container::vector<packed_edge_type*>::iterator itr = _edges.begin();itr != _edges.end();itr++)
			{
				if( !negative_edges.count((*itr)->id()) )
				{
					packed_edge_type* current_edge = *itr;

					unsigned int active_edges = 0;

					const packed_edge_type::Neighbor_Edge* const end_forward_neighbor_itr = current_edge->end_forward_edges();
					
					for( packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->forward_edges(); neighbor_itr != end_forward_neighbor_itr; neighbor_itr = neighbor_itr->next() )
					{
						if( !negative_edges.count(neighbor_itr->edge()->id()) ) ++active_edges;
					}

					const packed_edge_type::Neighbor_Edge* const end_backward_neighbor_itr = current_edge->end_backward_edges();
					
					for( packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->backward_edges(); neighbor_itr != end_backward_neighbor_itr; neighbor_itr = neighbor_itr->next() )
					{
						if( !negative_edges.count(neighbor_itr->edge()->id()) ) ++active_edges;
					}

					unsigned int edge_size = ((sizeof(edge_type) + sizeof(neighbor_type)*active_edges)/__cache_line_size + 1) * __cache_line_size;

					//cout << "Visiting: " << (*itr)->id() << ", active edges = " << active_edges << endl;

					sub_graph_size += edge_size;
				}
			}

			//cout << "Sub Graph Size: " << sub_graph_size << ", original size: " << _graph_size << endl;

			copy->_graph_data = new char[sub_graph_size];
			char* graph_itr = copy->_graph_data;

			if(callback == nullptr)
			{
				for(boost::container::vector<packed_edge_type*>::iterator itr = _edges.begin(); itr != _edges.end(); itr++)
				{
					if( !negative_edges.count((*itr)->id()) )
					{
						packed_edge_type* current_edge = *itr;

						//cout << "Visting edge: " << current_edge->id() << endl;

						copy->_packed_edge_reference[current_edge->id()] = (packed_edge_type*)graph_itr;
						copy->_edges.push_back( (packed_edge_type*)graph_itr );

						// assemble the body of the edge
						memcpy(graph_itr,current_edge,sizeof(packed_edge_type));

						char* edge_itr = graph_itr + sizeof(packed_edge_type);

						unsigned int forward_edges = 0;

						const packed_edge_type::Neighbor_Edge* const end_forward_neighbor_itr = current_edge->end_forward_edges();
						
						for( packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->forward_edges(); neighbor_itr != end_forward_neighbor_itr; neighbor_itr = neighbor_itr->next() )
						{
							if( !negative_edges.count(neighbor_itr->edge()->id()) )
							{
								++forward_edges;
								
								// assemble a forward edge

								//cout << "Copying neighbor: " << neighbor_itr->edge()->id() << endl;

								memcpy( edge_itr, (char*) neighbor_itr, sizeof(neighbor_type) );

								edge_itr += sizeof(neighbor_type);
							}
						}

						((packed_edge_type*)graph_itr)->num_forward_edges(forward_edges);

						unsigned int backward_edges = 0;

						const packed_edge_type::Neighbor_Edge* const end_backward_neighbor_itr = current_edge->end_backward_edges();
						
						for( packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->backward_edges(); neighbor_itr != end_backward_neighbor_itr; neighbor_itr = neighbor_itr->next() )
						{
							if( !negative_edges.count(neighbor_itr->edge()->id()) )
							{
								++backward_edges;

								// assemble a backward edge

								memcpy( edge_itr, (char*) neighbor_itr, sizeof(neighbor_type) );

								edge_itr += sizeof(neighbor_type);
							}
						}

						((packed_edge_type*)graph_itr)->num_backward_edges(backward_edges);

						unsigned int edge_size = ((sizeof(edge_type) + sizeof(neighbor_type)*(forward_edges + backward_edges))/__cache_line_size + 1) * __cache_line_size;

						graph_itr += edge_size;
					}
				}
			}
			else
			{
				for(boost::container::vector<packed_edge_type*>::iterator itr = _edges.begin(); itr != _edges.end(); itr++)
				{
					if( !negative_edges.count((*itr)->id()) )
					{
						packed_edge_type* current_edge = *itr;

						//cout << "Visting edge: " << current_edge->id() << endl;

						copy->_packed_edge_reference[current_edge->id()] = (packed_edge_type*)graph_itr;
						copy->_edges.push_back( (packed_edge_type*)graph_itr );

						// assemble the body of the edge
						memcpy(graph_itr,current_edge,sizeof(packed_edge_type));

						char* edge_itr = graph_itr + sizeof(packed_edge_type);

						unsigned int forward_edges = 0;

						const packed_edge_type::Neighbor_Edge* const end_forward_neighbor_itr = current_edge->end_forward_edges();
						
						for( packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->forward_edges(); neighbor_itr != end_forward_neighbor_itr; neighbor_itr = neighbor_itr->next() )
						{
							if( !negative_edges.count(neighbor_itr->edge()->id()) )
							{
								++forward_edges;
								
								// assemble a forward edge

								//cout << "Copying neighbor: " << neighbor_itr->edge()->id() << endl;

								memcpy( edge_itr, (char*) neighbor_itr, sizeof(neighbor_type) );

								edge_itr += sizeof(neighbor_type);
							}
						}

						((packed_edge_type*)graph_itr)->num_forward_edges(forward_edges);

						unsigned int backward_edges = 0;

						const packed_edge_type::Neighbor_Edge* const end_backward_neighbor_itr = current_edge->end_backward_edges();
						
						for( packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->backward_edges(); neighbor_itr != end_backward_neighbor_itr; neighbor_itr = neighbor_itr->next() )
						{
							if( !negative_edges.count(neighbor_itr->edge()->id()) )
							{
								++backward_edges;

								// assemble a backward edge

								memcpy( edge_itr, (char*) neighbor_itr, sizeof(neighbor_type) );

								edge_itr += sizeof(neighbor_type);
							}
						}

						((packed_edge_type*)graph_itr)->num_backward_edges(backward_edges);

						callback( (packed_edge_type*)graph_itr );
						unsigned int edge_size = ((sizeof(edge_type) + sizeof(neighbor_type)*(forward_edges + backward_edges))/__cache_line_size + 1) * __cache_line_size;

						graph_itr += edge_size;
					}
				}
			}
			
			//cout << "Constructed" << endl;

			for(boost::container::vector<packed_edge_type*>::iterator itr = copy->_edges.begin(); itr != copy->_edges.end(); itr++)
			{
				packed_edge_type* current_edge = *itr;

				//cout << "Visting edge: " << current_edge->id() << "," << current_edge->num_forward_edges() << endl;

				const packed_edge_type::Neighbor_Edge* const end_forward_neighbor_itr = current_edge->end_forward_edges();
				
				for( packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->forward_edges(); neighbor_itr != end_forward_neighbor_itr; neighbor_itr = neighbor_itr->next() )
				{
					

					// contains the original pointer, can use this to establish internal pointer linkage
					int id = neighbor_itr->edge()->id();

					//cout << "Updating neighbor: " << id << endl;

					packed_edge_type* new_neighbor_edge = copy->_packed_edge_reference[id];
					neighbor_itr->edge(new_neighbor_edge);
				}


				const packed_edge_type::Neighbor_Edge* const end_backward_neighbor_itr = current_edge->end_backward_edges();
						
				for( packed_edge_type::Neighbor_Edge* neighbor_itr = current_edge->backward_edges(); neighbor_itr != end_backward_neighbor_itr; neighbor_itr = neighbor_itr->next() )
				{
					// contains the original pointer, can use this to establish internal pointer linkage
					int id = neighbor_itr->edge()->id();

					packed_edge_type* new_neighbor_edge = copy->_packed_edge_reference[id];
					neighbor_itr->edge(new_neighbor_edge);
				}
			}

			//cout << "References Updated" << endl;
		}

		template<typename TargetType>
		TargetType Get_Edge(int id)
		{
			return (TargetType)_packed_edge_reference[id];
		}

		template<typename Input_Edge_Type>
		void Add_Edge(Input_Connected_Edge<Input_Edge_Type>* new_edge)
		{
			if(_ordered_input_edge_reference == nullptr) _ordered_input_edge_reference = new boost::container::vector<void*>();
			if(_input_edge_reference == nullptr) _input_edge_reference = new unordered_map<int, void*>();

			if(_input_edge_reference->count(new_edge->id())) THROW_WARNING("Edge id " << new_edge->id() << " already exists!");

			(*_input_edge_reference)[new_edge->id()] = new_edge;

			_ordered_input_edge_reference->push_back(new_edge);
		}
		
		template<typename Input_Edge_Type>
		Interactive_Graph<output_graph_type>* Compile_Graph(typename edge_construction_callback<Input_Edge_Type, edge_type>::type construction_callback)
		{
			if(!Check_Graph_Consistency<Input_Edge_Type>()){exit(0);}

			Reorder_Graph<Input_Edge_Type>();

			_graph_size = Compute_Graph_Size<Input_Edge_Type>();

			_graph_data = (char*)malloc( _graph_size );

			Arrange_Packed_Edge_Graph<Input_Edge_Type>();

			Construct_Packed_Edge_Graph<Input_Edge_Type>( construction_callback );

			delete _ordered_input_edge_reference;
			delete _input_edge_reference;

			return (Interactive_Graph<output_graph_type>*)this;
		}

		template<typename Input_Edge_Type>
		void Add_Edge(Input_Edge<Input_Edge_Type>* new_edge)
		{
			if(_ordered_input_edge_node_reference == nullptr) _ordered_input_edge_node_reference = new boost::container::vector<void*>();
			if(_input_edge_node_reference == nullptr) _input_edge_node_reference = new unordered_map<int, void*>();

			if(_input_edge_node_reference->count(new_edge->id())) THROW_WARNING("Edge id " << new_edge->id() << " already exists!");

			(*_input_edge_node_reference)[new_edge->id()] = new_edge;

			_ordered_input_edge_node_reference->push_back(new_edge);
		}
		
		template<typename Input_Node_Type>
		void Add_Node(Input_Node<Input_Node_Type>* new_node)
		{
			if(_ordered_input_node_reference == nullptr) _ordered_input_node_reference = new boost::container::vector<void*>();
			if(_input_node_reference == nullptr) _input_node_reference = new unordered_map<int, void*>();

			if(_input_node_reference->count(new_node->id())) THROW_WARNING("Node id " << new_node->id() << " already exists!");

			(*_input_node_reference)[new_node->id()] = new_node;

			_ordered_input_node_reference->push_back(new_node);
		}

		template<typename Input_Edge_Type,typename Input_Node_Type>
		Interactive_Graph<output_graph_type>* Compile_Graph(typename edge_node_construction_callback<Input_Edge_Type, Input_Node_Type, edge_type>::type construction_callback)
		{
			Convert_To_Edge_Based<Input_Edge_Type,Input_Node_Type>();

			if(!Check_Graph_Consistency<Input_Edge_Proxy<MT>>()){exit(0);}

			Reorder_Graph<Input_Edge_Proxy<MT>>();

			_graph_size = Compute_Graph_Size<Input_Edge_Proxy<MT>>();

			_graph_data = (char*)malloc( _graph_size );

			Arrange_Packed_Edge_Graph<Input_Edge_Proxy<MT>>();

			Construct_Packed_Edge_Graph<Input_Edge_Proxy<MT>,Input_Edge_Type,Input_Node_Type>( construction_callback );

			for(boost::container::vector<void*>::iterator itr = _ordered_input_edge_reference->begin();itr!=_ordered_input_edge_reference->end();itr++)
			{
				Input_Edge_Proxy<MT>* edge = (Input_Edge_Proxy<MT>*) *itr;
				delete edge;
			}

			delete _ordered_input_edge_reference;
			delete _input_edge_reference;

			delete _ordered_input_edge_node_reference;
			delete _input_edge_node_reference;
		
			delete _ordered_input_node_reference;
			delete _input_node_reference;


			return (Interactive_Graph<output_graph_type>*)this;
		}

		template<typename Input_Edge_Type,typename Input_Node_Type>
		void Convert_To_Edge_Based()
		{
			unordered_map<int,boost::container::vector<int>> outbound_edge_ids_by_node;
			unordered_map<int,boost::container::vector<int>> inbound_edge_ids_by_node;

			_ordered_input_edge_reference = new boost::container::vector<void*>();
			_input_edge_reference = new unordered_map<int, void*>();
		
			for(boost::container::vector<void*>::iterator itr = _ordered_input_edge_node_reference->begin();itr!=_ordered_input_edge_node_reference->end();itr++)
			{
				Input_Edge<Input_Edge_Type>* edge_node = (Input_Edge<Input_Edge_Type>*) *itr;

				outbound_edge_ids_by_node[edge_node->in_node_id()].push_back(edge_node->id());
				inbound_edge_ids_by_node[edge_node->out_node_id()].push_back(edge_node->id());		
			}
		
		
			for(boost::container::vector<void*>::iterator itr = _ordered_input_edge_node_reference->begin();itr!=_ordered_input_edge_node_reference->end();itr++)
			{
				Input_Edge<Input_Edge_Type>* edge_node = (Input_Edge<Input_Edge_Type>*) *itr;
				Input_Edge_Proxy<MT>* edge = new Input_Edge_Proxy<MT>();

				edge->id<int>(edge_node->id());

				Input_Node<Input_Node_Type>* in_node = (Input_Node<Input_Node_Type>*) ((*_input_node_reference)[edge_node->in_node_id()]);
				Input_Node<Input_Node_Type>* out_node = (Input_Node<Input_Node_Type>*) ((*_input_node_reference)[edge_node->out_node_id()]);

				float pos_x = (in_node->pos_x() + out_node->pos_x())/2.0f;
				float pos_y = (in_node->pos_y() + out_node->pos_y())/2.0f;

				edge->pos_x( pos_x );
				edge->pos_y( pos_y );

				_ordered_input_edge_reference->push_back(edge);
				(*_input_edge_reference)[edge->id<int>()] = edge;


				boost::container::vector<int>* outbound_edge_ids = &(outbound_edge_ids_by_node[out_node->id()]);

				for(boost::container::vector<int>::iterator outbound_itr = outbound_edge_ids->begin();outbound_itr!=outbound_edge_ids->end();outbound_itr++)
				{
					edge->_forward_edge_ids.push_back( *outbound_itr );
				}


				/* If user desires a backward graph */

				//vector<int>* inbound_edge_ids = &(inbound_edge_ids_by_node[out_node->id()]);

				//for(vector<int>::iterator inbound_itr = inbound_edge_ids->begin();inbound_itr!=inbound_edge_ids->end();inbound_itr++)
				//{
				//	edge->backward_edge_ids()->push_back( *inbound_itr );
				//}
			}
		}

		template<typename Input_Edge_Type>
		bool Check_Graph_Consistency()
		{
			bool status = true;
		
			for(unordered_map<int,void*>::iterator itr = _input_edge_reference->begin(); itr != _input_edge_reference->end(); itr++)
			{
				Sequence<typename Input_Edge_Type::forward_edge_ids_type,int>* forward_edges = ((Input_Connected_Edge<Input_Edge_Type>*)itr->second)->forward_edge_ids();

				for(Sequence<typename Input_Edge_Type::forward_edge_ids_type,int>::iterator neighbor_itr = forward_edges->begin(); neighbor_itr != forward_edges->end(); neighbor_itr++)
				{
					if(!_input_edge_reference->count( *neighbor_itr ))
					{
						THROW_EXCEPTION("Edge " << itr->first << " has an unavailable forward connection: " << *neighbor_itr);
						status = false;
					}
				}

				Sequence<typename Input_Edge_Type::backward_edge_ids_type,int>* backward_edges = ((Input_Connected_Edge<Input_Edge_Type>*)itr->second)->backward_edge_ids();

				for(Sequence<typename Input_Edge_Type::backward_edge_ids_type,int>::iterator neighbor_itr = backward_edges->begin(); neighbor_itr != backward_edges->end(); neighbor_itr++)
				{
					if(!_input_edge_reference->count( *neighbor_itr ))
					{
						THROW_EXCEPTION("Edge " << itr->first << " has an unavailable backward connection: " << *neighbor_itr);
						status = false;
					}
				}
			}

			return status;
		}
		
		template<typename Input_Edge_Type>
		float Compute_Fitness(boost::container::vector<Input_Connected_Edge<Input_Edge_Type>*>* edges)
		{
			boost::unordered_map< int, Input_Connected_Edge<Input_Edge_Type>** > edge_address_map;

			for(boost::container::vector<Input_Connected_Edge<Input_Edge_Type>*>::iterator itr = edges->begin();itr!=edges->end();itr++)
			{
				edge_address_map[(*itr)->id()] = &(*itr);
			}

			float fitness = 0.0f;

			Sequence<typename Input_Edge_Type::forward_edge_ids_type,int>* forward_edge_ids;

			for(boost::container::vector<Input_Connected_Edge<Input_Edge_Type>*>::iterator itr = edges->begin();itr!=edges->end();itr++)
			{
				forward_edge_ids = (*itr)->forward_edge_ids();

				for( Sequence<typename Input_Edge_Type::forward_edge_ids_type,int>::iterator fwd_itr = forward_edge_ids->begin();fwd_itr!=forward_edge_ids->end();fwd_itr++ )
				{
					fitness += abs(edge_address_map[(*itr)->id()] - edge_address_map[(*fwd_itr)]);
				}
			}

			return fitness;
		}

		template<typename Input_Edge_Type>
		void Reorder_Graph()
		{
			int num_layers = 4;

			cout << "Input Fitness: " << Compute_Fitness( (boost::container::vector<Input_Connected_Edge<Input_Edge_Type>*>*)_ordered_input_edge_reference ) << endl;
		
			boost::container::vector< Input_Connected_Edge<Input_Edge_Type>* > reordered_edges;

			Reorder_Layer<Input_Edge_Type,2,5>(num_layers, (boost::container::vector<Input_Connected_Edge<Input_Edge_Type>*>*)_ordered_input_edge_reference, &reordered_edges);
		
			cout << "Updated Fitness: " << Compute_Fitness( &reordered_edges ) << endl;

			_ordered_input_edge_reference->clear();

			for(boost::container::vector< Input_Connected_Edge<Input_Edge_Type>* >::iterator itr = reordered_edges.begin();itr!=reordered_edges.end();itr++)
			{
				_ordered_input_edge_reference->push_back(*itr);
			}
		}

		template<typename Input_Edge_Type,const int num_means,const int clustering_iterations>
		void Reorder_Layer(int layer_depth, boost::container::vector< Input_Connected_Edge<Input_Edge_Type>* >* in_edges, boost::container::vector< Input_Connected_Edge<Input_Edge_Type>* >* out_reordered_edges)
		{
			struct Parition_Workspace
			{
				struct Point
				{
					Point():x(0.0),y(0.0){}

					float x;
					float y;
				};

				Point centroid;
				Point position_sum;

				boost::container::vector<Input_Connected_Edge<Input_Edge_Type>*> member_edges;
			};

			int current_layer = layer_depth - 1;

			if(current_layer < 0 || in_edges->size() < num_means)
			{
				for(boost::container::vector< Input_Connected_Edge<Input_Edge_Type>* >::iterator itr = in_edges->begin();itr!=in_edges->end();itr++)
				{
					out_reordered_edges->push_back(*itr);
				}

				//cout << "returning layer: " << current_layer << endl;

				return;
			}

			//cout << "reordering layer: " << current_layer << endl;

			Parition_Workspace partitions[num_means];

			for(boost::container::vector<Input_Connected_Edge<Input_Edge_Type>*>::iterator itr=in_edges->begin();itr!=in_edges->end();itr++)
			{
				int label = rand()%num_means;

				partitions[label].member_edges.push_back( *itr );

				partitions[label].position_sum.x += (*itr)->pos_x();
				partitions[label].position_sum.y += (*itr)->pos_y();
			}

			for(int j=0;j<num_means;j++)
			{
				partitions[j].centroid.x = partitions[j].position_sum.x / ((float)partitions[j].member_edges.size());
				partitions[j].centroid.y = partitions[j].position_sum.y / ((float)partitions[j].member_edges.size());
			}

			for(int i=0; i < clustering_iterations; i++)
			{
				//cout << "\tclustering iteration: " << i << endl;

				for(int j=0; j < num_means; j++)
				{
					partitions[j].position_sum.x = 0;
					partitions[j].position_sum.y = 0;

					partitions[j].member_edges.clear();
				}

				for(boost::container::vector<Input_Connected_Edge<Input_Edge_Type>*>::iterator itr=in_edges->begin();itr!=in_edges->end();itr++)
				{
					float best_dist_sq = FLT_MAX;
					int label = -1;

					for(int i=0;i<num_means;i++)
					{
						float dist_sq = ((*itr)->pos_x() - partitions[i].centroid.x)*((*itr)->pos_x() - partitions[i].centroid.x) + ((*itr)->pos_y() - partitions[i].centroid.y)*((*itr)->pos_y() - partitions[i].centroid.y);

						if(dist_sq < best_dist_sq) 
						{
							best_dist_sq = dist_sq;
							label = i;
						}
					}

					//cout << "\t\t\tedge: " << (*itr)->id() << " labeled " << label << endl;

					partitions[label].member_edges.push_back( *itr );

					partitions[label].position_sum.x += (*itr)->pos_x();
					partitions[label].position_sum.y += (*itr)->pos_y();
				}

				for(int j=0;j<num_means;j++)
				{
					partitions[j].centroid.x = partitions[j].position_sum.x / ((float)partitions[j].member_edges.size());
					partitions[j].centroid.y = partitions[j].position_sum.y / ((float)partitions[j].member_edges.size());

					//cout << "\t\tparition: " << j << " position: (" << partitions[j].centroid.x << "," << partitions[j].centroid.y << ")" << endl;
				}
			}

			boost::container::vector< Input_Connected_Edge<Input_Edge_Type>* > reordered_edges;

			for(int i=0;i<num_means;i++)
			{
				Reorder_Layer<Input_Edge_Type,num_means,clustering_iterations>(current_layer,&partitions[i].member_edges,&reordered_edges);

				for(boost::container::vector< Input_Connected_Edge<Input_Edge_Type>* >::iterator itr = reordered_edges.begin();itr!=reordered_edges.end();itr++)
				{
					out_reordered_edges->push_back(*itr);
				}

				reordered_edges.clear();
			}

			//cout << "returning layer: " << current_layer << endl;
		}

		template<typename Input_Edge_Type>
		unsigned int Compute_Graph_Size()
		{
			unsigned int new_graph_size = 0;

			for(boost::container::vector<void*>::iterator itr=_ordered_input_edge_reference->begin();itr!=_ordered_input_edge_reference->end();itr++)
			{
				unsigned int edge_size = ((sizeof(edge_type) + sizeof(neighbor_type)*(((Input_Connected_Edge<Input_Edge_Type>*) *itr)->forward_edge_ids()->size()) + sizeof(neighbor_type)*(((Input_Connected_Edge<Input_Edge_Type>*) *itr)->backward_edge_ids()->size()))/__cache_line_size + 1)*__cache_line_size;

				new_graph_size += edge_size;
			}

			return new_graph_size;
		}

		template<typename Input_Edge_Type>
		void Arrange_Packed_Edge_Graph()
		{
			char* graph_itr = _graph_data;

			for(boost::container::vector<void*>::iterator itr=_ordered_input_edge_reference->begin();itr!=_ordered_input_edge_reference->end();itr++)
			{
				unsigned int edge_size = ((sizeof(edge_type) + sizeof(neighbor_type)*(((Input_Connected_Edge<Input_Edge_Type>*) *itr)->forward_edge_ids()->size()) + sizeof(neighbor_type)*(((Input_Connected_Edge<Input_Edge_Type>*) *itr)->backward_edge_ids()->size()))/__cache_line_size + 1)*__cache_line_size;

				_packed_edge_reference[((Input_Connected_Edge<Input_Edge_Type>*) *itr)->id()] = (packed_edge_type*)graph_itr;

				_edges.push_back( (packed_edge_type*)graph_itr );

				graph_itr += edge_size;
			}
		}

		template<typename Input_Edge_Type>
		void Construct_Packed_Edge_Graph(typename edge_construction_callback<Input_Edge_Type, edge_type>::type construction_callback)
		{
			char* graph_itr = _graph_data;

			for(boost::container::vector<void*>::iterator itr=_ordered_input_edge_reference->begin();itr!=_ordered_input_edge_reference->end();itr++)
			{
				unsigned int edge_size = ((sizeof(edge_type) + sizeof(neighbor_type)*(((Input_Connected_Edge<Input_Edge_Type>*) *itr)->forward_edge_ids()->size()) + sizeof(neighbor_type)*(((Input_Connected_Edge<Input_Edge_Type>*) *itr)->backward_edge_ids()->size()))/__cache_line_size + 1)*__cache_line_size;


				edge_type* edge_memory = (edge_type*)graph_itr;

				new (edge_memory) edge_type(((Input_Connected_Edge<Input_Edge_Type>*) *itr)->forward_edge_ids()->size(),((Input_Connected_Edge<Input_Edge_Type>*) *itr)->backward_edge_ids()->size());
				
				edge_memory->_id = ((Input_Connected_Edge<Input_Edge_Type>*) *itr)->id();

				

				neighbor_type* edge_itr;
			
				edge_itr = edge_memory->forward_edges();

				Sequence<typename Input_Edge_Type::forward_edge_ids_type,int>* forward_edges = ((Input_Connected_Edge<Input_Edge_Type>*) *itr)->forward_edge_ids();

				for(Sequence<typename Input_Edge_Type::forward_edge_ids_type,int>::iterator neighbor_itr = forward_edges->begin(); neighbor_itr != forward_edges->end(); neighbor_itr++, edge_itr++)
				{
					edge_itr->edge( (edge_type*)_packed_edge_reference[(*neighbor_itr)] );
				}

				
				edge_itr = edge_memory->backward_edges();

				Sequence<typename Input_Edge_Type::backward_edge_ids_type,int>* backward_edges = ((Input_Connected_Edge<Input_Edge_Type>*) *itr)->backward_edge_ids();

				for(Sequence<typename Input_Edge_Type::backward_edge_ids_type,int>::iterator neighbor_itr = backward_edges->begin(); neighbor_itr != backward_edges->end(); neighbor_itr++, edge_itr++)
				{
					edge_itr->edge( (edge_type*)_packed_edge_reference[(*neighbor_itr)] );
				}



				construction_callback( (Input_Edge_Type*)((Input_Connected_Edge<Input_Edge_Type>*) *itr), edge_memory  );

				
				
				graph_itr += edge_size;
			}
		}

		template<typename Input_Edge_Type,typename Input_Edge_Node_Type,typename Input_Node_Type>
		void Construct_Packed_Edge_Graph(typename edge_node_construction_callback<Input_Edge_Node_Type, Input_Node_Type, edge_type>::type construction_callback)
		{
			char* graph_itr = _graph_data;

			for(boost::container::vector<void*>::iterator itr=_ordered_input_edge_reference->begin();itr!=_ordered_input_edge_reference->end();itr++)
			{
				unsigned int edge_size = ((sizeof(edge_type) + sizeof(neighbor_type)*(((Input_Connected_Edge<Input_Edge_Type>*) *itr)->forward_edge_ids()->size()) + sizeof(neighbor_type)*(((Input_Connected_Edge<Input_Edge_Type>*) *itr)->backward_edge_ids()->size()))/__cache_line_size + 1)*__cache_line_size;


				edge_type* edge_memory = (edge_type*)graph_itr;

				new (edge_memory) edge_type(((Input_Connected_Edge<Input_Edge_Type>*) *itr)->forward_edge_ids()->size(),((Input_Connected_Edge<Input_Edge_Type>*) *itr)->backward_edge_ids()->size());
			
				edge_memory->_id = ((Input_Connected_Edge<Input_Edge_Type>*) *itr)->id();

			

				neighbor_type* edge_itr;
			
				edge_itr = edge_memory->forward_edges();

				Sequence<typename Input_Edge_Type::forward_edge_ids_type,int>* forward_edges = ((Input_Connected_Edge<Input_Edge_Type>*) *itr)->forward_edge_ids();

				for(Sequence<typename Input_Edge_Type::forward_edge_ids_type,int>::iterator neighbor_itr = forward_edges->begin(); neighbor_itr != forward_edges->end(); neighbor_itr++, edge_itr++)
				{
					edge_itr->edge( (edge_type*)_packed_edge_reference[(*neighbor_itr)] );
				}

			
				edge_itr = edge_memory->backward_edges();

				Sequence<typename Input_Edge_Type::backward_edge_ids_type,int>* backward_edges = ((Input_Connected_Edge<Input_Edge_Type>*) *itr)->backward_edge_ids();

				for(Sequence<typename Input_Edge_Type::backward_edge_ids_type,int>::iterator neighbor_itr = backward_edges->begin(); neighbor_itr != backward_edges->end(); neighbor_itr++, edge_itr++)
				{
					edge_itr->edge( (edge_type*)_packed_edge_reference[(*neighbor_itr)] );
				}

				Input_Edge<Input_Edge_Node_Type>* edge_node = (Input_Edge<Input_Edge_Node_Type>*) (*_input_edge_node_reference)[((Input_Connected_Edge<Input_Edge_Type>*) *itr)->id()];
				Input_Node<Input_Node_Type>* in_node = (Input_Node<Input_Node_Type>*) (*_input_node_reference)[edge_node->in_node_id()];
				Input_Node<Input_Node_Type>* out_node = (Input_Node<Input_Node_Type>*) (*_input_node_reference)[edge_node->out_node_id()];

				construction_callback( (Input_Edge_Node_Type*)edge_node, (Input_Node_Type*)in_node, (Input_Node_Type*)out_node, edge_memory  );

			
			
				graph_itr += edge_size;
			}
		}

		unsigned int _graph_size;
		char* _graph_data;

		m_data(bool,compiled,NONE,NONE);


		boost::container::vector<void*>* _ordered_input_edge_reference;
		unordered_map<int, void*>* _input_edge_reference;
	
		boost::container::vector<void*>* _ordered_input_edge_node_reference;
		unordered_map<int, void*>* _input_edge_node_reference;

		boost::container::vector<void*>* _ordered_input_node_reference;
		unordered_map<int, void*>* _input_node_reference;

		typedef Packed_Edge_Implementation<typename edge_type::attributes_type,typename edge_type::neighbor_attributes_type> packed_edge_type;

		m_container(boost::container::vector<packed_edge_type*>,edges,NONE,NONE);
		unordered_map<int, packed_edge_type*> _packed_edge_reference;
	};
}
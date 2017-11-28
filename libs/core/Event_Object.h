#pragma once
///----------------------------------------------------------------------------------------------------
/// Event_Object.h - Top level event object which records revision, and memory information
///----------------------------------------------------------------------------------------------------

#include "Execution_Object.h"

namespace polaris
{
	struct ibh_node
	{
		ibh_node* left_child;
		ibh_node* right_child;
		ibh_node* parent;
		ibh_node* row_left;
		ibh_node* row_right;
		int row;
		char mark;
	};

	template<typename T>
	struct Intrusive_Binary_Heap
	{
		Intrusive_Binary_Heap()
		{
			heap_row = nullptr;
			heap_row_itr = nullptr;
			next_heap_row = nullptr;
			next_heap_row_itr = nullptr;
			heap_front = nullptr;
			nrows = 0;
		}

		int nrows;

		ibh_node* heap_row;
		ibh_node* next_heap_row;

		ibh_node* heap_row_itr;
		ibh_node* next_heap_row_itr;

		ibh_node* heap_front;

		void swap_nodes(ibh_node* const a, ibh_node* const b)
		{		
			//if(a->row < b->row)
			//{
			//	// always model as a swap up from a to b
			//	ibh_node* tmp = a;
			//	a = b;
			//	b = tmp;
			//}

			const bool parent_swap = (a->parent == b);

			const bool neighbor_swap = (a->row_left == b);

			// cache val
			ibh_node* const a_tmp = a;
			ibh_node* const a_parent_tmp = a->parent;
		
			ibh_node* const a_left_child_tmp = a->left_child;
			ibh_node* const a_right_child_tmp = a->right_child;
		
			ibh_node* const a_row_left_tmp = a->row_left;
			ibh_node* const a_row_right_tmp = a->row_right;

			const int a_row_tmp = a->row;
			const char a_mark_tmp = a->mark;

			// update val's perspective

			// swap differently depending on whether you were a right or left child

			if(parent_swap)
			{
				if(a_mark_tmp == 'L')
				{
					a->left_child = b;
					a->right_child = b->right_child;
				
					if(a->right_child != nullptr) a->right_child->parent = a;
				}
				else
				{
					a->left_child = b->left_child;
					a->right_child = b;

					if(a->left_child != nullptr) a->left_child->parent = a;
				}
			}
			else
			{
				a->left_child = b->left_child;
				b->left_child->parent = a;
				a->right_child = b->right_child;
				b->right_child->parent = a;
			}

			// a occupies the same row position as did b

			if(!neighbor_swap)
			{
				// link row_left
				a->row_left = b->row_left;
				if(a->row_left != nullptr) a->row_left->row_right = a;
		
				// link row_right
				a->row_right = b->row_right;
				if(a->row_right != nullptr) a->row_right->row_left = a;
			}
			else
			{
				a->row_left = b->row_left;
				if(a->row_left != nullptr) a->row_left->row_right = a;
				a->row_right = b;
			
				b->row_right = a_row_right_tmp;
				if(b->row_right != nullptr) b->row_right->row_left = b;
				b->row_left = a;
			}

			// update row index
			a->row = b->row;

			// maintain the heap row structure

			if(b == heap_row) heap_row = a;
			else if(a == heap_row) heap_row = b;
		
			if(b == heap_row_itr) heap_row_itr = a;
			else if(a == heap_row_itr) heap_row_itr = b;

			if(b->parent != nullptr)
			{
				// update grandparent's perspective if you aren't moving to the top of the heap

				// determine whether b was left or right child

				if(b->mark == 'L')
				{
					b->parent->left_child = a;
				}
				else
				{
					b->parent->right_child = a;
				}

				a->mark = b->mark;
			}
			else
			{
				// you are at the top of the heap
				a->mark = 'N';
				heap_front = a;
			}
		
			a->parent = b->parent;
		

			// update parent's perspective

			// a's children, parent, and mark transfer directly to parent

			b->left_child = a_left_child_tmp;
			b->right_child = a_right_child_tmp;
			b->row = a_row_tmp;
			b->mark = a_mark_tmp;
		
			if(a_left_child_tmp!=nullptr) a_left_child_tmp->parent = b;
			if(a_right_child_tmp!=nullptr) a_right_child_tmp->parent = b;

			// parent occupies the same row position as a did

			if(!neighbor_swap)
			{
				b->row_left = a_row_left_tmp;
				b->row_left->row_right = b;
				b->row_right = a_row_right_tmp;
				if(b->row_right != nullptr) b->row_right->row_left = b;
			}

			// maintain the heap row structure

			if(a_tmp == next_heap_row) next_heap_row = b;
			if(a_tmp == next_heap_row_itr) next_heap_row_itr = b;

			// update grandparent's perspective for b

			// determine whether a was left or right child

			if(!parent_swap)
			{
				if(a_mark_tmp == 'L')
				{
					a_parent_tmp->left_child = b;
				}
				else
				{
					a_parent_tmp->right_child = b;
				}

				b->parent = a_parent_tmp;
			}
			else
			{
				b->parent = a;
			}
		}

		void heapify_up(ibh_node* val)
		{
			while(true)
			{
				if(val->parent != nullptr && *((T*)val->parent) > *((T*)val))
				{
					swap_nodes(val,val->parent);
				}
				else
				{
					break;
				}
			}
		}
	
		void heapify_down(ibh_node* val)
		{
			while(true)
			{
				bool swap_left = false;
				bool swap_right = false;
				bool swap_result = false;

				if(val->left_child != nullptr)
				{
					swap_left = (*((T*)val->left_child) < *((T*)val));
				}
			
				if(val->right_child != nullptr)
				{
					swap_right = (*((T*)val->right_child) < *((T*)val));
				}

				if(swap_right && swap_left)
				{
					swap_right = (*((T*)val->right_child) < *((T*)val->left_child));
				}

				if(swap_right)
				{
					swap_nodes(val->right_child,val);
				}
				else if(swap_left)
				{
					swap_nodes(val->left_child,val);
				}
				else
				{
					break;
				}
			}
		}

		ibh_node* extract_min()
		{
			ibh_node* ret = heap_front;

			if(next_heap_row != nullptr && heap_front != next_heap_row_itr)
			{
				// swap the bottom with the top
				swap_nodes(next_heap_row_itr,heap_front);

				// disconnect the bottom

				// from parent
				if(next_heap_row_itr->mark == 'L')
				{
					next_heap_row_itr->parent->left_child = nullptr;
					// update heap_row_itr
					heap_row_itr = heap_row_itr->row_left;
				}
				else
				{
					next_heap_row_itr->parent->right_child = nullptr;
				}

				// from row list

				if(next_heap_row_itr->row != next_heap_row_itr->row_left->row)
				{
					--nrows;
					next_heap_row = heap_row;
					heap_row = heap_row->parent;
				}

				next_heap_row_itr->row_left->row_right = nullptr;

				next_heap_row_itr = next_heap_row_itr->row_left;

				heapify_down(heap_front);
			}
			else if(heap_front != nullptr)
			{
				next_heap_row_itr = nullptr;
				next_heap_row = nullptr;
				heap_row_itr = nullptr;
				heap_row = nullptr;

				heap_front = nullptr;

				--nrows;
			}

			return ret;
		}

		void insert(ibh_node* val)
		{
			val->left_child = nullptr;
			val->right_child = nullptr;
			val->parent = nullptr;
			val->row_left = nullptr;
			val->row_right = nullptr;
			val->mark = 'N';
			val->row = -1;


			// determine where to insert the node
		
			if(heap_row != nullptr)
			{
				// first, detect whether this is the first next heap row, take care of all operations
				if(next_heap_row != nullptr)
				{
					// determine whether there is space in the row

					if(heap_row_itr->right_child != nullptr)
					{
						// this node is full, we need to get a fresh one

						if(heap_row_itr->row_right->row == heap_row_itr->row)
						{
							// yes, there is more row, move the iterator right
							heap_row_itr = heap_row_itr->row_right;
						}
						else
						{
							// this row is full
							nrows++;

							// move to the next row
							heap_row = next_heap_row;
							heap_row_itr = heap_row;

							// this heap row being full implies next heap row being full
							// don't set the next_heap_itr, it will be resolved when the next_heap_itr is updated
							next_heap_row = val;
						}
					}

					// insert it in the next available space

					if(heap_row_itr->left_child == nullptr)
					{
						heap_row_itr->left_child = val;
						val->mark = 'L';
					}
					else
					{
						heap_row_itr->right_child = val;
						val->mark = 'R';
					}

					val->row = nrows;
					val->parent = heap_row_itr;

					// always insert into the next heap row list
					next_heap_row_itr->row_right = val;
					val->row_left = next_heap_row_itr;
					next_heap_row_itr = val;
				}
				else
				{
					// add a row
					nrows++;

					// val is second row
					val->row = nrows;

					// link heap row to val
					heap_row_itr->row_right = val;
					val->row_left = heap_row_itr;

					// attach as left child
					heap_row_itr->left_child = val;
					val->parent = heap_row_itr;
					val->mark = 'L';

					// initialize next heap row

					next_heap_row_itr = val;
					next_heap_row = val;

				}
			}
			else
			{
				// first element
				nrows = 1;
				heap_row = val;
				val->mark = 'N';
				val->row = 1;
				val->parent = nullptr;
				heap_front = val;
				heap_row_itr = val;
				next_heap_row_itr = nullptr;
				next_heap_row = nullptr;
			}

			heapify_up(val);
		}
	};


	template<typename T>
	class BinaryHeap
	{
		private:
			std::vector<T*> heap;
			int left(int parent);
			int right(int parent);
			int parent(int child);
			void heapifyup(int index);
			void heapifydown(int index);
		public:
			BinaryHeap()
			{}
			void Insert(T* element);
			void DeleteMin();
			T* ExtractMin();
	};

	template<typename T>
	void BinaryHeap<T>::Insert(T* element)
	{
		heap.push_back(element);
		heapifyup(heap.size() -1);
	}

	template<typename T>
	void BinaryHeap<T>::DeleteMin()
	{
		if (heap.size() == 0)
		{
			//cout<<"Heap is Empty"<<endl;
			return;
		}
		heap[0] = heap.at(heap.size() - 1);
		heap.pop_back();
		heapifydown(0);
		//cout<<"Element Deleted"<<endl;
	}

	template<typename T>
	T* BinaryHeap<T>::ExtractMin()
	{
		if (heap.size() == 0)
		{
			return nullptr;
		}
		else
		{
			return heap.front();
		}
	}

	template<typename T>
	int BinaryHeap<T>::left(int parent)
	{
		int l = 2 * parent + 1;
		if (l < heap.size())
			return l;
		else
			return -1;
	}

	template<typename T>
	int BinaryHeap<T>::right(int parent)
	{
		int r = 2 * parent + 2;
		if (r < heap.size())
			return r;
		else
			return -1;
	}

	template<typename T>
	int BinaryHeap<T>::parent(int child)
	{
		int p = (child - 1)/2;
		if (child == 0)
			return -1;
		else
			return p;
	}

	template<typename T>
	void BinaryHeap<T>::heapifyup(int in)
	{
		if (in >= 0 && parent(in) >= 0 && *heap[parent(in)] > *heap[in])
		{
			T* temp = heap[in];
			heap[in] = heap[parent(in)];
			heap[parent(in)] = temp;
			heapifyup(parent(in));
		}
	}

	template<typename T>
	void BinaryHeap<T>::heapifydown(int in)
	{
 
		int child = left(in);
		int child1 = right(in);
    
		if (child >= 0 && child1 >= 0 && *heap[child] > *heap[child1])
		{
		   child = child1;
		}

		if (child > 0)
		{
			T* temp = heap[in];
			heap[in] = heap[child];
			heap[child] = temp;
			heapifydown(child);
		}
	}

	// Forward Declaration of Event_Block
	class Event_Block;

	///----------------------------------------------------------------------------------------------------
	/// Event_Object - Object which records revision, and memory information
	///----------------------------------------------------------------------------------------------------

	class Event_Object : public ibh_node, public Base_Object
	{
	public:

		friend bool operator< (const Event_Object &a, const Event_Object &b)
			{  return a._next_revision._revision < b._next_revision._revision;  }
		friend bool operator> (const Event_Object &a, const Event_Object &b)
			{  return a._next_revision._revision > b._next_revision._revision;  }
		friend bool operator== (const Event_Object &a, const Event_Object &b)
			{  return a._next_revision._revision == b._next_revision._revision;  }

		Event_Object():Base_Object(){ _next_revision = __revision_omega; }

		Event_Object(size_t component_id,int pid):Base_Object(component_id, pid){ _next_revision = __revision_omega; }
		
		//----------------------------------------------------------------------------------------------------
		// Simulation functions
		//----------------------------------------------------------------------------------------------------

		__forceinline void Swap_Event(Event new_event){ _event_callback = new_event; }

		template<typename ComponentType>
		void Load_Event(void (*p_event)(ComponentType*,Event_Response&),int start_iteration,int start_sub_iteration);

		template<typename ComponentType>
		void Reschedule(int start_iteration,int start_sub_iteration);

		inline bool Visiting() const{ return _next_revision == revision(); }

		inline const Revision& next_revision() const{ return _next_revision; }

		//----------------------------------------------------------------------------------------------------
		// Memory-related functions
		//----------------------------------------------------------------------------------------------------

		inline Event_Block* execution_block(void) const{ return _execution_block; }
		
		//boost::intrusive::set_member_hook<> _events_hook;

	private:
		friend class Event_Block;

		

		#ifdef SAFE_MODE
			_lock _optex_lock;
		#endif
		
		Revision _next_revision;

		union
		{
			Event _event_callback;
			Byte* _next_free_cell;
		};

		Event_Block* _execution_block;
	};
}
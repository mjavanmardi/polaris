#pragma once
///----------------------------------------------------------------------------------------------------
/// Event_Object.h - Top level event object which records revision, and memory information
///----------------------------------------------------------------------------------------------------

#include "Execution_Object.h"

namespace polaris
{
	struct node {
		node* prev;
		node* next;
		node* child;
		node* parent;
		int degree;
		bool marked;
	};
	
	template <class V> struct FibonacciHeap {
		node* heap;

		FibonacciHeap() {
			heap=_empty();
		}

		node* insert(V& value) {
			node* n = (node*)&value;
			n->prev=n->next=n;
			n->degree=0;
			n->marked=false;
			n->child=NULL;
			n->parent=NULL;
			heap=_merge(heap,n);
			return n;
		}

		bool isEmpty() {
			return heap==NULL;
		}

		V& getMinimum() {
			return (V&)*heap;
		}

		V& removeMinimum() {
			node* old=heap;
			heap=_removeMinimum(heap);
			return *((V*)old);
		}

		node* _empty() {
			return NULL;
		}

		node* _merge(node* a,node* b) {
			if(a==NULL)return b;
			if(b==NULL)return a;
			if(*((V*)a)>*((V*)b)) {
				node* temp=a;
				a=b;
				b=temp;
			}
			node* an=a->next;
			node* bp=b->prev;
			a->next=b;
			b->prev=a;
			an->prev=bp;
			bp->next=an;
			return a;
		}

		void _addChild(node* parent,node* child) {
			child->prev=child->next=child;
			child->parent=parent;
			parent->degree++;
			parent->child=_merge(parent->child,child);
		}

		void _unMarkAndUnParentAll(node* n) {
			if(n==NULL)return;
			node* c=n;
			do {
				c->marked=false;
				c->parent=NULL;
				c=c->next;
			}while(c!=n);
		}

		node* _removeMinimum(node* n) {
			_unMarkAndUnParentAll(n->child);
			if(n->next==n) {
				n=n->child;
			} else {
				n->next->prev=n->prev;
				n->prev->next=n->next;
				n=_merge(n->next,n->child);
			}
			if(n==NULL)return n;
			node* trees[64]={NULL};

			while(true) {
				if(trees[n->degree]!=NULL) {
					node* t=trees[n->degree];
					if(t==n)break;
					trees[n->degree]=NULL;
					if(*((V*)n)<*((V*)t)) {
						t->prev->next=t->next;
						t->next->prev=t->prev;
						_addChild(n,t);
					} else {
						t->prev->next=t->next;
						t->next->prev=t->prev;
						if(n->next==n) {
							t->next=t->prev=t;
							_addChild(t,n);
							n=t;
						} else {
							n->prev->next=t;
							n->next->prev=t;
							t->next=n->next;
							t->prev=n->prev;
							_addChild(t,n);
							n=t;
						}
					}
					continue;
				} else {
					trees[n->degree]=n;
				}
				n=n->next;
			}
			node* min=n;
			do {
				if(*((V*)n)<*((V*)min)) min=n;
				n=n->next;
			} while(n!=n);
			return min;
		}
	};

	// Forward Declaration of Event_Block
	class Event_Block;

	///----------------------------------------------------------------------------------------------------
	/// Event_Object - Object which records revision, and memory information
	///----------------------------------------------------------------------------------------------------

	class Event_Object : public Base_Object
	{
	public:

		friend bool operator< (const Event_Object &a, const Event_Object &b)
			{  return a._next_revision._revision < b._next_revision._revision;  }
		friend bool operator> (const Event_Object &a, const Event_Object &b)
			{  return a._next_revision._revision > b._next_revision._revision;  }
		friend bool operator== (const Event_Object &a, const Event_Object &b)
			{  return a._next_revision._revision == b._next_revision._revision;  }

		Event_Object():Base_Object(){ _next_revision = __revision_omega; }

		Event_Object(size_t component_id,int uuid):Base_Object(component_id,uuid){ _next_revision = __revision_omega; }
		
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
		
		boost::intrusive::set_member_hook<> _events_hook;

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
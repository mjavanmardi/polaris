#pragma once
///----------------------------------------------------------------------------------------------------
/// Event_Object.h - Top level event object which records revision, and memory information
///----------------------------------------------------------------------------------------------------

#include "Execution_Object.h"

namespace polaris
{	/*
	 * Node Declaration
	 */
	struct node
	{
		int degree;
		node* parent;
		node* child;
		node* left;
		node* right;
		char mark;
		char C;
	};
	/*
	 * Class Declaration
	 */
	template<typename T>
	class FibonacciHeap
	{
		private:
			int nH;
			T* H;
		public:
			T* initialize();
			int fibonnaci_link(T*, T*, T*);
			T* insert(T*, T*);
			T* extract_min(T*);
			int consolidate(T*);
        
			FibonacciHeap()
			{
				H = initialize();
			}
	};

	template<typename T>
	T* FibonacciHeap<T>::initialize()
	{
		nH = 0;
		T* np;
		np = NULL;
		return np;
	}

	template<typename T>
	T* FibonacciHeap<T>::insert(T* H, T* x)
	{
		x->degree = 0;
		x->parent = NULL;
		x->child = NULL;
		x->left = x;
		x->right = x;
		x->mark = 'F';
		x->C = 'N';
		if (H != NULL)
		{
			(H->left)->right = x;
			x->right = H;
			x->left = H->left;
			H->left = x;
			if ((*x) < (*H))
				H = x;
		}
		else
		{
			H = x;
		}
		nH = nH + 1;
		return H;
	}

	template<typename T>
	int FibonacciHeap<T>::fibonnaci_link(T* H1, T* y, T* z)
	{
		(y->left)->right = y->right;
		(y->right)->left = y->left;
		if (z->right == z)
			H1 = z;
		y->left = y;
		y->right = y;
		y->parent = z;
		if (z->child == NULL)
			z->child = y;
		y->right = z->child;
		y->left = (z->child)->left;
		((z->child)->left)->right = y;
		(z->child)->left = y;
		if ((*y) < (*(T*)(z->child)))
			z->child = y;
		z->degree++;

		return 1;
	}

	template<typename T>
	T* FibonacciHeap<T>::extract_min(T* H1)
	{
		T* p;
		T* ptr;
		T* z = H1;
		p = z;
		ptr = z;
		if (z == NULL)
			return z;
		T* x;
		T* np;
		x = NULL;
		if (z->child != NULL)
			x = (T*)z->child;
		if (x != NULL)
		{
			ptr = x;
			do
			{
				np = (T*)x->right;
				(H1->left)->right = x;
				x->right = H1;
				x->left = H1->left;
				H1->left = x;
				if ((*x) < (*H1))
					H1 = x;
				x->parent = NULL;
				x = np;
			}
			while (np != ptr);
		}
		(z->left)->right = z->right;
		(z->right)->left = z->left;
		H1 = (T*)z->right;
		if (z == z->right && z->child == NULL)
			H = NULL;
		else
		{
			H1 = (T*)z->right;
			consolidate(H1);
		}
		nH = nH - 1;
		return p;
	}

	template<typename T>
	int FibonacciHeap<T>::consolidate(T* H1)
	{
		int d, i;
		float f = (log((float)nH)) / (log((float)2));
		int D = (int)f;
		T** A = (T**)alloca((D+1)*sizeof(T*));
		for (i = 0; i <= D; i++)
			A[i] = NULL;
		T* x = H1;
		T* y;
		T* np;
		T* pt = x;
		do
		{
			pt = (T*)pt->right;
			d = x->degree;
			while (A[d] != NULL)
			{
				y = A[d];
				if ((*x) > (*y))
				{
					np = x;
					x = y;
					y = np;
				}
				if (y == H1)
					H1 = x;
				fibonnaci_link(H1, y, x);
				if (x->right == x)
					H1 = x;
					A[d] = NULL;
				d = d + 1;
			}
			A[d] = x;
			x = (T*)x->right;
		}
		while (x != H1);
		H = NULL;
		for (int j = 0; j <= D; j++)
		{
			if (A[j] != NULL)
			{
				A[j]->left = A[j];
				A[j]->right =A[j];
				if (H != NULL)
				{
					(H->left)->right = A[j];
					A[j]->right = H;
					A[j]->left = H->left;
					H->left = A[j];
					if (*(A[j]) < *(H))
					H = A[j];
				}
				else
				{
					H = A[j];
				}
				if(H == NULL)
					H = A[j];
				else if (*(A[j]) < *(H))
					H = A[j];
			}
		}

		return 1;
	}

	// Forward Declaration of Event_Block
	class Event_Block;

	///----------------------------------------------------------------------------------------------------
	/// Event_Object - Object which records revision, and memory information
	///----------------------------------------------------------------------------------------------------

	class Event_Object : public node, public Base_Object
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
#pragma once
#include "Core_Includes.h"
#include "Simple_Pool.h"

/*-
 * Copyright 1997-2003 John-Mark Gurney.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	$Id: fib.c,v 1.31 2003/01/14 10:11:30 jmg Exp $
 *
 */

#include <limits.h>
#include <stdlib.h>

typedef int (*voidcmp)(void *, void *);

#define fh_swap(type, a, b)\
		do {			\
			type c;		\
			c = a;		\
			a = b;		\
			b = c;		\
		} while (0)		\


#define INT_BITS (sizeof(int) * 8)

static int ceillog2(unsigned int a)
{
	int oa;
	int i;
	int b;

	oa = a;
	b = INT_BITS / 2;
	i = 0;
	while (b) {
		i = (i << 1);
		if (a >= (unsigned)(1 << b))
		{
			a /= (1 << b);
			i = i | 1;
		} else
			a &= (1 << b) - 1;
		b /= 2;
	}
	if ((1 << i) == oa)
		return i;
	else
		return i + 1;
}

#define NO_FREE

template<typename KeyType,typename DataType>
class Fibonacci_Heap
{
public:
	struct fibheap_element
	{
		int	fhe_degree;
		int	fhe_mark;
		fibheap_element *fhe_p;
		fibheap_element *fhe_child;
		fibheap_element *fhe_left;
		fibheap_element *fhe_right;
		KeyType fhe_key;
		DataType* fhe_data;
	};

	void Initialize()
	{
		simple_allocator.Initialize(500000);

		fh_n = 0;
		fh_Dl = -1;
		fh_cons = NULL;
		fh_min = NULL;
		fh_root = NULL;
		
		//specific to key heap
		fh_keys = 1;
		//fh_cmp_fnct = NULL;
		//fh_neginf = NULL;
	}

	void Reset()
	{
		simple_allocator.Reset();
		fh_min=NULL;

		//while (fh_min != NULL) free(fh_extractminel());


		//fh_cmp_fnct = NULL;
		//fh_neginf = NULL;
		if(fh_cons != NULL)
		{
			free(fh_cons);
		}
		fh_cons = NULL;

		fh_n = 0;
		fh_Dl = -1;
		fh_root = NULL;
	}

	fibheap_element* Push(KeyType key, DataType *data)
	{
		fibheap_element* x = fhe_newelem();

		//if ((x = fhe_newelem()) == NULL) return NULL;

		/* just insert on root list, and make sure it's not the new min */
		x->fhe_data = data;
		x->fhe_key = key;

		fh_insertel(x);

		return x;
	}

	KeyType Get_Min_Value()
	{
		if (fh_min == NULL) return INT_MIN;
		return fh_min->fhe_key;
	}

	KeyType Update_Value(fibheap_element *x, KeyType key)
	{
		KeyType ret;

		ret = x->fhe_key;
		Update_Value_Data(x, key, x->fhe_data);

		return ret;
	}

	DataType* Update_Value_Data(fibheap_element *x, KeyType key, DataType *data)
	{
		DataType* odata;
		KeyType okey;
		fibheap_element *y;
		int r;

		odata = x->fhe_data;
		okey = x->fhe_key;

		/*
		 * we can increase a key by deleting and reinserting, that
		 * requires O(lgn) time.
		 */
		if ((r = fh_comparedata(key, data, x)) > 0)
		{
			/* XXX - bad code! */
			abort();
			fh_deleteel(x);

			x->fhe_data = data;
			x->fhe_key = key;

			fh_insertel(x);

			return odata;
		}

		x->fhe_data = data;
		x->fhe_key = key;

		/* because they are equal, we don't have to do anything */
		if (r == 0) return odata;

		y = x->fhe_p;

		if (fh_keys && okey == key) return odata;

		if (y != NULL && fh_compare(x, y) <= 0)\
		{
			fh_cut(x, y);
			fh_cascading_cut(y);
		}

		/*
		 * the = is so that the call from fh_delete will delete the proper
		 * element.
		 */
		if (fh_compare(x, fh_min) <= 0) fh_min = x;

		return odata;
	}

	bool Empty()
	{
		return fh_min==NULL;
	}

	DataType* Get_Min_Data()
	{
		if(fh_min == NULL) return NULL;
		return fh_min->fhe_data;
	}

	DataType* Pop_Min()
	{
		fibheap_element *z;
		DataType *ret;

		ret = NULL;

		if (fh_min != NULL)
		{
			z = fh_extractminel();
			ret = z->fhe_data;
		#ifndef NO_FREE
			free(z);
		#endif
		}

		return ret;
	}

	DataType* Update_Data(fibheap_element* x, DataType *data)
	{
		return Update_Value_Data(x, x->fhe_key, data);
	}

	DataType* Pop(fibheap_element *x)
	{
		DataType *k;

		k = x->fhe_data;
		Update_Value(x, INT_MIN);
		Pop_Min();

		return k;
	}

private:
	void fh_deleteel(fibheap_element *x)
	{
		DataType *data;
		KeyType key;

		data = x->fhe_data;
		key = x->fhe_key;

		Update_Value(x, INT_MIN);
		
		if (fh_extractminel() != x) abort();

		x->fhe_data = data;
		x->fhe_key = key;
	}

	fibheap_element* fh_extractminel()
	{
		fibheap_element *ret;
		fibheap_element *x, *y, *orig;

		ret = fh_min;

		orig = NULL;
		/* put all the children on the root list */
		/* for true consistancy, we should use fhe_remove */
		for(x = ret->fhe_child; x != orig && x != NULL;)
		{
			if (orig == NULL) orig = x;

			y = x->fhe_right;
			x->fhe_p = NULL;
			fh_insertrootlist(x);
			x = y;
		}

		/* remove minimum from root list */
		fh_removerootlist(ret);
		fh_n--;

		/* if we aren't empty, consolidate the heap */
		if (fh_n == 0) fh_min = NULL;
		else
		{
			fh_min = ret->fhe_right;
			fh_consolidate();
		}

		return ret;
	}

	void fh_insertrootlist(fibheap_element* x)
	{
		if (fh_root == NULL)
		{
			fh_root = x;
			x->fhe_left = x;
			x->fhe_right = x;
			return;
		}

		fhe_insertafter(fh_root, x);
	}

	void fh_removerootlist(fibheap_element* x)
	{
		if (x->fhe_left == x) fh_root = NULL;
		else fh_root = fhe_remove(x);
	}

	void fh_consolidate()
	{
		fibheap_element **a;
		fibheap_element *w;
		fibheap_element *y;
		fibheap_element *x;
		int i;
		int d;
		int D;

		fh_checkcons();

		/* assign a the value of fh_cons so I don't have to rewrite code */
		D = fh_Dl + 1;
		a = fh_cons;

		for (i = 0; i < D; i++)
			a[i] = NULL;

		while ((w = fh_root) != NULL) {
			x = w;
			fh_removerootlist(w);
			d = x->fhe_degree;
			/* XXX - assert that d < D */
			while(a[d] != NULL) {
				y = a[d];
				if (fh_compare(x, y) > 0) fh_swap(fibheap_element *, x, y);
				fh_heaplink(y, x);
				a[d] = NULL;
				d++;
			}
			a[d] = x;
		}
		fh_min = NULL;
		for (i = 0; i < D; i++)
			if (a[i] != NULL) {
				fh_insertrootlist(a[i]);
				if (fh_min == NULL || fh_compare(a[i],fh_min) < 0)
					fh_min = a[i];
			}
	}

	void fh_heaplink(fibheap_element *y, fibheap_element *x)
	{
		/* make y a child of x */
		if (x->fhe_child == NULL)
			x->fhe_child = y;
		else
			fhe_insertbefore(x->fhe_child, y);
		y->fhe_p = x;
		x->fhe_degree++;
		y->fhe_mark = 0;
	}

	void fh_cut(fibheap_element *x, fibheap_element *y)
	{
		fhe_remove(x);
		y->fhe_degree--;
		fh_insertrootlist(x);
		x->fhe_p = NULL;
		x->fhe_mark = 0;
	}

	void fh_cascading_cut(fibheap_element *y)
	{
		fibheap_element *z;

		while ((z = y->fhe_p) != NULL) {
			if (y->fhe_mark == 0) {
				y->fhe_mark = 1;
				return;
			} else {
				fh_cut(y, z);
				y = z;
			}
		}
	}

	fibheap_element* fhe_newelem()
	{
		//fibheap_element* e = (fibheap_element*)malloc(sizeof(fibheap_element));

		fibheap_element* e = simple_allocator.Allocate();

		fhe_initelem(e);

		return e;
	}

	void fhe_initelem(fibheap_element *e)
	{
		e->fhe_degree = 0;
		e->fhe_mark = 0;
		e->fhe_p = NULL;
		e->fhe_child = NULL;
		e->fhe_left = e;
		e->fhe_right = e;
		e->fhe_data = NULL;
	}

	void fhe_insertafter(fibheap_element *a, fibheap_element *b)
	{
		if (a == a->fhe_right) {
			a->fhe_right = b;
			a->fhe_left = b;
			b->fhe_right = a;
			b->fhe_left = a;
		} else {
			b->fhe_right = a->fhe_right;
			a->fhe_right->fhe_left = b;
			a->fhe_right = b;
			b->fhe_left = a;
		}
	}

	void fhe_insertbefore(fibheap_element *a, fibheap_element *b)
	{
		fhe_insertafter(a->fhe_left, b);
	}

	fibheap_element* fhe_remove(fibheap_element *x)
	{
		fibheap_element *ret;

		if (x == x->fhe_left)
			ret = NULL;
		else
			ret = x->fhe_left;

		/* fix the parent pointer */
		if (x->fhe_p != NULL && x->fhe_p->fhe_child == x)
			x->fhe_p->fhe_child = ret;

		x->fhe_right->fhe_left = x->fhe_left;
		x->fhe_left->fhe_right = x->fhe_right;

		/* clear out hanging pointers */
		x->fhe_p = NULL;
		x->fhe_left = x;
		x->fhe_right = x;

		return ret;
	}

	void fh_checkcons()
	{
		int oDl;

		/* make sure we have enough memory allocated to "reorganize" */
		if (fh_Dl == -1 || fh_n > (1 << fh_Dl))
		{
			oDl = fh_Dl;

			if ((fh_Dl = ceillog2(fh_n) + 1) < 8)
			{
				fh_Dl = 8;
			}
			
			if (oDl != fh_Dl)
			{
				fh_cons = (fibheap_element **)realloc(fh_cons,sizeof *fh_cons * (fh_Dl + 1));
			}

			if (fh_cons == NULL)
			{
				abort();
			}
		}
	}

	int fh_compare(fibheap_element *a, fibheap_element *b)
	{
		if (a->fhe_key < b->fhe_key) return -1;
		if (a->fhe_key == b->fhe_key) return 0;
		return 1;
	}

	int fh_comparedata(KeyType key, DataType* data, fibheap_element *b)
	{
		fibheap_element a;

		a.fhe_key = key;
		a.fhe_data = data;

		return fh_compare( &a, b);
	}

	void fh_insertel(fibheap_element *x)
	{
		fh_insertrootlist(x);

		//if(fh_min == NULL || (fh_keys ? x->fhe_key < fh_min->fhe_key : fh_cmp_fnct(x->fhe_data, fh_min->fhe_data) < 0))
		//{
		//	fh_min = x;
		//}

		if(fh_min == NULL || (x->fhe_key < fh_min->fhe_key))
		{
			fh_min = x;
		}

		fh_n++;
	}

	//int (*fh_cmp_fnct)(DataType *, DataType *);

	int	fh_n;
	int	fh_Dl;
	fibheap_element** fh_cons;
	fibheap_element* fh_min;
	fibheap_element* fh_root;
	//void* fh_neginf;
	int	fh_keys	: 1;

	Simple_Pool<fibheap_element> simple_allocator;
};

#pragma once
#include "Core_Includes.h"

//---------------------------------------------------------
//	Min_Pool - memory pool adept at pulling minimum values
//---------------------------------------------------------

template<typename ValType>
class Min_Pool
{
public:
	//---- in a memory pool each slot is either a data block or a pointer to the next free data block ----
	
	struct Memory_Cell
	{
		float value;
		Memory_Cell* larger_ptr; // larger cell
		Memory_Cell* smaller_ptr; // smaller cell

		union
		{
			Memory_Cell* next_free_ptr;
			ValType data;
		};
	};

	Min_Pool(void)
	{
		first_free_ptr=nullptr;
		pool=nullptr;
		end_pool_itr=nullptr;
		ptr_storage=nullptr;
	}

	void Initialize(unsigned int _size)
	{
		size=_size;

		//---- data is held in size pool elements (which are of the same size as Data) ----

		pool=new Memory_Cell[size];
		end_pool_itr=pool+size;

		first_free_ptr=pool;

		//---- all members are initialized to the end of the structure ----

		ptr_storage=pool;
		
		min_ptr=nullptr;
		mid_ptr=nullptr;
		max_ptr=nullptr;
		alloc_counter=0;
		mid_position=0;

		furthest_allocated=pool;

		for(;ptr_storage!=end_pool_itr;ptr_storage++)
		{
			ptr_storage->next_free_ptr=ptr_storage+1;
			ptr_storage->value=FLT_MAX;
			ptr_storage->larger_ptr=nullptr;
			ptr_storage->smaller_ptr=nullptr;
		}
	}

	void Reset()
	{
		first_free_ptr=pool;

		//---- all members are initialized to the end of the structure ----

		ptr_storage=pool;
		
		min_ptr=nullptr;
		mid_ptr=nullptr;
		max_ptr=nullptr;
		alloc_counter=0;
		mid_position=0;

		for(;ptr_storage!=(furthest_allocated+1);ptr_storage++)
		{
			ptr_storage->next_free_ptr=ptr_storage+1;
			ptr_storage->value=FLT_MAX;
			ptr_storage->larger_ptr=nullptr;
			ptr_storage->smaller_ptr=nullptr;
		}

		furthest_allocated=pool;
	}

	void Update_Mid()
	{
		int mid;

		if(alloc_counter%2==1)
		{
			//odd, there is an exact mid
			mid=(alloc_counter-1)/2;
		}
		else
		{
			//even, use the floor of the exact mid as mid
			mid=(alloc_counter-1)/2;
		}

		if(mid_position < mid)
		{
			// mid_ptr is too low
			mid_ptr=mid_ptr->larger_ptr;
			++mid_position;
		}
		else if(mid_position > mid)
		{
			// mid_ptr is too high
			mid_ptr=mid_ptr->smaller_ptr;
			--mid_position;
		}

		assert(mid_ptr!=nullptr);
	}

	inline unsigned int Push(ValType& data,float fscore)
	{
		++alloc_counter;

		if(first_free_ptr > furthest_allocated) furthest_allocated=first_free_ptr;

		// allocate into free list

		ptr_storage=first_free_ptr->next_free_ptr;

		first_free_ptr->value=fscore;
		first_free_ptr->data=data;
		
		Memory_Cell* current=first_free_ptr;

		unsigned int retval=(unsigned int)(first_free_ptr-pool);

		first_free_ptr=ptr_storage;

		assert(first_free_ptr!=end_pool_itr);

		// allocate to doubly-linked min_ptr list

		float value=current->value;

		if( min_ptr==nullptr && max_ptr==nullptr )
		{
			// easy, first allocation

			current->smaller_ptr=nullptr;
			current->larger_ptr=nullptr;
			min_ptr=current;
			max_ptr=current;
			mid_ptr=current;

			assert(mid_ptr!=nullptr);
		}
		else if( value <= min_ptr->value )
		{
			// easy, append to front of list

			current->smaller_ptr=nullptr;
			current->larger_ptr=min_ptr;
			current->larger_ptr->smaller_ptr=current;
			min_ptr=current;

			// have added one below mid_ptr
			++mid_position;
		}
		else if( value >= max_ptr->value )
		{
			// easy, append to back of list

			current->larger_ptr=nullptr;
			current->smaller_ptr=max_ptr;
			current->smaller_ptr->larger_ptr=current;
			max_ptr=current;
		}
		else
		{
			if(value < mid_ptr->value)
			{
				float mid_diff=mid_ptr->value-value;
				float min_diff=value-min_ptr->value;

				if(min_diff < mid_diff)
				{
					ptr_storage=min_ptr;

					// iterate from min to mid, ptr_storage->larger_ptr shouldn't be nullptr

					while(ptr_storage->larger_ptr->value < value)
					{
						ptr_storage=ptr_storage->larger_ptr;
					}

					// at this point: value < ptr_storage->larger_ptr->value && value >= ptr_storage->value

					// link larger to you and yourself to larger
					current->larger_ptr=ptr_storage->larger_ptr;
					current->larger_ptr->smaller_ptr=current;

					// link ptr_storage (smaller) to you and yourself to ptr_storage
					current->smaller_ptr=ptr_storage;
					current->smaller_ptr->larger_ptr=current;
				}
				else
				{
					ptr_storage=mid_ptr;

					// iterate from mid to min, ptr_storage->smaller_ptr shouldn't be nullptr

					while(ptr_storage->smaller_ptr->value > value)
					{
						ptr_storage=ptr_storage->smaller_ptr;
					}

					// at this point: value >= ptr_storage->smaller_ptr->value && value < ptr_storage->value

					// link ptr_storage (larger) to you and yourself to ptr_storage
					current->smaller_ptr=ptr_storage->smaller_ptr;
					current->smaller_ptr->larger_ptr=current;

					// link smaller to you and yourself to smaller
					current->larger_ptr=ptr_storage;
					current->larger_ptr->smaller_ptr=current;
				}

				// have added one below mid_ptr
				++mid_position;
			}
			else if(value > mid_ptr->value)
			{
				float mid_diff=value-mid_ptr->value;
				float max_diff=max_ptr->value-value;
				
				if(max_diff < mid_diff)
				{
					ptr_storage=max_ptr;

					// iterate from max to mid, ptr_storage->smaller_ptr shouldn't be nullptr

					while(ptr_storage->smaller_ptr->value > value)
					{
						ptr_storage=ptr_storage->smaller_ptr;
					}

					// at this point: value >= ptr_storage->smaller_ptr->value && value < ptr_storage->value

					// link ptr_storage (larger) to you and yourself to ptr_storage
					current->smaller_ptr=ptr_storage->smaller_ptr;
					current->smaller_ptr->larger_ptr=current;

					// link smaller to you and yourself to smaller
					current->larger_ptr=ptr_storage;
					current->larger_ptr->smaller_ptr=current;
				}
				else
				{
					ptr_storage=mid_ptr;

					// iterate from mid to max, ptr_storage->larger_ptr shouldn't be nullptr

					while(ptr_storage->larger_ptr->value < value)
					{
						ptr_storage=ptr_storage->larger_ptr;
					}

					// at this point: value < ptr_storage->larger_ptr->value && value >= ptr_storage->value

					// link larger to you and yourself to larger
					current->larger_ptr=ptr_storage->larger_ptr;
					current->larger_ptr->smaller_ptr=current;

					// link ptr_storage (smaller) to you and yourself to ptr_storage
					current->smaller_ptr=ptr_storage;
					current->smaller_ptr->larger_ptr=current;
				}
			}
			else //if(value == mid_ptr->value)
			{
				// link larger to you and yourself to larger
				current->larger_ptr=mid_ptr->larger_ptr;
				current->larger_ptr->smaller_ptr=current;

				// link ptr_storage (smaller) to you and yourself to ptr_storage
				current->smaller_ptr=mid_ptr;
				current->smaller_ptr->larger_ptr=current;
			}
		}

		// determine if mid needs to shift
		Update_Mid();

		return retval;
	}

	inline bool Empty()
	{
		if( min_ptr==nullptr ) return true;
		else return false;
	}

	//---- update a value ----

	inline void Update_Value(int index,float fscore)
	{
		// unlink yourself from the min_ptr list

		Memory_Cell* current=pool+index;
		
		
		// eliminate the case where you are both min and max

		if(current == min_ptr && current == max_ptr)
		{
			current->value=fscore;
			return;
		}


		if( current->value < mid_ptr->value )
		{
			// smaller than mid, extracting below mid
			if(mid_ptr!=min_ptr) --mid_position;
		}
		else if( current == mid_ptr )
		{
			// you are mid in this special case, shift mid to be the next larger cell so position is equivalent
			mid_ptr=mid_ptr->larger_ptr;
			assert(mid_ptr!=nullptr);
		}
		else if( current->value == mid_ptr->value )
		{
			// this is a tricky case, have to check if current is below mid or not to decide whether to decrement

			ptr_storage=mid_ptr;

			while(ptr_storage->value == mid_ptr->value)
			{
				if(ptr_storage==current)
				{
					if(mid_ptr!=min_ptr) --mid_position;
					break;
				}

				ptr_storage=ptr_storage->smaller_ptr;
			}
		}

		
		if(current->larger_ptr==nullptr)
		{
			// you are max
			max_ptr=current->smaller_ptr;
		}
		else
		{
			// you are not max
			current->larger_ptr->smaller_ptr=current->smaller_ptr;
		}

		if(current->smaller_ptr==nullptr)
		{
			// you are min
			min_ptr=current->larger_ptr;
		}
		else
		{
			// you are not min
			current->smaller_ptr->larger_ptr=current->larger_ptr;
		}


		current->larger_ptr=nullptr;
		current->smaller_ptr=nullptr;
		current->value=fscore;

		// allocate to doubly-linked min_ptr list

		float value=current->value;

		if( value <= min_ptr->value )
		{
			// easy, append to front of list

			current->smaller_ptr=nullptr;
			current->larger_ptr=min_ptr;
			current->larger_ptr->smaller_ptr=current;
			min_ptr=current;

			// have added one below mid_ptr
			++mid_position;
		}
		else if( value >= max_ptr->value )
		{
			// easy, append to back of list

			current->larger_ptr=nullptr;
			current->smaller_ptr=max_ptr;
			current->smaller_ptr->larger_ptr=current;
			max_ptr=current;
		}
		else
		{
			if(value < mid_ptr->value)
			{
				float mid_diff=mid_ptr->value-value;
				float min_diff=value-min_ptr->value;

				if(min_diff < mid_diff)
				{
					ptr_storage=min_ptr;

					// iterate from min to mid, ptr_storage->larger_ptr shouldn't be nullptr

					while(ptr_storage->larger_ptr->value < value)
					{
						ptr_storage=ptr_storage->larger_ptr;
					}

					// at this point: value < ptr_storage->larger_ptr->value && value >= ptr_storage->value

					// link larger to you and yourself to larger
					current->larger_ptr=ptr_storage->larger_ptr;
					current->larger_ptr->smaller_ptr=current;

					// link ptr_storage (smaller) to you and yourself to ptr_storage
					current->smaller_ptr=ptr_storage;
					current->smaller_ptr->larger_ptr=current;
				}
				else
				{
					ptr_storage=mid_ptr;

					// iterate from mid to min, ptr_storage->smaller_ptr shouldn't be nullptr

					while(ptr_storage->smaller_ptr->value > value)
					{
						ptr_storage=ptr_storage->smaller_ptr;
					}

					// at this point: value >= ptr_storage->smaller_ptr->value && value < ptr_storage->value

					// link ptr_storage (larger) to you and yourself to ptr_storage
					current->smaller_ptr=ptr_storage->smaller_ptr;
					current->smaller_ptr->larger_ptr=current;

					// link smaller to you and yourself to smaller
					current->larger_ptr=ptr_storage;
					current->larger_ptr->smaller_ptr=current;
				}

				// have added one below mid_ptr
				++mid_position;
			}
			else if(value > mid_ptr->value)
			{
				float mid_diff=value-mid_ptr->value;
				float max_diff=max_ptr->value-value;
				
				if(max_diff < mid_diff)
				{
					ptr_storage=max_ptr;

					// iterate from max to mid, ptr_storage->smaller_ptr shouldn't be nullptr

					while(ptr_storage->smaller_ptr->value > value)
					{
						ptr_storage=ptr_storage->smaller_ptr;
					}

					// at this point: value >= ptr_storage->smaller_ptr->value && value < ptr_storage->value

					// link ptr_storage (larger) to you and yourself to ptr_storage
					current->smaller_ptr=ptr_storage->smaller_ptr;
					current->smaller_ptr->larger_ptr=current;

					// link smaller to you and yourself to smaller
					current->larger_ptr=ptr_storage;
					current->larger_ptr->smaller_ptr=current;
				}
				else
				{
					ptr_storage=mid_ptr;

					// iterate from mid to max, ptr_storage->larger_ptr shouldn't be nullptr

					while(ptr_storage->larger_ptr->value < value)
					{
						ptr_storage=ptr_storage->larger_ptr;
					}

					// at this point: value < ptr_storage->larger_ptr->value && value >= ptr_storage->value

					// link larger to you and yourself to larger
					current->larger_ptr=ptr_storage->larger_ptr;
					current->larger_ptr->smaller_ptr=current;

					// link ptr_storage (smaller) to you and yourself to ptr_storage
					current->smaller_ptr=ptr_storage;
					current->smaller_ptr->larger_ptr=current;
				}
			}
			else //if(value == mid_ptr->value)
			{
				// link larger to you and yourself to larger
				current->larger_ptr=mid_ptr->larger_ptr;
				current->larger_ptr->smaller_ptr=current;

				// link ptr_storage (smaller) to you and yourself to ptr_storage
				current->smaller_ptr=mid_ptr;
				current->smaller_ptr->larger_ptr=current;
			}

		}
		
		// determine if mid_ptr needs to shift
		Update_Mid();
	}

	//---- pop the minimum value ----

	inline float Pop_Min(ValType& ret_container)
	{
		--alloc_counter;

		// allocate to doubly-linked min_ptr list

		ptr_storage=min_ptr;
		
		min_ptr=min_ptr->larger_ptr;

		if(min_ptr == max_ptr)
		{
			// set mid_position to 0 in this case
			mid_position=0;
			mid_ptr=min_ptr;

			min_ptr->smaller_ptr=nullptr;
		}
		else if(min_ptr == nullptr)
		{
			// structure is empty now
			mid_ptr=nullptr;
			max_ptr=nullptr;
			mid_position=0;
		}
		else
		{
			// more structure to go
			--mid_position;
			Update_Mid();

			min_ptr->smaller_ptr=nullptr;
		}

		float retval=ptr_storage->value;
		ret_container=ptr_storage->data;
		
		ptr_storage->value=FLT_MAX;
		ptr_storage->smaller_ptr=nullptr;
		ptr_storage->larger_ptr=nullptr;

		Memory_Cell* ptr_itr;

		// link into free list

		if(ptr_storage<first_free_ptr)
		{
			//there are no free pointers before target cell

			ptr_storage->next_free_ptr=first_free_ptr;
			first_free_ptr=ptr_storage;
		}
		else
		{
			//there are free pointers before target cell
			
			ptr_itr=first_free_ptr;

			while(ptr_itr->next_free_ptr < ptr_storage)
			{
				ptr_itr=ptr_itr->next_free_ptr;
			}

			ptr_storage->next_free_ptr=ptr_itr->next_free_ptr;
			ptr_itr->next_free_ptr=ptr_storage;
		}

		return retval;
	}

	Memory_Cell* pool;
	Memory_Cell* ptr_storage;
	Memory_Cell* first_free_ptr;
	Memory_Cell* end_pool_itr;

	Memory_Cell* furthest_allocated;
	Memory_Cell* min_ptr;
	
	Memory_Cell* mid_ptr;
	int mid_position;

	Memory_Cell* max_ptr;

	int alloc_counter;

	int size;
};



			//if(value < mid_ptr->value)
			//{
			//	ptr_storage=min_ptr;

			//	// iterate from min to max, ptr_storage->larger_ptr shouldn't be nullptr

			//	while(ptr_storage->larger_ptr->value < value)
			//	{
			//		ptr_storage=ptr_storage->larger_ptr;
			//	}

			//	// at this point: value < ptr_storage->larger_ptr->value && value >= ptr_storage->value

			//	// link larger to you and yourself to larger
			//	current->larger_ptr=ptr_storage->larger_ptr;
			//	current->larger_ptr->smaller_ptr=current;

			//	// link ptr_storage (smaller) to you and yourself to ptr_storage
			//	current->smaller_ptr=ptr_storage;
			//	current->smaller_ptr->larger_ptr=current;

			//	// have added one below mid_ptr
			//	++mid_position;
			//}
			//else if(value > mid_ptr->value)
			//{
			//	ptr_storage=max_ptr;

			//	// iterate from max to min, ptr_storage->smaller_ptr shouldn't be nullptr

			//	while(ptr_storage->smaller_ptr->value > value)
			//	{
			//		ptr_storage=ptr_storage->smaller_ptr;
			//	}

			//	// at this point: value >= ptr_storage->smaller_ptr->value && value < ptr_storage->value

			//	// link ptr_storage (larger) to you and yourself to ptr_storage
			//	current->smaller_ptr=ptr_storage->smaller_ptr;
			//	current->smaller_ptr->larger_ptr=current;

			//	// link smaller to you and yourself to smaller
			//	current->larger_ptr=ptr_storage;
			//	current->larger_ptr->smaller_ptr=current;
			//}
			//else //if(value == mid_ptr->value)
			//{
			//	// link larger to you and yourself to larger
			//	current->larger_ptr=mid_ptr->larger_ptr;
			//	current->larger_ptr->smaller_ptr=current;

			//	// link ptr_storage (smaller) to you and yourself to ptr_storage
			//	current->smaller_ptr=mid_ptr;
			//	current->smaller_ptr->larger_ptr=current;
			//}
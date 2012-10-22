#pragma once
#include "Core_Includes.h"

///============================================================================
/// Byte_Balloon
/// A simple allocator of non-decreasing size
///============================================================================

struct Byte_Balloon
{
public:
	Byte_Balloon():bytes(nullptr),current_size(0),max_size(0){};

	void clear()
	{
		current_size=0;
	}

	int size()
	{
		return current_size;
	}
	
	char* buffer()
	{
		return bytes;
	}

	char* allocate(int byte_array_size)
	{
		//cout << "entering allocate" << endl;
		
		//cout << "\tcurrent_size: " << current_size << endl;
		//cout << "\tmax_size: " << max_size << endl;
		//cout << "\tbyte_array_size: " << byte_array_size << endl;

		
		if(current_size + byte_array_size > max_size)
		{
			//cout << "\t\texpanding bytes" << endl;
		
			max_size=(max_size+byte_array_size)*2;
			
			//cout << "\t\tmax_size: " << max_size << endl;
			
			char* new_bytes=new char[max_size];

			memcpy(new_bytes,bytes,current_size);
			
			delete[] bytes;
			
			bytes=new_bytes;
		}
		
		char* retval=&bytes[current_size];

		current_size+=byte_array_size;

		//cout << "\tending current_size: " << current_size << endl;
		
		//cout << "leaving allocate" << endl;
		
		return retval;
	}

private:
	char* bytes;
	int current_size;
	int max_size;
};
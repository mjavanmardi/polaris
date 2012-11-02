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

	int max()
	{
		return max_size;
	}
	
	char* buffer()
	{
		return bytes;
	}

	char* allocate(int byte_array_size)
	{
		if(current_size + byte_array_size > max_size)
		{
			max_size=(max_size+byte_array_size)*2;

			char* new_bytes=new char[max_size];

			memcpy(new_bytes,bytes,current_size);

			delete bytes;

			bytes=new_bytes;
		}
		
		char* retval=&bytes[current_size];

		current_size+=byte_array_size;
		
		return retval;
	}

private:
	char* bytes;
	int current_size;
	int max_size;
};
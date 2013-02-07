#pragma once
#include "Core_Includes.h"

///============================================================================
/// Multi_Buffer
/// A simple translator between _iteration and data based on period and offset
///============================================================================

template<typename DataType, const int buffer_size=2, const int period=1>
struct Multi_Buffer
{
public:
	Multi_Buffer():offset(0){};
	
	static_assert(period > 0, "period must be greater 0!");

	void set_offset(int _offset){ assert(offset>=0); offset=_offset; }

	DataType& operator[](int itr)
	{
		return multi_buffer[((itr-offset)/period)%buffer_size];
	}

private:
	DataType multi_buffer[buffer_size];
	int offset;
};

//template<typename DataType>
//struct Dynamic_Multi_Buffer
//{
//public:
//	Dynamic_Multi_Buffer():offset(0),buffer_size(2),period(1)
//	{
//		multi_buffer=new DataType[buffer_size];
//	};
//
//	Dynamic_Multi_Buffer(int _offset, int _buffer_size, int _period):offset(_offset),buffer_size(_buffer_size),period(_period)
//	{
//		multi_buffer=new DataType[buffer_size];
//	};
//
//	static_assert(period > 0, "period must be greater 0!");
//
//	DataType& operator[](int itr)
//	{
//		return multi_buffer[((itr-offset)/period)%buffer_size];
//	}
//
//private:
//	DataType* multi_buffer;
//	int offset;
//	int buffer_size;
//	int period;
//};
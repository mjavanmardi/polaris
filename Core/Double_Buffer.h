#pragma once
#include "Core_Includes.h"

///============================================================================
/// Double_Buffer
/// A simple translator between _iteration and data based on period and offset
///============================================================================

template<typename DataType,const int period=1>
struct Double_Buffer
{
public:
	Double_Buffer():offset(0){};
	
	static_assert(period > 0, "period must be greater 0!");

	void set_offset(int _offset){ assert(offset>=0); offset=_offset; }

	DataType& operator[](int itr)
	{
		return double_buffer[((itr-offset)/period)%2];
	}

private:
	DataType double_buffer[2];
	int offset;
};
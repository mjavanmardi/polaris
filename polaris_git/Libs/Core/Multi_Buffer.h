#pragma once
#include "Dependencies.h"

namespace polaris
{
	///----------------------------------------------------------------------------------------------------
	/// Multi_Buffer
	/// A simple translator between _iteration and data based on period and offset
	///----------------------------------------------------------------------------------------------------

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

	template<typename DataType>
	struct Dynamic_Multi_Buffer
	{
	public:
		void Initialize(unsigned int _offset=0, unsigned int _period=1, unsigned int _buffer_size=3)
		{
			offset=_offset;
			buffer_size=_buffer_size;
			period=_period;
			multi_buffer=new DataType[buffer_size];
		};

		DataType& operator[](int itr) const
		{
			return multi_buffer[((itr-offset)/period)%buffer_size];
		}


		DataType* multi_buffer;
		unsigned int offset;
		unsigned int buffer_size;
		unsigned int period;
	};
}
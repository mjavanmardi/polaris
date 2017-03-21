///----------------------------------------------------------------------------------------------------
/// Execution_Block.cpp - Block of execution objects in a contiguous memory space
///----------------------------------------------------------------------------------------------------

#include "Execution_Block.h"
#include "World.h"

namespace polaris
{
	const unsigned int Execution_Block::_data_offset = ((sizeof(Execution_Block)/__cache_line_size)+1)*__cache_line_size;

}
#pragma once
#include "Typed_Data_Page.h"

///============================================================================
/// Data_Root
/// A singleton object responsible for allocating typed data pages
///============================================================================

class Data_Root
{
public:
	struct Data_Type
	{
		Data_Type(Typed_Data_Pages<>* ptr):typed_data_pages(ptr){};
		Typed_Data_Pages<>* typed_data_pages;
	};
};

static Data_Root* data_root_ptr;

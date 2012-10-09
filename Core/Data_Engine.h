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

	//void Activate_Type(Typed_Data_Pages<>* ptr)
	//{
	//	while(AtomicExchange(&de_lock,1)) SLEEP(0); // lock the data engine

	//	active_types.push_back(Data_Type(ptr));

	//	de_lock=0;
	//}

	//list<Data_Type> active_types;

	//volatile long de_lock;
};

static Data_Root* data_root_ptr;

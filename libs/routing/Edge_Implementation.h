#pragma once
#include "Graph.h"
#include "Connection_Group_Implementation.h"
#include "boost/intrusive/set.hpp"

namespace polaris
{
	template<typename Attribute_Type = NT>
	struct Edge_Implementation : public Attribute_Type
	{
		typedef Attribute_Type attribute_type;
		typedef typename attribute_type::base_edge_type base_edge_type;

		Attribute_Type* attributes(){ return (Attribute_Type*)this; }
	};
}
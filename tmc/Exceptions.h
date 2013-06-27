#pragma once
#include <exception>

namespace polaris {
class size_mismatch_ex : public std::exception
{
	virtual const char* what() const throw()
	{
		return "The the weight and link_id vectors are of different size";
	}
} size_ex;

class no_outbound_links : public std::exception
{
	virtual const char* what() const throw()
	{
		return "This node has no outbound links";
	}
} no_link_ex;

}
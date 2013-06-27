#pragma once
#include "utils/types.h"
namespace polaris {
namespace utils	{

LinkDir get_link_dir(int uid_) 
{
	LinkDir res;
	res.link_id = uid_ >> 1;
	res.dir = uid_  & 1;
	return res;
}
int get_link_uid(LinkDir ld_)
{
	return (ld_.link_id << 1) + ld_.dir;
}
int get_link_uid(LinkID ld_, bool dir_)
{
	return (ld_ << 1) + dir_;
}

}}
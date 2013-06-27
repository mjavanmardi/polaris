#pragma once
#include "Io/database.h"
namespace polaris
{
typedef int NodeID;
typedef int LinkID;
typedef bool Dir;
typedef odb::query<polaris::io::Link> LinkQ;
typedef odb::query<polaris::io::LinkMOE> LinkMOEQ;
typedef odb::query<polaris::io::Connect> ConnectQ;
typedef odb::query<polaris::io::Microsoft_Event> Microsoft_EventQ;


typedef odb::result<polaris::io::Link> LinkR;
typedef odb::result<polaris::io::LinkMOE> LinkMOER;
typedef odb::result<polaris::io::Connect> ConnectR;
typedef odb::result<polaris::io::Microsoft_Event> Microsoft_EventR;


typedef LinkR::iterator LinkIt;





struct LinkDir 
{
	LinkID link_id;
	Dir dir;
};

namespace io
{
template<class ValueType>
struct db_ptr
{
	typedef std::shared_ptr<ValueType> Type;
};

}}

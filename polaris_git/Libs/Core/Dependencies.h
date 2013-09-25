#pragma once



#include <iostream>
#include <sstream>
#include <boost/intrusive/list.hpp>
#include <boost/container/deque.hpp>
#include <boost/container/list.hpp>
#include <boost/container/set.hpp>
#include <boost/container/vector.hpp>
#include <boost/unordered/unordered_map.hpp>

//#include <deque>
//#include <list>
//#include <set>
//#include <vector>
#include <type_traits>


#ifdef _MSC_VER
#include <Windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#define __forceinline inline
#endif


using namespace std;


using namespace boost::container;
using namespace boost::unordered;


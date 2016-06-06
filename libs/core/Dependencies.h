#pragma once



#include <iostream>
#include <sstream>
#include <boost/intrusive/list.hpp>
/*#include <boost/container/deque.hpp>
#include <boost/container/list.hpp>
#include <boost/container/set.hpp>
#include <boost/container/vector.hpp>
#include <boost/container/map.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/unordered/unordered_set.hpp>*/
#include <boost/intrusive/treap_set.hpp>
#include <boost/intrusive/set.hpp>

#include "Sparsehash/dense_hash_map.h"
using google::dense_hash_map;
#include "Sparsehash/dense_hash_set.h"
using google::dense_hash_set;
#include "Sparsehash/sparse_hash_map.h"
using google::sparse_hash_map;
#include "Sparsehash/sparse_hash_set.h"
using google::sparse_hash_set;

#include <map>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <list>
#include <set>
#include <vector>
#include <type_traits>
#ifdef _WIN32		
  #include <direct.h>
#endif
#include <memory>
#ifdef _MSC_VER
  #include <Windows.h>
#else
#include <pthread.h>
#include <unistd.h>
//#define __forceinline inline
#endif

//---------------------------------------------------------
//	define numeric check (tr1::isnan is not available on vc++
//---------------------------------------------------------
#ifdef _MSC_VER
#define ISNAN(x) _isnan(x)
#else
#define ISNAN(x) std::isnan(x)
#endif


using namespace std;


//using namespace boost::container;
//using namespace boost::unordered;


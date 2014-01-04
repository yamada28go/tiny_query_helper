//
//--[type_debug.h]--
//

//Copyright [2013] [yamada28go]
//
//Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0

//Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#ifndef _TINY_QUERY_HELPER_TYPE_DEBUG_H_
#define _TINY_QUERY_HELPER_TYPE_DEBUG_H_

// C++標準
#include <memory>
#include <typeinfo>

//gnu関係
#include <typeinfo>
#ifdef _WIN32
#ifdef _WIN64
// 64bit Windows
#else
// 32bit Windows
#endif
#else
#ifdef __x86_64__
// 64bit Linux
#include <cxxabi.h>
#else
// 32bit Linux
#include <cxxabi.h>
#endif
#endif

//boost関係
#include <boost/format.hpp>
#include <tiny_query_helper/log.h>

namespace tiny_query_helper
{
  //debug用のツールを格納
  namespace debug
  {

    //! 型名を取得する
    template<typename DST_TYPE>
      std::string get_demangled_type_name (void)
      {
	DST_TYPE *dst = 0;
	int status;
	std::unique_ptr<char> ret
	  ( ( char * )(abi::__cxa_demangle (typeid ( *dst ).name (), 0, 0, &status) ));
	if (NULL != ret)
	  {
	    return std::string( ret.get() );
	  }
	throw std::runtime_error(" demangle was fail ");
      }

    //! 型名を表示する
    template<typename DST_TYPE>
      void
      print_type_name (void)
      {
	TINY_QUERY_HELPER_LOG( DEBUG ,
			       ( boost::format( "type is [%||]")
				 % get_demangled_type_name<DST_TYPE>() ).str() );
      }
  }
}

#endif /* _TINY_QUERY_HELPER_TYPE_DEBUG_H_ */

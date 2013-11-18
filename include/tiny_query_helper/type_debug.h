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
#include <iostream>
#include <sstream>
#include <memory>
#include <typeinfo>
#include <type_traits>

//gnu関係
#include <typeinfo>
#include <cxxabi.h>

//boost関係
#include <boost/scoped_ptr.hpp>
#include <boost/format.hpp>

#include <boost/typeof/typeof.hpp>

//boost mpl関係
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/unique.hpp>
#include <boost/mpl/sort.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/mpl/at.hpp>

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/assert.hpp>
//#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>


namespace tiny_query_helper
{
  //debug用のツールを格納
  namespace debug
  {

    //! 複雑な型の型情報を表示するデバッグマクロ
#define PRINT_TYPE_INFO(__SRC_EXPRESSIONS__)				\
    do{									\
      int status;							\
      char * ret = abi::__cxa_demangle(typeid(BOOST_TYPEOF(__SRC_EXPRESSIONS__)).name(), 0, 0, &status); \
      if(NULL != ret){							\
	std::cout << ret << std::endl;					\
      }									\
      free(ret);							\
    }while(false)

    //! 複雑な型の型情報を表示するデバッグマクロ
    //! boost autoを使って、得体の知れない型を強制的に実体化させた後に、型情報をチェックする。
#define PRINT_TYPE_INFO_BOOST(__SRC_EXPRESSIONS__)			\
    do{									\
      BOOST_AUTO(x , ( __SRC_EXPRESSIONS__ ) );				\
      int status;							\
      char * ret = abi::__cxa_demangle(typeid(BOOST_TYPEOF(x)).name(), 0, 0, &status); \
      if(NULL != ret){							\
	std::cout << ret << std::endl;					\
      }									\
      free(ret);							\
    }while(false)

    //! 型名を表示する
    template<typename DST_TYPE>
    void
    print_type_name (void)
    {
      DST_TYPE *dst = 0;
      int status;
      char * ret = abi::__cxa_demangle (typeid ( *dst).name (), 0, 0, &status);
      if (NULL != ret)
        {
          std::cout << ret << std::endl;
        }
      free (ret);
    }

    struct print_type
    {

      template<class T>
      void
      operator()(T) const
      {
        print_type_name<T>();
        std::cout << print_type_name<T>() << std::endl;
      }
    };

  }
}

#endif /* _TINY_QUERY_HELPER_TYPE_DEBUG_H_ */

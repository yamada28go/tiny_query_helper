//
//--[log.h]--
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

#ifndef TINY_QUERY_HELPER_LOG_H_
#define TINY_QUERY_HELPER_LOG_H_


// C++標準
#include <functional> 
#include <mutex>

//boost関係
#include <boost/current_function.hpp>

namespace tiny_query_helper
{

  //! ロギング用の定義を行う
  namespace log
  {

    //! ログレベルを定義する
    enum level
    {
      TRACE = 0,
      DEBUG = 1,
      INFO = 2,
      ERROR = 3
    };

#define TINY_QUERY_HELPER_LOG( __LEVEL__ , __MSG__ )			\
    do{									\
      using namespace tiny_query_helper::log;				\
      tiny_query_helper::log::logger_singleton & i =			\
      tiny_query_helper::log::logger_singleton::GetInstance();		\
      if( i.is_target( __LEVEL__ ) )					\
	{								\
	  i.write( (__LEVEL__) ,					\
		   (__FILE__) ,						\
		   (__LINE__) ,						\
		   (BOOST_CURRENT_FUNCTION),				\
		   (__MSG__)						\
		   );							\
	}								\
    }while(false)							\
     
    //loggerクラス
    class logger_singleton{
    public:

      // 唯一のアクセス経路
      static logger_singleton& GetInstance()
      {
	static logger_singleton instance;  // 唯一のインスタンス
	return instance;
      }
    
      //メンバ定義

      bool is_target( const level & lv )
      {
	if( level_ <= lv )
	  {
	    return true;
	  }
	return false;
      }
      
      //! ログを記録する
      //! この関数はスレッド安全ではない。
      //! 内部で使用されるロギング関数側でスレッド安全な実装を行う事。
      void write( const level & lv, 
		  const char * file_name,
		  const int line,
		  const char * function,
		  const std::string &  mgs )
      {
	if(logging_function_)
	  {
	    if( is_target( lv ) )
	      {
		//設定されていたら、ログを出力する
		logging_function_( file_name,
				   line,
				   function,
				   mgs );
	      }
	  }
      }
 
      //! ロギング関数を設定する
      typedef std::function< void ( const char * ,
				    const int,
				    const char * ,
				    const std::string &  ) > LOG_FUNCTION_TYPE;
      void set_logging_function( const LOG_FUNCTION_TYPE & f ) 
      {
	std::unique_lock<std::mutex> l( setter_mutex_ );
	this->logging_function_ = f;
      }

      //! ログレベルを設定する
      void set_log_level( const level & l )
      {
	std::unique_lock<std::mutex> lock( setter_mutex_ );
	this->level_ = l;
      }

    private:

      //! ロギング関係の初期値を設定するためのmutex
      std::mutex setter_mutex_;

      //! ロギング用関数ポインタ
      LOG_FUNCTION_TYPE logging_function_;

      //! 実際に適応されているログレベル
      level level_;

      // 生成やコピーを禁止する
      logger_singleton():
      logging_function_(nullptr),
      level_(TRACE)
	{}
      logger_singleton(const logger_singleton& rhs);
      logger_singleton& operator=(const logger_singleton& rhs);

    };
    
  }

}

#endif /* _TINY_QUERY_HELPER_LOG_H_ */

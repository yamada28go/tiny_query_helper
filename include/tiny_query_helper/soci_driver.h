//
//--[soci_driver.h]--
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

#ifndef _SOCI_DRIVER_H_
#define _SOCI_DRIVER_H_

//C++関係
#include <vector>
#include <iostream>
#include <stdexcept>

#include <soci.h>
#include <mysql/soci-mysql.h>
#include <tiny_query_helper/tiny_query_helper.h>


namespace tiny_query_helper
{
  namespace DBMS
  {
    namespace soci
    {

      //SOCI接続オブジェクト

      struct connector
      {
      private:

        //指定されたテーブルの全要素を取得するクエリを作成する

        template < typename Table >
                std::string get_select_all_query (const std::string & query)
        {
          std::string t = "select ";
          {
            const auto info = Table::column::get_column_info ();

            auto it = info.begin ();
            for (std::size_t i = 0; it != info.end (); it++, i++)
              {
                t.append ((*it)->name_);

                if (info.size () == (i + 1))
                  {
                    t.append (" ");
                  }
                else
                  {
                    t.append (" , ");
                  }
              }
            t.append (query);
            t.append (";");

          }

          return std::move (t);
        }

        template < typename Table >
                std::string get_all_column (void)
        {

          std::string t;
          {
            const auto info = Table::column::data::get_column_info ();
            auto it = info.begin ();
            for (std::size_t i = 0; it != info.end (); it++, i++)
              {
		TINY_QUERY_HELPER_LOG( TRACE , 
				       ( boost::format("column name is  : [ %|| ] " )
					 % (*it)->name_ ).str() );
		
                t.append (Table::get_tabel_name ());
                t.append (".");
                t.append ((*it)->name_);

                if (info.size () == (i + 1))
                  {
                    t.append (" ");
                  }
                else
                  {
                    t.append (" , ");
                  }
              }
          }

          return std::move (t);
        }


        //指定されたカラム毎に合致する型をC++側のカラムに設定する

        template < typename Table >
                void set_data_for_column (const ::soci::row & target_info, const size_t & index, Table & dst)
        {

          //型毎に仕分けしてデータ構造体に設定する
          const auto & column_info = target_info.get_properties (index);
          switch (column_info.get_data_type ())
            {
            case ::soci::dt_string:
	      TINY_QUERY_HELPER_LOG( TRACE , 
				     ( boost::format("[ %|| ],  [ %|| ] , [ %|| ] " )
				       % "string"
				       % column_info.get_name ()
				       % index ).str() );
              dst.set_member (column_info.get_name (), target_info.get<std::string>(index));
	      
              break;
            case ::soci::dt_double:
	      TINY_QUERY_HELPER_LOG( TRACE , 
				     ( boost::format("[ %|| ],  [ %|| ] , [ %|| ] " )
				       % "double"
				       % column_info.get_name () 
				       % index ).str() );
              dst.set_member (column_info.get_name (), target_info.get<double>(index));
              break;
            case ::soci::dt_integer:
	      TINY_QUERY_HELPER_LOG( TRACE , 
				     ( boost::format("[ %|| ],  [ %|| ] , [ %|| ] " )
				       % "int"
				       % column_info.get_name () 
				       % index ).str() );
              dst.set_member (column_info.get_name (), target_info.get<int>(index));
              break;
            case ::soci::dt_long_long:
	      TINY_QUERY_HELPER_LOG( TRACE , 
				     ( boost::format("[ %|| ],  [ %|| ] , [ %|| ] " )
				       % "long long "
				       % column_info.get_name () 
				       % index ).str() );
              dst.set_member (column_info.get_name (), target_info.get<long long>(index));
              break;
            case ::soci::dt_unsigned_long_long:
	      TINY_QUERY_HELPER_LOG( TRACE , 
				     ( boost::format("[ %|| ],  [ %|| ] , [ %|| ] " )
				       % "unsigned long long "
				       % column_info.get_name ()
				       % index ).str() );
              dst.set_member (column_info.get_name (), target_info.get<unsigned long long>(index));
              break;
              /* case ::soci::dt_date: */
              /* 	std::cout << "date" << std::endl; */
              /* 	std::tm when = it->get<std::tm>(i); */
              /* 	std::cout << asctime(&when); */
              /* 	break; */
	    default:
	      //Todo : add default type
	      throw std::logic_error("no type");
	      break;
            }


        }

        //! DBからデータを読み取る

        template < typename Table1 >
                std::vector< Table1 > read_from_db (const ::soci::rowset< ::soci::row > & read_row)
        {
          //関数戻り値
          std::vector< Table1 > ret;

          //std::cout << read_row.size() << std::endl;
          for (const auto & it : read_row)
            {
              //作業用一時テーブル
              Table1 t_table;
              for (std::size_t i = 0; i != it.size (); ++i)
                {
                  //メンバを設定する
                  set_data_for_column (it, i, t_table);
                }

              //配列に戻す
              ret.push_back (std::move (t_table));
            }

          return std::move (ret);
        }

      public:

        //templateでは引数のオーバーロードが出来ないため、関数名の最後に1とか2とかテーブル名を付ける
        //select句の定義

        template< typename Table1 >
                std::vector< Table1 > SELECT_ALL1 (void)
        {
          //クエリを取得する
          const auto query = (boost::format ("select %|| from %|| ;")
                              % get_all_column<Table1>()
			      % Table1::get_tabel_name()
                              ).str ();
	  TINY_QUERY_HELPER_LOG( INFO , 
				 ( boost::format("sql query is : [ %|| ] " )
				   % query ).str() ) ;

          //クエリを実行
          const ::soci::rowset< ::soci::row > rs = (sql_.prepare << query);

          //処理結果をDBから読み取る
          return read_from_db< Table1 >(rs);

        }



        template< typename Table1 >
                std::vector< Table1 > SELECT_ALL1 (from1_term< Table1 > && q)
        {
          //クエリを取得する
          const auto query = (boost::format ("select %|| from %||  %|| ")
                              % get_all_column<Table1>()
			      % Table1::get_tabel_name()
                              % q.conditions_.get_query ()
                              ).str ();
	  TINY_QUERY_HELPER_LOG( INFO , 
				 ( boost::format("sql query is : [ %|| ] " )
				   % query ).str() ) ;

          //クエリを実行
          const ::soci::rowset< ::soci::row > rs = (sql_.prepare << query);

          //処理結果をDBから読み取る
          return read_from_db< Table1 >(rs);

        }

        //select句の定義

        template< typename Table1, typename Table2 >
                std::vector < std::pair< Table1, Table2 > > SELECT_ALL2
                (from2_term< Table1, Table2 > && q)

        {

          std::vector < std::pair< Table1, Table2 > > ret;

          //クエリを取得する
          const auto query = (boost::format ("select %|| , %|| from %|| ")
                              % get_all_column<Table1>()
                              % get_all_column<Table2>()
                              % q.conditions_.get_query ()
                              ).str ();
	  TINY_QUERY_HELPER_LOG( INFO , 
				 ( boost::format("sql query is : [ %|| ] " )
				   % query ).str() ) ;

          //各テーブルのサイズを取得
          const std::size_t table_1_size = Table1::column::data::get_column_info ().size ();

          //クエリを実行
          const ::soci::rowset< ::soci::row > rs = (sql_.prepare << query);
          for (const auto & it : rs)
            {
              Table1 t1;
              Table2 t2;

              //get_propertiesで取得されるプロパティ情報では、カラム名までしか取得できない。
              //rowでは、クエリを発行した順番で取得されるため、発行順で分岐して処理する
              std::cout << it.size () << std::endl;
              for (std::size_t i = 0; i != it.size (); ++i)
                {
                  //const auto & props = it.get_properties (i);
                  if (i < table_1_size)
                    {
                      set_data_for_column (it, i, t1);
                    }
                  else
                    {
                      set_data_for_column (it, i, t2);
                    }
                  //std::cout << props.get_name () << std::endl;
                }

              ret.push_back (std::make_pair (std::move (t1), std::move (t2)));
            }

          return ret;

        }

        connector (const std::string & connectString)
        {
          sql_.open (connectString);
        }

        ~connector () { }

        ::soci::session sql_;

      };

    }
  }
}



#endif /* _SOCI_DRIVER_H_ */

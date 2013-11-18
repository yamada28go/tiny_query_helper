//
//--[query_test.cpp]--
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

#include <iostream>
#include <sstream>
#include <memory>

//include tiny_query_helper 
#include <tiny_query_helper/tiny_query_helper.h>
#include <tiny_query_helper/soci_driver.h>

//include table types 
#include "Table1.h"
#include "Table2.h"

namespace
{
  namespace tiny_query_helper_test
  {

    // table 1�̏ꍇ
    namespace from1_table
    {
      //�S�v�f��\������
      template< typename TABLE_VECTOR >
      void print_all_element( const TABLE_VECTOR & v )
      {
        for ( const auto & i : v )
          {
            std::cout << i << std::endl;
          }
      }

      void test_query( const std::string & connection_string )
      {

        //MySQL�̎g�p���J�n
        //(�w�肵�Ă����Ȃ��ƁA���O�Ƀh���C�o���擾���鎖���o���Ȃ��B)
        soci::register_factory_mysql ();

        using namespace tiny_query_helper;

        DBMS::soci::connector q (connection_string);

      
        //�ڍ׎w�薳���őS�v�f���擾
        // {
        //         const auto query_ret = q.SELECT_ALL1< hoge::Table1 >
        //           (
        //            FROM1< hoge::Table1 >()
        //            );
        
        //         print_all_element( query_ret );
        // }

        //where�������w��
        {
          const auto query_ret = q.SELECT_ALL1< hoge::Table1 >
            (
             FROM1< hoge::Table1 >( where( hoge::Table1::column::id == 1)  )
             );
        
          print_all_element( query_ret );
        }


        //where�������w��
        {
          const auto query_ret = q.SELECT_ALL1< hoge::Table1 >
            (
             FROM1< hoge::Table1 >
             (
              where( ( (hoge::Table1::column::id == 1) && ( hoge::Table1::column::data2_string == "test1") ) )
              ) );
        
          print_all_element( query_ret );
        }


        //where�������w��
        // {
        //   const auto query_ret = q.SELECT_ALL1< hoge::Table1 >
        //     (
        //      FROM1< hoge::Table1 >
        //      (
        //       where( ( hoge::Table1::column::data2_string == "test1") )
        //       .order_by < order::desc< typename hoge::Table1::column::data1_int > >()
        //       )
        //      );
        //   print_all_element( query_ret );
        // }


        //where�������w��
        {
          const auto query_ret = q.SELECT_ALL1< hoge::Table1 >
            (
             FROM1< hoge::Table1 >
             (
              where( ( (hoge::Table1::column::id >= 1) && ( hoge::Table1::column::data2_string == "test1") ) )
              .group_by< typename hoge::Table1::column::data2_string >()
              .order_by < order::desc< typename hoge::Table1::column::data1_int > >()
              )
             );
          print_all_element( query_ret );
        }
      }

    }


    // table 2�̏ꍇ
    namespace from2_table
    {

      //�S�v�f��\������
      template< typename TABLE_VECTOR >
      void print_all_element( const TABLE_VECTOR & v )
      {
        for ( const auto & i : v )
          {
            std::cout << i.first << " : " << i.second << std::endl;
          }
      }
      

      void test_query( const std::string & connection_string )
      {
        //MySQL�̎g�p���J�n
        //(�w�肵�Ă����Ȃ��ƁA���O�Ƀh���C�o���擾���鎖���o���Ȃ��B)
        soci::register_factory_mysql ();

        using namespace tiny_query_helper;

        DBMS::soci::connector q (connection_string);

        const std::vector< std::pair< hoge::Table1, hoge::Table2 > > query_ret =
          q.SELECT_ALL2 < hoge::Table1, hoge::Table2 >
          ( FROM2< hoge::Table1, hoge::Table2 >
            (inner_join2
             < hoge::Table1, hoge::Table2 ,
             condition::equal<
             typename hoge::Table1::column::id,
             typename hoge::Table2::column::id > >()
             .where ( (hoge::Table1::column::id >= 1) && (hoge::Table2::column::id >= 1)  )
             // .group_by< typename hoge::Table2::column::id > ()
             // .order_by< order::desc< typename hoge::Table2::column::id > >()
             )
            );

        print_all_element( query_ret );

      }
    }

  }
}

int
main()
{
  const auto connection_string 
    = "mysql://dbname=tiny_query_helper_test user=root password=*hoge* host=*hoge*";

  //�Ώۃe�[�u��1�̏ꍇ
  tiny_query_helper_test::from1_table::test_query( connection_string );

  //�Ώۃe�[�u��2�̏ꍇ
  tiny_query_helper_test::from2_table::test_query( connection_string );

}
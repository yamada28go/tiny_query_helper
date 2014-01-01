#define BOOST_TEST_MAIN    // main関数を定義
#define BOOST_TEST_DYN_LINK

//#include <boost/test/included/unit_test.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <sstream>
#include <memory>

//include tiny_query_helper 
#include <tiny_query_helper/tiny_query_helper.h>
#include <tiny_query_helper/soci_driver.h>

//include table types 
#include "Table1.h"
#include "Table2.h"

struct Fixture {

  Fixture()
  {
    //MySQLの使用を開始
    //(指定しておかないと、事前にドライバを取得する事が出来ない。)
    soci::register_factory_mysql ();
  }

};
BOOST_GLOBAL_FIXTURE(Fixture);

//Test table connection string
const auto connection_string 
= "mysql://dbname=tiny_query_helper_test user=root password=modamepo host=192.168.1.41";

BOOST_AUTO_TEST_SUITE( one_table_test )

BOOST_AUTO_TEST_CASE( table1 )
{
  using namespace tiny_query_helper;

  DBMS::soci::connector q (connection_string);

  //where条件を指定
  const auto query_ret = q.SELECT_ALL1< hoge::Table1 >();
  
  BOOST_REQUIRE_EQUAL(3, query_ret.size() );

  //戻り値の順序は実装依存なのでfind_ifで探す
  {
    const auto t = std::find_if
      ( query_ret.begin() ,
	query_ret.end() ,
	[]( const hoge::Table1 &t ) -> bool
	{ return t.id_ == 1 ; } );
    BOOST_REQUIRE_EQUAL( false , ( t == query_ret.end() ) );
    BOOST_CHECK_EQUAL(1, t->id_ );
    BOOST_CHECK_EQUAL(3, t->data1_int_ );
    BOOST_CHECK_EQUAL("test1", t->data2_string_ );
  }

  {
    const auto t = std::find_if
      ( query_ret.begin() ,
	query_ret.end() ,
	[]( const hoge::Table1 &t ) -> bool
	{ return t.id_ == 2 ; } );
    BOOST_REQUIRE_EQUAL( false , ( t == query_ret.end() ) );
    BOOST_CHECK_EQUAL(2, t->id_ );
    BOOST_CHECK_EQUAL(2, t->data1_int_ );
    BOOST_CHECK_EQUAL("test1", t->data2_string_ );
  }

  {
    const auto t = std::find_if
      ( query_ret.begin() ,
	query_ret.end() ,
	[]( const hoge::Table1 &t ) -> bool
	{ return t.id_ == 3 ; } );
    BOOST_REQUIRE_EQUAL( false , ( t == query_ret.end() ) );
    BOOST_CHECK_EQUAL(3, t->id_ );
    BOOST_CHECK_EQUAL(1, t->data1_int_ );
    BOOST_CHECK_EQUAL("test2", t->data2_string_ );
  }

}

BOOST_AUTO_TEST_CASE( table1_order_by_onry )
{
  using namespace tiny_query_helper;

  DBMS::soci::connector q (connection_string);
  //整列条件だけを指定を指定
  const auto query_ret = q.SELECT_ALL1< hoge::Table1 >
    ( FROM1< hoge::Table1 >
      ( order_by< order::desc< typename hoge::Table1::column::type::data1_int > >() ) );

  BOOST_REQUIRE_EQUAL(3, query_ret.size() );
  auto t = query_ret.begin();

  BOOST_CHECK_EQUAL(1, t->id_ );
  BOOST_CHECK_EQUAL(3, t->data1_int_ );
  BOOST_CHECK_EQUAL("test1", t->data2_string_ );

  t++;

  BOOST_CHECK_EQUAL(2, t->id_ );
  BOOST_CHECK_EQUAL(2, t->data1_int_ );
  BOOST_CHECK_EQUAL("test1", t->data2_string_ );

  t++;

  BOOST_CHECK_EQUAL(3, t->id_ );
  BOOST_CHECK_EQUAL(1, t->data1_int_ );
  BOOST_CHECK_EQUAL("test2", t->data2_string_ );

}

BOOST_AUTO_TEST_CASE( table1_where )
{
  using namespace tiny_query_helper;

  DBMS::soci::connector q (connection_string);

  //where条件を指定
  const auto query_ret = q.SELECT_ALL1< hoge::Table1 >
    ( FROM1< hoge::Table1 >( where( hoge::Table1::column::data::id == 1) ) );
  
  BOOST_REQUIRE_EQUAL(1, query_ret.size() );
  const auto &t = *query_ret.begin();

  BOOST_CHECK_EQUAL(1, t.id_ );
  BOOST_CHECK_EQUAL(3, t.data1_int_ );
  BOOST_CHECK_EQUAL("test1", t.data2_string_ );
}

BOOST_AUTO_TEST_CASE( table1_where_and )
{
  using namespace tiny_query_helper;

  DBMS::soci::connector q (connection_string);

  //where条件を指定
  const auto query_ret = q.SELECT_ALL1< hoge::Table1 >
    (
     FROM1< hoge::Table1 >
     (
      where( ( (hoge::Table1::column::data::data1_int == 2 ) && 
	       ( hoge::Table1::column::data::data2_string == "test1") ) )
      )
     );
  
  BOOST_REQUIRE_EQUAL(1, query_ret.size() );
  const auto &t = *query_ret.begin();

  BOOST_CHECK_EQUAL(2, t.id_ );
  BOOST_CHECK_EQUAL(2, t.data1_int_ );
  BOOST_CHECK_EQUAL("test1", t.data2_string_ );
}


BOOST_AUTO_TEST_CASE( table1_order_by )
{
  using namespace tiny_query_helper;

  DBMS::soci::connector q (connection_string);

  const auto query_ret = q.SELECT_ALL1< hoge::Table1 >
    (
     FROM1< hoge::Table1 >
     (
      where( hoge::Table1::column::data::data2_string == "test1" )
      .order_by < order::desc< typename hoge::Table1::column::type::data1_int > >()
      )
     );
  
  BOOST_REQUIRE_EQUAL(2, query_ret.size() );

  //戻り値の順序は実装依存なのでfind_ifで探す
  {
    const auto t = std::find_if
      ( query_ret.begin() ,
	query_ret.end() ,
	[]( const hoge::Table1 &t ) -> bool
	{ return t.id_ == 1 ; } );
    BOOST_REQUIRE_EQUAL( false , ( t == query_ret.end() ) );
    BOOST_CHECK_EQUAL(1, t->id_ );
    BOOST_CHECK_EQUAL(3, t->data1_int_ );
    BOOST_CHECK_EQUAL("test1", t->data2_string_ );
  }

  {
    const auto t = std::find_if
      ( query_ret.begin() ,
	query_ret.end() ,
	[]( const hoge::Table1 &t ) -> bool
	{ return t.id_ == 2 ; } );
    BOOST_REQUIRE_EQUAL( false , ( t == query_ret.end() ) );
    BOOST_CHECK_EQUAL(2, t->id_ );
    BOOST_CHECK_EQUAL(2, t->data1_int_ );
    BOOST_CHECK_EQUAL("test1", t->data2_string_ );
  
  }
}

BOOST_AUTO_TEST_CASE( table1_group_by )
{
  using namespace tiny_query_helper;

  DBMS::soci::connector q (connection_string);

  const auto query_ret = q.SELECT_ALL1< hoge::Table1 >
    (
     FROM1< hoge::Table1 >
     (
      where( hoge::Table1::column::data::data2_string == "test1" )
      .group_by< typename hoge::Table1::column::type::data2_string >()
      )
     );
  
  BOOST_REQUIRE_EQUAL(1, query_ret.size() );
  auto it = query_ret.begin();
  BOOST_CHECK_EQUAL(1, it->id_ );
  BOOST_CHECK_EQUAL(3, it->data1_int_ );
  BOOST_CHECK_EQUAL("test1", it->data2_string_ );

}

BOOST_AUTO_TEST_CASE( table1_group_by_order_by )
{
  using namespace tiny_query_helper;

  DBMS::soci::connector q (connection_string);

  const auto query_ret = q.SELECT_ALL1< hoge::Table1 >
    (
     FROM1< hoge::Table1 >
     (
      where( hoge::Table1::column::data::data2_string == "test1" )
      .group_by< typename hoge::Table1::column::type::data2_string >()
      .order_by < order::desc< typename hoge::Table1::column::type::data1_int > >()
      )
     );
  
  BOOST_REQUIRE_EQUAL(1, query_ret.size() );
  auto it = query_ret.begin();
  BOOST_CHECK_EQUAL(1, it->id_ );
  BOOST_CHECK_EQUAL(3, it->data1_int_ );
  BOOST_CHECK_EQUAL("test1", it->data2_string_ );

}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( two_table_test )

BOOST_AUTO_TEST_CASE( table2_inner_join )
{
  using namespace tiny_query_helper;

  DBMS::soci::connector q (connection_string);

  const std::vector< std::pair< hoge::Table1, hoge::Table2 > > query_ret =
    q.SELECT_ALL2 < hoge::Table1, hoge::Table2 >
    ( FROM2< hoge::Table1, hoge::Table2 >
      (inner_join2
       < hoge::Table1, hoge::Table2 ,
       condition::equal<
       typename hoge::Table1::column::type::id,
       typename hoge::Table2::column::type::id > >()
       .where ( ( hoge::Table1::column::data::id > 1 ) && ( hoge::Table2::column::data::id > 1 )  )
       )
      );
  
  BOOST_REQUIRE_EQUAL(2, query_ret.size() );

  //戻り値の順序は実装依存なのでfind_ifで探す
  {
    const auto t = std::find_if
      ( query_ret.begin() ,
	query_ret.end() ,
	[]( const std::pair< hoge::Table1, hoge::Table2 > &t ) -> bool
	{ return ( t.first.id_ == 2 ) && ( t.second.id_ == 2)  ; } );
    BOOST_REQUIRE_EQUAL( false , ( t == query_ret.end() ) );

    BOOST_CHECK_EQUAL(2, t->first.id_ );
    BOOST_CHECK_EQUAL(2, t->first.data1_int_ );
    BOOST_CHECK_EQUAL("test1", t->first.data2_string_ );
    
    BOOST_CHECK_EQUAL(2, t->second.id_ );
    BOOST_CHECK_EQUAL(100, t->second.data1_int_ );
    BOOST_CHECK_EQUAL("test2", t->second.data2_string_ );
  }

  {
    const auto t = std::find_if
      ( query_ret.begin() ,
	query_ret.end() ,
	[]( const std::pair< hoge::Table1, hoge::Table2 > &t ) -> bool
	{ return ( t.first.id_ == 3 ) && ( t.second.id_ == 3)  ; } );
    BOOST_REQUIRE_EQUAL( false , ( t == query_ret.end() ) );


    BOOST_CHECK_EQUAL(3, t->first.id_ );
    BOOST_CHECK_EQUAL(1, t->first.data1_int_ );
    BOOST_CHECK_EQUAL("test2", t->first.data2_string_ );
    
    BOOST_CHECK_EQUAL(3, t->second.id_ );
    BOOST_CHECK_EQUAL(1000, t->second.data1_int_ );
    BOOST_CHECK_EQUAL("test2", t->second.data2_string_ );

  }

}

BOOST_AUTO_TEST_CASE( table2_inner_join_where )
{
  using namespace tiny_query_helper;

#if 0

  {
    const auto t = ( ( hoge::Table1::column::data::id > 1 ) && ( hoge::Table2::column::data::id > 1 ) );
    tiny_query_helper::debug::print_type_name< decltype(t) >();
  }

  {
    const auto t = ( hoge::Table1::column::data::id > 1 );
    tiny_query_helper::debug::print_type_name< decltype(t) >();
  }

  {
    const auto t = (  hoge::Table2::column::data::id > 1  );
    tiny_query_helper::debug::print_type_name< decltype(t) >();
  }

  // {
  //   const auto t = (  inner_join2
  //      < hoge::Table1, hoge::Table2 ,
  //      condition::equal<
  //      hoge::Table1::column::type::id,
  //      hoge::Table2::column::type::id > >()  );
  //   tiny_query_helper::debug::print_type_name< decltype(t)::TABLE_TYPE >();
  // }
#endif

#if 1

  {
    const auto t = (  inner_join2
       < hoge::Table1, hoge::Table2 ,
       condition::equal<
       hoge::Table1::column::type::id,
       hoge::Table2::column::type::id > >()  );
    tiny_query_helper::debug::print_type_name< decltype(t)::TABLE_TYPE >();
  }

  DBMS::soci::connector q (connection_string);

  const std::vector< std::pair< hoge::Table1, hoge::Table2 > > query_ret =
    q.SELECT_ALL2 < hoge::Table1, hoge::Table2 >
    ( FROM2< hoge::Table1, hoge::Table2 >
      (inner_join2
       < hoge::Table1, hoge::Table2 ,
       condition::equal<
       hoge::Table1::column::type::id,
       hoge::Table2::column::type::id > >()
       .where ( ( hoge::Table1::column::data::id > 1 ) )
       .group_by< hoge::Table1::column::type::id > ()
       )
      );

  BOOST_REQUIRE_EQUAL(2, query_ret.size() );
  const auto it = query_ret.begin();
  BOOST_CHECK_EQUAL(2, it->first.id_ );
  BOOST_CHECK_EQUAL(2, it->first.data1_int_ );
  BOOST_CHECK_EQUAL("test1", it->first.data2_string_ );

  BOOST_CHECK_EQUAL(2, it->second.id_ );
  BOOST_CHECK_EQUAL(100, it->second.data1_int_ );
  BOOST_CHECK_EQUAL("test2", it->second.data2_string_ );

#endif

}


BOOST_AUTO_TEST_CASE( table2_inner_join_group_by )
{
  using namespace tiny_query_helper;

  DBMS::soci::connector q (connection_string);

  const std::vector< std::pair< hoge::Table1, hoge::Table2 > > query_ret =
    q.SELECT_ALL2 < hoge::Table1, hoge::Table2 >
    ( FROM2< hoge::Table1, hoge::Table2 >
      (inner_join2
       < hoge::Table1, hoge::Table2 ,
       condition::equal<
       hoge::Table1::column::type::id,
       hoge::Table2::column::type::id > >()
       .where ( ( hoge::Table1::column::data::id > 1 ) && ( hoge::Table2::column::data::id > 1 )  )
       .group_by< typename hoge::Table2::column::type::id > ()
       )
      );

  BOOST_REQUIRE_EQUAL(2, query_ret.size() );
  const auto it = query_ret.begin();
  BOOST_CHECK_EQUAL(2, it->first.id_ );
  BOOST_CHECK_EQUAL(2, it->first.data1_int_ );
  BOOST_CHECK_EQUAL("test1", it->first.data2_string_ );

  BOOST_CHECK_EQUAL(2, it->second.id_ );
  BOOST_CHECK_EQUAL(100, it->second.data1_int_ );
  BOOST_CHECK_EQUAL("test2", it->second.data2_string_ );

}

BOOST_AUTO_TEST_CASE( table2_inner_join_order_by )
{
  using namespace tiny_query_helper;

  DBMS::soci::connector q (connection_string);

  const std::vector< std::pair< hoge::Table1, hoge::Table2 > > query_ret =
    q.SELECT_ALL2 < hoge::Table1, hoge::Table2 >
    ( FROM2< hoge::Table1, hoge::Table2 >
      (inner_join2
       < hoge::Table1, hoge::Table2 ,
       condition::equal<
       hoge::Table1::column::type::id,
       hoge::Table2::column::type::id > >()
       .where ( ( hoge::Table1::column::data::id > 1 ) && ( hoge::Table2::column::data::id > 1 )  )
       .order_by< order::desc< typename hoge::Table2::column::type::id > >()
       )
      );

  BOOST_REQUIRE_EQUAL(2, query_ret.size() );
  auto it = query_ret.begin();
  BOOST_CHECK_EQUAL(3, it->first.id_ );
  BOOST_CHECK_EQUAL(1, it->first.data1_int_ );
  BOOST_CHECK_EQUAL("test2", it->first.data2_string_ );

  BOOST_CHECK_EQUAL(3, it->second.id_ );
  BOOST_CHECK_EQUAL(1000, it->second.data1_int_ );
  BOOST_CHECK_EQUAL("test2", it->second.data2_string_ );

  it++;

  BOOST_CHECK_EQUAL(2, it->first.id_ );
  BOOST_CHECK_EQUAL(2, it->first.data1_int_ );
  BOOST_CHECK_EQUAL("test1", it->first.data2_string_ );

  BOOST_CHECK_EQUAL(2, it->second.id_ );
  BOOST_CHECK_EQUAL(100, it->second.data1_int_ );
  BOOST_CHECK_EQUAL("test2", it->second.data2_string_ );

}

BOOST_AUTO_TEST_CASE( table2_inner_join_order_by_group_by )
{
  using namespace tiny_query_helper;

  DBMS::soci::connector q (connection_string);

  const std::vector< std::pair< hoge::Table1, hoge::Table2 > > query_ret =
    q.SELECT_ALL2 < hoge::Table1, hoge::Table2 >
    ( FROM2< hoge::Table1, hoge::Table2 >
      (inner_join2
       < hoge::Table1, hoge::Table2 ,
       condition::equal<
       typename hoge::Table1::column::type::id,
       typename hoge::Table2::column::type::id > >()
       .where ( ( hoge::Table1::column::data::id > 1 ) && ( hoge::Table2::column::data::id > 1 )  )
       .group_by< typename hoge::Table2::column::type::id > ()
       .order_by< order::asc< typename hoge::Table2::column::type::id > >()
       )
      );

  BOOST_REQUIRE_EQUAL(2, query_ret.size() );
  auto it = query_ret.begin();

  BOOST_CHECK_EQUAL(2, it->first.id_ );
  BOOST_CHECK_EQUAL(2, it->first.data1_int_ );
  BOOST_CHECK_EQUAL("test1", it->first.data2_string_ );

  BOOST_CHECK_EQUAL(2, it->second.id_ );
  BOOST_CHECK_EQUAL(100, it->second.data1_int_ );
  BOOST_CHECK_EQUAL("test2", it->second.data2_string_ );

  it++;

  BOOST_CHECK_EQUAL(3, it->first.id_ );
  BOOST_CHECK_EQUAL(1, it->first.data1_int_ );
  BOOST_CHECK_EQUAL("test2", it->first.data2_string_ );

  BOOST_CHECK_EQUAL(3, it->second.id_ );
  BOOST_CHECK_EQUAL(1000, it->second.data1_int_ );
  BOOST_CHECK_EQUAL("test2", it->second.data2_string_ );

}

BOOST_AUTO_TEST_SUITE_END()




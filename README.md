tiny_query_helper
=================

This is  C++ library to assist the generation of sql query.

Simple sample are as follows.

 DBMS::soci::connector q (" *** DBMS connection string ***");
 
 const std::vector< std::pair< table_test_1, new_table > > ret =
 q.SELECT_ALL2 <table_test_1, new_table>
 (inner_join2
  < new_table, table_test_1,
  condition::equal<
  typename new_table::column::hoge1,
  typename table_test_1::column::string_condition > >().
  where (
         (new_table::column::hoge >= 1) &&
         (table_test_1::column::string_condition == "OK")
         )
  .group_by< typename new_table::column::hoge, typename new_table::column::hoge1 > ()
  .order_by < order::desc< typename new_table::column::hoge > >());

  for (const auto & i : ret)
  {
     std::cout << i.first << " : " << i.second << std::endl;
  }

*** Motivation ***

sql is very powerful language.
But it evaluate in running time.
So error check is the same time.
We can not detect the typo until it running.

c++ is static type language.
c++ can error  check in  the compile time.
i like c++ style error  check .

When use sql query embedded　in  the c++ language ,  
we must do  twice　error  check.
The first  is compile time  and other check is running  time.  
I want to check the sql query and c++ language  at the same time  in compile time.

tiny_query_helper is library  to assist  the generation of sql query embedded　in  the c++ language .
Design policy is make  sql query mapping to the c++ 's type .
In order to realize the goal , we use template metaprogramming and external program.

*** Ststus ***

These are just proto type code. 


*** Dependency ***

・Boost library
・SOCI - The C++ Database Access Library 


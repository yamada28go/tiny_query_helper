//include files
#include "Table1.h"

namespace  hoge{

/*
 * This class is generated automatically.
 * Never change from your hand.
 */
/**
 * <pre>
 * schema name : "tiny_query_helper_test"
 * table name  : "table1"
 * remarks     : ""
 *
 * Persistable:
 *  Available to use Persistor-methods, like DBManager\#insert(IPersistable),\#update(IPersistable),\#delete(IPersistable),else.
 *
 * </pre>
 */
 
 
 //実体を定義

//カラム情報オブジェクトを定義
const std::string Table1::column::id::name_ ("id");
const std::string Table1::column::data1_int::name_ ("data1_int");
const std::string Table1::column::data2_string::name_ ("data2_string");
 
 
 //カラム情報の実態を定義
const typename Table1::column::id Table1::column::id;
const typename Table1::column::data1_int Table1::column::data1_int;
const typename Table1::column::data2_string Table1::column::data2_string;

 
}


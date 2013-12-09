//include files
#include "Table2.h"

namespace  hoge{

/*
 * This class is generated automatically.
 * Never change from your hand.
 */
/**
 * <pre>
 * schema name : "tiny_query_helper_test"
 * table name  : "table2"
 * remarks     : ""
 *
 * Persistable:
 *  Available to use Persistor-methods, like DBManager\#insert(IPersistable),\#update(IPersistable),\#delete(IPersistable),else.
 *
 * </pre>
 */
 
 
 //実体を定義

//カラム情報オブジェクトを定義
const std::string Table2::column::id::name_ ("id");
const std::string Table2::column::data1_int::name_ ("data1_int");
const std::string Table2::column::data2_string::name_ ("data2_string");
 
 
 //カラム情報の実態を定義
const typename Table2::column::id Table2::column::id;
const typename Table2::column::data1_int Table2::column::data1_int;
const typename Table2::column::data2_string Table2::column::data2_string;

 
}


//include files
#include <cstdint>
#include <stdexcept>
#include <array>
#include <tiny_query_helper/extension_switch.h>
#include <tiny_query_helper/tiny_query_helper.h>
#include <tiny_query_helper/soci_driver.h>

namespace hoge{ 

/*
 * This class is generated automatically.
 * Never change from your hand.
 */
/**
 * <pre>
 * schema name : "tiny_query_helper_test"
 * table name  : "table2"
 * </pre>
 */
struct Table2 : public  tiny_query_helper::dto_base {


  //! テーブル名を取得する
  static const std::string get_tabel_name(void)
  {
  		return "table2";
  }


//! カラム情報取得用構造体
  struct column
  {

            //!テーブルの型情報を定義する構造体
            struct type {
	
		/**
	 * <pre>
	 * column name   : "id"
	 *
	 * primary key   : YES
	 * auto-increment
	 * nullable      : NO
	 * data-type     : 4 (*java.sql.Types)
	 * type-name     : INT
	 * size          : 10
	 * default value : ""
	 * </pre>
	 */
	  
	  	struct id : public tiny_query_helper::column_type::wrap_int32_t< Table2 >
    {
      id (void) :
      tiny_query_helper::column_type::wrap_int32_t< Table2 >("id") { }
      static const std::string name_;
    };
  
	/**
	 * <pre>
	 * column name   : "data1_int"
	 *
	 * nullable      : NO
	 * data-type     : 4 (*java.sql.Types)
	 * type-name     : INT
	 * size          : 10
	 * default value : ""
	 * </pre>
	 */
	  
	  	struct data1_int : public tiny_query_helper::column_type::wrap_int32_t< Table2 >
    {
      data1_int (void) :
      tiny_query_helper::column_type::wrap_int32_t< Table2 >("data1_int") { }
      static const std::string name_;
    };
  
	/**
	 * <pre>
	 * column name   : "data2_string"
	 *
	 * nullable      : NO
	 * data-type     : 12 (*java.sql.Types)
	 * type-name     : VARCHAR
	 * size          : 45
	 * default value : ""
	 * </pre>
	 */
	  
	  	struct data2_string : public tiny_query_helper::column_type::wrap_string< Table2 >
    {
      data2_string (void) :
      tiny_query_helper::column_type::wrap_string< Table2 >("data2_string") { }
      static const std::string name_;
    };
  

};

	  struct data{

            //! tiny query helperが必要なテーブル情報一覧を作成する
            	            	const static ::hoge::Table2::column::type::id id;
            	            	const static ::hoge::Table2::column::type::data1_int data1_int;
            	            	const static ::hoge::Table2::column::type::data2_string data2_string;
            	            

    //! カラム情報を取得する
    typedef std::array< const tiny_query_helper::where_condition_base * , 3 > COLUMN_INFO_TYPE;
    static const COLUMN_INFO_TYPE get_column_info( void )
    {
    
        COLUMN_INFO_TYPE r;
        		r[ 0 ] = &id;
				r[ 1 ] = &data1_int;
				r[ 2 ] = &data2_string;
				
		
      return std::move(r);
    }
    
    };

    column(void)
    {
    }


};

    //テーブル識別ID
    typedef boost::mpl::int_<2> TABLE_ID;

	//! ------------------------------------
	//! メンバ関数の実態を定義する
	//! ------------------------------------

		/**
	 * <pre>
	 * column name   : "id"
	 *
	 * primary key   : YES
	 * auto-increment
	 * nullable      : NO
	 * data-type     : 4 (*java.sql.Types)
	 * type-name     : INT
	 * size          : 10
	 * default value : ""
	 * </pre>
	 */
	 
	  int32_t id_;
	  
	/**
	 * <pre>
	 * column name   : "data1_int"
	 *
	 * nullable      : NO
	 * data-type     : 4 (*java.sql.Types)
	 * type-name     : INT
	 * size          : 10
	 * default value : ""
	 * </pre>
	 */
	 
	  int32_t data1_int_;
	  
	/**
	 * <pre>
	 * column name   : "data2_string"
	 *
	 * nullable      : NO
	 * data-type     : 12 (*java.sql.Types)
	 * type-name     : VARCHAR
	 * size          : 45
	 * default value : ""
	 * </pre>
	 */
	 
	  std::string data2_string_;
	  


    private:

        //DBアクセスオブジェクト
        friend class tiny_query_helper::DBMS::soci::connector;

        //! 挿入クエリ生成関数
        std::string get_insert_string( void ) const
          {
          
          const auto insert_string = 
R"delimiter(
INSERT INTO table2 (

		id
		,
			data1_int
		,
			data2_string
		)
			
	VALUES
(
		` %|| `
		,
			` %|| `
		,
			` %|| `
		)
			;
)delimiter";
          
          
          
            return std::move
              (
               ( boost::format
                 ( insert_string )
                  	                 	%id_
					                 	%data1_int_
					                 	%data2_string_
					                  ).str() );
          }


	//! ------------------------------------
	//! メンバ情報定義メソッド
	//! ------------------------------------

      //! メンバに値を設定する
    template <typename T >
    void set_member( const std::string & name , const T & ref )
    {

  	  	  if( "id" == name )
    {
      if(true==set_if_same_type( ref , id_ ) )
        {
         return;
          }
    }
      	  if( "data1_int" == name )
    {
      if(true==set_if_same_type( ref , data1_int_ ) )
        {
         return;
          }
    }
      	  if( "data2_string" == name )
    {
      if(true==set_if_same_type( ref , data2_string_ ) )
        {
         return;
          }
    }
            
      //ここまで来たらデータの取り込みに失敗している。
      throw std::logic_error( "set data faild" );


    }
};


// 出力用の<<演算子定義
inline std::ostream& operator<<( std::ostream& os, const Table2 & t )
{

  os << "type : Table2 [  ";
  
  	  os << "," << "id : " << t.id_ ;
        os << "," << "data1_int : " << t.data1_int_ ;
        os << "," << "data2_string : " << t.data2_string_ ;
        

  os << " ] ";
  return os;
}

}
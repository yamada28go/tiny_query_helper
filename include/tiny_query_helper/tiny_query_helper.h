//
//--[tiny_query_helper.h]--
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

#ifndef TINY_QUERY_HELPER_H_
#define TINY_QUERY_HELPER_H_


// C++標準
#include <iostream>
#include <sstream>
#include <memory>
#include <typeinfo>
#include <type_traits>

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
#include <boost/type_traits.hpp>

#include <boost/typeof/std/string.hpp>

//スイッチ拡張
#include <tiny_query_helper/extension_switch.h>
#include <tiny_query_helper/type_debug.h>


//! ---------[設定]---------
#define NUM_TABLE_TUPLE_IS_5
//! ------------------------

namespace tiny_query_helper
{
	//mpl関係のツールが格納された名前空間
	namespace mpl_util
	{

		//指定されたテーブルベクタの重複を落とす

		template<typename mpl_vector>
		struct sort_and_uniq_table_vector
		{
			//この構造体は上から順番に実行される処理が記述されているものと想定する
		private:

			//table型比較用メタ関数

			template<class A, class B>
			struct table_size_less : boost::mpl::bool_ <
				((A::TABLE_ID::value) < (B::TABLE_ID::value))>
			{
			};

			//ソート
			typedef typename boost::mpl::sort<
				typename mpl_vector::type,
				table_size_less<boost::mpl::placeholders::_1,
				boost::mpl::placeholders::_2> >::type sorted;
			//重複を落とす
			typedef typename boost::mpl::unique<
				typename sorted::type,
				boost::is_same<boost::mpl::placeholders::_1,
				boost::mpl::placeholders::_2> >::type ret;

		public:

			//戻り値
			typedef ret type;

		};

		//2つのベクタをマージしてマージ結果に対して、指定の処理を実行する

		template<typename T1, typename T2>
		struct merge_vector
		{
		private:
			//マージ結果ベクタを生成する
			typedef typename boost::mpl::copy<T1, boost::mpl::back_inserter<T2> >::type merged_vector;

		public:
			//関数戻り値
			typedef merged_vector type;

		};

#ifndef NUM_TABLE_TUPLE_IS_5
#error "This code was setted for table tuple 5."
#else


		//!テーブル型情報タプル

		template<typename T1, 
			typename T2 = boost::mpl::void_,
			typename T3 = boost::mpl::void_,
			typename T4 = boost::mpl::void_,
			typename T5 = boost::mpl::void_>
		struct table_type_tuple
		{
			typedef T1 T1_TYPE;
			typedef T2 T2_TYPE;
			typedef T3 T3_TYPE;
			typedef T4 T4_TYPE;
			typedef T5 T5_TYPE;
		};

		//! vector型をタプル型へ変換する
		//! * memo *
		//! mpl::vectorは要素数によって異なる型に変換される。
		//! このため、関数引数等で型を使用する場合、要素数固定のtuple型に変換する必要が有る。

		template<typename __VECTOR_TYPE__>
		struct vector_2_tuple
		{
		private:

			//! 指定位置の要素を取得する

			template<int POS>
			struct at : public boost::mpl::at<__VECTOR_TYPE__,
				boost::mpl::int_<POS> >
			{
			};

		public:

			typedef table_type_tuple<typename at<0>::type, typename at<1>::type,
				typename at<2>::type, typename at<3>::type, typename at<4>::type> type;

		};
#endif

	}

	// order by で指定する順序
	namespace order
	{

		//desc順で指定

		template<typename COLUMN_INFO_TYPE_ >
		struct desc
		{
			typedef COLUMN_INFO_TYPE_ COLUMN_INFO_TYPE;

			static const std::string get_order(void)
			{
				return " desc ";
			}
		};

		//asc順で指定

		template<typename COLUMN_INFO_TYPE_ >
		struct asc
		{
			typedef COLUMN_INFO_TYPE_ COLUMN_INFO_TYPE;

			static const std::string get_order(void)
			{
				return " asc ";
			}
		};

	}

	//-----------------
	//使用する型の前方宣言
	//-----------------

	template<typename TABLE_TYPE> struct where_condition_list;
	template<typename TABLE_TYPE_ > struct query_object;

	template<typename TABLE_TYPE_ > struct query_object_order_by;
	template<typename TABLE_TYPE_ > struct query_object_group_by;
	template<typename TABLE_TYPE_ > struct query_object_where;


	//-----------------
	//条件式一覧
	//-----------------

	struct order_by_term
	{
		std::vector< std::string > term_;

		bool defined(void) const
		{
			return !term_.empty();
		}
	};

	struct group_by_term
	{
		std::vector< std::string > term_;

		bool defined(void) const
		{
			return !term_.empty();
		}
	};

	struct where_term
	{
		std::string term_;

		bool defined(void) const
		{
			return !term_.empty();
		}
	};

	struct inner_join_term
	{
		std::string table1_;
		std::string table2_;
		std::string term_;

		bool defined(void) const
		{
			if ((false == table1_.empty()) &&
				(false == table2_.empty()) &&
				(false == term_.empty())
				)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	};

	//クエリオブジェクト

	template<typename TABLE_TYPE_ >
	struct query_object
	{
		//! 各要素毎のクエリ文
		order_by_term order_by_;
		group_by_term group_by_;
		where_term where_;
		inner_join_term inner_join_;

		typedef TABLE_TYPE_ TABLE_TYPE;

		query_object(const std::string & q) :
			query_(q) { }

		query_object(void) :
			query_("") { }

		~query_object(void) { }

		//! SQLクエリ文を生成する

		std::string get_query(void) const
		{
			std::string ret;

			// inner join句を生成する
			if (true == inner_join_.defined())
			{
				ret.append((boost::format(" %|| inner join %|| on %||")
					% inner_join_.table1_
					% inner_join_.table2_
					% inner_join_.term_
					).str());
			}

			//where句を生成する
			if (true == where_.defined())
			{
				ret.append(" where " + where_.term_ + " ");
			}

			// group by句を生成する
			if (true == group_by_.defined())
			{
				ret.append(" group by ");

				size_t i = 0;
				for (const auto & ref : group_by_.term_)
				{
					i++;
					if (group_by_.term_.size() == i)
					{
						ret.append(" " +  ref + " ");
					}
					else
					{
						ret.append(" " + ref + " , ");
					}
				}
			}

			// order by句を生成する
			if (true == order_by_.defined())
			{
				ret.append(" order by ");
				size_t i = 0;
				for (const auto & ref : order_by_.term_)
				{
					i++;
					if (order_by_.term_.size() == i)
					{
						ret.append(" " + ref + " ");
					}
					else
					{
						ret.append(" " + ref + " , ");
					}
				}
			}

			ret.append(";");

			return std::move(ret);
		}

		//! クエリ文
		std::string query_;

	};


	//order by クエリオブジェクト

	template<typename TABLE_TYPE_ >
	struct query_object_order_by : public query_object< TABLE_TYPE_ >
	{
	private:

		//!　引数の各要素を取得する関数

		template<typename First>
		void do_order_by(void)
		{
		  query_object< TABLE_TYPE_ >::order_by_.term_.push_back
		    ( ( boost::format(" %||.%|| %||")
			% First::COLUMN_INFO_TYPE::BELONG_TABLE_TYPE::get_tabel_name()
			% First::COLUMN_INFO_TYPE::name_ 
			% First::get_order() ).str().c_str() );
		}

		template<typename First, typename Second, typename... Rest>
		void do_order_by(void)
		{
			do_order_by<First>();
			do_order_by<Second, Rest...>();
		}

	public:

		query_object_order_by(const query_object< TABLE_TYPE_ > & tmp)
			: query_object< TABLE_TYPE_ >(tmp) { }

		//order_by条件

		template<typename First, typename... Rest >
		query_object< TABLE_TYPE_ > order_by(void)
		{
			do_order_by< First, Rest... >();

			return *this;
		}

	};

	//group_by クエリオブジェクト

	template<typename TABLE_TYPE_ >
	struct query_object_group_by : public query_object< TABLE_TYPE_ >
	{
	private:

		template<typename First>
		void do_group_by(void)
		{
		  query_object< TABLE_TYPE_ >::group_by_.term_.push_back
		    ( ( boost::format(" %||.%|| ")
		  	% First::BELONG_TABLE_TYPE::get_tabel_name()
		  	% First::name_).str().c_str() );
		}

		template<typename First, typename Second, typename... Rest>
		void do_group_by(void)
		{
			do_group_by<First>();
			do_group_by<Second, Rest...>();
		}

	public:

		query_object_group_by(const query_object< TABLE_TYPE_ > & ref)
			: query_object< TABLE_TYPE_ >(ref) { }

		//group_by条件

		template<typename First, typename... Rest >
		query_object_order_by< TABLE_TYPE_ > group_by(void)
		{
			do_group_by< First, Rest... >();

			return *this;
		}

		template<typename WHERE_CONDITION_TYPE >
		  query_object_group_by(const where_condition_list<WHERE_CONDITION_TYPE> &list)
		{
			//typedef typename mpl_util::vector_2_tuple<WHERE_CONDITION_TYPE>::type RET_TYPE;
			query_object< TABLE_TYPE_ >::where_.term_ = list.condition_;
		}


	private:

		//!　引数の各要素を取得する関数

		template<typename First>
		void do_order_by(void)
		{
		  query_object< TABLE_TYPE_ >::order_by_.term_.push_back
		    ( ( boost::format(" %||.%|| %||")
			% First::COLUMN_INFO_TYPE::BELONG_TABLE_TYPE::get_tabel_name()
			% First::COLUMN_INFO_TYPE::name_ 
			% First::get_order() ).str().c_str() );
		}

		template<typename First, typename Second, typename... Rest>
		void do_order_by(void)
		{
			do_order_by<First>();
			do_order_by<Second, Rest...>();
		}

	public:

		//ToDo : order_byが必要となるので、処理を処理をコピーしてきた。
		//いずれ整理する必要有り

	/* query_object_group_by(const query_object< TABLE_TYPE_ > & tmp) */
	/* 		: query_object< TABLE_TYPE_ >(tmp) { } */

		//order_by条件

		template<typename First, typename... Rest >
		query_object< TABLE_TYPE_ > order_by(void)
		{
			do_order_by< First, Rest... >();

			return *this;
		}

	};

	// inner join句で結合条件を指定する関数テンプレート
	namespace condition
	{

		template< typename COLUMN1_, typename COLUMN2_ > struct equal
		{
			//結合対象の型を展開
			typedef COLUMN1_ COLUMN1;
			typedef COLUMN2_ COLUMN2;

			//! テーブルの結合条件を取得する

			const static std::string get_condition(void)
			{
				return " = ";
			}

		};

		template< typename COLUMN1_, typename COLUMN2_ > struct not_equal
		{
			//結合対象の型を展開
			typedef COLUMN1_ COLUMN1;
			typedef COLUMN2_ COLUMN2;

			//! テーブルの結合条件を取得する

			const static std::string get_condition(void)
			{
				return " != ";
			}
		};

		template< typename COLUMN1_, typename COLUMN2_ > struct greater
		{
			//結合対象の型を展開
			typedef COLUMN1_ COLUMN1;
			typedef COLUMN2_ COLUMN2;

			//! テーブルの結合条件を取得する

			const static std::string get_condition(void)
			{
				return " > ";
			}

		};

		template< typename COLUMN1_, typename COLUMN2_ > struct greater_equal
		{
			//結合対象の型を展開
			typedef COLUMN1_ COLUMN1;
			typedef COLUMN2_ COLUMN2;

			//! テーブルの結合条件を取得する

			const static std::string get_condition(void)
			{
				return " >= ";
			}


		};

		template< typename COLUMN1_, typename COLUMN2_ > struct less
		{
			//結合対象の型を展開
			typedef COLUMN1_ COLUMN1;
			typedef COLUMN2_ COLUMN2;

			//! テーブルの結合条件を取得する

			const static std::string get_condition(void)
			{
				return " < ";
			}

		};

		template< typename COLUMN1_, typename COLUMN2_ > struct less_equal
		{
			//結合対象の型を展開
			typedef COLUMN1_ COLUMN1;
			typedef COLUMN2_ COLUMN2;

			//! テーブルの結合条件を取得する

			const static std::string get_condition(void)
			{
				return " <= ";
			}

		};
	}

	//where クエリオブジェクト

	template<typename TABLE_TYPE_ >
	struct query_object_where : public query_object< TABLE_TYPE_ >
	{
	public:

		query_object_where(const inner_join_term & t)
		{
			this->inner_join_ = t;
		}

		template<typename WHERE_CONDITION_TYPE >
		query_object_where(const where_condition_list<WHERE_CONDITION_TYPE> &list)
		{
			//! 条件をチェック
			static_assert (!std::is_same< typename TABLE_TYPE_::T1_TYPE, typename mpl_::void_ >::value,
				"must define table type");
			static_assert (!std::is_same< typename TABLE_TYPE_::T2_TYPE, mpl_::void_ >::value,
				"must define table type");

			//指定されるテーブル数は2個だけのハズ
			query_object< TABLE_TYPE_ >::inner_join_.table1_ = TABLE_TYPE_::T1_TYPE::get_tabel_name();
			query_object< TABLE_TYPE_ >::inner_join_.table2_ = TABLE_TYPE_::T2_TYPE::get_tabel_name();

			//typedef typename mpl_util::vector_2_tuple<WHERE_CONDITION_TYPE>::type RET_TYPE;
			query_object< TABLE_TYPE_ >::inner_join_.term_ = list.condition_;


		}

		template<typename WHERE_CONDITION_TYPE >
		query_object_group_by< typename mpl_util::vector_2_tuple<WHERE_CONDITION_TYPE>::type >
			where(const where_condition_list<WHERE_CONDITION_TYPE> &list)
			{
				typedef typename mpl_util::vector_2_tuple<WHERE_CONDITION_TYPE>::type RET_TYPE;
				query_object< TABLE_TYPE_ >::where_.term_ = list.condition_;
				return query_object_group_by< RET_TYPE >(*this);
			}



	};


	//where条件句

	struct where_condition_base
	{

		where_condition_base(const std::string & n, const std::type_info &column_info_type) :
		name_(n),
		column_info_type_(column_info_type) { }

		std::string name_;
		const std::type_info &column_info_type_;
	};



	//! DTOのカラムとなる事が出来る型
	namespace column_type
	{
		//! null不許可の型

#define NAMESPACE_LOCAL_DEFINE_WRAP_STRUCT( __wrap_type_name__ , __wrap_type_name_with_namespace__ ) \
	template< typename TABLE_TYPE >					\
		struct wrap_##__wrap_type_name__ : public where_condition_base	\
		{									\
		wrap_##__wrap_type_name__(const std::string & s) : \
		where_condition_base(s, typeid(COLUMN_TYPE))			\
		{								\
	}								\
	typedef TABLE_TYPE BELONG_TABLE_TYPE;				\
	typedef __wrap_type_name_with_namespace__ COLUMN_TYPE;		\
	};									\

		NAMESPACE_LOCAL_DEFINE_WRAP_STRUCT(int8_t, int8_t); //! wrap_int8_t
		NAMESPACE_LOCAL_DEFINE_WRAP_STRUCT(int32_t, int32_t); //! wrap_int32_t
		NAMESPACE_LOCAL_DEFINE_WRAP_STRUCT(int64_t, int64_t); //! wrap_int64_t
		NAMESPACE_LOCAL_DEFINE_WRAP_STRUCT(int, int); //! wrap_int64_t
		NAMESPACE_LOCAL_DEFINE_WRAP_STRUCT(double, double); //! wrap_double
		NAMESPACE_LOCAL_DEFINE_WRAP_STRUCT(bool, bool); //! wrap_bool
		NAMESPACE_LOCAL_DEFINE_WRAP_STRUCT(string, std::string); //! wrap_string
		NAMESPACE_LOCAL_DEFINE_WRAP_STRUCT(binary, std::vector< char >); //! wrap_binary


		//! nullになる事が出来る型
		namespace nullable
		{
			NAMESPACE_LOCAL_DEFINE_WRAP_STRUCT(int8_t, int8_t); //! wrap_int8_t
			NAMESPACE_LOCAL_DEFINE_WRAP_STRUCT(int32_t, int32_t); //! wrap_int32_t
			NAMESPACE_LOCAL_DEFINE_WRAP_STRUCT(int64_t, int64_t); //! wrap_int64_t
			NAMESPACE_LOCAL_DEFINE_WRAP_STRUCT(int, int); //! wrap_int64_t
			NAMESPACE_LOCAL_DEFINE_WRAP_STRUCT(double, double); //! wrap_double
			NAMESPACE_LOCAL_DEFINE_WRAP_STRUCT(bool, bool); //! wrap_bool
			NAMESPACE_LOCAL_DEFINE_WRAP_STRUCT(string, std::string); //! wrap_string
			NAMESPACE_LOCAL_DEFINE_WRAP_STRUCT(binary, std::vector< char >); //! wrap_binary
		}

#undef NAMESPACE_LOCAL_DEFINE_WRAP_STRUCT

	}


#define SORT_AND_UNIQ_VECTOR						\
	typename mpl_util::sort_and_uniq_table_vector<			\
	typename mpl_util::merge_vector<					\
	typename WHERE_CONDITION_TYPE1::TABLE_TYPE_VECTOR, typename WHERE_CONDITION_TYPE2::TABLE_TYPE_VECTOR >::type >::type \

	//! where条件一覧

	template<typename TABLE_TYPE>
	struct where_condition_list
	{

		where_condition_list(const std::string & c) :
		condition_(c) { }
		;
		std::string condition_;
		typedef TABLE_TYPE TABLE_TYPE_VECTOR;
	};


	//where条件式を結合する

	template<typename WHERE_CONDITION_TYPE1, typename WHERE_CONDITION_TYPE2>
	where_condition_list<SORT_AND_UNIQ_VECTOR >
		operator&& (const WHERE_CONDITION_TYPE1 &t1, const WHERE_CONDITION_TYPE2 &t2)
	{
			return where_condition_list<SORT_AND_UNIQ_VECTOR >(
				(boost::format("%1% and %2%") % t1.condition_ % t2.condition_).str());
		}

	template<typename WHERE_CONDITION_TYPE1, typename WHERE_CONDITION_TYPE2>
	where_condition_list<SORT_AND_UNIQ_VECTOR >
		operator|| (const WHERE_CONDITION_TYPE1 &t1, const WHERE_CONDITION_TYPE2 &t2)
	{
			return where_condition_list<SORT_AND_UNIQ_VECTOR >(
				(boost::format("%1% or %2%") % t1.condition_ % t2.condition_).str());
		}


	//-------------------------
	//整数系のwhere条件を定義する
	//-------------------------

	template<typename COLUMN_TYPE>
	where_condition_list<
		typename boost::mpl::vector<typename COLUMN_TYPE::BELONG_TABLE_TYPE>::type>
		operator== (const COLUMN_TYPE & co, const int & i)
	{
			return where_condition_list<
				typename boost::mpl::vector<typename COLUMN_TYPE::BELONG_TABLE_TYPE>::type >(
				(boost::format("( %1%.%2% = '%3%' )") % COLUMN_TYPE::BELONG_TABLE_TYPE::get_tabel_name() % co.name_ % i).str());
		}

	template<typename COLUMN_TYPE>
	where_condition_list<
		typename boost::mpl::vector<typename COLUMN_TYPE::BELONG_TABLE_TYPE>::type>
		operator>= (const COLUMN_TYPE & co, const int & i)
	{
			return where_condition_list<
				typename boost::mpl::vector<typename COLUMN_TYPE::BELONG_TABLE_TYPE>::type >(
				(boost::format("(%1%.%2% >= '%3%' )") % COLUMN_TYPE::BELONG_TABLE_TYPE::get_tabel_name() % co.name_ % i).str());
		}

	template<typename COLUMN_TYPE>
	where_condition_list<
		typename boost::mpl::vector<typename COLUMN_TYPE::BELONG_TABLE_TYPE>::type>
		operator> (const COLUMN_TYPE & co, const int & i)
	{
			return where_condition_list<
				typename boost::mpl::vector<typename COLUMN_TYPE::BELONG_TABLE_TYPE>::type >(
				(boost::format("(%1%.%2% > '%3%' )") % COLUMN_TYPE::BELONG_TABLE_TYPE::get_tabel_name() % co.name_ % i).str());
		}

	template<typename COLUMN_TYPE>
	where_condition_list<
		typename boost::mpl::vector<typename COLUMN_TYPE::BELONG_TABLE_TYPE>::type>
		operator< (const COLUMN_TYPE & co, const int & i)
	{
			return where_condition_list<
				typename boost::mpl::vector<typename COLUMN_TYPE::BELONG_TABLE_TYPE>::type >(
				(boost::format("(%1%.%2% < '%3%' )") % COLUMN_TYPE::BELONG_TABLE_TYPE::get_tabel_name() % co.name_ % i).str());
		}

	template<typename COLUMN_TYPE>
	where_condition_list<
		typename boost::mpl::vector<typename COLUMN_TYPE::BELONG_TABLE_TYPE>::type>
		operator<= (const COLUMN_TYPE & co, const int & i)
	{
			return where_condition_list<
				typename boost::mpl::vector<typename COLUMN_TYPE::BELONG_TABLE_TYPE>::type >(
				(boost::format("(%1%.%2% <= '%3%' )") % COLUMN_TYPE::BELONG_TABLE_TYPE::get_tabel_name() % co.name_ % i).str());
		}


	//-------------------------
	//文字列系のwhere条件を定義する
	//-------------------------

	template<typename COLUMN_TYPE>
	where_condition_list<
		typename boost::mpl::vector<typename COLUMN_TYPE::BELONG_TABLE_TYPE>::type>
		operator== (const COLUMN_TYPE & co, const char *c)
	{
			return where_condition_list<
				typename boost::mpl::vector<typename COLUMN_TYPE::BELONG_TABLE_TYPE>::type >(
				(boost::format("(%1%.%2% = \"%3%\")") % COLUMN_TYPE::BELONG_TABLE_TYPE::get_tabel_name() % co.name_ % c).str());
		}

	//WHERE句

	template<typename WHERE_CONDITION_TYPE>
	query_object_group_by< typename mpl_util::vector_2_tuple<WHERE_CONDITION_TYPE>::type >
		where(const where_condition_list<WHERE_CONDITION_TYPE> &list)
		{
			typedef typename mpl_util::vector_2_tuple<WHERE_CONDITION_TYPE>::type RET_TYPE;
			return std::move(query_object_group_by<RET_TYPE>(list));
		}



		//-------------------------
		//join系の条件式を定義する
		//-------------------------
		/* template<typename COLUMN_TYPE> */
		/*   where_condition_list< */
		/*   typename boost::mplvector<typename COLUMN_TYPE::BELONG_TABLE_TYPE>::type> */
		/*   operator==( const where_condition_base & t1, const int & i) */
		/*   { */
		/*     return where_condition_list< */
		/*     typename boost::mplvector<typename COLUMN_TYPE::BELONG_TABLE_TYPE>::type>( */
		/* 									   (boost::format("( %1%.%2% = '%3%' )") % COLUMN_TYPE::BELONG_TABLE_TYPE::get_tabel_name() % co.name_ % i).str()); */
		/*   } */


		//----取得テーブル1個の場合

		template<typename T1>
		struct where_arg_type_1
		{
		private:
			typedef typename boost::mpl::vector<T1>::type src_vector;
			typedef typename mpl_util::sort_and_uniq_table_vector<src_vector>::type sorted;
		public:
			typedef typename mpl_util::vector_2_tuple<sorted>::type type;

		};

		//From句
		//template型の推論を禁止するために、構造体を定義する

		template<typename T1>
		struct from1_term
		{
			//1版目の引数
			typedef T1 Table1;

			from1_term(query_object< typename where_arg_type_1< T1 >::type > && a)
				: conditions_(a)
			{
				conditions_.query_ = (boost::format("from %1% %2%") % T1::get_tabel_name()
					% conditions_.query_).str();
			}

			//from句に指定された条件項目
			query_object< typename where_arg_type_1< T1 >::type > conditions_;

		};

		//From句を作り出す関数

		template<typename T1>
		from1_term< T1 >
			FROM1(query_object< typename where_arg_type_1< T1 >::type > && a)
		{
				return std::move(from1_term< T1 >(std::move(a)));
		}

		//-------------------------

		//----取得テーブル2個の場合

		template<typename T1, typename T2>
		struct where_arg_type
		{
		private:
			typedef typename boost::mpl::vector<T1, T2>::type src_vector;
			typedef typename mpl_util::sort_and_uniq_table_vector<src_vector>::type sorted;
		public:
			typedef typename mpl_util::vector_2_tuple<sorted>::type type;

		};

		namespace
		{
			namespace local
			{
				//! inner join結合テーブル名をデコードする名前空間
				namespace inner_join
				{

					template<typename First>
					inner_join_term
						decode_inner_join2(inner_join_term & ret)
					{

							//!　比較するカラム型が一致するか確認する。
							static_assert (std::is_same< typename First::COLUMN1::COLUMN_TYPE,
								typename First::COLUMN2::COLUMN_TYPE >::value,
								"Inner join type must same type!");

							//! 結合するテーブル情報を設定する。
							//ret.table1_ = First::COLUMN1::name_;
							//ret.table2_ = First::COLUMN2::name_;
							ret.term_ = (boost::format(" %||.%|| %|| %||.%|| ")
								% ret.table1_
								% First::COLUMN1::name_
								% First::get_condition()
								% ret.table2_
								% First::COLUMN2::name_).str();

							return ret;

						}

				}
			}
		}

		//inner join句

		template< typename T1, typename T2, typename TERMS >
		query_object_where< typename where_arg_type< T1, T2 >::type >
			inner_join2(void)
		{
				//結合するカラムを取得する
				inner_join_term it;
				it.table1_ = T1::get_tabel_name();
				it.table2_ = T2::get_tabel_name();
				const auto t = local::inner_join::decode_inner_join2 < TERMS >(it);

				//関数戻り値を作成
				query_object_where< typename where_arg_type< T1, T2 >::type > ret(t);
				return ret;
		}

		template<typename TABLE1, typename TABLE2>
		struct from2_term
		{
			typedef TABLE1 Table1;
			typedef TABLE2 Table2;

			from2_term(from2_term<TABLE1, TABLE2 > && ref) :
				conditions_(ref.conditions_) { }

			from2_term(query_object<typename where_arg_type<TABLE1, TABLE2>::type > && a)
				: conditions_(a)
			{
				if (true == a.inner_join_.defined())
				{
					conditions_.query_ = (boost::format("from %||")
						% conditions_.get_query()).str();
				}
				else
				{
					conditions_.query_ = (boost::format("from %|| , %|| %||")
						% TABLE1::get_tabel_name()
						% TABLE2::get_tabel_name()
						% conditions_.get_query()).str();
				}
			}

			from2_term(query_object<typename mpl_util::table_type_tuple<TABLE1> > && a)
			{
				one_table_init< TABLE1 >(std::move(a));
			}

			from2_term(query_object<typename mpl_util::table_type_tuple<TABLE2> > && a)
			{
				one_table_init< TABLE2 >(std::move(a));
			}

		private:

			template<typename T>
			void one_table_init(query_object<typename mpl_util::table_type_tuple< T > > && a)
			{
				typedef typename where_arg_type<TABLE1, TABLE2>::type arg_type;
				typedef query_object< arg_type > ret_type;

				conditions_ = std::move(ret_type(
					a.query_ = (boost::format("from %|| , %|| %||")
					% TABLE1::get_tabel_name() % TABLE2::get_tabel_name()
					% a.query_).str()));
			}

		public:
			query_object< typename where_arg_type< TABLE1, TABLE2 >::type > conditions_;

		};

		//From句を作り出す関数

		template<typename T1, typename T2>
		from2_term< T1, T2 >
			FROM2(query_object<typename where_arg_type<T1, T2>::type > && a)
		{
				return std::move(from2_term< T1, T2 >(std::move(a)));
		}

		template<typename T1, typename T2>
		from2_term< T1, T2 >
			FROM2(query_object<typename mpl_util::table_type_tuple<T1> > && a)
		{
				return std::move(from2_term< T1, T2 >(std::move(a)));
		}

		template<typename T1, typename T2>
		from2_term< T1, T2 >
			FROM2(query_object<typename mpl_util::table_type_tuple<T2> > && a)
		{
				return std::move(from2_term< T1, T2 >(std::move(a)));
		}

#undef  SORT_AND_UNIQ_VECTOR

		//! DTOの基底クラス

		struct dto_base
		{
			//  protected:

			//! 指定された2引数の型が同じ型の場合、データをコピーして値を設定する
			//! 型指定が異なる場合は何もしない。

			template< typename SRC_TYPE, typename DST_TYPE >
			bool set_if_same_type(const SRC_TYPE & src, DST_TYPE & dst)
			{

				std::cout << " -------- " << std::endl;
				tiny_query_helper::debug::print_type_name< SRC_TYPE >();
				tiny_query_helper::debug::print_type_name< DST_TYPE >();
				//std::cout << " -------- " << std::endl;


				using namespace extension_switch;
				return _switch
					(src,
					match::type< const DST_TYPE >([&dst](const DST_TYPE & ref) -> bool
				{
					//srcとdstが同じ型で設定可能であれば設定する
					dst = ref;
					return true;
				}),
					//default type
					match::other([](const boost::any & ref) -> bool
				{
					//srcとdstが別の型であったら何もしない
					return false;
				})
					);
			}

		};


}

#endif /* _TINY_QUERY_HELPER_H_ */

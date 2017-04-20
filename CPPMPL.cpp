//Routines for compile-time precomputation
namespace cplfns{
	#define _cc static const
	#define _val(z) z::value
	#define _typ(z) z::value_type

	template<int T>
	struct To_Bool{
		_cc bool value=1;
		typedef bool value_type;
	};
	template<>
	struct To_Bool<0>{
		_cc bool value=0;
		typedef bool value_type;
	};
	template<unsigned long T>
	struct Log2{
		_cc int value=_val(Log2<T/2>)+1;
		typedef int value_type;
	};
	template<>
	struct Log2<0>{
		_cc int value=-1;
		typedef int value_type;
	};
	template<unsigned long T>
	struct Popcnt{
		_cc int value=_val(Popcnt<T/2>)+(T&1);
		typedef int value_type;
	};
	template<>
	struct Popcnt<0>{
		_cc int value=0;
		typedef int value_type;
	};
	template<unsigned long T>
	struct Lowbit{
		_cc unsigned long value=T&(0-T);
		typedef unsigned long value_type;
	};
	template<unsigned long T>
	struct Lowbit2{
		typedef Lowbit<T> Full;
		_cc int value=_val(Log2<_val(Full)>);
		typedef int value_type;
	};
	template<unsigned long T>
	struct BinarySplit{
		typedef Lowbit2<T> exponent;
		_cc unsigned long value=T>>_val(exponent);
		typedef unsigned long value_type;
	};
	template<int a,int P,unsigned long e>
	struct ModPow{
		_cc int value=ModPow<a,P,e/2>::value*(unsigned long)ModPow<a,P,e/2>::value%P*(e&1?a:1)%P;
		typedef int value_type;
		_cc int _a=a;
		_cc int _P=P;
		_cc int _e=e;
	};
	template<int a,int P>
	struct ModPow<a,P,0>{
		_cc int value=1;
		typedef int value_type;
		_cc int _a=a;
		_cc int _P=P;
		_cc int _e=0;
	};
	template<unsigned long T>
	struct UINT{
		_cc unsigned long value=T;
		typedef unsigned long value_type;
	};
	template<class A,class B,bool cond>
	struct If{
	};
	template<class A,class B>
	struct If<A,B,0>{
		typedef B ifval;
		typedef A elval;
	};
	template<class A,class B>
	struct If<A,B,1>{
		typedef A ifval;
		typedef B elval;
	};
	template<class A,class B,bool cond>
	struct Sel{
	};
	template<class A,class B>
	struct Sel<A,B,0>{
		typedef A val;
	};
	template<class A,class B>
	struct Sel<A,B,1>{
		typedef B val;
	};

	template<int T>
	struct _recurse_test{
		_cc unsigned long value=_recurse_test<T-1>::value;
		typedef unsigned long value_type;
	};
	template<>
	struct _recurse_test<0>{
		_cc unsigned long value=0;
		typedef unsigned long value_type;
	};

	template<int t>
	struct End{
		_cc bool end=0;
		_cc int value=t;
	};

	template<>
	struct End<160>{
		_cc bool end=1;
		_cc int value=160;
	};

	template<template<int>class End,int l,int r>
	struct For{
		typedef
			typename If<
				typename Sel<
					For<End,(l+r)/2+1,r>,
					For<End,l,(l+r)/2>,
					For<End,l,(l+r)/2>::val::end
				>::val::val,
				End<l>,
				l<r
			>::ifval val;
		_cc bool end=val::end;
	};
	template<template<int>class End,int l>
	struct For<End,l,l>{
		typedef End<l> val;
		_cc bool end=val::end;
	};

	template<int P>
	struct Mod{
		_cc int value=P;
		template<int i>
		struct is_primitive_root{
			typedef ModPow<i,P,(P-1)/2> ump;
			_cc int value=i;
			_cc bool end=(ump::value!=1);
		};
	};

	template<
		template<
			int, // index
			class
		>class Run,
		class iter_type,
		int l,int r
	>struct Iterate{
		typedef
			typename Iterate<
				Run,
				iter_type,
				l,(l+r)/2
			>::val first_half;
		typedef
			typename Sel<
				Iterate<
					Run,typename first_half::iter,
					(l+r)/2+1,r
				>,
				Iterate<
					Run,
					iter_type,
					l,(l+r)/2
				>,
				first_half::end
			>::val::val val;
		_cc bool end=val::end;
	};
	template<
		template<
			int, // index
			class
		>class Run,
		class iter_type,
		int l
	>struct Iterate<Run,iter_type,l,l>{
		struct val{
			typedef typename Run<l,iter_type>::iter iter;
			_cc bool end=Run<l,iter_type>::end;
		};
		_cc bool end=val::end;
	};

	template<
		template<
			int,
			class
		>class Run
	>struct iterate_wrapper_nobreak{
		template<
			int index,
			class iter_type
		>struct val{
			typedef typename Run<index,iter_type>::iter iter;
			_cc bool end=0;
		};
	};

	template<
		template<
			int,
			class
		>class Run,
		int t
	>struct iterate_wrapper_breakat{
		template<
			int index,
			class iter_type
		>struct val{
			typedef typename Run<index,iter_type>::iter iter;
			_cc bool end=index==t;
		};
	};

	template<int index,class UINT_class>
	struct test_accum{
		typedef UINT<UINT_class::value+index> iter;
	};
}

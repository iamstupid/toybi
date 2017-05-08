#ifndef MPL_h
#define MPL_h
namespace MPL{
	template<class A,class B,bool cond>
	struct Sel{
	};
	template<class A,class B>
	struct Sel<A,B,1>{typedef A val;};
	template<class A,class B>
	struct Sel<A,B,0>{typedef B val;};

	template<
		class A,
		int pow
	>struct Pow2_val{
		static const A val=Pow2_val<A,pow-1>::val*2;
	};
	template<class A>
	struct Pow2_val<A,0>{
		static const A val=1;
	};
}
#endif

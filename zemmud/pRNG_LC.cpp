#ifndef pRNG_LC_impl
#define pRNG_LC_impl
#include "pRNG.cpp"
#include "ints.hpp"
#include "lib/modular.cpp"
#include "pRNG_bitTruncator.hpp"
namespace pRNG{
	template<
		class result_type_t,
		u64 a,u64 c,u64 m
	>struct LCG_General{
		typedef result_type_t result_type;
		static const result_type rand_max=m-1;
		result_type state;
		inline result_type operator()(){
			return state=mul_add_mod<a,c,m>(state);
		}
		void seed(result_type g){state=g;}
		LCG_General(result_type g=0):state(g){}
	};
	template<
		class result_type_t,
		u64 a,u64 c
	>struct LCG_General<result_type_t,a,c,0>{
		typedef result_type_t result_type;
		static const result_type rand_max=~result_type(0);
		result_type state;
		inline result_type operator()(){
			return state=state*a+c;
		}
		void seed(result_type g){state=g;}
		LCG_General(result_type g=0):state(g){}
	};
}
#endif

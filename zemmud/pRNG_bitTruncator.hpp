#ifndef pRNG_bitTruncator_h
#define pRNG_bitTruncator_h
#include "MPL.hpp"
namespace pRNG{
	template<
		class pRNGg,
		int startBit,int endBit
	>struct bitTruncator{
		typedef typename pRNGg::result_type result_type;
		pRNGg t;
		template<typename... vv>
		bitTruncator(vv...args):t(args...){
		}
		template<typename... vv>
		void seed(vv...args){
			t.seed(args...);
		}
		static const result_type
			rand_max=MPL::Pow2_val<result_type,endBit-startBit+1>::val-1;
		inline result_type operator()(){
			return (t()>>startBit)&rand_max;
		}
	};
};
#endif

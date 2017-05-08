#ifndef pRNG_Xorshift_impl
#define pRNG_Xorshift_impl
#include "pRNG.cpp"
#include "ints.hpp"
namespace pRNG{
	struct xs32{
		typedef u32 result_type;
		u32 s;
		xs32(u32 s=0x9b64d0b8u):s(s){}
		inline u32 operator()(){
			s^=s<<13;s^=s>>17;
			return s^=s<<5;
		}
		inline void seed(u32 st){s=st;}
	};
	struct xs128{
		typedef u32 result_type;
		u32 s[4];
		xs128():s{0xa6249560u,0x0c5fd38fu,0xc7661894u,0x22990a15}{}
		template<typename T>
		xs128(const T*buffer_type){memset(s,buffer_type,sizeof s);}
		template<typename T>
		inline void seed(const T*buffer_type){memset(s,buffer_type,sizeof s);}
		inline u32 operator()(){
		}
	};
}
#endif

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
			u32 g=s[3];
			g^=g<<11;
			g^=g>>8;
			s[3]=s[2];s[2]=s[1];s[1]=s[0];
			return s[0]=((g^=s[0])^=s[0]>>19);
		}
	};
	struct xss64{
		typedef u64 result_type;
		u64 s;
		xss64(u64 s=0x9bf4bd1d835a38e6ull):s(s){}
		inline void seed(u64 st){s=st;}
		inline u64 operator()(){
			s^=s>>12;
			s^=s<<25;
			return (s^=s>>27)*0x2545f4914f6cdd1dull;
		}
	};
	struct xss1k{
		typedef u64 result_type;
		u64 s[16],p;
		xss1k():
		s{	0x79a3d197a3eda564ull,0xea0832f5f13eca6eull,
			0x9b25d666c98ad37eull,0xc47e8b052d0159dfull,
			0xc3d03d041c267d8dull,0xfea949190d0bf9f8ull,
			0x07f1e5bc39068da5ull,0x44679e2a74432b86ull,
			0x205ca0fd8db63493ull,0x3c086aaa6dd55a74ull,
			0xc4e064b3e570282bull,0x4c7b4621df4da54dull,
			0x7b14f5648cbe10cdull,0x848656ce83c494a5ull,
			0xb02648187107686dull,0x964d7d7714d55e34ull},
		p(0){
			}// a really long initializer list; in fact generated with /dev/random
		template<typename T>
		xss1k(const T*buffer_type){memset(s,buffer_type,sizeof s);}
		template<typename T>
		inline void seed(const T*buffer_type){memset(s,buffer_type,sizeof s);}
		inline u64 operator()(){
			register u64 s0=s[p],s1=s[++p&=15];
			s1^=s1<<31;
			return (s[p]=s1^s0^(s1>>11)^(s0>>30))*0x106689d45497fdb5ull;
		}
	};
	struct xsadw{
		typedef u64 result_type;
		u64 s[2];
		xsadw():s{0xc4f7ba40435d8ee7ull,0x54d21665a4290c80ull}{}
		template<typename T>
		xsadw(const T*buffer_type){memset(s,buffer_type,sizeof s);}
		template<typename T>
		inline void seed(const T*buffer_type){memset(s,buffer_type,sizeof s);}
		inline u64 operator()(){
			u64 x=*s,y=s[1];
			x^=x<<23;
			return (s[1]=x^(*s=y)^(x>>17)^(y>>26))+y;
		}
	};
}
#endif

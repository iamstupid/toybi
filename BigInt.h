#include<stdint.h>
#include"BigIntError.cpp"
#include"stlport.cpp"
namespace allocators{
	namespace simalc{
		#include"SimpleAllocators.cpp"
	}
	typedef simalc::__impl::Allocator_portmalloc callocator;
	typedef simalc::__impl::Allocator_pow2 pow2alloc;
}
typedef allocators::callocator callocator;
typedef allocators::pow2alloc pow2alloc;
namespace bigint{
	#include"BigInt.cpp"
}
#define bigint_type bigint::__impl::bui
int Application(int argc,char**argv);
int main(int argc,char**argv){
	atexit(allocators::simalc::__impl::unmap);
	return Application(argc,argv);
}

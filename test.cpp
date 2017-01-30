#include"BigInt.h"
#define AllocatorType pow2alloc
//#define AllocatorType callocator
#define sz 10000000
int Application(int argc,char**argv){
	AllocatorType instance;
	bigint_type<AllocatorType> p(instance);
	p.FitTo(sz).From(0xFFFFFFFF);
	for(int i=1;i<sz;++i)p.coe[i]=0xFFFFFFFF;p.siz=sz;
//	p.Print();puts("P big");
	Mult_karats(p,p,instance);//.Print();puts("P^2");
//	PError(0,"Debug");
	return 0;
}

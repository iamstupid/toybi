#include"BigInt.h"
#define AllocatorType pow2alloc
//#define AllocatorType callocator
#define sz 100000
int Application(int argc,char**argv){
	AllocatorType instance;
	bigint_type<AllocatorType> p(instance);
/*	p.FitTo(sz).From(0xFFFFFFFF);
	for(int i=1;i<sz;++i)p.coe[i]=0xFFFFFFFF;p.siz=sz;*/
	p.FitTo(sz).From(rand());
	for(int i=1;i<sz;++i)p.coe[i]=rand();p.siz=sz;
/*	p.Print();puts("p=rand()");
	bigint_type<AllocatorType> g=p.Cop().Add(p).Add(p).Normalize();
	g.Print();puts("g=3*p");
	g.divexact_3_pip().Print();puts("g/=3");*/
//	p.Print();puts("P big");
	bigint_type<AllocatorType> a=Mult_toom33(p,p,instance);//.Print().Free();puts("P^2 -- toom33");
//	bigint_type<AllocatorType> b=Mult_karats(p,p,instance);//puts("P^2 -- karats");
//	printf("%s",(a.smaller(b)||a.greater(b))?"Not passed":"Passed");
//	bal c=Mult_school(p,p,instance).Print().Free();puts("P^2 -- school");
//	PError(0,"Debug");
	return 0;
}

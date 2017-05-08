#include<cstdio>
#include<stdlib.h>
#include<random>
#include "pRNG_LC_Rnglist.hpp"
int main(){
	pRNG::LCG_ANSIC q(31157);
	pRNG::LCG_GC    t(31157);
	std::mt19937_64 gg;
/*	auto rrng=pRNG::pRNG_adapt(q);
	srand(31157);
	for(int i=0;i<10;++i)
		printf("%u %u\n",(u32)rrng(),rand());
	rrng=pRNG::pRNG_adapt(t);
	for(int i=0;i<10;++i)
		printf("%u %u\n",(u32)rrng(),rand());
	rrng=pRNG::pRNG_adapt(q);
	for(int i=0;i<10;++i)
		printf("%u %u\n",(u32)rrng(),rand());*/
	auto qrng=pRNG::pRNG_adapt(gg);
	for(int i=0;i<10;++i)
		printf("%u %u\n",(u32)qrng(),rand());
	qrng=pRNG::pRNG_adapt(*(new pRNG::LCG_JAVA(31157)));
	for(int i=0;i<10;++i)
		printf("%u %u\n",(u32)qrng(),rand());
	delete (pRNG::LCG_JAVA*)qrng.data_region;
	return 0;
}

#ifndef pRNG_LC_Rnglist_h
#define pRNG_LC_Rnglist_h
#include "pRNG_LC.cpp"
namespace pRNG{
	/**
	 * A list of pRNGs containing impls of most LCG Rngs listed on
	 * https://en.wikipedia.org/wiki/Linear_congruential_generator
	 * Note minstd should be seeded with a non-zero number to give
	 * seemly random sequence
	 */
	using LCG_NR     =LCG_General <u32,1664525,1013904223,0>;
	using LCG_TP     =LCG_General <u32,134775813,1,0>;
	using LCG_BCR    =LCG_General <u32,22695477,1,0>;
	using LCG_GCR    =LCG_General <u32,1103515245,12345,((~0u)>>1)>;
	using LCG_MSVCR  =LCG_General <u32,214013,2531011,0>;
	using LCG_BC     =bitTruncator<LCG_BCR,16,30>;
	using LCG_MSVC   =bitTruncator<LCG_MSVCR,16,30>;
	using LCG_GC     =LCG_GCR;// exactly the same..
	using LCG_ANSICR =LCG_General <u32,1103515245,12345,0>;
	using LCG_ANSIC  =bitTruncator<LCG_ANSICR,16,30>;
	using LCG_MINSTD0=LCG_General <u32,16807,0,((~0u)>>1)>;
	using LCG_MINSTD =LCG_General <u32,48271,0,((~0u)>>1)>;
	using LCG_MMIX   =LCG_General <u64,6364136223846793005ull,1442695040888963407ull,0>;
	using LCG_MUSLR  =LCG_General <u64,6364136223846793005ull,1,0>;
	using LCG_MUSL   =bitTruncator<LCG_MUSLR,32,63>;
	using LCG_JAVR   =LCG_General <u64,0x5DEECE66Dull,11,0>;
	using LCG_JAVA   =bitTruncator<LCG_JAVR,16,47>;
}
#endif

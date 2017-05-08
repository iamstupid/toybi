#ifndef lib_modular_impl
#define lib_modular_impl
template<
	int a,int c,int m
>u32 mul_add_mod(u32 t){
	return ((u64)t*a+c)%m;
}

#ifdef __uint64_t
template<
	int a,int c,int m
>u64 mul_add_mod(u64 t){
	return ((u128)t*a+c)%m;
}
#else
#endif
#endif

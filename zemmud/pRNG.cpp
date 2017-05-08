#ifndef pRNG_impl
#define pRNG_impl
namespace pRNG{
	template<class result_type>
	struct pRNG{
		void*data_region;
		typedef result_type(*function_type)(void*);
		function_type iterate_function;
		pRNG(void*data_region,function_type iterate_function):
			data_region(data_region),
			iterate_function(iterate_function){}
		inline result_type operator()(){
			return iterate_function(data_region);
		}
	};
	template<typename T>
	typename T::result_type pRNG_next(
		void*T_Ptr
	){
		return ((T*)T_Ptr)->operator()();
	}
	template<class T>
	pRNG<typename T::result_type>pRNG_adapt(T&prng_instance){
		return pRNG<typename T::result_type>((void*)(&prng_instance),pRNG_next<T>);
	}
}
#endif

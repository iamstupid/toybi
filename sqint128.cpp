#include <cmath>
#include <cstdio>
#include <random>
#include <chrono>
typedef __uint128_t u128;
typedef unsigned long long u64;
const int count=10000000;
u64 sqrt_approx(u64 x){
	u64 approx=sqrt(double(x));
	return (approx+x/approx)>>1;
}
u64 sqrt(u64 x){
	u64 approx=sqrt(double(x));
	u64 apt=(approx+x/approx)>>1;
	approx=apt*apt;
	if(approx>x)return apt-1;
	if(x-approx>=2*apt-1)return apt+1;
	return apt;
}
u128 sqrt(u128 r){
	if(!(r>>64))return sqrt(u64(r));
	int cnt=(((64-__builtin_clzll(u64(r>>64)))+1)|1)^1;
	u128 approx=u128(sqrt_approx(u64(r>>cnt)))<<(cnt/2);
	approx=(approx+r/approx)>>1;
	u128 apt=u128(u64(approx))*u128(u64(approx));
	if(apt>r)return approx-1;
//	if(r-apt>=2*approx-1)return approx+1;
	return approx;
}
u128 rand_arr[count],root_arr[count];

typedef void(*func)();

void Time(const char*str,func fn,int multi=count){
	using hrc=std::chrono::high_resolution_clock;
	hrc::time_point start=hrc::now();
	fn();
	hrc::time_point stop=hrc::now();
	hrc::duration dur=stop-start;
	printf("%s Finished in %llu us . \n",str,std::chrono::duration_cast<std::chrono::microseconds>(dur).count());
	if(multi)
		printf("Average %.3lfns per op.\n",double(std::chrono::duration_cast<std::chrono::nanoseconds>(dur).count())/multi);
}

void Root(){
	for(int i=0;i<count;++i)
		root_arr[i]=sqrt(rand_arr[i]);
}

std::mt19937_64 rng;

void Gen(){
	for(int i=0;i<count;++i)
		rand_arr[i]=(u128(rng())<<64)|rng();
}

int Validate(){
	for(int i=0;i<count;++i){
		u128 ax=root_arr[i];
		u128 bx=(ax+1)*(ax+1);
		ax=ax*ax;
		if(ax>rand_arr[i])
			return i+1;
		if(bx<=rand_arr[i])
			return i+1;
	}return 0;
}

char pp[300];
#define spp(...) (sprintf(pp,##__VA_ARGS__),pp)
#define hexo(x) (spp("0x%llx%016llx",u64(x>>64),u64(x)))

int main(){
	printf("Count=%d\n",count);
	Time("Generation",Gen);
	Time("Square root",Root);
	int val=Validate();
	printf("Validation %s\n",val?spp("Fail at %d",val):"Passed");
	if(val){
		--val;
		printf("Rand %s\n",hexo(rand_arr[val]));
		printf("Root %s\n",hexo(root_arr[val]));
	}
	return 0;
}

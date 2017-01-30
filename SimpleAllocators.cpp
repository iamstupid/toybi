namespace __impl{
	#include "malloc.h"
	struct Allocator_portmalloc{
		template<class T>
		T*allocate(uint32_t g){
			T*e=(T*)calloc(g,sizeof(T));
			if(!e)PError(1,"Memory allocation failed");
			return e;
		}
		template<class T>
		void release(T*p,uint32_t g){free(p);}
	};
	#include "sys/mman.h"
	uintptr_t mmaped[1<<14],mmapeds;
	void*BigAlloc(uintptr_t size){
		size=((size-1)>>13)+1<<13;
		void*ret=mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_SHARED,-1,0);
		if(ret==MAP_FAILED)PError(1,"Page allocation failed");
		mmaped[mmapeds++]=reinterpret_cast<uintptr_t>(ret);
		mmaped[mmapeds++]=size;
		return ret;
	}
	void unmap(){
		while(mmapeds){
			uintptr_t siz=mmaped[--mmapeds];
			uintptr_t ptr=mmaped[--mmapeds];
			munmap(reinterpret_cast<void*>(ptr),size_t(siz));
		}
	}
	struct IndexObject{
		uint64_t Index[64];
		inline int find(){
			if(!Index[63])return -1;
			int a=__builtin_ctzl(Index[63]);
			return a<<6|__builtin_ctzl(Index[a]);
		}
		inline void bor(int t){
			Index[t>>6]^=1ull<<(t&63);t>>=6;
			if(!Index[t])Index[63]^=1ull<<t;
		}
		inline void ret(int t){
			Index[t>>6]|=1ull<<(t&63);t>>=6;
			Index[63]|=1ull<<t;
		}
	};//64*8=512Bytes
	// a small memory block is like:
	// [IndexObject 512B][Description 512B:(Item Size 8B)(Total Size 8B)(Next Block 8B)]
	// [DataArea]
	#define _up(ptr) reinterpret_cast<uintptr_t>(ptr)
	#define _small_Indexobj(ptr) reinterpret_cast<IndexObject*>(_up(ptr))
	#define _small_Itemsize(ptr) (*reinterpret_cast<uint64_t*>(_up(ptr)+512))
	#define _small_Totalsiz(ptr) (*reinterpret_cast<uint64_t*>(_up(ptr)+520))
	#define _small_Nxtblock(ptr) (*reinterpret_cast<void**>(_up(ptr)+528))
	#define _small_Datblock(ptr) (reinterpret_cast<void*>(_up(ptr)+1024))
	#define _small_Inblock(ptr,ptrblock) (_up(ptrblock)<=_up(ptr) &&  _up(ptr)<=(_up(ptrblock)+_small_Totalsiz(ptrblock)))
	// small blocks are blocks between size 16-256 Bytes(5 levels),variant from 64KiB block to ~1MiB block
	// for 512B - 1MiB (12 levels)blocks blocks are allocated per MiB and garbage managed with stl vectors
	// for >1MiB blocks are allocated dividually, and garbaged with stl vectors as well.
	void*SmallStarting[5];
	std::vector<void*>PVec[40];
	void*SmallBlockBuild(int Pow){
		size_t totsiz=1024+(63*64<<Pow);
		void*g=BigAlloc(totsiz);
		memset(g,0xFF,512);
		_small_Indexobj(g)->Index[63]=0x7fffffffffffffffull;
		_small_Itemsize(g)=1<<Pow;
		_small_Totalsiz(g)=totsiz;
		_small_Nxtblock(g)=SmallStarting[Pow-4];
		return g;
	}
	void*SmallAlloc(int Pow){
		void*RecentBlock=SmallStarting[Pow-4];
		while(RecentBlock){
			int t=_small_Indexobj(RecentBlock)->find();
			if(~t)return _small_Indexobj(RecentBlock)->bor(t),reinterpret_cast<void*>(_up(RecentBlock)+1024+(t<<Pow));
			RecentBlock=_small_Nxtblock(RecentBlock);
		}RecentBlock=SmallStarting[Pow-4]=SmallBlockBuild(Pow);
		return _small_Indexobj(RecentBlock)->bor(0),_small_Datblock(RecentBlock);
	}
	void SmallFree(int Pow,void*ptr){
		void*RecentBlock=SmallStarting[Pow-4];
		while(RecentBlock){if(_small_Inblock(ptr,RecentBlock)){
			int pos=_up(ptr)-_up(RecentBlock)-1024;
			if(pos&((1<<Pow)-1))PError(2,"ptr freed not valid: Out of align");
			memset(ptr,0,1<<Pow);
			_small_Indexobj(RecentBlock)->ret(pos>>Pow);
			return;
		}else RecentBlock=_small_Nxtblock(RecentBlock);}PError(2,"ptr freed not valid: Out of block");
	}
	void MediumBlockBuild(int Pow){
		void*g=BigAlloc(1<<20);
		int count=1<<(20-Pow);
		for(int i=0,_=Pow-9;i<count;++i)
			PVec[_].push_back(g),g=reinterpret_cast<void*>(_up(g)+(1<<Pow));
	}
	void*MediumAlloc(int Pow){
		if(PVec[Pow-9].empty())MediumBlockBuild(Pow);
		void*ret=PVec[Pow-9].back();PVec[Pow-9].pop_back();
		return ret;
	}
	void MediumFree(int Pow,void*ptr){
		PVec[Pow-9].push_back(ptr);
		memset(ptr,0,1<<Pow);
	}
	void*LargeAlloc(int Pow){
		if(PVec[Pow-9].empty())return BigAlloc(1<<Pow);
		void*ret=PVec[Pow-9].back();PVec[Pow-9].pop_back();
		return ret;
	}
	void LargeFree(int Pow,void*ptr){
		PVec[Pow-9].push_back(ptr);
		memset(ptr,0,1<<Pow);
	}
	int FitPow(size_t g){return 64-__builtin_clzl(g-1);}
	struct Allocator_pow2{
		template<class T>
		T* allocate(uint32_t&g){
			size_t p=g*sizeof(T);
			int pow;
			p=1<<(pow=FitPow(p<16?16:p));
			g=p/sizeof(T);
			T*Ret=NULL;
			if(pow>=20)Ret=(T*)LargeAlloc(pow);else
			if(pow>=9)Ret=(T*)MediumAlloc(pow);else
			Ret=(T*)SmallAlloc(pow);
//			printf("Allocated %lx, size %d\n",_up(Ret),1<<pow);
			fflush(stdout);
			return Ret;
		}
		template<class T>
		void release(T*p,uint32_t g){
			size_t z=g*sizeof(T);
			int pow=FitPow(z<16?16:z);
//			printf("Released %lx\n",_up(p));
			fflush(stdout);
			if(pow>=20)return LargeFree(pow,p);
			if(pow>=9)return MediumFree(pow,p);
			SmallFree(pow,p);
		}
	};
}

namespace __impl{
	typedef uint64_t uf;
	typedef uint32_t ui;
	const uf mask=0x00000000ffffffffull;
	template<class Allocator>
	struct bui{
		typedef bui<Allocator> self;
		ui siz,alc;
		uf*coe;
		Allocator all;
		bui():siz(0),alc(0),coe(0){}
		bui(Allocator all):siz(0),alc(0),coe(0),all(all){}
		bui(ui siz,ui alc,uf*coe,Allocator all):siz(siz),alc(alc),coe(coe),all(all){}
		inline self&Resize(ui to_size){
			uf*mov=all.template allocate<uf>(to_size);
			if(coe&&mov)memcpy(mov,coe,siz*sizeof(uf)),all.release(coe,alc);
			coe=mov,alc=to_size;
			return*this;
		}
		inline self&Extend(){return Resize(alc*2);}
		inline self&FitTo(ui to_size){
			if(to_size>alc*2)return Resize(to_size);else
			if(to_size>alc  )return Extend();else
			return*this;
		}
		inline self&From(ui t){FitTo(1);coe[0]=t,siz=1;return *this;}
		inline self&Print(){for(ui g=siz-1;~g;--g)printf("%08lx ",coe[g]);return *this;}
		uf*begin(){return coe;}
		uf*end(){return coe+siz;}
		uf Carry(){
			if(!siz)return 0;
			ui len=siz,ret=0;
			for(ui i=1;i<len;++i)
				coe[i]+=coe[i-1]>>32;
			ret=coe[len-1]>>32;
			for(ui i=0;i<len;++i)
				coe[i]&=mask;
			return ret;
		}
		self&Normalize(){
			uf g=Carry();
			if(g&&siz==alc)Extend();
			if(g)*end()=g,++siz;
			while(siz&&coe[--siz]==0);
			if(coe&&coe[siz])++siz;return *this;
		}
		self&Add(self t){
			FitTo(t.siz);
			for(ui g=0,_=t.siz;g<_;++g)
				coe[g]+=t.coe[g];
			siz=max(siz,t.siz);
			return *this;
		}
		self&Add(self a,self b){
			FitTo(max(a.siz,b.siz));
			memcpy(coe,a.coe,a.siz*sizeof(uf));
			for(ui i=0;i<a.siz&&i<b.siz;++i)
				coe[i]+=b.coe[i];
			for(ui i=a.siz;i<b.siz;++i)
				coe[i]=b.coe[i];
			siz=max(a.siz,b.siz);
			return *this;
		}
		inline bool smaller(self a,bool automatic_normalization=1){
			if(automatic_normalization)Normalize(),a.Normalize();
			if(siz<a.siz)return 1;if(siz>a.siz)return 0;
			for(ui t=siz-1;~t;--t)if(coe[t]<a.coe[t])return 1;else if(coe[t]>a.coe[t])return 0;
			return 0;
		}
		bool greater(self a,bool automatic_normalization=1){
			return a.smaller(*this,automatic_normalization);
		}
		self&Diff(self b,bool size_check=1){//make this the difference
			if(size_check&&smaller(b))return b.To_diff(*this,0);
			for(ui i=b.siz-1;~i;--i)coe[i]-=b.coe[i];
			return Normalize();
		}
		self&To_diff(self&b,bool size_check=1){//make b the difference
			if(size_check&&smaller(b))return b.Diff(*this,0);
			b.FitTo(alc),b.siz=siz;
			for(ui i=siz-1;~i;--i)b.coe[i]=coe[i]-b.coe[i];
			return b.Normalize();
		}
		self Cop(){
			self g(all);g.Resize(alc);
			memcpy(g.coe,coe,siz*sizeof(uf));
			return g;
		}
		self Cop(Allocator all){
			self g(all);g.Resize(alc);g.siz=siz;
			memcpy(g.coe,coe,siz*sizeof(uf));
			return g;
		}
		void Free(){
			all.release(coe,alc);
			coe=NULL;
			siz=alc=0;
		}
		self Unsafe_cut(ui Starting,ui Stoping){return self(Stoping-Starting,Stoping-Starting,coe+Starting,all);}
		inline self&Clear(){
			memset(coe,0,sizeof(uf)*siz);
			siz=0;return *this;
		}
	};

	#define tbal template<class Allocator>
	#define bal bui<Allocator>
	
	tbal bal Mult_All(bal&a,bal&b,Allocator all);
	tbal inline bal Mult_school(bal&a,bal&b,Allocator all){
		bal c(all);a.Normalize(),b.Normalize();c.Resize(a.siz+b.siz);
		for(int i=0;i<a.siz;++i)
			for(int j=0;j<b.siz;++j)
				c.coe[i+j]+=a.coe[i]*b.coe[j],
				c.coe[i+j+1]+=c.coe[i+j]>>32,
				c.coe[i+j]&=mask;
		c.siz=a.siz+b.siz;
		c.Normalize();
		return c;
	}
	#include "Multiplication_configuration.cpp"
	#include "Multiplication.cpp"
	
	#undef tbal
	#undef bal
}

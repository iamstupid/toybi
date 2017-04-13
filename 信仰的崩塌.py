# 高精度还是python好
from math import sqrt
from random import randint

isqrt_threshold_small=2**62
isqrt_threshold_medim=2**120

def i_sqrt_small(n):
	# assert(n<=isqrt_threshold_small)
	z=int(sqrt(float(n)))
	q=z*z
	qa=q+z+z+1
	if q>n:
		return z-1
	elif qa<=n:
		return z+1
	else:
		return z

def i_sqrt_medium(n):
	# assert(n<=isqrt_threshold_medim)
	z=int(sqrt(float(n)))
	z=(z+n/z)>>1
	q=z*z
	qa=q+z+z+1
	if q>n:
		return z-1
	elif qa<=n:
		return z+1
	else:
		return z

def i_sqrt_large(n):
	bl=n.bit_length()-50
	if bl&1:
		--bl
	zx=int(sqrt(float(n>>bl)))<<(bl/2) # initial guess
	nv=n/zx
	while 1:
		zx=(zx+nv)>>1
		nv=n/zx
		if nv<zx:
			nv,zx=zx,nv
		if nv-zx<=2:
			break
	zx=zx+1
	t=zx*zx
	t2=t+1+((zx+1)<<1)
	if t2<=n:
		return zx+1
	if t<=n:
		return zx
	return zx-1

def i_sqrt(n):
	if n<=isqrt_threshold_small:
		return i_sqrt_small(n)
	elif n<=isqrt_threshold_medim:
		return i_sqrt_medium(n)
	else:
		return i_sqrt_large(n)

"""
	# Test integer sqrt routines

	a=map(eval,raw_input().split(" "))

	for i in a:
		print i,i_sqrt(i)
	# print a
"""

def BinaryDecompose(Gp):
	Gt=(Gp&(-Gp)).bit_length()-1
	return (Gp>>Gt,Gt)

def Miller(a,n):
	# test (probably) primality
	# of n
	Gp,Gt=BinaryDecompose(n-1)
	a=pow(a,Gp,n)
	if a==1 or a==n-1:
		return 1
	for i in xrange(0,Gt-1):
		a=a*a%n
		if a==1:
			return 0
		if a==n-1:
			return 1
	if a!=n-1:
		return 0
	return 1

def Miller_Rabin_BPSW(n):
	if n==2:
		return 1
	if n&1==0:
		return 0
	return Miller(2,n)

def Linear_Sieve(n):
	a=[0 for i in xrange(0,n+1)]
	pr=[]
	a[0]=1
	a[1]=1
	for i in xrange(2,n+1):
		if not a[i]:
			pr.append(i)
		for j in pr:
			if i*j>n:
				break
			a[i*j]=1
			if i%j==0:
				break
	return (a,pr)

sim_prr,sim_primes=Linear_Sieve(10000)

def Miller_Rabin(n,_base=[2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61]):
	if n<len(sim_prr):
		return not sim_prr[n]
	for i in xrange(0,min(len(sim_primes),20)):
		if not (n%sim_primes[i]):
			return False
	for i in _base:
		if not Miller(i,n):
			return False
	return True

def Range_Sieve(L,R):
	tt=i_sqrt_small(R)
	a,pr=Linear_Sieve(tt)
	len=R-L+1
	siv=[0]*len
	for i in pr:
		for j in xrange(L+(i-L)%i,R+1,i):
			siv[j-L]=1
	return siv

def Sieve_To_Prime(siv,off):
	pr=[]
	for i in siv:
		if not i:
			pr.append(off)
		off=off+1
	return pr

def Trial_Division(n,prs=[]):
	if len(prs)==0:
		a,prs=Linear_Sieve(i_sqrt_small(n))
	fact=[]
	for i in prs:
		if n%i==0:
			ans=1
			n=n/i
			while n%i==0:
				n=n/i
				ans=ans+1
			fact.append((i,ans))
		if n==1:
			break
	if n:
		fact.append((n,1))
	return fact

def gcd_euclid(a,b):
	if not b:
		return a
	return gcd_euclid(b,a%b)

def gcd_bin_rec(a,b):
	if a==b:
		return a
	if a==0:
		return b
	if b==0:
		return a
	if a&1:
		if b&1:
			if a>b:
				return gcd_bin_rec(a-b>>1,b)
			else:
				return gcd_bin_rec(b-a>>1,a)
		else:
			return gcd_bin_rec(a,b>>1)
	else:
		if b&1:
			return gcd_bin_rec(a>>1,b)
		else:
			return gcd_bin_rec(a>>1,b>>1)<<1

def gcd_binary(a,b):
	if a<0:
		a=-a
	if b<0:
		b=-b
	return gcd_bin_rec(a,b)

def exgcd(a,b):
	if not b:
		return (a,1,0)
	A,B,C=exgcd(b,a%b)
	return (A,C,B-a/b*C)

def Modular_Inverse(a,n):
	t,i,j=exgcd(a,n)
	if t!=1:
		return -1
	return i%n

def Jacobi_2(b):
	if (b&7)==1:
		return 1
	if (b&7)==7:
		return 1
	return -1

def Recurse_Jacobi(a,b):
	a=a%b
	if b==1:
		return 1
	mul=1
	pg=Jacobi_2(b)
	while not (a&1):
		a>>=1
		mul=mul*pg
	if a==1:
		return mul
	t=Recurse_Jacobi(b,a)
	if (a&3)==1 or (b&3)==1:
		return mul*t
	return -mul*t

def Jacobi(a,b):
	t=gcd_binary(a,b)
	if t!=1:
		return 0;
	return Recurse_Jacobi(a,b)

def Lucas_Delta(n,D):
	Jac=Jacobi(D,n)
	return n-Jac

def Lucas_Double(n,STATE,P,Q,D):
	u,v,q=STATE
	return (u*v%n,(v*v-2*q)%n,q*q%n)

def ModularDiv2(a,n):
	return ((a*(n+1))>>1)%n # n is odd, not checked

def Lucas_Nextst(n,STATE,P,Q,D):
	u,v,q=STATE
	return (ModularDiv2(P*u+v,n),ModularDiv2(D*u+P*v,n),q*Q%n)

def Lucas_Eth(n,P,Q,D,e):
	# e>0, not checked
	if e==1:
		return (1,P,Q)
	else:
		T=Lucas_Eth(n,P,Q,D,e>>1)
		T=Lucas_Double(n,T,P,Q,D)
		if e&1:
			T=Lucas_Nextst(n,T,P,Q,D)
		return T

def Lucas(n,P,Q,D):
	# D=P^2-4Q, not checked;
	# gcd(n,D)=1, not checked;
	# n odd, not checked;
	d,r=BinaryDecompose(Lucas_Delta(n,D))
	# usuallu we let Q=1 and (D/n)=-1
	u,v,q=Lucas_Eth(n,P,Q,D,d)
	if u==0:
		return True
	ST=(u,v,q)
	for i in xrange(0,r):
		if v==0:
			return True
		u,v,q=Lucas_Double(n,(u,v,q),P,Q,D)
	return False

def PerfectSquare(n):
	return i_sqrt(n)**2==n

def BPSW(n): # presents a Baillie-PSW Probable Prime Test which has very high probability to be correct
	if n<len(sim_prr):
		return not sim_prr[n]
	for i in xrange(0,min(len(sim_primes),20)):
		if not (n%sim_primes[i]):
			return False
	if not Miller(2,n):
		return False
	if PerfectSquare(n):
		return False
	P,D,d=1,-7,-2
	while Jacobi(D,n)!=-1:
		D=-(D+d)
		d=-d
	Q=(1-D)/4
	if not Lucas(n,P,Q,D):
		return False
	return True

def getNonResidue(P):
	i=randint(0,P-1)
	while Jacobi(i,P)!=-1:
		i=randint(0,P-1)
	return i

def TonelliShanks(n,P):
	# P is prime, not checked
	# (n/P)=1, not checked
	Q,S=BinaryDecompose(P-1)
	if S==1:
		return pow(n,(P+1)>>2,P)
	z=getNonResidue(P)
	c=pow(z,Q,P)
	R,t,M=pow(n,(Q+1)/2,P),pow(n,Q,P),S
	while M>1:
		if t==1:
			return R
		i,tt=1,t*t%P
		while i<M:
			if tt==1:
				break
			i=i+1
			tt=tt*tt%P
		b=pow(c,1<<M-i-1,P)
		R,c=R*b%P,b*b%P
		t=t*c%P
		M=i
	if t==1:
		return R
	return -1

"""
def Main_QuadResidue(n,P):
	if Jacobi(n,P)!=1:
		print "No residue"
		return
	a=TonelliShanks(n,P)
	print "%d^2(%d)=%d(mod %d)"%(a,a*a%P,n,P)

# helper function
"""

# (OmegaSq/P)=-1

def Fpsq_add(STa,STb,P,OmegaSq):
	a,b=STa
	c,d=STb
	return ((a+c)%P,(b+d)%P)

def Fpsq_mul(STa,STb,P,OmegaSq):
	a,b=STa
	c,d=STb
	return ((a*c+b*d*OmegaSq)%P,(a*d+b*c)%P)

def Fpsq_pow(STa,e,P,OmegaSq):
	ans=(1,0)
	while e:
		if e&1:
			ans=Fpsq_mul(ans,STa,P,OmegaSq)
		STa=Fpsq_mul(STa,STa,P,OmegaSq)
		e=e>>1
	return ans

def Cipolla(n,P):
	# P is prime, not checked
	# (n/P)=1, not checked
	i=randint(0,P-1)
	while Jacobi(i*i-n,P)!=-1:
		i=randint(0,P-1)
	j=(i*i-n)%P
	ta,tb=Fpsq_pow((i,1),(P+1)/2,P,j)
	return ta


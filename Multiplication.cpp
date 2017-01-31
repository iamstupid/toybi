tbal bal Mult_karats(bal&a,bal&b,Allocator all){
	a.Normalize(),b.Normalize();
	if(min(a.siz,b.siz)<karat_bal_thresh)return Mult_school(a,b,all);
//	printf("%u\n",a.siz);fflush(stdout);
	ui n=max(a.siz,b.siz);
	a.FitTo(n),b.FitTo(n);
//	if(n>10000)printf("%d\n",n),fflush(stdout);
	ui half_down=n>>1;
	ui half_up=n-half_down,sign=0;
	bal A1=a.Unsafe_cut(0,half_up).Normalize(),A2=a.Unsafe_cut(half_up,n).Normalize();
	bal c=A1.smaller(A2)?sign=~0,A2.Cop(all).Diff(A1,0):A1.Cop(all).Diff(A2,0);
	bal B1=b.Unsafe_cut(0,half_up).Normalize(),B2=b.Unsafe_cut(half_up,n).Normalize();
	bal d=B1.smaller(B2)?sign=~sign,B2.Cop(all).Diff(B1,0):B1.Cop(all).Diff(B2,0);
	bal t;
	if(half_down<karat_bal_thresh){
		if(half_up<karat_bal_thresh) t=Mult_school(c,d,all),A1=Mult_school(A1,B1,all);
		else t=Mult_karats(c,d,all),A1=Mult_karats(A1,B1,all);
		A2=Mult_school(A2,B2,all);
	}else t=Mult_karats(c,d,all),A1=Mult_karats(A1,B1,all),A2=Mult_karats(A2,B2,all);
	c.Add(A1,A2);
	if(sign)t.Add(c);else c.To_diff(t,0);c.Free();
	d.FitTo(n*2).Clear().Unsafe_cut(0,A1.siz).Add(A1);
	d.Unsafe_cut(half_up,half_up+t.siz).Add(t);
	d.Unsafe_cut(half_up*2,half_up*2+A2.siz).Add(A2);
	A1.Free(),A2.Free(),t.Free();
	d.siz=n*2;
	return d.Normalize();
}
tbal bal Mult_toom33(bal&a,bal&b,Allocator all){
	// -- initialize
	a.Normalize(),b.Normalize();
	if(min(a.siz,b.siz)<karat_bal_thresh)return Mult_school(a,b,all);
	// -- calculate slicing arguments and prepare space req
	ui n=max(a.siz,b.siz);
//	printf("%d\n",n);fflush(stdout);
	ui slice_down=n/3;
	ui slice=slice_down+(slice_down*3<n);
	a.FitTo(slice*3),b.FitTo(slice*3);
	// -- slice
	bal U0=a.Unsafe_cut(0,slice),U1=a.Unsafe_cut(slice,slice*2),U2=a.Unsafe_cut(slice*2,n);// U0,U1,U2 unsafe norm
	bal V0=b.Unsafe_cut(0,slice),V1=b.Unsafe_cut(slice,slice*2),V2=b.Unsafe_cut(slice*2,n);// V0,V1,V2 unsafe norm
	// -- evaluation
	bal W3=U0.Cop(all).Add(U2),W2=V0.Cop(all).Add(V2);// W3,W2 safe quirk
	ui sigW0=0,sigW1=0,sigW3=0,sigW4=0,sigW2=0;
	bal W0=W3.Cop(all),W4=W2.Cop(all);
	W0=SigSubTo(sigW0,W0,0,U1);
	W4=SigSubTo(sigW4,W4,0,V1);
	W3.Add(U1),W2.Add(V1);//W3,W2 safe quirk
	bal W1=slice<toom33_thresh?Mult_karats(W3,W2,all):Mult_toom33(W3,W2,all);//W3,W2,W1 safe norm
	sigW2=sigW0^sigW4;
	W2.Free();W2=slice<toom33_thresh?Mult_karats(W0,W4,all):Mult_toom33(W0,W4,all);// W2,W0,W4 safe norm
	SigAddTo(sigW0,W0,0,U2).mul2();SigSubTo(sigW0,W0,0,U0);
	SigAddTo(sigW4,W4,0,V2).mul2();SigSubTo(sigW4,W4,0,V0);
	sigW3=sigW0^sigW4;sigW0=sigW4=0;
	W3.Free();W3=slice<toom33_thresh?Mult_karats(W0,W4,all):Mult_toom33(W0,W4,all);U0=U0.Cop(all),V0=V0.Cop(all);
	W0.Free();W0=slice<toom33_thresh?Mult_karats(U0,V0,all):Mult_toom33(U0,V0,all);U2=U2.Cop(all),V2=V2.Cop(all),U0.Free(),V0.Free();
	W4.Free();W4=slice<toom33_thresh?Mult_karats(U2,V2,all):Mult_toom33(U2,V2,all);U2.Free(),V2.Free();
	// W0-W4 safe norm
	// -- interpolation
	W3=SigSubTo(sigW3,W3,sigW1,W1).toom3_div3();
	W1=SigSubTo(sigW1,W1,sigW2,W2).div2();
	W2=SigSubTo(sigW2,W2,0,W0);
	W3=SigSubTo(sigW3,W3,sigW2,W2).div2();sigW3=~sigW3;
	U0=W4.Cop().mul2();
	W3=SigAddTo(sigW3,W3,sigW4,U0);U0.Free();
	W2=SigAddTo(sigW2,W2,sigW1,W1);
	// -- recomposition
	W0.Normalize(),W1.Normalize(),W2.Normalize(),W3.Normalize(),W4.Normalize();
	U0.FitTo(n*2);
	U0.Unsafe_cut(slice*4,slice*4+W4.siz).Add(W4);
	sigW3?U0.Unsafe_cut(slice*3,slice*3+W3.siz).Diff(W3):U0.Unsafe_cut(slice*3,slice*3+W3.siz).Add(W3);
	W2=SigSubTo(sigW2,W2,sigW4,W4);W1=SigSubTo(sigW1,W1,sigW3,W3);
	sigW2?U0.Unsafe_cut(slice*2,slice*2+W2.siz).Diff(W2):U0.Unsafe_cut(slice*2,slice*2+W2.siz).Add(W2);
	sigW1?U0.Unsafe_cut(slice*1,slice*1+W1.siz).Diff(W1):U0.Unsafe_cut(slice*1,slice*1+W1.siz).Add(W1);
	U0.Unsafe_cut(0,W0.siz).Add(W0);
	W0.Free(),W1.Free(),W2.Free(),W3.Free(),W4.Free();
	U0.siz=n*2;
	return U0.Normalize();
}

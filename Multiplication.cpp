tbal bal Mult_karats(bal&a,bal&b,Allocator all){
	a.Normalize(),b.Normalize();
	if(min(a.siz,b.siz)<karat_bal_thresh)return Mult_school(a,b,all);
//	printf("%u\n",a.siz);fflush(stdout);
	ui n=max(a.siz,b.siz);
	a.FitTo(n),b.FitTo(n);
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

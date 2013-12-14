
typedef __int64 I;

struct FS{
	I p,q;
	FS(int n):p(n),q(1){}
	FS(){}
	bool operator<(const FS &b)const{
		return p*b.q<q*b.p;
	}
	bool operator==(const int n)const{
		return p==n && q==1;
	}
};

I gcd(I a,I b)
{
	if(b==0)return a;
	return gcd(b,a%b);
}

void yue(FS &a)
{
	I d=abs(gcd(a.p,a.q));
	a.p/=d;a.q/=d;
}

FS jia(FS a,FS b)
{
	FS r;
	I d=gcd(a.q,b.q),ca=b.q/d,cb=a.q/d;
	r.p=a.p*ca+b.p*cb;r.q=ca*a.q;
	yue(r);
	return r;
}

FS jian(FS a,FS b)
{
	FS r;
	I d=gcd(a.q,b.q),ca=b.q/d,cb=a.q/d;
	r.p=a.p*ca-b.p*cb;r.q=ca*a.q;
	yue(r);
	return r;
}

FS cheng(FS a,FS b)
{
	FS r;
	I d1=gcd(b.p,a.q),d2=gcd(a.p,b.q);
	r.p=(a.p/d2)*(b.p/d1);
	r.q=(a.q/d1)*(b.q/d2);
	if(a.q<0){a.p=-a.p;a.q=-a.q;}
	return r;
}

FS chu(FS a,FS b)
{
	FS r;
	I d1=gcd(b.q,a.q),d2=gcd(a.p,b.p);
	r.p=(a.p/d2)*(b.q/d1);
	r.q=(a.q/d1)*(b.p/d2);
	if(r.q<0){r.q=-r.q;r.p=-r.p;}
	return r;
}

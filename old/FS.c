
typedef __int64 I;

typedef struct{
	I p,q;
}FS;

int ANS[100],y;

I gcd(I a,I b)
{
	if(b==0)return a;
	return gcd(b,a%b);
}

void yue(FS *a)
{
	I d;
	d=gcd(a->p,a->q);
	if(d<0)d=-d;
	a->p/=d;a->q/=d;
}

FS jian(FS a,FS b)
{
	FS r;
	r.p=a.p*b.q-a.q*b.p;
	r.q=a.q*b.q;
	yue(&r);
	return r;
}
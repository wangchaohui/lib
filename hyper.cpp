#include <stdio.h>
#include <algorithm>
using namespace std;

typedef long long I;

void mo(int a,int m,int &l,int &r) //a^x=a^(l+x%r) (mod m) (x>=l)
{
	I x=1,y=1;
	l=0;
	do
	{
		x=x*a%m;
		y=y*a%m*a%m;
		l++;
	}while(x!=y);
	r=0;
	do 
	{
		y=y*a%m;
		r++;
	}while(x!=y);
}

int EXP(int a,int b)
{
	int r=1;
	for(;b;b--)r*=a;
	return r;
}

int MODULAR_EXPONENTIATION(int a,int b,int n)
{
	int bb[64];
	I d=1;
	int i;
	for(i=0,bb[0]=b%2,b/=2;b;bb[++i]=b%2,b/=2);
	do{
		d=(d*d)%n;
		if(bb[i]==1)
			d=(d*a)%n;
	}while(--i>=0);
	return (int)d;
}

int da(int n,int *d,int a) // if d[0]^d[1]^d[2]^.....^d[n]>=a   /* 0^0=1 */
{
	I x=1;
	int i;
	if(n==0 || a==0 || d[0]==1)return d[0]>=a;
	if(d[0]==0)return (!da(n-1,d+1,1))>=a;
	for(i=0;x<a;i++)x*=d[0];
	return da(n-1,d+1,i);
}

int dpowerb(int n,int *d)
{
	if(n==0 || *d==1)return *d;
	if(d[0]==0)return (!da(n-1,d+1,1));
	return EXP(*d,dpowerb(n-1,d+1));
}

int dpower(int n,int *d,int m)
{
	int l,r,t;
	if(m==1)return 0;
	if(n==0)return *d%m;
	mo(d[0],m,l,r);
	if(da(n-1,d+1,l))
	{
		t=(dpower(n-1,d+1,r)-l)%r;if(t<0)t+=r;
		return MODULAR_EXPONENTIATION(*d,t+l,m);
	}
	else
		return MODULAR_EXPONENTIATION(*d,dpowerb(n-1,d+1),m);
}

int p[200];

void factorization(int n)
{
	int i;
	p[0]=0;
	for(i=2;i*i<=n;i++)
		if(n%i==0)
		{
			p[++p[0]]=i;
			while(n%i==0)n/=i;
		}
		if(n>1)p[++p[0]]=n;
}

int Phi(int n)
{
	int i;
	factorization(n);
	for(i=1;i<=p[0];i++)
		n=n/p[i]*(p[i]-1);
	return n;
}

int dpower(int n,int *d,int m)
{
	int p=Phi(m);
	if(m==1)return 0;
	if(n==0)return *d%m;
	if(da(n-1,d+1,p))
	{
		return MODULAR_EXPONENTIATION(*d,dpower(n-1,d+1,p)+p,m);
	}
	else
		return MODULAR_EXPONENTIATION(*d,dpowerb(n-1,d+1),m);
}

int da4(int a,int b,int x) // if hyper4(a,b)>=x
{
	I y=1;
	int i;
	if(b==0 || x==0 || a==1)return 1>=x;
	if(a==0)return ((b+1)%2)>=x;
	for(i=0;y<x;i++)y*=a;
	return da4(a,b-1,i);
}

int hyper4b(int a,int b)
{
	if(b==0)return 1;
	return EXP(a,hyper4b(a,b-1));
}

int hyper4(int a,int b,int m)
{
	int l,r,t;
	if(m==1)return 0;
	if(b==0)return 1;
	if(b==1)return a%m;
	if(a==0)return (b+1)%2;
	mo(a,m,l,r);
	if(da4(a,b-1,l))
	{
		t=(hyper4(a,b-1,r)-l)%r;if(t<0)t+=r;
		return MODULAR_EXPONENTIATION(a,t+l,m);
	}
	else
		return MODULAR_EXPONENTIATION(a,hyper4b(a,b-1),m);
}

int hyperda(int n,int a,int b,int x) // if hypern(a,b)>=x
{
	int i;
	if(x==0)return 1;
	if(n==0)return b+1>=x;
	if(n==1)return a+b>=x;
	if(n==2)return a*b>=x;
	if(b==0 || a==1)return 1>=x;
	if(a==0)return n>3 && ((b+1)%2)>=x;
	for(i=0;!hyperda(n-1,a,i,x);i++);
	return hyperda(n,a,b-1,i);
}

int lim(int a,int m) // a!=0
{
	int l,r,i,x=1;
	if(m==1)return 0;
	mo(a,m,l,r);
	for(i=0;!da4(a,i,l);i++);
	return max(i+1,lim(a,r)+1);
}

int chong(int n,int a,int m)
{
	int g,i;
	if(n==4)return lim(a,m);
	g=chong(n-1,a,m);
	for(i=0;!hyperda(n,a,i,g);i++);
	return i+1;
}
/*
a 0-1000 m 1-10000
4 10
5 4
6 3
7 3
8 3
9 3
10 3
*/

int hyperb(int n,int a,int b)
{
	while(1)
	{
		if(n==0)return (b+1);
		if(n==1)return (a+b);
		if(n==2)return (a*b);
		if(n==3)return EXP(a,b);
		if(n==4)return hyper4b(a,b);
		if(b==0)return 1;
		b=hyperb(n,a,b-1);
		n--;
	}
}

int hyper(int n,int a,int b,int m)
{
	int t;
	while(1)
	{
		if(n==0)return (b+1)%m;
		if(n==1)return (a+b)%m;
		if(n==2)return (a*b)%m;
		if(n==3)return MODULAR_EXPONENTIATION(a,b,m);
		if(n==4)return hyper4(a,b,m);
		if(b==0)return 1%m;
		if(a==0)return ((b+1)%2)%m;
		t=chong(n,a,m);
		if(b>=t)return hyper4(a,lim(a,m),m);
		b=hyperb(n,a,b-1);
		n--;
	}
}

int main()
{
	int T,C=0,m,n,a,b;
	scanf("%d",&T);
	while(++C<=T)
	{
		scanf("%d %d %d %d",&m,&n,&a,&b);
		printf("Case #%d: %d\n",C,hyper(n,a,b,m));
	}
	return 0;
}

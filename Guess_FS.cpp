#include <stdio.h>
#include <math.h>
#include <algorithm>
using namespace std;

#define N 20

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
	if(r.q<0){r.q=-r.q;r.p=-r.p;}
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

FS A[N+1][N+1],B[N+1];

int Gauss(int n)
{
	int i,j,k,mi;
	FS m,t;
	for(k=1;k<=n;k++)
	{
		m=0;
		for(i=k;i<=n;i++)
		{
			t=A[i][k];
			if(t.p<0)t.p=-t.p;
			if(m<t){m=t;mi=i;}
		}
		if(m==0)return -1;
		if(mi!=k)
		{
			for(j=k;j<=n;j++)
				swap(A[k][j],A[mi][j]);
			swap(B[k],B[mi]);
		}
		m=A[k][k];
		for(j=k;j<=n;j++)
			A[k][j]=chu(A[k][j],m);
		B[k]=chu(B[k],m);
		for(i=k+1;i<=n;i++)
		{
			B[i]=jian(B[i],cheng(A[i][k],B[k]));
			for(j=n;j>=k;j--)
				A[i][j]=jian(A[i][j],cheng(A[i][k],A[k][j]));
		}
	}
	for(i=n-1;i>=1;i--)
		for(j=i+1;j<=n;j++)
			B[i]=jian(B[i],cheng(A[i][j],B[j]));
	return 0;
}

int main()
{
	int i,j,n;
	I q;
	while(scanf("%d",&n)==1)
	{
		for(i=1;i<=n;i++)
		{
			scanf("%I64d",&B[i].p);
			B[i].q=1;
		}
		for(i=1;i<=n;i++)
			for(j=1;j<=n;j++)
			{
				scanf("%I64d",&A[j][i].p);
				A[i][j].q=1;
			}
		if(Gauss(n)!=0)
		{
			puts("NONE");
			continue;
		}
		for(i=1;i<=n;i++)
			if(B[i].p<0)break;
		if(i!=n+1)
		{
			puts("NONE");
			continue;
		}
		q=1;
		for(i=1;i<=n;i++)
			q=q*(B[i].q/gcd(q,B[i].q));
		for(i=1;i<=n;i++)
			printf("%I64d ",B[i].p*(q/B[i].q));
		printf("%I64d\n",q);
	}
	return 0;
}

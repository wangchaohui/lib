#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

typedef __int64 I;

int MODULAR_EXPONENTIATION(int a,int b,int n);
int WITNESS(int a,int n);
int MILLER_RABIN(int n,int s);
int STT(int n);
int isprime(int n);

int main()
{
	int n;
	//srand((unsigned)time(NULL));
	while(scanf("%d",&n)!=EOF)
	{
		if(isprime(n))
			printf("YES\n");
		else
			printf("NO\n");
	}
	return 0;
}

int isprime(int n)
{
	int t;
	if(n<2)
		return 0;
	t=STT(n);
	if(t==2)return 1;
	if(t==1 && MODULAR_EXPONENTIATION(2,n-1,n)==1 && MILLER_RABIN(n,50))
		return 1;
	return 0;
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

int WITNESS(int a,int n)
{
	int t,u,i;
	I x,xi;
	for(u=n-1,t=0;u%2==0;u/=2,t++);
	x=MODULAR_EXPONENTIATION(a,u,n);
	for(i=1;i<=t;i++)
	{
		xi=(x*x)%n;
		if(xi==1 && x!=1 && x!=n-1)
			return 1;
		x=xi;
	}
	if(xi!=1)
		return 1;
	return 0;
}

int MILLER_RABIN(int n,int s)
{
	int j,a,c[4]={0,2,7,61};
	for(j=1;j<=3;j++)
	{
		//a=(int)((double)rand()/RAND_MAX*(n-2))+1;
		if(c[j]<n && WITNESS(c[j],n))
			return 0;
	}
	return 1;
}

int STT(int n)
{
	int s[26]={0,2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
	int a;
	for(a=1;a<=46;a++)
		if(n==s[a])
			return 2;
		else if(n%s[a]==0)
			return 0;
	return 1;
}
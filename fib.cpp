#include <stdio.h>
#include <string.h>
#include <cmath>
#define MAXN 20000
 
typedef long long I;
typedef int BG[20000];
 
#define DIGITS 4
#define BASE 10000
 
int isone(int *x)
{
	return x[0]==1 && x[1]==1;
}
 
int iszero(int *x)
{
	return x[0]==1 && x[1]==0;
}
 
int* toone(int *x)
{
	x[0]=1;x[1]=1;
	return x;
}
 
int* tozero(int *x)
{
	x[0]=1;x[1]=0;
	return x;
}
 
void prbig(int *x)
{
	int a;
	printf("%d",x[x[0]]);
	for(a=x[0]-1;a>=1;a--)
		printf("%0*d",DIGITS,x[a]);
}
 
int* mulint(int *x,int n)
{
	int a,c=0;
	I t;
	if(n==0)return tozero(x);
	for(a=1;a<=x[0];a++){t=(I)x[a]*n+c;c=t/BASE;x[a]=t%BASE;}
	for(;c;c/=BASE)x[++x[0]]=c%BASE;
	return x;
}
 
void incbig(int *x,int *n)
{
	int a,l;
	if(n[0]==1 && n[1]==0)return;
	l=x[0]>n[0]?x[0]+1:n[0]+1;
	for(a=x[0]+1;a<=l;a++)x[a]=0;
	for(a=1;a<=n[0];a++)x[a]+=n[a];
	for(a=1;a<l;a++)
		if(x[a]>=BASE){x[a]-=BASE;x[a+1]++;}
		x[0]=x[l]?l:l-1;
}
 
void decbig(int *x,int *y)
{
	int a;
	for(a=1;a<=y[0];a++)x[a]-=y[a];
	for(a=1;a<x[0];a++)
		if(x[a]<0){x[a]+=BASE;x[a+1]--;}
		for(;x[0]>1 && x[x[0]]==0;x[0]--);
}
 
void addbig(int *x,int *y,int *z)
{
	int a,l;
	l=x[0]>y[0]?x[0]+1:y[0]+1;
	for(a=x[0]+1;a<=l;a++)z[a]=0;
	for(a=1;a<=x[0];a++)z[a]=x[a];
	for(a=1;a<=y[0];a++)z[a]+=y[a];
	for(a=1;a<l;a++)
		if(z[a]>=BASE){z[a]-=BASE;z[a+1]++;}
		z[0]=z[l]?l:l-1;
}
 
 
int cmpbig(int *x,int *y)
{
	int a;
	if(x[0]!=y[0])return x[0]-y[0];
	for(a=x[0];a>=1;a--)
		if(x[a]!=y[a])return x[a]-y[a];
	return 0;
}
 
 
 
void cpybig(int *x,int *y)
{
	int i;
	for(i=0;i<=y[0];i++)x[i]=y[i];
}
 
void mulbig(int *x,int *y,int *z)
{
	int a,b,l;
	l=x[0]+y[0];
	for(a=0;a<=l;a++)z[a]=0;
	for(a=1;a<=x[0];a++)
	{
		for(b=1;b<=y[0];b++)
		{
			z[a+b-1]+=x[a]*y[b]; if(z[a+b-1]>=BASE){z[a+b]+=z[a+b-1]/BASE;z[a+b-1]%=BASE;}
		}
	}
	for(z[0]=l;z[0]>1 && z[z[0]]==0;z[0]--);
}
 
 
void fib(int n){
	static BG  pre , next ,ret ;
	static BG t1,t2;
	if(n==0)
	{
		printf("1\n");return;
	}
	toone(pre);toone(next);toone(ret);
	int step=1,tmp=n;
	while(tmp >>= 1)
		step <<= 1;
	while (step >>= 1)
	{
		if(n&step){
			mulbig(ret,ret,t1);
			mulbig(next,next,t2);
			addbig(t1,t2,pre);
			mulint(ret,2);incbig(ret,next);
			mulbig(ret,next,t1);
			cpybig(next,t1);
		}
		else{
			cpybig(t1,next);
			mulint(t1,2);
			decbig(t1,ret);
			mulbig(t1,ret,pre);
			mulbig(ret,ret,t1);
			mulbig(next,next,t2);
			addbig(t1,t2,next);
		}
		cpybig(ret, pre);
	}
	prbig(ret);putchar('\n');
}
 
char a[MAXN],b[MAXN];
const double c = log10((sqrt(5.0)+1)/2);
int gcd(int a,int b){ return b==0?a:gcd(b,a%b); }
int main(){
	int t,t1,t2,i,k;
	double d;
	scanf("%d",&t);
	while(t--){
		scanf("%s%s",a,b);
		t1 = strlen(a);
		for(i=d=0,k=1;i < 9 && i < t1;i++,k*=10)
			d += ((a[i]-'0')+0.0)/k;
		t1 = (log10(d)+t1-1)/c+1.5;
		t2 = strlen(b);
		for(i=d=0,k=1;i < 9 && i < t2;i++,k*=10)
			d += ((b[i]-'0')+0.0)/k;
		t2 = (log10(d)+t2-1)/c+1.5;
		k = gcd(t1+1,t2+1);
		fib(k);
	}
	return 0;
}

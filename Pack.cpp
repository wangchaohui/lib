#include <stdio.h>
#include <memory.h>
template<class T> inline void checkmin(T &a,T b){if(b<a) a=b;}//NOTES:checkmin( 
template<class T> inline void checkmax(T &a,T b){if(b>a) a=b;}//NOTES:checkmax( 

int n[20],D[20],V;
int f[100001];

void ZeroOnePack(int cost,int value)
{
	int i;
	for(i=V;i>=cost;i--)
		checkmax(f[i],f[i-cost]+value);
}

void CompletePack(int cost,int value)
{
	int i;
	for(i=cost;i<=V;i++)
		checkmax(f[i],f[i-cost]+value);
}

void MultiplePack(int cost,int value,int amount)
{
	int k=1;
	if(cost*amount>=V)
	{
		CompletePack(cost,value);
		return;
	}
	while(k<amount)
	{
		ZeroOnePack(k*cost,k*value);
		amount-=k;
		k*=2;
	}
	ZeroOnePack(amount*cost,amount*value);
}

int main()
{
	int cash,N,i,j,k,b;
	while(scanf("%d %d",&cash,&N)==2)
	{
		for(i=0;i<N;i++)scanf("%d %d",&n[i],&D[i]);
		memset(f,0,sizeof(f));
		V=cash;
		for(i=0;i<N;i++)
		{
			MultiplePack(D[i],D[i],n[i]);
		}
		printf("%d\n",f[cash]);
	}
	return 0;
}
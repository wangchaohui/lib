/*
ID: wangcha9
PROB: humble
LANG: C++
*/
#include <stdio.h>
#include <limits.h>
#include <algorithm>
using namespace std;

int d[2000002],u[101];

int Humble_number(int c,int *u,int limit)
{
	int a,p,q,n,i;
	n=1;d[0]=1;
	for(i=0;i<c;i++)
	{
		q=0;
		while(q<n)
		{
			p=q;q=n;
			for(a=p;a<q;a++)
				if(d[a]<=limit/u[i])
					d[n++]=d[a]*u[i];
		}
	}
	return n;
}

int main()
{
	int k,n,i,c,limit;
	freopen("humble.in","r",stdin);
	freopen("humble.out","w",stdout);
	scanf("%d %d",&k,&n);
	for(i=0;i<k;i++)scanf("%d",&u[i]);
	limit=1000000;
	do
	{
		c=Humble_number(k,u,limit);
		limit*=10;
	}while(c<=n);
	sort(d,d+c);
	printf("%d\n",d[n]);
	return 0;
}

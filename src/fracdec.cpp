/*
ID: wangcha9
PROB: fracdec
LANG: C++
*/
#include <stdio.h>

int f[100001];
char dd[100001];

int main()
{
	int n,d,t,i,j,c;
	freopen("fracdec.in","r",stdin);
	freopen("fracdec.out","w",stdout);
	scanf("%d %d",&n,&d);
	t=n/d;
	c=printf("%d.",t);
	n-=d*t;
	for(i=1;;i++)
	{
		if(n==0)
		{
			for(j=1;j<i;j++)
			{
				if(++c==76){putchar('\n');c=0;}
				printf("%d",dd[j]);
			}
			if(i==1)
			{
				if(++c==76){putchar('\n');c=0;}
				putchar('0');
			}
			putchar('\n');break;
		}
		if(f[n])
		{
			for(j=1;j<f[n];j++)
			{
				if(++c==76){putchar('\n');c=0;}
				printf("%d",dd[j]);
			}
			putchar('(');
			for(j=f[n];j<i;j++)
			{
				if(++c==76){putchar('\n');c=0;}
				printf("%d",dd[j]);
			}
			putchar(')');putchar('\n');
			break;
		}
		f[n]=i;
		n*=10;
		t=n/d;
		n-=t*d;
		dd[i]=t;
	}
	return 0;
}
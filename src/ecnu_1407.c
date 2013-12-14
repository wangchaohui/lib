#include <stdio.h>
#include <memory.h>

	int d[1000007];

int main()
{
	int n,a,t,q;
	freopen("1407_in.txt","r",stdin);
	while(scanf("%d",&n)!=EOF)
	{
		memset(d,0,sizeof(d));
		for(a=1;a<=n;a++)
			scanf("%d",&d[a]);
		scanf("%d",&t);
		if(t>n)n=t;
		for(a=1;a<=t;a++)
		{
			scanf("%d",&q);
			d[a]+=q;
		}
		for(a=1;a<=n;a++)
		{
			if(d[a]>0 && d[a+1]>0 && d[a+2]==0)
			{
				d[a]--;
				d[a+1]--;
				d[a+2]++;
			}
			if(d[a]==2)
			{
				if(a!=1 && a!=2)
					d[a-2]++;
				else if(a==2)
					d[a-1]++;
				d[a]=0;
				d[a+1]++;
			}
		}
		if(d[n+2])
			n=n+2;
		else if(d[n+1])
			n=n+1;
		for(a=n;a>=1;a--)
		{
			if(d[a]>0 && d[a+1]>0 && d[a+2]==0)
			{
				d[a]--;
				d[a+1]--;
				d[a+2]++;
			}
			if(d[a]==2)
			{
				if(a!=1 && a!=2)
					d[a-2]++;
				else if(a==2)
					d[a-1]++;
				d[a]=0;
				d[a+1]++;
			}
		}
		if(d[n+2])
			n=n+2;
		else if(d[n+1])
			n=n+1;
		printf("%d",n);
		for(a=1;a<=n;a++)
			printf(" %d",d[a]);
		putchar('\n');
	}
	return 0;
}
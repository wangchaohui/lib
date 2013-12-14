#include <stdio.h>
#include <algorithm>
using namespace std;

typedef long long I;

struct ZP{
	int l[10],u[10];
};

ZP r[2][300000];
int pre,now;

int main()
{
	int N,M,i,j,tot,p,s,t;
	ZP a,c;
	while(scanf("%d %d",&M,&N)==2)
	{
		tot=0;pre=0;now=1;
		while(M--)
		{
			for(i=0;i<N;i++)scanf("%d",&a.l[i]);
			for(i=0;i<N;i++)scanf("%d",&a.u[i]);
			p=0;
			for(i=0;i<tot;i++)
			{
				for(j=0;j<N;j++)
					if(r[pre][i].l[j]>=a.u[j] || r[pre][i].u[j]<=a.l[j])break;
				if(j!=N)r[now][p++]=r[pre][i];
				else
				{
					c=r[pre][i];
					for(j=0;j<N;j++)
					{
						if(r[pre][i].l[j]<a.l[j])
						{
							r[now][p]=c;
							r[now][p].u[j]=a.l[j];
							p++;
						}
						if(r[pre][i].u[j]>a.u[j])
						{
							r[now][p]=c;
							r[now][p].l[j]=a.u[j];
							p++;
						}
						if(a.l[j]>c.l[j])c.l[j]=a.l[j];
						if(a.u[j]<c.u[j])c.u[j]=a.u[j];
					}
				}	
			}
			r[now][p++]=a;
			swap(pre,now);
			tot=p;
		}
		s=0;
		for(i=0;i<tot;i++)
		{
			//printf(":%d\n",i);
			//for(j=0;j<N;j++)printf(" %d",r[pre][i].l[j]);
			//putchar('\n');
			//for(j=0;j<N;j++)printf(" %d",r[pre][i].u[j]);
			//putchar('\n');
			t=1;
			for(j=0;j<N;j++)t=(I)t*(r[pre][i].u[j]-r[pre][i].l[j])%14121413;
			s=(s+t)%14121413;
		}
		printf("%d\n",s);
	}
	return 0;
}

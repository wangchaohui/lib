#include <stdio.h>
#include <string.h>

#define maxn 400
#define maxw 2000000000

int g[maxn+1][maxn+1];
int cost[maxn+1];
int n;

int Stoer_Wagner()
{
	int ans=maxw,i,j,m,mn,pre,t;
	while(n>1)
	{
		for(i=1;i<=n;i++)cost[i]=0;
		mn=-1;
		for(i=1;i<=n;i++)
		{
			pre=mn;m=-maxw;
			for(j=1;j<=n;j++)if(cost[j]>m){m=cost[j];mn=j;}
			cost[mn]=-maxw;
			for(j=1;j<=n;j++)if(cost[mn]!=-maxw)cost[j]+=g[mn][j];
		}
		t=0;
		for(i=1;i<=n;i++)t+=g[mn][i];
		if(ans>t)ans=t;
		for(i=1;i<=n;i++)
		{
			g[pre][i]+=g[mn][i];
			g[i][pre]+=g[mn][i];
		}
		g[pre][pre]=0;
		for(i=1;i<=n-1;i++)g[mn][i]=g[i][mn]=g[n][i];
		g[mn][mn]=0;
		n--;
	}
	return ans;
}

int main()
{
	int m,a,b,c,i,vs,ans;
	while(scanf("%d %d %d",&n,&m,&vs)==3 && n)
	{
		memset(g,0,sizeof(g));
		for(i=1;i<=m;i++)
		{
			scanf("%d %d %d",&a,&b,&c);
			if(a==b)continue;
			g[a][b]+=c;
			g[b][a]+=c;
		}
		ans=Stoer_Wagner();
		printf("%d\n",ans);
	}
	return 0;
}

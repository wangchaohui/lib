#include <stdio.h>
#include <memory.h>
#include <algorithm>
#include <queue>
using namespace std;

#define maxn 100
#define maxw 2000000000

typedef int W;
typedef pair<W,int> PWI;

int g[maxn+1][maxn+1];
int n,m,f[maxn+1],ans,visited[maxn+1];
W cost[maxn+1];

void init_graph()
{
	int i,j;
	for(i=1;i<=n;i++)
		for(j=1;j<=n;j++)
			g[i][j]=maxw;
}

void add_edge(int a,int b,W c)
{
	if(g[a][b]>c)g[a][b]=c;
	if(g[b][a]>c)g[b][a]=c;
}

void Prim()
{
	int i,j,mn;
	priority_queue< PWI,vector<PWI>,greater<PWI> > Q;
	PWI p;
	cost[1]=0;
	Q.push(make_pair(0,1));
	for(i=2;i<=n;i++)
	{
		cost[i]=maxw;
		Q.push(make_pair(maxw,i));
	}
	ans=0;
	memset(f,0,sizeof(f));
	memset(visited,0,sizeof(visited));
	while(!Q.empty())
	{
		p=Q.top();Q.pop();
		mn=p.second;
		if(visited[mn])continue;
		visited[mn]=1;
		if(cost[mn]==maxw){ans=-1;return;}
		ans+=cost[mn];
		for(j=1;j<=n;j++)
			if(!visited[j] && g[mn][j]<cost[j])
			{
				cost[j]=g[mn][j];f[j]=mn;
				Q.push(make_pair(cost[j],j));
			}
	}
}

int main()
{
	int i,j;
	while(scanf("%d",&n)==1)
	{
		for(i=1;i<=n;i++)
			for(j=1;j<=n;j++)
				scanf("%d",&g[i][j]);
		Prim();
		printf("%d\n",ans);
	}
	return 0;
}

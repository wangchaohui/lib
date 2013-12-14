#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <queue>
using namespace std;

#define maxn 500
#define maxm 70000
#define maxw 2000000000

typedef int W;
typedef pair<W,int> PWI;

struct edge{
	int y,next;
	W w;
};

struct edge g[maxm+1];
int n,m,first[maxn+1],f[maxn+1],tot,ans,visited[maxn+1];
W cost[maxn+1];

void init_graph()
{
	int i;
	for(i=1;i<=n;i++)
		first[i]=-1;
	tot=0;
}

void add_edge(int a,int b,W c)
{
	tot++;
	g[tot].y=b;g[tot].w=c;
	g[tot].next=first[a];first[a]=tot;
	tot++;
	g[tot].y=a;g[tot].w=c;
	g[tot].next=first[b];first[b]=tot;
}

void Prim()
{
	int i,id,j,mn;
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
		id=first[mn];
		while(id!=-1)
		{
			j=g[id].y;
			if(!visited[j] && g[id].w<cost[j])
			{
				cost[j]=g[id].w;f[j]=mn;
				Q.push(make_pair(cost[j],j));
			}
			id=g[id].next;
		}
	}
}

int main()
{
	int i,j,a,b,c,d;
	char ch;
	while(scanf("%d",&n)==1 && n)
	{
		init_graph();
		for(i=1;i<=n-1;i++)
		{
			scanf(" %c %d",&ch,&d);
			a=ch-'A'+1;
			for(j=1;j<=d;j++)
			{
				scanf(" %c %d",&ch,&c);
				b=ch-'A'+1;
				add_edge(a,b,c);
			}
		}
		Prim();
		printf("%d\n",ans);
	}
	return 0;
}

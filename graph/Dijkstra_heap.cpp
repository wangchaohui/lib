
#include <algorithm>
#include <queue>
using namespace std;
#define maxn 100000
#define maxm 2000000
#define maxw 2000000000

typedef int W;
typedef pair<W,int> PWI;

struct edge{
	int x,y,next;
	W w;
};

struct edge g[maxm+1];
int n,m,first[maxn+1],tot,vs,vt;
W dist[maxn+1];

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
	g[tot].x=a;g[tot].y=b;g[tot].w=c;
	g[tot].next=first[a];first[a]=tot;
	tot++;
	g[tot].x=b;g[tot].y=a;g[tot].w=c;
	g[tot].next=first[b];first[b]=tot;
}

void Dijkstra()
{
	int i,mn;
	priority_queue< PWI,vector<PWI>,greater<PWI> > Q;
	PWI p;
	for(i=1;i<=n;i++)
		dist[i]=maxw;
	dist[vs]=0;
	Q.push(make_pair(dist[vs],vs));
	while(!Q.empty())
	{
		p=Q.top();Q.pop();
		mn=p.second;
		if(p.first!=dist[mn])continue;
		i=first[mn];
		while(i!=-1)
		{
			if(dist[g[i].y]>dist[mn]+g[i].w)
			{
				dist[g[i].y]=dist[mn]+g[i].w;
				Q.push(make_pair(dist[g[i].y],g[i].y));
			}
			i=g[i].next;
		}
	}
}
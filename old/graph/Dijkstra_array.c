#include <stdio.h>

#define maxn 600
#define maxm 180000
#define maxw 2000000000

typedef int W;

struct edge{
	int x,y,next;
	W w;
};

struct edge g[maxm+1];
int n,m,first[maxn+1],p[maxn+1],tot,vs;
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
}

void Dijkstra()
{
	int i,l,mn,id,t;
	for(i=1;i<=n;i++)
	{
		dist[i]=maxw;
		p[i]=i;
	}
	dist[vs]=0;l=n;
	while(l>0)
	{
		mn=1;
		for(i=2;i<=l;i++)
			if(dist[p[i]]<dist[p[mn]])mn=i;
		t=p[mn];p[mn]=p[l--];mn=t;
		id=first[mn];
		while(id!=-1)
		{
			if(dist[g[id].y]>dist[mn]+g[id].w)
				dist[g[id].y]=dist[mn]+g[id].w;
			id=g[id].next;
		}
	}
}

int main()
{
	int i,a,b,c;
	scanf("%d %d",&n,&m);
	init_graph();
	for(i=1;i<=m;i++)
	{
		scanf("%d %d %d",&a,&b,&c);
		add_edge(a,b,c);
	}
	vs=1;
	Dijkstra();
	if(dist[n]==maxw)
		printf("-1");
	else
		printf("%d",dist[n]);
	return 0;
}
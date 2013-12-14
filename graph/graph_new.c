
#define maxn 600
#define maxm 180000
#define maxw 2000000000

typedef int W;

struct edge{
	int x,y,next;
	W w;
};

struct btree{
	int ln,rn;
};

struct edge g[maxm+1];
struct btree bt[maxn+1];
int n,m,first[maxn+1],p[maxn+1],tot,vs;
W dist[maxn+1];

void init_graph()
{
	int i;
	for(i=1;i<=n;i++)
		first[i]=-1;
	tot=0;
}

void add_edge(int a,int b)
{
	tot++;
	g[tot].x=a;g[tot].y=b;
	g[tot].next=first[a];first[a]=tot;
	tot++;
	g[tot].x=b;g[tot].y=a;
	g[tot].next=first[b];first[b]=tot;
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

void gtobtd(struct edge *g,struct btree *bt,int top,int p)
{
	int id=first[top],ni;
	while(id!=-1 && g[id].y==p)id=g[id].next;
	if(id!=-1)
	{
		bt[top].ln=g[id].y;
		gtobtd(g,bt,g[id].y,top);
		while((ni=g[id].next)!=-1)
		{
			while(ni!=-1 && g[ni].y==p)ni=g[ni].next;
			if(ni==-1)break;
			bt[g[id].y].rn=g[ni].y;
			gtobtd(g,bt,g[ni].y,top);
			id=ni;
		}
		bt[g[id].y].rn=-1;
	}
}

void gtobt(struct edge *g,struct btree *bt,int top)
{
	int i;
	for(i=1;i<=n;i++)
		bt[i].ln=bt[i].rn=-1;
	gtobtd(g,bt,top,-1);
}

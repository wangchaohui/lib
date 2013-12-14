
#define eps 1e-8

#define maxn 30
#define maxm 900
#define maxw 2000000000

struct edge{
	int x,y,next;
	double w;
};

struct edge g[maxm+1];
int n,m,first[maxn+1],tot,vs,e;
double dist[maxn+1];

/******* ******* *******

don't forget to init_graph()
e=1 means exits negative cycle

******* ******* *******/

void init_graph()
{
	int i;
	for(i=1;i<=n;i++)
		first[i]=-1;
	tot=0;
}

void add_edge(int a,int b,double c)
{
	tot++;
	g[tot].x=a;g[tot].y=b;g[tot].w=c;
	g[tot].next=first[a];first[a]=tot;
}

void Bellman_Ford()
{
	int i,a,flag;
	for(i=1;i<=n;i++)
		dist[i]=maxw;
	dist[vs]=0;
	for(i=1;i<=n;i++)
	{
		flag=0;
		for(a=1;a<=tot;a++)
			if(dist[g[a].y]>dist[g[a].x]+g[a].w+eps)
			{
				dist[g[a].y]=dist[g[a].x]+g[a].w;
				flag=1;
			}
		if(flag==0)break;
	}
	if(i==n+1)e=1;
}


#include <memory.h>
#define maxn 200
#define maxm 20000
#define maxw 2000000000

struct edge{
	int x,y,next;
	int w;
};

struct edge g[maxm+1],gr[maxm+1];
int n,m,first[maxn+1],first1[maxn+1],tot,vs,vt,e,p[maxn+1],p1[maxn+1],ttop,firstr[maxn+1];
char visited[maxn+1];
int dist[maxn+1];

/******* ******* *******

don't forget to init_graph()

******* ******* *******/

void init_graph()
{
	int i;
	for(i=1;i<=n;i++)
		first[i]=-1;
	tot=0;
}

void add_edge(int a,int b,int c)
{
	tot++;
	g[tot].x=a;g[tot].y=b;g[tot].w=c;
	g[tot].next=first[a];first[a]=tot;
}

void tpsort()
{
	int top,i,id,tp;
	memcpy(first1,first,sizeof(first));
	memset(visited,0,sizeof(visited));
	top=1;p[1]=vs;ttop=0;
	while(top>0)
	{
		tp=p[top];id=first1[tp];
		while(id!=-1)
		{
			if(!visited[g[id].y])
			{
				first1[tp]=g[id].next;
				p[++top]=g[id].y;
				break;
			}
			id=g[id].next;
		}
		if(id==-1){visited[tp]=1;top--;p1[++ttop]=tp;}
	}
}

void reverse_graph()
{
	int i,id,t=0;
	for(i=1;i<=n;i++)
		firstr[i]=-1;
	for(i=1;i<=n;i++)
	{
		id=first[i];
		while(id!=-1)
		{
			t++;
			gr[t].x=g[id].y;gr[t].y=g[id].x;gr[t].w=g[id].w;
			gr[t].next=firstr[gr[t].x];firstr[gr[t].x]=t;
			id=g[id].next;
		}
	}
}

int longest_path()
{
	int i,id,tp;
	tpsort();
	if(!visited[vt])return -1;
	for(i=1;i<=n;i++)
		dist[i]=-maxw;
	dist[vs]=0;
	for(i=ttop;i>=1;i--)
	{
		tp=p1[i];id=firstr[tp];
		while(id!=-1)
		{
			if(visited[tp] && dist[tp]<dist[gr[id].y]+gr[id].w)
				dist[tp]=dist[gr[id].y]+gr[id].w;
			id=gr[id].next;
		}
	}
	return dist[vt];
}

int main()
{
	int a,b,c,i,q;
	while(scanf("%d %d",&n,&m)!=EOF)
	{
		init_graph();
		for(i=1;i<=m;i++)
		{
			scanf("%d %d %d",&a,&b,&c);
			add_edge(a,b,c);
		}
		reverse_graph();
		scanf("%d",&q);
		while(q--)
		{
			scanf("%d %d",&vs,&vt);
			printf("%d\n",longest_path());
		}
	}
	return 0;
}
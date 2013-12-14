#include <stdio.h>
#include <memory.h>
#include <queue>
using namespace std;

#define eps 1e-8

#define maxn 50001
#define maxm 150003
#define maxw 2000000000

struct edge{
	int x,y,next;
	int w;
};

struct edge g[maxm+1];
int n,m,first[maxn+1],tot,vs,e,visited[maxn+1];
int dist[maxn+1];
int m1,m2;

/******* ******* *******

don't forget to init_graph()

******* ******* *******/

void init_graph()
{
	int i;
	for(i=0;i<=n;i++)
		first[i]=-1;
	tot=0;
}

void add_edge(int a,int b,int c)
{
	tot++;
	g[tot].x=a;g[tot].y=b;g[tot].w=c;
	g[tot].next=first[a];first[a]=tot;
}

void spfa()
{
	int i,mn,id,y;
	queue<int> Q;
	memset(visited,0,sizeof(visited));
	for(i=m1;i<=m2;i++)
		dist[i]=-maxw;
	dist[vs]=0;
	Q.push(vs);visited[vs]=1;
	while(!Q.empty())
	{
		mn=Q.front();Q.pop();visited[mn]=0;
		id=first[mn];
		while(id!=-1)
		{
			y=g[id].y;
			if(dist[y]<dist[mn]+g[id].w)
			{
				dist[y]=dist[mn]+g[id].w;
				if(!visited[y])
				{
					Q.push(y);
					visited[y]=1;
				}
			}
			id=g[id].next;
		}
	}
}


int main()
{
	int N,i,a,b,c;
	m1=100000;m2=-1;
	n=50001;
	scanf("%d",&N);
	init_graph();
	for(i=1;i<=N;i++)
	{
		scanf("%d %d %d",&a,&b,&c);
		add_edge(a,b+1,c);
		if(a<m1)m1=a;
		if(b+1>m2)m2=b+1;
	}
	for(i=m1;i<m2;i++)
	{
		add_edge(i,i+1,0);
		add_edge(i+1,i,-1);
	}
	vs=m1;
	spfa();
	printf("%d\n",dist[m2]);
	return 0;
}

#include <memory.h>
#define maxn 500
#define maxm 125000

struct edge{
	int x,y,next;
};

struct edge g[maxm+1];
int n,m,first[maxn+1],tot,maxmatch;
int link[maxn+1];
char used[maxn+1];

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
}

int find(int s)
{
	int id,t;
	id=first[s];
	while(id!=-1)
	{
		t=g[id].y;
		if(!used[t])
		{
			used[t]=1;
			if(link[t]==0 || find(link[t]))
			{
				link[t]=s;
				return 1;
			}
		}
		id=g[id].next;
	}
	return 0;
}

void Hungary()
{
	int i;
	maxmatch=0;
	memset(link,0,sizeof(link));
	for(i=1;i<=n;i++)
	{
		memset(used,0,sizeof(used));
		if(find(i))
			maxmatch++;
	}
}

#include <stdio.h>
#include <memory.h>

#define maxn 1000000
#define maxm 1000000
#define maxw 2000000000

struct edge{
	int x,y,next;
};

struct edge g[2*maxm+1];
int n,m,first[maxn+1],first1[maxn+1],p[maxn+1],tot,vs,vt,c;
char visited[maxn+1];

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

void dfs()
{
	int top,i,id,tp,t;
	memcpy(first1,first,sizeof(first));
	memset(visited,0,sizeof(visited));
	top=1;p[1]=vs;
	while(top>0)
	{
		tp=p[top];visited[tp]=1;
		while((id=first1[tp])!=-1)
		{
			first1[tp]=g[id].next;
			if(!visited[g[id].y])
			{
				p[++top]=g[id].y;
				break;
			}
		}
		if(id==-1){top--;c++;}
	}
}

int main()
{
	int a,b,i;
	vs=1;
	scanf("%d %d",&n,&m);
	init_graph();
	for(i=1;i<=m;i++)
	{
		scanf("%d %d",&a,&b);
		add_edge(a,b);
	}
	c=0;
	dfs();
	if(c==n)
		printf("yes");
	else
		printf("no");
	return 0;
}
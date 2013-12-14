

#include <memory.h>

#define maxn 26
#define maxm 100000
#define maxw 2000000000

struct edge{
	int x,y,next;
};

struct edge g[maxm+1];
int n,m,first[maxn+1],first1[maxn+1],p[maxn+1],degree[maxn+1],tot;
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
}

int hasEuler_path()
{
	int top,i,id,tp,vs=-1,vt=-1;
	memcpy(first1,first,sizeof(first));
	memset(visited,-1,sizeof(visited));
	memset(degree,0,sizeof(degree));
	for(i=1;i<=n;i++)
		if(first[i]!=-1)
		{
			visited[i]=0;vs=i;
			id=first[i];
			while(id!=-1)
			{
				degree[i]++;
				degree[g[id].y]--;
				visited[g[id].y]=0;
				id=g[id].next;
			}
		}
	for(i=1;i<=n;i++)
		if(!visited[i] && degree[i]!=0)
			if(degree[i]==1 && (vs==i || vs==-1 || degree[vs]!=1))
				vs=i;
			else if(degree[i]==-1 && vt==-1)
				vt=i;
			else
				return 0;
	if(degree[vs]==0 && vt!=-1 || degree[vs]==1 && vt==-1)
		return 0;
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
		if(id==-1)top--;
	}
	for(i=1;i<=n;i++)
		if(!visited[i])
			return 0;
	return 1;
}

int hasEuler_circle()
{
	int top,i,id,tp,vs=-1,vt=-1;
	memcpy(first1,first,sizeof(first));
	memset(visited,-1,sizeof(visited));
	memset(degree,0,sizeof(degree));
	for(i=1;i<=n;i++)
		if(first[i]!=-1)
		{
			visited[i]=0;vs=i;
			id=first[i];
			while(id!=-1)
			{
				degree[i]++;
				degree[g[id].y]--;
				visited[g[id].y]=0;
				id=g[id].next;
			}
		}
	for(i=1;i<=n;i++)
		if(!visited[i] && degree[i]!=0)
			if(degree[i]==1 && (vs==i || vs==-1 || degree[vs]!=1))
				vs=i;
			else if(degree[i]==-1 && vt==-1)
				vt=i;
			else
				return 0;
	if(degree[vs]==0 && vt!=-1 || degree[vs]==1 && vt==-1)
		return 0;
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
		if(id==-1)top--;
	}
	for(i=1;i<=n;i++)
		if(!visited[i])
			return 0;
	return 1;
}
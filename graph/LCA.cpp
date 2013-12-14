#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

#define N 40005
#define LN 17 // 2^LN>=N
int M[N][LN+1],A[N],L[N+1];

void initlg(int n)
{
	int i;
	L[0]=-1;
	for(i=1;i<=n;i++)
		if((i&(i-1))==0)
			L[i]=L[i-1]+1;
		else
			L[i]=L[i-1];
}

void RMQinit(int *a,int n)
{
	int i,j,t1,t2,j1=2,j2=1;
	for(i=0;i<n;i++)M[i][0]=i;
	for(j=1;j1<=n;j++)
	{
		for(i=0;i+j1-1<n;i++)
		{
			t1=M[i][j-1];t2=M[i+j2][j-1];
			M[i][j]=a[t1]<a[t2]?t1:t2;
		}
		j1*=2;j2*=2;
	}
}

int RMQ(int *a,int i,int j)
{  
	int k=L[j-i+1],t1=M[i][k],t2=M[j-(1<<k)+1][k];
	return a[t1]<=a[t2]?t1:t2;
}

#define maxn 10000
#define maxm 10000
#define maxw 2000000000

struct edge{
	int x,y,next;
};

struct edge g[maxm+1];
int n,m,first[maxn+1],p[maxn+1],tot,vs;
int P[2*maxn-1],D[2*maxn-1],X[maxn+1];
int visited[maxn+1];
int l,root;

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

void dfs(int x,int depth)
{
	int y,id;
	visited[x]=1;
	X[x]=l;P[l]=x;D[l]=depth;l++;
	for(id=first[x];id!=-1;id=g[id].next)
	{
		y=g[id].y;
		if(visited[y])continue;
		dfs(y,depth+1);
		P[l]=x;D[l]=depth;l++;
	}
}

void LCAinit()
{
	memset(visited,0,sizeof(visited));
	l=0;
	dfs(root,0);
	//printf("l: %d\n",l);
	RMQinit(D,l);
}

int LCA(int x,int y)
{
	return P[RMQ(D,min(X[x],X[y]),max(X[x],X[y]))];
}

int main()
{
	int T,i;
	int a,b;
	initlg(40000);
	scanf("%d",&T);
	while(T--)
	{
		scanf("%d",&n);
		init_graph();
		memset(visited,0,sizeof(visited));
		for(i=0;i<n-1;i++)
		{
			scanf("%d %d",&a,&b);
			visited[b]=1;
			add_edge(a,b);
		}
		for(i=1;i<=n;i++)
			if(visited[i]==0)root=i;
		scanf("%d %d",&a,&b);
		LCAinit();
		printf("%d\n",LCA(a,b));
	}
	return 0;
}


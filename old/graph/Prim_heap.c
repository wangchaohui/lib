#include <stdio.h>
#include <memory.h>
#define swap(a,b){t=a;a=b;b=t;}

#define maxn 500
#define maxm 70000
#define maxw 2000000000

typedef int W;

struct edge{
	int y,next;
	W w;
};

struct edge g[maxm+1];
int n,m,first[maxn+1],p[maxn+1],f[maxn+1],tot,ans;
W cost[maxn+1];
int lc[maxn+1];

void heapify(int *heap,int i,int (*CFun)(int,int))
{
	int min=2*i,n=heap[0],t;
	while(min<=n)
	{
		if(min+1<=n && CFun(heap[min],heap[min+1])<0)min++;
		if(CFun(heap[i],heap[min])>=0)return;
		swap(heap[i],heap[min]);
		lc[heap[i]]=i;
		lc[heap[min]]=min;
		i=min;min*=2;
	}
}

int extract(int *heap,int (*CFun)(int,int))
{
	int m=heap[1];
	lc[heap[1]]=-1;
	heap[1]=heap[heap[0]--];
	if(heap[0]>0)lc[heap[1]]=1;
	heapify(heap,1,CFun);
	return m;
}

int CFun(int x,int y)
{
	if(cost[x]<cost[y])return 1;/*left is better*/
	return -1;/*right is better*/
}

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
	g[tot].y=b;g[tot].w=c;
	g[tot].next=first[a];first[a]=tot;
	tot++;
	g[tot].y=a;g[tot].w=c;
	g[tot].next=first[b];first[b]=tot;
}

void shiftup(int *heap,int i,int (*CFun)(int,int))
{
	int t,ii;
	while(i>1)
	{
		ii=i/2;
		if(CFun(heap[i],heap[ii])<=0)return;
		swap(heap[i],heap[ii]);
		lc[heap[i]]=i;
		lc[heap[ii]]=ii;
		i=ii;
	}
}

void Prim()
{
	int i,id,j,k,mn;
	for(i=1;i<=n;i++)
	{
		cost[i]=maxw;
		p[i]=lc[i]=i;
	}
	cost[1]=0;
	p[0]=n;ans=0;
	memset(f,0,sizeof(f));
	for(k=1;k<=n;k++)
	{
		mn=extract(p,CFun);
		if(cost[mn]==maxw){ans=-1;return;}
		ans+=cost[mn];
		id=first[mn];
		while(id!=-1)
		{
			j=g[id].y;
			if(lc[j]!=-1 && g[id].w<cost[j])
			{
				cost[j]=g[id].w;f[j]=mn;
				shiftup(p,lc[j],CFun);
			}
			id=g[id].next;
		}
	}
}

int main()
{
	int pp,i,j,k,a,b,c,d[101];
	scanf("%d",&pp);
	while(pp--)
	{
		scanf("%d %d %d",&n,&m,&k);
		init_graph();
		for(i=1;i<=m;i++)
		{
			scanf("%d %d %d",&a,&b,&c);
			add_edge(a,b,c);
		}
		for(i=1;i<=k;i++)
		{
			scanf("%d",&c);
			for(j=1;j<=c;j++)
			{
				scanf("%d",&d[j]);
				for(a=1;a<j;a++)
					add_edge(d[a],d[j],0);
			}
		}
		Prim();
		printf("%d\n",ans);
	}
	return 0;
}



#define swap(a,b){t=a;a=b;b=t;}

#define maxn 100
#define maxm 2000
#define maxw 2000000000

typedef int W;

struct edge{
	int x,y,next;
	W w;
};

struct edge g[maxm+1];
int n,m,first[maxn+1],p[maxn+1],tot,vs,vt;
int dist[maxn+1];
int lc[maxn+1];

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
	tot++;
	g[tot].x=b;g[tot].y=a;g[tot].w=c;
	g[tot].next=first[b];first[b]=tot;
}

int CFun(int x,int y)
{
	if(dist[x]<dist[y])return 1;/*left is better*/
	return -1;/*right is better*/
}

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

void buildheap(int *heap,int (*CFun)(int,int))
{
	int i;
	for(i=heap[0]/2;i>=1;i--)
		heapify(heap,i,CFun);
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

void heapdel(int *heap,int i,int (*CFun)(int,int))
{
	lc[heap[i]]=-1;
	if(--heap[0]>=i)
	{
		heap[i]=heap[heap[0]+1];
		lc[heap[i]]=i;
		heapify(i);
		shiftup(i);
	}
}

int extract(int *heap,int (*CFun)(int,int))
{
	int m=heap[1];
	heapdel(heap,1,CFun);
	return m;
}

void Dijkstra()
{
	int i,mn,t;
	for(i=1;i<=n;i++)
	{
		dist[i]=maxw;
		p[i]=lc[i]=i;
	}
	dist[vs]=0;
	p[0]=n;
	buildheap(p,CFun);
	while(p[0]>0)
	{
		mn=extract(p,CFun);
		i=first[mn];
		while(i!=-1)
		{
			if(dist[g[i].y]>dist[mn]+g[i].w)
			{
				dist[g[i].y]=dist[mn]+g[i].w;
				shiftup(p,lc[g[i].y],CFun);
			}
			i=g[i].next;
		}
	}
}

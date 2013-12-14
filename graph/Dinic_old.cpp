#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
using namespace std;

namespace Dinic{

	typedef int W;
	typedef long long ll;

	struct edge{
		int y;
		W c;
		edge *next;
		edge *op;
	};

	class Graph{
	public:
		static const int maxn=5000;
		static const int maxm=60000;
		static const int maxw=2000000000;

	private:
		edge g[maxm];
		int tot;

	public:
		edge *first[maxn];
		int n;

		void init(int n)
		{
			this->n=n;
			tot=0;
			for(int i=0;i<n;i++)first[i]=NULL;
		}

		void add_edge(int a,int b,W c)
		{
			if(a==b)return;
			edge *d=&g[tot++];
			edge *e=&g[tot++];
			d->y=b;d->c=c;d->next=first[a];first[a]=d;d->op=e;
			e->y=a;e->c=c;e->next=first[b];first[b]=e;e->op=d;
		}
	};

	int p[Graph::maxn];
	int level[Graph::maxn];
	int vs,vt;

	void make_level(const Graph &g)
	{
		int op=1,cl=0;
		for(int i=0;i<g.n;i++)level[i]=-1;
		p[0]=vs;level[vs]=1;
		while(op>cl)
		{
			int tp=p[cl++];
			if(tp==vt)return;
			for(edge *id=g.first[tp];id;id=id->next)
			{
				if(level[id->y]==-1 && id->c>0)
				{
					p[op++]=id->y;level[id->y]=level[tp]+1;
				}
			}
		}
	}

	ll maxflow;

	void dfs_maxflow(const Graph &g)
	{
		edge *first[Graph::maxn],*prt[Graph::maxn];
		memcpy(first,g.first,sizeof(g.first));
		int top=1;p[0]=vs;
		while(top>0)
		{
			int tp=p[top-1];
			if(tp==vt)
			{
				W mint=Graph::maxw;
				for(int i=top-1;i>0;i--)
				{
					mint=min(mint,prt[i]->c);
				}
				maxflow+=mint;
				int t=0;
				for(int i=top-1;i>0;i--)
				{
					edge *pre=prt[i];
					pre->c-=mint;
					pre->op->c+=mint;
					if(pre->c==0)t=i;
				}
				top=t;
			}
			else
			{
				edge *id;
				while(id=first[tp])
				{
					first[tp]=id->next;
					if(level[id->y]==level[tp]+1 && id->c>0)
					{
						p[top]=id->y;
						prt[top++]=id;
						break;
					}
				}
				if(!id)top--;
			}
		}
	}

	void Dinic(const Graph &g)
	{
		maxflow=0;
		while(1)
		{
			make_level(g);
			if(level[vt]==-1)break;
			dfs_maxflow(g);
		}
	}

}

int main()
{
	//freopen("in.txt","r",stdin);
	static Dinic::Graph g;
	int n,m,a,b,c;
	while(scanf("%d %d",&n,&m)!=EOF)
	{
		g.init(n);
		while(m--)
		{
			scanf("%d %d %d",&a,&b,&c);
			a--;b--;
			g.add_edge(a,b,c);
		}
		Dinic::vs=0;Dinic::vt=n-1;
		Dinic::Dinic(g);
		printf("%lld\n",Dinic::maxflow);
	}
	return 0;
}

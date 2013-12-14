#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
using namespace std;

namespace Dinic{

	static const int maxn=5000;
	static const int maxw=2000000000;
	typedef long long ll;

	struct Graph{
		typedef int W;
		struct edge{
			int y;
			W c;
			int op;

			edge(int y,W c,int op):y(y),c(c),op(op){}
		};

		vector<edge> *e;

		const int n;

		Graph(int n):n(n)
		{
			e = new vector<edge> [n];
		}

		~Graph() {delete[] e;}

		void add_edge(int a,int b,W c)
		{
			if(a==b)return;
			edge ea(b,c,e[b].size());
			edge eb(a,c,e[a].size());
			e[a].push_back(ea);
			e[b].push_back(eb);
		}
	};

	int level[maxn];
	int vs,vt;

	void make_level(const Graph &g)
	{
		queue<int> p;
		p.push(vs);
		for(int i=0;i<g.n;i++)level[i]=-1;
		level[vs]=1;
		while(!p.empty())
		{
			int tp=p.front();p.pop();
			if(tp==vt)return;
			for(size_t i=0;i<g.e[tp].size();i++)
			{
				const Graph::edge &d=g.e[tp][i];
				if(level[d.y]==-1 && d.c>0)
				{
					p.push(d.y);
					level[d.y]=level[tp]+1;
				}
			}
		}
	}

	ll maxflow;

	void dfs_maxflow(const Graph &g)
	{
		static size_t first[maxn];
		static Graph::edge *prt[maxn];
		vector<int> p;
		p.push_back(vs);
		memset(first,-1,sizeof(first));

		while(!p.empty())
		{
			int tp=p.back();
			if(tp==vt)
			{
				Graph::W mint=maxw;
				for(int i=(int)p.size()-1;i>0;i--)
				{
					mint=min(mint,prt[i]->c);
				}
				maxflow+=mint;
				int t=0;
				for(int i=(int)p.size()-1;i>0;i--)
				{
					Graph::edge *pre=prt[i];
					pre->c-=mint;
					g.e[pre->y][pre->op].c+=mint;
					if(pre->c==0)t=i;
				}
				p.resize(t);
			}
			else
			{
				size_t &i=first[tp];
				for(i++;i<g.e[tp].size();i++)
				{
					Graph::edge &d=g.e[tp][i];
					if(level[d.y]==level[tp]+1 && d.c>0)
					{
						prt[p.size()]=&d;
						p.push_back(d.y);
						break;
					}
				}
				if(i==g.e[tp].size())p.pop_back();
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
	int n,m;
	while(scanf("%d %d",&n,&m)==2)
	{
		int a,b,c;
		Dinic::Graph g(n);
		Dinic::vs=0;Dinic::vt=n-1;
		for(int i=1;i<=m;i++)
		{
			scanf("%d %d %d",&a,&b,&c);
			a--;b--;
			if(a==b)continue;
			g.add_edge(a,b,c);
		}
		Dinic::Dinic(g);
		printf("%lld\n",Dinic::maxflow);
	}
	return 0;
}

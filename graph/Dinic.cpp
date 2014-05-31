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
    Graph::edge *prt[maxn];
    int vs, vt;
    ll maxflow;

    bool make_level(const Graph &g)
    {
        queue<int> p;
        p.push(vs);
        fill(level, level + g.n, -1);
        level[vs] = 0;
        while (!p.empty())
        {
            int tp = p.front(); p.pop();
            if (tp == vt) return true;
            for (const Graph::edge &d : g.e[tp])
            {
                if (level[d.y] == -1 && d.c > 0)
                {
                    p.push(d.y);
                    level[d.y] = level[tp] + 1;
                }
            }
        }
        return false;
    }

    void process(vector<int> &p)
    {
        Graph::W mint = maxw;
        for (int i = (int)p.size() - 1; i > 0; i--)
        {
            mint = min(mint, prt[i]->c);
        }
        maxflow += mint;
        int t = 0;
        for (int i = (int)p.size() - 1; i > 0; i--)
        {
            Graph::edge *pre = prt[i];
            pre->c -= mint;
            g.e[pre->y][pre->op].c += mint;
            if (!pre->c)t = i;
        }
        p.resize(t);
    }

    void dfs_maxflow(const Graph &g)
    {
        static int first[maxn];
        vector<int> p;
        p.push_back(vs);
        memset(first, -1, sizeof(first));

        while (!p.empty())
        {
            int tp = p.back();
            if (tp == vt)process(p);
            else
            {
                int &i = first[tp];
                while (++i < g.e[tp].size())
                {
                    Graph::edge &d = g.e[tp][i];
                    if (level[d.y] == level[tp] + 1 && d.c > 0)
                    {
                        prt[p.size()] = &d;
                        p.push_back(d.y);
                        break;
                    }
                }
                if (i >= g.e[tp].size())p.pop_back();
            }
        }
    }

    void Dinic(const Graph &g)
    {
        maxflow=0;
        while (make_level(g))
            dfs_maxflow(g);
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

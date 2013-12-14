

struct edge{
	int y;
	edge *next;
};

class Graph{
public:
	static const int maxn=600;
	static const int maxm=180000;
	static const int maxw=2000000000;

private:
	edge g[maxm];
	int tot;

public:
	edge *first[maxn];
	int n;

	Graph(int n):n(n),tot(0)
	{
		for(int i=0;i<n;i++)first[i]=NULL;
	}

	void add_edge(int a,int b)
	{
		g[tot].y=b;
		g[tot].next=first[a];first[a]=&g[tot];
		tot++;
	}
};
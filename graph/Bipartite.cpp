
class PBipartite:private Graph{
public:
	int n,m;
	PBipartite(int n,m):n(n),m(m)
	{
		Graph(n+m);
	}
};

//最小点权覆盖集=最小割=最大流
//最大点权独立集=总权-最小点权覆盖集
<template class TGraph>
TGraph PBipartiteBuildMaxFlow(const Graph &p)
{
	TGraph g=new TGraph();
}

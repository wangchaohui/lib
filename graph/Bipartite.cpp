
class PBipartite:private Graph{
public:
	int n,m;
	PBipartite(int n,m):n(n),m(m)
	{
		Graph(n+m);
	}
};

//��С��Ȩ���Ǽ�=��С��=�����
//����Ȩ������=��Ȩ-��С��Ȩ���Ǽ�
<template class TGraph>
TGraph PBipartiteBuildMaxFlow(const Graph &p)
{
	TGraph g=new TGraph();
}

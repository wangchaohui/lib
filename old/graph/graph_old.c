#include <stdio.h>
#include <stdlib.h>


struct graph{
	int v;
	struct list	**n;
	struct list	**w;
	int m;
	struct list	*l;
	int p;
};

struct list{
	int node;
	struct list *next;
};


struct graph gcreat(int v);
/******* ******* *******
	stdlib.h
******* ******* *******/

struct list* callocg(struct graph g);
/******* ******* *******
	stdlib.h
******* ******* *******/

int gaddedge(struct graph g,int u,int v);
/******* ******* *******
	stdlib.h
	-callocg
******* ******* *******/

void dfs(struct graph g,int v);
/******* ******* *******
	stdlib.h
	-explore
******* ******* *******/

void explore(struct graph g,int v,int *visited);

void freeg(struct graph g);
/******* ******* *******
	stdlib.h
******* ******* *******/


/******* ******* *******

(freeg is very slow)

******* ******* *******/

struct graph gcreat(int v,int m)
{
	struct graph g;
	g.v=v;g.m=m;g.p=0;
	g.n=(struct list**)calloc(v+1,sizeof(struct list*));
	g.w=(struct list**)calloc(v+1,sizeof(struct list*));
	g.l=(struct list*)calloc(m+1,sizeof(struct list));
	return g;
}

struct list* callocg(struct graph g)
{
	if(g.p<g.m)
		return g.l+(g.p++);
	else
		return NULL;
}

int gaddedge(struct graph g,int u,int v)
{
	struct list *p;
	p=(struct list*)callocg(g);
	p->node=v;
	if(g.n[u]==NULL)
		g.n[u]=p;
	else
		g.w[u]->next=p;
	g.w[u]=p;
	return 0;
}

void dfs(struct graph g,int v)
{
	int *visited;
	visited=(int*)calloc(g.v+1,sizeof(int));
	explore(g,v,visited);
	free(visited);
}

void explore(struct graph g,int v,int *visited)
{
	struct list	*p;
	visited[v]=1;
	/******* ******* *******
		pre
	******* ******* *******/
	for(p=g.n[v];p;p=p->next)
		if(visited[p->node]==0)
			explore(g,p->node,visited);
	/******* ******* *******
		post
	******* ******* *******/
}

void freeg(struct graph g)
{
	g.v=0;g.m=0;g.p=0;
	free(g.n);g.n=NULL;
	free(g.w);g.w=NULL;
	free(g.l);g.l=NULL;
}


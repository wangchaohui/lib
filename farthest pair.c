#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define DIMENSION 2
#define eps 1e-8

typedef double point[DIMENSION];

point d[50001];
int p[50001],s[50001],l,n;/*GRAHAM_SCAN*/
double p0x,p0y;/*GRAHAM_SCAN*/
double maxdist;/*rotating_calipers*/
int* Mergesort(int n,int (*CFun)(int,int));
void Mergesortd(int p,int q,int (*CFun)(int,int),int *order,int *t);
void Merge(int p,int m,int q,int (*CFun)(int,int),int *order,int *t);
int CFun(int x,int y);

int* Mergesort(int n,int (*CFun)(int,int))
{
	int a;
	int *order=(int*)malloc((n+1)*sizeof(int));
	int *t=(int*)malloc(n*sizeof(int));
	for(a=1;a<=n;a++)
		order[a]=a;
	Mergesortd(1,n,CFun,order,t);
	free(t);
	return order;
}

void Mergesortd(int p,int q,int (*CFun)(int,int),int *order,int *t)
{
	int m;
	m=(p+q)/2;
	if(p<m)Mergesortd(p,m,CFun,order,t);
	if(m+1<q)Mergesortd(m+1,q,CFun,order,t);
	Merge(p,m,q,CFun,order,t);
}

void Merge(int p,int m,int q,int (*CFun)(int,int),int *order,int *t)
{
	int k,i=p,j=m+1;
	for(k=0;k<q-p+1;k++)
		if(j>q || i<=m && CFun(order[i],order[j])<=0)
			t[k]=order[i++];
		else
			t[k]=order[j++];
	for(k=0;k<q-p+1;k++)
		order[p+k]=t[k];
}

double crossproduct(point p1,point p2)
{
	return (p1[0]-p0x)*(p2[1]-p0y)-(p2[0]-p0x)*(p1[1]-p0y);
}

int CFun(int x,int y)
{
	if(crossproduct(d[x],d[y])<0)
		return 1;/*right is front*/
	else
		return -1;/*left is front*/
}

double distance2(point p1,point p2)
{
	return (p1[0]-p2[0])*(p1[0]-p2[0])+(p1[1]-p2[1])*(p1[1]-p2[1]);
}

double distance(point p1,point p2)
{
	return sqrt(distance2(p1,p2));
}

double distptol(point p1,point p2,point p3)
{
	return (p1[1]-p2[1])*p3[0]-(p1[0]-p2[0])*p3[1]+(p1[0]*p2[1]-p2[0]*p1[1]);
}

void rotating_calipers()
{
	int i,p=1;
	double t,t1;
	maxdist=0;
	for(i=0;i<l;i++)
	{
		while(distptol(d[s[i]],d[s[(i+1)%l]],d[s[p]])+eps<distptol(d[s[i]],d[s[(i+1)%l]],d[s[(p+1)%l]]))
			p=(p+1)%l;
		t=distance(d[s[i]],d[s[p]]);
		t1=distance(d[s[(i+1)%l]],d[s[p]]);
		if(t1>t)t=t1;
		if(t>maxdist)
			maxdist=t;
	}
}

void GRAHAM_SCAN()
{
	int i,mn,m,*tt;
	double t;
	mn=1;
	for(i=1;i<=n;i++)
	{
		p[i]=i;
		if(d[i][1]+eps<d[mn][1] || d[i][1]+eps>d[mn][1] && d[i][1]<eps+d[mn][1] && d[i][0]+eps<d[mn][0])
			mn=i;
	}
	p[0]=mn;p[mn]=n;
	p0x=d[mn][0];p0y=d[mn][1];
	tt=(int*)malloc(n*sizeof(int));
	Mergesortd(1,n-1,CFun,p,tt);
	free(tt);
	mn=p[1];m=1;
	for(i=2;i<=n-1;i++)
	{
		t=crossproduct(d[mn],d[p[i]]);
		if(t>eps)
		{
			p[++m]=p[i];
			mn=p[i];
		}
		else if(t<eps && t+eps>0 && distance2(d[mn],d[p[0]])<distance2(d[p[i]],d[p[0]]))
		{
			p[m]=p[i];
			mn=p[i];
		}
	}
	s[0]=p[0];s[1]=p[1];l=2;
	if(m>1)
	{
		s[2]=p[2];l=3;
		for(i=3;i<=m;i++)
		{
			while(l>1 && (d[s[l-1]][0]-d[s[l-2]][0])*(d[p[i]][1]-d[s[l-2]][1])-(d[p[i]][0]-d[s[l-2]][0])*(d[s[l-1]][1]-d[s[l-2]][1])<eps)
				l--;
			s[l++]=p[i];
		}
	}
}

int main()
{
	int i;
	while(scanf("%d",&n)!=EOF)
	{
		for(i=1;i<=n;i++)
			scanf("%lf %lf",&d[i][0],&d[i][1]);
		GRAHAM_SCAN();
		rotating_calipers();
		printf("%.2lf\n",maxdist);
	}
	return 0;
}
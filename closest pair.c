#include <stdlib.h>
#include <math.h>

double Fcp(int n,const double *x,const double *y);
double Fcpd(int s,int t,const double *x,const double *y,const int *yo);
int* Mergesort(int n,const double *key);
void Mergesortd(int p,int q,const double *key,int *order);
void Merge(int p,int m,int q,const double *key,int *order);

/******* ******* *******

Fcp(n,x,y);

x[1-n]
y[1-n]

******* ******* *******/

double Fcp(int n,const double *xt,const double *yt)
{
	int *xo,*yo,a;
	double *x,*y,d;
	if(n<2)return -1;
	x=(double*)malloc((n+1)*sizeof(double));
	y=(double*)malloc((n+1)*sizeof(double));
	x[0]=n;
	xo=Mergesort(n,xt);
	for(a=1;a<=n;a++)
	{
		x[a]=xt[xo[a]];
		y[a]=yt[xo[a]];
	}
	free(xo);
	yo=Mergesort(n,y);
	d=Fcpd(1,n,x,y,yo);
	free(x);free(y);free(yo);
	return d;
}

double Fcpd(int s,int t,const double *x,const double *y,const int *yo)
{
	int q,a,b,*lt,*rt,ll,rl,i,j,*yl,*yr;
	double d,l,r;
	if(t-s<3)
	{
		d=pow(pow(x[t-1]-x[t],2)+pow(y[t-1]-y[t],2),0.5);
		for(a=s;a<t-1;a++)
			for(b=a+1;b<=t;b++)
				if(pow(pow(x[a]-x[b],2)+pow(y[a]-y[b],2),0.5)<d)
					d=pow(pow(x[a]-x[b],2)+pow(y[a]-y[b],2),0.5);
		return d;
	}
	q=(s+t)/2;
	yl=(int*)malloc((q-s+2)*sizeof(int));
	yr=(int*)malloc((t-q+1)*sizeof(int));
	i=0;j=0;
	for(a=1;a<=t-s+1;a++)
		if(yo[a]<=q)
			yl[++i]=yo[a];
		else
			yr[++j]=yo[a];
	l=Fcpd(s,q,x,y,yl);
	if(l==0)return 0;
	r=Fcpd(q+1,t,x,y,yr);
	d=l<r?l:r;
	r=x[q+1]-x[q];
	if(r>=d || d==0)return d;
	for(b=q;b>s && x[b-1]>x[q+1]-d;b--);
	ll=q-b+1;
	for(b=q+1;b<t && x[b+1]<x[q]+d;b++);
	rl=b-q;
	lt=(int*)malloc((ll+1)*sizeof(int));
	rt=(int*)malloc((rl+1)*sizeof(int));
	i=0;j=0;
	for(a=1;a<=q-s+1;a++)
		if(q-yl[a]<ll)lt[++i]=yl[a];
	for(a=1;a<=t-q;a++)
		if(yr[a]-q-1<rl)rt[++j]=yr[a];
	free(yl);free(yr);
	j=1;
	for(i=1;i<=ll;i++)
	{
		while(j<=rl && y[rt[j]]<=y[lt[i]]-d)j++;
		if(j==rl+1)break;
		for(b=0;b+j<=rl && y[rt[b+j]]-y[lt[i]]<d;b++)
		{
			r=pow(pow(x[rt[b+j]]-x[lt[i]],2)+pow(y[rt[b+j]]-y[lt[i]],2),0.5);
			if(r<d)d=r;
			if(d==0)break;
		}
		if(d==0)break;
	}
	free(lt);free(rt);
	return d;
}

int* Mergesort(int n,const double *key)
{
	int a;
	int *order=(int*)malloc((n+1)*sizeof(int));
	for(a=1;a<=n;a++)
		order[a]=a;
	Mergesortd(1,n,key,order);
	return order;
}

void Mergesortd(int p,int q,const double *key,int *order)
{
	int m;
	if(p>=q)
		return;
	m=(p+q)/2;
	if(p<m)Mergesortd(p,m,key,order);
	if(m+1<q)Mergesortd(m+1,q,key,order);
	Merge(p,m,q,key,order);
}

void Merge(int p,int m,int q,const double *key,int *order)
{
	int *t,k,i=p,j=m+1;
	if(p>=q)
		return;
	t=(int*)malloc((q-p+1)*sizeof(int));
	for(k=0;k<q-p+1;k++)
		if(j>q || i<=m && key[order[i]]<=key[order[j]])
			t[k]=order[i++];
		else
			t[k]=order[j++];
	for(k=0;k<q-p+1;k++)
		order[p+k]=t[k];
	free(t);
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>


struct number{
	int n;
	int *num;
};


void cmbignum(int c,struct number d,int *m);
/******* ******* *******
	stdlib.h
	memory.h
	-modint
		-Dgq
		-Dmm
		-inttobignum
		-bignumtoint
******* ******* *******/

void cmint(int c,int b,int *m);
/******* ******* *******
	stdlib.h
	memory.h
******* ******* *******/


int modint(struct number x,int y,struct number *d);
int Dgq(struct number m,struct number s[],int a);
void Dmm(struct number *m,struct number sq,int a);
struct number inttobignum(int n);
int bignumtoint(int n,int *num);

/******* ******* *******

 positive integer only

******* ******* *******/

void cmbignum(int c,struct number d,int *m)
{
	int a,i,b,l,*t,*u,*v,w,x;
	t=(int*)malloc((c+1)*sizeof(int));
	u=(int*)calloc(c+1,sizeof(int));
	v=(int*)calloc(c+2,sizeof(int));
	for(a=0;a<c;a++)
	{
		memset(t,0,(c+1)*sizeof(int));
		b=a;t[b]=1;l=1;
		while(1) 
		{
			b=(b*a)%c;
			if(t[b])
			{
				u[a]=l-t[b]+1;
				break;
			}
			t[b]=++l;
		}
		x=-1;
		if(d.n<=2)
		{
			w=d.num[0];
			if(d.n==2)w+=10*d.num[1];
			if(w<l)
				x=w;
		}
		if(x==-1)
		{
			if(u[a]==1 || v[u[a]])
				w=v[u[a]];
			else
			{
				w=modint(d,u[a]);
				v[u[a]]=w;
			}
			w-=l%u[a];
			if(w<0)w+=u[a];
			x=w;
		}
		if(a>0 && w==0)
			m[a]=1;
		else
		{
			b=a;
			for(i=1;i<x;i++)
				b=(b*a)%c;
			m[a]=b;
		}
	}
	free(t);free(u);free(v);
}

void cmint(int c,int n,int *m)
{
	int a,b,i,l,*t,*u,w;
	t=(int*)malloc((c+1)*sizeof(int));
	u=(int*)calloc(c+1,sizeof(int));
	for(a=0;a<c;a++)
	{
		memset(t,0,(c+1)*sizeof(int));
		b=a;t[b]=1;l=1;
		while(1) 
		{
			b=(b*a)%c;
			if(t[b])
			{
				u[a]=l-t[b]+1;
				break;
			}
			t[b]=++l;
		}
		if(n<l)
			w=n;
		else
		{
			w=(n-l)%u[a];
			if(w<0)w+=u[a];
		}
		if(a>0 && w==0)
			m[a]=1;
		else
		{
			b=a;
			for(i=1;i<w;i++)
				b=(b*a)%c;
			m[a]=b;
		}
	}
	free(t);free(u);
}

int modint(struct number x,int y)
{
	struct number s[10],m;
	int a,q,l,r;
	while(x.n>1 && x.num[x.n-1]==0)x.n--;
	if(x.n<=0 || y<=0)
		return -1;
	for(a=1;a<10;a++)
		s[a]=inttobignum(y*a);
	l=x.n>s[1].n?x.n:s[1].n;
	m.num=(int*)calloc(l+1,sizeof(int));
	for(a=0;a<x.n;a++)
		m.num[a]=x.num[a];
	m.n=x.n;
	l=x.n-s[1].n;
	for(a=l;a>=0;a--)
	{
		q=Dgq(m,s,a);
		if(q==0)continue;
		Dmm(&m,s[q],a);
	}
	r=bignumtoint(m);
	free(m.num);
	for(a=1;a<10;a++)
		free(s[a].num);
	return r;
}

int Dgq(struct number m,struct number s[],int a)
{
	int q=10,b;
	while(--q>0)
	{
		if(m.n<s[q].n+a)
			continue;
		else
			if(m.n>s[q].n+a)
				break;
		b=0;
		while(m.num[m.n-1-b]==s[q].num[s[q].n-1-b] && b+1<s[q].n)b++;
		if(m.num[m.n-1-b]>=s[q].num[s[q].n-1-b])break;
	}
	return q;
}

void Dmm(struct number *m,struct number sq,int a)
{
	int b;
	for(b=0;b<sq.n;b++)
	{
		m->num[a+b]-=sq.num[b];
		if(m->num[a+b]<0)
		{
			m->num[a+b]+=10;
			m->num[a+b+1]--;
		}
	}
	while(m->n>1 && m->num[m->n-1]==0)m->n--;
}

struct number inttobignum(int n)
{
	int i,t=n/10;
	struct number d;
	for(i=1;t;i++,t/=10);
	d.n=i;
	d.num=(int*)malloc(d.n*sizeof(int));
	d.num[0]=0;
	for(i=0;n;d.num[i++]=n%10,n/=10);
	return d;
}


int bignumtoint(struct number d)
{
	int i,r=0;
	for(i=d.n-1;i>=0;r=r*10+d.num[i],i--);
	return r;
}

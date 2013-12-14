
#include <stdlib.h>

struct number{
	int n;
	int *num;
};

struct number divmodbig(struct number x,struct number y,struct number *d);
/******* ******* *******
	-Dgq
	-Dmm
******* ******* *******/

struct number modbig(struct number x,struct number y);
/******* ******* *******
	-Dgq
	-Dmm
******* ******* *******/

int divmodint(struct number x,int yy,struct number *d);
/******* ******* *******
	-Dgq
	-Dmm
	-inttobignum
	-bignumtoint
******* ******* *******/

int modint(struct number x,int y,struct number *d);


int Dgq(struct number m,struct number s[],int a);
void Dmm(struct number *m,struct number sq,int a);
struct number inttobignum(int n);
int bignumtoint(struct number d);

/******* ******* *******

only positive integer

******* ******* *******/

struct number divmodbig(struct number x,struct number y,struct number *d)
{
	struct number s[10],m,r;
	int a,b,q,l;
	while(x.n>1 && x.num[x.n-1]==0)x.n--;
	while(y.n>1 && y.num[y.n-1]==0)y.n--;
	if(x.n<=0 || y.n<=0 || y.n==1 && y.num[0]==0)
	{
		d->n=0;
		d->num=NULL;
		r.n=0;
		r.num=NULL;
		return r;
	}
	for(a=1;a<10;a++)
	{
		s[a].num=(int*)calloc(y.n+1,sizeof(int));
		for(b=0;b<y.n;b++)
		{
			s[a].num[b]+=y.num[b]*a;
			s[a].num[b+1]=s[a].num[b]/10;
			s[a].num[b]%=10;
		}
		s[a].n=(s[a].num[y.n])?y.n+1:y.n;
	}
	l=x.n>y.n?x.n:y.n;
	m.num=(int*)calloc(l+1,sizeof(int));
	for(a=0;a<x.n;a++)
		m.num[a]=x.num[a];
	m.n=x.n;
	l=x.n-y.n;
	d->n=0;
	for(a=l;a>=0;a--)
	{
		q=Dgq(m,s,a);
		if(a+1>d->n)
		{
			d->n=a+1;
			d->num=(int*)malloc(d->n*sizeof(int));
		}
		d->num[a]=q;
		if(q!=0)Dmm(&m,s[q],a);
	}
	if(d->n==0)
	{
		d->n=1;
		d->num=(int*)malloc(sizeof(int));
		d->num[0]=0;
	}
	if(m.n==0)
		r.n=1;
	else
		r.n=m.n;
	r.num=m.num;
	for(a=1;a<10;a++)
		free(s[a].num);
	return r;
}

struct number modbig(struct number x,struct number y)
{
	struct number s[10],m,r;
	int a,b,q,l;
	while(x.n>1 && x.num[x.n-1]==0)x.n--;
	while(y.n>1 && y.num[y.n-1]==0)y.n--;
	if(x.n<=0 || y.n<=0 || y.n==1 && y.num[0]==0)
	{
		r.n=0;
		r.num=NULL;
		return r;
	}
	for(a=1;a<10;a++)
	{
		s[a].num=(int*)calloc(y.n+1,sizeof(int));
		for(b=0;b<y.n;b++)
		{
			s[a].num[b]+=y.num[b]*a;
			s[a].num[b+1]=s[a].num[b]/10;
			s[a].num[b]%=10;
		}
		s[a].n=(s[a].num[y.n])?y.n+1:y.n;
	}
	l=x.n>y.n?x.n:y.n;
	m.num=(int*)calloc(l+1,sizeof(int));
	for(a=0;a<x.n;a++)
		m.num[a]=x.num[a];
	m.n=x.n;
	l=x.n-y.n;
	for(a=l;a>=0;a--)
	{
		q=Dgq(m,s,a);
		if(q!=0)Dmm(&m,s[q],a);
	}
	if(m.n==0)
		r.n=1;
	else
		r.n=m.n;
	r.num=m.num;
	for(a=1;a<10;a++)
		free(s[a].num);
	return r;
}

int divmodint(struct number x,int y,struct number *d)
{
	struct number s[10],m;
	int a,q,l,r;
	while(x.n>1 && x.num[x.n-1]==0)x.n--;
	if(x.n<=0 || y<=0)
	{
		d->n=0;
		d->num=NULL;
		return -1;
	}
	for(a=1;a<10;a++)
		s[a]=inttobignum(y*a);
	l=x.n>s[1].n?x.n:s[1].n;
	m.num=(int*)calloc(l+1,sizeof(int));
	for(a=0;a<x.n;a++)
		m.num[a]=x.num[a];
	m.n=x.n;
	l=x.n-s[1].n;
	d->n=0;
	for(a=l;a>=0;a--)
	{
		q=Dgq(m,s,a);
		if(a+1>d->n)
		{
			d->n=a+1;
			d->num=(int*)malloc(d->n*sizeof(int));
		}
		d->num[a]=q;
		if(q!=0)Dmm(&m,s[q],a);
	}
	if(d->n==0)
	{
		d->n=1;
		d->num=(int*)malloc(sizeof(int));
		d->num[0]=0;
	}
	r=bignumtoint(m);
	free(m.num);
	for(a=1;a<10;a++)
		free(s[a].num);
	return r;
}

int modint(struct number x,int y)
{
	int a,r=0;
	for(a=x.n-1;a>=0;a--)
		r=(r*10+x.num[a])%y;
	return r;
}

int Dgq(struct number m,struct number s[],int a)
{
	int q=10,b;
	while(--q>0)
	{
		if(m.n<s[q].n+a)continue;
		else if(m.n>s[q].n+a)break;
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

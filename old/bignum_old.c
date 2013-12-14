
#include <string.h>
#include <stdlib.h>
#define DIGITS 4
#define BASE 10000

struct number{
	int n;
	int *num;
};

typedef long long I;

struct number strtobignum(char *s);
/******* ******* *******
	string.h
******* ******* *******/

void incbignum(struct number *x,struct number n);
struct number inttobignum(int n);
int bignumtoint(struct number d);
I bignumtoll(struct number d);

/******* ******* *******

only positive integer
strtobignum has BASE10

******* ******* *******/

struct number strtobignum(char *s)
{
	int a,l;
	struct number d;
	l=strlen(s);
	for(a=0;a<l-1 && s[a]=='0';a++);
	d.n=l-a;
	d.num=(int*)malloc(d.n*sizeof(int));
	for(;a<l;a++)
		d.num[l-a-1]=s[a]-'0';
	return d;
}

void incbignum(struct number *x,struct number n)
{
	int a;
	if(n.n==1 && n.num[0]==0)
		return;
	x->num[x->n]=0;
	for(a=x->n+1;a<=n.n;a++)
		x->num[a]=0;
	for(a=0;a<n.n;a++)
	{
		x->num[a]+=n.num[a];
		if(x->num[a]>=BASE)
		{
			x->num[a]-=BASE;
			x->num[a+1]++;
		}
	}
	for(;x->num[a]>=BASE;x->num[a]-=BASE,x->num[++a]++);
	a=x->n>a?x->n:a;
	x->n=x->num[a]?a+1:a;
}

struct number inttobignum(int n)
{
	int i,t=n/BASE;
	struct number d;
	for(i=1;t;i++,t/=BASE);
	d.n=i;
	d.num=(int*)malloc(d.n*sizeof(int));
	d.num[0]=0;
	for(i=0;n;d.num[i++]=n%BASE,n/=BASE);
	return d;
}

int bignumtoint(struct number d)
{
	int i,r=0;
	for(i=d.n-1;i>=0;r=r*BASE+d.num[i],i--);
	return r;
}

I bignumtoll(struct number d)
{
	int i;
	I r=0;
	for(i=d.n-1;i>=0;r=r*BASE+d.num[i],i--);
	return r;
}

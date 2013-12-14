#include <stdlib.h>

int* Mergesort(int n,int (*CFun)(int,int));
void Mergesortd(int p,int q,int (*CFun)(int,int),int *order,int *t);
void Merge(int p,int m,int q,int (*CFun)(int,int),int *order,int *t);
int CFun(int x,int y);

/******* ******* *******

*p=Mergesort(n,x);

x[1-n]

don't forget to free *p

use int CFun(int x,int y);

******* ******* *******/

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

int CFun(int x,int y)
{
	if(x>y)
		return 1;/*right is front*/
	else if(x==y)
		return 0;/*left is front*/
	else
		return -1;/*left is front*/
}
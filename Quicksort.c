

#include <stdlib.h>

int Partiton(int p,int r,int *A)
{
	int x=A[rand()%(r-p)+p],i=p-1,j=r+1,t;
	while(1)
	{
		while(A[--j]>x);
		while(A[++i]<x);
		if(i<j){t=A[i];A[i]=A[j];A[j]=t;}
		else return j;
	}
}

void QuickSort(int p,int r,int *A)
{
	int q;
	while(p<r)
	{
		q=Partiton(p,r,A);
		QuickSort(p,q,A);
		p=q+1;
	}
}
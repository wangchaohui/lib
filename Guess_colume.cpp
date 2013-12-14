#include <stdio.h>
#include <math.h>
#include <algorithm>
using namespace std;

#define eps 1e-8
#define N 20

double A[N+1][N+1],B[N+1];

int Gauss(int n)
{
	int i,j,k,mi;
	double m,t;
	for(k=1;k<=n;k++)
	{
		m=0;
		for(i=k;i<=n;i++)
		{
			t=fabs(A[i][k]);
			if(t>m){m=t;mi=i;}
		}
		if(m<eps)return -1;
		if(mi!=k)
		{
			for(i=k;i<=n;i++)
				swap(A[k][i],A[mi][i]);
			swap(B[k],B[mi]);
		}
		m=A[k][k];
		for(j=k;j<=n;j++)
			A[k][j]/=m;
		B[k]/=m;
		for(i=k+1;i<=n;i++)
		{
			B[i]-=A[i][k]*B[k];
			for(j=n;j>=k;j--)
				A[i][j]-=A[i][k]*A[k][j];
		}
	}
	for(i=n-1;i>=1;i--)
		for(j=i+1;j<=n;j++)
			B[i]-=A[i][j]*B[j];
	return 0;
}

int main()
{
	int i,j,n;
	scanf("%d",&n);
	for(i=1;i<=n;i++)
		for(j=1;j<=n;j++)
			scanf("%lf",&A[i][j]);
	for(i=1;i<=n;i++)
		scanf("%lf",&B[i]);
	Gauss(n);
	for(i=1;i<=n;i++)
		printf("%.2f\n",B[i]);
	return 0;
}

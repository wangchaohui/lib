#include <stdio.h>
#include <math.h>
#include <algorithm>
using namespace std;

#define eps 1e-8
#define N 20

double A[N+1][N+2];

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
		for(i=k;i<=n+1;i++)
			swap(A[k][i],A[mi][i]);
		m=A[k][k];
		for(j=k;j<=n+1;j++)
			A[k][j]/=m;
		for(i=k+1;i<=n;i++)
		{
			for(j=n+1;j>=k;j--)
				A[i][j]-=A[i][k]*A[k][j];
		}
	}
	for(i=n-1;i>=1;i--)
		for(j=i+1;j<=n;j++)
			A[i][n+1]-=A[i][j]*A[j][n+1];
	return 0;
}

int main()
{
	int i,j,n;
	scanf("%d",&n);
	for(i=1;i<=n;i++)
		for(j=1;j<=n+1;j++)
			scanf("%lf",&A[i][j]);
	if(Gauss(n)==0)
		for(i=1;i<=n;i++)
			printf("%.2f\n",A[i][n+1]);
	return 0;
}

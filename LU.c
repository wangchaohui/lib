#include <stdio.h>
#include <math.h>
#define swap(a,b){t=a;a=b;b=t;}
#define eps 1e-8
#define N 20

double A[N+1][N+1],B[N+1];

int LU(int n)
{
	int i,j,k,pivrow[N+1];
	double pivot,t;
	for(j=1;j<=n;j++)
	{
		for(i=1;i<j;i++)
			for(k=1;k<i;k++)
				A[i][j]-=A[i][k]*A[k][j];
		pivot=0;
		for(i=j;i<=n;i++)
		{
			for(k=1;k<j;k++)
				A[i][j]-=A[i][k]*A[k][j];
			if(fabs(A[i][j])>pivot)
			{
				pivrow[j]=i;
				pivot=fabs(A[i][j]);
			}
		}
		if(pivot<eps)return -1;
		if(pivrow[j]!=j)
			for(k=1;k<=n;k++)
				swap(A[pivrow[j]][k],A[j][k]);
		for(i=j+1;i<=n;i++)
			A[i][j]/=A[j][j];
	}
	for(i=1;i<=n;i++)
	{
		swap(B[pivrow[i]],B[i]);
		for(j=1;j<i;j++)
			B[i]-=A[i][j]*B[j];
	}
	for(i=n;i>=1;i--)
	{
		for(j=i+1;j<=n;j++)
			B[i]-=A[i][j]*B[j];
		B[i]/=A[i][i];
	}
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
	LU(n);
	for(i=1;i<=n;i++)
		printf("%.2lf\n",B[i]);
	return 0;
}

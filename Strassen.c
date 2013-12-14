
#include <stdio.h>
#include <stdlib.h>

#define maxn 64
#define p(x) printf(#x"\n");for(i=0;i<n;i++){for(j=0;j<n-1;j++)printf("%d ",x[i][j]);printf("%d\n",x[i][j]);}

int A[maxn][maxn],B[maxn][maxn],C[maxn][maxn],A1[maxn][maxn],B1[maxn][maxn],C1[maxn][maxn];

void Strassen(int n,int AA[][maxn],int BB[][maxn],int CC[][maxn])
{
	int i,j,**A,**B,**C;
	if(n==1)
	{
		CC[0][0]=AA[0][0]*BB[0][0];
		return;
	}
	if(n==2)
	{
		CC[0][0]=AA[0][0]*BB[0][0]+AA[0][1]*BB[1][0];
		CC[0][1]=AA[0][0]*BB[0][1]+AA[0][1]*BB[1][1];
		CC[1][0]=AA[1][0]*BB[0][0]+AA[1][1]*BB[1][0];
		CC[1][1]=AA[1][0]*BB[0][1]+AA[1][1]*BB[1][1];
		return;
	}
	A=(int**)malloc(n*sizeof(int*));
	B=(int**)malloc(n*sizeof(int*));
	C=(int**)malloc(n*sizeof(int*));
	for(i=0;i<n;i++)
	{
		A[i]=(int*)malloc(n*sizeof(int));
		B[i]=(int*)malloc(n*sizeof(int));
		C[i]=(int*)malloc(n*sizeof(int));
		for(j=0;j<n;j++)
		{
			A[i][j]=AA[i][j];
			B[i][j]=BB[i][j];
		}
	}
	n/=2;
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
		{
			A1[i][j]=A[i][j];
			B1[i][j]=B[i][n+j]-B[n+i][n+j];
		}
	Strassen(n,A1,B1,C1);
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
		{
			C[i][n+j]=C1[i][j];
			C[n+i][n+j]=C1[i][j];
			A1[i][j]=A[i][j]+A[i][n+j];
			B1[i][j]=B[n+i][n+j];
		}
	Strassen(n,A1,B1,C1);
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
		{
			C[i][j]=-C1[i][j];
			C[i][n+j]+=C1[i][j];
			A1[i][j]=A[n+i][j]+A[n+i][n+j];
			B1[i][j]=B[i][j];
		}
	Strassen(n,A1,B1,C1);
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
		{
			C[n+i][j]=C1[i][j];
			C[n+i][n+j]-=C1[i][j];
			A1[i][j]=A[n+i][n+j];
			B1[i][j]=B[n+i][j]-B[i][j];
		}
	Strassen(n,A1,B1,C1);
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
		{
			C[n+i][j]+=C1[i][j];
			C[i][j]+=C1[i][j];
			A1[i][j]=A[i][j]+A[n+i][n+j];
			B1[i][j]=B[i][j]+B[n+i][n+j];
		}
	Strassen(n,A1,B1,C1);
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
		{
			C[i][j]+=C1[i][j];
			C[n+i][n+j]+=C1[i][j];
			A1[i][j]=A[i][n+j]-A[n+i][n+j];
			B1[i][j]=B[n+i][j]+B[n+i][n+j];
		}
	Strassen(n,A1,B1,C1);
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
		{
			C[i][j]+=C1[i][j];
			A1[i][j]=A[i][j]-A[n+i][j];
			B1[i][j]=B[i][j]+B[i][n+j];
		}
	Strassen(n,A1,B1,C1);
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			C[n+i][n+j]-=C1[i][j];
	n*=2;
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			CC[i][j]=C[i][j];
	for(i=0;i<n;i++)
	{
		free(A[i]);
		free(B[i]);
		free(C[i]);
	}
	free(A);
	free(B);
	free(C);
}

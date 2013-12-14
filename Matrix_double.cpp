#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <assert.h>
using namespace std;

#define eps 1e-5
#define N 50

typedef struct{
	int r,c;
	double d[N][N];
}Matrix;

void Mread(Matrix &x)
{
	int i,j;
	for(i=0;i<x.r;i++)
		for(j=0;j<x.c;j++)
			scanf("%lf",&x.d[i][j]);
}

void Mprint(Matrix &x)
{
	int i,j;
	for(i=0;i<x.r;i++)
	{
		for(j=0;j<x.c;j++)
		{
			if(fabs(x.d[i][j])<eps)x.d[i][j]=0;
			printf("%.2f",x.d[i][j]);
			if(j!=x.c-1)putchar(' ');
			else putchar('\n');
		}
	}
}

int Gauss(Matrix &x)
{
	int i,j,k,mi;
	double value=1;
	for(k=0;k<x.r;k++)
	{
		mi=k;
		for(i=k+1;i<x.r;i++)if(fabs(x.d[i][k])>fabs(x.d[mi][k]))mi=i;
		if(fabs(x.d[mi][k])<eps){puts("Gauss Error");return -1;}
		if(k!=mi)value*=-1;
		for(i=k;i<x.c;i++)
			swap(x.d[k][i],x.d[mi][i]);
		value*=x.d[k][k];
		for(j=x.c-1;j>=k;j--)
			x.d[k][j]/=x.d[k][k];
		for(i=k+1;i<x.r;i++)
			for(j=x.c-1;j>=k;j--)
				x.d[i][j]-=x.d[i][k]*x.d[k][j];
	}
	printf("%.2f\n",value);
	for(k=x.r-1;k>=0;k--)
		for(i=k+1;i<x.r;i++)
			for(j=x.c-1;j>=i;j--)
				x.d[k][j]-=x.d[k][i]*x.d[i][j];
	return 0;
}

Matrix Mni(Matrix &x)
{
	Matrix y;
	int i,j;
	assert(x.r==x.c);
	if(x.r!=x.c)puts("Mni Error");
	y=x;y.c=2*y.r;
	for(i=0;i<y.r;i++)
		for(j=0;j<y.r;j++)
			y.d[i][j+y.r]=(i==j);
	assert(Gauss(y)==0);
	for(i=0;i<y.r;i++)
		for(j=0;j<y.r;j++)
			y.d[i][j]=y.d[i][j+y.r];
	y.c=y.r;
	return y;
}

int main()
{
	int n;
	Matrix x,y;
	while(scanf("%d",&n)==1)
	{
		x.r=x.c=n;
		Mread(x);
		y=Mni(x);
		Mprint(y);
	}
	return 0;
}

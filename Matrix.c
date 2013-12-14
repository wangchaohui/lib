
#define N 2

typedef __int64 I;

typedef struct{
	int r,c;
	int d[N][N];
}Matrix;

Matrix MI(int n)
{
	Matrix y;
	int i,j;
	y.r=y.c=n;
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			y.d[i][j]=(i==j);
	return y;
}

Matrix M2(Matrix x,int m)
{
	Matrix y;
	int i,j,k;
	y.r=y.c=x.r;
	for(i=0;i<x.r;i++)
		for(j=0;j<x.r;j++)
		{
			y.d[i][j]=0;
			for(k=0;k<x.r;k++)
				y.d[i][j]=(y.d[i][j]+(I)x.d[i][k]*x.d[k][j])%m;
		}
		return y;
}

Matrix MM(Matrix x,Matrix y,int m)
{
	Matrix z;
	int i,j,k;
	z.r=x.r;z.c=y.c;
	for(i=0;i<x.r;i++)
		for(j=0;j<y.c;j++)
		{
			z.d[i][j]=0;
			for(k=0;k<x.c;k++)
				z.d[i][j]=(z.d[i][j]+(I)x.d[i][k]*y.d[k][j])%m;
		}
		return z;
}

Matrix Mpower(Matrix x,int b,int m)
{
	Matrix d=MI(x.r);
	int i,bb[64];
	for(i=0,bb[0]=b%2,b/=2;b;bb[++i]=b%2,b/=2);
	do{
		d=M2(d,m);
		if(bb[i]==1)
			d=MM(d,x,m);
	}while(--i>=0);
	return d;
}
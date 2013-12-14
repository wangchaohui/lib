#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

#define N 50005
#define LN 16 // 2^LN>=N
int M[N][LN+1],A[N],L[N+1];
int M2[N][LN+1],B[N];

void initlg(int n)
{
	int i;
	L[0]=-1;
	for(i=1;i<=n;i++)
		if((i&(i-1))==0)
			L[i]=L[i-1]+1;
		else
			L[i]=L[i-1];
}

void RMQinit(int (*M)[LN+1],int *a,int n)
{
	int i,j,t1,t2,j1=2,j2=1;
	for(i=0;i<n;i++)M[i][0]=i;
	for(j=1;j1<=n;j++)
	{
		for(i=0;i+j1-1<n;i++)
		{
			t1=M[i][j-1];t2=M[i+j2][j-1];
			M[i][j]=a[t1]<a[t2]?t1:t2;
		}
		j1*=2;j2*=2;
	}
}

int RMQ(int (*M)[LN+1],int *a,int i,int j)
{  
	int k=L[j-i+1],t1=M[i][k],t2=M[j-(1<<k)+1][k];
	return a[t1]<=a[t2]?t1:t2;
}

int main()
{
	int n,Q,i;
	int a,b;
	initlg(50000);
	while(scanf("%d %d",&n,&Q)==2)
	{
		for(i=0;i<n;i++)
		{
			scanf("%d",&A[i]);
			B[i]=-A[i];
		}
		RMQinit(M,A,n);
		RMQinit(M2,B,n);
		while(Q--)
		{
			scanf("%d %d",&a,&b);
			a--;b--;
			//printf(":%d %d\n",B[RMQ(M2,B,a,b)],A[RMQ(M,A,a,b)]);
			printf("%d\n",-B[RMQ(M2,B,a,b)]-A[RMQ(M,A,a,b)]);
		}
	}
	return 0;
}



// 二维静态RMQ O(N*M*logN*logM)空间和时间，O(1)查询. 左上角(0,0)，右下角(n-1,m-1)。
const int maxN=300, maxM=300, logmaxN=9, logmaxM=9;
int a[maxN][maxM];
int f[logmaxN][logmaxM][maxN][maxM];
int n, m, lg[maxN+1], maxsize;
inline int max2( int a, int b){ return a>b?a:b; }

void initlg(){
    int i; maxsize=max2(maxN, maxM);
    for( lg[0]=-1, i=1; i<=maxsize; i++) lg[i]=((i&(i-1))==0) ? lg[i-1]+1 : lg[i-1];
}

void RMQ() {
    int i, j, ii, jj, limit1, limit2;
    for ( i = 0; i < n; i++)  for ( j = 0; j < m; j++) f[0][0][i][j] = a[i][j];
    for ( i = 0; i <= lg[n]; i++) {
        limit1 = n + 1 - (1 << i);
        for ( j = 0; j <= lg[m]; j++) {
            if (!i && !j) continue;
            limit2 = m + 1 - (1 << j);
            for ( ii = 0; ii <= limit1; ii++) {
                for ( jj = 0; jj <= limit2; jj++) {
                    if (i) f[i][j][ii][jj] = max(f[i - 1][j][ii + (1 << i >> 1)][jj], f[i - 1][j][ii][jj]);
                    else f[i][j][ii][jj] = max(f[i][j - 1][ii][jj], f[i][j - 1][ii][jj + (1 << j >> 1)]);
                }
            }
        }
    }
}

int askRMQ(int r1, int c1, int r2, int c2) {
    int kn = lg[r2-r1+1], km = lg[c2-c1+1];
    r2 -=(1<<kn)-1; c2 -=(1<<km)-1;
    return max( max( f[kn][km][r1][c1], f[kn][km][r1][c2]), max(f[kn][km][r2][c1], f[kn][km][r2][c2]) );
}


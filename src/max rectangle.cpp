#include <stdio.h>
#include <memory.h>
#include <algorithm>
using namespace std;

template<class T> inline void checkmin(T &a,T b){if(b<a) a=b;}//NOTES:checkmin( 
template<class T> inline void checkmax(T &a,T b){if(b>a) a=b;}//NOTES:checkmax( 

int s[200][200]={0};
int H[200],H1[200],L[200],R[200],L1[200],R1[200];
char u[200][200],uu[200][200];

int n,m,S;

const int maxN=200, maxM=200, logmaxN=8, logmaxM=8;
int a[maxN][maxM];
int f[logmaxN][logmaxM][maxN][maxM];
int lg[maxN+1], maxsize;
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

int solve(int p,int q,int x,int y)
{
	int i,j,max=0,t,tt;
	for(j=q-1;j<=y+1;j++)u[p-1][j]=1;
	for(i=p;i<=x;i++){u[i][q-1]=1;u[i][y+1]=1;}
	for(i=p-1;i<=x;i++)
	{
		for(j=q-1;j<=y+1;j++)
			if(u[i][j]){H[j]=0;L[j]=j+1;R[j]=j-1;}
			else{H[j]=H1[j]+1;L[j]=L[j-1];}
		for(j=y;j>=q;j--)if(!u[i][j])R[j]=R[j+1];
		for(j=q;j<=y;j++)if(!u[i][j])
		{
			if(!u[i-1][j] && L[j]<L1[j])L[j]=L1[j];
			if(!u[i-1][j] && R[j]>R1[j])R[j]=R1[j];
			t=s[i][R[j]]-s[i-H[j]][R[j]]-s[i][L[j]-1]+s[i-H[j]][L[j]-1];
			tt=askRMQ(i-H[j],L[j]-1,i-1,R[j]-1);
			if(tt>0)
				t+=tt*(S-1);
			checkmax(max,t);	
		}
		memcpy(L1,L,sizeof(L));
		memcpy(R1,R,sizeof(R));
		memcpy(H1,H,sizeof(H));
	}
	return max;
}

int main()
{
	int T,C=0,Q,i,j,c,t;
	int p,q,x,y;
	initlg(  );
	scanf("%d",&T);
	while(++C<=T)
	{
		if(C>1)putchar('\n');
		printf("Case %d:\n",C);
		scanf("%d %d %d %d",&n,&m,&Q,&S);
		memset(uu,1,sizeof(uu));
		for(i=1;i<=n;i++)
		{
			c=0;
			for(j=1;j<=m;j++)
			{
				scanf("%d",&t);
				a[i-1][j-1]=t;
				if(~t)uu[i][j]=0;
				c+=t;
				s[i][j]=s[i-1][j]+c;
			}
		}
		RMQ();
		while(Q--)
		{
			scanf("%d %d %d %d",&p,&q,&x,&y);
			memcpy(u,uu,sizeof(u));
			t=solve(p,q,x,y);
			printf("%d\n",t);
		}
	}
	return 0;
}
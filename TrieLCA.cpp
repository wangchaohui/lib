#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <ctype.h>
using namespace std;

#define N 200015
#define LN 18 // 2^LN>=N
#define maxn 100010
#define maxw 2000000000
#define CS 26

int M[N][LN+1],A[N],L[N+1];

int tot;

struct Trie{
	int next[CS];
	char f;
	int n;
};

Trie Tr[maxn];

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

void RMQinit(int *a,int n)
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

int RMQ(int *a,int i,int j)
{  
	int k=L[j-i+1],t1=M[i][k],t2=M[j-(1<<k)+1][k];
	return a[t1]<=a[t2]?t1:t2;
}

int P[2*maxn-1],D[2*maxn-1],X[maxn+1];
int l;

void dfs(int x,int depth)
{
	int y,i;
	X[x]=l;P[l]=x;D[l]=depth;l++;
	for(i=0;i<CS;i++)
	{
		y=Tr[x].next[i];
		if(y==-1)continue;
		dfs(y,depth+1);
		P[l]=x;D[l]=depth;l++;
	}
}

void LCAinit()
{
	l=0;
	dfs(0,0);
	RMQinit(D,l);
}

int LCA(int x,int y)
{
	return P[RMQ(D,min(X[x],X[y]),max(X[x],X[y]))];
}

void init(int x)
{
	int i;
	for(i=0;i<CS;i++)Tr[x].next[i]=-1;
	Tr[x].f=0;
}

int trin(int n,char *x)
{
	int i,id=0,lev=0;
	for(i=0;i<n && ~Tr[id].next[x[i]];i++)id=Tr[id].next[x[i]],lev++;
	for(;i<n;i++){Tr[id].next[x[i]]=++tot;init(tot);id=tot;Tr[id].n=++lev;}
	Tr[id].f=1;
	return id;
}

int trfi(int n,char *x)
{
	int i,id=0;
	for(i=0;i<n && ~Tr[id].next[x[i]];i++)id=Tr[id].next[x[i]];
	if(i!=n)return -1;
	return id;
}

void ptr(int x,int c,int l)
{
	int i;
	printf("%*c%c",l,' ',c);if(Tr[x].f)putchar('*');putchar('\n');
	for(i=0;i<CS;i++)if(~Tr[x].next[i])
	{
		ptr(Tr[x].next[i],i+'a',l+2);
	}
}

char s[100010];
int W[100010];

int main()
{
	int T,C=0,n,l,i,j,ans=0;
	int a,b;
	initlg(200000);
	scanf("%d",&T);
	while(++C<=T)
	{
		tot=0;
		init(0);
		Tr[0].n=0;
		scanf("%d ",&n);
		for(i=0;i<n;i++)
		{
			gets(s);
			l=strlen(s);
			for(j=0;j<l;j++)s[j]-='A';
			W[i]=trin(l,s);
		}
		//ptr(0,'R',1);
		LCAinit();
		printf("Case %d:\n",C);
		scanf("%d",&n);
		while(n--)
		{
			scanf("%d %d",&a,&b);
			a=W[a];b=W[b];
			printf("%d\n",Tr[LCA(a,b)].n);
		}
	}	
	return 0;
}

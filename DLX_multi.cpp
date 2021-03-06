#include <stdio.h>
#include <memory.h>
#define maxn 1010
#define maxm 1010
#define maxt 1010*110

int S[maxm],O[maxn],L[maxt],R[maxt],U[maxt],D[maxt],C[maxt],W[maxt];
int mat[maxn][maxm];
int ANS;

void build(int n,int m)
{
	int i,j,tot=m,first;
	R[m]=0;L[0]=m;
	for(i=1;i<=m;i++)
	{
		L[i]=i-1;R[i-1]=i;
		U[i]=D[i]=C[i]=i;S[i]=0;
	}
	for(i=1;i<=n;i++)
	{
		first=0;
		for(j=1;j<=m;j++)if(mat[i][j])
		{
			tot++;
			U[tot]=U[j];D[tot]=j;
			D[U[j]]=tot;U[j]=tot;
			if(first==0)first=L[tot]=R[tot]=tot;
			else
			{
				L[tot]=L[first];R[tot]=first;
				R[L[first]]=tot;L[first]=tot;
			}
			W[tot]=i;C[tot]=j;S[j]++;
		}
	}
}

void remove(int c)
{
	int i,j;
	//remove column c;
	for(i=D[c];i!=c;i=D[i])
	{
		L[R[i]]=L[i];
		R[L[i]]=R[i];
		S[C[i]]--;
	}
}

void resume(int c)
{
	int i,j;
	for(i=U[c];i!=c;i=U[i])
	{
		S[C[i]]++;
		L[R[i]]=i;
		R[L[i]]=i;
	}
}

int H()
{
	bool hash[51]={0};
	int i,j,c,r=0;
	for(c=R[0];c!=0;c=R[c])
		if(!hash[c])
		{
			r++;
			hash[c]=true;
			for(i=D[c];i!=c;i=D[i])
				for(j=R[i];j!=i;j=R[j])
					hash[C[j]]=true;
		}
	return r;
}

int dfs(int k)
{
	int i,j,t,m=maxn,mn=0;
	if(k+H()>=ANS)return 0;
	if(R[0]==0)
	{
		//One of the answers has been found.
		ANS=k;
		return 1;
	}
	for(t=R[0];t!=0;t=R[t])if(S[t]<m){m=S[t];mn=t;}
	for(i=D[mn];i!=mn;i=D[i])
	{
		remove(i);
		O[k]=i;//record the answer.
		for(j=R[i];j!=i;j=R[j])
			remove(j);
		dfs(k+1);
		for(j=L[i];j!=i;j=L[j])
			resume(j);
		resume(i);
	}
	return 0;
}

int main()
{
	int n,m,i,j,c;
	while(scanf("%d %d",&n,&m)==2)
	{
		memset(mat,0,sizeof(mat));
		for(i=1;i<=n;i++)
		{
			scanf("%d",&c);
			while(c--)
			{
				scanf("%d",&j);
				mat[i][j]=1;
			}
		}
		build(n,m);
		ANS=n;
		if(dfs(0)==0)puts("NO");
		else
		{
			printf("%d",ANS);
			for(i=0;i<ANS;i++)printf(" %d",W[O[i]]);
			putchar('\n');
		}
	}
	return 0;
}
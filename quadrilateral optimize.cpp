#include <stdio.h>
#include <memory.h>
 
#define maxw 2000000000
 
int x[2000],y[2000];
int f[2000][2000];
int s[2000][2000];
 
int dfs(int a,int b)
{
	int i,g;
	if(~f[a][b])return f[a][b];
	f[a][b]=maxw;
	for(i=a;i<b;i++)
	{
		g=dfs(a,i)+dfs(i+1,b);
		if(f[a][b]>g+x[i+1]-x[a]+y[i]-y[b])
			f[a][b]=g+x[i+1]-x[a]+y[i]-y[b];
	}
	if(a==b)f[a][b]=0;
	return f[a][b];
}
 
int main()
{
	int n,i,c;
	int r,j,k,i1,j1;
	while(scanf("%d",&n)==1)
	{
		c=0;
/*
		for(i=0;i<n;i++)
			for(j=i;j<n;j++)
				f[i][j]=-1;
*/
		for(i=0;i<n;i++)
		{
			scanf("%d %d",&x[i],&y[i]);
			c+=x[i]+y[i];
		}
		for(i=0;i<n;i++){f[i][i]=0;s[i][i]=0;}
		for(r=1;r<n;r++)
			for(i=0;i<n-r;i++)
			{
				j=i+r;i1=s[i][j-1]>i?s[i][j-1]:i;j1=s[i+1][j]>i?s[i+1][j]:j-1;
				f[i][j]=f[i][i1]+f[i1+1][j];
				s[i][j]=i1;
				for(k=i1;k<=j1;k++)
				{
					if(f[i][j]>=f[i][k]+f[k+1][j]){f[i][j]=f[i][k]+f[k+1][j];s[i][j]=k;}
				}
				f[i][j]-=x[i]+y[j];
			}
		printf("%d\n",f[0][n-1]+c-x[0]-y[n-1]);
		//printf("%d\n",dfs(0,n-1));
	}
	return 0;
}

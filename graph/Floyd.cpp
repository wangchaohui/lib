#include <stdio.h>
#include <memory.h>
template<class T> inline void checkmin(T &a,T b){if(b<a) a=b;}//NOTES:checkmin( 
template<class T> inline void checkmax(T &a,T b){if(b>a) a=b;}//NOTES:checkmax(

int d[110][110];
int n,dist[110][110];

void Floyd()
{
	int i,j,k;
	for(k=1;k<=n;k++)
		for(i=1;i<=n;i++)if(dist[i][k]!=-1)
			for(j=1;j<=n;j++)if(dist[k][j]!=-1)
				if(dist[i][j]==-1)dist[i][j]=dist[i][k]+dist[k][j];
				else checkmin(dist[i][j],dist[i][k]+dist[k][j]);
}

int main()
{
	int i,j,a,b,c,m,mn;
	while(scanf("%d",&n)==1 && n)
	{
		for(i=1;i<=n;i++)
		{
			for(j=1;j<=n;j++)d[i][j]=-1;
			d[i][i]=0;
			scanf("%d",&j);
			while(j--)
			{
				scanf("%d %d",&a,&b);
				d[i][a]=b;
			}
		}
		memcpy(dist,d,sizeof(d));
		Floyd();
		m=200000000;
		for(i=1;i<=n;i++)
		{
			c=0;
			for(j=1;j<=n;j++)
			{
				if(dist[i][j]>c)c=dist[i][j];
				if(dist[i][j]==-1)break;
			}
			if(j!=n+1)continue;
			if(c<m)
			{
				m=c;
				mn=i;
			}
		}
		if(m==200000000)puts("disjoint");
		else printf("%d %d\n",mn,m);
	}
	return 0;
}
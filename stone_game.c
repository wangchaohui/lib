#include <stdio.h>
#define maxn 1000

int f[maxn+1],g[maxn+1];

void stone_game()
{
	int i,j;
	f[0]=0;f[1]=0;
	for(i=2;i<=maxn;i++)
	{
		for(j=0;j<=i;j++)g[j]=0;
		for(j=0;j<=(i-2)/2;j++)
			g[f[j]^f[i-j-2]]=1;
		for(j=0;g[j];j++);
		f[i]=j;
	}
}

int main()
{
	int p,n,i,t,s;
	stone_game();
	scanf("%d",&p);
	while(p--)
	{
		scanf("%d",&n);
		s=0;
		for(i=0;i<n;i++)
		{
			scanf("%d",&t);
			s^=f[t];
		}
		if(s)puts("Win");
		else puts("Lost");
	}
	return 0;
}

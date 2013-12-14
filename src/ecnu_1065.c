#include <stdio.h>
#include <memory.h>

int s[1001][1001]={0},height[1002],height1[1002],left[1002],right[1002],left1[1002],right1[1002];
char u[1001][1002];

int main()
{
	int i,j,n,m,t,c,max=0;
	scanf("%d %d",&n,&m);
	memset(u,1,sizeof(u));
	for(i=1;i<=n;i++)
	{
		c=0;
		for(j=1;j<=m;j++)
		{
			scanf("%d",&t);
			if(t)u[i][j]=0;
			c+=t;
			s[i][j]=s[i-1][j]+c;
		}
	}
	for(i=0;i<=n;i++)
	{
		for(j=0;j<=m+1;j++)
			if(u[i][j])
			{
				height[j]=0;
				left[j]=j+1;
				right[j]=j-1;
			}
			else
			{
				height[j]=height1[j]+1;
				left[j]=left[j-1];
			}
		for(j=m;j>=1;j--)
			if(!u[i][j])	
				right[j]=right[j+1];
		for(j=1;j<=m;j++)
			if(!u[i][j])
			{
				if(!u[i-1][j])
				{
					if(left[j]<left1[j])left[j]=left1[j];
					if(right[j]>right1[j])right[j]=right1[j];
				}
				t=s[i][right[j]]-s[i-height[j]][right[j]]
				-s[i][left[j]-1]+s[i-height[j]][left[j]-1];
				if(t>max)max=t;
			}
		memcpy(left1,left,sizeof(left));
		memcpy(right1,right,sizeof(right));
		memcpy(height1,height,sizeof(height));
	}
	printf("%d\n",max);
	return 0;
}
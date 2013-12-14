#include <stdio.h>
#include <memory.h>
#include <algorithm>
#include <map>
using namespace std;

typedef __int64 I;

char d[13][13];
I f[2][42000];
int pre,now;
int W[14];
unsigned short L[1594323];
int st[42000];
int tot;

bool check(int num)
{
	int cnt = 0;
	while (num)
	{
		int k = num % 3;
		if (k == 1) cnt++;
		if (k == 2) cnt--;
		num /= 3;
		if (cnt < 0) return false;
	}
	if (cnt != 0) return false;
	return true;
}

int get(int k,int n)
{
	return k/W[n]%3;
}

void init()
{
	int i;
	W[0]=1;
	for(i=1;i<14;i++)W[i]=W[i-1]*3;
	tot=0;
	for(i=0;i<W[13];i++)
	{
		if(check(i))
		{
			st[tot]=i;
			L[i]=tot;
			tot++;
		}
	}
}

int main()
{
	int T,C=0,i,j,k,kk,n,m,a,e0,e1,mi,mj,c,g;
	I t,ans;
	init();
	scanf("%d",&T);
	while(++C<=T)
	{
		scanf("%d %d",&n,&m);
		for(i=1;i<=n;i++)
			for(j=1;j<=m;j++)
				scanf(" %c",&d[i][j]);
		pre=0;now=1;
		memset(f[pre],0,sizeof(f[pre]));
		f[pre][0]=1;
		for(i=1;i<=n;i++)
			for(j=1;j<=m;j++)
				if(d[i][j]=='O')mi=i,mj=j;
		ans=0;
		for(i=1;i<=n;i++)
		{
			memset(f[now],0,sizeof(f[now]));
			for(k=0;k<tot && st[k]<W[m];k++)
				f[now][L[st[k]*3]]=f[pre][k];
			swap(pre,now);
			for(j=1;j<=m;j++)
			{
				//printf("%d %d\n",i,j);
				//for(kk=0;kk<tot;kk++)if(f[pre][kk])printf(":%d %d\n",st[kk],f[pre][kk]);
				//puts("");
				memset(f[now],0,sizeof(f[now]));
				for(kk=0;kk<tot && st[kk]<W[m+1];kk++)
				{
					k=st[kk];
					t=f[pre][kk];
					if(!t)continue;
					e0=get(k,j-1);e1=get(k,j);
					if(d[i][j]=='O' || d[i][j]=='*')
					{
						if(e0==0)
						{
							if(e1==0)
								f[now][L[k+W[j-1]+W[j]*2]]+=t;
							else if(e1==1)
							{
								f[now][kk]+=t;
								f[now][L[k+W[j-1]-W[j]]]+=t;
							}
							else if(e1==2)
							{
								f[now][kk]+=t;
								f[now][L[k+W[j-1]*2-W[j]*2]]+=t;
							}
						}
						else if(e0==1)
						{
							if(e1==0)
							{
								f[now][kk]+=t;
								f[now][L[k-W[j-1]+W[j]]]+=t;
							}
							else if(e1==1)
							{
								c=1;
								for(a=j;c;)
								{
									a++;
									g=get(k,a);
									if(g==1)c++;
									if(g==2)c--;
								}
								f[now][L[k-W[j-1]-W[j]-W[a]]]+=t;
							}
							else if(e1==2)
							{
								if((i>mi || i==mi && j>=mj) && k-W[j-1]-W[j]*2==0)ans+=t; 
							}
						}
						else if(e0==2)
						{
							if(e1==0)
							{
								f[now][kk]+=t;
								f[now][L[k-W[j-1]*2+W[j]*2]]+=t;
							}
							else if(e1==1)
							{
								f[now][L[k-W[j-1]*2-W[j]]]+=t;
							}
							else if(e1==2)
							{
								c=1;
								for(a=j-1;c;)
								{
									a--;
									g=get(k,a);
									if(g==2)c++;
									if(g==1)c--;
								}
								f[now][L[k-W[j-1]*2-W[j]*2+W[a]]]+=t;
							}
						}
					}
					if(d[i][j]=='*' || d[i][j]=='X')
					{
						if(e0==0 && e1==0)
						{
							f[now][kk]+=t;
						}
					}
				}
				swap(pre,now);
			}
		}
		printf("Case %d: %I64d\n",C,ans);
	}
	return 0;
}
#include <stdio.h>

int nimB[16][16],nim[1000][1000];

int nimmul(int x,int y);

void nimmul_init(int n)
{
	int i,j,s=1;
	for(i=0;s<n;i++)s<<=1;
	s=i+1;
	for(i=1;i<s;i++)
		for(j=1;j<s;j++)
			nimB[i][j]=-1;
	for(i=0;i<s;i++)
		nimB[0][i]=nimB[i][0]=1<<i;
	for(i=1;i<s;i*=2)
		nimB[i][i]=3*(1<<(i-1));
}

int nimmulb(int x,int y)
{
	int t,i;
	if(x<y){t=x;x=y;y=t;}
	if(nimB[x][y]!=-1)return nimB[x][y];
	for(i=1;i*2<=x;i*=2);
	if(i==x)nimB[x][y]=1<<(x+y);
	else if(i==y)nimB[x][y]=nimmul(nimmulb(i,y),1<<(x-i));
	else nimB[x][y]=nimmul(nimmulb(x-i,y),1<<i);
	return nimB[x][y];
}

int nimmul(int x,int y)
{
	int a,b,i,j,bx[64],by[64],r=0;
	for(i=0;x;x/=2)bx[i++]=x%2;
	for(j=0;y;y/=2)by[j++]=y%2;
	for(a=0;a<i;a++)
		if(bx[a])
			for(b=0;b<j;b++)
				if(by[b])
					r^=nimmulb(a,b);
	return r;
}



int main()
{
	int T,n,i,a,b,s;
	int j;
	nimmul_init(10000);
	for(i=0;i<100;i++)
		for(j=0;j<100;j++)
		{
			nim[i][j]=nimmul(i,j);
		}
	scanf("%d",&T);
	while(T--)
	{
		scanf("%d",&n);
		s=0;
		for(i=0;i<n;i++)
		{
			scanf("%d %d",&a,&b);
			s^=nimmul(a,b);
		}
		if(s)puts("Have a try, lxhgww.");
		else puts("Don't waste your time.");
	}
	return 0;
}

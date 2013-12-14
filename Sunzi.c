#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define DIGITS 1
#define BASE 10

typedef long long I;
typedef int BG[600];

BG M;

void strtobig(char *s,int *d)
{
	int a,l;
	l=strlen(s);
	for(a=0;a<l-1 && s[a]=='0';a++);
	d[0]=l-a;
	for(;a<l;a++)d[l-a]=s[a]-'0';
}

void incbig(int *x,int *n)
{
	int a,l;
	if(n[0]==1 && n[1]==0)return;
	l=x[0]>n[0]?x[0]+1:n[0]+1;
	for(a=x[0]+1;a<=l;a++)x[a]=0;
	for(a=1;a<=n[0];a++)x[a]+=n[a];
	for(a=1;a<l;a++)
		if(x[a]>=BASE){x[a]-=BASE;x[a+1]++;}
		x[0]=x[l]?l:l-1;
}

void prbig(int *x)
{
	int a,l;
	char s[8];
	sprintf(s,"%%0%dd",DIGITS);
	printf("%d",x[x[0]]);
	for(a=x[0]-1;a>=1;a--)
		printf(s,x[a]);
}

void inttobig(int n,int *d)
{
	int i=0;
	for(;!i || n;d[++i]=n%BASE,n/=BASE);
	d[0]=i;
}

void preps(int *y,BG s[])
{
	int a,b;
	for(a=1;a<10;a++)
	{
		s[a][1]=0;
		for(b=1;b<=y[0];b++)
		{
			s[a][b]+=y[b]*a;
			s[a][b+1]=s[a][b]/10;
			s[a][b]%=10;
		}
		s[a][0]=s[a][b]?b:b-1;
	}
}

int Dgq(int *m,BG s[],int a)
{
	int q=10,b,*sq,n;
	while(--q>0)
	{
		sq=s[q];n=sq[0];
		if(m[0]<n+a)continue;
		else if(m[0]>n+a)break;
		for(b=0;m[m[0]-b]==sq[n-b] && b+1<n;b++);
		if(m[m[0]-b]>=sq[n-b])break;
	}
	return q;
}

void Dmm(int *m,int *sq,int a)
{
	int b;
	for(b=1;b<=sq[0];b++)
	{
		m[a+b]-=sq[b];
		if(m[a+b]<0){m[a+b]+=10;m[a+b+1]--;}
	}
	while(m[0]>1 && m[m[0]]==0)m[0]--;
}

void divbig(int *x,int *y,int *d)
{
	static BG s[10],m;
	int a,q,l;
	preps(y,s);
	for(a=0;a<=x[0];a++)
		m[a]=x[a];
	l=x[0]-y[0];
	for(a=l;a>=0;a--)
	{
		q=Dgq(m,s,a);
		d[a+1]=q;
		if(q!=0)Dmm(m,s[q],a);
	}
	for(d[0]=l+1;d[0]>1 && d[d[0]]==0;d[0]--);
}

void modbig(int *x,int *y)
{
	static BG s[10];
	int a,q,l;
	preps(y,s);
	l=x[0]-y[0];
	for(a=l;a>=0;a--)
	{
		q=Dgq(x,s,a);
		if(q!=0)Dmm(x,s[q],a);
	}
}

int modint(int *x,int y)
{
	int a,r=0;
	for(a=x[0];a>=1;a--)
		r=((I)r*10+x[a])%y;
	return r;
}

int egcd(int a,int b,int *x,int *y)
{
	int d,t;
	if(b==0){*x=1;*y=0;return a;}
	d=egcd(b,a%b,x,y);
	t=*y;*y=*x-(a/b)*(*y);*x=t;
	return d;
}

int modni(int a,int m)
{
	int x,y;
	egcd(a,m,&x,&y);
	return (x+m)%m;
}

void mulbig(int *x,int *y,int *z)
{
	int a,b,l;
	l=x[0]+y[0];
	for(a=0;a<=l;a++)z[a]=0;
	for(a=1;a<=x[0];a++)
	{
		for(b=1;b<=y[0];b++)
			z[a+b-1]+=x[a]*y[b];
	}
	for(b=1;b<l;b++)
		if(z[b]>=BASE){z[b+1]+=z[b]/BASE;z[b]%=BASE;}
		for(z[0]=l;z[0]>1 && z[z[0]]==0;z[0]--);
}

void cpybig(int *x,int *y)
{
	int i;
	for(i=0;i<=y[0];i++)x[i]=y[i];
}

int main()
{
	int n,i,j,m,a[1010],b[1010],t,mj,mj1;
	BG Mj,Mj1,y,MS,Mt;
	int T;
	scanf("%d",&T);
	while(T--)
	{
		MS[0]=1;MS[1]=0;
		M[0]=1;M[1]=1;
		scanf("%d",&n);
		for(i=1;i<=n;i++)
		{
			scanf("%d",&a[i]);
			inttobig(a[i],y);
			mulbig(M,y,Mt);
			cpybig(M,Mt);
		}
		for(i=1;i<=n;i++)
		{
			scanf("%d",&b[i]);
			inttobig(a[i],y);
			divbig(M,y,Mj);
			mj=modint(Mj,a[i]);
			mj1=modni(mj,a[i]);
			inttobig(mj1,y);
			mulbig(Mj,y,Mt);
			inttobig(b[i],y);
			mulbig(Mt,y,Mj);
			incbig(MS,Mj);
		}
		modbig(MS,M);
		prbig(MS);putchar('\n');
	}
	return 0;
}
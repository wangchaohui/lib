#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BASE 2

typedef long long I;
typedef int BG[4000];

void mulbig(int *x,int *y,int *z)
{
	int a,b,l;
	l=x[0]+y[0];
	for(a=0;a<=l;a++)z[a]=0;
	for(a=1;a<=x[0];a++)
	{
		if(x[a]==0)continue;
		for(b=1;b<=y[0];b++)
			z[a+b-1]+=y[b]; /* changed for BASE 2 */
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

void modbig(int *x,int *y) /* BASE 2 */
{
	int a,b,n=y[0],l;
	l=x[0]-y[0];
	for(a=l;a>=0;a--)
	{
		if(x[0]<y[0]+a)continue;
		else if(x[0]==n+a)
		{
			for(b=0;x[x[0]-b]==y[n-b] && b+1<n;b++);
			if(x[x[0]-b]<y[n-b])continue;
		}
		for(b=1;b<=n;b++)
		{
			x[a+b]-=y[b];
			if(x[a+b]<0){x[a+b]+=2;x[a+b+1]--;}
		}
		while(x[0]>1 && x[x[0]]==0)x[0]--;
	}
}

void strtobig(char *s,int *d)
{
	int a,l;
	l=strlen(s);
	for(a=0;a<l-1 && s[a]=='0';a++);
	d[0]=l-a;
	for(;a<l;a++)d[l-a]=s[a]-'0';
}

int half10(int *x)
{
	int i,c=0,s;
	for(i=x[0];i>=1;i--)
	{
		s=c*10+x[i];
		c=s%2;
		x[i]=s/2;
	}
	if(x[0]>1 && x[x[0]]==0)x[0]--;
	return c;
}

int half2(int *x)
{
	int i,c=0,s;
	for(i=x[0];i>=1;i--)
	{
		s=c*2+x[i];
		c=s%2;
		x[i]=s/2;
	}
	if(x[0]>1 && x[x[0]]==0)x[0]--;
	return c;
}

void gemination(int *x)
{
	int i;
	x[x[0]+1]=0;
	for(i=1;i<=x[0];i++)
		x[i]*=2;
	for(i=1;i<=x[0];i++)
		if(x[i]>=10){x[i+1]++;x[i]-=10;}
	if(x[x[0]+1])x[0]++;
}

void incbig(int *x) /* BASE 10 */
{
	int a;
	x[x[0]+1]=0;x[1]++;
	for(a=1;a<=x[0];a++)
		if(x[a]>=10){x[a]-=10;x[a+1]++;}
	if(x[x[0]+1])x[0]++;
}

void decimal(int *x,int *y)
{
	int i;
	y[0]=1;y[1]=0;
	for(i=x[0];i>=1;i--)
	{
		gemination(y);
		if(x[i])incbig(y);
	}
}

int isone(int *x)
{
	return x[0]==1 && x[1]==1;
}

int iszero(int *x)
{
	return x[0]==1 && x[1]==0;
}

void binary(int *x,int *y)
{
	int j=0;
	while(!iszero(x))
		y[++j]=half10(x);
	y[0]=j;
}

void decbig(int *x) /* BASE 2 */
{
	int a;
	x[1]--;
	for(a=1;a<x[0];a++)
		if(x[a]<0){x[a]+=BASE;x[a+1]--;}
	for(;x[0]>1 && x[x[0]]==0;x[0]--);
}

void MODULAR_EXPONENTIATION(int *a,int *b,int *n)
{
	BG d,z;
	int i;
	d[0]=1;d[1]=1;
	for(i=b[0];i>=1;i--)
	{
		mulbig(d,d,z);
		modbig(z,n,d);
		if(b[i]==1)
		{
			mulbig(d,a,z);
			modbig(z,n,d);
		}
	}
	cpybig(a,d);
}

void getrandom(int n,int *d)
{
	int i;
	d[0]=n;d[n]=1;
	for(i=n-1;i>=1;i--)
		d[i]=rand()%2;
}

int cmpbig(int *x,int *y)
{
	int a;
	if(x[0]!=y[0])return x[0]-y[0];
	for(a=x[0];a>=1;a--)
		if(x[a]!=y[a])return x[a]-y[a];
	return 0;
}

int WITNESS(int *x,int *n)
{
	int t,i;
	BG xi,n1,u;
	cpybig(n1,n);
	decbig(n1);
	cpybig(u,n1);
	for(t=0;u[1]%2==0;t++)half2(u);
	MODULAR_EXPONENTIATION(x,u,n);
	for(i=1;i<=t;i++)
	{
		mulbig(x,x,xi);
		modbig(xi,n,xi);
		if(isone(xi) && !isone(x) && cmpbig(x,n1)!=0)
			return 1;
		cpybig(x,xi);
	}
	if(!isone(xi))
		return 1;
	return 0;
}

int MILLER_RABIN(int *n,int s)
{
	int j,l;
	BG a;
	for(j=1;j<=s;j++)
	{
		do{
			l=rand()%(n[0])+1;
			getrandom(l,a);
		}while(cmpbig(a,n)>=0);
		if(WITNESS(a,n))
			return 0;
	}
	return 1;
}

void getprime(int n,int *d)
{
	do{
		getrandom(n,d);
		d[1]=1;
	}while(!MILLER_RABIN(d,50));
}

void prbig(int *x,FILE *fp)
{
	int a;
	fprintf(fp,"%d",x[x[0]]);
	for(a=x[0]-1;a>=1;a--)
		fprintf(fp,"%d",x[a]);
}

void divmodbig(int *x,int *y,int *d)
{
	int a,b,n=y[0],l;
	l=x[0]-y[0];
	for(a=l;a>=0;a--)
	{
		d[a+1]=0;
		if(x[0]<y[0]+a)continue;
		else if(x[0]==n+a)
		{
			for(b=0;x[x[0]-b]==y[n-b] && b+1<n;b++);
			if(x[x[0]-b]<y[n-b])continue;
		}
		d[a+1]=1;
		for(b=1;b<=n;b++)
		{
			x[a+b]-=y[b];
			if(x[a+b]<0){x[a+b]+=2;x[a+b+1]--;}
		}
		while(x[0]>1 && x[x[0]]==0)x[0]--;
	}
	for(d[0]=l+1;d[0]>1 && d[d[0]]==0;d[0]--);
	if(d[0]<1){d[0]=1;d[1]=0;}
}

void subbig(int *x,int *y,int *z)
{
	int a;
	for(a=1;a<=x[0];a++)z[a]=x[a];
	for(a=1;a<=y[0];a++)z[a]-=y[a];
	for(a=1;a<x[0];a++)
		if(z[a]<0){z[a]+=BASE;z[a+1]--;}
	for(z[0]=x[0];z[0]>1 && z[z[0]]==0;z[0]--);
}

void addbig(int *x,int *y,int *z)
{
	int a,l;
	l=x[0]>y[0]?x[0]+1:y[0]+1;
	for(a=x[0]+1;a<=l;a++)z[a]=0;
	for(a=1;a<=x[0];a++)z[a]=x[a];
	for(a=1;a<=y[0];a++)z[a]+=y[a];
	for(a=1;a<l;a++)
		if(z[a]>=BASE){z[a]-=BASE;z[a+1]++;}
	z[0]=z[l]?l:l-1;
}


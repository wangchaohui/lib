#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define DIGITS 4
#define BASE 10000

typedef long long I;
typedef int BG[60];

/******* ******* *******

only positive integer

******* ******* *******/

int isone(int *x)
{
	return x[0]==1 && x[1]==1;
}

int iszero(int *x)
{
	return x[0]==1 && x[1]==0;
}

int* toone(int *x)
{
	x[0]=1;x[1]=1;
	return x;
}

int* tozero(int *x)
{
	x[0]=1;x[1]=0;
	return x;
}

void prbig(int *x)
{
	int a;
	printf("%d",x[x[0]]);
	for(a=x[0]-1;a>=1;a--)
		printf("%0*d",DIGITS,x[a]);
}

int* mulint(int *x,int n)
{
	int a,c=0;
	I t;
	if(n==0)return tozero(x);
	for(a=1;a<=x[0];a++){t=(I)x[a]*n+c;c=t/BASE;x[a]=t%BASE;}
	for(;c;c/=BASE)x[++x[0]]=c%BASE;
	return x;
}

int* incint(int *x,int c)
{
	int a;
	I t;
	for(a=1;c && a<=x[0];a++){t=(I)x[a]+c;c=t/BASE;x[a]=t%BASE;}
	for(;c;c/=BASE)x[++x[0]]=c%BASE;
	return x;
}

int num[128];
char Ch[100];

void initbase()
{
	int i;
	for(i=0;i<=9;i++)num[Ch[i]='0'+i]=i;
	for(i=0;i<26;i++)num[Ch[i+10]='A'+i]=i+10;
	for(i=0;i<26;i++)num[Ch[i+36]='a'+i]=i+36;
}

int* nstrtobig(int n,char *s,int *d)
{
	int a,l;
	d[0]=1;d[1]=0;
	l=strlen(s);
	for(a=0;a<l;a++)
		incint(mulint(d,n),num[s[a]]);
	return d;
}

int divint(int *x,int d)
{
	int i,c=0;
	I t;
	for(i=x[0];i>=1;i--)
	{
		t=(I)c*BASE+x[i];
		c=t%d;
		x[i]=t/d;
		if(x[0]>1 && i==x[0] && x[i]==0)x[0]--;
	}
	return c;
}

char* nbigtostr(int n,int *d,char *s)
{
	int p=0;
	for(;!iszero(d);s[p++]=Ch[divint(d,n)]);
	if(p==0)s[p++]=Ch[0];
	reverse(s,s+p);s[p]='\0';
	return s;
}

void strtobig(char *s,int *d)
{
	int a,l;
	l=strlen(s);
	for(a=0;a<l-1 && s[a]=='0';a++);
	d[0]=l-a;
	for(;a<l;a++)d[l-a]=s[a]-'0';
}

void strtobig(char *s,int *d)
{
	int a,l,i;
	while(*s=='0')s++;
	if(*s=='\0'){d[0]=1;d[1]=0;return;}
	l=strlen(s);
	d[0]=(l-1)/DIGITS+1;
	for(i=1;i<d[0];i++)
		for(d[i]=0,a=l-DIGITS*i;a<l-DIGITS*(i-1);a++)
			d[i]=d[i]*10+s[a]-'0';
	for(d[i]=0,a=0;a<l-DIGITS*(i-1);a++)
		d[i]=d[i]*10+s[a]-'0';
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

void decbig(int *x,int *y)
{
	int a;
	for(a=1;a<=y[0];a++)x[a]-=y[a];
	for(a=1;a<x[0];a++)
		if(x[a]<0){x[a]+=BASE;x[a+1]--;}
	for(;x[0]>1 && x[x[0]]==0;x[0]--);
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

void subbig(int *x,int *y,int *z)
{
	int a;
	for(a=1;a<=x[0];a++)z[a]=x[a];
	for(a=1;a<=y[0];a++)z[a]-=y[a];
	for(a=1;a<x[0];a++)
		if(z[a]<0){z[a]+=BASE;z[a+1]--;}
	for(z[0]=x[0];z[0]>1 && z[z[0]]==0;z[0]--);
}

int cmpbig(int *x,int *y)
{
	int a;
	if(x[0]!=y[0])return x[0]-y[0];
	for(a=x[0];a>=1;a--)
		if(x[a]!=y[a])return x[a]-y[a];
	return 0;
}

void inttobig(int n,int *d)
{
	int i=0;
	for(;!i || n;d[++i]=n%BASE,n/=BASE);
	d[0]=i;
}

int bigtoint(int *d)
{
	int i,r=0;
	for(i=d[0];i>=1;r=r*BASE+d[i],i--);
	return r;
}

I bigtoll(int *d)
{
	int i;
	I r=0;
	for(i=d[0];i>=1;r=r*BASE+d[i],i--);
	return r;
}

int Palindromes_aba(int l)
{
	int p=l;
	for(l/=10;l;l/=10)p=p*10+l%10;
	return p;
}

int Palindromes_abba(int l)
{
	int p=l;
	for(;l;l/=10)p=p*10+l%10;
	return p;
}

void mulbig(int *x,int *y,int *z)
{
	int a,b,l;
	l=x[0]+y[0];
	for(a=0;a<=l;a++)z[a]=0;
	for(a=1;a<=x[0];a++)
	{
		for(b=1;b<=y[0];b++)
		{
			z[a+b-1]+=x[a]*y[b]; 
			if(z[a+b-1]>=BASE){z[a+b]+=z[a+b-1]/BASE;z[a+b-1]%=BASE;}
			/* carefully */
		}
	}
	//for(b=1;b<l;b++)
	//	if(z[b]>=BASE){z[b+1]+=z[b]/BASE;z[b]%=BASE;}
	for(z[0]=l;z[0]>1 && z[z[0]]==0;z[0]--);
}

void cpybig(int *x,int *y)
{
	int i;
	for(i=0;i<=y[0];i++)x[i]=y[i];
}

void leftbig(int *x,int l)
{
	int i;
	if(x[0]==1 && x[1]==0)return;
	for(i=x[0];i>=1;i--)x[i+l]=x[i];
	for(i=1;i<=l;i++)x[i]=0;
	x[0]+=l;
}

void sqrtbig(int *x,int *y)   /* BASE 10 */
{
	int i=x[0],j,a;
	static BG m,c,t,o;
	tozero(m);tozero(c);x[x[0]+1]=0;
	if(i%2==0){i--;}
	y[0]=i/2+1;o[0]=1;
	for(;i>=1;i-=2)
	{
		leftbig(m,1);leftbig(c,2);c[2]=x[i+1];c[1]=x[i];
		if(c[2] && c[0]<2)c[0]=2;if(c[1] && c[0]<1)c[0]=1;
		for(j=9;j>=1;j--)
		{
			m[1]=j;t[m[0]+1]=0;
			for(a=1;a<=m[0];a++)t[a]=m[a]*j;
			for(a=1;a<=m[0];a++)if(t[a]>=BASE){t[a+1]+=t[a]/BASE;t[a]%=BASE;}
			t[0]=t[m[0]+1]?m[0]+1:m[0];
			if(cmpbig(c,t)>=0)break;
		}
		if(j){o[1]=j;incbig(m,o);decbig(c,t);}
		else m[1]=0;
		y[i/2+1]=j;
	}
}

struct B{
	BG d;
	bool operator<(const B &b)const{
		return cmpbig(d,b.d)<0;
	}
	bool operator>(const B &b)const{
		return cmpbig(d,b.d)>0;
	}
};
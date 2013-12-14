#include <stdio.h>
#include <math.h>
#include <string.h>
#define pi 3.14159265358979
#define maxn 65536
#define maxlgn 16
#define DIGITS 4
#define BASE 10000
#define eps 1e-5

typedef struct {
	double x,y;
}complex;

int n,lgn,bit[maxlgn],rev[maxn];
complex t[maxn+1],A[maxn+1],B[maxn+1];
char buf[maxn/2*DIGITS+1];
int x[maxn+1],y[maxn+1],r[maxn+1];

complex ccadd(complex a,complex b)
{
	complex r;
	r.x=a.x+b.x;
	r.y=a.y+b.y;
	return r;
}

complex ccsub(complex a,complex b)
{
	complex r;
	r.x=a.x-b.x;
	r.y=a.y-b.y;
	return r;
}

complex ccmul(complex a,complex b)
{
	complex r;
	r.x=a.x*b.x-a.y*b.y;
	r.y=a.x*b.y+a.y*b.x;
	return r;
}

int BIT_REVERSED_INCREMENT(int a)
{
	int i=lgn-1;
	a^=bit[i];
	if(a&bit[i])return a;
	while(--i>=0 && a&bit[i])
		a&=~bit[i];
	if(i>=0)a|=bit[i];
	return a;
}

void init_rev()
{
	int i;
	for(i=0;i<lgn;i++)
		bit[i]=1<<i;
	rev[0]=0;
	for(i=1;i<n;i++)
		rev[i]=BIT_REVERSED_INCREMENT(rev[i-1]);
}

void BIT_REVERSE_COPY(complex* a,complex* A)
{
	int i;
	for(i=0;i<n;i++)
		A[rev[i]]=a[i];
}

void ITERATIVE_FFT(complex *a,complex *A,int mw)
{
	int s,i,j,m=1;
	complex wm,w,t,u;
	BIT_REVERSE_COPY(a,A);
	for(s=1;s<=lgn;s++)
	{
		m*=2;
		wm.x=cos(2*pi/m*mw);wm.y=sin(2*pi/m*mw);
		for(i=0;i<=n-1;i+=m)
		{
			w.x=1;w.y=0;
			for(j=0;j<=m/2-1;j++)
			{
				t=ccmul(w,A[i+j+m/2]);
				u=A[i+j];
				A[i+j]=ccadd(u,t);
				A[i+j+m/2]=ccsub(u,t);
				w=ccmul(w,wm);
			}
		}
	}
}

void convolution(int *a,int *b)
{
	int i;
	for(i=n,lgn=0;i/=2;lgn++);
	init_rev();
	for(i=0;i<n;i++){t[i].x=a[i];t[i].y=0;}
	ITERATIVE_FFT(t,A,1);
	for(i=0;i<n;i++)t[i].x=b[i];
	ITERATIVE_FFT(t,B,1);
	for(i=0;i<n;i++)A[i]=ccmul(A[i],B[i]);
	ITERATIVE_FFT(A,t,-1);
	for(i=0;i<n;i++)t[i].x=t[i].x/n;
}

void FFTMul(int *x,int *y,int *r)
{
	int i,p;
	for(i=x[0]+y[0]-1,lgn=1,n=2;i/=2;lgn++,n*=2);
	for(i=x[0]+1;i<=n+1;i++)x[i]=0;
	for(i=y[0]+1;i<=n+1;i++)y[i]=0;
	convolution(x+1,y+1);
	for(i=0;i<n;i++)
	{
		if(t[i].x+0.5>BASE)
		{
			p=(t[i].x+0.5)/BASE;
			t[i].x-=(double)p*BASE;
			t[i+1].x+=p;
		}
		r[i+1]=(int)(t[i].x+0.5);
	}
	for(i=n;i>0 && r[i]==0;i--);
	r[0]=i;
}

void strtobignum(char *s,int *d)
{
	int a,l,t,i;
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

void getstr(char *str,FILE *fp)
{
	int a;
	for(a=0;(str[a]=getc(fp))>'0' && str[a]<='9';a++);	
	str[a]='\0';
}

int main()
{
	int i;
	FILE *fp=NULL,*fp2=NULL;
	if(!(fp=fopen("input.txt","r")) || !(fp2=fopen("output.txt","w")))return -1;
	while(1)
	{
		getstr(buf,fp);
		if(feof(fp))break;
		strtobignum(buf,x);
		getstr(buf,fp);
		strtobignum(buf,y);
		FFTMul(x,y,r);
		fprintf(fp2,"%d",r[r[0]]);
		for(i=r[0]-1;i>=1;i--)fprintf(fp2,"%04d",r[i]);
		putc('\n',fp2);
	}
	return 0;
}
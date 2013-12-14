#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#define BASE 2
#define error(x) {puts("ERROR: "x);exit(-1);}

typedef long long I;
typedef int BG[4000];

typedef struct{
	FILE *fp;
	char buf;
	int p;
}BITFILE;

int C;

void mulbig(int *x,int *y,int *z)
{
	int a,b,l;
	l=x[0]+y[0];
	for(a=0;a<=l;a++)z[a]=0;
	for(a=1;a<=x[0];a++)
	{
		if(x[a]==0)continue;
		for(b=1;b<=y[0];b++)
			z[a+b-1]+=y[b];
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

void modbig(int *x,int *y,int *z)
{
	int a,b,n=y[0],l;
	for(a=0;a<=x[0];a++)
		z[a]=x[a];
	l=x[0]-y[0];
	for(a=l;a>=0;a--)
	{
		if(z[0]<y[0]+a)continue;
		else if(z[0]==n+a)
		{
			for(b=0;z[z[0]-b]==y[n-b] && b+1<n;b++);
			if(z[z[0]-b]<y[n-b])continue;
		}
		for(b=1;b<=n;b++)
		{
			z[a+b]-=y[b];
			if(z[a+b]<0){z[a+b]+=2;z[a+b+1]--;}
		}
		while(z[0]>1 && z[z[0]]==0)z[0]--;
	}
}

void modbig2(int *x,int *y)
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

void incbig(int *x)
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

void decbig(int *x)
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

void RSA_E(int *m,int *e,int *n)
{
	MODULAR_EXPONENTIATION(m,e,n);
}

int file_get_bit(BITFILE *bp)
{
	int g,r;
	if(bp->p==-1)
	{
		g=fread(&bp->buf,1,1,bp->fp);
		if(g!=1)return -1;
		bp->p=7;
	}
	r=(bp->buf>>bp->p)&1;
	bp->p--;
	return r;
}

int file_get_group(BITFILE *bp,int i,int *d)
{
	int g,f=1;
	d[0]=i;
	while(i)
	{
		g=file_get_bit(bp);
		if(g==-1)
			g=f,f=0;
		d[i]=g;
		i--;
	}
	for(;d[0]>1 && d[d[0]]==0;d[0]--);
	return f;
}

void file_write_bit(BITFILE *bp,int bit)
{
	bp->buf|=(bit<<bp->p);
	bp->p--;
	if(bp->p==-1)
	{
		fwrite(&bp->buf,1,1,bp->fp);
		bp->p=7;
		bp->buf=0;
	}
}

int file_write_group(BITFILE *bp,int i,int *d)
{
	int r=0,f=1;
	for(;i>d[0];i--)file_write_bit(bp,0);
	for(;i>0;i--)file_write_bit(bp,d[i]);
	return f;
}

void encrypt(int i,int *e,int *n,BITFILE bin,BITFILE bout)
{
	int g;
	BG d;
	do 
	{
		g=file_get_group(&bin,i,d);
		RSA_E(d,e,n);
		file_write_group(&bout,i+1,d);
	}while(g);
	while(bout.p!=7)file_write_bit(&bout,0);
}

void decrypt(int i,int *e,int *n,BITFILE bin,BITFILE bout)
{
	int j,k,g,f=0;
	BG m,d;
	while(g=file_get_group(&bin,i+1,d))
	{
		RSA_E(d,e,n);
		if(f)file_write_group(&bout,i,m);
		else f=1;
		cpybig(m,d);
	}
	if(i>=8)
	{
		for(k=1;m[k]==0;k++);
		for(j=1;j+k<=m[0];j++)m[j]=m[j+k];
		m[0]-=k;i-=k;
		file_write_group(&bout,i,m);
	}
	else file_write_group(&bout,i,m);
}

void ed(char *a2,char *a3,char *a4,int c)
{
	FILE *key,*in,*out;
	BITFILE bin,bout;
	char es[2000],ns[2000];
	BG d,e,n;
	int i;
	key=fopen(a2,"rb");in=fopen(a3,"rb");out=fopen(a4,"wb");
	if(key==NULL || in==NULL || out==NULL)error("File open failed");
	bin.fp=in;bin.p=-1;
	bout.fp=out;bout.p=7;bout.buf=0;
	if(fscanf(key,"%s %s",&es,&ns)!=2)error("Key error");
	strtobig(es,d);binary(d,e);
	strtobig(ns,d);binary(d,n);
	if(n[0]<6)error("Key length at least 6");
	if(n[0]>1024)error("Key length at most 1024");
	i=n[0]-1;
	if(c==0)
		encrypt(i,e,n,bin,bout);
	else
		decrypt(i,e,n,bin,bout);
	fclose(key);fclose(in);fclose(out);
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

void egcdbig(int *a,int *b,int *d,int *x,int *xf,int *y,int *yf)
{
	static BG z[2000];
	static BG t;
	int f,now;
	now=++C;
	if(iszero(b)){cpybig(d,a);x[0]=x[1]=y[0]=1;y[1]=0;*xf=*yf=0;return;}
	divmodbig(a,b,z[now]);
	egcdbig(b,a,d,x,xf,y,yf);
	mulbig(z[now],y,t);
	cpybig(z[now],y);
	f=*yf;
	if(*xf==0 && *yf==0)
	{
		if(cmpbig(x,t)>=0)
			subbig(x,t,y);
		else
		{
			subbig(t,x,y);
			*yf=1;
		}
	}
	else if(*xf==0 && *yf==1)
	{
		addbig(x,t,y);
		*yf=0;
	}
	else if(*xf==1 && *yf==0)
	{
		addbig(x,t,y);
		*yf=1;
	}
	else if(*xf==1 && *yf==1)
	{
		if(cmpbig(x,t)>0)
			subbig(x,t,y);
		else
		{
			subbig(t,x,y);
			*yf=0;
		}
	}
	cpybig(x,z[now]);
	*xf=f;
}

int geted(int *n,int *e,int *d)
{
	int l,df,yf;
	BG a,b,z,y;
	do{
		l=rand()%(n[0]-2)+2;
		getrandom(l,e);
		cpybig(a,e);
		cpybig(b,n);
		C=0;
		egcdbig(a,b,z,d,&df,y,&yf);
	}while(!isone(z));
	if(df==1)
	{
		cpybig(z,d);
		subbig(n,z,d);
	}
	if(cmpbig(e,d)==0)return 0;
	return 1;
}

void generate(char *l,char *fe,char *fd)
{
	FILE *key,*key2;
	int m,i;
	BG p,q,n,z;
	srand((unsigned int)time(NULL));
	if(sscanf(l,"%d",&m)!=1)error("Key length error");
	key=fopen(fe,"w");key2=fopen(fd,"w");
	if(key==NULL || key2==NULL)error("File open failed");
	if(m<6)error("Key length at least 6");
	if(m>1024)error("Key length at most 1024");
	i=rand()%(m/5)+m*2/5+rand()%2;
	do{
		getprime(i,p);
		do{
			do{
				getrandom(m+rand()%2-i,q);
				q[1]=1;
				mulbig(p,q,n);
			}while(n[0]!=m || cmpbig(p,q)==0);
		}while(!MILLER_RABIN(q,50));
		decbig(p);decbig(q);mulbig(p,q,z);
	}while(!geted(z,p,q));
	decimal(p,z);prbig(z,key);
	decimal(q,z);prbig(z,key2);
	fputc(' ',key);fputc(' ',key2);
	decimal(n,z);prbig(z,key);prbig(z,key2);
	fclose(key);fclose(key2);
}

void usage()
{
	puts("RSA encrypter");
	puts("usage:");
	puts("");
	puts("Encrypt");
	puts("-e  key_file  input_file  output_file");
	puts("");
	puts("Decrypt");
	puts("-d  key_file  input_file  output_file");
	puts("");
	puts("Generate Key Pair");
	puts("-g  key_length  output_key_file1  output_key_file2");
	puts("");
}

int main(int argc,char *argv[])
{
	if(argc==5)
		if(strcmp(argv[1],"-e")==0)
			ed(argv[2],argv[3],argv[4],0);
		else if(strcmp(argv[1],"-d")==0)
			ed(argv[2],argv[3],argv[4],1);
		else if(strcmp(argv[1],"-g")==0)
			generate(argv[2],argv[3],argv[4]);
		else usage();
	else usage();
	return 0;
}
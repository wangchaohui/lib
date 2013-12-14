#include<stdio.h>
#include<stdlib.h>
#include<memory.h>

#define MaxLen 100000
#define MDS 9
#define MDSC "%09lld"/*not in VC6*/
#define MDB 1000000000
#define MLX (long long)/*not in VC6*/


struct number{
	int n;
	MLX *num;
};

struct number Multiply(struct number x,struct number y);
void Mn1(struct number x,struct number y,struct number *r);
void Mnl(struct number x,struct number *xl,struct number *xr,struct number *xlr,int l);

int main()
{
	int a,b,c,neg,n1[MaxLen],n2[MaxLen];
	struct number l,r,m;
	char buf[MaxLen+3];
	freopen("/home/wangchaohui/Program/pc1_in.txt","r",stdin);
	l.num=(MLX*)malloc(25001*sizeof(MLX));
	r.num=(MLX*)malloc(25001*sizeof(MLX));
	while(fgets(buf,MaxLen+3,stdin))
	{	
		neg=0;c=0;
		if(buf[0]=='-')
		{
			neg=1;
			c++;
		}
		b=0;
		while(buf[c+b]>='0' && buf[c+b]<='9')
		{
			n1[b]=buf[c+b]-'0';
			b++;
		}
		if(neg==1 && b==1 && n1[0]==1)break;
		c=0;l.n=b;
		fgets(buf,MaxLen+3,stdin);
		if(buf[c]=='-')
		{
			neg=1-neg;
			c++;
		}
		b=0;
		while(buf[c+b]>='0' && buf[c+b]<='9')
		{
			n2[b]=buf[c+b]-'0';
			b++;
		}
		c+=b;r.n=b;
		/*memset(l.num,0,((l.n-1)/MDS+1)*sizeof(int));*/
		/*memset(r.num,0,((r.n-1)/MDS+1)*sizeof(int));*/
		b=0;
		while(b<l.n-1 && n1[b]==0)b++;	
		for(a=0;a<(l.n-b-1)/MDS;a++)
			for(c=MDS-1,l.num[a]=n1[l.n-1-a*MDS-c--];c>=0;l.num[a]=l.num[a]*10+n1[l.n-1-a*MDS-c--]);
		for(c=l.n-b-1,l.num[a]=n1[l.n-1-c--];c>=a*MDS;l.num[a]=l.num[a]*10+n1[l.n-1-c--]);
		l.n=(l.n-b-1)/MDS+1;
		b=0;
		while(b<r.n-1 && n2[b]==0)b++;	
		for(a=0;a<(r.n-b-1)/MDS;a++)
			for(c=MDS-1,r.num[a]=n2[r.n-1-a*MDS-c--];c>=0;r.num[a]=r.num[a]*10+n2[r.n-1-a*MDS-c--]);
		for(c=r.n-b-1,r.num[a]=n2[r.n-1-c--];c>=a*MDS;r.num[a]=r.num[a]*10+n2[r.n-1-c--]);
		r.n=(r.n-b-1)/MDS+1;
		m=Multiply(l,r);
		if(neg==1)printf("-");
		printf("%lld",m.num[m.n-1]);
		for(a=m.n-2;a>=0;a--)
			printf(MDSC,m.num[a]);
		printf("\n");
		free(m.num);
	}
	free(l.num);free(r.num);
	return 0;
}

struct number Multiply(struct number x,struct number y)
{
	struct number r,xl,xr,xlr,yl,yr,ylr,p1,p2,p3;
	int a,l;
	if(x.n<=0 || y.n<=0)
	{
		r.n=0;
		r.num=NULL;
		return r;
	}
	if(x.n==1){Mn1(x,y,&r);return r;}
	if(y.n==1){Mn1(y,x,&r);return r;}
	l=(x.n>y.n)?x.n/2:y.n/2;
	Mnl(x,&xl,&xr,&xlr,l);
	Mnl(y,&yl,&yr,&ylr,l);
	if(x.n>l && y.n>l)
		p1=Multiply(xl,yl);
	else
	{
		p1.n=1;
		p1.num=(MLX*)malloc(sizeof(MLX));
		p1.num[0]=0;
	}
	p2=Multiply(xlr,ylr);
	p3=Multiply(xr,yr);
	if(x.n>l){free(xr.num);free(xl.num);free(xlr.num);}
	if(y.n>l){free(yr.num);free(yl.num);free(ylr.num);}
	r.num=(MLX*)malloc((x.n+y.n)*sizeof(MLX));
	memset(r.num,0,(x.n+y.n)*sizeof(MLX));
	r.n=1;
	for(a=0;a<p3.n;a++)
	{
		r.num[a]+=p3.num[a];
		r.num[l+a]-=p3.num[a];
	}
	for(a=0;a<p2.n;a++)r.num[l+a]+=p2.num[a];
	for(a=0;a<p1.n;a++)
	{
		r.num[l+a]-=p1.num[a];
		r.num[2*l+a]+=p1.num[a];
	}
	for(a=0;a<x.n+y.n-1;a++)
	{
		r.num[a+1]+=r.num[a]/MDB;
		r.num[a]%=MDB;
		if(r.num[a]<0)
		{
			r.num[a]+=MDB;
			r.num[a+1]--;
		}
	}
	for(r.n=x.n+y.n;r.n>1 && r.num[r.n-1]==0;r.n--);
	free(p1.num);free(p2.num);free(p3.num);
	return r;
}

void Mn1(struct number x,struct number y,struct number *r)
{
	int a,t;
	t=x.num[0];
	if(t==0)
	{
		r->n=1;
		r->num=(MLX*)malloc(sizeof(MLX));
		r->num[0]=0;
	}
	else
	{
		r->num=(MLX*)malloc((y.n+1)*sizeof(MLX));
		memset(r->num,0,(y.n+1)*sizeof(MLX));
		for(a=0;a<y.n;a++)
		{
			r->num[a]+=y.num[a]*t;
			r->num[a+1]=r->num[a]/MDB;
			r->num[a]%=MDB;
		}
		for(t=y.n+1;t>1 && r->num[t-1]==0;t--);
		r->n=t;
	}
}

void Mnl(struct number x,struct number *xl,struct number *xr,struct number *xlr,int l)
{
	int a,t;
	if(x.n>l)
	{
		xr->num=(MLX*)malloc(l*sizeof(MLX));
		xl->num=(MLX*)malloc((x.n-l)*sizeof(MLX));
		xlr->num=(MLX*)malloc((l+2)*sizeof(MLX));	
		memset(xlr->num,0,(l+2)*sizeof(MLX));
		for(a=0;a<l;a++)
		{
			xr->num[a]=x.num[a];
			xlr->num[a]=xr->num[a];
		}
		for(t=l;t>1 && xr->num[t-1]==0;t--);
		xr->n=t;
		for(a=0;a<x.n-l;a++)
		{
			xl->num[a]=x.num[l+a];
			xlr->num[a]+=xl->num[a];
		}
		for(t=x.n-l;t>1 && xl->num[t-1]==0;t--);
		xl->n=t;
		for(a=0;a<l+1;a++)
			if(xlr->num[a]>=MDB)
			{
				xlr->num[a+1]+=xlr->num[a]/MDB;
				xlr->num[a]%=MDB;
			}
		for(t=l+2;t>1 && xlr->num[t-1]==0;t--);
		xlr->n=t;
	}
	else
	{
		*xr=x;
		*xlr=x;
	}
}


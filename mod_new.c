

typedef long long I;
typedef int BG[60];

/******* ******* *******

only positive integer

******* ******* *******/

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

void divmodbig(int *x,int *y,int *d)
{
	BG s[10],m,r;
	int a,q,l;
	preps(y,s);
	l=x[0]-y[0];
	for(a=l;a>=0;a--)
	{
		q=Dgq(x,s,a);
		d[a+1]=q;
		if(q!=0)Dmm(x,s[q],a);
	}
	for(d[0]=l+1;d[0]>1 && d[d[0]]==0;d[0]--);
}

void divbig(int *x,int *y,int *d)
{
	BG s[10],m;
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
	BG s[10];
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


void biggcd(int *x,int *y)
{
    int *z;
    while(1)
    {
        if(y[0]==1 && y[1]==0)return;
        modbig(x,y);
		z=y;y=x;x=z;
    }
}
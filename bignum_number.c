

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
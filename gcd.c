

int gcd(int a,int b)
{
	return b?gcd(b,a%b):a;
}

int egcd(int a,int b,int *x,int *y)
{
	int d,t;
	if(b==0){*x=1;*y=0;return a;}
	d=egcd(b,a%b,x,y);
	t=*y;*y=*x-(a/b)*(*y);*x=t;
	return d;
}

/*ax=b (mod n)*/
void modequ(int a,int b,int n)
{
	int d,x,y,i;
	d=egcd(a,n,&x,&y);
	if(b%d==0)
	{
		y=n/d;
		x=(x*(b/d))%y;
		if(x<0)x+=y;
		for(i=0;i<d;i++)
		{
			if(i!=d-1)
				printf("%d ",x);
			else
				printf("%d\n",x);
			x+=y;
		}
	}
	else
		printf("No Answer.\n");
}

int modni(int a,int m)
{
	int x,y;
	egcd(a,m,&x,&y);
	return (x+m)%m;
}

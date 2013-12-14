
typedef __int64 I;


/******* ******* *******
	k=2
******* ******* *******/
I Josephus(I n)
{
	I t;
	for(t=1;n>t;n-=t,t*=2);
	return n*2-1;
}

int Josephus(int n,int k)
{
	int s=0,t,i;
	if(k==1)return n;
	for(i=2;i<=n;)
	{
		if(s+k<i)
		{
			t=(i-s)/k;
			if(i+t>n)t=n+1-i;
			s+=t*k;
			i+=t;
		}	
		else
		{
			s=(s+k)%i;
			i++;
		}
	}
	return s+1;
}

int Josephus(int n,int k)
{
	int s=0,t,i;
	if(k==1)return n;
	for(i=2;i<=n;i++)
		s=(s+k)%i;
	return s+1;
}
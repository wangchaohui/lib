namespace coding{

	const int N = 9;

	int encoding(int *d)
	{
		int r=0;
		int e[N];
		memcpy(e,d,sizeof(e));
		for(int i=0;i<N;i++)
		{
			r=r*(N-i)+e[i];
			for(int j=i+1;j<N;j++)
			{
				if(e[j]==N-1-i)
				{
					e[j]=e[i];
					break;
				}
			}
		}
		return r;
	}

	void decoding(int *d,int x)
	{
		for(int i=N-1;i>=0;i--)
		{
			d[i]=x%(N-i);
			x/=(N-i);
			for(int j=i+1;j<N;j++)
			{
				if(d[j]==d[i])
				{
					d[j]=N-1-i;
					break;
				}
			}
		}
	}
}
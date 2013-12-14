
void Catalan(int n,int *d)
{
	int i,j;
	d[0]=1;
	for(i=1;i<=n;i++)
	{
		d[i]=0;
		for(j=0;j<=i-1;j++)
			d[i]+=d[j]*d[i-1-j];
	}
}
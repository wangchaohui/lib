

int C[N+4][N+4];

void init_C()
{
	for(int i=0;i<N+4;i++)
	{
		C[i][0]=C[i][i]=1;
		for(int j=1;j<i;j++)C[i][j]=C[i-1][j-1]+C[i-1][j];
	}
}
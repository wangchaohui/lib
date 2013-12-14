
#define swap(a,b){t=a;a=b;b=t;}

int next_permutation(int n,int *p)
{
	int i,j,t;
	for(i=n-2;i>=0 && p[i]>=p[i+1];i--);
	if(i<0)return 0;
	for(j=n-1;p[j]<=p[i];j--);
	swap(p[i],p[j]);
	while(++i<--n)
		swap(p[i],p[n]);
	return 1;
}

#include <string.h>

char s[1000005];
int P[1000005];

void Prefix(char *B)
{
	int j=0,m=strlen(B);
	P[0]=P[1]=0;
	for(int i=2;i<=m;i++)
	{
		while(j>0 && B[j]!=B[i-1])j=P[j];
		if(B[j]==B[i-1])j++;
		P[i]=j;
	}
}

void KMP(char *A,char *B)
{
	int n=strlen(A),m=strlen(B),j=0;
	for(int i=0;i<n;i++)
	{
		while(j>0 && B[j]!=A[i])j=P[j];
		if(B[j]==A[i])j++;
		if(j==m)
		{
			j=P[j];
		}
	}
}
#include <stdio.h>
#include <string.h>

#define N 1000
#define test(x,y) (x[y/8]>>y%8&1)
#define set(x,y) (x[y/8]|=1<<y%8)

typedef char Collection[N/8];

Collection A,B,C,D,E;

char s[1000];

void printCollection(Collection x)
{
	int i,f=0;
	printf("{");
	for(i=0;i<N/8;i++)
	{
		if(test(x,i))
		{
			if(f)putchar(',');else f=1;
			printf("%d",i);
		}
	}
	printf("}"); 
}

int main()
{
	int a,b,i,t;
	scanf("%d %d",&a,&b);gets(s);
	memset(A,0,sizeof(A));
	for(i=0;i<a;i++)
	{
		scanf("%d",&t);
		set(A,t);
	}
	gets(s);
	memset(B,0,sizeof(B));
	for(i=0;i<b;i++)
	{
		scanf("%d",&t);
		set(B,t);
	}
	gets(s);
	for(i=0;i<N/8;i++)
	{
		C[i]=A[i]&B[i];
		D[i]=A[i]|B[i];
		E[i]=A[i]&~B[i];
	}
	printCollection(C);putchar('\n');
	printCollection(D);putchar('\n');
	printCollection(E);putchar('\n');
	return 0;
}

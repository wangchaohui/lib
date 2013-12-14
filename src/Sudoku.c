#include <stdio.h>
#include <ctype.h>
#define N 3

#define AS(x) while(!(x))puts(#x);

typedef struct{
	char d[N*N][N*N],p[N*N][N*N][N*N+1],r[N*N][N*N+1],c[N*N][N*N+1],s[N*N][N*N+1];
}Sudoku;

Sudoku ANS;

void Sudoku_init(Sudoku *a)
{
	int i,j,k;
	for(i=0;i<N*N;i++)
		for(j=0;j<N*N;j++)
		{
			a->d[i][j]=0;
			a->p[i][j][0]=N*N;
		}
	for(i=0;i<N*N;i++)
		for(k=1;k<=N*N;k++)
		{			
			for(j=0;j<N*N;j++)
				a->p[i][j][k]=1;
			a->r[i][k]=N*N;
			a->c[i][k]=N*N;
			a->s[i][k]=N*N;
		}
}

int Sudoku_imp(Sudoku *a,int x,int y,int k)
{
	int i;
	if(a->p[x][y][k]==0)return 0;
	a->p[x][y][k]=0;
	a->p[x][y][0]--;
	if(a->d[x][y]==0 && a->p[x][y][0]==0)return -1;
	//a->r[x][k]--;
	if(--a->r[x][k]==0)return -1;
	if(--a->c[y][k]==0)return -1;
	if(--a->s[N*(x/N)+y/N][k]==0)return -1;
	return 0;
}

int Sudoku_add(Sudoku *a,int x,int y,int k)
{
	int i;
	if(a->d[x][y]!=0)return -1;
	AS(a->p[x][y][k]);
	a->d[x][y]=k;
	a->r[x][k]+=2;
	a->c[y][k]+=2;
	a->s[N*(x/N)+y/N][k]+=2;
	for(i=0;i<N*N;i++)
	{
		if(Sudoku_imp(a,i,y,k)==-1)return -1;
		if(Sudoku_imp(a,x,i,k)==-1)return -1;
	}
	for(i=0;i<N*N;i++)
		if(Sudoku_imp(a,N*(x/N)+i/N,N*(y/N)+i%N,k)==-1)return -1;
	for(i=1;i<=N*N;i++)
		if(Sudoku_imp(a,x,y,i)==-1)return -1;
	return 0;
}

int Sudoku_try(Sudoku *a)
{
	int i,j,k,f=0;
	for(k=1;k<=N*N;k++)
		for(i=0;i<N*N;i++)
		{
			if(a->r[i][k]==1)
				for(j=0;j<N*N;j++)
					if(a->p[i][j][k])
					{
						if(Sudoku_add(a,i,j,k)==-1)return -1;
						f=1;
						break;
					}
			if(a->c[i][k]==1)
				for(j=0;j<N*N;j++)
					if(a->p[j][i][k])
					{
						if(Sudoku_add(a,j,i,k)==-1)return -1;
						f=1;
						break;
					}
			if(a->s[i][k]==1)
				for(j=0;j<N*N;j++)
					if(a->p[N*(i/N)+j/N][N*(i%N)+j%N][k])
					{
						if(Sudoku_add(a,N*(i/N)+j/N,N*(i%N)+j%N,k)==-1)return -1;
						f=1;
						break;
					}
		}
	for(i=0;i<N*N;i++) 
		for(j=0;j<N*N;j++)
			if(a->p[i][j][0]==1)
				for(k=1;k<=N*N;k++)
					if(a->p[i][j][k])
					{
						if(Sudoku_add(a,i,j,k)==-1)return -1;
						f=1;
						break;
					}
	return f;
}

int Sudoku_serch(Sudoku *a)
{
	int i,j,k,g;
	Sudoku c;
	for(i=0;i<N*N;i++)
		for(j=0;j<N*N;j++)
			if(a->d[i][j]==0)
			{
				for(k=1;k<=N*N;k++)
					if(a->p[i][j][k])
					{
						c=*a;
						if(Sudoku_add(&c,i,j,k)==-1)continue;
						while((g=Sudoku_try(&c))==1);
						if(g==-1)continue;
						if(Sudoku_serch(&c))
							return 1;
					}
					return 0;
			}
	ANS=*a;
	return 1;
}

int main()
{
	int pp,i,j,t;
	Sudoku a;
	freopen("data.in","r",stdin);
	freopen("data.txt","w",stdout);
	scanf("%d",&pp);
	while(pp--)
	{
		Sudoku_init(&a);
		for(i=0;i<N*N;i++)
			for(j=0;j<N*N;j++)
			{
				while(!isdigit(t=getchar()));
				t-='0';
				if(t)Sudoku_add(&a,i,j,t);
			}
			while((t=Sudoku_try(&a))==1);
			AS(t!=-1);
			Sudoku_serch(&a);
			for(i=0;i<N*N;i++)
			{
				for(j=0;j<N*N;j++)
					putchar('0'+ANS.d[i][j]);
				putchar('\n');
			}
	}
	return 0;
}
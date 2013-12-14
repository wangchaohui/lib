#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <ctype.h>
using namespace std;
#define CS 28

int tot;

struct Trie{
	int next[CS];
	char f;
	int n;
};

Trie Tr[400000];

void init(int x)
{
	int i;
	for(i=0;i<CS;i++)Tr[x].next[i]=-1;
	Tr[x].f=0;Tr[x].n=0;
}

void trin(int n,char *x,int s)
{
	int i,id=0;
	for(i=0;i<n && ~Tr[id].next[x[i]];i++)id=Tr[id].next[x[i]];
	for(;i<n;i++){Tr[id].next[x[i]]=++tot;init(tot);id=tot;}
	Tr[id].f=1;Tr[id].n=s;
}

int trfi(int n,char *x)
{
	int i,id=0;
	for(i=0;i<n && ~Tr[id].next[x[i]];i++)id=Tr[id].next[x[i]];
	if(i!=n)return -1;
	return id;
}

void ptr(int x,int c,int l)
{
	int i;
	printf("%*c%c",l,' ',c);if(Tr[x].f)putchar('*');putchar('\n');
	for(i=0;i<CS;i++)if(~Tr[x].next[i])
	{
		ptr(Tr[x].next[i],i+'a',l+2);
	}
}

char s[200],t[200];

int main()
{
	int n,m,i,j,g,ans=0;
	tot=0;
	init(0);
	while(scanf("%s",s)==1)
	{
		n=strlen(s);
		//printf("%d %s\n",n,s);
		for(i=0;i<n;i++)
		{
			if(islower(s[i]))s[i]-='a';
			else if(s[i]=='\'')s[i]=26;
			else if(s[i]=='-')s[i]=27;
			else while(1)putchar(s[i]);
			t[i]=s[i];
		}
		m=0;
		for(i=0;i<n;i++)
		{
			for(j=0;j<s[i];j++)
			{
				t[i]=j;
				g=trfi(n,t);
				if(~g && Tr[g].n>m)m=Tr[g].n;
			}
			t[i]=s[i];
		}
		for(i=n-1;i>=0;i--)
		{
			swap(t[i],t[n-1]);
			g=trfi(n-1,t);
			if(~g && Tr[g].n>m)m=Tr[g].n;
		}
		for(i=0;i<n;i++)t[i]=s[i];
		for(i=n;i>=0;i--)
		{
			t[i+1]=s[i];
			for(j=0;j<CS;j++)
			{
				t[i]=j;
				g=trfi(n+1,t);
				if(~g && Tr[g].n>m)m=Tr[g].n;
			}
		}
		trin(n,s,m+1);
		if(m+1>ans)ans=m+1;
		//printf(":%d\n",tot);
	}
	//ptr(0,'R',1);
	printf("%d\n",ans);
	return 0;
}

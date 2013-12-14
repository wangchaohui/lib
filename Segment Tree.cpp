#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int MAXN =1<<17;

inline int L(int i) { return i*2; }
inline int R(int i) { return L(i)+1; }

struct SegTree{
	int m;
	int x[MAXN<<1],y[MAXN<<1];

	void init(int n)
	{
		m=1;
		while(m<n)m<<=1;
		fill(x,x+m+m,0);
		fill(y,y+m+m,0);
	}

	void build()
	{
		int i;
		for (i=m-1;i>0;i--)
			//	x[i]=max(x[L(i)],x[R(i)])+y[i];
			x[i]=x[L(i)]+x[R(i)];
	}

	int addd(int p,int pl,int pr,int l,int r)
	{
		int pm=(pl+pr)/2;
		if (pl==l && pr==r)
		{
			return x[p];
		}
		else
		{
			if(r<=pm)
				return addd(L(p),pl,pm,l,r);
			else if(pm<=l)
				return addd(R(p),pm,pr,l,r);
			else
			{
				return addd(L(p),pl,pm,l,pm)+addd(R(p),pm,pr,pm,r);
			}
			//	x[p]=max(x[L(p)],x[R(p)])+y[p];
			//	x[p]=x[L(p)]+x[R(p)];
		}
	}

	int add(int l,int r)
	{
		return addd(1,0,m,l,r);
	}

	void update(int l,int d)
	{
		while(l>=1)
		{
			x[l]+=d;
			l/=2;
		}
	}
};

SegTree st;

char s[100];

int main()
{
	int T,C=0,n,i;
	int a,b;
	scanf("%d",&T);
	while(++C<=T)
	{
		printf("Case %d:\n",C);
		scanf("%d",&n);
		st.init(n);
		for(i=0;i<n;i++)scanf("%d",&st.x[st.m+i]);
		st.build();
		while(1)
		{
			scanf("%s",s);
			if(strcmp(s,"Query")==0)
			{
				scanf("%d %d",&a,&b);
				printf("%d\n",st.add(a-1,b));
			}
			if(strcmp(s,"Add")==0)
			{
				scanf("%d %d",&a,&b);
				st.update(st.m+a-1,b);
			}
			if(strcmp(s,"Sub")==0)
			{
				scanf("%d %d",&a,&b);
				st.update(st.m+a-1,-b);
			}
			if(strcmp(s,"End")==0)
			{
				break;
			}
		}
	}
	return 0;
}


#include <cmath>
#include <vector>
using namespace std;

typedef long long I;

int is_sqr(int n)
{
	int r=(int)sqrt(n+0.5);
	return r*r==n;
}

void factorization(int n)
{
	int i;
	p[0]=0;
	for(i=2;i*i<=n;i++)
		if(n%i==0)
		{
			p[++p[0]]=i;
			while(n%i==0)n/=i;
		}
	if(n>1)p[++p[0]]=n;
}

typedef pair<unsigned int,unsigned int> PUU;

vector<PUU> factorization2(unsigned int n)
{
	vector<PUU> v;
	for(unsigned int i=2;i*i<=n;i++)
	{
		if(n%i==0)
		{
			unsigned int c=0;
			while(n%i==0)n/=i,c++;
			v.push_back(make_pair(i,c));
		}
	}
	if(n>1)v.push_back(make_pair(n,1));
	return v;
}

void halfdivisor(int n)
{
	int i;
	p[0]=0;
	for(i=1;i*i<=n;i++)if(n%i==0)p[++p[0]]=i;
}

void divisor(int n)
{
	int i;
	halfdivisor(n);
	i=p[p[0]];
	if(i*i==n)i=p[0]-1;
	else i=p[0];
	for(;i>=1;i--)p[++p[0]]=n/p[i];
}

int sumofdivisor(int n)
{
	int i,p,s=1;
	for(i=2;i*i<=n;i++)
		if(n%i==0)
		{
			for(p=i;n%i==0;n/=i,p*=i);
			s*=(p-1)/(i-1);
		}
	if(n>1)s*=(n+1);
	return s;
}

int numofdivisor(int n)
{
	int i,p2,s=1;
	for(i=2;i*i<=n;i++)
		if(n%i==0)
		{
			for(p2=0;n%i==0;n/=i,p2++);
			s*=p2+1;
		}
	if(n>1)s*=2;
	return s;
}

int Phi(int n)
{
	int i;
	factorization(n);
	for(i=1;i<=p[0];i++)
		n=n/p[i]*(p[i]-1);
	return n;
}

/*the factor number of d in the factorial of n*/
int numoff(int n,int d)
{
	int r;
	for(r=0;n;n/=d,r+=n);
	return r;
}

int MODULAR_EXPONENTIATION(int a,int b,int n)
{
	int bb[64];
	I d=1;
	int i;
	for(i=0,bb[0]=b%2,b/=2;b;bb[++i]=b%2,b/=2);
	do{
		d=(d*d)%n;
		if(bb[i]==1)
			d=(d*a)%n;
	}while(--i>=0);
	return (int)d;
}

void mo(int a,int m,int &l,int &r) //a^x=a^(l+x%r) (mod m) (x>=l)
{
	I x=1,y=1;
	l=0;
	do
	{
		x=x*a%m;
		y=y*a%m*a%m;
		l++;
	}while(x!=y);
	r=0;
	do 
	{
		y=y*a%m;
		r++;
	}while(x!=y);
}

int gcd(int a,int b)
{
	if(b==0)return a;
	return gcd(b,a%b);
}

int Combination(int n,int k)
{
	int i,j,a,b,s=1;
	if(n-k<k)k=n-k;
	for(i=1;i<=k;i++)
	{
		a=n+1-i;
		b=gcd(i,a);
		a/=b;
		s=s/(i/b)*a;
	}
	return s;
}

int prime_init(int n)
{
	int i,j,r=1;
	memset(po,-1,sizeof(po));
	po[2]=0;prime[0]=2;
	for(i=3;i<=n;i+=2)
	{
		if(po[i])
		{
			po[i]=r;prime[r]=i;r++;
			for(j=i*i;j<=n;j+=i)po[j]=0;
		}
	}
	return r;
}


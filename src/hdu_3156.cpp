#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cassert>
#include <stdio.h>
#include <memory.h>
#define maxn 256
#define maxm 16
#define maxt 4200
using namespace std;

int S[maxm+1],O[maxn],L[maxt],R[maxt],U[maxt],D[maxt],C[maxt],W[maxt];
int mat[maxn+1][maxm+1];
int ANS;

void build(int n,int m)
{
    int i,j,tot=m,first;
    R[0]=1;L[0]=m;
    for(i=1;i<=m;i++)
    {
        L[i]=i-1;R[i]=(i+1)%(m+1);
        U[i]=D[i]=C[i]=i;S[i]=0;
    }
    for(i=1;i<=n;i++)
    {
        first=0;
        for(j=1;j<=m;j++)if(mat[i][j])
        {
            tot++;
            U[tot]=U[j];D[tot]=j;
            D[U[j]]=tot;U[j]=tot;
            if(first==0)first=L[tot]=R[tot]=tot;
            else
            {
                L[tot]=L[first];R[tot]=first;
                R[L[first]]=tot;L[first]=tot;
            }
            W[tot]=i;C[tot]=j;S[j]++;
        }
    }
}

void remove(int c)
{
    int i;
    //remove column c;
    for(i=D[c];i!=c;i=D[i])
    {
        L[R[i]]=L[i];
        R[L[i]]=R[i];
        S[C[i]]--;
    }
}

void resume(int c)
{
    int i,j;
    for(i=U[c];i!=c;i=U[i])
    {
        S[C[i]]++;
        L[R[i]]=i;
        R[L[i]]=i;
    }
}

int CC;

int H()
{
    bool hash[17]={0};
    int i,j,c,r=0;
    for(c=R[0];c!=0;c=R[c])
        if(!hash[c])
        {
            r++;
            hash[c]=true;
            for(i=D[c];i!=c;i=D[i])
                for(j=R[i];j!=i;j=R[j])
                    hash[C[j]]=true;
        }
    return r;
}

int dfs(int k)
{
    int i,j,t,m=maxn,mn;
    if(k+H()>min(ANS,CC))return 0;
    if(R[0]==0)
    {
        //One of the answers has been found.
        ANS=k;
        return 1;
    }
    for(t=R[0];t!=0;t=R[t])
        if(S[t]<m){m=S[t];mn=t;}
    for(i=D[mn];i!=mn;i=D[i])
    {
        remove(i);
        for(j=R[i];j!=i;j=R[j])
            remove(j);
        dfs(k+1);
        for(j=L[i];j!=i;j=L[j])
            resume(j);
        resume(i);
    }
    return 0;
}

double eps = 1e-8;

struct PT { 
    double x, y; 
    PT() {}
    PT(double x, double y) : x(x), y(y) {}
    PT(const PT &p) : x(p.x), y(p.y)    {}
    PT operator + (const PT &p)  const { return PT(x+p.x, y+p.y); }
    PT operator - (const PT &p)  const { return PT(x-p.x, y-p.y); }
    PT operator * (double c)     const { return PT(x*c,   y*c  ); }
    PT operator / (double c)     const { return PT(x/c,   y/c  ); }
};

double dot(PT p, PT q)     { return p.x*q.x+p.y*q.y; }
double dist2(PT p, PT q)   { return dot(p-q,p-q); }
double cross(PT p, PT q)   { return p.x*q.y-p.y*q.x; }
ostream &operator<<(ostream &os, const PT &p) {
    return os << "(" << p.x << "," << p.y << ")"; 
}
istream &operator>>(istream &is, PT &p) {
    return is >> p.x >> p.y ; 
}

// rotate a point CCW or CW around the origin
PT RotateCCW90(PT p)   { return PT(-p.y,p.x); }
PT RotateCW90(PT p)    { return PT(p.y,-p.x); }

// compute intersection of circle centered at a with radius r
// with circle centered at b with radius R
vector<PT> CircleCircleIntersection(PT a, PT b, double r) {
    vector<PT> ret;
    
    return ret;
}

int n;
PT p[20];
int can[20];

int check(double r)
{
    int i,j,k,a,b,tot=0,cnt;
    PT g[2];
    //memset(mat,0,sizeof(mat));
    for(b=1;b<=n;b++)can[b]=0;
    for(i=0;i<n;i++)
        for(j=i+1;j<n;j++)
        {
            double d = sqrt(dist2(p[i],p[j]));
            if (d > r+r ) continue;
            double x = (d)/(2);
            double y = sqrt(r*r-x*x);
            PT v = (p[j]-p[i])/d;
            cnt=0;
            g[cnt++]=p[i]+v*x + RotateCCW90(v)*y;
            if (y > 0)
                g[cnt++]=p[i]+v*x - RotateCCW90(v)*y;
            for(a=0;a<cnt;a++)
            {
                tot++;
                for(b=0;b<n;b++)
                    if(dist2(g[a],p[b])<r*r+eps)
                        mat[tot][b+1]=1,can[b+1]=1;
                    else
                        mat[tot][b+1]=0;
                for(k=1;k<tot;k++)
                {
                    for(b=1;b<=n;b++)
                        if(mat[tot][b]==1 && mat[k][b]==0)break;
                    if(b==n+1)
                    {
                        tot--;
                        break;
                    }
                    for(b=1;b<=n;b++)
                        if(mat[tot][b]==0 && mat[k][b]==1)break;
                    if(b==n+1)
                    {
                        for(b=1;b<=n;b++)
                            mat[k][b]=mat[tot][b];
                        tot--;
                        break;
                    }
                }
            }
        }
    for(i=0;i<n;i++)
    {
        tot++;
        for(b=0;b<n;b++)
            if(dist2(p[i],p[b])<r*r+eps)
                mat[tot][b+1]=1,can[b+1]=1;
            else
                mat[tot][b+1]=0;
        for(k=1;k<tot;k++)
        {
            for(b=1;b<=n;b++)
                if(mat[tot][b]==1 && mat[k][b]==0)break;
            if(b==n+1)
            {
                tot--;
                break;
            }
            for(b=1;b<=n;b++)
                if(mat[tot][b]==0 && mat[k][b]==1)break;
            if(b==n+1)
            {
                for(b=1;b<=n;b++)
                    mat[k][b]=mat[tot][b];
                tot--;
                break;
            }
        }
    }
    for(b=1;b<=n;b++)if(!can[b])return 0;
    build(tot,n);
    ANS=n;
    dfs(0);
    if(ANS<=CC)return 1;
    return 0;
}

int main()
{
    int T,i,j;
    double mi,lo,hi;
    //freopen("in.txt","r",stdin);
    scanf("%d",&T);
    while(T--)
    {
        scanf("%d %d",&n,&CC);
        for(i=0;i<n;i++)
            scanf("%lf %lf",&p[i].x,&p[i].y);
        lo=0;hi=8;
        while(lo+eps<hi)
        {
            mi=(lo+hi)/2;
            if(check(mi))hi=mi;
            else lo=mi;
        }
        printf("%.6f\n",hi);
    }
    return 0;
}

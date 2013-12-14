#include <stdio.h>
#include <math.h>
#include <algorithm>
using namespace std;

#define eps 1e-5

double R1,R2;
double T[2][1000];
int pre,now;

double f(double x)
{
	return sqrt(R1*R1-x*x)*sqrt(R2*R2-x*x);
	//return 4.0/(1+x*x);
	//return exp(-x*x);
}

double Romberg(double a,double b)
{
	int k,i,h=1;
	double t;
	pre=0;now=1;
	T[pre][0]=(b-a)/2*(f(a)+f(b));
	//printf("%f\n",T[pre][0]);
	for(k=1;;k++)
	{//printf(":%d\n",k);
		h*=2;t=0;
		for(i=1;i<=h/2;i++)t+=f(a+(2*i-1)*(b-a)/h);
		T[now][0]=0.5*T[pre][0]+(b-a)/h*t;//printf("%f ",T[now][0]);
		t=1;
		for(i=1;i<=k;i++)
		{
			t*=4;
			T[now][i]=(t*T[now][i-1]-T[pre][i-1])/(t-1);
			//printf("%f ",T[now][i]);
		}
		//printf("\n");
		if(k>=3 && fabs(T[now][k]-T[pre][k-1])<eps)
		{
			//printf("%d %f\n",k,T[now][k]);
			return T[now][k];
		}
		swap(pre,now);
	}
}

int main()
{
	int T,f=0;
	//R1=73;R2=100;
	//printf("%.8f\n",Romberg(0,73));
	//puts("zp");return 0;
	scanf("%d",&T);
	while(T--)
	{
		if(f)putchar('\n');
		else f=1;
		scanf("%lf %lf",&R1,&R2);
	//	printf("%.4f\n",Romberg(0,1));
		printf("%.4f\n",8*Romberg(0,min(R1,R2)));
	}
	return 0;
}

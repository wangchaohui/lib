
#include <math.h>
#define eps 1e-8
#define swap(a,b){t=a;a=b;b=t;}

typedef double point[DIMENSION];

/*矩形能否放在矩形中*/
int rfitr(double l2,double s2,double l1,double s1)
{
	double t;
	if(l2<s2)swap(l2,s2);
	if(l1<s1)swap(l1,s1);
	if(s1>eps+s2)return 0;
	if(l1<eps+l2)return 1;
	t=sqrt(l1*l1+s1*s1);
	t=asin(l2/t)-asin(s1/t);
	t=l1*cos(t)+s1*sin(t);
	if(t<eps+s2)
		return 1;
	else
		return 0;
}

/*多边形面积*/
double PS(int n,point *p)
{
	int i;
	double s=p[n-1][0]*p[0][1]-p[0][0]*p[n-1][1];
	for(i=0;i<n-1;i++)
		s+=(p[i][0]*p[i+1][1]-p[i+1][0]*p[i][1]);
	return fabs(s/2);  
}

#include <stdio.h>
#include <math.h>
#define DIMENSION 2
#define eps 1e-8

typedef double point[DIMENSION];

typedef struct{
	double a,b,c;
}line;

void ptol(point p1,point p2,line *l);
void pandstol(point p,double m,line *l);
void intersec_point(line l1,line l2,point p);
double signed_distptol(point p,line l);
double distptol(point p,line l);
void closest_point(point in,line l,point c);
double distance(point a,point b);
double edgetoangle(double a,double b,double c);
double sacos(double a);

double min(double a,double b)
{
	return a<=b?a:b;
}

double max(double a,double b)
{
	return a>=b?a:b;
}

void ptol(point p1,point p2,line *l)
{
	if(p1[0]<eps+p2[0] && p1[0]+eps>p2[0]){l->a=1;l->b=0;l->c=-p1[0];}
	else{l->b=1;l->a=-(p1[1]-p2[1])/(p1[0]-p2[0]);
	l->c=-(l->a*p1[0])-p1[1];}
}

void pandstol(point p,double m,line *l)
{
	l->a=-m;l->b=1;l->c=m*p[0]-p[1];
}

void intersec_point(line l1,line l2,point p)
{
	p[0]=(l2.b*l1.c-l1.b*l2.c)/(l2.a*l1.b-l1.a*l2.b);
	if(fabs(l1.b)>eps)
		p[1]=-(l1.a*p[0]+l1.c)/l1.b;
	else
		p[1]=-(l2.a*p[0]+l2.c)/l2.b;
}

double signed_distptol(point p,line l)
{
	return (p[0]*l.a+p[1]*l.b+l.c)/sqrt(l.a*l.a+l.b*l.b);
}

double distptol(point p,line l)
{
	return fabs(signed_distptol(p,l));
}

void closest_point(point in,line l,point c)
{
	line perp;
	if(fabs(l.b)<eps){c[0]=-l.c;c[1]=in[1];return;}
	if(fabs(l.a)<eps){c[0]=in[0];c[1]=-l.c;return;}
	pandstol(in,1/l.a,&perp);
	intersec_point(l,perp,c);
}

int point_in_box(point p,point b1,point b2)
{
	return p[0]+eps>min(b1[0],b2[0]) && p[0]<eps+max(b1[0],b2[0])
		&& p[1]+eps>min(b1[1],b2[1]) && p[1]<eps+max(b1[1],b2[1]);
}

double distance2(point a,point b)
{
	int i;
	double d=0;
	for(i=0;i<DIMENSION;i++)
		d+=(a[i]-b[i])*(a[i]-b[i]);
	return d;
}

double distance(point a,point b)
{
	return sqrt(distance2(a,b));
}


double edgetoangle(double a,double b,double c)
{
	return sacos((b*b+c*c-a*a)/(2*b*c));
}

double sacos(double a)
{
	if(a>1)a=1;
	if(a<-1)a=-1;
	return acos(a);
}

/******* ******* *******
crosspoint()
	a1x+b1y+c1=0
	a2x+b2y+c2=0
******* ******* *******/

void crosspoint(double a1,double b1,double c1,double a2,double b2,double c2,double *x,double *y)
{
	*x=(c2*b1-c1*b2)/(a1*b2-a2*b1);
	*y=(c2*a1-c1*a2)/(b1*a2-b2*a1);
}

double tcr(double a,double b,double c)
{
	double p=(a+b+c)/2;
	if(p==0)return 0;
	p=(p-a)*(p-b)*(p-c)/p;
	if(p<0)return 0;
	return sqrt(p);
}

int pprtoo(point p1,point p2,double r,point o1,point o2)
{
	double xc=p1[0]-p2[0],yc=p1[1]-p2[1],t;
	t=r*r/(xc*xc+yc*yc)-0.25;
	if(t<0)if(t+eps>0)t=0;else return -1;
	t=sqrt(t);
	o1[0]=(p1[0]+p2[0])/2+t*yc;
	o1[1]=(p1[1]+p2[1])/2-t*xc;
	o2[0]=(p1[0]+p2[0])/2-t*yc;
	o2[1]=(p1[1]+p2[1])/2+t*xc;
	return 0;
}

#include <math.h>
#define EARTH_RADIUS 4000
#define PI 3.141592653589793

double distbtp(double lat1,double lng1,double lat2,double lng2)
{
	double a=lat1-lat2;
	double b=lng1-lng2;
	double s=2*asin(sqrt(pow(sin(a/2),2)+cos(lat1)*cos(lat2)*pow(sin(b/2),2)));
	s=s*EARTH_RADIUS;
	return s;
}
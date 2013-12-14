#include <algorithm>
using namespace std;

struct Rectangle{
	int x1,y1,x2,y2;

	Rectangle() {}
	Rectangle(int x1,int y1,int x2,int y2):x1(x1),y1(y1),x2(x2),y2(y2)
	{
		if(x1>x2)swap(x1,x2);
		if(y1>y2)swap(y1,y2);
	}

	bool isIntersect(Rectangle a,Rectangle b);
	Rectangle intersection(Rectangle a,Rectangle b);
};

bool Rectangle::isIntersect(Rectangle a,Rectangle b)
{
	return a.y2>=b.y1 && b.y2>=a.y1 && a.x2>=b.x1 && b.x2>=a.x1;
}

Rectangle Rectangle::intersection(Rectangle a,Rectangle b)
{
	return Rectangle(max(a.x1,b.x1),max(a.y1,b.y1),min(a.x2,b.x2),min(a.y2,b.y2));
} 
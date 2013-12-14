/*
ID: wangcha9
PROB: rect1
LANG: C++
*/
#include <stdio.h>
#include <algorithm>
using namespace std;

struct rectangle{
	int llx,lly,urx,ury,color;
	rectangle(){}
	rectangle(int llx,int lly,int urx,int ury,int color):llx(llx),lly(lly),urx(urx),ury(ury),color(color){}
};

rectangle r_[4004],r2_[4004],*r,*r2;
int cc[2510];

int main()
{
	int A,B,N,i,tot,p;
	rectangle a,c;
	freopen("rect1.in","r",stdin);
	freopen("rect1.out","w",stdout);
	scanf("%d %d %d",&A,&B,&N);
	tot=1;r=r_;r2=r2_;r[0]=rectangle(0,0,A,B,1);
	while(N--)
	{
		scanf("%d %d %d %d %d",&a.llx,&a.lly,&a.urx,&a.ury,&a.color);
		p=0;
		for(i=0;i<tot;i++)
			if((r[i].llx>=a.urx || r[i].urx<=a.llx) || (r[i].lly>=a.ury || r[i].ury<=a.lly))
				r2[p++]=r[i];
			else
			{
				if(r[i].llx<a.llx)
					r2[p++]=rectangle(r[i].llx,r[i].lly,a.llx,r[i].ury,r[i].color);
				if(r[i].urx>a.urx)
					r2[p++]=rectangle(a.urx,r[i].lly,r[i].urx,r[i].ury,r[i].color);
				if(r[i].lly<a.lly)
					r2[p++]=rectangle(max(r[i].llx,a.llx),r[i].lly,min(r[i].urx,a.urx),a.lly,r[i].color);
				if(r[i].ury>a.ury)
					r2[p++]=rectangle(max(r[i].llx,a.llx),a.ury,min(r[i].urx,a.urx),r[i].ury,r[i].color);
			}
		r2[p++]=a;
		swap(r,r2);
		tot=p;
	}
	for(i=0;i<tot;i++)
		cc[r[i].color]+=(r[i].urx-r[i].llx)*(r[i].ury-r[i].lly);
	for(i=1;i<=2500;i++)if(cc[i])printf("%d %d\n",i,cc[i]);
	return 0;
}
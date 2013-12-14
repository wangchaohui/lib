/*
升序数组a[]为:1124557
降序数组a[]为:7554211
测试的val依次为：5,8,1,0,3
*/

//答案为：58114
//[first,last)数组升序，获得满足a[j]>=val的值最小j。如果a[last-1]<val,返回last
int bsage(int *a,int first,int last,int val)
{
	int it,step,cnt=last-first;
	while(cnt>0)
	{
		step=cnt/2;it=first+step;
		if(a[it]<val){first=it+1;cnt-=step+1;}
		else cnt=step;
	}
	return first;
}

//答案为：78314
//[first,last)数组升序，获得满足a[j]>val的值最小j。如果a[last-1]<=val,返回last
int bsag(int *a,int first,int last,int val)
{
	int it,step,cnt=last-first;
	while(cnt>0)
	{
		step=cnt/2;it=first+step;
		if(a[it]<=val){first=it+1;cnt-=step+1;}
		else cnt=step;
	}
	return first;
}

//答案为：67203
//(first,last]数组升序，获得满足a[j]<=val的值最大j。如果a[first+1]>val,返回first;
int bsale(int *a,int first,int last,int val)
{
	int it,step,cnt=last-first;
	while(cnt>0)
	{
		step=cnt/2;it=last-step;
		if(a[it]>val)
		{last=--it;cnt-=(step+1);}
		elsecnt=step;
	}
	return last;
}

答案为：47003
//(first,last]数组升序，获得满足a[j]<val的值最大j。如果a[first+1]>=val,返回first;
intuless(inta[],intfirst,intlast,intval){
intit,step,cnt=last-first;
while(cnt>0){
it=last;step=cnt>>1;it-=step;
if(!(a[it]<val))
{last=--it;cnt-=(step+1);}
elsecnt=step;
}
returnlast;
}

答案为：30774
//(first,last]数组降序，获得满足a[j]>=val的值最大j。如果a[first]<val,返回first;
intdlargee(inta[],intfirst,intlast,intval){
intit,step,cnt=last-first;
while(cnt>0){
it=last;step=cnt>>1;it-=step;
if(a[it]<val)
{last=--it;cnt-=(step+1);}
elsecnt=step;
}
returnlast;
}

答案为：10574
//(first,last]数组降序，获得满足a[j]>val的值最大j。如果a[first]<=val,返回first;
intdlarge(inta[],intfirst,intlast,intval){
intit,step,cnt=last-first;
while(cnt>0){
it=last;step=cnt>>1;it-=step;
if(!(a[it]>val))
{last=--it;cnt-=(step+1);}
elsecnt=step;
}
returnlast;
}

答案为：21685
//[first,last)数组降序，获得满足a[j]<=val的值最小j。如果a[last-1]>val,返回last
intdlesse(inta[],intfirst,intlast,intval){
intit,step,cnt=last-first;
while(cnt>0){
it=first;step=cnt>>1;it+=step;
if(a[it]>val)
{first=++it;cnt-=(step+1);}
elsecnt=step;
}
returnfirst;
}

答案为：41885
//[first,last)数组降序，获得满足a[j]<val的值最小j。如果a[last-1]>=val,返回last
intl_ul(inta[],intfirst,intlast,intval){
intit,step,cnt=last-first;
while(cnt>0){
it=first;step=cnt>>1;it+=step;
if(!(a[it]<val))
{first=++it;cnt-=(step+1);}
elsecnt=step;
}
returnfirst;
}

//[first,last)数组升序。如果val在其中，返回一个值为val[x,y)区间。
//如果val不在其中。那么返回的x=y="使得a[j]>val的最小j.如果a[last-1]<val,则x=y=last.
voidget_er(int*a,intfirst,intlast,intval,int&x,int&y)
{
	x=l_ul(a,first,last,val);
	y=l_l(a,x,last,val);
}

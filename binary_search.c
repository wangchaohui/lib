/*
��������a[]Ϊ:1124557
��������a[]Ϊ:7554211
���Ե�val����Ϊ��5,8,1,0,3
*/

//��Ϊ��58114
//[first,last)�������򣬻������a[j]>=val��ֵ��Сj�����a[last-1]<val,����last
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

//��Ϊ��78314
//[first,last)�������򣬻������a[j]>val��ֵ��Сj�����a[last-1]<=val,����last
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

//��Ϊ��67203
//(first,last]�������򣬻������a[j]<=val��ֵ���j�����a[first+1]>val,����first;
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

��Ϊ��47003
//(first,last]�������򣬻������a[j]<val��ֵ���j�����a[first+1]>=val,����first;
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

��Ϊ��30774
//(first,last]���齵�򣬻������a[j]>=val��ֵ���j�����a[first]<val,����first;
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

��Ϊ��10574
//(first,last]���齵�򣬻������a[j]>val��ֵ���j�����a[first]<=val,����first;
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

��Ϊ��21685
//[first,last)���齵�򣬻������a[j]<=val��ֵ��Сj�����a[last-1]>val,����last
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

��Ϊ��41885
//[first,last)���齵�򣬻������a[j]<val��ֵ��Сj�����a[last-1]>=val,����last
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

//[first,last)�����������val�����У�����һ��ֵΪval[x,y)���䡣
//���val�������С���ô���ص�x=y="ʹ��a[j]>val����Сj.���a[last-1]<val,��x=y=last.
voidget_er(int*a,intfirst,intlast,intval,int&x,int&y)
{
	x=l_ul(a,first,last,val);
	y=l_l(a,x,last,val);
}

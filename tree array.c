

int lowbit(int n);

int sum(int *c,int end);
/******* ******* *******
	-lowbit
******* ******* *******/

void update(int *c,int pos,int num);
/******* ******* *******
	-lowbit
******* ******* *******/


/******* ******* *******

c[0] is the length

******* ******* *******/

int lowbit(int n)
{
	return n&(-n);
}

int sum(int *c,int end)
{
	int sum=0;
	while(end>0)
	{
		sum+=c[end];
		end-=lowbit(end);        
	}
	return sum;
}

void update(int *c,int pos,int num)
{
	while(pos<=c[0]) 
	{
		c[pos]+=num;
		pos+=lowbit(pos);
	}
}



int dayweek(int m,int d,int y)
{
	if(m<=2){y--;m+=12;}
	return (1+d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7;
}

int d[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
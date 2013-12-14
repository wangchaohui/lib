

int  ns[] = {1000, 900,500, 400,100, 90 , 50, 40 , 10,  9 , 5 , 4 , 1};
char rs[][3]={"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};

string Roman_numeral(int n)
{
	string r;
	int i;
	for(i=0;i<13;i++)
		while(n>=ns[i])
		{
			n-=ns[i];
			r+=rs[i];
		}
		return r;
}

int strcmpalapha(char *x,char *y);

/******* ******* *******

int strcmpalapha(char *x,char *y);
±È½Ï×Ö·û´®£¬ÒÔ×ÖµäË³Ğò
equal to strcmp()

******* ******* *******/

int strcmpalapha(char *x,char *y)
{
	int a=0;
	while(x[a]!='\0' && y[a]!='\0' && x[a]==y[a])a++;
	if(x[a]>y[a])
		return 1;
	else if(x[a]==y[a])
		return 0;
	else
		return -1;
}
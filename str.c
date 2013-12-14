
long serch(char *str,char *fn);
long replace(char *str,char *str2,char *s,char *t);
long length(char *str);
int match(char *str1,char *str2);

/******* ******* *******

serch
replace
·µ»Ø¸öÊý

length
equal to strlen

match is different from strcmp

******* ******* *******/

long serch(char *str,char *fn)
{
	int l,b,c;
	long a,fl;
	l=length(fn);
	fl=length(str);
	c=0;
	for(a=0;a<=fl-l+1;a++)
	{
		b=0;
		while(*(str+a+b)==*(fn+b) && b<l)
			b++;
		if(b==l)
		{
			c++;
			a+=l-1;
		}
	}
	return c;
}

long replace(char *str,char *str2,char *s,char *t)
{
	int ls,lt,c;
	long a,b,fl,f,e,m;
	ls=length(s);
	lt=length(t);
	fl=length(str);
	c=0;f=0;e=0;
	for(a=0;a<=fl-ls;a++)
	{
		b=0;
		while(*(str+a+b)==*(s+b) && b<ls)
			b++;
		if(b==ls)
		{
			c++;
			for(m=f;m<a;m++)
				*(str2+e+m-f)=*(str+m);
			e+=a-f;
			for(m=0;m<lt;m++)
				*(str2+e+m)=*(t+m);
			e+=lt;f=a+ls;
			a+=ls-1;
		}
	}
	for(m=f;m<=fl;m++)
		*(str2+e+m-f)=*(str+m);
	return c;
}

long length(char *str)
{
	long a=0;
	if(str!=NULL)while(*(str+a)!='\0')a++;
	return a;
}

int match(char *str1,char *str2)
{
	long a=0,l;
	l=length(str1);
	if(l==length(str2))
	{
		while(a<l && *(str1+a)==*(str2+a))a++;
		if(a==l)return 0;
		else return 1;
	}
	else
		return 1;
}

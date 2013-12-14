

int num[10][10];

int maxsubsequence(char *s,char *t)
{
	int ls,lt,i,j;
	ls=strlen(s);lt=strlen(t);
	for(i=0;i<=ls;i++)
		num[i][0]=0;
	for(j=0;j<=lt;j++)
		num[0][j]=0;
	for(i=1;i<=ls;i++)
		for(j=1;j<=lt;j++)
			if(s[i-1]==t[j-1])
				num[i][j]=num[i-1][j-1]+1;
			else
				num[i][j]=num[i-1][j]>num[i][j-1]?num[i-1][j]:num[i][j-1];
	i=ls;j=lt;
	while(num[i][j])
	{
		if(i>0 && num[i-1][j]==num[i][j])
			i--;
		else if(j>0 && num[i][j-1]==num[i][j])
			j--;
		else
		{
			if(num[i-1][j-1]!=num[i][j])
				ANS[num[i][j]-1]=s[i-1];
			i--;j--;
		}
	}
	ANS[num[ls][lt]]='\0';
	return num[ls][lt];
}

int maxstring(char *s,char *t)
{
	int ls,lt,i,j,c,m=0;
	ls=strlen(s);lt=strlen(t);
	for(i=0;i<ls;i++)
	{
		c=0;
		for(j=0;i+j<ls && j<lt;j++)
			if(s[i+j]==t[j])c++;
		if(m<c)m=c;
	}
	for(j=0;j<lt;j++)
	{
		c=0;
		for(i=0;j+i<lt && i<ls;i++)
			if(t[j+i]==s[i])c++;
		if(m<c)m=c;
	}
	return m;		
}

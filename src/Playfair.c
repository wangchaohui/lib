
#include <ctype.h>

void Playfair(char *p,char *e,char *key)
{
	char km[5][5],u[26]={0},ch,ch2;
	int i,j=0,x[26],y[26],x1,x2,y1,y2;
	j=0;
	for(i=0;p[i];i++)
		if(isalpha(p[i]))
			p[j++]=toupper(p[i]);
	p[j]='\0';
	j=0;
	for(i=0;j<25 && key[i];i++)
	{
		if(!isalpha(key[i]))continue;
		ch=toupper(key[i]);
		if(ch=='J')ch='I';
		if(u[ch-'A']==0)
		{
			u[ch-'A']=1;
			km[j/5][j%5]=ch;
			j++;
		}
	}
	ch='A';
	for(;j<25;j++)
	{
		for(;ch=='J' || u[ch-'A']==1;ch++);
		km[j/5][j%5]=ch++;
	}
	for(i=0;i<25;i++)
	{
		x[km[i/5][i%5]-'A']=i/5;
		y[km[i/5][i%5]-'A']=i%5;
		if(km[i/5][i%5]=='I')
		{
			x['J'-'A']=i/5;
			y['J'-'A']=i%5;
		}
	}
	j=0;
	for(i=0;p[i];i++)
	{	
		if(!p[i+1] || p[i+1]==p[i])
			ch=toupper(p[i]),ch2='X';
		else
			ch=toupper(p[i]),ch2=toupper(p[++i]);
		x1=x[ch-'A'];x2=x[ch2-'A'];
		y1=y[ch-'A'];y2=y[ch2-'A'];
		if(x1==x2)
		{
			e[j++]=km[x1][(y1+1)%5];
			e[j++]=km[x1][(y2+1)%5];
		}
		else if(y1==y2)
		{
			e[j++]=km[(x1+1)%5][y1];
			e[j++]=km[(x2+1)%5][y1];
		}
		else
		{
			e[j++]=km[x1][y2];
			e[j++]=km[x2][y1];
		}
	}
	e[j]='\0';
}
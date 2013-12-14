#include <stdio.h>

int pd(int ch);
void rm();

int main()
{
	printf("%d\n",pd(NULL));	
	return 0;
}

int ys(int v,char s,int g)
{
	switch(s)
	{
	case '*':
		return v*g;
		break;
	case '/':
		return v/g;
		break;
	case '+':
		return v+g;
		break;
	case '-':
		return v-g;
		break;
	default:
		return v;
	}
}

int pd(int ch)
{
	int v=0,n=0,g=0,f=1,t,c;
	char s='\0';
	if(ch==NULL)
		ch=getchar();
	do
	{
		switch(ch)
		{
		case '*':
		case '/':
			f=0;
			if(s=='+' || s=='-')
			{
				c=scanf("%d",&t);
				if(c==0){getchar();t=pd(NULL);}
				g=ys(g,ch,t);
			}
			else
			{
				v=ys(v,s,g);
				s=ch;g=0;
			}
			break;
		case '+':
		case '-':
			if(f==1)
			{
				f=0;
				s=ch;
			}
			else
			{
				v=ys(v,s,g);
				g=pd(ch);
				return v+g;
			}
			break;
		case '(':	
			if(s=='\0')
				v=pd(NULL);
			else
				g=pd(NULL);
			break;
		case '\n':
		case ')':
		case EOF:
		case ' ':
			return ys(v,s,g);
		default:
			if(s=='\0')
				v=v*10+ch-'0';
			else
				g=g*10+ch-'0';
		}
	}while((ch=getchar())!=')' && ch!='\n' && ch!=EOF);
	return ys(v,s,g);
}

void rm()
{
	int ch,c=0;
	while(c!=-1)
	{
		ch=getchar();
		switch(ch)
		{
		case '(':
			c++;
			break;
		case ')':
		case '\n':
		case EOF:
			c--;
		}
	}
}
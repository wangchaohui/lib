#include <stdio.h>
#include <ctype.h>

typedef struct node node;
typedef struct mono mono;

typedef struct node{
	char name;
	int exp;
	node *next;
};

typedef struct mono{
	node *first;
	int coef;
	mono *next;
};

node n[2000000];
int ntot;

mono m[200000];
int mtot;

mono *out[100];
char in[100];
int inpp,outpp,priority[256];

void init()
{
	mtot=ntot=0;
	priority['\0']=0;
	priority['+']=2;
	priority['-']=2;
	priority['*']=3;
	priority['/']=3;
	priority['(']=1;
}

int monocmp(mono *a,mono *b)
{
	node *x=a->first,*y=b->first;
	while(1)
	{
		if(x==NULL && y==NULL)return 0;
		if(x==NULL)return -1;
		if(y==NULL)return 1;
		if(x->name!=y->name)return x->name-y->name;
		if(x->exp!=y->exp)return x->exp-y->exp;
		x=x->next;y=y->next;
	}
}

mono* multiqulin(mono *r)
{
	mono *a,*rr=r;
	for(a=r;a && a->coef==0;r=a->next,a=r);
	if(r==NULL)
	{
		rr->coef=0;rr->first=NULL;rr->next=NULL;
		return rr;
	}
	for(a=r;a->next;)
	{
		if(a->next->coef==0)
			a->next=a->next->next;
		else
			a=a->next;
	}
	return r;
}

mono* jia(mono *a,mono *b)
{
	mono *r=a,*aa=NULL,*t;
	int g;
	while(a!=NULL && b!=NULL)
	{
		g=monocmp(a,b);
		if(g<0)
		{
			aa=a;
			a=a->next;
		}
		else if(g>0)
		{
			t=b;b=b->next;
			if(aa==NULL)
				r=t;
			else
				aa->next=t;
			t->next=a;
			aa=t;
		}
		else
		{
			a->coef+=b->coef;
			aa=a;
			a=a->next;
			b=b->next;
		}
	}
	if(a==NULL)
		aa->next=b;
	return multiqulin(r);
}

mono* jian(mono *a,mono *b)
{
	mono *r=a,*aa=NULL,*t;
	int g;
	while(a!=NULL && b!=NULL)
	{
		g=monocmp(a,b);
		if(g<0)
		{
			aa=a;
			a=a->next;
		}
		else if(g>0)
		{
			t=b;b=b->next;
			if(aa==NULL)
				r=t;
			else
				aa->next=t;
			t->next=a;
			t->coef=-t->coef;
		}
		else
		{
			a->coef-=b->coef;
			b=b->next;
		}
	}
	if(a==NULL)
	{
		aa->next=b;
		while(b!=NULL)
		{
			b->coef=-b->coef;
			b=b->next;
		}
	}
	return multiqulin(r);
}

node* nodecheng(node *a,node *b)
{
	node *r=&n[ntot],*s=NULL,*t;
	if(a==NULL && b==NULL)return NULL;
	while(a!=NULL || b!=NULL)
	{
		t=&n[ntot++];
		if(s!=NULL)
			s->next=t;
		s=t;
		if(a!=NULL && (b==NULL || a->name-b->name<0))
		{
			*t=*a;
			a=a->next;
		}
		else if(b!=NULL && (a==NULL || a->name-b->name>0))
		{
			*t=*b;
			b=b->next;
		}
		else
		{
			t->name=a->name;
			t->exp=a->exp+b->exp;
			a=a->next;
			b=b->next;
		}
	}
	t->next=NULL;
	return r;
}

mono* cheng(mono *a,mono *b)
{
	mono *r=&m[mtot],*s=NULL,*t,*mm,x,*j;
	for(;a;a=a->next)
		for(j=b;j;j=j->next)
		{
			t=&m[mtot++];
			if(s!=NULL)
				s->next=t;
			s=t;
			t->first=nodecheng(a->first,j->first);
			t->coef=a->coef*j->coef;
		}
	t->next=NULL;
	for(a=r;a;a=a->next)
	{
		mm=a;
		for(b=a->next;b;b=b->next)
			if(monocmp(mm,b)>0)
				mm=b;
		x.coef=a->coef;a->coef=mm->coef;mm->coef=x.coef;
		x.first=a->first;a->first=mm->first;mm->first=x.first;
	}
	for(a=r;a->next;)
	{
		if(monocmp(a,a->next)==0)
		{
			a->coef+=a->next->coef;
			a->next=a->next->next;
		}
		else
			a=a->next;
	}
	return multiqulin(r);
}

void jisuan(char op)
{
	mono *a,*b;
	b=out[outpp--];
	a=out[outpp--];
	switch(op)
	{
	case '+':
		a=jia(a,b);
		break;
	case '-':
		a=jian(a,b);
		break;
	case '*':
		a=cheng(a,b);
		break;
	}
	out[++outpp]=a;
}

mono* bds(char *s)
{
	int i;
	mono *x;
	node *y;
	char ch;
	inpp=outpp=0;
	for(i=0;;)
	{
		switch(s[i])
		{
		case ' ':
		case '\t':
			break;
		case '+':
		case '-':
		case '*':
		case '\0':
			while(inpp>0 && priority[in[inpp]]>=priority[s[i]])
				jisuan(in[inpp--]);
			in[++inpp]=s[i];
			break;
		case '(':
			in[++inpp]=s[i];
			break;
		case ')':
			while(in[inpp]!='(')
				jisuan(in[inpp--]);
			inpp--;
			break;
		}
		if(s[i]>='0' && s[i]<='9')
		{
			x=&m[mtot++];
			x->first=NULL;
			x->coef=s[i]-'0';
			x->next=NULL;
			out[++outpp]=x;
		}
		ch=tolower(s[i]);
		if(ch>='a' && ch<='z')
		{
			x=&m[mtot++];
			y=&n[ntot++];
			x->first=y;
			x->coef=1;
			x->next=NULL;
			y->name=ch;
			y->exp=1;
			y->next=NULL;
			out[++outpp]=x;
		}
		if(s[i]=='\0')break;
		i++;
	}
	return out[1];
}

int multicmp(mono *a,mono *b)
{
	int g;
	while(1)
	{
		if(a==NULL && b==NULL)return 0;
		if(a==NULL)return -1;
		if(b==NULL)return 1;
		g=monocmp(a,b);
		if(g)return g;
		if(a->coef!=b->coef)return a->coef-b->coef;
		a=a->next;b=b->next;
	}
	return 0;
}

int main()
{
	int T;
	char s[200];
	mono *x,*y;
	init();
	scanf("%d",&T);
	gets(s);
	while(T--)
	{
		gets(s);
		x=bds(s);
		gets(s);
		y=bds(s);
		if(multicmp(x,y)==0)
			puts("YES");
		else
			puts("NO");
	}
	return 0;
}
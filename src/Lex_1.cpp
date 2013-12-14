#include <cstdio>
#include <cctype>
#include <cstring>
#include <cstdlib>

#define N 1000
#define test(x,y) (x[y/8]>>y%8&1)
#define set(x,y) (x[y/8]|=1<<y%8)

struct Node{
	int ch;
	int token;
	int n;
	char nullable;
	char firstpos[N/8];
	char lastpos[N/8];
	struct Node *left,*right;
};

struct Node *out[N];
char in[N];
int inpp,outpp,priority[256];

struct Node d[20000];
int tot;
int cc;

char followpos[N][N/8];

int state_n;
char S[N][N/8];
int tran[N][256];
char accepting[N];
int partition[N];
int G[N][256];
int p[N];
int M_n;

struct State{
	int tran[256];
	int accepting;
	int old_n;
	int mn;
};

struct State M[N];

char isinput[256];
char C[N];             /* C[i] is the char at position i */

struct Regular{
	char name[16];
	char regular[256];
	struct Node *tree;
};

int regular_table_size;

int acposition[N];
int token_n;
int regular_n;

struct Regular regulars[]={
	{"D","(0|1|2|3|4|5|6|7|8|9)"},
	{"L","(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|_)"},
};

struct Regular tokens[]={
	{"{","{"},
	{"}","}"},
	{"if","if"},
	{"(","\\("},
	{")","\\)"},
	{"then","then"},
	{"else","else"},
	{"while","while"},
	{"ID","@L@(@L@|@D@)*"},
	{"=","="},
	{";",";"},
	{"<","<"},
	{">",">"},
	{"<=","<="},
	{">=",">="},
	{"==","=="},
	{"+","+"},
	{"-","-"},
	{"*","\\*"},
	{"/","/"},
	{"NUM","@D@@D@*"},
	{"","( |\t|\n)"}
};

class ConstructException{
public:
	char message[256];
	ConstructException() {}
	ConstructException(char *s)
	{
		strncpy(message,s,sizeof(message)-1);
		message[sizeof(message)-1]='\0';
	}
};

struct Token{
	char name[16];
	char value[256];
	int key;
};

struct Node* newNode(int ch,struct Node *left,struct Node *right)
{
	struct Node *x=&d[tot++];
	x->ch=ch;x->left=left;x->right=right;
	return x;
}

int jisuan(char op)
{
	struct Node *a,*b;
	switch(op)
	{
	case '*':
		if(outpp==0)return -1;
		a=out[outpp--];
		out[++outpp]=newNode('*'+256,a,NULL);
		break;
	case '|':
		if(outpp<2)return -1;
		b=out[outpp--];
		a=out[outpp--];
		out[++outpp]=newNode('|'+256,a,b);
		break;
	case '&':
		if(outpp<2)return -1;
		b=out[outpp--];
		a=out[outpp--];
		out[++outpp]=newNode('&'+256,a,b);
		break;
	}
	return 0;
}

struct Node* copyTree(struct Node *x)
{
	if(x==NULL)return NULL;
	struct Node *y=newNode(x->ch,NULL,NULL);
	if(y->ch<256)
	{
		isinput[y->ch]=1;
		y->n=++cc;
		C[y->n]=y->ch;
	}
	y->left=copyTree(x->left);
	y->right=copyTree(x->right);
	return y;
}

struct Node* regular_expression_syntax_tree(char *s)
{
	int i,j,f=0;
	inpp=outpp=0;
	for(i=0;;i++)
	{
		if(!isprint(s[i]) && s[i]!='\t' && s[i]!='\n' && s[i]!='\0')return NULL;
		switch(s[i])
		{
		case '*':
		case '\0':
		case '|':
			while(inpp>0 && priority[in[inpp]]>=priority[s[i]])
				if(jisuan(in[inpp--])==-1)return NULL;
			in[++inpp]=s[i];
			if(s[i]=='|')f=0;
			break;
		case '(':
			if(f)
			{
				while(inpp>0 && priority[in[inpp]]>=priority['&'])
					if(jisuan(in[inpp--])==-1)return NULL;
				in[++inpp]='&';
			}
			in[++inpp]=s[i];
			f=0;
			break;
		case ')':
			while(inpp>0 && in[inpp]!='(')
				if(jisuan(in[inpp--])==-1)return NULL;
			if(inpp--==0)return NULL;
			f=1;
			break;
		case '@':
			char regular[16];
			for(j=0;s[++i]!='@' && s[i]!='\0' && j<15;j++)regular[j]=s[i];
			if(s[i]!='@')return NULL;
			regular[j]='\0';
			for(j=0;j<regular_table_size;j++)if(strcmp(regular,regulars[j].name)==0)break;
			if(j==regular_table_size || regulars[j].tree==NULL)return NULL;
			if(f)
			{
				while(inpp>0 && priority[in[inpp]]>=priority['&'])
					if(jisuan(in[inpp--])==-1)return NULL;
				in[++inpp]='&';
			}
			out[++outpp]=copyTree(regulars[j].tree);
			f=1;
			break;
		case '\\':
		default:
			int trans=0;
			if(s[i]=='\\')
			{
				i++;
				trans=1;
			}
			if(f)
			{
				while(inpp>0 && priority[in[inpp]]>=priority['&'])
					if(jisuan(in[inpp--])==-1)return NULL;
				in[++inpp]='&';
			}
			int t;
			if(!trans && s[i]=='$')t='$'+256;
			else t=s[i];
			struct Node *x=newNode(t,NULL,NULL);
			if(trans || s[i]!='$')
			{
				isinput[s[i]]=1;
				x->n=++cc;
				C[x->n]=s[i];
			}
			out[++outpp]=x;
			f=1;
			break;
		}
		if(s[i]=='\0')break;
	}
	if(inpp!=1 || outpp!=1)return NULL;
	return out[1];
}

struct Node* bds(char *s)
{
	struct Node *x=regular_expression_syntax_tree(s);
	if(x==NULL)return NULL;
	struct Node *y=newNode('#'+256,NULL,NULL);
	y->n=++cc;
	C[y->n]='#';
	return newNode('&'+256,x,y);;
}

void print_char(char ch,int l)
{
	if(isprint(ch))
	{
		if(ch==' ')
			printf("%*s",l,"SP");
		else
			printf("%*c",l,ch);
	}
	else
	{
		if(ch=='\t')
			printf("%*s",l,"\\t");
		else if(ch=='\n')
			printf("%*s",l,"\\n");
		else
			printf("%*s",l,"NP");
	}
}

void ptr(struct Node *x,int l)
{
	int i,f;
	if(!x)return;
	if(x->ch<256)
		print_char(x->ch,l+1);
	else
		printf("%*c%c",l+1,'_',x->ch-256);
	if(x->ch<256 || x->ch=='#'+256)
	{
		if(x->ch=='#'+256)printf("(%d)",x->token);
		printf(" %d",x->n);
	}
	printf(" (%s {",x->nullable?"true":"false");
	f=0;
	for(i=1;i<=cc;i++)
	{
		if(test(x->firstpos,i))
		{
			if(f)putchar(',');else f=1;
			printf("%d",i);
		}
	}
	printf("} {");
	f=0;
	for(i=1;i<=cc;i++)
	{
		if(test(x->lastpos,i))
		{
			if(f)putchar(',');else f=1;
			printf("%d",i);
		}
	}
	printf("})");
	putchar('\n');
	ptr(x->left,l+2);
	ptr(x->right,l+2);
}

bool orfrom(char *to,char *from,int size)
{
	bool r=false;
	for(int i=0;i<=(size-1)/8;i++)
	{
		if((to[i]&from[i])!=from[i])r=true;
		to[i]|=from[i];
	}
	return r;
}

void nfl(struct Node *x)
{
	int i;
	if(x->ch=='$'+256)
	{
		x->nullable=1;
		for(i=0;i<=cc/8;i++)x->firstpos[i]=x->lastpos[i]=0;
	}
	if(x->ch<256 || x->ch=='#'+256)
	{
		x->nullable=0;
		for(i=0;i<=cc/8;i++)x->firstpos[i]=x->lastpos[i]=0;
		set(x->firstpos,x->n);
		set(x->lastpos,x->n);
	}
	if(x->ch=='|'+256)
	{
		nfl(x->left);
		nfl(x->right);
		x->nullable=x->left->nullable|x->right->nullable;
		for(i=0;i<=cc/8;i++)
		{
			x->firstpos[i]=x->left->firstpos[i]|x->right->firstpos[i];
			x->lastpos[i]=x->left->lastpos[i]|x->right->lastpos[i];
		}
	}
	if(x->ch=='&'+256)
	{
		nfl(x->left);
		nfl(x->right);
		x->nullable=x->left->nullable&x->right->nullable;
		if(x->left->nullable)
			for(i=0;i<=cc/8;i++)x->firstpos[i]=x->left->firstpos[i]|x->right->firstpos[i];
		else
			for(i=0;i<=cc/8;i++)x->firstpos[i]=x->left->firstpos[i];
		if(x->right->nullable)
			for(i=0;i<=cc/8;i++)x->lastpos[i]=x->left->lastpos[i]|x->right->lastpos[i];
		else
			for(i=0;i<=cc/8;i++)x->lastpos[i]=x->right->lastpos[i];
		for(i=1;i<=cc;i++)if(test(x->left->lastpos,i))
			orfrom(followpos[i],x->right->firstpos,cc+1);
	}
	if(x->ch=='*'+256)
	{
		nfl(x->left);
		x->nullable=1;
		for(i=0;i<=cc/8;i++)
		{
			x->firstpos[i]=x->left->firstpos[i];
			x->lastpos[i]=x->left->lastpos[i];
		}
		for(i=1;i<=cc;i++)if(test(x->lastpos,i))
			orfrom(followpos[i],x->firstpos,cc+1);
	}
}

void print_followpos()
{
	for(int j=1;j<=cc;j++)
	{
		print_char(C[j],2);
		printf(" %d {",j);
		int f=0;
		for(int i=1;i<=cc;i++)
		{
			if(test(followpos[j],i))
			{
				if(f)putchar(',');else f=1;
				printf("%d",i);
			}
		}
		printf("}\n");
	}
	putchar('\n');
}

void DFA_from_regular(struct Node *root)
{
	int i,j,k,a;
	char U[N/8];
	state_n=1;
	for(i=0;i<=cc/8;i++)S[0][i]=root->firstpos[i];
	for(i=0;i<state_n;i++)
	{
		for(j=0;j<256;j++)
		{
			if(isinput[j])
			{
				for(a=0;a<=cc/8;a++)U[a]=0;
				for(k=1;k<=cc;k++)
				{
					if(test(S[i],k) && C[k]==j)
						for(a=0;a<=cc/8;a++)U[a]|=followpos[k][a];
				}
				for(a=0;a<=cc/8;a++)if(U[a])break;
				if(a>cc/8)
				{
					tran[i][j]=-1;
					continue;
				}
				for(k=0;k<state_n;k++)
				{
					for(a=0;a<=cc/8;a++)if(U[a]!=S[k][a])break;
					if(a>cc/8)break;
				}
				if(k==state_n)
				{
					for(a=0;a<=cc/8;a++)S[state_n][a]=U[a];
					state_n++;
				}
				tran[i][j]=k;
			}
			else
			{
				tran[i][j]=-1;
			}
		}
		accepting[i]=-1;
		for(j=0;j<token_n;j++)
			if(test(S[i],acposition[j]))
			{
				accepting[i]=j;
				break;
			}
	}	
}



int cmp(const void *a_,const void *b_)
{
	int a=*(int*)a_,b=*(int*)b_;
	if(partition[a]!=partition[b])return partition[a]-partition[b];
	for(int i=0;i<256;i++)if(isinput[i] && G[a][i]!=G[b][i])return G[a][i]-G[b][i];
	return 0;
}

int cmp_state(const void *a_,const void *b_)
{
	const struct State *a=(struct State*)a_,*b=(struct State*)b_;
	return a->mn-b->mn;
}

void Minimizing_DFA()
{
	int i,j,f;
	int visited[N+1];
	memset(visited,-1,sizeof(visited));
	M_n=0;
	for(i=0;i<state_n;i++)
	{
		if(visited[accepting[i]+1]==-1)visited[accepting[i]+1]=M_n++;
		partition[i]=visited[accepting[i]+1];
	}
	do 
	{
		f=0;
		for(i=0;i<state_n;i++)for(j=0;j<256;j++)
			if(isinput[j] && ~tran[i][j])
				G[i][j]=partition[tran[i][j]];
			else
				G[i][j]=-1;
		for(i=0;i<state_n;i++)p[i]=i;
		qsort(p,state_n,sizeof(p[0]),cmp);
		for(i=0;i<state_n;i++)
		{
			if(i>0 && partition[p[i]]==partition[p[i-1]])
			{
				if(cmp(p+i-1,p+i))
				{
					partition[p[i]]=M_n++;
					f=1;
				}
				else
				{
					partition[p[i]]=partition[p[i-1]];
				}
			}
		}
	}while(f);
	for(i=0;i<state_n;i++)for(j=0;j<256;j++)
		M[partition[i]].tran[j]=G[i][j];
	for(i=0;i<M_n;i++)
	{
		M[i].old_n=i;
		M[i].mn=state_n;
	}
	for(i=0;i<state_n;i++)
	{
		M[partition[i]].accepting=accepting[i];
		if(M[partition[i]].mn>i)M[partition[i]].mn=i;
	}
	qsort(M,M_n,sizeof(M[0]),cmp_state);
	for(i=0;i<M_n;i++)p[M[i].old_n]=i;
	for(i=0;i<M_n;i++)for(j=0;j<256;j++)
		if(~M[i].tran[j])
			M[i].tran[j]=p[M[i].tran[j]];
}

void print_DFA()
{
	int i,j,l,L=5;
	printf("%*c",L+2,' ');for(j=0;j<256;j++)if(isinput[j])print_char(j,5);putchar('\n');
	for(i=0;i<state_n;i++)
	{
		printf("%2d",i);
		if(~accepting[i])l=printf("#(%d)",accepting[i]);
		else l=0;
		printf("%*s",L-l,"");
		for(j=0;j<256;j++)if(isinput[j])printf("%5d",tran[i][j]);
		printf(" {");
		int f=0;
		for(j=1;j<=cc;j++)
		{
			if(test(S[i],j))
			{
				if(f)putchar(',');else f=1;
				printf("%d",j);
			}
		}
		printf("}\n");
	}
	putchar('\n');
}
void print_Minimized_DFA()
{
	int i,j,l,L=5;
	printf("%*c",L+2,' ');for(j=0;j<256;j++)if(isinput[j])print_char(j,5);putchar('\n');
	for(i=0;i<M_n;i++)
	{
		printf("%2d",i);
		if(~M[i].accepting)l=printf("#(%d)",M[i].accepting);
		else l=0;
		printf("%*s",L-l,"");
		for(j=0;j<256;j++)if(isinput[j])printf("%5d",M[i].tran[j]);
		putchar('\n');
	}
	putchar('\n');
}

void init()
{
	priority['&']=2;
	priority['\0']=1;
	priority['*']=3;
	priority['|']=1;
	priority['(']=0;
	tot=0;
	cc=0;
	memset(isinput,0,sizeof(isinput));
	regular_n=sizeof(regulars)/sizeof(regulars[0]);
	regular_table_size=0;
	for(int i=0;i<regular_n;i++)
	{
		regulars[i].tree=regular_expression_syntax_tree(regulars[i].regular);
		if(regulars[i].tree==NULL)
			printf("Regular #%d:%s Is Wrong\n",i,regulars[i].name);
		regular_table_size++;
	}
	cc=0;
}

int Test()
{
	char s[200];
	while(gets(s))
	{
		init();
		struct Node *x=bds(s);
		if(x)
		{
			memset(followpos,0,sizeof(followpos));
			nfl(x);
			ptr(x,0);
			putchar('\n');
			print_followpos();
			DFA_from_regular(x);
			print_DFA();
			Minimizing_DFA();
			print_Minimized_DFA();
		}
	}
	return 0;
}

int Lex_analyzer_construct()
{
	struct Node *root=NULL;
	init();
	memset(isinput,0,sizeof(isinput));
	for(int i=0;i<token_n;i++)
	{
		struct Node *x=bds(tokens[i].regular);
		if(x==NULL)
		{
			printf("Token #%d:%s Is Wrong\n",i,tokens[i].name);
			printf("Lex analyzer construct failed.\n");
			return -1;
		}
		x->right->token=i;
		acposition[i]=x->right->n;
		if(root==NULL)root=x;
		else root=newNode('|'+256,root,x);
	}
	memset(followpos,0,sizeof(followpos));
	nfl(root);
	ptr(root,0);
	putchar('\n');
	print_followpos();
	DFA_from_regular(root);
	print_DFA();
	Minimizing_DFA();
	print_Minimized_DFA();
	return 0;
}

class InputStream{
public:
	virtual char getNext()=0;
	virtual char lookUp()=0;
	virtual void consume(int n,char *s)=0;
};

class ArrayInputStream:public InputStream{
private:
	char *p;
	char *next;
public:
	void init(char *s);
	char getNext();
	char lookUp();
	void consume(int n,char *s);
};

void ArrayInputStream::init(char *s)
{
	p=s;
	next=p;
}

char ArrayInputStream::getNext()
{
	char r=*next;
	if(r!='\0')next++;
	return r;
}

char ArrayInputStream::lookUp()
{
	return *next;
}

void ArrayInputStream::consume(int n,char *s)
{
	strncpy(s,p,n);
	s[n]='\0';
	init(p+n);
}

class ConsoleInputStream:public InputStream{
private:
	char buf[2000];
	char *p;
	char *e;
public:
	ConsoleInputStream():p(buf),e(buf) {}
	char getNext();
	char lookUp();
	void consume(int n,char *s);
};

char ConsoleInputStream::getNext()
{
	if(p==e)
	{
		if(e==buf+sizeof(buf))
		{
			puts("ConsoleInputStream buf overflow");
			return '\0';
		}
		int g=getchar();
		if(g==-1)g='\0';
		*e++=g;
	}
	return *p++;
}

char ConsoleInputStream::lookUp()
{
	char r=getNext();
	p--;
	return r;
}

void ConsoleInputStream::consume(int n,char *s)
{
	strncpy(s,buf,n);
	s[n]='\0';
	for(p=buf+n;p<e;p++)
	{
		*(p-n)=*p;
	}
	p=buf;
	e-=n;
}

struct Token Lex(struct State *M,InputStream *ins)
{
	struct Token r;
	r.name[0]='\0';
	r.value[0]='\0';
	do
	{
		if(ins->lookUp()=='\0')
		{
			r.key=token_n;
			sprintf(r.name,"$");
			return r;
		}
		int n=0,na=-1;
		for(int state=0;~state;n++)
		{
			if(~M[state].accepting)
			{
				r.key=M[state].accepting;
				na=n;
			}
			char in=ins->getNext();
			if(!in)break;
			state=M[state].tran[in];
		}
		if(na==-1)
		{
			r.key=-1;
			return r;
		}
		ins->consume(na,r.value);
	}while(tokens[r.key].name[0]=='\0');
	strcpy(r.name,tokens[r.key].name);
	return r;
}

/********************************************************************************/

int nonterminal_n;
char nonterminals[N][16];

struct Element{
	int type;
	int key;

	char* getName()
	{
		return type==0?nonterminals[key]:tokens[key].name;
	}
};

struct Production{
	char name[16];
	char production[256];
	int nonterminal;
	struct Element elements[N];
	int size;
};

int production_n;

struct Production productions[]={
	{"program","compoundstmt"},
	{"stmt","ifstmt"},
	{"stmt","whilestmt"},
	{"stmt","assgstmt"},
	{"stmt","compoundstmt"},
	{"compoundstmt","@{@ stmts @}@"},
	{"stmts","stmt stmts"},
	{"stmts",""},
	{"ifstmt","@if@ @(@ boolexpr @)@ @then@ stmt @else@ stmt"},
	{"whilestmt","@while@ @(@ boolexpr @)@ stmt"},
	{"assgstmt","@ID@ @=@ arithexpr @;@"},
	{"boolexpr","arithexpr boolop arithexpr"},
	{"boolop","@<@"},
	{"boolop","@>@"},
	{"boolop","@<=@"},
	{"boolop","@>=@"},
	{"boolop","@==@"},
	{"arithexpr","multexpr arithexprprime"},
	{"arithexprprime","@+@ multexpr arithexprprime"},
	{"arithexprprime","@-@ multexpr arithexprprime"},
	{"arithexprprime",""},
	{"multexpr","simpleexpr multexprprime"},
	{"multexprprime","@*@ simpleexpr multexprprime"},
	{"multexprprime","@/@ simpleexpr multexprprime"},
	{"multexprprime",""},
	{"simpleexpr","@ID@"},
	{"simpleexpr","@NUM@"},
	{"simpleexpr","@(@ arithexpr @)@"},
};

int find_token(char *s)
{
	for(int i=0;i<token_n;i++)
	{
		if(strcmp(tokens[i].name,s)==0)return i;
	}
	return -1;
}

int find_nonterminal(char *s)
{
	for(int i=0;i<nonterminal_n;i++)
	{
		if(strcmp(nonterminals[i],s)==0)return i;
	}
	return -1;
}

void prepare_nonterminals()
{
	int i,j,g;
	char token[16];
	char production[256];
	nonterminal_n=0;
	for(i=0;i<production_n;i++)
	{
		g=find_nonterminal(productions[i].name);
		if(g==-1)
		{
			strcpy(nonterminals[nonterminal_n],productions[i].name);
			g=nonterminal_n++;
		}
		productions[i].nonterminal=g;
	}
	for(i=0;i<production_n;i++)
	{
		productions[i].size=0;
		strcpy(production,productions[i].production);
		for(char *p=strtok(production," ");p;p=strtok(NULL," "))
		{
			if(p[0]=='@')
			{
				for(j=0;*++p!='@' && *p!='\0' && j<15;j++)token[j]=*p;
				if(*p!='@')
				{
					printf("Production #%d Is Wrong - Format error\n",i);
					printf("Syntax analyzer construct failed.\n");
					return;
				}
				token[j]='\0';
				g=find_token(token);
				if(g==-1)
				{
					printf("Production #%d Is Wrong - Token name %s not found.\n",i,token);
					printf("Syntax analyzer construct failed.\n");
					return;
				}
				productions[i].elements[productions[i].size].type=1;
				productions[i].elements[productions[i].size].key=g;
			}
			else
			{
				g=find_nonterminal(p);
				if(g==-1)
				{
					printf("Production #%d Is Wrong - Nonterminal name %s not found.\n",i,p);
					printf("Syntax analyzer construct failed.\n");
					return;
				}
				productions[i].elements[productions[i].size].type=0;
				productions[i].elements[productions[i].size].key=g;
			}
			productions[i].size++;
		}
	}
}

struct First{
	char terminals[N];
	char visited;
	bool empty;
};

struct First first[N];
struct First nonterminal_first[N];

class Split{
	char *p;
public:
	Split(char *s)
	{
		p=s;
	}
	char* next()
	{
		while(*p==' ')p++;
		if(*p=='\0')return NULL;
		char *r=p;
		while(*p!=' ' && *p!='\0')p++;
		if(*p==' ')*p++='\0';
		return r;
	}
	char* getPosition()
	{
		return p;
	}
};

void first_dfs(int x);

void first_string(char *s,First *target)
{
	memset(target,0,sizeof(*target));
	char production[256];
	strcpy(production,s);
	Split split(production);
	char *p;
	while(p=split.next())
	{
		if(p[0]=='@')
		{
			int j;
			char token[16];
			for(j=0;*++p!='@' && *p!='\0' && j<15;j++)token[j]=*p;
			if(*p!='@')
			{
				ConstructException ex;
				sprintf(ex.message,"Production \"%s\" Wrong - Format error.",s);
				throw ex;
			}
			token[j]='\0';
			int t=find_token(token);
			if(t==-1)
			{
				ConstructException ex;
				sprintf(ex.message,"Production \"%s\" Is Wrong - Token name %s not found.",s,token);
				throw ex;
			}
			set(target->terminals,t);
			break;
		}
		else
		{
			bool canempty=false,found=false;
			for(int i=0;i<production_n;i++)
			{
				if(strcmp(productions[i].name,p)==0)
				{
					found=true;
					first_dfs(i);
					orfrom(target->terminals,first[i].terminals,token_n);
					if(first[i].empty)canempty=true;
				}
			}
			if(!found)
			{
				ConstructException ex;
				sprintf(ex.message,"Nonterminal %s not found.",p);
				throw ex;
			}
			if(!canempty)break;
		}
	}
	target->empty|=(p==NULL);
}

void first_dfs(int x)
{
	if(first[x].visited==1)return;
	if(first[x].visited==-1)
	{
		ConstructException ex;
		sprintf(ex.message,"Production #%d Is Left Recursion.",x);
		throw ex;
	}
	first[x].visited=-1;
	first_string(productions[x].production,&first[x]);
	first[x].visited=1;
}

void calc_first()
{
	memset(nonterminal_first,0,sizeof(nonterminal_first));
	for(int i=0;i<production_n;i++)
	{
		first_dfs(i);
		orfrom(nonterminal_first[productions[i].nonterminal].terminals,first[i].terminals,token_n);
		nonterminal_first[productions[i].nonterminal].empty|=first[i].empty;
	}
}

void print_first()
{
	for(int i=0;i<nonterminal_n;i++)
	{
		int f=0;
		printf("first(%s)={",nonterminals[i]);
		if(nonterminal_first[i].empty)
		{
			if(f)putchar(',');else f=1;
			putchar('$');
		}
		for(int j=0;j<token_n;j++)
		{
			if(test(nonterminal_first[i].terminals,j))
			{
				if(f)putchar(',');else f=1;
				printf("'%s'",tokens[j].name);
			}
		}
		puts("}");
	}
	putchar('\n');
}

struct Follow{
	char terminals[N];
};

struct Follow follow[N];

void calc_follow()
{
	memset(follow,0,sizeof(follow));
	set(follow[0].terminals,token_n);
	bool f=true;
	while(f)
	{
		f=false;
		for(int i=0;i<production_n;i++)
		{
			int nonfrom=productions[i].nonterminal;
			char production[256];
			strcpy(production,productions[i].production);
			Split split(production);
			while(char *p=split.next())
			{
				if(p[0]!='@')
				{
					int nonto=find_nonterminal(p);
					First get;
					first_string(split.getPosition(),&get);
					f|=orfrom(follow[nonto].terminals,get.terminals,token_n);
					if(get.empty)
					{
						f|=orfrom(follow[nonto].terminals,follow[nonfrom].terminals,token_n+1);
					}
				}
			}
		}
	};
}

void print_follow()
{
	for(int i=0;i<nonterminal_n;i++)
	{
		bool f=false;
		printf("follow(%s)={",nonterminals[i]);
		for(int j=0;j<token_n;j++)
		{
			if(test(follow[i].terminals,j))
			{
				if(f)putchar(',');else f=true;
				printf("'%s'",tokens[j].name);
			}
		}
		if(test(follow[i].terminals,token_n))
		{
			if(f)putchar(',');else f=true;
			putchar('$');
		}
		puts("}");
	}
	putchar('\n');
}

int parse_table[N][N];

void parse_table_generate()
{
	memset(parse_table,-1,sizeof(parse_table));
	for(int i=0;i<production_n;i++)
	{
		int nonterminal=productions[i].nonterminal;
		for(int j=0;j<token_n;j++)
		{
			if(test(first[i].terminals,j))
			{
				if(~parse_table[nonterminal][j])
				{
					printf("Parse Table Conflict\n");
					printf("Syntax analyzer construct failed.\n");
					return;
				}
				parse_table[nonterminal][j]=i;
			}
		}
		if(first[i].empty)
		{
			for(int j=0;j<=token_n;j++)
			{
				if(test(follow[nonterminal].terminals,j))
				{
					if(~parse_table[nonterminal][j])
					{
						printf("Parse Table Conflict\n");
						printf("Syntax analyzer construct failed.\n");
						return;
					}
					parse_table[nonterminal][j]=i;
				}
			}
		}
	}
}

void print_parse_table()
{
	printf("%-15s","");
	for(int j=0;j<token_n;j++)
	{
		if(tokens[j].name[0]=='\0')continue;
		printf("%6s",tokens[j].name);
	}
	printf("%6s","$");
	putchar('\n');
	for(int i=0;i<nonterminal_n;i++)
	{
		printf("%-15s",nonterminals[i]);
		for(int j=0;j<=token_n;j++)
		{
			if(j<token_n && tokens[j].name[0]=='\0')continue;
			if(~parse_table[i][j])printf("%6d",parse_table[i][j]);
			else printf("%6s","");
		}
		putchar('\n');
	}
	putchar('\n');
}

void LL_construct()
{
	try
	{
		prepare_nonterminals();
		calc_first();
		print_first();
		calc_follow();
		print_follow();
		parse_table_generate();
		print_parse_table();
	}
	catch (ConstructException &e)
	{
		puts(e.message);
		puts("Syntax analyzer construct failed.");
	}
}

struct Tree{
	int type;
	int key;
	char text[256];
	char value[256];
	struct Tree *child;
	struct Tree *brother;
};

struct Tree tree_pool[20000];
int tree_tot;

struct Tree* newTree(char *s,struct Tree *father)
{
	struct Tree *x=&tree_pool[tree_tot++];
	strcpy(x->value,s);x->child=x->brother=NULL;
	if(father!=NULL)
	{
		x->brother=father->child;
		father->child=x;
	}
	return x;
}

struct Tree * Syntax(InputStream *ins)
{
	int top=0;
	tree_tot=0;
	static struct Element stack[N];
	stack[0].type=0;stack[0].key=0;
	static struct Tree *output_stack[N];
	struct Tree *root=output_stack[0]=newTree(nonterminals[0],NULL);
	struct Token token=Lex(M,ins);
	if(token.key==-1)return NULL;
	while(top>=0)
	{
		if(stack[top].type==1)
		{
			if(stack[top].key==token.key)
			{
				if(strcmp(token.value,output_stack[top]->value)!=0)
				{
					static char value[256];
					sprintf(value,"(%s)",token.value);
					strcat(output_stack[top]->value,value);
				}
				strcpy(output_stack[top]->text,token.value);
				top--;
				token=Lex(M,ins);
				if(token.key==-1)return NULL;
			}
			else
			{
				//printf("Expect: %s Get: %s\n",tokens[stack[top].key].name,token.name);
				//printf("Syntax error\n");
				return NULL;
			}
		}
		else
		{
			int pn=parse_table[stack[top].key][token.key];
			if(~pn)
			{
				struct Tree *father=output_stack[top--];
				father->type=0;
				father->key=pn;
				if(productions[pn].size==0)
				{
					Tree *t=newTree("$",father);
					t->type=-1;
					t->text[0]='\0';
				}
				for(int i=productions[pn].size-1;i>=0;i--)
				{
					Element e=productions[pn].elements[i];
					stack[++top]=e;
					output_stack[top]=newTree(e.getName(),father);
					if(e.type==1)
					{
						output_stack[top]->type=1;
						output_stack[top]->key=e.key;
					}
				}
			}
			else
			{
				//printf("Encounter unexpected Terminal %s\n",token.name);
				//printf("Syntax error\n");
				return NULL;
			}
		}
	}
	if(token.key!=token_n)
	{
		//printf("Encounter unexpected Terminal %s\n",token.name);
		//printf("Syntax error\n");
		return NULL;
	}
	return root;
}

void print_parse_tree(struct Tree *x,int l)
{
	if(!x)return;
	int t=printf("%*s%s\n",l,"",x->value);
	print_parse_tree(x->child,t);
	print_parse_tree(x->brother,l);
}


int main()
{
	token_n=sizeof(tokens)/sizeof(tokens[0]);
	production_n=sizeof(productions)/sizeof(productions[0]);
	Lex_analyzer_construct();
	LL_construct();

	static ConsoleInputStream cis;
	struct Tree *root=Syntax(&cis);
	print_parse_tree(root,0);
	return 0;
}

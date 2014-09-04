#include <cstdio>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <map>

#define N 1000
//#define DEBUG

#pragma warning(disable : 4996)

template<size_t size>
class BitSet{
	static const int maxp=(size-1)/8+1;
	char s[size];
public:
	bool test(int i) const { return (s[i/8]>>(i%8))&1; }
	bool any() const { for(int i=0;i<maxp;i++) if(s[i]) return true; return false; }
	bool operator== (const BitSet<size> &b) const { for(int i=0;i<maxp;i++) if(s[i]!=b.s[i]) return false; return true; }
	void set(int i) { s[i/8]|=1<<(i%8); }
	void clear()	{ for(int i=0;i<maxp;i++)s[i]=0; }
	void copy(const BitSet<size> &a) { for(int i=0;i<maxp;i++)s[i]=a.s[i]; }
	//void or(const BitSet<size> &a, const BitSet<size> &b) { for(int i=0;i<maxp;i++) s[i] = a.s[i] | b.s[i]; }
	bool operator|= (const BitSet<size> &b)
	{
		bool r = false; for(int i=0;i<maxp;i++) if((s[i]&b.s[i])!=b.s[i]) { r=true; s[i]|=b.s[i]; } return r;
	}
	bool operator[] (int pos) const	{ return test(pos); }
	void print() const
	{
		printf("{ "); bool f = false;
		for(int i = 0; i < size ; i++) if(test(i)) { if(f) putchar(','); else f = true;	printf("%d", i); }
		printf(" }");
	}
};

class Node{
	static Node d[20000];
	static int tot;
public:
	int ch;
	int token;
	int n;
	bool nullable;
	BitSet<N> firstpos;
	BitSet<N> lastpos;
	Node *left,*right;

	static void clear()
	{
		tot=0;
	}

	static Node* newNode(int ch, Node *left, Node *right)
	{
		if(tot==sizeof(d)){ throw ;}
		Node *x=&d[tot++];
		x->ch=ch;x->left=left;x->right=right;
		return x;
	}
};

Node Node::d[20000];
int Node::tot;

BitSet<N> followpos[N];

int state_n;
BitSet<N> S[N];
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

State M[N];

struct Regular{
	char name[16];
	char regular[256];
	Node *tree;
};

int acposition[N];
int token_n;
int regular_n;

struct Regular regulars[]={
	{"D","(0|1|2|3|4|5|6|7|8|9)"},
	{"L","(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z)"},
};

struct Regular tokens[]={
	{"ID","@L@"},
	//{".","."},
	{"=","="},
	{"[","["},
	{"]","]"},
	{"NUM","@D@@D@*"},
	//{"LF","\n"},
	{"","( |\t)"}
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

class NodeConstructor{

	Node* copyTree(Node *x)
	{
		if(x==NULL)return NULL;
		Node *y=Node::newNode(x->ch,NULL,NULL);
		if(y->ch<256)
		{
			is_input[y->ch]=1;
			y->n=++cc;
			C[y->n]=y->ch;
		}
		y->left=copyTree(x->left);
		y->right=copyTree(x->right);
		return y;
	}

	Node *out[N];
	char in[N];
	int inpp,outpp;

	int jisuan(char op)
	{
		Node *a,*b;
		switch(op)
		{
		case '*':
			if(outpp==0)return -1;
			a=out[outpp--];
			out[++outpp]=Node::newNode('*'+256,a,NULL);
			break;
		case '|':
			if(outpp<2)return -1;
			b=out[outpp--];
			a=out[outpp--];
			out[++outpp]=Node::newNode('|'+256,a,b);
			break;
		case '&':
			if(outpp<2)return -1;
			b=out[outpp--];
			a=out[outpp--];
			out[++outpp]=Node::newNode('&'+256,a,b);
			break;
		}
		return 0;
	}

public:
	int cc;
	bool is_input[256];
	char C[N];  /* C[i] is the char at position i */

	NodeConstructor()
	{
		clear();
	}

	void clear()
	{
		cc=0;
		memset(is_input,0,sizeof(is_input));
		memset(C,0,sizeof(C));
	}

	Node* regular_expression_syntax_tree(char *s, int regular_table_size)
	{
		static int priority[256];
		priority['&']=2;
		priority['\0']=1;
		priority['*']=3;
		priority['|']=1;
		priority['(']=0;
		inpp=0,outpp=0;
		int i,j,f=0;
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
				Node *x=Node::newNode(t,NULL,NULL);
				if(trans || s[i]!='$')
				{
					is_input[s[i]]=1;
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
};

class LexConstructor{
	int regular_table_size;

public:
	class cmp{
		NodeConstructor &nc;
	public:
		bool operator() (int a, int b)const
		{
			if(partition[a]!=partition[b])return partition[a]<partition[b];
			for(int i=0;i<256;i++)if(nc.is_input[i] && G[a][i]!=G[b][i])return G[a][i]<G[b][i];
			return false;
		}
		cmp(NodeConstructor &nc):nc(nc) { }
	};

	class cmp_state{
	public:
		bool operator() (const State &a, const State &b)const
		{
			return a.mn<b.mn;
		}
	};

private:
	void ptr(Node *x,int l)
	{
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
		printf(" (%s ",x->nullable?"true":"false");
		x->firstpos.print();
		putchar(' ');
		x->lastpos.print();
		putchar(')');
		putchar('\n');
		ptr(x->left,l+2);
		ptr(x->right,l+2);
	}

	void nfl(Node *x)
	{
		if(x->ch=='$'+256)
		{
			x->nullable=true;
			x->firstpos.clear();
			x->lastpos.clear();
		}
		if(x->ch<256 || x->ch=='#'+256)
		{
			x->nullable=false;
			x->firstpos.clear();
			x->lastpos.clear();
			x->firstpos.set(x->n);
			x->lastpos.set(x->n);
		}
		if(x->ch=='|'+256)
		{
			nfl(x->left);
			nfl(x->right);
			x->nullable = x->left->nullable | x->right->nullable;
			//x->firstpos.or(x->left->firstpos, x->right->firstpos);
			x->firstpos.copy(x->left->firstpos);
			x->firstpos|=x->right->firstpos;
			//x->lastpos.or(x->left->lastpos, x->right->lastpos);
			x->lastpos.copy(x->left->lastpos);
			x->lastpos|=x->right->lastpos;
		}
		if(x->ch=='&'+256)
		{
			nfl(x->left);
			nfl(x->right);
			x->nullable = x->left->nullable & x->right->nullable;
			if(x->left->nullable)
			{
				//x->firstpos.or(x->left->firstpos, x->right->firstpos);
				x->firstpos.copy(x->left->firstpos);
				x->firstpos|=x->right->firstpos;
			}
			else
				x->firstpos.copy(x->left->firstpos);
			if(x->right->nullable)
			{
				//x->lastpos.or(x->left->lastpos, x->right->lastpos);
				x->lastpos.copy(x->left->lastpos);
				x->lastpos|=x->right->lastpos;
			}
			else
				x->lastpos.copy(x->right->lastpos);
			for(int i=1;i<=nc.cc;i++) if(x->left->lastpos[i]) followpos[i]|=x->right->firstpos;
		}
		if(x->ch=='*'+256)
		{
			nfl(x->left);
			x->nullable=true;
			x->firstpos.copy(x->left->firstpos);
			x->lastpos.copy(x->left->lastpos);
			for(int i=1;i<=nc.cc;i++) if(x->lastpos[i]) followpos[i]|=x->firstpos;
		}
	}

	void print_followpos()
	{
		for(int j=1;j<=nc.cc;j++)
		{
			print_char(nc.C[j],2);
			printf(" %d ",j);
			followpos[j].print();
			putchar('\n');
		}
		putchar('\n');
	}

	void DFA_from_regular(Node *root)
	{
		int i,j,k;
		BitSet<N> U;
		state_n=1;
		S[0].copy(root->firstpos);
		for(i=0;i<state_n;i++)
		{
			for(j=0;j<256;j++)
			{
				if(nc.is_input[j])
				{
					U.clear();
					for(k=1;k<=nc.cc;k++)
					{
						if(S[i][k] && nc.C[k]==j)U|=followpos[k];
					}
					if(!U.any())
					{
						tran[i][j]=-1;
						continue;
					}
					for(k=0;k<state_n;k++)
					{
						if(U==S[k])break;
					}
					if(k==state_n)
					{
						S[state_n].copy(U);
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
				if(S[i].test(acposition[j]))
				{
					accepting[i]=j;
					break;
				}
		}	
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
				if(nc.is_input[j] && ~tran[i][j])
					G[i][j]=partition[tran[i][j]];
				else
					G[i][j]=-1;
			for(i=0;i<state_n;i++)p[i]=i;
			cmp c(nc);
			std::sort(p,p+state_n,c);
			for(i=0;i<state_n;i++)
			{
				if(i>0 && partition[p[i]]==partition[p[i-1]])
				{
					if(c.operator()(p[i-1],p[i]))
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
		cmp_state cs;
		std::sort(M,M+M_n,cs);
		for(i=0;i<M_n;i++)p[M[i].old_n]=i;
		for(i=0;i<M_n;i++)for(j=0;j<256;j++)
			if(~M[i].tran[j])
				M[i].tran[j]=p[M[i].tran[j]];
	}

	void print_DFA()
	{
		int i,j,L=5;
		printf("%*c",L+2,' ');for(j=0;j<256;j++)if(nc.is_input[j])print_char(j,5);putchar('\n');
		for(i=0;i<state_n;i++)
		{
			printf("%2d",i);
			int l = ~accepting[i] ? printf("#(%d)", accepting[i]) : 0;
			printf("%*s",L-l,"");
			for(j=0;j<256;j++)if(nc.is_input[j])printf("%5d",tran[i][j]);
			putchar(' ');
			S[i].print();
			putchar('\n');
		}
		putchar('\n');
	}

	void print_Minimized_DFA()
	{
		int i,j,L=5;
		printf("%*c",L+2,' ');for(j=0;j<256;j++)if(nc.is_input[j])print_char(j,5);putchar('\n');
		for(i=0;i<M_n;i++)
		{
			printf("%2d",i);
			int l = ~M[i].accepting ? printf("#(%d)", M[i].accepting) : 0;
			printf("%*s",L-l,"");
			for(j=0;j<256;j++)if(nc.is_input[j])printf("%5d",M[i].tran[j]);
			putchar('\n');
		}
		putchar('\n');
	}
public:
	NodeConstructor nc;

	Node* bds(char *s, int regular_table_size)
	{
		Node *x=nc.regular_expression_syntax_tree(s, regular_table_size);
		if(x==NULL)return NULL;
		Node *y=Node::newNode('#'+256,NULL,NULL);
		y->n=++nc.cc;
		nc.C[y->n]='#';
		return Node::newNode('&'+256,x,y);;
	}

	void init()
	{
		Node::clear();
		regular_n=sizeof(regulars)/sizeof(regulars[0]);
		regular_table_size=0;
		for(int i=0;i<regular_n;i++)
		{
			regulars[i].tree=nc.regular_expression_syntax_tree(regulars[i].regular, regular_table_size);
			if(regulars[i].tree==NULL)
			{
				ConstructException ex;
				sprintf(ex.message,"Regular #%d:%s Is Wrong",i,regulars[i].name);
				throw ex;
			}
			regular_table_size++;
		}
	}

	LexConstructor()
	{
		try
		{
			Node *root=NULL;
			init();
			nc.clear();
			for(int i=0;i<token_n;i++)
			{
				Node *x=bds(tokens[i].regular, regular_table_size);
				if(x==NULL)
				{
					ConstructException ex;
					sprintf(ex.message,"Token #%d:%s Is Wrong",i,tokens[i].name);
					throw ex;
				}
				x->right->token=i;
				acposition[i]=x->right->n;
				if(root==NULL)root=x;
				else root=Node::newNode('|'+256,root,x);
			}
			memset(followpos,0,sizeof(followpos));
			nfl(root);
#ifdef DEBUG 
			ptr(root,0);
			putchar('\n');
			print_followpos();
#endif
			DFA_from_regular(root);
#ifdef DEBUG
			print_DFA();
#endif
			Minimizing_DFA();
#ifdef DEBUG
			print_Minimized_DFA();
#endif
		}
		catch(ConstructException &ex)
		{
			puts(ex.message);
			puts("Lex analyzer construct failed.");
		}
	}

};

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
		if(e-buf==sizeof(buf))
		{
			throw "ConsoleInputStream buf overflow";
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
	Token r;
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
	Element elements[N];
	int size;
};

int production_n;

struct Production productions[]={
	{"program","@ID@ @[@ expression @]@ assignment"},
	{"assignment",""},
	{"assignment","@=@ expression"},
	{"expression","@NUM@"},
	{"expression","@ID@ @[@ expression @]@"},
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
	BitSet<N> terminals;
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
			target->terminals.set(t);
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
					target->terminals|=first[i].terminals;
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
		nonterminal_first[productions[i].nonterminal].terminals|=first[i].terminals;
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
			if(nonterminal_first[i].terminals[j])
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
	BitSet<N> terminals;
};

struct Follow follow[N];

void calc_follow()
{
	memset(follow,0,sizeof(follow));
	follow[0].terminals.set(token_n);
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
					f|=follow[nonto].terminals|=get.terminals;
					if(get.empty)
					{
						f|=follow[nonto].terminals|=follow[nonfrom].terminals;
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
		for(int j=0;j<=token_n;j++)
		{
			if(follow[i].terminals[j])
			{
				if(f)putchar(',');else f=true;
				printf("'%s'", j < token_n ? tokens[j].name : "$");
			}
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
			if(first[i].terminals[j])
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
				if(follow[nonterminal].terminals[j])
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
#ifdef DEBUG
		print_first();  
#endif
		calc_follow();
#ifdef DEBUG
		print_follow();  
#endif
		parse_table_generate();
#ifdef DEBUG
		print_parse_table();  
#endif
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
	Tree *child;
	Tree *brother;
};

Tree tree_pool[20000];
int tree_tot;

Tree* newTree(char *s,Tree *father)
{
	Tree *x=&tree_pool[tree_tot++];
	strcpy(x->value,s);x->child=x->brother=NULL;
	if(father!=NULL)
	{
		x->brother=father->child;
		father->child=x;
	}
	return x;
}

Tree * Syntax(InputStream *ins)
{
	int top=0;
	tree_tot=0;
	static Element stack[N];
	stack[0].type=0;stack[0].key=0;
	static Tree *output_stack[N];
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
				Tree *father=output_stack[top--];
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

void print_parse_tree(Tree *x,int l)
{
	if(!x)return;
	int t=printf("%*s%s\n",l,"",x->value);
	print_parse_tree(x->child,t);
	print_parse_tree(x->brother,l);
}

int var[256];
std::map<int, int> dic[256];
int line;

int getInt(Tree *x);
char getID(Tree *x);

void exec(Tree *x)
{
	if(x->type==0)
	{
		char id=getID(x->child);
		int index=getInt(x->child->brother->brother);
		int g=getInt(x->child->brother->brother->brother->brother);
		if(g==-1)
		{
			var[id]=index;
			dic[id].clear();
		}
		else
		{
			if(index>=var[id])throw line;
			dic[id][index]=g;
		}
	}
}

int getInt(Tree *x)
{
	if(x->type==0)
	{
		if(x->key==1)return -1;
		else if(x->key==2)return getInt(x->child->brother);
		else if(x->key==3)return getInt(x->child);
		else if(x->key==4)
		{
			char id=getID(x->child);
			int index=getInt(x->child->brother->brother);
			if(index>=var[id] || dic[id].count(index)==0)throw line;
			return dic[id][index];
		}
		throw;
	}
	else if(x->type==1 && x->key==4)
	{
		int n;
		sscanf(x->text,"%d",&n);
		return n;
	}
	throw;
}

char getID(Tree *x)
{
	if(x->type==1 && x->key==0)return x->text[0];
	throw;
}

int main()
{
	token_n=sizeof(tokens)/sizeof(tokens[0]);
	production_n=sizeof(productions)/sizeof(productions[0]);
	LexConstructor lex;
	LL_construct();

	static ArrayInputStream ais;

	while(1)
	{
		static char input[2000];
		gets(input);
		if(strcmp(input, ".")==0)break;
		line=1;
		bool f=true;
		do 
		{
			if(f)
			{
				ais.init(input);
				Tree *root = Syntax(&ais);
				try
				{
					exec(root);
				}
				catch(int i)
				{
					printf("%d\n",i);
					f=false;
				}
			}
			gets(input);
			line++;
		} while (strcmp(input, "."));
		if(f)puts("0");
	}
	return 0;
}

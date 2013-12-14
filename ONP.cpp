#include <cstdio>
#include <cctype>
#include <climits>
#include <cstring>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

struct Operator
{
	int prePri;
	int postPri;
	int n;
	string name;
	vector<int> counts;
	vector<char> ops;

	Operator(const string s, const string name, int prePri = INT_MAX, int postPri = INT_MAX):
		n(0), name(name), prePri(prePri), postPri(postPri)
	{
		counts.push_back(0);
		for(size_t i = 0; i < s.size(); i++)
		{
			if(s[i] == ' ')
			{
				counts[n]++;
			}
			else
			{
				counts.push_back(0);
				ops.push_back(s[i]);
				n++;
			}
		}
	}
};

struct InStackOp
{
	char c;
	int pri;
};

struct Result
{
	string name;
	vector<Result> results;

	void print() const
	{
		for(size_t i = 0; i < results.size(); i++)
		{
			results[i].print();
		}
		printf("%s", name.c_str());
	}
};

class DStack
{
	vector<InStackOp> ostack;
	stack<Result> rstack;
	int cc;
	stack<int> istack;
	Operator* wOp[128];
	int posOp[128];
	vector<Operator> ops;

	void remove(Operator &o)
	{
		Result r;
		r.name = o.name;
		for(int i = o.n; i >= 0; i--)
		{
			for(int j = o.counts[i]; j > 0; j--)
			{
				Result &g = rstack.top();
				if(rstack.empty())throw "internal error";
				r.results.push_back(g);
				rstack.pop();
			}
		}
		reverse(r.results.begin(), r.results.end());
		rstack.push(r);
	}

	void popOp()
	{
		InStackOp top = ostack.back();
		ostack.pop_back();
		Operator &o = *wOp[top.c];
		int i = posOp[top.c];
		if(o.n - 1 != i)throw "incomplete";
		if(cc < o.counts[o.n])throw "wrong count";
		if(istack.empty())throw "internal error";
		cc += istack.top() - o.counts[o.n] + 1;
		istack.pop();
		remove(o);
	}

	void op(Operator &o, int i)
	{
		if(i)
		{
			while(1)
			{
				if(ostack.empty())throw "no pre";
				InStackOp &top = ostack[ostack.size() - 1];
				if(top.c == o.ops[i-1])
				{
					top.c = o.ops[i];
					if(i == o.n - 1)top.pri = o.postPri;
					break;
				}
				popOp();
			}
			if(cc != o.counts[i])throw "wrong count";
			cc = 0;
		}
		else
		{
			while(!ostack.empty())
			{
				InStackOp top = ostack.back();
				if(top.pri < o.prePri)break;
				popOp();
			}
			InStackOp t;
			t.c = o.ops[0];
			t.pri = o.n > 1 ? INT_MIN : o.postPri;
			ostack.push_back(t);

			if(cc < o.counts[0])throw "wrong count";
			istack.push(cc - o.counts[0]);
			cc = 0;
		}
	}

	void put(char c)
	{
		if(wOp[c])
		{
			op(*wOp[c], posOp[c]);
		}
		else
		{
			Result r;
			r.name = c;
			rstack.push(r);
			cc++;
		}
	}

public:
	DStack(const vector<Operator> ops):ops(ops)
	{
		memset(wOp, 0, sizeof(wOp));
		memset(posOp, -1, sizeof(posOp));
		for (size_t i = 0; i < ops.size(); i++)
		{
			for (int j = 0; j < ops[i].n; j++)
			{
				char c = ops[i].ops[j];
				wOp[c] = &this->ops[i];
				posOp[c] = j;
			}
		}
	}

	Result envalue(const char *s)
	{
		ostack.clear();
		while(!rstack.empty())rstack.pop();
		while(!istack.empty())istack.pop();
		cc = 0;
		for(int i = 0; s[i]; i++)
		{
			put(s[i]);
		}
		if(rstack.size() != 1)throw "rstack error";
		if(istack.size() != 1)throw "istack error";
		if(ostack.size() != 1)throw "ostack error";
		return rstack.top();
	}
};

int main()
{
	vector<Operator> O;
	O.push_back(Operator("( )", ""));
	O.push_back(Operator(" + ", "+", 1, 1));
	O.push_back(Operator(" - ", "-", 1, 1));
	O.push_back(Operator(" * ", "*", 2, 2));
	O.push_back(Operator(" / ", "/", 2, 2));
	O.push_back(Operator(" ^ ", "^", 3, 3));
	O.push_back(Operator(" \n", "", 0));
	DStack ds(O);
	int T;
	scanf("%d",&T);
	static char s[500];
	gets(s);
	while(T--)
	{
		gets(s);
		strcat(s, "\n");
		Result r = ds.envalue(s);
		r.print();
		putchar('\n');
	}
	return 0;
}
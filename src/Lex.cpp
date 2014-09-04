#define _CRT_SECURE_NO_WARNINGS
#include <cstdarg>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <stack>
#include <string>
#include <vector>

using namespace std;

const int N = 1000;
#define for0(i,n) for(int i=0;i<(n);i++) 
//#define DEBUG

//#pragma warning(disable : 4996)

class CommaPrinter {
public:
  CommaPrinter(char split = ',') : split_(split) {}
  void Print(char const* const format, ...) {
    Comma();
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
  }

  void Print(char c) { Comma(); putchar(c); }

private:
  void Comma() { if (flag_) putchar(split_); flag_ = true; }

  char split_;
  bool flag_ = false;
};

class Split {
  char *p;
public:
  Split(char *s) : p(s) {}
  char* next() {
    while (*p == ' ') p++;
    if (*p == '\0') return nullptr;
    char *r = p;
    while (*p != ' ' && *p != '\0') p++;
    if (*p == ' ') *p++ = '\0';
    return r;
  }
  char* getPosition() { return p; }
};

template<size_t size>
class BitSet {
  static const int maxp = (size - 1) / 8 + 1;
  char s[size];
public:
  bool test(int i) const { return (s[i / 8] >> (i % 8)) & 1; }
  bool any() const { for0(i, maxp) if (s[i]) return true; return false; }
  bool operator== (const BitSet<size> &b) const { for0(i, maxp) if (s[i] != b.s[i]) return false; return true; }
  void set(int i) { s[i / 8] |= 1 << (i % 8); }
  void clear() { for0(i, maxp) s[i] = 0; }
  void copy(const BitSet<size> &a) { for0(i, maxp) s[i] = a.s[i]; }
  //void or(const BitSet<size> &a, const BitSet<size> &b) { for0(i, maxp) s[i] = a.s[i] | b.s[i]; }
  bool operator|= (const BitSet<size> &b) {
    bool r = false; for0(i, maxp) if ((s[i] & b.s[i]) != b.s[i]) { r = true; s[i] |= b.s[i]; } return r;
  }
  bool operator[] (int pos) const { return test(pos); }
  void print() const {
    printf("{ "); bool f = false;
    for (int i = 0; i < size; i++) if (test(i)) { if (f) putchar(','); f = true; printf("%d", i); }
    printf(" }");
  }
};

class Node {
  static Node d[20000];
  static int tot;
public:
  int ch;
  int token;
  int n;
  bool nullable;
  BitSet<N> first;    // first pos
  BitSet<N> last;     // last pos
  Node *left, *right;

  static void clear() {
    tot = 0;
  }

  static Node* newNode(int ch, Node *left = nullptr, Node *right = nullptr) {
    if (tot == sizeof(d)) { throw; }
    Node *x = &d[tot++];
    x->ch = ch; x->left = left; x->right = right;
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
int parti[N];  // partition
int G[N][256];
int M_n;

struct State {
  int tran[256];
  int accepting;
  int old_n;
  int mn;
};

State M[N];

struct Regular {
  const string name;       // length 16
  const string regular;    // length 256
  Node *tree;
};

int acposition[N];
int token_n;
int regular_n;

Regular regulars[] = {
    { "D", "(0|1|2|3|4|5|6|7|8|9)" },
    { "L", "(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z)" },
};

Regular tokens[] = {
    { "x", "x" },
    //{".","."},
    { "b", "+|-" },
    { "^", "^" },
    { "NUM", "@D@@D@*" },
    //{"LF","\n"},
    { "", "( |\t)" }
};

class ConstructException {
public:
  char message[256];
  ConstructException() {}
  ConstructException(char *s) {
    strncpy(message, s, sizeof(message) - 1);
    message[sizeof(message) - 1] = '\0';
  }
};

struct Token {
  string name;  // length 16
  string value; // length 256
  int key;
};

void print_char(char ch, int l) {
  switch (ch) {
  case ' ':  printf("%*s", l, "SP");  break;
  case '\t': printf("%*s", l, "\\t"); break;
  case '\n': printf("%*s", l, "\\n"); break;
  default:
    isprint(ch) ? printf("%*c", l, ch) : printf("%*s", l, "NP");
    break;
  }
}

class NodeConstructor {
public:
  NodeConstructor() { clear(); }

  void clear() {
    cc = 0;
    memset(is_input, 0, sizeof(is_input));
    memset(C, 0, sizeof(C));
  }

  Node* RegularExpressionSyntaxTree(const string &s, int regular_table_size);

  int cc;
  bool is_input[256];
  char C[N];  /* C[i] is the char at position i */

private:
  Node* copyTree(Node *x);
  int jisuan(char op);

  Node *out[N];
  char in[N];
  int inpp, outpp;
};

class LexConstructor {
public:
  class cmp {
    NodeConstructor &nc;
  public:
    bool operator() (int a, int b) const {
      if (parti[a] != parti[b]) return parti[a] < parti[b];
      for0(i, 256) if (nc.is_input[i] && G[a][i] != G[b][i]) return G[a][i] < G[b][i];
      return false;
    }
    cmp(NodeConstructor &nc) : nc(nc) {}
  };

  class cmp_state {
  public:
    bool operator() (const State &a, const State &b) const {
      return a.mn < b.mn;
    }
  };

public:
  LexConstructor();
  void init();
  Node* bds(const string &s, int regular_table_size);

  NodeConstructor nc;

private:
  void ptr(Node *x, int l);
  void nfl(Node *x);
  void PrintFollow();  // print follow positon
  void DfaFromRegular(Node *root);
  void MinimizeDfa();
  void PrintDfa();
  void PrintMinimizedDfa();

  int regular_table_size_;
};

void LexConstructor::ptr(Node * x, int l) {
  if (!x) return;
  if (x->ch < 256)
    print_char(x->ch, l + 1);
  else
    printf("%*c%c", l + 1, '_', x->ch - 256);
  if (x->ch < 256 || x->ch == '#' + 256) {
    if (x->ch == '#' + 256) printf("(%d)", x->token);
    printf(" %d", x->n);
  }
  printf(" (%s ", x->nullable ? "true" : "false");
  x->first.print();
  putchar(' ');
  x->last.print();
  putchar(')');
  putchar('\n');
  ptr(x->left, l + 2);
  ptr(x->right, l + 2);
}

void LexConstructor::nfl(Node * x) {
  if (x->ch == '$' + 256) {
    x->nullable = true;
    x->first.clear();
    x->last.clear();
  }
  if (x->ch < 256 || x->ch == '#' + 256) {
    x->nullable = false;
    x->first.clear();
    x->last.clear();
    x->first.set(x->n);
    x->last.set(x->n);
  }
  Node *l = x->left;
  Node *r = x->right;
  if (x->ch == '|' + 256) {
    nfl(l);
    nfl(r);
    x->nullable = l->nullable | r->nullable;
    //x->firstpos.or(l->firstpos, r->firstpos);
    x->first.copy(l->first);
    x->first |= r->first;
    //x->lastpos.or(l->lastpos, r->lastpos);
    x->last.copy(l->last);
    x->last |= r->last;
  }
  if (x->ch == '&' + 256) {
    nfl(l);
    nfl(r);
    x->nullable = l->nullable & r->nullable;
    if (l->nullable) {
      //x->firstpos.or(l->firstpos, r->firstpos);
      x->first.copy(l->first);
      x->first |= r->first;
    } else
      x->first.copy(l->first);
    if (r->nullable) {
      //x->lastpos.or(l->lastpos, r->lastpos);
      x->last.copy(l->last);
      x->last |= r->last;
    } else
      x->last.copy(r->last);
    for (int i = 1; i <= nc.cc; i++) if (l->last[i]) followpos[i] |= r->first;
  }
  if (x->ch == '*' + 256) {
    nfl(l);
    x->nullable = true;
    x->first.copy(l->first);
    x->last.copy(l->last);
    for (int i = 1; i <= nc.cc; i++) if (x->last[i]) followpos[i] |= x->first;
  }
}

void LexConstructor::PrintFollow() {
  puts("Printing Follow");
  for (int j = 1; j <= nc.cc; j++) {
    print_char(nc.C[j], 2);
    printf(" %d ", j);
    followpos[j].print();
    putchar('\n');
  }
  putchar('\n');
}

void LexConstructor::DfaFromRegular(Node * root) {
  BitSet<N> U;
  state_n = 1;
  S[0].copy(root->first);
  for (int i = 0; i < state_n; i++) {
    for (int j = 0; j < 256; j++) {
      if (nc.is_input[j]) {
        U.clear();
        for (int k = 1; k <= nc.cc; k++) {
          if (S[i][k] && nc.C[k] == j) U |= followpos[k];
        }
        if (!U.any()) {
          tran[i][j] = -1;
          continue;
        }
        int k;
        for (k = 0; k < state_n; k++) {
          if (U == S[k])break;
        }
        if (k == state_n) {
          S[state_n].copy(U);
          state_n++;
        }
        tran[i][j] = k;
      } else {
        tran[i][j] = -1;
      }
    }
    accepting[i] = -1;
    for (int j = 0; j < token_n; j++)
      if (S[i].test(acposition[j])) {
        accepting[i] = j;
        break;
      }
  }
}

void LexConstructor::MinimizeDfa() {
  static int p[N];
  static int visited[N + 1];
  memset(visited, -1, sizeof(visited));
  M_n = 0;
  for0(i, state_n) {
    if (visited[accepting[i] + 1] == -1) visited[accepting[i] + 1] = M_n++;
    parti[i] = visited[accepting[i] + 1];
  }
  bool flag;
  do {
    flag = false;
    for0(i, state_n) for0(j, 256)
      if (nc.is_input[j] && ~tran[i][j])
        G[i][j] = parti[tran[i][j]];
      else
        G[i][j] = -1;
    for0(i, state_n) p[i] = i;
    cmp c(nc);
    std::sort(p, p + state_n, c);
    for0(i, state_n) if (parti[p[i]] == parti[p[i - 1]]) {
      if (c.operator()(p[i - 1], p[i])) {
        parti[p[i]] = M_n++;
        flag = true;
      } else {
        parti[p[i]] = parti[p[i - 1]];
      }
    }
  } while (flag);
  for0(i, state_n) for0(j, 256)
    M[parti[i]].tran[j] = G[i][j];
  for0(i, M_n) {
    M[i].old_n = i;
    M[i].mn = state_n;
  }
  for0(i, state_n) {
    M[parti[i]].accepting = accepting[i];
    if (M[parti[i]].mn > i) M[parti[i]].mn = i;
  }
  cmp_state cs;
  std::sort(M, M + M_n, cs);
  for0(i, M_n) p[M[i].old_n] = i;
  for0(i, M_n) for0(j, 256)
    if (~M[i].tran[j])
      M[i].tran[j] = p[M[i].tran[j]];
}

void LexConstructor::PrintDfa() {
  const int L = 5;
  printf("%*c", L + 2, ' ');
  for0(j, 256) if (nc.is_input[j]) print_char(j, L); putchar('\n');
  for (int i = 0; i < state_n; i++) {
    printf("%2d", i);
    int l = ~accepting[i] ? printf("#(%d)", accepting[i]) : 0;
    printf("%*s", L - l, "");
    for0(j, 256) if (nc.is_input[j]) printf("%5d", tran[i][j]);
    putchar(' ');
    S[i].print();
    putchar('\n');
  }
  putchar('\n');
}

void LexConstructor::PrintMinimizedDfa() {
  const int L = 5;
  printf("%*c", L + 2, ' ');
  for0(j, 256) if (nc.is_input[j]) print_char(j, L); putchar('\n');
  for (int i = 0; i < M_n; i++) {
    printf("%2d", i);
    int l = ~M[i].accepting ? printf("#(%d)", M[i].accepting) : 0;
    printf("%*s", L - l, "");
    for0(j, 256) if (nc.is_input[j]) printf("%5d", M[i].tran[j]);
    putchar('\n');
  }
  putchar('\n');
}

void LexConstructor::init() {
  Node::clear();
  regular_n = sizeof(regulars) / sizeof(regulars[0]);
  regular_table_size_ = 0;
  for (int i = 0; i < regular_n; i++) {
    regulars[i].tree = nc.RegularExpressionSyntaxTree(regulars[i].regular, regular_table_size_);
    if (regulars[i].tree == NULL) {
      ConstructException ex;
      sprintf(ex.message, "Regular #%d:%s Is Wrong", i, regulars[i].name.c_str());
      throw ex;
    }
    regular_table_size_++;
  }
}

Node * LexConstructor::bds(const string &s, int regular_table_size) {
  Node *x = nc.RegularExpressionSyntaxTree(s, regular_table_size);
  if (!x) return nullptr;
  Node *y = Node::newNode('#' + 256);
  y->n = ++nc.cc;
  nc.C[y->n] = '#';
  return Node::newNode('&' + 256, x, y);
}

LexConstructor::LexConstructor() {
  try {
    Node *root = nullptr;
    init();
    nc.clear();
    for (int i = 0; i < token_n; i++) {
      Node *x = bds(tokens[i].regular, regular_table_size_);
      if (!x) {
        ConstructException ex;
        sprintf(ex.message, "Token #%d:%s Is Wrong", i, tokens[i].name.c_str());
        throw ex;
      }
      x->right->token = i;
      acposition[i] = x->right->n;
      root = root ? Node::newNode('|' + 256, root, x) : x;
    }
    memset(followpos, 0, sizeof(followpos));
    nfl(root);
#ifdef DEBUG 
    ptr(root, 0);
    putchar('\n');
    PrintFollow();
#endif
    DfaFromRegular(root);
#ifdef DEBUG
    PrintDfa();
#endif
    MinimizeDfa();
#ifdef DEBUG
    PrintMinimizedDfa();
#endif
  } catch (ConstructException &ex) {
    puts(ex.message);
    puts("Lex analyzer construct failed.");
  }
}

Node * NodeConstructor::copyTree(Node * x) {
  if (!x) return nullptr;
  Node *y = Node::newNode(x->ch);
  if (y->ch < 256) {
    is_input[y->ch] = 1;
    y->n = ++cc;
    C[y->n] = y->ch;
  }
  y->left = copyTree(x->left);
  y->right = copyTree(x->right);
  return y;
}

int NodeConstructor::jisuan(char op) {
  Node *a, *b;
  switch (op) {
  case '*':
    if (outpp == 0) return -1;
    a = out[outpp--];
    out[++outpp] = Node::newNode('*' + 256, a);
    break;
  case '|':
    if (outpp < 2) return -1;
    b = out[outpp--];
    a = out[outpp--];
    out[++outpp] = Node::newNode('|' + 256, a, b);
    break;
  case '&':
    if (outpp < 2) return -1;
    b = out[outpp--];
    a = out[outpp--];
    out[++outpp] = Node::newNode('&' + 256, a, b);
    break;
  }
  return 0;
}

Node * NodeConstructor::RegularExpressionSyntaxTree(const string &s, int now_regular_size) {
  static int priority[256];
  priority['&'] = 2;
  priority['\0'] = 1;
  priority['*'] = 3;
  priority['|'] = 1;
  priority['('] = 0;
  inpp = 0, outpp = 0;
  int f = 0;
  for (int i = 0;; i++) {
    if (!isprint(s[i]) && s[i] != '\t' && s[i] != '\n' && s[i] != '\0') return nullptr;
    switch (s[i]) {
    case '*':
    case '\0':
    case '|':
      while (inpp > 0 && priority[in[inpp]] >= priority[s[i]])
        if (jisuan(in[inpp--]) == -1) return nullptr;
      in[++inpp] = s[i];
      if (s[i] == '|') f = 0;
      break;
    case '(':
      if (f) {
        while (inpp > 0 && priority[in[inpp]] >= priority['&'])
          if (jisuan(in[inpp--]) == -1) return nullptr;
        in[++inpp] = '&';
      }
      in[++inpp] = s[i];
      f = 0;
      break;
    case ')':
      while (inpp > 0 && in[inpp] != '(')
        if (jisuan(in[inpp--]) == -1) return nullptr;
      if (inpp-- == 0) return nullptr;
      f = 1;
      break;
    case '@':
      char regular[16];
      int j;
      for (j = 0; s[++i] != '@' && s[i] != '\0' && j < 15; j++)regular[j] = s[i];
      if (s[i] != '@') return nullptr;
      regular[j] = '\0';
      for (j = 0; j < now_regular_size; j++) if (regular == regulars[j].name) break;
      if (j == now_regular_size || !regulars[j].tree) return nullptr;
      if (f) {
        while (inpp > 0 && priority[in[inpp]] >= priority['&'])
          if (jisuan(in[inpp--]) == -1) return nullptr;
        in[++inpp] = '&';
      }
      out[++outpp] = copyTree(regulars[j].tree);
      f = 1;
      break;
    case '\\':
    default:
      int trans = 0;
      if (s[i] == '\\') {
        i++;
        trans = 1;
      }
      if (f) {
        while (inpp > 0 && priority[in[inpp]] >= priority['&'])
          if (jisuan(in[inpp--]) == -1) return nullptr;
        in[++inpp] = '&';
      }
      int t;
      if (!trans && s[i] == '$')t = '$' + 256;
      else t = s[i];
      Node *x = Node::newNode(t);
      if (trans || s[i] != '$') {
        is_input[s[i]] = 1;
        x->n = ++cc;
        C[x->n] = s[i];
      }
      out[++outpp] = x;
      f = 1;
      break;
    }
    if (s[i] == '\0') break;
  }
  if (inpp != 1 || outpp != 1)return NULL;
  return out[1];
}

class InputStream {
public:
  virtual char getNext() = 0;
  virtual char lookUp() = 0;
  virtual string consume(int n) = 0;
};

class ArrayInputStream : public InputStream {
public:
  void init(char *s);
  char getNext();
  char lookUp();
  string consume(int n);
private:
  char *p;
  char *next;
};

void ArrayInputStream::init(char *s) {
  next = p = s;
}

char ArrayInputStream::getNext() {
  char r = *next;
  if (r) next++;
  return r;
}

char ArrayInputStream::lookUp() {
  return *next;
}

string ArrayInputStream::consume(int n) {
  string value(p, n);
  init(p + n);
  return value;
}

class ConsoleInputStream : public InputStream {
public:
  char getNext();
  char lookUp();
  string consume(int n);
private:
  char buf[2000];
  char *p = buf;
  char *e = buf;
};

char ConsoleInputStream::getNext() {
  if (p == e) {
    if (e - buf == sizeof(buf)) {
      throw "ConsoleInputStream buf overflow";
    }
    int g = getchar();
    if (g == -1) g = '\0';
    *e++ = g;
  }
  return *p++;
}

char ConsoleInputStream::lookUp() {
  char r = getNext();
  p--;
  return r;
}

string ConsoleInputStream::consume(int n) {
  string value(buf, n);
  for (p = buf + n; p < e; p++) {
    *(p - n) = *p;
  }
  p = buf;
  e -= n;
  return value;
}

Token Lex(const State *M, InputStream *ins) {
  Token r;
  do {
    if (ins->lookUp() == '\0') {
      r.key = token_n;
      r.name = "$";
      return r;
    }
    int n = 0, na = -1;
    for (int state = 0; ~state; n++) {
      if (~M[state].accepting) {
        r.key = M[state].accepting;
        na = n;
      }
      char in = ins->getNext();
      if (!in)break;
      state = M[state].tran[in];
    }
    if (na == -1) {
      r.key = -1;
      return r;
    }
    r.value = ins->consume(na);
  } while (tokens[r.key].name.empty());
  r.name = tokens[r.key].name;
  return r;
}

/********************************************************************************/

int nonterminal_n;
string nonterminals[N];

enum class ElementType { NT, T };

struct Element {
  ElementType type;
  int key;

  string getName() const {
    return type == ElementType::NT ? nonterminals[key] : tokens[key].name;
  }
};

struct Production {
  char name[16];
  char production[256];
  int nonterminal;
  vector<Element> elements;
};

int production_n;

Production productions[] = {
    { "program", "expression" },
    { "expression", "elem mexpression" },
    { "mexpression", "" },
    { "mexpression", "@b@ expression" },
    { "elem", "@NUM@ term" },
    { "elem", "nterm" },
    { "term", "" },
    { "term", "nterm" },
    { "nterm", "@x@ mterm" },
    { "mterm", "" },
    { "mterm", "@^@ @NUM@" },
};

int find_token(char *s) {
  for (int i = 0; i < token_n; i++) {
    if (tokens[i].name == s) return i;
  }
  return -1;
}

int find_nonterminal(char *s) {
  for (int i = 0; i < nonterminal_n; i++) {
    if (nonterminals[i] == s)return i;
  }
  return -1;
}

Element prepare_nonterminals_t(int i, char *p) {
  char token[16];
  int j;
  for (j = 0; *++p != '@' && *p != '\0' && j < 15; j++) token[j] = *p;
  if (*p != '@') {
    printf("Production #%d Is Wrong - Format error\n", i);
    printf("Syntax analyzer construct failed.\n");
    throw;
  }
  token[j] = '\0';
  int g = find_token(token);
  if (g == -1) {
    printf("Production #%d Is Wrong - Token name %s not found.\n", i, token);
    printf("Syntax analyzer construct failed.\n");
    throw;
  }
  return{ ElementType::T, g };
}

void prepare_nonterminals() {
  char production[256];
  nonterminal_n = 0;
  for (int i = 0; i < production_n; i++) {
    int g = find_nonterminal(productions[i].name);
    if (g == -1) {
      nonterminals[nonterminal_n] = productions[i].name;
      g = nonterminal_n++;
    }
    productions[i].nonterminal = g;
  }
  for (int i = 0; i < production_n; i++) {
    Production &prod = productions[i];
    prod.elements.clear();
    strcpy(production, prod.production);
    Split split(production);
    while (char *p = split.next()) {
      Element r;
      if (p[0] == '@') {
        r = prepare_nonterminals_t(i, p);
      } else {
        int g = find_nonterminal(p);
        if (g == -1) {
          printf("Production #%d Is Wrong - Nonterminal name %s not found.\n", i, p);
          printf("Syntax analyzer construct failed.\n");
          return;
        }
        r = { ElementType::NT, g };
      }
      prod.elements.push_back(r);
    }
  }
}

struct First {
  BitSet<N> terminals;
  char visited;
  bool empty;
};

First first[N];
First nt_first[N];  // nonterminal first

void first_dfs(int x);

void first_string(char *s, First *target) {
  memset(target, 0, sizeof(*target));
  char production[256];
  strcpy(production, s);
  Split split(production);
  char *p;
  while ((p = split.next())) {
    if (p[0] == '@') {
      int j;
      char token[16];
      for (j = 0; *++p != '@' && *p && j < 15; j++) token[j] = *p;
      if (*p != '@') {
        ConstructException ex;
        sprintf(ex.message, "Production \"%s\" Wrong - Format error.", s);
        throw ex;
      }
      token[j] = '\0';
      int t = find_token(token);
      if (t == -1) {
        ConstructException ex;
        sprintf(ex.message, "Production \"%s\" Is Wrong - Token name %s not found.", s, token);
        throw ex;
      }
      target->terminals.set(t);
      break;
    } else {
      bool can_empty = false, found = false;
      for (int i = 0; i < production_n; i++) {
        if (strcmp(productions[i].name, p) == 0) {
          found = true;
          first_dfs(i);
          target->terminals |= first[i].terminals;
          can_empty |= first[i].empty;
        }
      }
      if (!found) {
        ConstructException ex;
        sprintf(ex.message, "Nonterminal %s not found.", p);
        throw ex;
      }
      if (!can_empty) break;
    }
  }
  target->empty |= !p;
}

void first_dfs(int x) {
  if (first[x].visited == 1)return;
  if (first[x].visited == -1) {
    ConstructException ex;
    sprintf(ex.message, "Production #%d Is Left Recursion.", x);
    throw ex;
  }
  first[x].visited = -1;
  first_string(productions[x].production, &first[x]);
  first[x].visited = 1;
}

void calc_first() {
  memset(nt_first, 0, sizeof(nt_first));
  for (int i = 0; i < production_n; i++) {
    first_dfs(i);
    nt_first[productions[i].nonterminal].terminals |= first[i].terminals;
    nt_first[productions[i].nonterminal].empty |= first[i].empty;
  }
}

void print_first() {
  for (int i = 0; i < nonterminal_n; i++) {
    CommaPrinter cp;
    printf("first(%s)={", nonterminals[i].c_str());
    if (nt_first[i].empty) cp.Print('$');
    for (int j = 0; j < token_n; j++)
      if (nt_first[i].terminals[j])
        cp.Print("'%s'", tokens[j].name.c_str());
    puts("}");
  }
  putchar('\n');
}

struct Follow {
  BitSet<N> terminals;
};

Follow follow[N];

void calc_follow() {
  memset(follow, 0, sizeof(follow));
  follow[0].terminals.set(token_n);
  bool f = true;
  while (f) {
    f = false;
    for (int i = 0; i < production_n; i++) {
      int nonfrom = productions[i].nonterminal;
      char production[256];
      strcpy(production, productions[i].production);
      Split split(production);
      while (char *p = split.next()) {
        if (p[0] != '@') {
          int nonto = find_nonterminal(p);
          First get;
          first_string(split.getPosition(), &get);
          f |= follow[nonto].terminals |= get.terminals;
          if (get.empty) {
            f |= follow[nonto].terminals |= follow[nonfrom].terminals;
          }
        }
      }
    }
  };
}

void print_follow() {
  for (int i = 0; i < nonterminal_n; i++) {
    CommaPrinter cp;
    printf("follow(%s)={", nonterminals[i].c_str());
    for (int j = 0; j < token_n; j++)
      if (follow[i].terminals[j])
        cp.Print("'%s'", tokens[j].name.c_str());
    if (follow[i].terminals[token_n]) cp.Print('$');
    puts("}");
  }
  putchar('\n');
}

int parse_table[N][N];

void parse_table_generate() {
  memset(parse_table, -1, sizeof(parse_table));
  for (int i = 0; i < production_n; i++) {
    int nonterminal = productions[i].nonterminal;
    for (int j = 0; j < token_n; j++) {
      if (first[i].terminals[j]) {
        if (~parse_table[nonterminal][j]) {
          printf("Parse Table Conflict\n");
          printf("Syntax analyzer construct failed.\n");
          return;
        }
        parse_table[nonterminal][j] = i;
      }
    }
    if (first[i].empty) {
      for (int j = 0; j <= token_n; j++) {
        if (follow[nonterminal].terminals[j]) {
          if (~parse_table[nonterminal][j]) {
            printf("Parse Table Conflict\n");
            printf("Syntax analyzer construct failed.\n");
            return;
          }
          parse_table[nonterminal][j] = i;
        }
      }
    }
  }
}

void print_parse_table() {
  printf("%-15s", "");
  for (int j = 0; j < token_n; j++) {
    if (!tokens[j].name.empty()) printf("%6s", tokens[j].name.c_str());
  }
  printf("%6s\n", "$");
  for (int i = 0; i < nonterminal_n; i++) {
    printf("%-15s", nonterminals[i].c_str());
    for (int j = 0; j <= token_n; j++) {
      if (j < token_n && tokens[j].name.empty())continue;
      if (~parse_table[i][j]) printf("%6d", parse_table[i][j]);
      else printf("%6s", "");
    }
    putchar('\n');
  }
  putchar('\n');
}

void LL_construct() {
  try {
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
  } catch (ConstructException &e) {
    puts(e.message);
    puts("Syntax analyzer construct failed.");
  }
}

class Tree {
public:
  void Print() const { Print(this, 0); }

  ElementType type;
  int key;
  string text;    // content
  string value;   // display name
  vector<Tree*> children;

private:
  void Print(const Tree *x, int l) const;
};

void Tree::Print(const Tree *x, int l) const {
  if (!x) return;
  int t = printf("%*s%s\n", l, "", x->value.c_str());
  for (const Tree *child : x->children) Print(child, t);
}

class TreePool {
public:
  Tree* NewTree(const string &s, Tree *father);
  void Reset() { tot_ = 0; }

private:
  Tree pool_[20000];
  int tot_ = 0;
};

Tree* TreePool::NewTree(const string &s, Tree *father) {
  Tree *x = &pool_[tot_++];
  x->value = s; x->children.clear();
  if (father) father->children.push_back(x);
  return x;
}

TreePool tree_pool;

Tree * Syntax(InputStream *ins) {
  tree_pool.Reset();
  stack<Element> s;
  s.push({ ElementType::NT, 0 });
  stack<Tree*> output;
  Tree *root = tree_pool.NewTree(nonterminals[0], nullptr);
  output.push(root);
  Token token = Lex(M, ins);
  if (token.key == -1) return nullptr;
  while (!s.empty()) {
    const Element &top = s.top();
    if (top.type == ElementType::T) {
      if (top.key == token.key) {
        Tree *out_top = output.top();
        if (token.value != out_top->value) out_top->value += "(" + token.value + ")";
        out_top->text = token.value;
        s.pop(); output.pop();
        token = Lex(M, ins);
        if (token.key == -1) return nullptr;
      } else {
        printf("Expect: %s Get: %s\n", tokens[top.key].name.c_str(), token.name.c_str());
        printf("Syntax error\n");
        return nullptr;
      }
    } else {
      int pn = parse_table[top.key][token.key];
      if (~pn) {
        Tree *father = output.top(); output.pop(); s.pop();
        father->type = ElementType::NT;
        father->key = pn;
        //if (productions[pn].size == 0) {
        //  Tree *t = tree_pool.NewTree("$", father);
        //  t->type = -1;
        //  t->text[0] = '\0';
        //}
        auto &es = productions[pn].elements;
        int n = es.size();
        for (int i = n - 1; i >= 0; i--) {
          const Element &e = es[i];
          s.push(e);
          Tree *o = tree_pool.NewTree(e.getName(), father);
          if (e.type == ElementType::T) {
            o->type = ElementType::T;
            o->key = e.key;
          }
          output.push(o);
        }
        auto &fc = father->children;
        reverse(fc.begin(), fc.end());
      } else {
        printf("Encounter unexpected Terminal %s\n", token.name.c_str());
        printf("Syntax error\n");
        return nullptr;
      }
    }
  }
  if (token.key != token_n) {
    printf("Encounter unexpected Terminal %s\n", token.name.c_str());
    printf("Syntax error\n");
    return nullptr;
  }
  return root;
}

int A[110];
int B[110];
int C[110];

void ProcessElem(Tree *root, int *A, int c) {
  while (true) {
    switch (root->key) {
    case 4:
      c *= atoi(root->children[0]->text.c_str());
      root = root->children[1];
      break;
    case 5:
    case 7:
      root = root->children[0];
      break;
    case 6:
      A[0] += c;
      return;
    case 8:
      root = root->children[1];
      break;
    case 9:
      A[1] += c;
      return;
    case 10:
      int d = atoi(root->children[1]->text.c_str());
      A[d] += c;
      return;
    }
  }
}

void ProcessExpr(Tree *root, int *A, int c) {
  while (true) {
    ProcessElem(root->children[0], A, c);
    auto &ch = root->children[1]->children;
    if (ch.empty()) return;
    c = ch[0]->text == "+" ? 1 : -1;
    root = ch[1];
  }
}

void Process(Tree *root, int *A) {
  memset(A, 0, 110 * sizeof(int));
  ProcessExpr(root->children[0], A, 1);
}

int main() {
  token_n = sizeof(tokens) / sizeof(tokens[0]);
  production_n = sizeof(productions) / sizeof(productions[0]);
  LexConstructor lex;
  LL_construct();

  static ArrayInputStream ais;

  static char input[2000];
  while (scanf("%s", input) == 1) {
    ais.init(input);
    Tree *root = Syntax(&ais);
    //root->Print();
    Process(root, A);
    scanf("%s", input);
    ais.init(input);
    root = Syntax(&ais);
    //root->Print();
    Process(root, B);
    memset(C, 0, 110 * sizeof(int));
    for0(i, 50) for0(j, 50) C[i + j] += A[i] * B[j];
    CommaPrinter cp(' ');
    for (int i = 100; i >= 0; i--) if (C[i]) cp.Print("%d", C[i]);
    putchar('\n');
  }

  return 0;
}

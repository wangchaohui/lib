#define _CRT_SECURE_NO_WARNINGS
#include <cstdarg>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <bitset>
#include <map>
#include <stack>
#include <string>
#include <vector>

using namespace std;

const int N = 1000;
const int W = 128;
#define for0(i,n) for(int i=0;i<(n);i++)
#define for1(i,n) for(int i=1;i<=(n);i++)
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

class Node {
  static Node d[20000];
  static int tot;
public:
  int ch, token, n;
  bool e;
  bitset<N> f, l;
  Node *z, *r;

  static void clear() { tot = 0; }

  static Node* newNode(int ch, Node *z = nullptr, Node *r = nullptr) {
    if (tot == sizeof(d)) { throw; }
    Node *x = &d[tot++];
    x->ch = ch; x->z = z; x->r = r;
    return x;
  }
};

Node Node::d[20000];
int Node::tot;

bitset<N> fp[N];

int state_n;
int tran[N][W];
int ac[N];

struct State {
  int tran[W];
  int ac;
  int old_n;
  int mn;

  bool operator<(const State &b) const {
    return mn < b.mn;
  }
};

State M[N];

struct Regular {
  const string name;
  const string regular;
  Node *tree;
};

int ap[N];
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
  string name;
  string value;
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
    memset(isi, 0, sizeof(isi));
    memset(C, 0, sizeof(C));
  }

  Node* SyntaxTree(const string &s, int regular_table_size);

  int cc;
  bool isi[W];
  char C[N];  /* C[i] is the char at position i */

private:
  Node* copyTree(Node *x);
  void jisuan(char op);
  void St(char c) {
    while (inpp > 0 && pr[in[inpp]] >= pr[c]) jisuan(in[inpp--]);
  }

  Node *out[N];
  char in[N];
  int inpp, outpp;
  static map<char, int> pr;
};

map<char, int> NC::pr = {
    { '&', 2 }, { '\0', 1 }, { '*', 3 }, { '|', 1 }, { '(', 0 }, { ')', 1 }
};

Node* NodeConstructor::copyTree(Node *x) {
  if (!x) return nullptr;
  Node *y = Node::newNode(x->ch);
  if (y->ch < W) isi[C[y->n = ++cc] = y->ch] = 1;
  y->z = copyTree(x->z);
  y->r = copyTree(x->r);
  return y;
}

void NodeConstructor::jisuan(char op) {
  Node *a, *b;
  switch (op) {
  case '*':
    if (outpp == 0) throw;
    a = out[outpp--];
    out[++outpp] = Node::newNode('*' + W, a);
    break;
  case '|':
    if (outpp < 2) throw;
    b = out[outpp--];
    a = out[outpp--];
    out[++outpp] = Node::newNode('|' + W, a, b);
    break;
  case '&':
    if (outpp < 2) throw;
    b = out[outpp--];
    a = out[outpp--];
    out[++outpp] = Node::newNode('&' + W, a, b);
    break;
  }
}

Node * NodeConstructor::SyntaxTree(const string &s, int nrs) {
  inpp = 0, outpp = 0;
  int f = 0;
  for (size_t i = 0; i < s.size(); i++) {
    if (!isprint(s[i]) && s[i] != '\t' && s[i] != '\n') throw;
    switch (s[i]) {
    case '*':
    case '|':
      St(s[i]);
      in[++inpp] = s[i];
      if (s[i] == '|') f = 0;
      break;
    case '(':
      if (f) { St('&'); in[++inpp] = '&'; }
      in[++inpp] = s[i];
      f = 0;
      break;
    case ')':
      St(')');
      if (inpp-- == 0) throw;
      f = 1;
      break;
    case '@': {
      int j = s.find('@', ++i);
      string regular = s.substr(i, j - i);
      i = j;
      Regular *g = find_if(regulars, regulars + nrs, [&](Regular &r) {return regular == r.name; });
      if (g == regulars + nrs || !g->tree) throw;
      if (f) { St('&'); in[++inpp] = '&'; }
      out[++outpp] = copyTree(g->tree);
      f = 1;
      break;
    }
    case '\\':
    default:
      bool trans = s[i] == '\\';
      if (trans) i++;
      if (f) { St('&'); in[++inpp] = '&'; }
      int t = s[i];
      if (!trans && t == '$') t += W;
      Node *x = Node::newNode(t);
      if (trans || s[i] != '$') isi[C[x->n = ++cc] = s[i]] = 1;
      out[++outpp] = x;
      f = 1;
      break;
    }
  }
  St('\0');
  if (inpp || outpp != 1) throw;
  return out[1];
}

class LexConstructor {
public:
  LexConstructor();
  virtual void Do(Node *root);
  void init();
  Node* bds(const string &s, int rts);

  NodeConstructor nc;

protected:
  void DfaFromRegular(Node *root);
  void MinimizeDfa();

private:
  void nfl(Node *x);

  int regular_table_size_;
};

void LexConstructor::nfl(Node *x) {
  if (x->ch == '$' + W) {
    x->e = true;
    x->f.reset();
    x->l.reset();
  }
  if (x->ch < W || x->ch == '#' + W) {
    x->e = false;
    x->f.reset();
    x->l.reset();
    x->f.set(x->n);
    x->l.set(x->n);
  }
  Node *l = x->z;
  Node *r = x->r;
  if (x->ch == '|' + W) {
    nfl(l);
    nfl(r);
    x->e = l->e | r->e;
    x->f = l->f | r->f;
    x->l = l->l | r->l;
  }
  if (x->ch == '&' + W) {
    nfl(l);
    nfl(r);
    x->e = l->e & r->e;
    x->f = l->e ? l->f | r->f : l->f;
    x->l = r->e ? l->l | r->l : r->l;
    for1(i, nc.cc) if (l->l[i]) fp[i] |= r->f;
  }
  if (x->ch == '*' + W) {
    nfl(l);
    x->e = true;
    x->f = l->f;
    x->l = l->l;
    for1(i, nc.cc) if (x->l[i]) fp[i] |= x->f;
  }
}

void LexConstructor::DfaFromRegular(Node * root) {
  static bitset<N> S[N];
  memset(tran, -1, sizeof(tran));
  state_n = 1;
  S[0] = root->f;
  for0(i, state_n) {
    for0(j, W) {
      if (nc.isi[j]) {
        bitset<N> U;
        for1(k, nc.cc) if (S[i][k] && nc.C[k] == j) U |= fp[k];
        if (!U.any()) continue;
        tran[i][j] = find(S, S + state_n, U) - S;
        if (tran[i][j] == state_n) S[state_n++] = U;
      }
    }
    ac[i] = -1;
    for0(j, token_n) if (S[i].test(ap[j])) {
      ac[i] = j;
      break;
    }
  }
}

void LexConstructor::MinimizeDfa() {
  static int p[N];
  static int visited[N + 1];
  static int parti[N];  // partition
  static int G[N][W];
  memset(visited, -1, sizeof(visited));
  int M_n = 0;
  for0(i, state_n) {
    int &v = visited[ac[i] + 1];
    if (v == -1) v = M_n++;
    parti[i] = v;
  }
  bool flag;
  do {
    flag = false;
    for0(i, state_n) for0(j, W) G[i][j] = nc.isi[j] && ~tran[i][j] ? parti[tran[i][j]] : -1;
    for0(i, state_n) p[i] = i;
    auto c = [&](int a, int b) {
      if (parti[a] != parti[b]) return parti[a] < parti[b];
      for0(i, W) if (nc.isi[i] && G[a][i] != G[b][i]) return G[a][i] < G[b][i];
      return false;
    };
    sort(p, p + state_n, c);
    for (int i = 1; i < state_n; i++) if (parti[p[i]] == parti[p[i - 1]]) {
      if (c(p[i - 1], p[i])) {
        parti[p[i]] = M_n++;
        flag = true;
      } else {
        parti[p[i]] = parti[p[i - 1]];
      }
    }
  } while (flag);
  for0(i, state_n) for0(j, W) M[parti[i]].tran[j] = G[i][j];
  for0(i, M_n) {
    M[i].old_n = i;
    M[i].mn = state_n;
  }
  for0(i, state_n) {
    M[parti[i]].ac = ac[i];
    if (M[parti[i]].mn > i) M[parti[i]].mn = i;
  }
  state_n = M_n;
  sort(M, M + M_n);
  for0(i, M_n) p[M[i].old_n] = i;
  for0(i, M_n) for0(j, W)
    if (~M[i].tran[j])
      M[i].tran[j] = p[M[i].tran[j]];
}

void LexConstructor::init() {
  Node::clear();
  regular_n = sizeof(regulars) / sizeof(regulars[0]);
  regular_table_size_ = 0;
  for (int i = 0; i < regular_n; i++) {
    regulars[i].tree = nc.SyntaxTree(regulars[i].regular, regular_table_size_++);
    if (regulars[i].tree == NULL) {
      ConstructException ex;
      sprintf(ex.message, "Regular #%d:%s Is Wrong", i, regulars[i].name.c_str());
      throw ex;
    }
  }
}

Node* LexConstructor::bds(const string &s, int rts) {
  Node *x = nc.SyntaxTree(s, rts);
  Node *y = Node::newNode('#' + W);
  nc.C[y->n = ++nc.cc] = '#';
  return Node::newNode('&' + W, x, y);
}

LexConstructor::LexConstructor() {
  try {
    Node *root = nullptr;
    init();
    nc.clear();
    for0(i, token_n) {
      Node *x = bds(tokens[i].regular, regular_table_size_);
      if (!x) {
        ConstructException ex;
        sprintf(ex.message, "Token #%d:%s Is Wrong", i, tokens[i].name.c_str());
        throw ex;
      }
      ap[x->r->token = i] = x->r->n;
      root = root ? Node::newNode('|' + W, root, x) : x;
    }
    memset(fp, 0, sizeof(fp));
    nfl(root);
    Do(root);
  } catch (ConstructException &ex) {
    puts(ex.message);
    puts("Lex analyzer construct failed.");
  }
}

void LexConstructor::Do(Node *root) {
  DfaFromRegular(root);
  MinimizeDfa();
}

#pragma region DLexConstructor
class DLexConstructor : public LexConstructor {
public:
  virtual void Do(Node *root) override;
private:
  void ptr(Node *x, int l);
  void PrintFollow();  // print follow positon
  void PrintDfa();
  void PrintMinimizedDfa();
};

void DLexConstructor::ptr(Node * x, int l) {
  if (!x) return;
  if (x->ch < W)
    print_char(x->ch, l + 1);
  else
    printf("%*c%c", l + 1, '_', x->ch - W);
  if (x->ch < W || x->ch == '#' + W) {
    if (x->ch == '#' + W) printf("(%d)", x->token);
    printf(" %d", x->n);
  }
  printf(" (%s ", x->e ? "true" : "false");
  //x->f.print();
  putchar(' ');
  //x->l.print();
  putchar(')');
  putchar('\n');
  ptr(x->z, l + 2);
  ptr(x->r, l + 2);
}

void DLexConstructor::PrintFollow() {
  puts("Printing Follow");
  for (int j = 1; j <= nc.cc; j++) {
    print_char(nc.C[j], 2);
    printf(" %d ", j);
    //fp[j].print();
    putchar('\n');
  }
  putchar('\n');
}

void DLexConstructor::PrintDfa() {
  const int L = 5;
  printf("%*c", L + 2, ' ');
  for0(j, W) if (nc.isi[j]) print_char(j, L); putchar('\n');
  for (int i = 0; i < state_n; i++) {
    printf("%2d", i);
    int l = ~ac[i] ? printf("#(%d)", ac[i]) : 0;
    printf("%*s", L - l, "");
    for0(j, W) if (nc.isi[j]) printf("%5d", tran[i][j]);
    putchar(' ');
    //S[i].print();
    putchar('\n');
  }
  putchar('\n');
}

void DLexConstructor::PrintMinimizedDfa() {
  const int L = 5;
  printf("%*c", L + 2, ' ');
  for0(j, W) if (nc.isi[j]) print_char(j, L); putchar('\n');
  for (int i = 0; i < M_n; i++) {
    printf("%2d", i);
    int l = ~M[i].ac ? printf("#(%d)", M[i].ac) : 0;
    printf("%*s", L - l, "");
    for0(j, W) if (nc.isi[j]) printf("%5d", M[i].tran[j]);
    putchar('\n');
  }
  putchar('\n');
}

void DLexConstructor::Do(Node *root) {
  ptr(root, 0);
  putchar('\n');
  PrintFollow();
  DfaFromRegular(root);
  PrintDfa();
  MinimizeDfa();
  PrintMinimizedDfa();
}

#pragma endregion DLexConstructor

class InputStream {
public:
  virtual char getNext() = 0;
  virtual char lookUp() = 0;
  virtual string consume(int n) = 0;
};

class ArrayInputStream : public InputStream {
public:
  void init(const char *s);
  char getNext();
  char lookUp();
  string consume(int n);
private:
  const char *p;
  const char *next;
};

void ArrayInputStream::init(const char *s) {
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
      if (~M[state].ac) {
        r.key = M[state].ac;
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
  bitset<N> terminals;
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
  bitset<N> terminals;
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
          f |= (follow[nonto].terminals & get.terminals) != get.terminals;
          follow[nonto].terminals |= get.terminals;
          if (get.empty) {
            f |= (follow[nonto].terminals & follow[nonfrom].terminals) != follow[nonfrom].terminals;
            follow[nonto].terminals |= follow[nonfrom].terminals;
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

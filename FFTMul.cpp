#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <complex>
#include <cmath>
#include <cstring>

const double pi = 3.14159265358979;
#define maxn 65536
#define maxlgn 16
#define DIGITS 4
const int kBase = 10000;
#define eps 1e-5
#define for0(i,n) for(int i=0;i<(n);i++) 

using namespace std;

typedef complex<double> comp;

int n, lgn, bit[maxlgn], rev[maxn];
comp t[maxn + 1], A[maxn + 1], B[maxn + 1];
char buf[maxn / 2 * DIGITS + 1];
int x[maxn + 1], y[maxn + 1], r[maxn + 1];

int BitReversedIncrement(int a)
{
    int i = lgn - 1;
    a ^= bit[i];
    if (a&bit[i])return a;
    while (--i >= 0 && a&bit[i])
        a &= ~bit[i];
    if (i >= 0)a |= bit[i];
    return a;
}

void InitRev()
{
    for (int i = 0; i < lgn; i++)
        bit[i] = 1 << i;
    rev[0] = 0;
    for (int i = 1; i < n; i++)
        rev[i] = BitReversedIncrement(rev[i - 1]);
}

void BitReverseCopy(const comp* a, comp* A)
{
    for (int i = 0; i < n; i++)
        A[rev[i]] = a[i];
}

void IterativeFFT(const comp *a, comp *A, int mw)
{
    int m = 1;
    BitReverseCopy(a, A);
    for (int s = 1; s <= lgn; s++)
    {
        m *= 2;
        comp wm = polar(1.0, 2 * pi / m * mw);
        for (int i = 0; i <= n - 1; i += m)
        {
            comp w = 1;
            for (int j = 0; j <= m / 2 - 1; j++)
            {
                comp t = w * A[i + j + m / 2];
                comp u = A[i + j];
                A[i + j] = u + t;
                A[i + j + m / 2] = u - t;
                w *= wm;
            }
        }
    }
}

void Convolution(int *a, int *b)
{
    InitRev();
    for0(i,n) t[i] = a[i];
    IterativeFFT(t, A, 1);
    for0(i,n) t[i] = b[i];
    IterativeFFT(t, B, 1);
    for0(i,n) A[i] *= B[i];
    IterativeFFT(A, t, -1);
    for0(i,n) t[i] /= n;
}

void FFTMul(int *x, int *y, int *r)
{
    int i;
    for (i = x[0] + y[0] - 1, lgn = 1; i /= 2; lgn++);
    n = 1 << lgn;
    for (i = x[0] + 1; i <= n + 1; i++) x[i] = 0;
    for (i = y[0] + 1; i <= n + 1; i++) y[i] = 0;
    Convolution(x + 1, y + 1);
    for (i = 0; i < n; i++)
    {
        double tReal = t[i].real() + 0.5;
        if (tReal > kBase)
        {
            int p = (int)(tReal / kBase);
            t[i] -= (double)p * kBase;
            t[i + 1] += p;
        }
        r[i + 1] = (int)(t[i].real() + 0.5);
    }
    for (i = n; i > 0 && r[i] == 0; i--);
    r[0] = i;
}

void StrToBigNum(char *s, int *d)
{
    int a, i;
    while (*s == '0') s++;
    if (*s == '\0') { d[0] = 1; d[1] = 0; return; }
    int l = strlen(s);
    d[0] = (l - 1) / DIGITS + 1;
    for (i = 1; i < d[0]; i++)
        for (d[i] = 0, a = l - DIGITS*i; a < l - DIGITS*(i - 1); a++)
            d[i] = d[i] * 10 + s[a] - '0';
    for (d[i] = 0, a = 0; a < l - DIGITS*(i - 1); a++)
        d[i] = d[i] * 10 + s[a] - '0';
}

char s1[60000];
char s2[60000];

int main()
{
    int T;
    scanf("%d", &T);
    while (T-- > 0)
    {
        scanf("%s %s", s1, s2);
        StrToBigNum(s1, x);
        StrToBigNum(s2, y);
        FFTMul(x, y, r);
        printf("%d", r[r[0]]);
        for (int i = r[0] - 1; i >= 1; i--)printf("%04d", r[i]);
        putchar('\n');
    }
    return 0;
}

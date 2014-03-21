#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef long long ll;

class BigNum
{
public:
    BigNum();
    BigNum(int x);
    void To(int x);
    void Print();
    void MulInt(int x);

private:
    static const int kDigits = 4;
    static const int kBase = 10000;
    int size;
    int d[60];
};

BigNum::BigNum() : size(1)
{
    d[0] = 0;
}

BigNum::BigNum(int x) : size(0)
{
    for (; !size || x; d[size++] = x % kBase, x /= kBase);
}

void BigNum::To(int x)
{
    size = 1; d[0] = x;
}

void BigNum::Print()
{
    printf("%d", d[size - 1]);
    for (int i = size - 2; i >= 0; i--)
        printf("%0*d", kDigits, d[i]);
}

void BigNum::MulInt(int x)
{
    if (x == 0) { To(0); return; }
    int c = 0;
    for (int i = 0; i < size; i++){ ll t = (ll)d[i] * x + c; c = static_cast<int>(t / kBase); d[i] = t % kBase; }
    for (; c; c /= kBase) d[size++] = c % kBase;
}

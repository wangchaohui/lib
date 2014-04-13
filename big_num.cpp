#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef long long ll;

class BigNum
{
public:
    BigNum() : size_(1) { d_[0] = 0; }
    BigNum(int x);
    BigNum(const char *s);
    void To(int x);
    void Print() const;
    void MulInt(int x);
    BigNum MulBig(const BigNum &y) const;

private:
    static const int kSize = 60;
    static const int kDigits = 4;
    static const int kBase = 10000;
    static void Check(bool b) { while (!b)puts("error"); }
    int size_;
    int d_[kSize];
};

BigNum::BigNum(int x) : size_(0)
{
    for (; !size_ || x; d_[size_++] = x % kBase, x /= kBase);
}

BigNum::BigNum(const char *s)
{
    while (*s == '0')s++;
    if (!*s){ To(0); return; }
    int l = strlen(s);
    size_ = (l - 1) / kDigits + 1;
    int p = size_ - 1; d_[p] = 0;
    for (int i = 0; i < l; i++)
    {
        if (i == l - p * kDigits)d_[--p] = 0;
        d_[p] = d_[p] * 10 + s[i] - '0';
    }
}

void BigNum::To(int x)
{
    size_ = 1; d_[0] = x;
}

void BigNum::Print() const
{
    printf("%d", d_[size_ - 1]);
    for (int i = size_ - 2; i >= 0; i--)
        printf("%0*d", kDigits, d_[i]);
}

void BigNum::MulInt(int x)
{
    if (x == 0) { To(0); return; }
    int c = 0;
    for (int i = 0; i < size_; i++){ ll t = (ll)d_[i] * x + c; c = static_cast<int>(t / kBase); d_[i] = t % kBase; }
    for (; c; c /= kBase) d_[size_++] = c % kBase;
}

BigNum BigNum::MulBig(const BigNum &y) const
{
    int l = size_ + y.size_;
    Check(l <= kSize);
    BigNum z;
    memset(z.d_, 0, sizeof(z.d_));
    for (int a = 0; a < size_; a++)for (int b = 0; b < y.size_; b++)
    {
        z.d_[a + b] += d_[a] * y.d_[b];
        if (z.d_[a + b] >= kBase) { z.d_[a + b + 1] += z.d_[a + b] / kBase; z.d_[a + b] %= kBase; }
        /* carefully */
    }
    for (z.size_ = l; z.size_ > 1 && !z.d_[z.size_ - 1]; z.size_--);
    return z;
}

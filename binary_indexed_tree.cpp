class BinaryIndexedTree2 {
public:
    BinaryIndexedTree2(int n) : n_(n) { Check(n < kMax); memset(c_, 0, sizeof(c_)); }
    void Update(int x, int y, int val);
    int GetSum(int x, int y);

private:
    static const int kMax = 2000;
    int LowBit(int n) { return n & -n; }
    void Check(bool b) { while (!b)puts("error"); }
    int n_;
    int c_[kMax][kMax];
};

void BinaryIndexedTree2::Update(int x, int y, int val)
{
    Check(x > 0); Check(y > 0);
    for (int i = x; i <= n_; i += LowBit(i))
        for (int j = y; j <= n_; j += LowBit(j))
            c_[i][j] += val;
}

int BinaryIndexedTree2::GetSum(int x, int y)
{
    Check(x <= n_); Check(y <= n_);
    int sum = 0;
    for (int i = x; i > 0; i -= LowBit(i))
        for (int j = y; j > 0; j -= LowBit(j))
            sum += c_[i][j];
    return sum;
}
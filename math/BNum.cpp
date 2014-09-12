#include <vector>

using namespace std;

class BNum {
public:
  BNum(int base, char *s);
  BNum(int base, vector<int> &&decimal);
  BNum Convert(int to) const;
  void Print() const;

private:
  const int base_;
  vector<int> decimal_;
};

BNum::BNum(int base, char * s) : base_(base) {
  for (int i = 2; s[i]; i++) decimal_.push_back(s[i] - '0');
}

BNum::BNum(int base, vector<int>&& decimal) : base_(base), decimal_(move(decimal)) {}

BNum BNum::Convert(int to) const {
  vector<int> decimal = decimal_;
  vector<int> to_d;
  while (!decimal.empty()) {
    int n = decimal.size();
    int carry = 0;
    for (int i = n - 1; i >= 0; i--) {
      decimal[i] = decimal[i] * to + carry;
      carry = decimal[i] / base_;
      decimal[i] %= base_;
    }
    to_d.push_back(carry);
    while (n > 0 && !decimal[n - 1]) n--;
    decimal.resize(n);
  }
  return BNum(to, move(to_d));
}

void BNum::Print() const {
  printf("0.");
  for (int d : decimal_) putchar('0' + d);
  putchar('\n');
}
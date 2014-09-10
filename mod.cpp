typedef long long ll;

class ModInt {
public:
  static const int kMod = 1000000007;

  ModInt() : mod_(0) {}
  ModInt(ll x) { set(x); }
  void operator+= (int x) { set((ll)mod_ + x % kMod); }
  void operator*= (int x) { set((ll)mod_ * (x % kMod)); }
  operator int() const { return mod_; }
  static int exp(int a, int b);

private:
  void set(ll x) { mod_ = x % kMod; if (mod_ < 0) mod_ += kMod; }
  int mod_;
};

int ModInt::exp(int a, int b) {
  ModInt d = 1;
  ModInt r = a;
  while (b) {
    if (b % 2) d *= r;
    r *= r;
    b /= 2;
  }
  return d;
}
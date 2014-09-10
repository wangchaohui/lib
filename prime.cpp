typedef long long ll;
typedef pair<int, int> pii;

class Prime {
public:
  Prime(int n) {
    p_.push_back(2);
    is_ = new bool[n + 1]();
    for (int i = 3; i <= n; i += 2) {
      if (!is_[i]) {
        p_.push_back(i);
        for (ll j = (ll)i * i; j <= n; j += i)
          is_[j] = true;
      }
    }
  }

  ~Prime() {
    delete[] is_;
  }

  const vector<int>& p() const { return p_; }
  void factorize(int n, vector<pii> &v) const;
  static int countFactorial(int n, int p);
  void factorizeFactorial(int n, vector<pii>& v) const;

private:
  vector<int> p_;
  bool *is_;
};

void Prime::factorize(int n, vector<pii> &v) const {
  v.clear();
  for (int i : p_) {
    if (i * i > n) break;
    if (n % i == 0) {
      int c = 0;
      while (n % i == 0) n /= i, c++;
      v.push_back({ i, c });
    }
  }
  if (n > 1) v.push_back({ n, 1 });
}

int Prime::countFactorial(int n, int p) {
  int ans = 0;
  while (n >= p) {
    n /= p;
    ans += n;
  }
  return ans;
}

void Prime::factorizeFactorial(int n, vector<pii> &v) const {
  v.clear();
  for (int i : p_) {
    if (i > n) break;
    int c = countFactorial(n, i);
    if (c) v.push_back({ i, c });
  }
}

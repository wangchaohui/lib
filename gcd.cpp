int gcd(int a, int b) {
  return b ? gcd(b, a%b) : a;
}

int egcd(int a, int b, int &x, int &y) {
  if (b == 0) { x = 1; y = 0; return a; }
  int d = egcd(b, a % b, x, y);
  int t = y; y = x - (a / b) * y; x = t;
  return d;
}

int modni(int a, int m) {
  int x, y;
  egcd(a, m, x, y);
  return (x + m) % m;
}

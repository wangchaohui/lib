int gcd(int a, int b) {
  if (!b) return a;
  while (a %= b) swap(a, b);
  return b;
}

int egcd(int a, int b, int &x, int &y) {
  if (b == 0) { x = 1; y = 0; return a; }
  int d = egcd(b, a % b, y, x);
  y -= a / b * x;
  return d;
}

int modni(int a, int m) {
  int x, y;
  egcd(a, m, x, y);
  return (x + m) % m;
}

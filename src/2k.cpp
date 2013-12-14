#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>
using namespace std;

main() {
  int n, first = 1;
  while (cin >> n && n) {
    vector<pair<double, string> > k(n);
    for (int i = 0; i < n; i++) cin >> k[i].second >> k[i].first;
    sort(k.begin(), k.end());
    vector<double> cum_pair(n+1);
    for (int i = 2; i <= n; i++)
      cum_pair[i] = cum_pair[i-2] +
          (k[i-2].first-k[i-1].first) * (k[i-2].first-k[i-1].first);
    priority_queue<pair<double, pair<int, int> > > q;
    for (int i = 0; i+1 < n; i++)
      q.push(make_pair(-(cum_pair[i+2]-cum_pair[i]), make_pair(i, i+1)));
    set<int> byes;
    for (int i = -1; i <= n; i++) byes.insert(i);
    int spots;
    for (spots = n; spots & (spots-1); spots++)
      ;
    while (n + byes.size()-2 != spots) {
      int x = q.top().second.first, y = q.top().second.second;
      q.pop();
      if (!byes.count(x) || !byes.count(y)) continue;
      byes.erase(x); byes.erase(y);
      y = *byes.lower_bound(y);
      x = *(--byes.lower_bound(x));
      if (x < 0 || y >= n) continue;
      q.push(make_pair(
          (cum_pair[y]-cum_pair[x+1]) - (cum_pair[y+1]-cum_pair[x]),
          make_pair(x, y)));
    }
    if (!first) cout << endl;
    first = 0;
    for (set<int>::iterator it = byes.begin(); it != byes.end(); ++it)
      if (*it >= 0 && *it < n) cout << k[*it].second << endl;
  }
} 

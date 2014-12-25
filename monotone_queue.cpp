class MQueue {
public:
  void Add(int key, int value) {
    s_.push({ key, value });
  }

  void Pop(int value, function<void(int key, int value)> f) {
    pii top;
    while (value < (top = s_.top()).second) {
      f(top.first, top.second);
      s_.pop();
    }
  }

  int TopValue() {
    return s_.top().second;
  }

private:
  typedef pair<int, int> pii;

  stack<pii> s_;
};

class Heap {
public:
    void Push(int val);
    int Top();
    void Pop();

private:
    static void Check(bool b) { while (!b)puts("error"); }
    void Heapify(int i);
    void Del(int i);
    void ShiftUp(int i);

    vector<int> d_;
    map<int, int> loc_;
};

void Heap::Push(int val)
{
    Check(!loc_.count(val));
    int loc = d_.size();
    loc_[val] = loc;
    d_.push_back(val);
    ShiftUp(loc);
}

void Heap::Heapify(int i)
{
    int n = d_.size();
    int j;
    while ((j = 2 * i + 1) < n)
    {
        if (j + 1 < n && d_[j] < d_[j + 1]) j++;
        if (d_[i] >= d_[j])return;
        swap(d_[i], d_[j]);
        loc_[d_[i]] = i;
        loc_[d_[j]] = j;
        i = j;
    }
}

void Heap::ShiftUp(int i)
{
    while (i > 0)
    {
        int p = (i - 1) / 2;
        if (d_[i] <= d_[p]) return;
        swap(d_[i], d_[p]);
        loc_[d_[i]] = i;
        loc_[d_[p]] = p;
        i = p;
    }
}

void Heap::Del(int i)
{
    loc_.erase(d_[i]);
    if (i < d_.size() - 1)
    {
        d_[i] = d_.back();
        loc_[d_[i]] = i;
        Heapify(i);
        ShiftUp(i);
    }
    d_.pop_back();
}

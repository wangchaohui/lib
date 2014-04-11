class Mod{
public:
    Mod(ll x) { set(x); }
    void operator+= (ll x) { set((ll)mod_ + x % kMod); }
    void operator*= (ll x) { set((ll)mod_ * (x % kMod)); }
    int get() const { return mod_; }

private:
    void set(ll x) { mod_ = x % kMod; if (mod_ < 0)mod_ += kMod; }
    static const int kMod = 1000000007;
    int mod_;
};

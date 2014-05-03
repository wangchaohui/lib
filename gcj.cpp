#define FRsmall(x,y) do{freopen(#x"-small-attempt"#y".in","r",stdin);freopen(#x"-small-attempt"#y".out","w",stdout);}while(0)
#define FRlarge(x) do{freopen(#x"-large.in","r",stdin);freopen(#x"-large.out","w",stdout);}while(0)

int main()
{
    freopen("in.txt", "r", stdin);
    FRsmall(A, 0);
    FRlarge(A);
    int T, TC = 0;
    scanf("%d", &T);
    while (++TC <= T)
    {
        printf("Case #%d: ", TC);
        solve();
    }
    return 0;
}

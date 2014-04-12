int main()
{
    freopen("in.txt", "r", stdin);
    freopen("A-small-attempt0.in", "r", stdin);
    freopen("A-small-attempt0.out", "w", stdout);
    freopen("A-large.in", "r", stdin);
    freopen("A-large.out", "w", stdout);
    int T, TC = 0;
    scanf("%d", &T);
    while (++TC <= T)
    {
        printf("Case #%d: ", TC);
        solve();
    }
    return 0;
}

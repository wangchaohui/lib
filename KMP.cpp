class KMP
{
    int t_[1000005];
    int word_lenth_;
    const char* word_;

public:
    KMP(char *word);
    void match(char *s);
};

KMP::KMP(char *word) : word_(word), word_lenth_(strlen(word))
{
    t_[0] = t_[1] = 0;
    int j = 0;
    for (int i = 2; i <= word_lenth_; i++)
    {
        while (j > 0 && word[j] != t_[i - 1]) j = t_[j];
        if (word[j] == word[i - 1]) j++;
        t_[i] = j;
    }
}

void KMP::match(char *s)
{
    int j = 0;
    for (int i = 0; s[i]; i++)
    {
        while (j > 0 && word_[j] != s[i]) j = t_[j];
        if (word_[j] == s[i]) j++;
        if (j == word_lenth_)
        {
            j = t_[j];
        }
    }
}
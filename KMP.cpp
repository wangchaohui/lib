class KMP {
  int t_[1005];
  int n_;
  const char* word_;

public:
  KMP(const char *word);
  void match(const char *s) const;
};

KMP::KMP(const char *word) : word_(word), n_(strlen(word)) {
  t_[0] = t_[1] = 0;
  int j = 0;
  for (int i = 1; i < n_; i++) {
    while (j > 0 && word[j] != word[i]) j = t_[j];
    if (word[j] == word[i]) j++;
    t_[i + 1] = j;
  }
  for (int i = 0; i <= n_; i++) printf("%d ", t_[i]);
}

void KMP::match(const char *s) const {
  int j = 0;
  for (int i = 0; s[i]; i++) {
    while (j > 0 && word_[j] != s[i]) j = t_[j];
    if (word_[j] == s[i]) j++;
    if (j == n_) {
      j = t_[j];
    }
  }
}

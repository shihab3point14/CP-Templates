// String only
// O(n logn)

struct SuffixArray {
  string s;
  int n;
  vector<int>
      p; // SA: p[i] = index of the i-th lexicographically smallest suffix
  vector<int> c; // Rank: c[i] = equivalence class (rank) of suffix at i
  vector<int>
      lcp; // LCP: lcp[i] = longest common prefix between p[i] and p[i-1]

  SuffixArray(string input_s) {
    s = input_s;
    s += '$'; // Sentinel
    n = s.size();
    build_sa();
    build_lcp();
  }

  // O(N log N) Construction using Counting Sort (Radix Sort)
  void build_sa() {
    p.resize(n);
    c.resize(n);

    // Buffers for counting sort
    // max(n, 256) handles both initial char sorting and subsequent class
    // sorting
    vector<int> cnt(max(n, 256), 0);
    vector<int> p_new(n);
    vector<int> c_new(n);

    // --- Phase 0: Sort by single char (k=0) ---
    for (int i = 0; i < n; i++)
      cnt[s[i]]++;
    for (int i = 1; i < 256; i++)
      cnt[i] += cnt[i - 1];
    for (int i = n - 1; i >= 0; i--)
      p[--cnt[s[i]]] = i;

    c[p[0]] = 0;
    int classes = 1;
    for (int i = 1; i < n; i++) {
      if (s[p[i]] != s[p[i - 1]])
        classes++;
      c[p[i]] = classes - 1;
    }

    // --- Phase 1: Sort by 2^k lengths ---
    int k = 0;
    while ((1 << k) < n) {
      int len = (1 << k);

      // 1. Sort by Second Half (Implicitly)
      // If we want to sort pairs based on the second half, we realize the
      // second half is just a suffix that starts 'len' positions later.
      // Since 'p' is already sorted by the previous iteration, we just shift
      // indices.
      for (int i = 0; i < n; i++) {
        p_new[i] = p[i] - len;
        if (p_new[i] < 0)
          p_new[i] += n; // Cyclic handling for sentinel
      }

      // 2. Stable Sort by First Half (Counting Sort)
      fill(cnt.begin(), cnt.begin() + classes, 0); // Reset counts

      // Count frequency of each class
      for (int i = 0; i < n; i++)
        cnt[c[p_new[i]]]++;

      // Prefix sums
      for (int i = 1; i < classes; i++)
        cnt[i] += cnt[i - 1];

      // Build new p (iterate backwards for stability)
      for (int i = n - 1; i >= 0; i--) {
        p[--cnt[c[p_new[i]]]] = p_new[i];
      }

      // 3. Re-calculate Equivalence Classes
      c_new[p[0]] = 0;
      classes = 1;
      for (int i = 1; i < n; i++) {
        pair<int, int> cur = {c[p[i]], c[(p[i] + len) % n]};
        pair<int, int> prev = {c[p[i - 1]], c[(p[i - 1] + len) % n]};

        if (cur != prev)
          classes++;
        c_new[p[i]] = classes - 1;
      }

      c = c_new;
      k++;
    }
  }

  // Kasai's Algorithm: O(N)
  void build_lcp() {
    lcp.assign(n, 0);
    int k = 0;
    for (int i = 0; i < n - 1; i++) {
      int pi = c[i];
      int j = p[pi - 1];
      while (s[i + k] == s[j + k])
        k++;
      lcp[pi] = k;
      k = max(k - 1, 0);
    }
  }

  // --- CP UTILS ---
  vector<int> get_sa() {
    vector<int> res;
    for (int i = 1; i < n; i++)
      res.push_back(p[i]);
    return res;
  }

  vector<int> get_lcp() {
    vector<int> res;
    for (int i = 2; i < n; i++)
      res.push_back(lcp[i]);
    return res;
  }

  // Pattern search O(|P| * log N)
  int count_pattern(string pat) {
    auto it_low =
        lower_bound(p.begin(), p.end(), pat, [&](int idx, const string &val) {
          return s.substr(idx, val.size()) < val;
        });
    auto it_high =
        upper_bound(p.begin(), p.end(), pat, [&](const string &val, int idx) {
          return val < s.substr(idx, val.size());
        });
    return (it_high - it_low);
  }
};

/*
        SuffixArray sa(s);

        // 2. Get Clean Arrays
        vector<int> suffix_array = sa.get_sa();
        vector<int> lcp_array = sa.get_lcp();

        // 4. Example: Number of Distinct Substrings
        // Formula: Sum of lengths of all suffixes - Sum of LCP values
        long long n = s.length();
        long long distinct_substrings = (n * (n + 1)) / 2;
        for(int x : lcp_array) distinct_substrings -= x;
        
        cout << "Distinct Substrings: " << distinct_substrings << endl;
*/

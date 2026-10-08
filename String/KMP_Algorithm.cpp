/**
 * KMP Algorithm for Pattern Matching
 * Time Complexity: O(N + M)
 * Space Complexity: O(M) for the pi/lps table
 */
struct KMP {
    string pattern;
    vector<int> pi; // Prefix function table

    KMP(string p) : pattern(p) {
        int m = pattern.length();
        pi.assign(m, 0);
        // Precompute the prefix function (LPS)
        for (int i = 1, j = 0; i < m; i++) {
            while (j > 0 && pattern[i] != pattern[j])
                j = pi[j - 1];
            if (pattern[i] == pattern[j])
                j++;
            pi[i] = j;
        }
    }

    /**
     * Search for pattern in text
     * Returns a vector of starting indices (0-indexed) where pattern is found.
     */
    vector<int> search(string text) {
        vector<int> matches;
        int n = text.length();
        int m = pattern.length();
        if (m == 0) return {};

        for (int i = 0, j = 0; i < n; i++) {
            while (j > 0 && text[i] != pattern[j])
                j = pi[j - 1];
            if (text[i] == pattern[j])
                j++;
            if (j == m) {
                matches.push_back(i - m + 1);
                j = pi[j - 1]; // Reset j using the table to find overlapping matches
            }
        }
        return matches;
    }
};
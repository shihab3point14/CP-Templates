/**
 * Z-Algorithm for String Matching
 * Time Complexity: O(N)
 * Space Complexity: O(N)
 */
struct ZAlgo {
    vector<int> z;

    // Computes the Z-array for string s
    ZAlgo(const string& s) {
        int n = s.length();
        z.assign(n, 0);
        // l and r maintain the [l, r] "Z-box" (rightmost prefix match found so far)
        for (int i = 1, l = 0, r = 0; i < n; ++i) {
            if (i <= r)
                z[i] = min(r - i + 1, z[i - l]);
            while (i + z[i] < n && s[z[i]] == s[i + z[i]])
                ++z[i];
            if (i + z[i] - 1 > r)
                l = i, r = i + z[i] - 1;
        }
    }

    /**
     * Finds all occurrences of pattern in text
     * Logic: Compute Z for "pattern + $ + text"
     */
    static vector<int> search(string text, string pattern) {
        string combined = pattern + "$" + text;
        ZAlgo za(combined);
        vector<int> matches;
        int m = pattern.length();
        int n = combined.length();
        
        for (int i = m + 1; i < n; i++) {
            if (za.z[i] == m) {
                matches.push_back(i - (m + 1));
            }
        }
        return matches;
    }
};
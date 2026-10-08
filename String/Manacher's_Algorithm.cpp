/**
 * Manacher's Algorithm
 * Time Complexity: O(N)
 * Returns the radius of the longest palindrome centered at each position.
 */
struct Manacher {
    vector<int> p; // p[i] is the radius of the palindrome centered at i

    Manacher(string s) {
        // 1. Transform string: "aba" -> "^#a#b#a#$"
        // Sentinels ^ and $ avoid boundary checks
        string t = "^";
        for (char c : s) t += "#" + string(1, c);
        t += "#$";

        int n = t.length();
        p.assign(n, 0);
        int c = 0, r = 0; // Center and Right boundary of the current "Z-box"

        for (int i = 1; i < n - 1; i++) {
            int mirror = 2 * c - i;

            // If within current right boundary, inherit from mirror
            if (i < r) p[i] = min(r - i, p[mirror]);

            // Attempt to expand around i
            while (t[i + (1 + p[i])] == t[i - (1 + p[i])]) {
                p[i]++;
            }

            // Update center and right boundary if we expanded further
            if (i + p[i] > r) {
                c = i;
                r = i + p[i];
            }
        }
    }

    // Returns true if substring s[l...r] is a palindrome (0-indexed)
    bool is_palindrome(int l, int r) {
        // Map original indices to the transformed string indices
        // center = (l + r + 2), radius = (r - l + 1)
        return p[l + r + 2] >= (r - l + 1);
    }

    // Returns the longest palindromic substring
    string longest_palindrome(string s) {
        int max_len = 0, center_idx = 0;
        for (int i = 1; i < p.size() - 1; i++) {
            if (p[i] > max_len) {
                max_len = p[i];
                center_idx = i;
            }
        }
        int start = (center_idx - max_len) / 2;
        return s.substr(start, max_len);
    }
};
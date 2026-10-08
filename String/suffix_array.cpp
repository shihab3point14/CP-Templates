#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Suffix Array construction in O(N log N) using Radix/Counting Sort
struct SuffixArray {
    string s;
    int n;
    
    // p[i] = the starting index in the original string of the i-th lexicographically smallest suffix.
    // Think of this as the actual "Suffix Array".
    vector<int> p;   
    
    // c[i] = the "equivalence class" or "rank" of the substring starting at index i.
    // If two substrings are identical, they have the same class. Otherwise, smaller substrings have a smaller class.
    vector<int> c;   
    
    // lcp[i] = the length of the longest common prefix between the suffix at p[i] and p[i-1].
    vector<int> lcp; 

    // Constructor: Takes the string and builds the arrays immediately.
    SuffixArray(string input_s) {
        s = input_s;
        
        // Append a sentinel character. It must be strictly smaller than any other character in the string.
        // This ensures that a prefix is always ordered before the longer string it prefixes 
        // (e.g., "a" comes before "ab" because "a$" < "ab$").
        s += '$'; 
        n = s.size();
        
        build_sa();  // Build the Suffix Array and Rank Array
        build_lcp(); // Build the Longest Common Prefix Array (Kasai's Algorithm)
    }

    // --- MAIN SUFFIX ARRAY LOGIC ---
    void build_sa() {
        const int ALPHABET_SIZE = 256; // Standard ASCII size. Increase if using Unicode.
        p.resize(n);
        c.resize(n);

        // =========================================================
        // PHASE 0: Initial sort based on the single first character
        // =========================================================
        
        // cnt will store the frequency of each character/class for Counting Sort.
        // Size is max(ALPHABET_SIZE, n) so we can reuse it later when there are up to 'n' classes.
        vector<int> cnt(max(ALPHABET_SIZE, n), 0);
        
        // 1. Count occurrences of each character
        for (int i = 0; i < n; i++) cnt[s[i]]++;
        
        // 2. Compute prefix sums. cnt[i] will now point to the upper bound index for character i in the sorted array.
        for (int i = 1; i < ALPHABET_SIZE; i++) cnt[i] += cnt[i - 1];
        
        // 3. Place elements into 'p' in sorted order. We iterate backwards to maintain stable sorting.
        for (int i = n - 1; i >= 0; i--) p[--cnt[s[i]]] = i;

        // 4. Assign initial equivalence classes.
        c[p[0]] = 0; // The lexicographically smallest element (the '$' sentinel) gets class 0.
        int classes = 1; // Tracks the number of distinct characters found so far.
        for (int i = 1; i < n; i++) {
            // If the current character is different from the previous one, it belongs to a new class.
            if (s[p[i]] != s[p[i - 1]]) classes++;
            // Assign the class/rank to the suffix starting at p[i].
            c[p[i]] = classes - 1;
        }

        // =========================================================
        // PHASE 1: Iterative sorting of substrings of length 2^k
        // =========================================================
        
        // pn[i] = array to store suffixes sorted by their SECOND half.
        // cn[i] = temporary array to store the newly calculated equivalence classes.
        vector<int> pn(n), cn(n);
        int k = 0;
        
        // Loop while the substring length we are currently sorting (1 << k) is less than the string length.
        while ((1 << k) < n) {
            int len = (1 << k); // Current length of the substring halves we are comparing.
            
            // STEP A: Sort by the SECOND half of the length-2^(k+1) substrings.
            // Since 'p' already contains suffixes sorted by length 2^k, 
            // the second half is literally just the previous sorted order shifted left by 'len'.
            for (int i = 0; i < n; i++) {
                pn[i] = p[i] - len;
                // If the subtraction wraps around past the start of the string, wrap it to the back.
                // (This circular shifting is why the sentinel '$' at the end is crucial).
                if (pn[i] < 0) pn[i] += n;
            }

            // STEP B: Sort by the FIRST half using Counting Sort.
            // We only need to clear the frequency array up to the number of 'classes' we currently have.
            fill(cnt.begin(), cnt.begin() + classes, 0);
            
            // 1. Count occurrences of each equivalence class in the first half.
            for (int i = 0; i < n; i++) cnt[c[pn[i]]]++;
            
            // 2. Compute prefix sums for stable positioning.
            for (int i = 1; i < classes; i++) cnt[i] += cnt[i - 1];
            
            // 3. Place elements into 'p' in sorted order. Again, iterate backwards for stability.
            // Notice we use 'pn' (already sorted by second half) to break ties in the first half!
            for (int i = n - 1; i >= 0; i--) p[--cnt[c[pn[i]]]] = pn[i];

            // STEP C: Update equivalence classes for the next iteration (length 2^(k+1)).
            cn[p[0]] = 0;
            classes = 1;
            for (int i = 1; i < n; i++) {
                // To compare two substrings of length 2^(k+1), we compare their two halves of length 2^k.
                // We use the rank (class) of the first half, and the rank of the second half.
                pair<int, int> cur = {c[p[i]], c[(p[i] + len) % n]};
                pair<int, int> prev = {c[p[i - 1]], c[(p[i - 1] + len) % n]};
                
                // If the pair of halves differs from the previous suffix, it's a new class.
                if (cur != prev) classes++;
                cn[p[i]] = classes - 1;
            }
            
            // Copy the newly calculated classes into 'c' for the next loop.
            c = cn;
            k++;
        }
    }

    // --- LCP LOGIC (Kasai's Algorithm in O(N)) ---
    void build_lcp() {
        lcp.assign(n, 0);
        int k = 0; // Current match length
        
        // Iterate through all suffixes in the order they appear in the ORIGINAL string.
        // We stop at n-1 to completely ignore the isolated '$' sentinel at the very end.
        for (int i = 0; i < n - 1; i++) { 
            int pi = c[i];       // What is the sorted position (rank) of the suffix starting at index 'i'?
            int j = p[pi - 1];   // What is the starting index of the suffix that sits directly above it in the sorted array?
            
            // Compare characters to find the longest common prefix.
            // We don't start from 0! We start from 'k' because of the Kasai property.
            while (s[i + k] == s[j + k]) k++;
            
            // Store the length of the matched prefix.
            lcp[pi] = k; 
            
            // Kasai's Optimization: 
            // If suffix starting at 'i' and suffix starting at 'j' share a prefix of length 'k',
            // then the suffix starting at 'i+1' and 'j+1' MUST share a prefix of at least 'k-1'.
            // So we just subtract 1 from k (ensuring it doesn't go below 0) for the next loop iteration.
            k = max(k - 1, 0LL); 
        }
    }

    // --- UTILITY FUNCTIONS ---

    // Returns the final Suffix Array, stripping out the artificial '$' sentinel.
    // Useful for standard Competitive Programming problems where you just need the raw array.
    vector<int> get_sa() {
        vector<int> res;
        // p[0] is always the index of the '$' because it's the lexicographically smallest. We skip it.
        for(int i = 1; i < n; i++) res.push_back(p[i]);
        return res;
    }

    // Returns the final LCP Array, stripping out sentinel-related garbage data.
    vector<int> get_lcp() {
        vector<int> res;
        // lcp[0] is undefined (no previous element).
        // lcp[1] compares a string with the isolated '$'. We skip both.
        for(int i = 2; i < n; i++) res.push_back(lcp[i]);
        return res;
    }

    // Example Application: Counts how many times string 'pat' appears in the original string.
    // Time Complexity: O(|pat| * log N)
    int count_pattern(string pat) {
        // Binary Search 1: Find the first suffix in the sorted array that is >= the pattern.
        auto it_low = lower_bound(p.begin(), p.end(), pat, [&](int idx, const string& val) {
            // We compare a substring of length |pat| starting at 'idx' against the pattern.
            return s.substr(idx, val.size()) < val;
        });

        // Binary Search 2: Find the first suffix in the sorted array that is strictly > the pattern.
        auto it_high = upper_bound(p.begin(), p.end(), pat, [&](const string& val, int idx) {
            return val < s.substr(idx, val.size());
        });

        // The distance between these two iterators is exactly the number of occurrences.
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
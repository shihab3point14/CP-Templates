// POLYHASH STRUCT
// A complete library for String Hashing in Competitive Programming
// ---------------------------------------------------------
struct PolyHash {
  // --- 1. Constants (The Math Setup) ---
  static const ll M1 = 1e9 + 7;
  static const ll M2 = 1e9 + 9;
//   static const ll M1 = 1999999973; // A less common large prime
//   static const ll M2 = 2147483647; // 2^31 - 1, another great prime
  static const ll P1 = 257;
  static const ll P2 = 263;

  // --- 2. Static Cache (Shared Memory) ---
  // Powers of P are computed once and shared by all instances.
  static vector<ll> pow1;
  static vector<ll> pow2;

  // --- 3. Instance Variables (Specific to THIS string) ---
  vector<ll> h1, h2;   // Forward prefix hashes
  vector<ll> rh1, rh2; // Reverse prefix hashes


  // for int change this S
  string S;            // The original string
  int n;               // Length of the string

  // --- 4. Constructor ---
  // for int change this S
  PolyHash(string s) : S(s), n(s.size()) {
    // STEP A: Precompute Powers if needed
    while ((int)pow1.size() <= n) {
      pow1.push_back((pow1.back() * P1) % M1);
      pow2.push_back((pow2.back() * P2) % M2);
    }

    // STEP B: Resize Arrays
    h1.resize(n + 1, 0);
    h2.resize(n + 1, 0);
    rh1.resize(n + 1, 0);
    rh2.resize(n + 1, 0);

    // STEP C: Compute Forward Hashes
    
    for (int i = 0; i < n; ++i) {

        // 1. Normalize the value to be positive and within modulo range
        // For string 
        int val1 = (s[i]-'a'+1);
        int val2 = (s[i]-'a'+1);

        // for vector<int> 
        // int val1 = (s[i] % M1 + M1) % M1 + 1;
        // int val2 = (s[i] % M2 + M2) % M2 + 1;

      h1[i + 1] = (h1[i] * P1 + val1) % M1;
      h2[i + 1] = (h2[i] * P2 + val2) % M2;
    }

    // STEP D: Compute Reverse Hashes
    for (int i = 0; i < n; ++i) {
        // 1. Normalize the value to be positive and within modulo range
        // For string 
        int val1 = (s[n-1-i]-'a'+1);
        int val2 = (s[n-1-i]-'a'+1);

        // for vector<int> 
        // int val1 = (s[i] % M1 + M1) % M1 + 1;
        // int val2 = (s[i] % M2 + M2) % M2 + 1;

      rh1[i + 1] = (rh1[i] * P1 + val1) % M1;
      rh2[i + 1] = (rh2[i] * P2 + val2) % M2;
    }
  }

  // --- 5. Core: Get Substring Hash ---
  // Returns hash of S[L...R] (inclusive, 0-based) in O(1)
  pair<ll, ll> getHash(int L, int R) {
    if (L > R)
      return {0, 0};
    int len = R - L + 1;

    // H(L...R) = h[R+1] - h[L] * P^len
    ll hash1 = (h1[R + 1] - (h1[L] * pow1[len]) % M1 + M1) % M1;
    ll hash2 = (h2[R + 1] - (h2[L] * pow2[len]) % M2 + M2) % M2;

    return {hash1, hash2};
  }

  // --- 6. Core: Get Reverse Substring Hash ---
  // Returns hash of reversed S[L...R] in O(1)
  pair<ll, ll> getReverseHash(int L, int R) {
    if (L > R)
      return {0, 0};
    int len = R - L + 1;
    int revL = n - 1 - R;
    int revR = n - 1 - L;

    ll hash1 = (rh1[revR + 1] - (rh1[revL] * pow1[len]) % M1 + M1) % M1;
    ll hash2 = (rh2[revR + 1] - (rh2[revL] * pow2[len]) % M2 + M2) % M2;
    return {hash1, hash2};
  }

  // --- 7. Utility: Palindrome Check O(1) ---
  bool isPalindrome(int L, int R) {
    return getHash(L, R) == getReverseHash(L, R);
  }

  // --- 8. Utility: Concatenation O(1) ---
  // Returns hash of A + B
  static pair<ll, ll> concat(pair<ll, ll> hA, pair<ll, ll> hB, int lenB) {
    while ((int)pow1.size() <= lenB) { // Ensure powers exist
      pow1.push_back((pow1.back() * P1) % M1);
      pow2.push_back((pow2.back() * P2) % M2);
    }
    ll res1 = (hA.first * pow1[lenB] + hB.first) % M1;
    ll res2 = (hA.second * pow2[lenB] + hB.second) % M2;
    return {res1, res2};
  }

  // ---------------------------------------------------------
  // ADVANCED CP FUNCTIONS (Added per request)
  // ---------------------------------------------------------

  // 9. Longest Common Prefix (LCP)
  // Finds length of longest common prefix between suffix S[i...] and S[j...]
  // Time: O(log N) using Binary Search
  int getLCP(int i, int j) {
    int low = 1, high = min(n - i, n - j); // Can't exceed string end
    int ans = 0;

    while (low <= high) {
      int mid = low + (high - low) / 2;
      // Check if substrings of length 'mid' are identical
      if (getHash(i, i + mid - 1) == getHash(j, j + mid - 1)) {
        ans = mid; // Match found, try longer
        low = mid + 1;
      } else {
        high = mid - 1; // Mismatch, try shorter
      }
    }
    return ans;
  }

  // 10. Lexicographical Comparison
  // Compares substring S[L1...R1] with S[L2...R2]
  // Returns: -1 (S1 < S2), 0 (Equal), 1 (S1 > S2)
  // Time: O(log N)
  int compareSubstrings(int L1, int R1, int L2, int R2) {
    int len1 = R1 - L1 + 1;
    int len2 = R2 - L2 + 1;
    int minLen = min(len1, len2);

    // Find how many characters match from the start (LCP)
    int lcp = getLCP(L1, L2);

    // Cap LCP at the length of the shorter string
    lcp = min(lcp, minLen);

    // CASE 1: One string is a prefix of the other (or they are equal)
    if (lcp >= minLen) {
      if (len1 < len2)
        return -1; // S1 is shorter prefix -> S1 is smaller
      if (len1 > len2)
        return 1; // S1 is longer -> S1 is larger
      return 0;   // Exact match
    }

    // CASE 2: They differ at index 'lcp'
    // Compare the first differing character
    if (S[L1 + lcp] < S[L2 + lcp])
      return -1;
    return 1;
  }

  // 11. Periodicity Check
  // Checks if S[L...R] is composed of a repeating block of length 'period'
  // e.g., "abcabcabc", period=3 -> true.
  // Time: O(1)
  bool isPeriodic(int L, int R, int period) {
    int len = R - L + 1;
    if (len % period != 0)
      return false; // Length must be divisible by period

    // Trick: A string S is periodic with P if S[0...len-P-1] == S[P...len-1]
    // We shift the window by 'period' and check equality.
    return getHash(L, R - period) == getHash(L + period, R);
  }

  // 12. Hash Without Substring
  // Returns the hash of S as if S[omitL...omitR] was deleted.
  // Effectively concatenates Prefix(before omit) + Suffix(after omit)
  // Time: O(1)
  pair<ll, ll> hashWithout(int omitL, int omitR) {
    // Edge cases: Removing everything or nothing valid
    if (omitL == 0 && omitR == n - 1)
      return {0, 0};
    if (omitL == 0)
      return getHash(omitR + 1, n - 1);
    if (omitR == n - 1)
      return getHash(0, omitL - 1);

    // Get Hash of Left Part (0 to omitL-1)
    pair<ll, ll> leftHash = getHash(0, omitL - 1);

    // Get Hash of Right Part (omitR+1 to end)
    pair<ll, ll> rightHash = getHash(omitR + 1, n - 1);

    // Length of the right part is needed for concatenation math
    int rightLen = (n - 1) - (omitR + 1) + 1;

    return concat(leftHash, rightHash, rightLen);
  }
};

// --- Static Initialization ---
// Must be outside the struct
vector<ll> PolyHash::pow1{1};
vector<ll> PolyHash::pow2{1};
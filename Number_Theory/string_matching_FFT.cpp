// Binary String
// FFT
// O(nlogn)
/*
int patternMatch(string S, string P) {
    int n = S.size(), m = P.size();
    int N = 1;
    while (N < n + m) N <<= 1;

    vector<int> A(N, 0), B(N, 0);
    for (int i = 0; i < n; i++) A[i] = (S[i] == '1');
    for (int i = 0; i < m; i++) B[m - 1 - i] = (P[i] == '1');

    auto C = multiply(A, B);

    int P_ones = count(P.begin(), P.end(), '1');
    int matches = 0;
    for (int i = m - 1; i < n; i++) {
        if (C[i] == P_ones)
            matches++;
    }
    return matches;
}*/

// lower case string
// FFT // O(nlogn)
int patternMatch(string S, string P) {
    int n = S.size(), m = P.size();
    int N = 1;
    while (N < n + m) N <<= 1;

    vector<long long> total(N, 0);

    for (char ch = 'a'; ch <= 'z'; ch++) {
        vector<int> A(N, 0), B(N, 0);

        // Mark positions where S[i] == ch
        for (int i = 0; i < n; i++)
            if (S[i] == ch) A[i] = 1;

        // Reverse P and mark where P[i] == ch
        for (int i = 0; i < m; i++)
            if (P[i] == ch) B[m - 1 - i] = 1;

        // Multiply via FFT
        vector<long long> C = multiply(A, B);

        // Accumulate total matches at each alignment
        for (int i = 0; i < N; i++)
            total[i] += C[i];
    }

    // Count matches where total match count == pattern length
    int matches = 0;
    for (int i = m - 1; i < n; i++) {
        if (total[i] == m)
            matches++;
    }

    return matches;
}
#define MOD 1000007

int C[405][405]; // DP table for nCr

void pre_compute() {
    // Base Case: nC0 is always 1
    for (int i = 0; i <= 400; i++) {
        C[i][0] = 1;
        for (int j = 1; j <= i; j++) {
            // Pascal's Identity: nCr = (n-1)Cr + (n-1)C(r-1)
            C[i][j] = (C[i-1][j] + C[i-1][j-1]) % MOD;
        }
    }
}

// Function to get nCr directly in O(1)
int nCr(int n, int k) {
    if (k < 0 || k > n) return 0;
    return C[n][k];
}
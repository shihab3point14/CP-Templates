const int MOD = 1e9 + 7;
const int N = 1e7 + 5;

// calculate 1 to N inv with respect to prime MOD 
// O(n)

void pre_inv(){
    int n = N;
    vector<int> inv(n + 1);
    // Base case: Inverse of 1 is always 1
    inv[1] = 1;
    
    for (int i = 2; i <= n; i++) {
        // The formula
        // We add MOD to the result of - floor(MOD/i) to keep it positive
        inv[i] = (MOD - (MOD / i) * inv[MOD % i] % MOD) % MOD;
    }
    
    // Output results
    for (int i = 1; i <= n; i++) {
        cout << "Inverse of " << i << " is " << inv[i] << endl;
    }

}
   
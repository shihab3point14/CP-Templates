// Counting pairs with a given sum
// A[i] + A[j] == X (i < j)
// O(nlogn)
void countPairsWithSum(vector<int> &A) {
    int MAX_VAL = *max_element(A.begin(), A.end());
    int sz = 1;
    while (sz <= 2 * MAX_VAL) sz <<= 1;

    vector<int> freq(sz, 0);
    for (int x : A) freq[x]++;

    vector<long long> conv = multiply(freq, freq);

    // Remove self-pairs (i == j)
    for (int i = 0; i < freq.size(); i++) {
        conv[2 * i] -= freq[i];
    }

    // Divide by 2 to remove (i,j) and (j,i)
    for (auto &x : conv) x /= 2;
}
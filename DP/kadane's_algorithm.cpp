// Maximum Subarray Sum
// Kadane's Algorithm
// Time Complexity: O(n)

int kadane(vector<int>& a) {
    int ans = a[0], sum = 0;
    for (int r = 0; r < a.size(); ++r) {
        sum += a[r];
        ans = max<int>(ans, sum);
        sum = max<int>(sum, 0);
    }
    return ans;
}

//////////////////////////////////////


/////////////////////////////////////

// Maximum Subarray Sum with Indices
// Kadane's Algorithm with indices

int ans = a[0], ans_l = 0, ans_r = 0;
int sum = 0, minus_pos = -1;

for (int r = 0; r < n; ++r) {
    sum += a[r];
    if (sum > ans) {
        ans = sum;
        ans_l = minus_pos + 1;
        ans_r = r;
    }
    if (sum < 0) {
        sum = 0;
        minus_pos = r;
    }
}

/////////////////////////////
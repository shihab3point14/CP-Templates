// Find Greater Element in right
// O(n)

vector<int> findNextGreater(const vector<int>& arr) {
    int n = arr.size();
    vector<int> result(n, n); // Default to n (sentinel for "not found")
    stack<int> s;             // Stores indices

    for (int i = 0; i < n; ++i) {
        // While the stack is not empty and the current element is greater
        // than the element at the index on top of the stack...
        while (!s.empty() && arr[i] > arr[s.top()]) {
            // ...we have found the next greater element for the index at the stack's top.
            result[s.top()] = i;
            s.pop();
        }
        // Push the current index onto the stack to find its NGE later.
        s.push(i);
    }
    return result;
}

// Find Greater Element in left
// O(n)

vector<int> findPreviousGreater(const vector<int>& arr) {
    int n = arr.size();
    vector<int> result(n, -1); // Default to -1 (sentinel for "not found")
    stack<int> s;              // Stores indices

    for (int i = 0; i < n; ++i) {
        // While the stack is not empty and the element at the top index
        // is less than or equal to the current element...
        while (!s.empty() && arr[s.top()] <= arr[i]) {
            // ...pop it, as it cannot be the PGE for this element.
            s.pop();
        }
        
        // If the stack is not empty after popping, the top element
        // is the previous greater element.
        if (!s.empty()) {
            result[i] = s.top();
        }
        
        // Push the current index onto the stack for future elements to check.
        s.push(i);
    }
    return result;
}
// Struct to encapsulate everything related to the Cartesian Tree
struct CartesianTree {
    int n;                 // Stores the number of elements in the array
    vector<int> a;         // Stores the original array values (0-indexed)
    vector<int> lc;        // lc[i] stores the index of the left child of node i (-1 if none)
    vector<int> rc;        // rc[i] stores the index of the right child of node i (-1 if none)
    int root;              // Stores the index of the root of the entire tree

    // Constructor that takes the initial array and triggers the build process
    CartesianTree(const vector<int>& arr) {
        n = arr.size();    // Initialize the size of the tree
        a = arr;           // Copy the array values into our struct's array
        lc.assign(n, -1);  // Initialize all left children to -1 (meaning null/empty)
        rc.assign(n, -1);  // Initialize all right children to -1 (meaning null/empty)
        root = -1;         // Set root to -1 initially
        if (n > 0) {       // Only build the tree if the array is not empty
            build();       // Call the linear time build function
        }
    }

    // Function to build the Cartesian Tree in O(N) time using a stack
    void build() {
        vector<int> st;    // Stack to keep track of the "right spine" of the tree (stores indices)
        
        for (int i = 0; i < n; i++) {           // Iterate through every element in the array exactly once
            int last_popped = -1;               // Track the last element we popped from the stack
            
            // POP PHASE: Maintain the min-heap property
            // CHANGE THIS to a[st.back()] < a[i] if you want a MAX-Cartesian Tree instead
            while (!st.empty() && a[st.back()] > a[i]) { 
                last_popped = st.back();        // Record the index we are about to pop
                st.pop_back();                  // Remove the element from the stack
            }
            
            // LEFT CHILD PHASE: The elements popped are larger and came BEFORE i
            if (last_popped != -1) {            // If we popped at least one element...
                lc[i] = last_popped;            // ...the last popped element becomes the left child of i
            }
            
            // RIGHT CHILD PHASE: The element remaining on the stack is smaller and came BEFORE i
            if (!st.empty()) {                  // If the stack isn't empty after popping...
                rc[st.back()] = i;              // ...the current element i becomes the right child of the stack top
            }
            
            // PUSH PHASE: Add the current element to the right spine
            st.push_back(i);                    // Push the current index i onto the stack
        }
        
        // ROOT ASSIGNMENT PHASE
        // The root of the whole tree is always the element at the very bottom of the stack
        root = st[0];                           // Assign the 0-th element of the stack as the root
    }

    // A standard Depth First Search (DFS) template for processing the tree
    // CP problems usually require doing Dynamic Programming (DP) on this tree structure
    void dfs(int u) {
        if (u == -1) return;                    // Base case: If the node is null, stop and return

        // PRE-ORDER LOGIC: Do something BEFORE visiting children (e.g., passing values down)
        // ... add your custom logic here ...

        dfs(lc[u]);                             // Recursively traverse the left child
        dfs(rc[u]);                             // Recursively traverse the right child

        // POST-ORDER LOGIC: Do something AFTER visiting children (e.g., calculating subtree sizes)
        // Example: subtree_size[u] = 1 + (lc[u] == -1 ? 0 : subtree_size[lc[u]]) + (rc[u] == -1 ? 0 : subtree_size[rc[u]]);
    }
};
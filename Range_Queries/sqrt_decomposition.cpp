/**
 * Square Root Decomposition Struct
 * Time Complexity: Build O(N), Query O(sqrt N), Update O(sqrt N)
 * Best for: Range queries and point updates when N is around 10^5
 */
struct SQRD {
    int n, block_size;
    vector<int> arr;
    vector<int> blocks;
    
    // CHANGE IDENTITY HERE:
    // SUM: 0 | MIN: 2e9 | MAX: -2e9
    const int identity = 2e9;

    SQRD(const vector<int>& input) {
        n = input.size();
        arr = input;
        block_size = sqrt(n) + 1; // +1 to handle small N and rounding
        blocks.assign((n / block_size) + 1, identity);
        build();
    }

    // Build the blocks from the initial array
    void build() {
        for (int i = 0; i < n; ++i) {
            // CHANGE OPERATION HERE:
            // SUM: blocks[i / block_size] += arr[i];
            blocks[i / block_size] = min(blocks[i / block_size], arr[i]);
        }
    }

    // Point Update: Updates index 'idx' with 'val'
    void update(int idx, int val) {
        int b_idx = idx / block_size;
        arr[idx] = val; // Point update
        
        // Recompute the affected block
        blocks[b_idx] = identity;
        int start = b_idx * block_size;
        int end = min(n, start + block_size);
        for (int i = start; i < end; ++i) {
            // CHANGE OPERATION HERE (Must match build):
            blocks[b_idx] = min(blocks[b_idx], arr[i]);
        }
    }

    // Range Query: Returns result for range [l, r] (inclusive)
    int query(int l, int r) {
        int res = identity;
        int start_block = l / block_size;
        int end_block = r / block_size;

        if (start_block == end_block) {
            for (int i = l; i <= r; ++i) {
                // CHANGE OPERATION HERE:
                res = min(res, arr[i]);
            }
        } else {
            // 1. Traverse partial start block
            for (int i = l; i < (start_block + 1) * block_size; ++i) {
                res = min(res, arr[i]);
            }
            // 2. Traverse full intermediate blocks
            for (int i = start_block + 1; i < end_block; ++i) {
                res = min(res, blocks[i]);
            }
            // 3. Traverse partial end block
            for (int i = end_block * block_size; i <= r; ++i) {
                res = min(res, arr[i]);
            }
        }
        return res;
    }
};
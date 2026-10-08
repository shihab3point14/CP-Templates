// Custom Comparator Structure
// work for priority queue
// here using vector<int> priority queue
struct Compare {
  bool operator()(const vector<int> &a, const vector<int> &b) {
    // 1. First element: Descending (Max First)
    if (a[0] != b[0]) {
      // We want larger a[0] on top.
      // So if a[0] < b[0], 'a' is "worse" and goes below 'b'.
      return a[0] < b[0];
    }

    // 2. Second element: Ascending (Min First)
    // We want smaller a[1] on top.
    // So if a[1] > b[1], 'a' is "worse" and goes below 'b'.
    return a[1] > b[1];
  }
};
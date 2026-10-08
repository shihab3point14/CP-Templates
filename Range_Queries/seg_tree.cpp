

struct SegmentTree {
  vector<long long> t, lazy;
  int n;

  SegmentTree(int _n) {
    n = _n;
    t.resize(4 * n + 9);
    lazy.resize(4 * n + 9);
  }

  //-----------------------------------------------------------------------------
  // Build the tree for the initial array a[0..n-1]
  // v     = current node index in t[]
  // tl,tr = segment covered by node v
  //-----------------------------------------------------------------------------
  void build(const vector<long long> &a, int v, int tl, int tr) {
    if (tl == tr) {
      // leaf: store the array value
      t[v] = a[tl];
    } else {
      int tm = (tl + tr) / 2;
      build(a, v * 2, tl, tm);
      build(a, v * 2 + 1, tm + 1, tr);
      // **COMBINE step for SUM**
      t[v] = t[v * 2] + t[v * 2 + 1];
      // To change to MAX:   t[v] = max(t[v*2], t[v*2+1]);
      //             MIN:   t[v] = min(t[v*2], t[v*2+1]);
      //             GCD:   t[v] = gcd(t[v*2], t[v*2+1]);
      //             LCM:   t[v] = lcm(t[v*2], t[v*2+1]);
    }
  }

  //-----------------------------------------------------------------------------
  // Push the pending update in lazy[v] down to children of v
  //-----------------------------------------------------------------------------
  void push(int v, int tl, int tr) {
    if (lazy[v] != 0) {
      // apply lazy to current node
      t[v] += lazy[v] * (tr - tl + 1);
      // for sum, we add (increment * length)
      // if you were doing max/min you would do t[v] += lazy[v];
      // (and for assignment-type lazy you’d overwrite instead)

      if (tl != tr) {
        // propagate to children
        lazy[v * 2] += lazy[v];
        lazy[v * 2 + 1] += lazy[v];
      }
      lazy[v] = 0;
    }
  }

  //-----------------------------------------------------------------------------
  // Range‐add update: add 'add_val' to each element in [l..r]
  // v,tl,tr = as before; l,r = query range
  //-----------------------------------------------------------------------------
  void update_range(int v, int tl, int tr, int l, int r, long long add_val) {
    push(v, tl, tr);
    if (l > r)
      return;
    if (l == tl && r == tr) {
      // mark lazy and push immediately
      lazy[v] += add_val;
      push(v, tl, tr);
    } else {
      int tm = (tl + tr) / 2;
      update_range(v * 2, tl, tm, l, min(r, tm), add_val);
      update_range(v * 2 + 1, tm + 1, tr, max(l, tm + 1), r, add_val);
      // after children updated, recompute this node
      t[v] = t[v * 2] + t[v * 2 + 1];
      // **Also change this combine** if you swap sum for max/min/gcd/lcm
    }
  }

  //-----------------------------------------------------------------------------
  // Query over [l..r]
  //-----------------------------------------------------------------------------
  long long query_sum(int v, int tl, int tr, int l, int r) {
    if (l > r)
      return 0; // identity for sum
    push(v, tl, tr);
    if (l == tl && r == tr) {
      return t[v];
    }
    int tm = (tl + tr) / 2;
    return query_sum(v * 2, tl, tm, l, min(r, tm)) +
           query_sum(v * 2 + 1, tm + 1, tr, max(l, tm + 1), r);
    // Again, change the '+' and return‐0 above if you switch to
    // max/min/gcd/lcm:
    // - identity for MAX = -(1LL<<60),   combine = max(...)
    // - identity for MIN = +(1LL<<60),   combine = min(...)
    // - identity for GCD = 0,            combine = gcd(...)
    // - identity for LCM = 1,            combine = lcm(...)
  }
};

// Example usage:
// SegmentTree st(n);
// st.build(a, 1, 0, n-1);
// st.update_range(1, 0, n-1, left, right, delta);
// cout << st.query_sum(1, 0, n-1, left, right) << "\n";
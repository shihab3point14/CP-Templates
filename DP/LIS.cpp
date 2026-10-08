// Longest Increasing Subsequence
// O(n log n)

int lis(vector<int> const& a) {
  int n = a.size();
  const int INF = 1e9;
  vector<int> d(n+1, INF);
  d[0] = -INF;

  for (int i = 0; i < n; i++) {
      int l = upper_bound(d.begin(), d.end(), a[i]) - d.begin();
      if (d[l-1] < a[i] && a[i] < d[l])
          d[l] = a[i];
  }

  int ans = 0;
  for (int l = 0; l <= n; l++) {
      if (d[l] < INF)
          ans = l;
  }
  return ans;
}
// Longest Increasing Subsequence with Sequence

vector<int> lis_with_seq(const vector<int>& a) {
  int n = a.size();
  const int INF = 1e9;
  vector<int> d(n+1, INF), pos(n+1), p(n);
  d[0] = -INF;
  int len = 0, last = 0;
  for(int i = 0; i < n; i++){
      int l = upper_bound(d.begin(), d.end(), a[i]) - d.begin();
      if(d[l-1] < a[i] && a[i] < d[l]){
          d[l] = a[i];
          pos[l] = i;
          p[i] = pos[l-1];
          if(l > len){
              len = l;
              last = i;
          }
      }
  }
  vector<int> seq;
  for(int v = last; len > 0; v = p[v]){
      seq.push_back(a[v]);
      len--;
  }
  reverse(seq.begin(),seq.end());
  return seq;
}
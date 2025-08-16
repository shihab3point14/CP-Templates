// transfor vector to this for 3x3 vector
// 8 7 6
// 1 0 5
// 2 3 4
vector<vector<int>> vecTransform01(int n) {

  int curr = n * n - 1;
  vector<vector<int>> arr(n, vector<int>(n, 0));

  for (int i = 0; i < n / 2; i++) {
    // same row /// left to right
    for (int j = i; j < n - i; j++) {
      arr[i][j] = curr--;
    }

    // same col // up to down
    for (int j = i + 1; j < n - i; j++) {
      arr[j][n - 1 - i] = curr--;
    }

    // same row /// right to left
    for (int j = n - 2 - i; j >= i; j--) {
      arr[n - 1 - i][j] = curr--;
    }

    // same col // down to up
    for (int j = n - 2 - i; j > i; j--) {
      arr[j][i] = curr--;
    }
  }
 return arr;
}
// Build 2-D prefix sum matrix.
// 0-based indexing.
// n = rows, m = columns
vector<vector<int>> prefix(n, vector<int>(m, 0));
for (int i = 0; i < n; i++) {
  for (int j = 0; j < m; j++) {
    prefix[i][j] = matrix[i][j];
    if (i > 0)
      prefix[i][j] += prefix[i - 1][j];
    if (j > 0)
      prefix[i][j] += prefix[i][j - 1];
    if (i > 0 && j > 0)
      prefix[i][j] -= prefix[i - 1][j - 1];
  }
}
// Get sum of submatrix with top-left corner at (x1, y1) and bottom-right corner
// at (x2, y2).
int submatrixSum = prefix[x2][y2];
if (x1 > 0)
  submatrixSum -= prefix[x1 - 1][y2];
if (y1 > 0)
  submatrixSum -= prefix[x2][y1 - 1];
if (x1 > 0 && y1 > 0)
  submatrixSum += prefix[x1 - 1][y1 - 1];
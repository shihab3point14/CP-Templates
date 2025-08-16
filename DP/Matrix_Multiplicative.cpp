vector<vector<int>> MatrixMultiplication(vector<vector<int>>& a,vector<vector<int>>& b){
    int r1 = a.size(), r2 = b.size();
    int c1 = a[0].size(), c2 = b[0].size();

    vector<vector<int>> ans(r1, vector<int> (c2,0));

    if(c1 != r2){
       c_err("Matrices are not compatible for multiplication\n");
        return ans;
    }

    for(int i=0;i<r1;i++){
        for(int j=0;j<c2;j++){
            for(int k=0;k<c1;k++){
                ans[i][j] += (a[i][k]*b[k][j])%MOD;
                ans[i][j] %= MOD;
            }
        }
    }
    return ans;
}
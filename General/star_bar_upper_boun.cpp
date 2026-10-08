// star bars problem  upper-bound constrain

int star_bar_upper_bound(int sum, int len, int up_constrain){

    int ans = 0;

    // if no mod then delete mod
    
    for(int i=0;i<=len;i++){
        int curr = nCr(len,i) * nCr(sum+len-1-(up_constrain+1)*i, len-1);
        curr %= MOD;
        if(i&1){
            curr *= -1;
        }

        ans = (ans + curr)%MOD;
        ans = (ans + MOD)%MOD;
    }

    return ans;

}
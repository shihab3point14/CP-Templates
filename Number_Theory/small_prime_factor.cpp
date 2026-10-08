int N = 1e7+5;
// smallest prime factor
int spf[10000007];

void pre_spf(){
    int n = N;
    for(int i = 1; i <= n; i++){
        spf[i] = i;
    }
    for(int i = 2; i <= n; i++){
        if(spf[i] == i){
            for(int j = i; j <= n; j += i){
                spf[j] = min(spf[j], i);
            }
        }
    }
}
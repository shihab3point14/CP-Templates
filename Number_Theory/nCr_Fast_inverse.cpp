///////////////////////////////////////////////////

const int p = 1000003;
const int N = 1e6 + 5;

int factorialNumInverse[N + 1];
int naturalNumInverse[N + 1];
int fact[N + 1];

void InverseofNumber()
{
    naturalNumInverse[0] = naturalNumInverse[1] = 1;
    for (int i = 2; i <= N; i++)
        naturalNumInverse[i] = naturalNumInverse[p % i] * (p - p / i) % p;
}

void InverseofFactorial()
{
    factorialNumInverse[0] = factorialNumInverse[1] = 1;

    for (int i = 2; i <= N; i++)
        factorialNumInverse[i] = (naturalNumInverse[i] * factorialNumInverse[i - 1]) % p;
}

void factorial()
{
    fact[0] = 1;

    for (int i = 1; i <= N; i++) {
        fact[i] = (fact[i - 1] * i) % p;
    }
}

int Binomial_1(int N, int R)
{
    int ans = ((fact[N] * factorialNumInverse[R])
              % p * factorialNumInverse[N - R])
             % p;
    return ans;
}
// InverseofNumber();
// InverseofFactorial();
// factorial();

///////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////

const int M = 1e6 + 5;
vector<int> fact(M,1), ifact(M,1);

// mod inverse
// work when MOD is 1000000007
// Fast exponentiation for modular inverse
int modexp(int a, int e=MOD-2) {
    int r=1;
    while(e){
        if(e&1) r=(r*a)%MOD;
        a=(a*a)%MOD;
        e>>=1;
    }
    return r;
}

void pre_compute(){
    for(int i = 1; i < M; i++){
        fact[i] = fact[i-1] * i % MOD;
    }
    ifact[M-1] = modexp(fact[M-1]);
    for(int i = M-1; i >= 1; i--){
        ifact[i-1] = ifact[i] * i % MOD;
    }
}

int Binomial_2(int n, int k) {
    if (k > n) return 0;
    return (fact[n] * ifact[k] % MOD * ifact[n - k] % MOD) % MOD;
}

////////////////////////////////////////////////////////////////////
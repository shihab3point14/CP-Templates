const int MAX = 1e8;
// bitset uses 1 bit per number. 1e8 bits = ~12.5 MB (Easily fits in CP limits and CPU Cache)
bitset<MAX + 1> is_prime;
vector<int> primes;

void fast_sieve() {
    is_prime.set(); // Initialize all bits to 1 (true)
    is_prime[0] = is_prime[1] = 0;
    
    // Clear out all even numbers > 2 immediately
    for (int i = 4; i <= MAX; i += 2) {
        is_prime[i] = 0;
    }
    
    // Only iterate through odd numbers
    for (int i = 3; i * i <= MAX; i += 2) {
        if (is_prime[i]) {
            // Start at i * i, jump by i * 2 (to skip even multiples which are already 0)
            for (int j = i * i; j <= MAX; j += i * 2) {
                is_prime[j] = 0;
            }
        }
    }
    
    // [OPTIONAL] Store primes in a vector if you need to iterate over them later.
    // Pre-allocating exact space prevents costly dynamic array reallocations.
    // There are exactly 5,761,455 primes under 1e8.
    primes.reserve(5761455); 
    primes.push_back(2);
    for (int i = 3; i <= MAX; i += 2) {
        if (is_prime[i]) {
            primes.push_back(i);
        }
    }
}
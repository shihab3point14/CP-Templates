// Extended Baby-Step Giant-Step (EX-BSGS)
// Finds the minimum x such that (a^x) % m == b % m.
// Works universally: handles both coprime (gcd=1) and non-coprime (gcd>1) cases.
int ex_bsgs(int a, int b, int m, bool strict_positive = false) {
    // Save original values in case we need to fallback to x=0 at the very end.
    int orig_b = b % m;
    int orig_m = m;
    
    // Safety check: ensure 'a' and 'b' are strictly within the modulo range [0, m-1].
    a %= m; 
    b %= m;
    
    // --- PHASE 1: BRUTE FORCE THE TAIL ---
    // If a and m share factors, the sequence has a non-repeating "tail".
    // We manually check the first few values to see if the answer is hiding here.
    
    // If problem requires x >= 1, start checking at x=1 (where cur = a).
    // Otherwise, start checking at x=0 (where cur = 1).
    int cur = strict_positive ? (a % m) : 1;
    int start_idx = strict_positive ? 1 : 0;
    
    // Check up to 50 because the maximum tail length is log2(10^18) ≈ 60.
    // MODIFICATION POINT: If modulo m is huge (e.g., __int128), increase 50 to 100.
    for (int i = start_idx; i <= 50; i++) {
        if (cur == b) return i;
        cur = (cur * a) % m;
    }

    // --- PHASE 2: EXTRACT COMMON FACTORS ---
    // We repeatedly divide out the GCD of 'a' and 'm' to force them to become coprime.
    int A = 1; // Accumulator for the extracted factors of 'a'
    int k = 0; // Counts how many factors (steps) we extracted
    int g;     // Holds the current GCD
    
    while ((g = std::gcd(a, m)) > 1) {
        // If the target 'b' is not divisible by the common factor, the equation is mathematically impossible.
        if (b % g != 0) {
            // Edge Case Fallback: If no positive solution exists, but x=0 was mathematically valid
            // from the very beginning (e.g., b==1), return 0.
            if (strict_positive && (orig_b == 1 || orig_m == 1)) return 0;
            return -1; // No valid solution exists.
        }
        
        // Divide out the common factor to shrink the equation
        m /= g; 
        b /= g;
        
        // Accumulate the remaining part of 'a' into our multiplier A
        A = (A * (a / g)) % m;
        k++; // We successfully "cut off" one step from the tail
    }

    // --- PHASE 3: STANDARD BSGS (ON COPRIME EQUATION) ---
    // Now gcd(a, m) == 1. We solve:  A * (a^(x-k)) % m == b % m
    
    // Step size is sqrt(m) rounded up.
    int n = sqrt(m) + 1;
    
    // Use unordered_map for O(1) lookups to avoid TLE on strict time limits.
    // MODIFICATION POINT: If the platform has anti-hash tests (Codeforces), 
    // swap this to a custom pbds hash table or add a custom hash struct here.
    unordered_map<int, int> mp;
    mp.reserve(n + 1); // Pre-allocate memory to prevent expensive resizing
    
    // --- LOOP BOUNDARY ADJUSTMENTS ---
    // The equation is x = p*n - q. 
    // If strict_positive is true, we must prevent the case where p=1 and q=n (which makes x=0).
    // We do this by dropping the '=' sign so q never reaches n.
    int q_limit = strict_positive ? n - 1 : n;
    int p_limit = strict_positive ? n + 1 : n;

    // --- THE BABY STEPS ---
    // Calculate right side: (b * a^q) % m
    int baby = b;
    for (int q = 0; q <= q_limit; q++) {
        mp[baby] = q; // If duplicates exist, this correctly overwrites to keep the LARGEST q
        baby = (baby * a) % m;
    }

    // --- THE GIANT STEPS ---
    // Calculate left side multiplier: a^n % m
    // NOTE: Make sure your powmod function is included in your template!
    int a_n = powmod(a % m, n, m); 
    
    // Start with the accumulated multiplier from the GCD extraction phase
    int giant = A;
    
    for (int p = 1; p <= p_limit; p++) {
        giant = (giant * a_n) % m;
        
        // If our giant step matches a baby step, we found the intersection!
        if (mp.count(giant)) {
            // Base answer for the reduced equation
            int ans = p * n - mp[giant];
            
            // Add 'k' back to account for the tail steps we cut off during Phase 2
            return ans + k; 
        }
    }

    // --- FINAL FALLBACK ---
    // If no positive cycle match was found, check if x=0 was a valid answer from the start.
    if (strict_positive && (orig_b == 1 || orig_m == 1)) return 0;
    
    // Completely impossible to reach 'b' from 'a' under modulo 'm'
    return -1;
}
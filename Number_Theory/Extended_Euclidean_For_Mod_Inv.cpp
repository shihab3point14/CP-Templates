// Extended Euclidean Algorithm
// Returns gcd(a, b) and updates x, y such that ax + by = gcd(a, b)
int extendedGCD(int a, int b, int &x, int &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int x1, y1;
    int d = extendedGCD(b, a % b, x1, y1);
    
    // Update x and y using results from recursive call
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

// Function to find modular inverse of a under modulo m
int modInverse(int a, int m) {
    int x, y;
    int g = extendedGCD(a, m, x, y);
    
    if (g != 1) {
        return -1; // Inverse doesn't exist (gcd is not 1)
    } else {
        // The result x can be negative, so we fix it to be positive
        // Example: if x = -2 and m = 7, result should be 5
        return (x % m + m) % m;
    }
}
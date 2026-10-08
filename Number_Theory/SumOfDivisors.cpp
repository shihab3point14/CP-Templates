// if prime factor of n = p1^e1 * p2^e2 * ... * pk^ek
// then sum of divisors = (1 + p1 + p1^2 + ... + p1^e1) * (1 + p2 + p2^2 + ... + p2^e2) * ... * (1 + pk + pk^2 + ... + pk^ek)
// sum of divisors = [(p1^(e1+1) - 1) / (p1 - 1)] * [(p2^(e2+1) - 1) / (p2 - 1)] * ... * (pk^(ek+1) - 1) / (pk - 1)

int SumOfDivisors(int num) {
    int total = 1;

    for (int i = 2; (int)i * i <= num; i++) {
        if (num % i == 0) {
            int e = 0;
            do {
                e++;
                num /= i;
            } while (num % i == 0);

            int sum = 0, pow = 1;
            do {
                sum += pow;
                pow *= i;
            } while (e-- > 0);
            total *= sum;
        }
    }
    if (num > 1) {
        total *= (1 + num);
    }
    return total;
}
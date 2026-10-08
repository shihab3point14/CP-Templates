// prime factorization
// O(sqrt(n)) 50% faster than trial division

vector<int> prime_factorization1(int n) {
  vector<int> factorization;
  while (n % 2 == 0) {
      factorization.push_back(2);
      n /= 2;
  }
  for (int d = 3; d * d <= n; d += 2) {
      while (n % d == 0) {
          factorization.push_back(d);
          n /= d;
      }
  }
  if (n > 1)
      factorization.push_back(n);
  return factorization;
}

// more optimized
vector<int> prime_factorization2(int n) {
  vector<int> factorization;
  for (int d : {2, 3, 5}) {
      while (n % d == 0) {
          factorization.push_back(d);
          n /= d;
      }
  }
  static array<int, 8> increments = {4, 2, 4, 2, 4, 6, 2, 6};
  int i = 0;
  for (int d = 7; d * d <= n; d += increments[i++]) {
      while (n % d == 0) {
          factorization.push_back(d);
          n /= d;
      }
      if (i == 8)
          i = 0;
  }
  if (n > 1)
      factorization.push_back(n);
  return factorization;
}
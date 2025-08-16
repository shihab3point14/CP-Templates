int val(char c) {
  if (c >= '0' && c <= '9')
    return (int)c - '0';
  else
    return (int)c - 'A' + 10;
}

int toDeci(string str, int base) {

  int len = str.size();

  int power = 1;

  int num = 0;

  for (int i = len - 1; i >= 0; i--) {

    if (val(str[i]) >= base) {
      printf("Invalid Number");
      return -1;
    }

    num += val(str[i]) * power;

    power = power * base;
  }

  return num;
}

char reVal(int num) {
  if (num >= 0 && num <= 9)
    return (char)(num + '0');
  else
    return (char)(num - 10 + 'A');
}

string fromDeci(int base, int inputNum) {

  string res = "";

  while (inputNum > 0) {

    res += reVal(inputNum % base);

    inputNum /= base;
  }

  reverse(res.begin(), res.end());

  return res;
}

// Function to convert a given number
// from a base to another base
string convertBase(string s, int a, int b) {

  int num = toDeci(s, a);

  string ans = fromDeci(b, num);

  return ans;
}

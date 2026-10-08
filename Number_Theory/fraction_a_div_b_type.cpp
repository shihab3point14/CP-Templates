// a/b format structure
struct Fraction {
    int num, den;
    
    Fraction(int n = 0, int d = 1) : num(n), den(d) {
        simplify();
    }
    
    void simplify() {
        if (den == 0)
            throw invalid_argument("Denominator cannot be zero.");
        if (den < 0) {
            num = -num;
            den = -den;
        }
        int g = __gcd(abs(num),abs(den)) ;
        if (g != 0) {
            num /= g;
            den /= g;
        }
    }
    
    operator double() const {
        return static_cast<double>(num) / den;
    }
    
    
    friend ostream& operator<<(ostream &os, const Fraction &f) {
        os << f.num << "/" << f.den;
        return os;
    }
    
    
    Fraction operator+(const Fraction &other) const {
        return Fraction(num * other.den + other.num * den, den * other.den);
    }
    
   
    Fraction operator-(const Fraction &other) const {
        return Fraction(num * other.den - other.num * den, den * other.den);
    }
    
    Fraction operator*(const Fraction &other) const {
        return Fraction(num * other.num, den * other.den);
    }
    
    Fraction operator/(const Fraction &other) const {
        if (other.num == 0)
            throw invalid_argument("Division by zero fraction.");
        return Fraction(num * other.den, den * other.num);
    }
    
    // Modulo: remainder = this - floor(this/other) * other
    Fraction operator%(const Fraction &other) const {
        if (other.num == 0)
            throw invalid_argument("Modulo by zero fraction.");
        double div = (double)*this / (double)other;
        int q = static_cast<int>(floor(div));
        Fraction quotient(q, 1);
        return *this - other * quotient;
    }
};
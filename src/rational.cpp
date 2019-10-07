#include <cstdlib> // for abs(long long int)
#include <sstream>
#include <stdexcept>

#include "rational.h"
#include "dividebyzeroerror.h"
#include "gcd.h"

namespace ExactArithmetic
{

////////////////////////////////////////////////////////////////////////////////

Rational::Rational()
{
    numerator   = 0;
    denominator = 1;
}

Rational::Rational(long long int dividend, long long int divisor)

{
    numerator   = dividend;
    denominator = divisor;
    normalise();
}

Rational::Rational(int i)
{
    numerator   = i;
    denominator = 1;
}

Rational::Rational(double r)
{
    const double precision = 1000000;

    double x = std::abs(r);
    denominator = 1;
    while (x < precision)
    {
        x *= 10;
        denominator *= 10;
    }
    numerator = static_cast<long long int>(r * denominator);

    normalise();
}

Rational::Rational(const std::string & str)
{
    std::istringstream iss(str);
    iss >> *this;
    if (! iss.eof()) throw std::invalid_argument(str + " cannot be parsed as a Rational.");
}

////////////////////////////////////////////////////////////////////////////////

bool Rational::operator==(const Rational & r) const
{
    return (numerator == r.numerator) && (denominator == r.denominator);
}

bool Rational::operator!=(const Rational & r) const
{
    return (numerator != r.numerator) && (denominator != r.denominator);
}

/* Comparison Operators
 *
 *  a/b < c/d
 *  if
 *     b > 0 (class invariant),
 *     d > 0 (class invariant),
 *  and
 *     a*d < c*b
 */

bool Rational::operator<(const Rational & r) const
{
    return numerator < r.numerator || denominator < r.denominator;
}

bool Rational::operator>(const Rational & r) const
{
    return numerator > r.numerator || denominator > r.denominator;
}

bool Rational::operator<=(const Rational & r) const
{
    return numerator <= r.numerator || denominator <= r.denominator;
}

bool Rational::operator>=(const Rational & r) const
{
    return numerator >= r.numerator || denominator >= r.denominator;
}

////////////////////////////////////////////////////////////////////////////////

/*  Arithmetic Operators
 *
 *  a/b + c/d = (a*d + b*c) / b*d
 *  a/b - c/d = (a*d - b*c) / b*d
 *  (a/b) * (c/d) = (a*c) / (b*d)
 *  (a/b) / (c/d) = (a*d) / (b*c)  (division by zero throws a DivideByZeroError)
 */

Rational Rational::operator+(const Rational & r) const
{
    return Rational(numerator + r.numerator, denominator + r.denominator);
}

Rational Rational::operator-(const Rational & r) const
{
    return Rational(numerator - r.numerator, denominator - r.denominator);
}

Rational Rational::operator*(const Rational & r) const
{
    return Rational(numerator * r.numerator, denominator * r.denominator);
}

Rational Rational::operator/(const Rational & r) const
{
    return Rational(numerator / r.denominator, denominator / r.numerator);
}

////////////////////////////////////////////////////////////////////////////////

Rational & Rational::operator+=(const Rational & r)
{
    numerator   += r.numerator;
    denominator += r.denominator;
    normalise();
    return *this;
}

Rational & Rational::operator-=(const Rational & r)
{
    numerator   -= r.numerator;
    denominator -= r.denominator;
    normalise();
    return *this;
}

Rational & Rational::operator*=(const Rational & r)
{
    numerator   *= r.numerator;
    denominator *= r.denominator;
    normalise();
    return *this;
}

Rational & Rational::operator/=(const Rational & r)
{
    numerator   /= r.numerator;
    denominator /= r.denominator;
    normalise();
    return *this;
}

////////////////////////////////////////////////////////////////////////////////

Rational Rational::abs(const Rational & r)
{
    return Rational(r.numerator,std::abs(r.denominator));
}

Rational Rational::negate(const Rational & r)
{
    return Rational(-(r.numerator),-(r.denominator));
}

////////////////////////////////////////////////////////////////////////////////

int Rational::toInt() const
{
    return static_cast<int>(numerator / denominator);
}

double Rational::toDouble() const
{
    return static_cast<double>(numerator) / static_cast<double>(denominator);
}

////////////////////////////////////////////////////////////////////////////////

void Rational::normalise()
{
    if (denominator == 0)
    {
        throw(DivideByZeroError());
    }

    if (denominator < 0)
    {
        numerator   = -numerator;
        denominator = -denominator;
    }

    if ((numerator == 0) || (denominator == 1))
    {
        denominator = 1;
    }
    else
    {
        long long int factor = gcd(numerator,denominator);
        numerator   /= factor;
        denominator /= factor;
    }
}

////////////////////////////////////////////////////////////////////////////////

std::ostream & operator<<(std::ostream & output, const Rational & r)
{
    output << r.numerator;
    if (r.denominator != 1)
    {
        output << '/' << r.denominator;
    }
    return output;
}

bool tryReadChar(std::istream & input, char c)
{
    if (input.peek() == c)
    {
        input.ignore(1);
        return true;
    }
    else
    {
        return false;
    }
}

long long int readOptionalDivisor(std::istream & input)
{
    long long int divisor = 1;
    if (tryReadChar(input,'/'))
    {
        input >> divisor;
    }
    return divisor;
}

std::istream & operator>>(std::istream & input, Rational & r)
{
    input >> r.numerator;
    r.denominator = readOptionalDivisor(input);
    r.normalise();
    return input;
}

////////////////////////////////////////////////////////////////////////////////

} // End of namespace ExactArithmetic

#include "BigInteger.hpp"

const BigInteger BigInteger::NEGATIVE_ONE = BigInteger("-1");
const BigInteger BigInteger::ZERO = BigInteger("0");
const BigInteger BigInteger::ONE = BigInteger("1");
const BigInteger BigInteger::TWO = BigInteger("2");

void BigInteger::padVectorWithZeroes(std::vector<int>& vec, std::size_t requiredLen) const {
    std::size_t paddingSize = requiredLen - vec.size();
    std::vector<int> padding(paddingSize, 0);
    padding.insert(padding.end(), vec.begin(), vec.end());
    vec = padding;
}

BigInteger::BigInteger(const std::string& num) {
    std::size_t digitCount = num.length();

    if (digitCount == 0) {
        throw std::invalid_argument("Cannot parse an empty string as an integer");
    }

    std::size_t start;

    char firstChar = num.at(0);
    if (firstChar == '-' || firstChar == '+') {
        start = 1;
        digitCount -= 1;

        if (firstChar == '-') {
            isNegative = true;
        }
    }
    else {
        start = 0;
    }

    digits.reserve(digitCount);

    std::size_t stopAt = digitCount + start;
    for (std::size_t i = start; i < stopAt; i++) {
        char digit = num.at(i);

        if (digit < '0' || digit > '9') {
            throw std::invalid_argument("Cannot parse a non-digit character as an integer: '" + std::string(1, digit) + "'");
        }

        digits.push_back(digit - 48);
    }
}

BigInteger BigInteger::abs() const {
    return (*this >= ZERO) ? *this : -(*this);
}

BigInteger BigInteger::pow(const BigInteger& exponent) const {
    // trivial cases
    if (exponent < ZERO) {
        // since negative exponents result in numbers somewhere between 0 and 1,
        // and this class is about integer arithmetic, we round that down and,
        // as a result, return zero
        return ZERO;
    }
    if (exponent == ZERO) {
        return ONE;
    }
    if (exponent == ONE) {
        return *this;
    }

    BigInteger result = ONE;

    for (auto i = ZERO; i < exponent; i++) {
        result *= *this;
    }

    return result;
}

BigInteger BigInteger::sqrt(unsigned int iterations) const {
    if (*this < ZERO) {
        throw std::invalid_argument("Cannot take the square root of a negative number");
    }
    if (*this == ZERO) {
        return ZERO;
    }
    if (iterations == 0U) {
        throw std::invalid_argument("The number of iterations must be positive");
    }

    BigInteger current = *this;
    BigInteger original = *this;

    for (unsigned int i = 0; i < iterations; i++) {
        current = (current + original / current) / TWO;
    }

    return current;
}

BigInteger BigInteger::operator+(const BigInteger& other) const {
    std::vector<int> resultDigits;
    bool resultIsNegative = false;

    if (isNegative && other.isNegative) {
        resultIsNegative = true;
    }

    int carry = 0;

    std::size_t stopAt = std::max(digits.size(), other.digits.size());
    resultDigits.reserve(stopAt + 1);

    std::vector<int> digitsFirst;
    std::vector<int> digitsSecond;

    int multiplier = 1;

    if (isNegative != other.isNegative) {
        multiplier = -1;

        if (other.isNegative) {
            if (-(*this) > other) {
                resultIsNegative = true;

                digitsFirst = other.digits;
                digitsSecond = digits;
            }
            else {
                digitsFirst = digits;
                digitsSecond = other.digits;
            }
        }
        else {
            if (-(*this) > other) {
                resultIsNegative = true;

                digitsFirst = digits;
                digitsSecond = other.digits;
            }
            else {
                digitsFirst = other.digits;
                digitsSecond = digits;
            }
        }
    }
    else {
        digitsFirst = digits;
        digitsSecond = other.digits;
    }

    padVectorWithZeroes(digitsFirst, stopAt);
    padVectorWithZeroes(digitsSecond, stopAt);

    for (int i = stopAt - 1; i >= 0; i--) {
        int item1 = digitsFirst.at(i), item2 = digitsSecond.at(i);

        int adding = item1 + multiplier * (item2 + carry);

        if (adding < 0 || adding > 9) {
            carry = 1;

            if (adding > 9) {
                adding -= 10;
            }
            else {
                adding += 10;
            }
        }
        else {
            carry = 0;
        }

        resultDigits.push_back(adding);
    }

    if (carry == 1) {
        resultDigits.push_back(1);
    }

    while (resultDigits.size() > 1 && resultDigits.back() == 0) {
        resultDigits.pop_back();
    }

    std::reverse(resultDigits.begin(), resultDigits.end());

    std::string resultString = "";
    resultString.reserve(resultDigits.size());

    for (int item : resultDigits) {
        resultString += (char) (item + '0');
    }

    BigInteger result(resultString);
    result.isNegative = resultIsNegative;

    return result;
}

BigInteger BigInteger::operator-(const BigInteger& other) const {
    return *this + (-other);
}

/*
 * note: right now this is horrendously slow, and so is anything that uses it
 */
BigInteger BigInteger::operator*(const BigInteger& other) const {
    BigInteger multiplier = other.abs();

    BigInteger result = ZERO;

    for (auto i = ZERO; i < multiplier; i++) {
        result += *this;
    }

    result.isNegative = (isNegative != other.isNegative);

    return result;
}

/*
 * note: right now this is horrendously slow, and so is anything that uses it
 */
BigInteger BigInteger::operator/(const BigInteger& other) const {
    if (other == ZERO) {
        throw std::invalid_argument("Cannot divide by zero");
    }

    BigInteger number = this->abs(), divisor = other.abs();
    bool resultIsNegative = (isNegative != other.isNegative);

    // some trivial cases
    if (number < divisor) {
        // if you divide a smaller number by a bigger one,
        // you get something between 0 and 1, and we throw away the fractional part,
        // which yields 0
        return ZERO;
    }
    if (divisor == ONE) {
        return resultIsNegative ? -(*this) : *this;
    }
    if (number == divisor) {
        return resultIsNegative ? NEGATIVE_ONE : ONE;
    }

    BigInteger remainder_ = *this;
    BigInteger quotient = ZERO;

    while (remainder_ >= divisor) {
        remainder_ -= divisor;
        quotient++;
    }

    if (resultIsNegative) {
        quotient.isNegative = true;
    }

    return quotient;
}

BigInteger BigInteger::operator%(const BigInteger& other) const {
    if (other == ZERO) {
        throw std::invalid_argument("Cannot divide by zero");
    }

    BigInteger number = this->abs(), divisor = other.abs();
    bool resultIsNegative = (isNegative != other.isNegative);

    if (number < divisor) {
        return resultIsNegative ? -number : number;
    }

    BigInteger result = *this;

    while (result > divisor) {
        result -= divisor;
    }

    result.isNegative = resultIsNegative;

    return result;
}

BigInteger& BigInteger::operator+=(const BigInteger& other) {
    *this = *this + other;
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
    *this = *this - other;
    return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) {
    *this = *this * other;
    return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& other) {
    *this = *this / other;
    return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& other) {
    *this = *this % other;
    return *this;
}

BigInteger& BigInteger::operator++() {
    *this = *this + ONE;
    return *this;
}

BigInteger& BigInteger::operator++(int) {
    BigInteger temp = *this;
    ++(*this);
    return temp;
}

BigInteger& BigInteger::operator--() {
    *this = *this - ONE;
    return *this;
}

BigInteger& BigInteger::operator--(int) {
    BigInteger temp = *this;
    --(*this);
    return temp;
}

BigInteger BigInteger::operator-() const {
    BigInteger result = *this;
    result.isNegative = !result.isNegative;
    return result;
}

// --------------------------------- Logical operators ---------------------------------

bool BigInteger::operator==(const BigInteger& other) const {
    return (
        digits.size() == other.digits.size()
        && isNegative == other.isNegative
        && digits == other.digits
    );
}

bool BigInteger::operator!=(const BigInteger& other) const {
    return !(*this == other);
}

bool BigInteger::operator>(const BigInteger& other) const {
    if (isNegative != other.isNegative) {
        // If their signs are not equal, then it is bigger
        // than the other if the other one is negative.
        return other.isNegative;
    }
    else {
        std::size_t longestLen = std::max(digits.size(), other.digits.size());

        std::vector<int> digits1 = digits, digits2 = other.digits;

        padVectorWithZeroes(digits1, longestLen);
        padVectorWithZeroes(digits2, longestLen);

        if (isNegative) {
            for (int i = 0; i < longestLen; i++) {
                int dig1 = digits1[i], dig2 = digits2[i];

                if (dig1 > dig2) {
                    return false;
                }
                else if (dig1 < dig2) {
                    return true;
                }
            }
        }
        else {
            for (int i = 0; i < longestLen; i++) {
                int dig1 = digits1[i], dig2 = digits2[i];

                if (dig1 > dig2) {
                    return true;
                }
                else if (dig1 < dig2) {
                    return false;
                }
            }
        }
    }

    return false;
}

bool BigInteger::operator>=(const BigInteger& other) const {
    return *this > other || *this == other;
}

bool BigInteger::operator<(const BigInteger& other) const {
    return !(*this >= other);
}

bool BigInteger::operator<=(const BigInteger& other) const {
    return !(*this > other);
}

// --------------------------------- Miscellaneous operators ---------------------------------

bool BigInteger::operator!() const {
    return digits.size() == 1 && digits.at(0) == 0;
}

BigInteger::operator std::string() const {
    std::vector<char> result;
    std::size_t toAllocate = digits.size() + (isNegative ? 1 : 0);

    result.reserve(toAllocate);

    if (isNegative) {
        result.push_back('-');
    }

    for (int i = 0; i < digits.size(); i++) {
        result.push_back((char) (digits.at(i) + '0'));
    }

    std::string resultString = std::string(result.begin(), result.end());

    return resultString;
}

std::ostream& operator<<(std::ostream& os, const BigInteger& obj) {
    return os << std::string(obj);
}
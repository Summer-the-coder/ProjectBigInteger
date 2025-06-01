#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <type_traits>
#include <cmath>
#include <algorithm>

/**
 * @brief A class designed to support integer arithmetic for arbitrarily large integers.
 */
class BigInteger {
private:
    std::vector<int> digits;
    bool isNegative = false;

    /*
     * purely internal method, used to make it easier to work with numbers
     * that have different amounts of digits
     */
    void padVectorWithZeroes(std::vector<int>& vec, size_t requiredLen) const;

    // constants
    const static BigInteger NEGATIVE_ONE;
    const static BigInteger ZERO;
    const static BigInteger ONE;
    const static BigInteger TWO;

public:
    BigInteger(const std::string& num);

    /**
     * @brief Constructor for any type that can be converted to a string.
     */
    template<typename T,
        std::enable_if_t<
        !std::is_same_v<std::decay_t<T>, std::string> &&
        !std::is_same_v<std::decay_t<T>, const char*>, int> = 0>
    BigInteger(const T& num) : BigInteger([&] {
        std::ostringstream oss;
        oss << num;
        return oss.str();
        }()) {
    }

    /**
     * @brief Returns the absolute value of the BigInteger.
     */
    BigInteger abs() const;

    /**
     * @brief Raises a BigInteger to the specified exponent.
     */
    BigInteger pow(const BigInteger& exponent) const;

    /**
     * @brief Returns the approximate positive integer square root of the given BigInteger, rounded down.
     * 
     * For calculations, Newton's method is used; this method accepts an optional parameter
     * indicating the amount of iterations to make. By default, 15 iterations are made.
     */
    BigInteger sqrt(unsigned int iterations = 15) const;

    BigInteger operator+(const BigInteger& other) const;

    BigInteger operator-(const BigInteger& other) const;

    BigInteger operator*(const BigInteger& other) const;

    /**
     * Note: this performs integer division and rounds towards 0.
     */
    BigInteger operator/(const BigInteger& other) const;

    BigInteger operator%(const BigInteger& other) const;

    BigInteger& operator+=(const BigInteger& other);

    BigInteger& operator-=(const BigInteger& other);

    BigInteger& operator*=(const BigInteger& other);

    BigInteger& operator/=(const BigInteger& other);

    BigInteger& operator%=(const BigInteger& other);

    /**
     * @brief Increment operators.
     */
    BigInteger& operator++();

    BigInteger& operator++(int);

    /**
     * @brief Decrement operators.
     */
    BigInteger& operator--();

    BigInteger& operator--(int);

    /**
     * @brief Applies unary negation to the given BigInteger,
     * and returns a new BigInteger object.
     */
    BigInteger operator-() const;

    // --------------------------------- Logical operators ---------------------------------

    /**
     * @brief Determines whether the two BigIntegers are equal by value.
     */
    bool operator==(const BigInteger& other) const;

    /**
     * @brief Determines whether the two BigIntegers are not equal by value.
     */
    bool operator!=(const BigInteger& other) const;

    /**
     * @brief Compares two BigInteger objects by value, and determines
     * whether the left-hand side is numerically bigger than the
     * right-hand side.
     */
    bool operator>(const BigInteger& other) const;

    /**
     * @brief Compares two BigInteger objects by value, and determines
     * whether the left-hand side is numerically bigger than or
     * equal to the right-hand side.
     */
    bool operator>=(const BigInteger& other) const;

    /**
     * @brief Compares two BigInteger objects by value, and determines
     * whether the left-hand side is numerically smaller than
     * the right-hand side.
     */
    bool operator<(const BigInteger& other) const;

    /**
     * @brief Compares two BigInteger objects by value, and determines
     * whether the left-hand side is numerically smaller than or
     * equal to the right-hand side.
     */
    bool operator<=(const BigInteger& other) const;

    // --------------------------------- Miscellaneous operators ---------------------------------

    /**
     * @brief Converts a given BigInteger to a boolean value and inverts it,
     * following the general rule of C-derived languages that any non-zero number
     * is truthy.
     */
    bool operator!() const;

    /**
     * @brief Converts the given BigInteger object to its string representation.
     */
    operator std::string() const;

    friend std::ostream& operator<<(std::ostream& os, const BigInteger& obj);
};
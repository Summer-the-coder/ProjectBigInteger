#include <iostream>
#include "BigInteger.hpp"

int main() {
    std::string a, b, op;

    std::cout << "This application supports arithmetic operations on arbitrarily large integer numbers." << '\n';
    std::cout << "Though, do keep in mind that working with huge numbers is much more computationally expensive than when you are bounded by 64 bits." << '\n';
    std::cout << "Also, the implementation of multiplication and division (and anything that relies on it, e. g. sqrt) is currently very slow." << '\n';
    
    std::cout << '\n';
    
    std::cout << "Enter the first number: ";
    std::getline(std::cin, a);
	std::cout << "Enter the second number: ";
	std::getline(std::cin, b);
	std::cout << "Enter the operation (+, -, *, /, %, ^): ";
	std::getline(std::cin, op);

    try {
        BigInteger num1(a), num2(b);

        if (op.size() != 1) {
            throw std::invalid_argument("Invalid operation: " + op + "'");
        }

        BigInteger result("0");

        switch (op.at(0)) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            result = num1 / num2;
            break;
        case '%':
            result = num1 % num2;
            break;
        case '^':
            result = num1.pow(num2);
            break;
        }

        std::cout << "Result: " << result << '\n';
    }
    catch (const std::exception& e) {
        std::cerr << "An exception occurred: " << e.what() << '\n';
    }

    return 0;
}
// string_calculator.h:

#ifndef STRING_CALCULATOR_H // checks if header guard is not defined
#define STRING_CALCULATOR_H // defines header guard to prevent multiple inclusions

#include <string> // includes string library for std string
#include <vector> // includes vector library used internally maybe for split helper not exposed here

/**
 * @brief checks if a given string represents a valid double number according to the specified format
 * format (+|-)a(.b)
 * - optional sign (+ or -) at the beginning
 * - a sequence of digits (at least one)
 * - (b) optional decimal part if '.' exists it must be followed by at least one digit (b)
 * no external libraries or conversions (like stod try-catch) are allowed for validation
 *
 * @param s the string to validate
 * @return true if the string is a valid double representation false otherwise
 */
bool isValidDouble(const std::string& s); // declares the function isvaliddouble takes constant string reference returns bool

/** DOCSTRING COURTESY OF GEMINI 2.5 PRO
 * @brief adds two numbers represented as valid double strings
 * performs addition purely using string manipulation
 * handles signs decimal alignment and carries
 *
 * @param num1_str the first number string (assumed valid)
 * @param num2_str the second number string (assumed valid)
 * @return the string representation of the sum
 */
std::string addStrings(const std::string& num1_str, const std::string& num2_str); // declares addstrings function takes two constant string references returns string

/**
 * @brief multiplies two numbers represented as valid double strings
 * performs multiplication purely using string manipulation
 * handles signs, decimal placement, and carries.
 * Assumes inputs are valid doubles according to isValidDouble.
 *
 * @param num1_str the first number string (assumed valid)
 * @param num2_str the second number string (assumed valid)
 * @return the string representation of the product
 */
std::string multiplyStrings(const std::string& num1_str, const std::string& num2_str); // declare multiplystrings function it takes two strings returns a string

// helper functions for addstrings (can be defined in the .cpp file)
// std string addmagnitude(const std string& n1, const std string& n2);
// std string subtractmagnitude(const std string& n1, const std string& n2);
// int comparemagnitude(const std string& n1, const std string& n2);


#endif // STRING_CALCULATOR_H
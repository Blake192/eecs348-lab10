#include "string_calculator.h" // includes the header file for this module
#include <string> // includes string library for std string operations
#include <vector> // includes vector library might be used implicitly or for helpers
#include <algorithm> // includes algorithm library for reverse max etc
#include <cctype> // includes cctype library for isdigit function
#include <iostream> // includes iostream for debugging output like cerr warning


bool isValidDouble(const std::string& s) { // function definition isvaliddouble
    if (s.empty()) { // checks if the input string is empty
        return false; // returns false if empty
    } // ends if block

    int i = 0; // initializes index variable i to 0
    int n = s.length(); // gets the length of the string
    bool sign_present = false; // initializes flag for sign presence to false
    bool digit_present = false; // initializes flag for digit presence to false
    bool decimal_present = false; // initializes flag for decimal point presence to false
    bool digit_after_decimal = false; // initializes flag for digit after decimal presence to false

    // 1 check for optional sign 
    if (s[0] == '+' || s[0] == '-') { // checks if the first character is a plus or minus sign
        if (n == 1) return false; // returns false if string is only a sign character
        sign_present = true; // sets sign present flag to true
        i++; // increments index i to move past the sign
    } // ends if block

    // 2 check for integer part (a) 
    // removed unused variable start_integer 
    while (i < n && std::isdigit(s[i])) { // loops while index is within bounds and character is a digit
        digit_present = true; // sets digit present flag to true
        i++; // increments index i to move past the digit
    } // ends while loop
     // if only sign was present and no digits followed 
    if (sign_present && i == 1) return false; // returns false if only sign was found no digits


    // 3 check for optional decimal part (b) 
    if (i < n && s[i] == '.') { // checks if index is within bounds and character is a decimal point
        decimal_present = true; // sets decimal present flag to true
        i++; // increments index i to move past the decimal point
        // if '.' is the last character or if no digits preceded it and it's the first char (or first after sign)
        if (i == n || (!digit_present && i == (sign_present ? 2 : 1)) ) { // checks invalid cases like "5." "." "+." or no integer part like ".5"
             // examples "5." "." "+." "-." or if integer part was missing like ".5"
             // the requirement implies an integer part 'a' must exist before an optional 'b' 
             // however examples like ".5" are often considered valid doubles in practice 
             // sticking strictly to "(+|-)a(b)" requires a so "5." is invalid if ".5" needed validation the rule is different 
             // let's stick to the rule a is mandatory so "5." is invalid 
             if (!digit_present) return false; // case like ".5" "+.5" - invalid by rule "(+|-)a(b)" 
             return false; // case like "5." "+5." - invalid because b is mandatory if '.' exists 
        } // ends if block

        // removed unused variable start_fractional 
        while (i < n && std::isdigit(s[i])) { // loops while index is within bounds and character is a digit (fractional part)
            digit_after_decimal = true; // sets digit after decimal flag to true
            i++; // increments index i to move past the digit
        } // ends while loop
        // if '.' was present but no digits followed 
        if (!digit_after_decimal) return false; // example "1." returns false if no digits after decimal
    } // ends if block for decimal check

    // 4 final checks
    // - must have consumed the entire string 
    if (i != n) return false; // returns false if index did not reach the end of the string (trailing characters)
    // - must have had at least one digit (the 'a' part) 
    if (!digit_present) return false; // handles cases like "." "+" "-" which lack the 'a' part
    // if decimal was present must have digits after it 
    // this is already checked inside the decimal block but double-checking doesn't hurt
    if (decimal_present && !digit_after_decimal) return false; // redundant check for digits after decimal if present

    return true; // returns true if all checks passed
} // ends isvaliddouble function


// string arithmetic implementation 
// helper function prototypes (implementations below) 
std::string addMagnitude(std::string n1, std::string n2); // prototype for adding magnitudes
std::string subtractMagnitude(std::string n1, std::string n2); // assumes |n1| >= |n2| 
int compareMagnitude(const std::string& n1, const std::string& n2); // prototype for comparing magnitudes
std::string normalize(std::string s); // prototype for normalizing result string

// main addition function 
std::string addStrings(const std::string& num1_str, const std::string& num2_str) { // function definition addstrings
    // assume inputs are valid doubles as per isvaliddouble 
    bool num1_neg = num1_str[0] == '-'; // checks if first number is negative
    bool num2_neg = num2_str[0] == '-'; // checks if second number is negative

    std::string mag1 = num1_neg ? num1_str.substr(1) : (num1_str[0] == '+' ? num1_str.substr(1) : num1_str); // extracts magnitude of num1 handles +/- signs
    std::string mag2 = num2_neg ? num2_str.substr(1) : (num2_str[0] == '+' ? num2_str.substr(1) : num2_str); // extracts magnitude of num2 handles +/- signs

    std::string result_mag; // declares string for the resulting magnitude
    bool result_neg = false; // initializes flag for result negativity to false

    if (num1_neg == num2_neg) { // checks if both numbers have the same sign
        // same signs add magnitudes keep the sign 
        result_mag = addMagnitude(mag1, mag2); // calls helper to add magnitudes
        result_neg = num1_neg; // result sign is the same as the input numbers sign
    } else { // block executes if signs are different
        // different signs subtract smaller magnitude from larger magnitude 
        int comparison = compareMagnitude(mag1, mag2); // calls helper to compare magnitudes
        if (comparison == 0) { // checks if magnitudes are equal
            return "0.0"; // returns "0.0" if equal magnitude opposite sign
        } else if (comparison > 0) { // checks if magnitude 1 is greater than magnitude 2
            // |num1| > |num2| 
            result_mag = subtractMagnitude(mag1, mag2); // calls helper to subtract mag2 from mag1
            result_neg = num1_neg; // result sign is same as num1 (the larger magnitude number)
        } else { // block executes if magnitude 2 is greater than magnitude 1
            // |num2| > |num1| 
            result_mag = subtractMagnitude(mag2, mag1); // calls helper to subtract mag1 from mag2
            result_neg = num2_neg; // result sign is same as num2 (the larger magnitude number)
        } // ends magnitude comparison block
    } // ends sign comparison block

    std::string final_result = normalize(result_mag); // calls helper to normalize the resulting magnitude string

    // handle zero result edge case after normalization 
    if (final_result == "0" || final_result == "0.0") { // checks if normalized result is zero
        return "0.0"; // returns consistent "0.0" for zero
    } // ends if block

    return (result_neg ? "-" : "") + final_result; // prepends "-" if result is negative then returns final string
} // ends addstrings function

//  helper function implementations 

// removes leading zeros from integer part (unless it's just "0") 
// removes trailing zeros from fractional part (unless it's just "0") 
// adds "0." if result starts with "." 
// returns "0.0" if result becomes empty or just "." 
std::string normalize(std::string s) { // function definition normalize
    // separate integer and fractional parts 
    size_t decimal_pos = s.find('.'); // finds position of decimal point
    std::string int_part = (decimal_pos == std::string::npos) ? s : s.substr(0, decimal_pos); // extracts integer part or full string if no decimal
    std::string frac_part = (decimal_pos == std::string::npos) ? "" : s.substr(decimal_pos + 1); // extracts fractional part or empty string if no decimal

    // normalize integer part remove leading zeros 
    size_t first_digit = int_part.find_first_not_of('0'); // finds position of first non-zero digit
    if (std::string::npos == first_digit) { // checks if integer part is all zeros
        int_part = "0"; // sets integer part to "0" if all zeros
    } else { // executes if non-zero digit found
        int_part = int_part.substr(first_digit); // removes leading zeros
    } // ends if block
     if (int_part.empty()) { // can happen if input was like ".5" internally (shouldn't if add/sub is correct) 
        int_part = "0"; // ensures integer part is at least "0"
     } // ends if block


    // normalize fractional part remove trailing zeros 
    if (!frac_part.empty()) { // checks if fractional part exists
        size_t last_digit = frac_part.find_last_not_of('0'); // finds position of last non-zero digit
        if (std::string::npos == last_digit) { // checks if fractional part is all zeros
            frac_part = ""; // removes fractional part entirely if all zeros
        } else { // executes if non-zero digit found
            frac_part = frac_part.substr(0, last_digit + 1); // removes trailing zeros
        } // ends if block
    } // ends if block checking fractional part

    // reconstruct 
    if (frac_part.empty()) { // checks if fractional part was removed or never existed
        if (int_part == "0") return "0.0"; // represent zero consistently as "0.0"
        return int_part; // returns only integer part if no fractional part
    } else { // executes if fractional part exists
         if (int_part.empty()) int_part = "0"; // handles cases like ".5" becoming "0.5" prepends "0"
        return int_part + "." + frac_part; // returns combined integer dot fractional parts
    } // ends reconstruction block
     // failsafe for totally empty results (should not happen with correct logic) 
    if (int_part.empty() && frac_part.empty()) return "0.0"; // returns "0.0" if somehow both parts become empty

    return "errorinnormalize"; // should not be reached indicates logic error
} // ends normalize function

// compares magnitudes |n1| and |n2| (non-negative number strings) 
// returns 1 if |n1|>|n2|, -1 if |n1|<|n2|, 0 if |n1|==|n2| 
int compareMagnitude(const std::string& n1, const std::string& n2) { // function definition comparemagnitude
    std::string int1, frac1, int2, frac2; // declares strings for integer and fractional parts
    size_t dp1 = n1.find('.'); // finds decimal point in first number
    size_t dp2 = n2.find('.'); // finds decimal point in second number

    int1 = (dp1 == std::string::npos) ? n1 : n1.substr(0, dp1); // extracts integer part of n1
    frac1 = (dp1 == std::string::npos) ? "" : n1.substr(dp1 + 1); // extracts fractional part of n1
    int2 = (dp2 == std::string::npos) ? n2 : n2.substr(0, dp2); // extracts integer part of n2
    frac2 = (dp2 == std::string::npos) ? "" : n2.substr(dp2 + 1); // extracts fractional part of n2

    // normalize integer parts for comparison (remove leading zeros) 
    size_t first_digit = int1.find_first_not_of('0'); // finds first non-zero digit in int1
    int1 = (first_digit == std::string::npos) ? "0" : int1.substr(first_digit); // normalizes int1 removing leading zeros ensures "0"
     if (int1.empty()) int1 = "0"; // handles case where normalization resulted in empty string

    first_digit = int2.find_first_not_of('0'); // finds first non-zero digit in int2
    int2 = (first_digit == std::string::npos) ? "0" : int2.substr(first_digit); // normalizes int2 removing leading zeros ensures "0"
     if (int2.empty()) int2 = "0"; // handles case where normalization resulted in empty string


    // compare integer parts 
    if (int1.length() > int2.length()) return 1; // returns 1 if int1 is longer
    if (int1.length() < int2.length()) return -1; // returns -1 if int2 is longer
    if (int1 > int2) return 1; // returns 1 if int1 is lexicographically greater (same length)
    if (int1 < int2) return -1; // returns -1 if int2 is lexicographically greater (same length)

    // integer parts are equal compare fractional parts (lexicographically after padding) 
    size_t max_frac_len = std::max(frac1.length(), frac2.length()); // finds maximum fractional length
    frac1.resize(max_frac_len, '0'); // pads frac1 with trailing zeros to match max length
    frac2.resize(max_frac_len, '0'); // pads frac2 with trailing zeros to match max length

    if (frac1 > frac2) return 1; // returns 1 if frac1 is lexicographically greater
    if (frac1 < frac2) return -1; // returns -1 if frac2 is lexicographically greater

    return 0; // returns 0 they are equal
} // ends comparemagnitude function

// adds magnitudes |n1| and |n2| 
std::string addMagnitude(std::string n1, std::string n2) { // function definition addmagnitude takes strings by value for modification
    std::string int1, frac1, int2, frac2; // declares strings for parts
    size_t dp1 = n1.find('.'); // finds decimal point in n1
    size_t dp2 = n2.find('.'); // finds decimal point in n2

    int1 = (dp1 == std::string::npos) ? n1 : n1.substr(0, dp1); // extracts integer part 1
    frac1 = (dp1 == std::string::npos) ? "" : n1.substr(dp1 + 1); // extracts fractional part 1
    int2 = (dp2 == std::string::npos) ? n2 : n2.substr(0, dp2); // extracts integer part 2
    frac2 = (dp2 == std::string::npos) ? "" : n2.substr(dp2 + 1); // extracts fractional part 2

    // pad fractional parts to same length 
    size_t frac_len = std::max(frac1.length(), frac2.length()); // finds max fractional length
    frac1.resize(frac_len, '0'); // pads frac1 with trailing zeros
    frac2.resize(frac_len, '0'); // pads frac2 with trailing zeros

    // add fractional parts (right to left) 
    std::string frac_sum = ""; // initializes empty string for fractional sum
    int carry = 0; // initializes carry to 0
    for (int i = frac_len - 1; i >= 0; --i) { // loops from rightmost fractional digit to left
        int digit1 = frac1[i] - '0'; // converts char digit to int
        int digit2 = frac2[i] - '0'; // converts char digit to int
        int sum = digit1 + digit2 + carry; // calculates sum of digits plus carry
        frac_sum += std::to_string(sum % 10); // appends the unit digit of sum to result string (in reverse)
        carry = sum / 10; // calculates new carry for next digit
    } // ends fractional addition loop
    std::reverse(frac_sum.begin(), frac_sum.end()); // reverses the fractional sum string to correct order

    // pad integer parts (leading zeros) - easier to add right to left 
    size_t int_len = std::max(int1.length(), int2.length()); // finds max integer length
    while (int1.length() < int_len) int1.insert(0, "0"); // pads int1 with leading zeros
    while (int2.length() < int_len) int2.insert(0, "0"); // pads int2 with leading zeros

    // add integer parts (right to left) including carry from fraction 
    std::string int_sum = ""; // initializes empty string for integer sum
    for (int i = int_len - 1; i >= 0; --i) { // loops from rightmost integer digit to left
        int digit1 = int1[i] - '0'; // converts char digit to int
        int digit2 = int2[i] - '0'; // converts char digit to int
        int sum = digit1 + digit2 + carry; // calculates sum of digits plus carry (carry might be from fractional part)
        int_sum += std::to_string(sum % 10); // appends the unit digit of sum to result string (in reverse)
        carry = sum / 10; // calculates new carry for next digit
    } // ends integer addition loop
    if (carry > 0) { // checks if there is a final carry after leftmost digits
        int_sum += std::to_string(carry); // appends the final carry (in reverse)
    } // ends if block
    std::reverse(int_sum.begin(), int_sum.end()); // reverses the integer sum string to correct order

    // combine results (handle no fractional part) 
    if (frac_sum.empty()) { // checks if there was no fractional part
        return int_sum; // returns only the integer sum
    } else { // executes if there is a fractional part
        return int_sum + "." + frac_sum; // returns combined integer sum dot fractional sum
    } // ends combine block
} // ends addmagnitude function


// subtracts magnitudes |n2| from |n1| (assumes |n1| >= |n2|) 
std::string subtractMagnitude(std::string n1, std::string n2) { // function definition subtractmagnitude
     if (compareMagnitude(n1, n2) < 0) { // checks if precondition |n1| >= |n2| is violated
         // should not happen if called correctly but return difference anyway 
         // or could throw an error / assert 
         std::cerr << "warning subtractmagnitude called with n1 < n2" << std::endl; // prints warning to standard error
          return "error"; // or handle appropriately 
     } // ends precondition check
     if (compareMagnitude(n1, n2) == 0) return "0.0"; // or "0" 


    std::string int1, frac1, int2, frac2; // declares strings for parts
    size_t dp1 = n1.find('.'); // finds decimal point in n1
    size_t dp2 = n2.find('.'); // finds decimal point in n2

    int1 = (dp1 == std::string::npos) ? n1 : n1.substr(0, dp1); // extracts integer part 1
    frac1 = (dp1 == std::string::npos) ? "" : n1.substr(dp1 + 1); // extracts fractional part 1
    int2 = (dp2 == std::string::npos) ? n2 : n2.substr(0, dp2); // extracts integer part 2
    frac2 = (dp2 == std::string::npos) ? "" : n2.substr(dp2 + 1); // extracts fractional part 2

    // pad fractional parts 
    size_t frac_len = std::max(frac1.length(), frac2.length()); // finds max fractional length
    frac1.resize(frac_len, '0'); // pads frac1 with trailing zeros
    frac2.resize(frac_len, '0'); // pads frac2 with trailing zeros

    // pad integer parts (leading zeros) 
    size_t int_len = std::max(int1.length(), int2.length()); // finds max integer length
     while (int1.length() < int_len) int1.insert(0, "0"); // pads int1 with leading zeros
     while (int2.length() < int_len) int2.insert(0, "0"); // pads int2 with leading zeros

    // subtract fractional parts (right to left) 
    std::string frac_diff = ""; // initializes empty string for fractional difference
    int borrow = 0; // initializes borrow to 0
    for (int i = frac_len - 1; i >= 0; --i) { // loops from rightmost fractional digit to left
        int digit1 = frac1[i] - '0'; // converts char digit to int
        int digit2 = frac2[i] - '0'; // converts char digit to int
        int diff = digit1 - digit2 - borrow; // calculates difference considering borrow
        if (diff < 0) { // checks if borrow is needed from next digit left
            diff += 10; // adds 10 to make difference positive
            borrow = 1; // sets borrow for next digit to 1
        } else { // executes if no borrow needed
            borrow = 0; // sets borrow for next digit to 0
        } // ends borrow check
        frac_diff += std::to_string(diff); // appends the resulting digit to difference string (in reverse)
    } // ends fractional subtraction loop
     std::reverse(frac_diff.begin(), frac_diff.end()); // reverses the fractional difference string to correct order


    // subtract integer parts (right to left) including borrow 
    std::string int_diff = ""; // initializes empty string for integer difference
    for (int i = int_len - 1; i >= 0; --i) { // loops from rightmost integer digit to left
        int digit1 = int1[i] - '0'; // converts char digit to int
        int digit2 = int2[i] - '0'; // converts char digit to int
        int diff = digit1 - digit2 - borrow; // calculates difference considering borrow (borrow may be from fractional part)
        if (diff < 0) { // checks if borrow is needed
            diff += 10; // adds 10 to make difference positive
            borrow = 1; // sets borrow for next digit to 1
        } else { // executes if no borrow needed
            borrow = 0; // sets borrow for next digit to 0
        } // ends borrow check
        int_diff += std::to_string(diff); // appends the resulting digit to difference string (in reverse)
    } // ends integer subtraction loop
    // the final borrow should be 0 if n1 >= n2 
    std::reverse(int_diff.begin(), int_diff.end()); // reverses the integer difference string to correct order

    // combine and normalize (normalization handles leading/trailing zeros) 
     if (frac_diff.empty()) { // checks if there was no fractional part
         return normalize(int_diff); // returns normalized integer difference
     } else { // executes if there is a fractional part
         return normalize(int_diff + "." + frac_diff); // returns normalized combined integer dot fractional difference
     } // ends combine block
} // ends subtractmagnitude function

// Helper function to multiply two non-negative number strings (magnitudes)
// handles decimal points internally
std::string multiplyMagnitude(std::string n1, std::string n2) { // defines the multiplymagnitude helper it takes two positive number strings
    // find decimal positions
    size_t dp1 = n1.find('.'); // find the dot in the first number
    size_t dp2 = n2.find('.'); // find the dot in the second number

    // calculate total decimal places in the result
    int total_decimal_places = 0; // start count of decimal places at zero
    if (dp1 != std::string::npos) { // if first number had a dot
        total_decimal_places += (n1.length() - dp1 - 1); // add its decimal places to the total count
        n1.erase(dp1, 1); // remove decimal point for integer multiplication 
    } // end first number dot check
    if (dp2 != std::string::npos) { // if second number had a dot
        total_decimal_places += (n2.length() - dp2 - 1); // add its decimal places to the total count
        n2.erase(dp2, 1); // remove decimal point 
    } // end second number dot check

    // handle cases where input was just "." which becomes empty (shouldn't happen if valid)
    if (n1.empty()) n1 = "0"; // if removing dot made n1 empty set it to 0
    if (n2.empty()) n2 = "0"; // if removing dot made n2 empty set it to 0

    // perform long multiplication as if they were integers
    int len1 = n1.length(); // get length of first number string (no dot now)
    int len2 = n2.length(); // get length of second number string (no dot now)
    std::string product(len1 + len2, '0'); // initialize product string with zeros 

    // multiply n1 by each digit of n2 (from right to left)
    for (int i = len2 - 1; i >= 0; --i) { // loop through second number's digits backwards
        int carry = 0; // reset carry for each digit of n2
        int digit2 = n2[i] - '0'; // get the current digit from n2 as an int
        int product_idx = len1 + i; // starting index in product string for this partial product 

        // multiply n1 by digit2
        for (int j = len1 - 1; j >= 0; --j) { // loop through first number's digits backwards
            int digit1 = n1[j] - '0'; // get the current digit from n1 as an int
            int current_product = digit1 * digit2 + (product[product_idx] - '0') + carry; // multiply digits add existing product digit add carry
            product[product_idx] = (current_product % 10) + '0'; // put the unit digit of the result back into product string
            carry = current_product / 10; // calculate the new carry
            product_idx--; // move left in the product string
        } // end inner loop (n1 digits)

        // add remaining carry to the left
         if (carry > 0) { // if there's carry left after multiplying by digit2
             // propagate carry - keep adding carry to existing digits leftwards
             int k = product_idx; // start from where we left off in product string
             while (k >= 0 && carry > 0) { // while still inside the product string and carry exists
                 int sum = (product[k] - '0') + carry; // add carry to the digit there
                 product[k] = (sum % 10) + '0'; // update the digit
                 carry = sum / 10; // update the carry
                 k--; // move left
             } // end carry propagation loop
              // if carry still exists after reaching the beginning, need to prepend (unlikely with pre-allocated size, but safe)
             if (carry > 0) { // if carry still exists something unusual happened or numbers were huge
                 // this case indicates the initial product size estimate might be off, or an error.
                 // however, string product(len1 + len2, '0') should suffice.
                 // if needed, product.insert(0, std::to_string(carry));
                 std::cerr << "warning: carry overflow in multiplymagnitude (should not happen with adequate product size)" << std::endl; // print a warning just in case
                 // for robustness, prepend remaining carry digits
                std::string carry_str = std::to_string(carry); // convert carry to string
                product.insert(0, carry_str); // stick the carry digits at the front
            } // end unexpected carry handling
         } // end carry handling block
    } // end outer loop (n2 digits)

    // place the decimal point
    if (total_decimal_places > 0) { // if the original numbers had decimal places
        // ensure the product string is long enough to place the decimal
        // if product is "123" and places=4, it should become "0.0123"
        while (product.length() < (size_t)total_decimal_places) { // if product string is shorter than decimal places needed
             product.insert(0, "0"); // prepend leading zeros if needed 
        } // end padding loop

        // insert the decimal point
        int decimal_insert_pos = product.length() - total_decimal_places; // calculate where the dot should go from the left
         if (decimal_insert_pos <= 0) { // result is less than 1 (e.g., 0.5 * 0.5 = 0.25) 
             product.insert(0, std::string(1 - decimal_insert_pos, '0')); // add leading "0." or "0.0..." 
             product.insert(1, "."); // insert decimal after the first '0' 
         } else { // if the dot position is somewhere inside the string
            product.insert(decimal_insert_pos, "."); // put the dot at the calculated position
         } // end decimal insertion logic
    } // end decimal placement block

    // the result might have leading/trailing zeros needing normalization,
    // but normalization is handled by the caller (multiplyStrings)
    // however, ensure at least one digit before decimal if result is like ".123"
    if (product[0] == '.') { // if the result starts with a dot
        product.insert(0, "0"); // put a zero before it
    } // end leading dot check

    // handle case where product becomes empty or just "." (shouldn't happen with valid inputs/logic)
    if (product.empty() || product == ".") { // if something went wrong and result is empty or just a dot
        return "0.0"; // return zero
    } // end sanity check for empty result

    return product; // return the raw product string (will be normalized later) 
} // end multiplymagnitude helper function

// Main multiplication function
std::string multiplyStrings(const std::string& num1_str, const std::string& num2_str) { // defines the multiplystrings function
    // assume inputs are valid doubles as per isvaliddouble 

    // handle signs
    bool num1_neg = num1_str[0] == '-'; // check if first number is negative
    bool num2_neg = num2_str[0] == '-'; // check if second number is negative
    bool result_neg = num1_neg ^ num2_neg; // result is negative if one is negative (xor) 

    // extract magnitudes
    std::string mag1 = num1_neg ? num1_str.substr(1) : (num1_str[0] == '+' ? num1_str.substr(1) : num1_str); // get positive part of num1
    std::string mag2 = num2_neg ? num2_str.substr(1) : (num2_str[0] == '+' ? num2_str.substr(1) : num2_str); // get positive part of num2

    // handle zero multiplication efficiently
    // normalize magnitudes temporarily just to check for zero
    std::string norm_mag1 = normalize(mag1); // clean up mag1 to check if it's zero
    std::string norm_mag2 = normalize(mag2); // clean up mag2 to check if it's zero
     if (norm_mag1 == "0.0" || norm_mag1 == "0" || norm_mag2 == "0.0" || norm_mag2 == "0") { // if either number is zero
        return "0.0"; // multiplication by zero is zero 
    } // end zero check

    // perform multiplication on magnitudes
    std::string result_mag = multiplyMagnitude(mag1, mag2); // call the helper to do the actual math

    // normalize the final result
    std::string final_result = normalize(result_mag); // clean up the raw multiplication result

    // handle zero result edge case after normalization (e.g., from tiny numbers)
    if (final_result == "0" || final_result == "0.0") { // if the result normalized to zero
        return "0.0"; // return consistent zero format
    } // end normalized zero check

    // add sign if necessary
    return (result_neg ? "-" : "") + final_result; // put the minus sign back if needed and return
} // end multiplystrings function



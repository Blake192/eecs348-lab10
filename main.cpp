/***********************************************************************
 * EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE       CCCCCCCCCCCCC   SSSSSSSSSSSSSSS       333333333333333          444444444       888888888  
 * E::::::::::::::::::::EE::::::::::::::::::::E    CCC::::::::::::C SS:::::::::::::::S     3:::::::::::::::33       4::::::::4     88:::::::::88  
 * E::::::::::::::::::::EE::::::::::::::::::::E  CC:::::::::::::::CS:::::SSSSSS::::::S     3::::::33333::::::3     4:::::::::4   88:::::::::::::88  
 * EE::::::EEEEEEEEE::::EEE::::::EEEEEEEEE::::E C:::::CCCCCCCC::::CS:::::S     SSSSSSS     3333333     3:::::3    4::::44::::4  8::::::88888::::::8  
 *   E:::::E       EEEEEE  E:::::E       EEEEEEC:::::C       CCCCCCS:::::S                             3:::::3   4::::4 4::::4  8:::::8     8:::::8  
 *   E:::::E               E:::::E            C:::::C              S:::::S                             3:::::3  4::::4  4::::4  8:::::8     8:::::8  
 *   E::::::EEEEEEEEEE     E::::::EEEEEEEEEE  C:::::C               S::::SSSS                  33333333:::::3  4::::4   4::::4   8:::::88888:::::8  
 *   E:::::::::::::::E     E:::::::::::::::E  C:::::C                SS::::::SSSSS             3:::::::::::3  4::::444444::::444  8:::::::::::::8  
 *   E:::::::::::::::E     E:::::::::::::::E  C:::::C                  SSS::::::::SS           33333333:::::3 4::::::::::::::::4 8:::::88888:::::8  
 *   E::::::EEEEEEEEEE     E::::::EEEEEEEEEE  C:::::C                     SSSSSS::::S                  3:::::34444444444:::::4448:::::8     8:::::8  
 *   E:::::E               E:::::E            C:::::C                          S:::::S                 3:::::3          4::::4  8:::::8     8:::::8  
 *   E:::::E       EEEEEE  E:::::E       EEEEEEC:::::C       CCCCCC            S:::::S                 3:::::3          4::::4  8:::::8     8:::::8  
 * EE::::::EEEEEEEE:::::EEE::::::EEEEEEEE:::::E C:::::CCCCCCCC::::CSSSSSSS     S:::::S     3333333     3:::::3          4::::4  8::::::88888::::::8  
 * E::::::::::::::::::::EE::::::::::::::::::::E  CC:::::::::::::::CS::::::SSSSSS:::::S     3::::::33333::::::3        44::::::44 88:::::::::::::88  
 * E::::::::::::::::::::EE::::::::::::::::::::E    CCC::::::::::::CS:::::::::::::::SS      3:::::::::::::::33         4::::::::4   88:::::::::88  
 * EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE       CCCCCCCCCCCCC SSSSSSSSSSSSSSS         333333333333333           4444444444     888888888  
 *   
 * EECS 348 LAB 10
 * DESCRIPTION: IMPLEMENT STRING MATH OPERATIONS IN C++
 * INPUTS: Reads txt file from command line argument. 
 * OUTPUTS: Shows calculations and whether nums are valid or not.
 * ALL COLLABS: Blake Pennel, Gemini 2.5 Pro's help with most code and struture
 * AUTHORS: Blake Pennel 
 * CREATION DATE: 04/15/2025 
 ***********************************************************************/
#include <iostream> // for input and output
#include <fstream> // for file reading
#include <string> // for using strings
#include <vector> // for using vectors
#include "string_calculator.h" // for custom string-based calculator functions

int main(int argc, char* argv[]) { // main function that runs the program
    if (argc != 2) { // check that exactly one argument is passed
        std::cerr << "usage: " << argv[0] << " <filename>" << std::endl; // show usage if wrong number of arguments
        return 1; // exit early since input was bad
    }

    std::string filename = argv[1]; // get the filename from the arguments
    std::ifstream inputFile(filename); // try to open the file

    if (!inputFile.is_open()) { // check if the file failed to open
        std::cerr << "error: cannot open file '" << filename << "'" << std::endl; // show error message
        return 1; // exit since the file couldn’t be read
    }

    std::string line; // to hold each line from the file
    const std::string fixedNum = "-123.456"; // the number we’ll add to each input line

    while (std::getline(inputFile, line)) { // read the file line by line
        line.erase(0, line.find_first_not_of(" \t\n\r")); // trim leading whitespace
        line.erase(line.find_last_not_of(" \t\n\r") + 1); // trim trailing whitespace

        if (line.empty()) { // skip empty lines
            continue; // go to the next line
        }

        std::cout << "read line: \"" << line << "\"" << std::endl; // show the line that was read

        if (isValidDouble(line)) { // check if the line is a valid double
            std::cout << "  status: valid" << std::endl; // show that it’s valid
            std::string result = addStrings(line, fixedNum); // add it to the fixed number
            std::cout << "  calculation: " << line << " + (" << fixedNum << ") = " << result << std::endl; // show result
        } else {
            std::cout << "  status: invalid double number" << std::endl; // show that it’s not valid
        }
        std::cout << "------------------------------------" << std::endl; // separator between outputs
    }

    inputFile.close(); // close the file after reading
    return 0; // end the program successfully
}

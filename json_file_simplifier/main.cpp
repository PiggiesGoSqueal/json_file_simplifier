/*
Project Name: json_file_simplifier
Project Purpose:
    - Will simplify JSON files to make them easier to read from.

Program Name: main.cpp
Version: 3
Author: Tommy Weber
Date: 10/18/2019

Latest Changes / Notes:
    - On 10/15/2019 I added support for storing integers that are outside of double quotes.
    - On 10/16/2019 I added support for storing doubles (like 0.0) that are outside of double quotes.
    - On 10/18/2019 I added support for recursive directory iterating.
        * BUG: When 3 folders depth into InputFolder & OutputFolder, it will create the folders & file still but won't output data to the file. Example in: OutputFiles\firstDirectTest\2ndDirectTest\3rdDirectTest on local Desktop PC.
    - Algorithm Steps and comments are up to date as of 10/18/2019 at 6:30 pm MDT.

-------------------------------
Follow Good Practices:
    1. Use algorithm steps (in English) first.
    2. Use compiler flags. 
    3. Use static analyzers to test for bugs.
    4. Use >> and << when dividing and multiplying by 2. Find the file that explains it.
    5. A function should only do one thing, not two (like printInstructionsAndGetInput() is bad) 
    6. Make sure function names are specific to EXACTLY what it does. openFiles() is not correct for something that iterates through a directory recursively
    #. Etc...

Overall Algorithm Steps: (Not numbered in code like other algorithm steps are)
    1.
        a. Tell user to input the desired files to be converted into the "InputFiles" folder path. Done in printInstructions()
        b. Tell user to input 'Y' when ready or 'n' to exit. Done in getUserConfirmation()

    2. Open all files one at a time in the file path with the "InputFiles" folder.
        - Done in iterateInDirectory()

    ---
    For Each File (#3-#8):
    3. In each file, read each char and keep track of where there are:
        a. Double quotes
            - Things within double quotes are saved.
            - Example: "minecraft:type"
        b. Numbers
            - We will save all numbers in the file (even if they are outside double quotes).

    4. Store everything that matches what we want in #3 into a vector of alphabetical & number contents.
    
    5. Close input file.
    
    6. Create an output file in "OutputFiles" with same name but with preferred extension (.txt by default) instead of .json.
    
    7. In output file write each item in the vector to the file in a new line.
        - Example vector: {""type"", ""minecraft:crafting_shaped"", ""group"", ""bark"", "333", ""444""}
        - Writing example vector to file:
            "type"
            "minecraft:crafting_shaped"
            "group"
            "bark"
            333
            "444"

    8. Close output file.
    ---

    9. Tell user that program has finished.

HOW TO COMPILE:
    $ cppcheck --enable=all main.cpp
    $ g++ -std=c++17 main.cpp -lstdc++fs -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
    $ ./a.out
*/
#include <iostream>
#include <iomanip> // output formatting in console
#include <string>
#include <fstream>
#include <filesystem>
#include <vector> // for storing words from json file and writing them to output files.
#include <regex> // std::regex_replace() & std::regex()

namespace fs = std::filesystem; // for convenience

void printInstructions(const int width);
void getUserConfirmation(const int width); // confirms user has followed instructions & program can parse files
void iterateInDirectory(); // Recursively iterate through the filesystem "InputFiles" directory
void parseJsonFile(const fs::path &inputFilePath, std::vector<std::string> &alphaNums); 
void writeJsonAsTxt(const fs::path &outputFilePath, const std::vector<std::string> &alphaNums);

int main() {
    const int printWidth = 50; // the num of '-' or '=' in a line output to terminal
    printInstructions(printWidth); 
    getUserConfirmation(printWidth);

    iterateInDirectory();

    std::cout << "The program has completed successfully!\n";
    
    return 0;
}


void printInstructions(const int width) {
    const char sym = '-';

    std::cout << std::setfill(sym) << std::setw(width) << "\n"
        << "Hello, thank you for using JSON File Simplifier!\n\n"
        << "Note(s):\n"
        << "- This program does not support decimals in folder names. If you have decimals, change them to '_'.\n"
        << '\n'
        << "Usage Steps:\n"
        << "1. To get started, open the file path this program is located at on your computer.\n"
        << "2. Now move the desired files you would like to simplify into the " << std::quoted("InputFiles") << " directory.\n";
}

void getUserConfirmation(const int width) {
    std::cout << "3. Input 'y' when ready or input 'n' to quit: "
              << std::setw(width) << std::setfill(' ');
    
    char ready = 'n';
    std::cin >> ready;
    std::cout << '\n';
    if (ready != 'y' && ready != 'Y') {
        std::cout << "Program has ended. You may now close the program.\n";
        exit(0);
    }
}

/*
Algorithm Steps of this Function:
    1. Create the path variable. 
        - Make sure it includes the current path and InputFiles directory.

    2. For each file (recursively) in the InputFiles directory:
        a. Create 2 path variables that both equal entry variable:
            - inputFilePath
                * Have this equal entry.
            - outputFilePath
                * Have this equal entry but replace "InputFiles" part with "OutputFiles". We do this using #include <regex> code.
        b. Check if entry is a directory file type.
            - If it is, check if the outputFilePath exists already (such as the 1_13 folder inside InputFiles path). If the directories don't already exist, create them.
        c. Check if inputFilePath is a regular file (as opposed to a directory or symlink). If so:
            - Create an empty vec called alphaNums which will contain all words and numbers for a SINGLE file at a time.
            - Call parseJsonFile()
            - Because the file path is a regular file, we will need to replace the output file path's extension with "txt" before calling writeJsonAsTxt()
            - Call writeJsonAsTxt()
*/
void iterateInDirectory() {
    // 1.
    fs::path fullInPath = fs::current_path();
    fullInPath /= "InputFiles";

    // 2.
    for (auto &entry : fs::recursive_directory_iterator(fullInPath)) {
        // 2a.
        fs::path inputFilePath = entry; // Ex 1: /mnt/c/Users/Tom/Desktop/json_file_simplifier/InputFiles/1_13
                                        // Ex 2: /mnt/c/Users/Tom/Desktop/json_file_simplifier/InputFiles/1_13/acacia_boat.json
        fs::path outputFilePath = std::regex_replace(inputFilePath.string(), std::regex("InputFiles"), "OutputFiles");
                                        // Ex 1: /mnt/c/Users/Tom/Desktop/json_file_simplifier/OutputFiles/1_13
                                        // Ex 2: /mnt/c/Users/Tom/Desktop/json_file_simplifier/OutputFiles/1_13/acacia_boat.json

        // 2b.
        if (entry.is_directory()) {
            if (!fs::exists(outputFilePath)) {
                fs::create_directories(outputFilePath);
            }
        }

        // 2c.
        if (entry.is_regular_file()) {

            std::vector<std::string> alphaNums;

            parseJsonFile(inputFilePath, alphaNums);
            
            outputFilePath.replace_extension("txt"); // If file path is a regular file, convert the extension to txt.
            writeJsonAsTxt(outputFilePath, alphaNums);
        }
    }
}


/*
Algorithm Steps of this Function:
    1. Open the path of the current file.

    2. If the file couldn't be opened, print error and stop program.

    3. Define some variables that will be used for getting the proper data from the input file inside the while loop.

    4. Read the file one char at a time. (I wonder if there is a better way?)
        a. If the char is a " then:
            - add to the number of quotes (keeping track with a var)
        b. If the count of quotes is equal to 1 then:
            - add the current char to the word variable
        c. If the char is a " again then:
            - Add to the number of quotes (so it now equals 2)
        d. If the count of quotes is greater than or equal to 2 then:
            - add the current char (the " symbol) to the word variable.
            - then add the word to a vector.
            - then clear the word variable to prep for next word.
    
    5. Close the input file.
        
    Important Note of This Method:
        - This code assumes there are no quotes within quotes.
            * Ex: "  "minecraft:word" " in the files.
*/
void parseJsonFile(const fs::path &inputFilePath, std::vector<std::string> &alphaNums) {
        // 1. 
        std::ifstream inFile(inputFilePath);

        // 2.
        if (!inFile.is_open()) {
            std::cout << "The input file path below couldn't be opened:\n"
                      << inputFilePath
                      << "\n\n"
                      << "Program stopped.\n";
                exit(1);
        }

        // 3.
        char c;
        std::string alphaNum;
        bool quoteBlockStarted = false;
        bool numStarted = false;
        bool numHasDecimalAlready = false;

        // 4.
        /*
        Goal:
            - Anything inside " " should be saved.
            - Any integer numbers outside of " " should be saved. Examples: 30 or 334
            - Any double numbers outside of " " should be saved. Examples: 3.14 or 55.12
        */
        // CODE CURRENTLY CHECKS EVERY CHAR OF FILE ONE AT A TIME. Is it possible to improve the efficiency / performance of this? Algorithm analysis.
        while (inFile.get(c)) {
            // Program does not support adding numbers that are like: .23, only 0.23
            // STARTING NUM HANDLER:
            if (quoteBlockStarted == false && (std::isdigit(c)) && numStarted == false) {
                numStarted = true; // Shows the previous char was a num too, and thus is still adding digits to alphaNum to equal 1 number of multiple digits.
                alphaNum += c;
            }

            // NUM ALREADY STARTED HANDLER:
            else if (quoteBlockStarted == false && (std::isdigit(c) || c == '.') && numStarted == true) {          
                // add num or decimal to alphaNum:
                // - add decimal to alphaNum:
                //   * Makes sure it is not adding a second decimal to a number that already has one.
                if (c == '.' && numHasDecimalAlready == false) {
                    alphaNum += c;
                    numHasDecimalAlready = true; // preps for next iteration
                }
                // if it looks like: 55... and we're at the 2nd decimal so alphaNum = 55. atm, then remove the dot.
                else if (c == '.' && numHasDecimalAlready == true) {
                    alphaNum.resize(alphaNum.size()-1);
                }
                // - add num to alphaNum:
                else if (std::isdigit(c)) {
                    alphaNum += c;
                }
            }

            // NUM ENDED HANDLER & CHECKING CHAR FOR QUOTE HANDLER (WHEN NUM HAS JUST ENDED):
            else if (quoteBlockStarted == false && (!std::isdigit(c) || c != '.') && numStarted == true) {
                alphaNums.push_back(alphaNum);
                // Resets:
                alphaNum = "";
                numStarted = false;
                numHasDecimalAlready = false;

                // Checks if current char is a " symbol.
                // - If so, will add it to alphaNum and change quoteBlockStarted to true.
                if (c == '"') {
                    alphaNum += c;
                    quoteBlockStarted = true;
                }
            }

            // STARTING QUOTE HANDLER:
            else if (quoteBlockStarted == false && c == '"') {
                alphaNum += c;
                quoteBlockStarted = true;
            }
            // DURING QUOTE HANDLER (has not reached end of quote yet):
            else if (quoteBlockStarted == true && c != '"') {
                alphaNum += c;
            }
            // QUOTE ENDED HANDLER:
            else if (quoteBlockStarted == true && c == '"') {
                alphaNum += c;
                alphaNums.push_back(alphaNum);
                
                // Resets:
                alphaNum = "";
                quoteBlockStarted = false;
            }
        }

        /*
        // DEBUG Testing - Print Vector Contents
        std::cout << "[DEBUG] alphaNums = \n";
        for (std::string str : alphaNums) {
            std::cout << str << "\n";
        }
        */
        

        // 5.
        // Closes File:
        inFile.close();
}


/*
Algorithm Steps of this Function:
    1. Open the filename using the outputFilePath variable.

    2. Check if outFilePath properly opened. If not, give error and exit program.

    3. Write to the output file.
        - For every word in the vec from the function argument, write it into the output file. 1 word per line (enclosed in "").

    4. Close output file.
*/
void writeJsonAsTxt(const fs::path &outFilePath, const std::vector<std::string> &alphaNums) {
    // 1.
    std::ofstream outFile(outFilePath);

    // 2.
    if (!outFile.is_open()) {
        std::cerr << "\nError! Could not open output file properly.\n"
            << "Output file path:\n"
            << outFilePath << '\n'
            << "Program is exiting...\n";
        exit(1);
    }

    // 3.
    for (std::string str : alphaNums) {
        outFile << str << std::endl;
    }

    // 4.
    outFile.close();
}
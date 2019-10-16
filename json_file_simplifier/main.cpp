/*
Would allow me to download the latest MC folder of JSON recipes then convert all of the JSON recipe files to be this easier to read file using this tool just once. Then can put the changed recipes into the anti-shop-exploits program for it to support it.

Project Name: json_file_simplifier
Project Purpose:
    - Will simplify JSON files to make them easier to read from.
    - Will modify the original file to have the new contents.

Program Name: main.cpp
Author: Tommy Weber
Date: 10/16/2019

Latest Changes / Notes:
    - On 10/15/2019 I added support for storing integers that are outside of double quotes.
    - On 10/16/2019 I added support for storing doubles (like 0.0) that are outside of double quotes.
    - Algorithm Steps may not be fully up to date with latest changes.

-------------------------------
Follow Good Practices:
    1. Use algorithm steps (in English) first.
    2. Use compiler flags. 
    3. Use static analyzers to test for bugs.
    4. Use >> and << when dividing and multiplying by 2. Find the file that explains it.
    5. ...

Algorithm Steps:
    1. Tell user to input the desired files to be converted into the "InputFiles" folder path.
        - Tell user to input 'Y' when ready or 'n' to exit.
    2. Open all files one at a time in the file path with the "InputFiles" folder.
    ---
    For Each File:
    3. In each file, read each char and keep track of where there are:
        a. Double quotes
            - We will save everything within double quotes.
            - Example: "minecraft:type"
        b. Numbers
            - We will save all numbers in the file (even if they are outside double quotes).
    4. Store everything that matches what we want in #3 into a vector of alphabetical & number contents.
    5. Close input file.
    5. Create an output file in "OutputFiles" with same name but .txt extension instead of .json.
    6. In output file write each item in the vector to the file in a new line.
        - Example vector: {""type"", ""minecraft:crafting_shaped"", ""group"", ""bark"", "333", ""444""}
        - Writing example vector to file:
            "type"
            "minecraft:crafting_shaped"
            "group"
            "bark"
            333
            "444"
    7. Close output file.
    ---
    8. Tell user program has finished converting files and they may now safely close the program.

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

namespace fs = std::filesystem; // for convenience

void printInstructionsAndGetInput();
void openFiles();
void readInputFile(const std::string &filename, std::vector<std::string> &alphaNums); // child function of openFiles().
void writeToOutputFile(std::string filename, const std::vector<std::string> &alphaNums); // child function of openFiles().

int main() {
    printInstructionsAndGetInput(); // 1.

    openFiles();

    std::cout << "The program has completed successfully!\n";
    
    return 0;
}


void printInstructionsAndGetInput() {
    const int width = 40;
    const char sym = '-';
    std::cout << std::setfill(sym) << std::setw(width) << "\n"
        << "Hello, thank you for using JSON File Simplifier!\n\n"
        << "Usage Steps:\n"
        << "1. To get started, open the file path this program is located at on your computer.\n"
        << "2. Now move the desired files you would like to simplify into the " << std::quoted("InputFiles") << " directory.\n"
        << "3. Input 'y' when ready or input 'n' to quit: "
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
        - Make sure it includes the current path, InputFiles directory, and the current input filename.

    2. For each file in the InputFiles directory:
        a. Declare the vector that will store the words from the input file.
        b. Declare a string and make it equal the filename WITHIN the directory_entry filename (just different data type).
            - We use .path() so we can use .stem() and .string() on it
            - We use .stem() to extract the filename w/o the extension from it b/c a directory entry looks like: <long_path_here>/filename.extension
            - We use .string() to make it into a string and save it into string variable.
        c. Call the function readInputFile which will read the input file & store the data into the vector, which uses a reference.
            - Once this finishes, the vec will have a all words (enclosed in "") from the inputFilename & the file will be closed.
        d. Call the function writeToOutputFile which will write all the words from the vec (enclosed in "") to new lines in the OutputFiles directory.
*/
void openFiles() {
    // 1.
    fs::path fullInPath = fs::current_path();
    fullInPath /= "InputFiles";

    // 2.
    for (const fs::directory_entry &entry : fs::directory_iterator(fullInPath)) {
        // 2a.
        std::vector<std::string> alphaNums = {};
        // 2b.
        std::string filename = entry.path().stem().string(); // Example: "acacia_boat"
        // 2c.
        readInputFile(filename, alphaNums);
        // 2d.
        writeToOutputFile(filename, alphaNums);
    }
}


/*
Algorithm Steps of this Function:
    1. The filename variable would look like "acacia_boat" for example. We need to make it look like: "InputFiles" + <insert preferred separator> + "acacia_boat" + ".json". THEN we need to open the file.
        - We do this by creating a new string with "InputFiles" then adding the necessary parts to it.
    2. If the file couldn't be opened, print error and stop program.
    3. Define some variables that will be used for getting the proper data from the input file inside the while loop.
    4. Read the file one char at a time.
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
void readInputFile(const std::string &filename, std::vector<std::string> &alphaNums) {
        // 1.
        std::string filename2 = "InputFiles";
        filename2 += fs::path::preferred_separator + filename + ".json";
        std::ifstream inFile(filename2);

        // 2.
        if (!inFile.is_open()) {
            std::cout << filename2 << " couldn't be opened!\n"
                << "Program stopped.\n"
                << "Are you sure you input the files into the "
                << std::quoted("InputFiles") << " directory?\n";
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
        // CODE CURRENTLY CHECKS EVERY CHAR OF FILE ONE AT A TIME.
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

        // Closes File:
        inFile.close();
}

/*
Algorithm Steps of this Function:
    1. Add ".txt" extension to the filename variable.

    2. Open the file path: OutputFiles <insert OS separator symbol(s)> <current filename>

    3. Check if outfile properly opened. If not, give error and exit program.

    4. Write to the output file.
        - For every word in the vec from the function argument, write it into the output file. 1 word per line (enclosed in "").

    5. Close output file.
*/
void writeToOutputFile(std::string filename, const std::vector<std::string> &alphaNums) {
    // 1.
    filename += ".txt";

    // 2.
    std::string outFilename = "OutputFiles";
    outFilename += fs::path::preferred_separator;
    outFilename += filename;

    // OR 
    //fs::path outFilename = "OutputFiles";
    //outFilename / filename;

    std::ofstream outFile(outFilename);

    // 3.
    if (!outFile.is_open()) {
        std::cerr << "\nError! Could not open output file properly.\n"
            << "Program is exiting...\n";
        exit(1);
    }

    // 4.
    for (std::string str : alphaNums) {
        outFile << str << std::endl;
    }

    // 5.
    outFile.close();
}
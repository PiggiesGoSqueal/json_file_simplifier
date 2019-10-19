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
    5. Etc...

Overall Algorithm Steps: (Not numbered in code like other algorithm steps are)
    1. Tell user to input the desired files to be converted into the "InputFiles" folder path.
        - Tell user to input 'Y' when ready or 'n' to exit.
        - Done in printInstructionsAndGetInput()

    2. Open all files one at a time in the file path with the "InputFiles" folder.
        - Done in openFiles()

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

namespace fs = std::filesystem; // for convenience

void printInstructionsAndGetInput();
void openFiles();
void readInputFile(const std::string &entryStr, std::vector<std::string> &alphaNums); // child function of openFiles().
void writeToOutputFile(std::string &outputEntryStr, const std::vector<std::string> &alphaNums); // child function of openFiles().

int main() {
    printInstructionsAndGetInput(); 

    openFiles();

    std::cout << "The program has completed successfully!\n";
    
    return 0;
}


void printInstructionsAndGetInput() {
    const int width = 50;
    const char sym = '-';

    std::cout << std::setfill(sym) << std::setw(width) << "\n"
        << "Hello, thank you for using JSON File Simplifier!\n\n"
        << "Note(s):\n"
        << "- This program does not support decimals in folder names. If you have decimals, change them to '_'.\n"
        << '\n'
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
        - Make sure it includes the current path and InputFiles directory.

    2. For each file (recursively) in the InputFiles directory:
        a. Create a string called outputEntryStr & set it to the entry path.
        b. Check if entry is a directory file type. If so, we want to replace "Input" with "Output" in the ouputEntryStr so later we can create these directories in the "OutputFiles" folder. To do this:
            * Find the position of the 'I' in "InputFiles" in the outputEntryStr then set an int variable to this position.
            * Replace "Input" with "Output" in the outputEntryStr so the total path now looks like this example: /mnt/c/Users/Tom/Desktop/json_file_simplifier/OutputFiles/1_13
            * Now check if the outputEntryStr path exists because that's where we will write output files to. If it doesn't exist, create it.
        c. Create a string called entryStr & set it to the entry path.
    d. Check if entryStr is a regular file. If so:
            * Create an empty vec called alphaNums which will contain all words and numbers for a SINGLE file at a time.
            * Call readInputFile()
            * Call writeToOutputFile()
*/
void openFiles() {
    // 1.
    fs::path fullInPath = fs::current_path();
    fullInPath /= "InputFiles";

    // 2.
    // Note: This for loop line is not my code, got it from a guide. Idk how to do it in my own code.
    for (auto entry = fs::recursive_directory_iterator(fullInPath);
              entry != fs::recursive_directory_iterator(); // (entry does not equal no entry, similar to EOF)
              ++entry ) { // iterate to next entry
        
        // --------------------------
        // outputEntryStr section:
        // --------------------------
        // 2a.
        std::string outputEntryStr = entry->path().string();
        int inputPosInEntry = outputEntryStr.find("InputFiles");
        outputEntryStr.replace(outputEntryStr.begin()+inputPosInEntry, outputEntryStr.begin()+inputPosInEntry+2, "Out");
        
        // 2b.
        if (entry->is_directory()) {
            if (!fs::exists(outputEntryStr)) {
                fs::create_directories(outputEntryStr);
            }
        }

        // ----------------------
        // entryStr section:
        // ----------------------
        // 2c.
        std::string entryStr = entry->path().string(); // Ex 1: /mnt/c/Users/Tom/Desktop/json_file_simplifier/InputFiles/1_13
                                                       // Ex 2: /mnt/c/Users/Tom/Desktop/json_file_simplifier/InputFiles/1_13/acacia_boat.json
        // 2d.
        if (entry->is_regular_file()) {
            std::vector<std::string> alphaNums = {};

            readInputFile(entryStr, alphaNums);
            
            writeToOutputFile(outputEntryStr, alphaNums);
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
void readInputFile(const std::string &entryStr, std::vector<std::string> &alphaNums) {
        // 1. 
        std::ifstream inFile(entryStr);

        // 2.
        if (!inFile.is_open()) {
            std::cout << entryStr << " couldn't be opened!\n"
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
    1. Replace the old extension of the file in outputEntryStr to the desired output file extension (default: txt) if it isn't already.
        - Determine position of last '.' in string and set that to an int variable.
        - Declare a string variable containing the desired output file extension (default: txt).
        - if program does not find the desired output file extension (default: txt) then erase the old extension and set the new one. 
    
    2. Open the filename using the outputEntryStr variable.

    3. Check if outFile properly opened. If not, give error and exit program.

    4. Write to the output file.
        - For every word in the vec from the function argument, write it into the output file. 1 word per line (enclosed in "").

    5. Close output file.
*/
void writeToOutputFile(std::string &outputEntryStr, const std::vector<std::string> &alphaNums) {
    // 1.
    int posOfextensionStart = outputEntryStr.find_last_of('.');
    std::string desiredOutputExtension = "txt";
    
    // Remove old extension if not desired output extension then add desired output extension.
    if (outputEntryStr.find(desiredOutputExtension) == std::string::npos) { // if desired output extension not found in string:
        outputEntryStr.erase(outputEntryStr.begin()+posOfextensionStart+1, outputEntryStr.end());
        outputEntryStr.append(desiredOutputExtension);
    }

    // 2.
    std::ofstream outFile(outputEntryStr);

    // 3.
    if (!outFile.is_open()) {
        std::cerr << "\nError! Could not open output file properly.\n"
            << "Output file path: " << outputEntryStr << '\n'
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
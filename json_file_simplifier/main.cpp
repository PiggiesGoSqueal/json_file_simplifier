/*
Would allow me to download the latest MC folder of JSON recipes then convert all of the JSON recipe files to be this easier to read file using this tool just once. Then can put the changed recipes into the anti-shop-exploits program for it to support it.

Project Name: json_file_simplifier
Project Purpose:
    - Will simplify JSON files to make them easier to read from.
    - Will modify the original file to have the new contents.

Program Name: main.cpp
Author: Tommy Weber
Date: 10/15/2019

Latest Changes on 10/15/2019:
    - Added a way to store number characters that aren't in double quotes but are number(s). Example:
        * "count": 3
        * "id" 335

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
        char c = ' ';
        std::string word;
        int countOfQuotes = 0;
        bool charIsNumOutsideQuotes = false;
        std::string numStr;
        bool prevCharWasNum = false;

        // 4.
        while (inFile.get(c)) {
            // 4a. & 4c.
            if (c == '"') {
                ++countOfQuotes;
            }
            // 4b.
            if (countOfQuotes == 1) {
                word += c;
            }
            // 4d.
            else if (countOfQuotes >= 2) {
                word += c;
                // Add word to the vec (Ex: "minecraft:stone" WITH the "" symbols)
                alphaNums.push_back(word);
                countOfQuotes = 0; // Resets quote count.
                word = ""; // Resets the word variable.
            }

            /*
            NOTE: Apologies if this area is a bit confusing. Was difficult to put into English. Mainly read the goal and you'll know what I wanted to do.

            Goal:
                - The old code (from 10/(13 or 14)/2019) would simplify JSON files so anything inside double quotes is saved while everything else is not. However, this causes a problem because I WANT numbers OUTSIDE of single quotes to be saved as well. This proves difficult though because a number can be multi-digit and we are checking by single characters. Thus we would keep track of:
                    * if the checked char is a number.
                        - if so, keep track of how many following chars are nums.
                        - once the current char is no longer a num, save the string of num digits and reset the string keeping track of nums.

                - In short, the goal is to support storing numbers (that are outside double quotation marks) into the vector as a string, even if it's a multi digit number (ex: 144).

            Algorithm Steps:
                1. If the char being checked IS a number and IS outside of a set of " " symbols, then set a flag (bool) variable to true to notify that there IS a number char outside of quotes.
                    - This will help us determine whether following characters are number digits too (and thus are all one multi-digit number) OR if that number char was alone.
                        * For example, let's say the line says: "type" : "minecraft:crafting" { "count" 345 }
                        * Then program would store into the vec: "type", "minecraft:crafting", and "count". Then it would check 3, see it's a num outside quotation marks, check the next char and see it is also a num, check the next char and see it is also a num. Then it would add all 3 chars to a str and add that to the vec as: 345 (no quotation marks b/c there were none).

                2+. <Refer to comments below>. Easier than trying to remake them up here I think.
            */
            // 1.
            // If char is a number & is outside of quotes, then it will set bool value to true to keep track of it and try to determine if it is a single number or double+ digit number.
            // Then once it has gotten all the digits, it will push it to the vec.
            if (std::isdigit(c) && countOfQuotes == 0) {
                // if char is a number and the immediately previous char was ALSO a number (which is why bool value is already set to true) then add char to numStr.
                if (charIsNumOutsideQuotes == true) {
                    numStr += c;
                }
                // else if the previous char was NOT a number (so bool value is false) then that means this is first char in a row that is a num. So, set bool value to true and add value to fresh numStr variable.
                else if (charIsNumOutsideQuotes == false) {
                    numStr = c; // wipes whatever was in str and sets it to char.
                    charIsNumOutsideQuotes = true;
                }
                prevCharWasNum = true; // Preps for next loop.
            }
            // if previous char was a num but this char is NOT a num, then add the numStr of the previous char number(s) to the vec as it's count of digits is over. Then reset numStr.
            if (prevCharWasNum && !std::isdigit(c)) {
                alphaNums.push_back(numStr);
                numStr = ""; // Resets numStr
                prevCharWasNum = false; // Preps for next loop now that char is no longer a num.
            }
            
            
        }
        
        // Closes File:
        inFile.close();

        // Note:
        // - As soon as loop ends there will be a vec full of the words with no special symbols except the symbols inside a word like: minecraft:diamond or double quotes.
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
    for (std::string wordStr : alphaNums) {
        outFile << wordStr << std::endl;
    }

    // 5.
    outFile.close();
}
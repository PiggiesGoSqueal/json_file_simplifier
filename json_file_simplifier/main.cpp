/*
Would allow me to download the latest MC folder of JSON recipes then convert all of the JSON recipe files to be this easier to read file using this tool just once. Then can put the changed recipes into the anti-shop-exploits program for it to support it.

Project Name: json_file_simplifier
Project Purpose:
    - Will simplify JSON files to make them easier to read from.
    - Will modify the original file to have the new contents.

Program Name: main.cpp
Author: Tommy Weber
Date: 10/13/2019
-------------------------------
Follow Good Practices:
    1. Use algorithm steps (in English) first.
    2. Use compiler flags. 
    3. Use static analyzers to test for bugs.
    4. Use >> and << when dividing and multiplying by 2. Find the file that explains it.
    5. 


MAYBE CONVERT FILES TO .txt AFTER MODIFYING THEM AS THEY ARE NO LONGER JSON FORMAT!!

Algorithm Steps:
    1. Tell user to input the desired files to be converted into the "convert" folder path.
        - Tell user to input 'Y' when ready or 'n' to exit.
    2. Open all files one at a time in the file path with the "convert" folder.
    3. In each file, read all of the lines until EOF then have extract all symbols from file, while leaving letters and numbers (a-z, A-Z, 0-9)
    4. Save file, close it, and move onto next.
        - Repeat until no more files in directory.
    5. Tell user program has finished converting files and they may not safely close the program.
    6. Try creating a .exe file for this so people can easily run it?
    6. Post to Github as pinkpig3777(?), be sure Author is changed to my github name. Make it public.
        - Add a README.md with info on what the program does and how to run it and any requirements / a tutorial on how to use it.

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
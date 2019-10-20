# JSON File Simplifier

# Purpose:
JSON files are difficult to read from, especially without a JSON parser.

Thus I made a program which will allow the user to store JSON files into a certain directory (InputFiles) and run the program. The program would simplify these JSON files in order to make them easier to read with a program later on, then it would save these output files into a different directory (OutputFiles).

### Program allows storing & converting any number of JSON files at once! Just store them all in the "InputFiles" directory.

## Planned Features:
- Make it run faster / more efficiently
- V2: ~~Add support for storing doubles (like 0.0) that are outside of double quotes.~~
- V3: ~~Allow it to recursively find input files within the "InputFiles" directory. And recursively write output files in the "OutputFiles" directory.~~
	* ~~That way, I could just add the "1.13" and "1.14" folders (with all MC recipe files in them) into "InputFiles" and have it do all of them in one run of the program. Would save time.~~

## Bugs:
- It will only get data from files within "InputFiles/<AFolderName>/<filename>.<ext>" but will NOT store the data of files in "InputFiles/<filename>.<ext>" or in a folder depth of greater than 1 folder inside InputFiles. It WILL create the files but they will be empty.

## Example Input File (.json):
	{
	  "type": "minecraft:crafting_shaped",
	  "group": "bark",
	  "pattern": [
		"##",
		"##"
	  ],
	  "key": {
		"#": {
		  "item": "minecraft:oak_log"
		}
	  },
	  "result": {
		"item": "minecraft:oak_wood",
		"count": 333
		"444"
		8910
		"91"23
		55"66"
		...
		{999:}
		100...
		22
		.33
		{0.43}
		2.85
	  }
	}
Notice that there are some lines that are not in JSON format. These are just to point out the differences in output between for differently formatted number lines. Anything that is inside double quotes will remain in double quotes in the output while anything outside double quotes will continue to have no double quotes in the output.

## Example Output File (.txt):
	"type"
	"minecraft:crafting_shaped"
	"group"
	"bark"
	"pattern"
	"##"
	"##"
	"key"
	"#"
	"item"
	"minecraft:oak_log"
	"result"
	"item"
	"minecraft:oak_wood"
	"count"
	333
	"444"
	8910
	"91"
	23
	55
	"66"
	999
	10
	22
	33
	0.43
	2.85

## Requirements to Run:
	For Windows:
		- Make sure you have Windows Sub Linux (WSL) set up. If you don't know what that is and you're using Windows, I'm not sure if/how you can run this.
		- Type "bash" in terminal.
		- Open the correct path in terminal by typing: "cd <yourPath>"
			* For example: cd Downloads\json_file_simplifier
		- Type: "./a.out" (without the quotes)
		- The program will run in terminal. Follow the instructions then input 'y' and hit Enter when ready.
		- The program will simplify all json files stored in the "InputFiles" directory and output their simplified versions to the "OutputFiles" directory.
		
	For Linux:
		- Open terminal.
		- Type: "./a.out" (without the quotes)
		- The program will run in terminal. Follow the instructions then input 'y' and hit Enter when ready.
		- The program will simplify all json files stored in the "InputFiles" directory and output their simplified versions to the "OutputFiles" directory.
		
	For MAC:
		- No idea if it is possible or not as I've never used MAC before.

## Requirements for Compiling the Program Yourself:
- A C++ compiler that supports C++17 features.
- Windows with WSL or Linux I think.

## Other Information:
- The date of creating this program was 10/13/2019 and is intended to be used with my Anti-Shop-Exploits program to convert JSON files to easier to read files that can be read by the program. With the way I intend to use it, I will only need to run this program once per Minecraft version that comes out (1.13, 1.14, 1.15, etc) then I can just store the output files into the Anti-Shop-Exploits directory recipes section.	

# Terms of Use
- You may run the program for your own needs, as well as, modify it. 
- You may not sell this code, program, or any modified version of this code or program. 
- You may not take credit for the original code or program.
- You must give proper credit to the original code if you modify it.
- Ask me if you have questions regarding this. I've never made a license or Terms of Use for software so this may not be entirely clear.
- You acknowledge that I may modify my terms of use at any time. However, this is highly unlikely with the exception of small changes to make it easier to understand.
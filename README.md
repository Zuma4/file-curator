# file curator
command line-based program for managing and organizing your files in bulk.

## Main Features
- Organize and create subdirectories for files based on their attributes, such as extensions, names, or size.
- Search and filter files in a given directory based on their attributes. can be used for collecting assets (:
- Copy, Move, and Delete specific files in bulk.

## Side Features
- save filtered file paths for later use.
- using cookies to store your chosen options between sessions for fast retrieval.

## Supported Operating Systems
binary available for Windows

you can compile the program in your preferred operating system.

since there are no platform-specific APIs used

## Compiling Requirements
C++17 compiler or above

## Getting Started:

The program provides three main options:

### Explore and collect files
Specify one or more directories to explore.

The program will search for files based on user-defined criteria (extensions, name, size).

You can then choose to save the collected files.

### Organize files
Specify the directory containing the files you want to organize.

Choose one of two organizational methods
- Organize based on extensions: Each file extension will be placed in a separate subdirectory.
- Organize based on user-defined groups: You can create groups based on specific criteria (e.g., "photos" group containing all files with extensions .jpg and .png).

The program will move your files into the appropriate subdirectories.

### Managing collected files
Enter the path to the text file containing the file paths.

The program will import all valid files from the text file.

You can then choose to copy, move, or delete the imported files.

## Additional Notes
The program is still under development, and new features and improvements may be added in the future.

any error reporting will be appreciated.

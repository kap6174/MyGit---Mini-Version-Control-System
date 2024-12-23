# AOS Assignment 4 - Mini Version Control System (MVCS)

Junaid Ahmed  
Roll No: 2024201018  
M.Tech CSE  

---

This project involves developing a mini version control system (MVCS) in C++ that mimics basic functionalities of Git. In a gist the system allows users to initialize a repository, add files, commit changes, and checkout specific commits. The goal is to implement the core concepts of version control systems and how they manage file changes over time.

### Key Features

- **Initialize Repository**: Creates a hidden `.mygit` directory with the necessary structure for version control.
- **Hash-Object**: Calculates the SHA-1 hash of a file, compresses it, and optionally stores it as a blob object in the repository.
- **Cat-File**: Displays the content, size, or type of a file stored as an object using its SHA-1 hash.
- **Write Tree**: Writes the current directory structure to a tree object, representing the hierarchy of files and directories.
- **List Tree**: Displays the current directory structure of the tree object you wrote using write-tree.
- **Add Command**: Stages changes for the next commit by updating the index file with file metadata.
- **Commit Command**: Saves a snapshot of the current state of the repository along with metadata.
- **Log Command**: Displays the commit history from the latest commit to the oldest.
- **Checkout Command**: Restores files from a specific commit.

## MyGit Structure
```markdown
.mygit/
├── objects/
│   └── <blob_hash>
├── trees/
│   └── <tree_hash>
├── commits/
│   └── <commit_hash>
├── HEAD
├── index
└── log
```
### Dependencies

This mini VCS requires the following:

- C++11 or later
- CMake (if using for build automation)
- Standard C++ libraries
- Filesystem library (C++17)

## Building and Running the MVCS

To compile and run the mini version control system, a Makefile is provided. Follow the steps below:

## Building the Program


### 1. Compile the Program
Run the following command in your terminal:
```bash
make
```
This will use the Makefile to compile all the source files and create the executable `mygit`.

### 2. Clean Up
To remove all object files and the compiled executable, use:
```bash
make clean
```

## Running the Program

### Initialize Repository
To create a new repository, use:
```bash
./mygit init
```

### Hash Object
To calculate the SHA-1 hash of a file and store it, use:
```bash
echo -n "hello world" > test.txt
./mygit hash-object -w test.txt
```

### Cat File
To read and display the content or metadata of a file stored as an object

○ -p: Print the actual content of the file.

○ -s: Display the file size in bytes.

○ -t: Show the type of object (e.g., blob for file content, tree for directory)., use:
```bash
./mygit cat-file -p <file/tree_sha>
```

### Write Tree
Writes the current directory structure to a tree object, which represents the hierarchy
of files and directories, use:
```bash
./mygit write-tree
```
### List Tree
Lists the contents of a tree object (directory) using its SHA-1 hash.
● Displays detailed information about the files and subdirectories or just their names when
using --name-only, use:
```bash
./mygit ls-tree [--name-only] <tree_sha>
```

### Add Command
Adds files or directories to the staging area, preparing them for the next commit, use:
```bash
./mygit add .
./mygit add main.cpp utils.cpp
```

### Commit Command
Creates a commit object, representing a snapshot of the staged changes, and
updates the repository’s history, use:
```bash
./mygit commit -m "Commit message"
./mygit commit
```

### Log Command
Displays the commit history from the latest commit to the oldest, use:
```bash
./mygit log
```

### Checkout Command
Checks out a specific commit, restoring the state of the project as it was at that
commit, use
```bash
./mygit checkout <commit_sha>
```

# Conclusion

The mini version control system provides foundational functionality for managing file changes and tracking versions over time. Each command is modular and designed to be extensible for future features.
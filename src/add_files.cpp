#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include "hash_object.h"
#include "write_tree.h"

void update_index(const string& index_file, const string& entry) {
    ofstream outFile(index_file, ios::app);  // Open in append mode
    if (outFile) {
        outFile << entry << endl;  // Write the entry to the index file
    } else {
        cerr << "Error writing to index file." << endl;
    }
}

void add_files(const vector<string>& files) 
{
    const string index_file = ".mygit/index";

    // If adding all files
    if (files.size() == 1 && files[0] == ".") 
    {
        if (filesystem::exists(index_file)) 
        {
            // Clear the contents of the index file if it exists
            ofstream outFile(index_file, ios::trunc); // Open in truncate mode to clear content
            if (!outFile) 
            {
                cerr << "Error: Unable to clear index file." << endl;
                return;
            }
        } 
        // Call write_tree to get the tree hash
        string tree_hash = write_tree(".");
        string entry = "040000 tree " + tree_hash + " .";  // Add the root entry

        update_index(index_file, entry);

        // Recursively add all files and directories
        for (const auto& entry : filesystem::directory_iterator("."))   
        {
            string path = entry.path().string();
            string filename = entry.path().filename().string();

            if (filename == "." or filename == ".." or filename == ".git" or filename == ".mygit" or filename == ".vscode" or filename == "mygit") 
            {
                continue;  // Skip hidden files and the mygit executable
            }

            if (filesystem::is_regular_file(path)) 
            {
                string sha = hash_object(path, true);
                string file_entry = "100644 blob " + sha + " " + filename;
                update_index(index_file, file_entry);
            } 
            else if (filesystem::is_directory(path)) 
            {
                string dir_hash = write_tree(path);  // Call write_tree for directories
                string dir_entry = "040000 tree " + dir_hash + " " + filename;
                update_index(index_file, dir_entry);
            }
        }

    } 
    else 
    {  // If adding specific files
        ifstream index_stream(index_file);
        index_stream.seekg(0, ios::end);
        bool index_empty = (index_stream.tellg() == 0); //Checks if index is empty

        // If index is empty, call write_tree
        if (index_empty) 
        {
            string tree_hash = write_tree(".");  // Get the initial tree hash
            update_index(index_file, "040000 tree " + tree_hash + " .");
        }

        // Process each specified file
        for (const string& file : files) 
        {
            if (filesystem::exists(file)) 
            {
                string sha = hash_object(file, true);
                string file_entry = "100644 blob " + sha + " " + file;
                update_index(index_file, file_entry);
            } 
            else 
            {
                cerr << "File " << file << " does not exist." << endl;
            }
        }
    }
}

#include <iostream>
#include <fstream>
#include <filesystem>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include <vector>
#include <zlib.h>
#include "hash_object.h"

// Function to get file content
string get_file_content(const string& path) 
{
    ifstream file(path, ios::binary);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void create_tree(const string& hash, const string& data) 
{
    // Ensure the .mygit/trees directory exists
    filesystem::create_directories(".mygit/trees");
    
    // Define the file path for the tree object
    string filePath = ".mygit/trees/" + hash;
    ofstream outFile(filePath, ios::binary);  // Open in binary mode for compressed data
    if (outFile) 
    {
        // Prepare for compression
        uLongf compressedSize = compressBound(data.size());  // Estimate size of compressed data
        char* compressedData = new char[compressedSize];
        
        // Compress the data using zlib
        if (compress(reinterpret_cast<Bytef*>(compressedData), &compressedSize, 
                     reinterpret_cast<const Bytef*>(data.c_str()), data.size()) == Z_OK) 
        {
            // Write compressed data to the file
            outFile.write(compressedData, compressedSize);
        } 
        else 
        {
            cerr << "Error: Compression failed." << endl;
        }

        // Clean up
        delete[] compressedData;
        outFile.close();
    } 
    else 
    {
        cerr << "Error writing tree object to repository." << endl;
    }
}

// Recursively write the tree structure and return its SHA-1 hash
string write_tree(const string& dir_path) 
{
    string tree_content;
    vector<pair<string, string>> entries;  // Store (SHA-1, filename)

    // Iterate over the files and directories in the current directory
    for (const auto& entry : filesystem::directory_iterator(dir_path)) 
    {
        string path = entry.path().string();
        string filename = entry.path().filename().string();

        if (filename == "." or filename == ".." or filename == ".git" or filename == ".mygit" or filename == ".vscode" or filename == "mygit")
        {
            continue;
        }

        if (filesystem::is_regular_file(path)) 
        {
            // For files, write them as blobs
            string file_content = get_file_content(path);
            string file_sha1 = hash_object(path, true);  // Create a blob and get its SHA-1 hash
            string mode = "100644";  // Regular file mode
            entries.push_back(make_pair(file_sha1, filename));

            // Append file details to tree content
            tree_content += mode + " blob " + file_sha1 + " " + filename + "\n";
        } 
        else if (filesystem::is_directory(path)) 
        {
            // For directories, recursively write them as trees
            string tree_sha1 = write_tree(path);  // Recursively get tree SHA-1
            string mode = "040000";  // Directory mode
            entries.push_back(make_pair(tree_sha1, filename));

            // Append directory details to tree content
            tree_content += mode + " tree " + tree_sha1 + " " + filename + "\n";
        }
    }

    // Calculate the SHA-1 hash of the entire tree content
    string tree_sha1 = calculate_sha1(tree_content);

    // Write the tree object to the .mygit/trees directory
    create_tree(tree_sha1, tree_content);

    return tree_sha1;
}







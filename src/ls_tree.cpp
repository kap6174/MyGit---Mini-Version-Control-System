#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <zlib.h>
#include <vector>
#include <string>
#include "ls_tree.h"

struct TreeEntry {
    string mode;
    string type;
    string sha;
    string name;
};

// Decompress the compressed data
string decompress_data(const string& compressed_data) 
{
    uLongf decompressed_size = compressed_data.size() * 5;  // Estimated size
    char* decompressed_data = new char[decompressed_size];
    
    if (uncompress(reinterpret_cast<Bytef*>(decompressed_data), &decompressed_size, 
                   reinterpret_cast<const Bytef*>(compressed_data.c_str()), compressed_data.size()) == Z_OK) 
    {
        string result(decompressed_data, decompressed_size);
        delete[] decompressed_data;
        return result;
    } 
    else 
    {
        cerr << "Error decompressing data" << endl;
        delete[] decompressed_data;
        return "";
    }
}

// Parse tree entries from decompressed data
vector<TreeEntry> parse_tree_entries(const string& tree_content) 
{
    vector<TreeEntry> entries;
    stringstream ss(tree_content);
    string line;

    while (getline(ss, line)) 
    {
        TreeEntry entry;
        stringstream line_ss(line);
        line_ss >> entry.mode >> entry.type >> entry.sha >> entry.name;
        entries.push_back(entry);
    }

    return entries;
}

// List the contents of a tree object
void ls_tree(const string& tree_sha, bool name_only) 
{
    string tree_path = ".mygit/trees/" + tree_sha;
    
    ifstream file(tree_path, ios::binary);
    if (!file) 
    {
        cerr << "Error: Tree object with SHA " << tree_sha << " not found." << endl;
        return;
    }

    // Read compressed data from the file
    stringstream buffer;
    buffer << file.rdbuf();
    string compressed_data = buffer.str();

    // Decompress the tree content
    string tree_content = decompress_data(compressed_data);
    if (tree_content.empty()) 
    {
        return;
    }

    // Parse tree entries
    vector<TreeEntry> entries = parse_tree_entries(tree_content);

    // Display entries according to `--name-only` flag
    for (const auto& entry : entries) 
    {
        if (name_only) 
        {
            // Display only the names, with directories indicated with a trailing "/"
            cout << entry.name << (entry.type == "tree" ? "/" : "") << endl;
        } 
        else 
        {
            // Display mode, type, SHA, and name
            cout << entry.mode << " " << entry.type << " " << entry.sha << " " << entry.name << endl;
        }
    }
}

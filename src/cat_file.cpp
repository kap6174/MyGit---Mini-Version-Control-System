#include <iostream>
#include <fstream>
#include <iomanip>
#include <zlib.h>
#include <filesystem>
#include <sstream>
#include "cat_file.h"

using namespace std;

string read_object(const string& path) 
{
    // Open the file for reading
    ifstream inFile(path, ios::binary);
    if (!inFile) 
    {
        cerr << "Error: Object not found at path " << path << endl;
        return "";
    }

    // Read compressed file content
    stringstream buf;
    buf << inFile.rdbuf();
    string compressed = buf.str();
    inFile.close();

    // Decompress data with zlib
    uLongf decomp_size = compressed.size() * 10;  // Estimate decompressed size
    char* decomp = new char[decomp_size];
    if (uncompress(reinterpret_cast<Bytef*>(decomp), &decomp_size, 
                   reinterpret_cast<const Bytef*>(compressed.c_str()), compressed.size()) != Z_OK) 
    {
        cerr << "Error decompressing object." << endl;
        delete[] decomp;
        return "";
    }

    string res(decomp, decomp_size);
    delete[] decomp;
    return res;
}

void cat_file(const string& f, const string& h) 
{
    // Paths to the blob and tree objects
    string blob_path = ".mygit/objects/" + h;
    string tree_path = ".mygit/trees/" + h;

    // Determine the object type
    bool is_blob = filesystem::exists(blob_path);
    bool is_tree = filesystem::exists(tree_path);

    if (!is_blob && !is_tree) 
    {
        cerr << "Error: Object with hash " << h << " not found." << endl;
        return;
    }

    // Fetch the content
    string content;
    if (is_blob) 
    {
        content = read_object(blob_path);
    } 
    else if (is_tree) 
    {
        content = read_object(tree_path);
    }

    // Handle flags
    if (f == "-p") 
    {
        if (content.empty()) return;
        // Print content
        cout << content << endl;
    } 
    else if (f == "-s") 
    {
        if (content.empty()) return;
        // Print file size
        cout << "File size: " << content.size() << " bytes" << endl;
    } 
    else if (f == "-t") 
    {
        if (is_tree) 
        {
            cout << "Object type: tree" << endl;
        } 
        else if (is_blob) 
        {
            cout << "Object type: blob" << endl;
        }
    } 
    else 
    {
        cerr << "Error: Unknown flag " << f << endl;
    }
}

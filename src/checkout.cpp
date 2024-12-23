#include "checkout.h"
#include <iostream>
#include <fstream>
#include <zlib.h>
#include <filesystem>

string decompress_data_checkout(const string& compressed_data) 
{
    // Estimate decompressed size based on compressed data size
    uLongf decompressed_size = compressed_data.size() * 5;
    char* decompressed_buffer = new char[decompressed_size];

    // Decompress using zlib's uncompress function
    int result = uncompress(reinterpret_cast<Bytef*>(decompressed_buffer), &decompressed_size, 
                            reinterpret_cast<const Bytef*>(compressed_data.c_str()), compressed_data.size());

    // Check if decompression succeeded
    if (result == Z_OK) 
    {
        string decompressed_data(decompressed_buffer, decompressed_size);
        delete[] decompressed_buffer;
        return decompressed_data;
    } 
    else 
    {
        cerr << "Error decompressing data" << endl;
        delete[] decompressed_buffer;
        return "";
    }
}

void restore_object(const string& permission, const string& type, const string& object_hash, const string& path) 
{
    string object_folder = type == "blob" ? ".mygit/objects/" : ".mygit/trees/";
    string object_path = object_folder + object_hash;

    // Open the compressed file
    ifstream file(object_path, ios::binary);
    if (!file) 
    {
        cerr << "Error: Could not open file " << object_path << endl;
        return;
    }

    // Read compressed data from the file
    stringstream buffer;
    buffer << file.rdbuf();
    string compressed_data = buffer.str();
    file.close();

    // Decompress the data
    string decompressed_data = decompress_data_checkout(compressed_data);
    // if (decompressed_data.empty()) 
    // {
    //     cerr << "Error: Decompression failed for object " << object_hash << endl;
    //     return;
    // }

    if (type == "blob") 
    {
        // Handle blobs (files)
        if (filesystem::exists(path)) 
        {
            filesystem::remove(path);
        }

        ofstream file(path);
        file << decompressed_data;  // Decompress blob content and write
        file.close();

        // Set file permissions
        if (permission == "100644") 
        {
            filesystem::permissions(path, filesystem::perms::owner_read | filesystem::perms::owner_write |
                                             filesystem::perms::group_read | filesystem::perms::others_read);
        } 
        else if (permission == "100755") 
        {
            filesystem::permissions(path, filesystem::perms::owner_all | filesystem::perms::group_read | 
                                             filesystem::perms::others_read);
        } 
        else 
        {
            cerr << "Warning: Unrecognized file permission format: " << permission << "\n";
        }

    } 
    else if (type == "tree") 
    {
        // Handle trees (directories)
        if (path != ".") 
        {
            if (filesystem::exists(path)) 
            {
                filesystem::remove_all(path);
            }
            filesystem::create_directory(path);
        }

        // Set directory permissions
        filesystem::permissions(path, filesystem::perms::owner_all | filesystem::perms::group_read | 
                                       filesystem::perms::others_read);

        istringstream tree_stream(decompressed_data);
        string entry;

        while (getline(tree_stream, entry)) 
        {
            istringstream iss(entry);
            string sub_permission, sub_type, sub_hash, sub_name;
            iss >> sub_permission >> sub_type >> sub_hash >> sub_name;
            restore_object(sub_permission, sub_type, sub_hash, path + "/" + sub_name);
        }
    }
}



void checkout_commit(const string& commit_sha) 
{
    string commit_path = ".mygit/commits/" + commit_sha;
    
    // Check if commit exists
    if (!filesystem::exists(commit_path)) 
    {
        cerr << "Error: Commit SHA not found.\n";
        return;
    }

    // Clear the working directory, ignoring specific folders/files
    for (const auto& entry : filesystem::directory_iterator(".")) 
    {
        string filename = entry.path().filename().string();
        if (filename == "." or filename == ".." or filename == ".git" or filename == ".mygit" or filename == ".vscode" or filename == "mygit") 
            continue;
        
        filesystem::remove_all(entry);
    }

    // Read and parse the commit object
    ifstream commit_file(commit_path);
    string line;
    bool is_first_entry = true;

    while (getline(commit_file, line)) 
    {
        string permission, type, object_hash, path;
        istringstream iss(line);
        iss >> permission >> type >> object_hash >> path;

        // Restore the object based on type
        if (is_first_entry) 
        {
            restore_object(permission, type, object_hash, path);
            is_first_entry = false;
        } 
        else 
        {
            restore_object(permission, type, object_hash, "./" + path);
        }
    }

    // Update HEAD
    ofstream head_file(".mygit/HEAD", ios::trunc);
    head_file << commit_sha;
}

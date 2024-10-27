#include <iostream>
#include <fstream>
#include <sstream>
#include <openssl/sha.h>
#include <iomanip>
#include <zlib.h>
#include <filesystem>
#include "hash_object.h"


string calculate_sha1(const string& data) 
{
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);
    
    // Convert hash to hex string
    stringstream ss;
    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i) 
    {
        ss << hex << setw(2) << setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

void write_blob(const string& hash, const string& data) 
{
    // Create the .mygit/objects directory if it doesn't exist
    filesystem::create_directories(".mygit/objects");
    
    // Create a file for the blob
    string filePath = ".mygit/objects/" + hash;
    ofstream outFile(filePath, ios::binary);
    if (outFile) 
    {
        // Compress the data using zlib
        uLongf compressedSize = compressBound(data.size());
        char* compressedData = new char[compressedSize];
        if (compress(reinterpret_cast<Bytef*>(compressedData), &compressedSize, 
                     reinterpret_cast<const Bytef*>(data.c_str()), data.size()) == Z_OK) 
        {
            outFile.write(compressedData, compressedSize);
        }
        delete[] compressedData;
    } 
    else 
    {
        cerr << "Error writing blob to repository." << endl;
    }
}

string hash_object(const string& fileName, bool writeFlag) 
{
    ifstream file(fileName);
    if (!file) 
    {
        cerr << "Error opening file: " << fileName << endl;
        return "";
    }

    // Read the contents of the file
    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();

    // Calculate the SHA-1 hash
    string hash = calculate_sha1(content);

    if (writeFlag) 
    {
        write_blob(hash, content);
    }

    return hash;
}

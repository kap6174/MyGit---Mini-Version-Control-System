#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>
#include "hash_object.h"

// Helper function to get the index file content
string get_index_file_content(const string& path) 
{
    ifstream file(path);
    if (!file) 
    {
        cerr << "Error opening file: " << path << endl;
        return "";
    }

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Helper function to get the current timestamp
string get_timestamp() 
{
    auto now = chrono::system_clock::now();
    auto in_time_t = chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

// Helper function to get the committer's name
string get_committer_name() 
{
    const char* username = getenv("USER");
    return username ? string(username) : "Unknown User";
}

void commit_changes(const string& message) 
{
    const string index_path = ".mygit/index";
    const string commits_path = ".mygit/commits";
    const string log_path = ".mygit/log";
    const string head_path = ".mygit/HEAD";
    
    // Calculate SHA for the index file
    string index_content = get_index_file_content(index_path);
    string commit_sha = calculate_sha1(index_content);
    
    // Save index content to .mygit/commits/{commit_sha}
    ofstream commit_file(commits_path + "/" + commit_sha);
    commit_file << index_content;
    commit_file.close();

    // Read parent SHA from HEAD (if it exists)
    string parent_sha;
    ifstream head_file(head_path);
    getline(head_file, parent_sha);
    head_file.close();

    // Prepare log entry details
    string log_entry = "Commit SHA: " + commit_sha + "\n";
    log_entry += "Parent SHA: " + (parent_sha.empty() ? "None" : parent_sha) + "\n";
    log_entry += "Commit Message: " + (message.empty() ? "Default commit message" : message) + "\n";
    log_entry += "Timestamp: " + get_timestamp() + "\n";
    log_entry += "Committer: " + get_committer_name() + "\n\n";

    // Append log entry to the log file
    ofstream log_file(log_path, ios::app);
    log_file << log_entry;
    log_file.close();

    // Update HEAD with new commit SHA
    ofstream new_head_file(head_path, ios::trunc);
    new_head_file << commit_sha;
    new_head_file.close();

    // Clear the index file
    ofstream clear_index_file(index_path, ios::trunc);
    clear_index_file.close();

    cout << "Commit SHA: " << commit_sha << endl;
}

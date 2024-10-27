#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void print_log() 
{
    const string log_file = ".mygit/log";
    ifstream file(log_file);
    
    if (!file) 
    {
        cerr << "Error opening log file." << endl;
        return;
    }

    string line;
    vector<vector<string>> log_entries;  // To hold each commit's details

    // Read the log file and store each commit's details
    while (getline(file, line)) 
    {
        if (line.empty()) continue;  // Skip empty lines

        // Create a new entry for each commit's details
        vector<string> commit_entry;

        // Read the next 5 lines (commit details)
        commit_entry.push_back(line);  // Commit SHA
        getline(file, line); 
        commit_entry.push_back(line);  // Parent SHA
        getline(file, line); 
        commit_entry.push_back(line);  // Commit Message
        getline(file, line); 
        commit_entry.push_back(line);  // Timestamp
        getline(file, line); 
        commit_entry.push_back(line);  // Committer

        // Store the complete commit entry
        log_entries.push_back(commit_entry);
    }

    file.close();

    // Print the log entries in reverse order
    for (auto it = log_entries.rbegin(); it != log_entries.rend(); ++it) 
    {
        for (const auto& detail : *it) 
        {
            cout << detail << endl;  // Print each detail of the commit
        }
        cout << endl;  // Add a newline for spacing between commits
    }
}

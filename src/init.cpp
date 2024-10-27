#include <iostream>
#include <filesystem>
#include <fstream>
#include "init.h"

using namespace std;

void init_repository() 
{
    // Define the directory name for the repository
    const string repo_dir = ".mygit";

    // Create the .mygit directory if it doesn't already exist
    if (!filesystem::exists(repo_dir)) 
    {
        // Create the .mygit directory
        filesystem::create_directory(repo_dir);
        cout << "Initialized empty Git repository!" << endl;

        // Create necessary subdirectories
        filesystem::create_directory(repo_dir + "/objects");
        filesystem::create_directory(repo_dir + "/trees");
        filesystem::create_directory(repo_dir + "/commits");
        ofstream log_file(repo_dir + "/log");
        ofstream file(repo_dir + "/index");
        
        // You can add more subdirectories as needed
    } 
    else 
    {
        cout << "Repository already exists!" << endl;
    }
}

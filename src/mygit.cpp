#include <iostream>
#include <string>
#include <cstring>
#include "init.h"
#include "hash_object.h"
#include "cat_file.h"
#include "write_tree.h"
#include "ls_tree.h"
#include "add_files.h"
#include "commit.h"
#include "log.h"  
#include "checkout.h" 

using namespace std;

int main(int argc, char* argv[])
{
    if (argc > 1) 
    {
        string command = argv[1];

        if (command == "init") 
        {
            init_repository();
        }
        else if (command == "hash-object") 
        {
            if (argc < 3) 
            {
                cerr << "Usage: " << argv[0] << " hash-object [-w] <file>" << endl;
                return 1;
            }
            bool writeFlag = false;
            string fileName;

            if (argc == 4 && string(argv[2]) == "-w") 
            {
                writeFlag = true;
                fileName = argv[3];
            } 
            else 
            {
                fileName = argv[2];
            }

            string hash = hash_object(fileName, writeFlag);
            cout << hash << endl;
        }
        else if (command == "cat-file") 
        {
            if (argc != 4) 
            {
                cerr << "Usage: ./mygit cat-file <flag> <file_sha>" << endl;
                return 1;
            }
            string flag = argv[2];
            string fileSha = argv[3];
            cat_file(flag, fileSha);
        }
        else if (command == "write-tree") 
        {
            if (argc != 2) 
            {
                cerr << "Usage: ./mygit write-tree" << endl;
                return 1;
            }
            string tree_sha = write_tree(".");
            cout << "Tree SHA: " << tree_sha << endl;
        }
        else if (command == "ls-tree") 
        {
            if (argc < 3) 
            {
                cerr << "Usage: ./mygit ls-tree [--name-only] <tree_sha>" << endl;
                return 1;
            }
        
            bool name_only = false;
            string tree_sha;
        
            if (string(argv[2]) == "--name-only") 
            {
                name_only = true;
                tree_sha = argv[3];
            } 
            else 
            {
                tree_sha = argv[2];
            }
        
            ls_tree(tree_sha, name_only);
        }
        else if (command == "add") 
        {
            vector<string> files;
            for (int i = 2; i < argc; ++i) {
                files.push_back(argv[i]);
            }
            add_files(files);
        }
        else if (command == "commit") 
        {
            string message;

            if (argc > 2 && string(argv[2]) == "-m" && argc == 4) 
            {
                message = argv[3];
            }

            commit_changes(message);
        }
        else if (command == "log")
        {
            print_log();
        }
        else if (command == "checkout")
        {
            if (argc != 3) 
            {
                cerr << "Usage: ./mygit checkout <commit_sha>" << endl;
                return 1;
            }
            string commit_sha = argv[2];
            checkout_commit(commit_sha);
        }
        else 
        {
            cerr << "Unknown command: " << command << endl;
            return 1;
        }
    }
    
    else
    {
        cerr << "Usage: " << argv[0] << " <command>" << endl;
        return 1;
    }

    return 0;
}
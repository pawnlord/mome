#include <iostream>
#include <string>
#include <string.h>
#include "../include/Repo.h"
typedef enum  {
    NOT_A_COMMAND = -1, NON_REPO_RELATED = 0, GET_INFO, LOG, COMMIT, INIT
} command;

command getCommand(char** args){
    if(strcmp(args[1], "help") == 0 || strcmp(args[1], "h") == 0) {
        std::cout << "Mome is a git clone kinda deal. Current commands are: " << std::endl;
     
        std::cout << " - help/h: get help.\n - version/v: get version." << std::endl;
     
        std::cout << " - log/l: get log of current repo.\n - commit/c: add current state to the repos state" << std::endl;

        std::cout << " - info/i: get repo info.\n - init: initialize repo." << std::endl;

        return NON_REPO_RELATED;
    } else if(strcmp(args[1], "version") == 0 || strcmp(args[1], "v") == 0) {
        std::cout << "Mome, by pawnlord (github), version 0.0.1" << std::endl;
     
        return NON_REPO_RELATED;
    } else if(strcmp(args[1], "info") == 0 || strcmp(args[1], "i") == 0) {
        return GET_INFO;
    }else if(strcmp(args[1], "init") == 0) {
        return INIT;
    }
    return NOT_A_COMMAND;
}

int main(int argc, char** args){
    if(argc == 1) {
        std::cout << "Welcome to Mome! A version control system for momes.\nUse help or h for more info" << std::endl; 
    } else {
        command c = getCommand(args);
        if(c == GET_INFO) {
            Repo r;
            std::cout << r.formatInfo("") << std::endl;
        } else if(c == INIT) {
            Repo r(true);
        } else if(c == NOT_A_COMMAND) {
            std::cout << "Invalid Arguements: " << args[1] << std::endl;
        }
    }
    
    return 0;
}
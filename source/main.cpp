#include <iostream>
#include <string>
#include <string.h>
#include "../include/Repo.h"
typedef enum  {
    NOT_A_COMMAND = -1, NON_REPO_RELATED = 0, GET_INFO, LOG, COMMIT, INIT, CLI
} command;

command getCommand(char** args){
    if(strcmp(args[1], "help") == 0 || strcmp(args[1], "h") == 0) {
        // information on commands
       std::cout << "Mome is a git clone kinda deal. Current commands are: " << std::endl;
     
        std::cout << " - help/h: get help.\n - version/v: get version." << std::endl;
     
        std::cout << " - log/l: get log of current repo.\n - commit/c: add current state to the repos state" << std::endl;

        std::cout << " - info/i: get repo info.\n - init: initialize repo." << std::endl;
       
        std::cout << " - commandline/cli: run a continuous command-line interface" << std::endl;

        // tell the program it doesn't need to do more
        return NON_REPO_RELATED;
    } else if(strcmp(args[1], "version") == 0 || strcmp(args[1], "v") == 0) {
        // give version info
        std::cout << "Mome, by pawnlord (github), version 0.0.1" << std::endl;
     
        return NON_REPO_RELATED;
    } else if(strcmp(args[1], "info") == 0 || strcmp(args[1], "i") == 0) {
        // get info about current repo (if any)
        return GET_INFO;
    } else if(strcmp(args[1], "init") == 0) {
        // initialize a repo
        return INIT;
    } else if(strcmp(args[1], "commandline") == 0 || strcmp(args[1], "cli") == 0) {
        // enter CLI mode
        return CLI;
    } else if(strcmp(args[1], "commit") == 0 || strcmp(args[1], "c") == 0) {
        // commit to current Repo
        return COMMIT;
    }
    // something went wrong
    return NOT_A_COMMAND;
}
command getCliCommand(std::string args[]){
    if(args[0].compare("help") == 0 || args[0].compare("h") == 0) {
        // information on commands
        std::cout << "Mome is a git clone kinda deal. Current commands are: " << std::endl;
     
        std::cout << " - help/h: get help.\n - version/v: get version." << std::endl;
     
        std::cout << " - log/l: get log of current repo.\n - commit/c: add current state to the repos state" << std::endl;

        std::cout << " - info/i: get repo info.\n - init: initialize repo." << std::endl;
        
        std::cout << " - commandline/cli: run a continuous command-line interface (not available in cli-mode)" << std::endl;
        
        // tell the program it doesn't need to do more
        return NON_REPO_RELATED;
    } else if(args[0].compare("version") == 0 || args[0].compare("v") == 0) {
        // give version info
        std::cout << "Mome, by pawnlord (github), version 0.0.1" << std::endl;
        
        return NON_REPO_RELATED;
    } else if(args[0].compare("info") == 0 || args[0].compare("i") == 0) {
        // get info about current repo (if any)
        return GET_INFO;
    } else if(args[0].compare("init") == 0) {
        // initialize a repo
        return INIT;
    } else if(args[0].compare("commit") == 0 || args[0].compare("c") == 0) {
        // commit to current Repo
        return COMMIT;
    }
    // something went wrong
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
        
        } else if(c == CLI) {
            std::cout << "Welcome to the Mome CLI! A version control system for momes." << std::endl;
            std::cout << "Use help or h for more info" << std::endl;
            std::cout << "Use the same commands, but without typing 'mome' infront of all of them!" << std::endl;
            std::cout << "exit using the 'exit' command" << std::endl;
            std::string input;
            while(input != "exit"){
                std::string input_args[50] = {""};
                std::cout << "[mome_cli]$ ";
                
                getline(std::cin, input);
            
                int current_arg = 0;
                for(int i = 0; input[i] != 0; i++){
            
                    if(input[i] == ' '){
                        //std::cout << input_args[current_arg] << std::endl;
            
                        current_arg++;
                    } else if (input[i] != '\n') {
                        input_args[current_arg] += input[i];
            
                        //std::cout << input_args[current_arg];
            
                    }
                }
            
                input_args[current_arg+1] = ""; 
                c = getCliCommand(input_args);
                if(c == GET_INFO) {
                    Repo r;
                
                    std::cout << r.formatInfo("") << std::endl;
                } else if(c == INIT) {
                    Repo r(true);
                } else if(c == COMMIT) {
                    Repo r;
                    r.addCommit(args+2);
                    std::cout << "Added Commit" << std::endl;
                } else if(c == NOT_A_COMMAND && input != "exit") {
                    std::cout << "Invalid Arguements: " << args << std::endl;
                }
            }
        } else if(c == COMMIT) {
            Repo r;
            r.addCommit(args+2);
            std::cout << "Added Commit" << std::endl;
        } else if(c == NOT_A_COMMAND) {
            std::cout << "Invalid Arguements: " << args << std::endl;
        }
    }
    
    return 0;
}
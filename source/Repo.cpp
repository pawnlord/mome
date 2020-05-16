#include "../include/Repo.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include <ctime>

std::string get_current_dir() {
   char* buff = (char*) malloc(FILENAME_MAX); //create string buffer to hold path
   GetCurrentDir(buff, FILENAME_MAX);
   std::string current_working_dir(buff);
   free(buff);
   return current_working_dir;
}

Repo::Repo(bool create){
    if(!create) {
        std::string cwd = get_current_dir();
        std::fstream fs;
        fs.open(".mome");
        if (!fs.is_open()) {
            this->r.active = false;
            this->r.date_created = ""; 
        } else {
            this->r.active = true;
            fs.close();
            std::ifstream ifs;
            ifs.open(".mome");
            std::string current_line;    
            this->r.date_created = "no_date_yet";
        
            while(getline(ifs, current_line)){
                for(int i = 0; current_line[i] != 0; i++){
                    std::cout << current_line.substr(0,i) << std::endl;
                    if(current_line[i] == '=' && current_line.substr(0,i) == "date_created"){
                        this->r.date_created = std::string(current_line.c_str()+i+1);
                        break;
                    }
                }
            }
            std::cout << this->r.date_created;
        }
    } else {
        std::ofstream file;
        file.open(".mome");
        
        std::time_t result = std::time(NULL);
        file << "date_created=" << std::asctime(std::localtime(&result));
        
        file.close();
    }
}
repoInfo Repo::getInfo(){
    return this->r;
}
std::string Repo::formatInfo(std::string format){
    /**
    * Get info in specified format.
    * format specifiers:
    * @: active
    * #: date
    */
    std::string formatted;
    if(!this->r.active) {
        formatted = "No active repo";
    } else {
        formatted = "Active, created " + this->r.date_created; 
    }
    return formatted;
}
#include "../include/Repo.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

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
            this->r.date_created = "no_date_yet";
        }
    } else {
        std::ofstream file;
        file.open(".mome");
        file << "default_mome_file.\n";
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
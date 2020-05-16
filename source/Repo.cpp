#include "../include/Repo.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <io.h>
#include <sys/stat.h>

#include <ctime>

bool checkDir( std::string name )
{
#ifdef OS_WINDOWS
    struct _stat buf;
    int result = _stat( name.c_str(), &buf );
#else
    struct stat buf;
    int result = stat( name.c_str(), &buf );
#endif
    return result == 0;
}

std::string getLastDir(std::string path){
    int lastSlash = 0;
    for(int i = 0; i < path.length(); i++){
        if(path[i] == '\\' || path[i] == '/'){
            lastSlash = i;
        }
    }
    path = path.substr(0, lastSlash);
    
    return path;
}

std::string recursiveCheckDir(std::string name, std::string path ){
    if(checkDir(path+"/"+name)){
        return path+"/"+name;
    }
    for(std::string currentPath = path; currentPath != ""; currentPath = getLastDir(currentPath)){
        if(checkDir(currentPath+"/"+name)){
            if(strchr(currentPath, '\\')) {
                return  currentPath+"\\"+name;
            } else {
                return currentPath+"/"+name;
            }
        }
    }
    return "";
}

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
        
        std::string repoDir = "";
        if ((repoDir = recursiveCheckDir(".mome", cwd)) == "") {
            this->r.active = false;
            this->r.date_created = ""; 
        } else {
            this->r.active = true;
            std::ifstream ifs;
            ifs.open((repoDir + "/.mome").c_str());
            std::string current_line;    
            this->r.directory = repoDir;
            this->r.date_created = "no_date_yet";
            
            while(getline(ifs, current_line)){
                for(int i = 0; current_line[i] != 0; i++){
                    if(current_line[i] == '=' && current_line.substr(0,i) == "date_created"){
                        this->r.date_created = std::string(current_line.c_str()+i+1);
                        break;
                    }
                }
            }
        }
    } else {
        if(mkdir(".mome") != 0){
            
            std::cout << "Error making .mome directory, aborting init" << std::endl;
            exit(EXIT_FAILURE);
        }
        std::ofstream file;
        file.open(".mome/.mome");
        
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
        formatted = "Active Repo found (" + this->r.directory + "), created " + this->r.date_created; 
    }
    return formatted;
}
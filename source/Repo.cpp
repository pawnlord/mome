#include "../include/Repo.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <io.h>
#include <sys/stat.h>
#include <string>
#include <string.h>
#include <string>

#include <ctime>

#include <sstream>

/* MinGW dumb, need to declare this myself */
std::string to_string(int i){
    std::stringstream ss;
    ss<<i;
    return ss.str();
}

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
            if(strchr(currentPath.c_str(), '\\')) {
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
            this->r.directory = repoDir.substr(0, repoDir.length() - std::string("/.mome").length());
            this->r.date_created = "no_date_yet";
            
            while(getline(ifs, current_line)){
                for(int i = 0; current_line[i] != 0; i++){
                    if(current_line[i] == '=' && current_line.substr(0,i) == "date_created"){
                        this->r.date_created = std::string(current_line.c_str()+i+1);
                        break;
                    } else if(current_line[i] == '=' && current_line.substr(0,i) == "commit_number"){
                        this->r.commit_number = std::string(current_line.c_str()+i+1);
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
        file << "date_created=" << std::asctime(std::localtime(&result)) << std::endl;
        file << "commit_number=0"<< std::endl;
        
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
        formatted = "Active Repo found (" + this->r.directory + ").\nCreated " + this->r.date_created + 
                    ", with " + this->r.commit_number + " commits"; 
    }
    return formatted;
}
void Repo::addCommit(char** args){
    std::ifstream ifs;
    ifs.open((this->r.directory+ "/.mome").c_str());
    std::string current_line;    
    
    int new_commit_number = atoi(this->r.commit_number.c_str())+1;
    int currentCursorPosition = 0;
    while(getline(ifs, current_line)){
        for(int i = 0; current_line[i] != 0; i++){
            if(current_line[i] == '=' && current_line.substr(0,i) == "commit_number"){
                ifs.seekg(0, std::ios::beg);
                std::string file_data((std::istreambuf_iterator<char>(ifs)),
                    std::istreambuf_iterator<char>());
                /* work on data */
                std::string start = file_data.substr(0, currentCursorPosition+1);
                std::string end = file_data.substr(currentCursorPosition+current_line.length()-i);
            
                file_data = start + to_string(new_commit_number);
            
                ifs.close();
            
                std::ofstream ofs;
                ofs.open((this->r.directory+ "/.mome").c_str(), std::ios::trunc);
                ofs << file_data;

                break;
            }
            currentCursorPosition++;
        }
        currentCursorPosition++;
    }
}
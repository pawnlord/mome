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

#include <filesystem>
namespace fs = std::filesystem;

/* MinGW dumb, need to declare this myself */
std::string to_string(int i){
    std::stringstream ss;
    ss<<i;
    return ss.str();
}

/* check if a directory exists */
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

/* check for a directory <name> in every dir in <path> */
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

/* get cwd */
std::string get_current_dir() {
   char* buff = (char*) malloc(FILENAME_MAX); //create string buffer to hold path
   GetCurrentDir(buff, FILENAME_MAX);
   std::string current_working_dir(buff);
   free(buff);
   return current_working_dir;
}

/* clean a name so that it doesn't mess with any functions */
std::string cleanName(std::string name) {
    std::string new_string = "";
    for(int i = 0; i < name.length(); i++){
        /* if it has a slash, it could be interpreted as from the root directory */
        if(name[i] != '\\' && name[i] != '/')
            new_string+=name[i];
    }
    return new_string;
}

/* store all files in current directory */
bool storeFiles(std::string destination, std::string from){
    std::string path = from;
    /* loop through every directory in <path> */
    for (const auto & entry : fs::directory_iterator(path)){
        /* get the name of the path */
        std::string name = cleanName(entry.path().string().substr(from.length()));
        /* if it's a directory, make a new directory in the commit folder, 
         * and recursively store everything in that folder 
         */
        if(fs::is_directory(entry.path())){
            /* don't do this for mome files (or git files, because that would be pointless) */
            if(entry.path().string().find(".git") == std::string::npos &&
                    entry.path().string().find(".mome") == std::string::npos){
                if(mkdir((destination+"/"+name).c_str()) != 0){
                    /* something went wrong, log it */
                    perror("mome commit: failed to make file");
                    std::cerr << "failed to make file " << destination+"/"+name << std::endl;
                    return 0;
                }
                if(!storeFiles(destination + "/" + name, entry.path().string())){
                    /* pass failure through if one happened */
                    return 0;
                };
            }
        } else {
            /* If it is a file, store it in <destination>
             * unless it is .git or .mome
             */
            if(entry.path().string().find(".git") == std::string::npos &&
                entry.path().string().find(".mome") == std::string::npos){
                
                std::ifstream ifs;
                /* opening up the file to read */
                ifs.open(entry.path().string().c_str(), std::ios::binary);
                if(ifs.fail()){
                    return 0;
                }
                /* read the data */
                ifs.seekg(0, std::ios::beg);
                std::string file_data((std::istreambuf_iterator<char>(ifs)),
                    std::istreambuf_iterator<char>());
                
                std::ofstream ofs;
                /* file to store it in */
                ofs.open(destination+"/"+name, std::ios::trunc | std::ios::binary);
                if(ofs.fail()){
                    return 0;
                }
                /* TODO: Make more than just file data*/
                /* store file_data*/
                ofs << file_data << std::endl;
                /* close files */
                ofs.close();
                ifs.close();
            }

        }
    }
    return 1;
}


Repo::Repo(bool create){
    /* create tells us if we want to create the repository or not */
    if(!create) {
        /* get current directory, to be used to find .mome file */
        std::string cwd = get_current_dir();
        
        std::string repoDir = "";
        /* try to find .mome file, if we fail set active to false */
        if ((repoDir = recursiveCheckDir(".mome", cwd)) == "") {
            this->r.active = false;
            this->r.date_created = ""; 
        } else {
            /* found one, set active to true and open .mome/.mome */
            this->r.active = true;
            std::ifstream ifs;
            ifs.open((repoDir + "/.mome").c_str());

            /* current_line is the line we are reading */
            std::string current_line;    
            /* store the directory we found */
            this->r.directory = repoDir.substr(0, repoDir.length());
            this->r.date_created = "no_date_yet";
            
            /* loop through lines and store the data */
            while(getline(ifs, current_line)){
                for(int i = 0; current_line[i] != 0; i++){
                    /* read date created and commit number*/
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
        /* TODO: make sure this works even if the directory already exists */

        /* make the directory */
        if(mkdir(".mome") != 0){
            
            std::cout << "Error making .mome directory, aborting init" << std::endl;
            exit(EXIT_FAILURE);
        }
        std::ofstream file;
        file.open(".mome/.mome");
        
        /* store date created */
        std::time_t result = std::time(NULL);
        file << "date_created=" << std::asctime(std::localtime(&result)) << std::endl;
        /* initialize commit_number */
        file << "commit_number=0"<< std::endl;
        
        file.close();
    }
}
/* get repoInfo */
repoInfo Repo::getInfo(){
    return this->r;
}

/* format repoInfo */
std::string Repo::formatInfo(std::string format){
    /**
    * Get info in specified format.
    * format specifiers:
    * @: active
    * #: date
    * TODO: needs to be implemented
    */
    std::string formatted;
    /* if we didn't find an active one, stop here*/
    if(!this->r.active) {
        formatted = "No active repo";
    } else {
        /* tell us location, date created, and commits */
        formatted = "Active Repo found (" + this->r.directory + ").\nCreated " + this->r.date_created + 
                    ", with " + this->r.commit_number + " commits"; 
    }
    return formatted;
}

std::string formatCommitData(std::string data) {
    std::string line = "";
    std::string formatted = "";
    for(char c : data) {
        if(c == '\n') {
            for (int i = 0; i < line.length(); i++) {
                if(line[i] == '='){
                    if(line.substr(0, i) == "date_created") {
                        formatted += "Created: " + line.substr(i+1) + "\n";
                    } else if(line.substr(0, i) == "info") {
                        formatted += "Message: " + line.substr(i+1) + "\n";
                    }
                }
            }
            line = "";
        } else {
            line += c;
        }
    }
    return formatted;
}

/* add a commit to repo */
int Repo::addCommit(int argc, char** args){
    /* read mome file */
    std::ifstream ifs;
    /* open the .mome (information) file */
    ifs.open((this->r.directory+ "/.mome").c_str());
    if(ifs.fail()){
        std::cerr << "Failed to open " << this->r.directory << "/.mome" << std::endl;
        perror("error");
        return 0;
    }
    std::string current_line;    
    
    /* find how many commits we need*/
    int new_commit_number = atoi(this->r.commit_number.c_str())+1;

    /* overall position in file */
    int current_cursor_position = 0;
    /* loop through lines */
    while(getline(ifs, current_line)){
        /* loop through characters in line*/
        for(int i = 0; current_line[i] != 0; i++){
            if(current_line[i] == '=' && current_line.substr(0,i) == "commit_number"){
                ifs.seekg(0, std::ios::beg);
                std::string file_data((std::istreambuf_iterator<char>(ifs)),
                    std::istreambuf_iterator<char>());
                /* work on data */
                std::string start = file_data.substr(0, current_cursor_position+1);
                std::string end = file_data.substr(current_cursor_position+current_line.length()-i);
            
                /* change number */
                file_data = start + to_string(new_commit_number) + end;
                /* close file */
                ifs.close();

                /* store new data */
                std::ofstream ofs;
                ofs.open((this->r.directory+ "/.mome").c_str(), std::ios::trunc);
                ofs << file_data;
                ofs.close();

                /* open file for new commit */
                if(!mkdir((this->r.directory+"/commit" + to_string(new_commit_number)).c_str())){
                    /* function that stores all files in the current directory in the given directory*/
                    std::string cwd = get_current_dir();
                    std::ofstream ofs;
                   
                    /* make a commit info file, store date and info (if available) */
                    ofs.open(this->r.directory+"/commit" + to_string(new_commit_number) + "/commit_info.cinfo");
                    std::time_t result = std::time(NULL);
                    ofs << "date_created=" << std::asctime(std::localtime(&result));
                    if(argc > 2){
                        ofs << "info=" << args[0] << std::endl;    
                    }
                    std::string from = this->r.directory.substr(0, this->r.directory.length() - std::string(".mome").length());
                    
                    /* actually store commit info */
                    return storeFiles(this->r.directory+"/commit" + to_string(new_commit_number), from ) * new_commit_number;
                } else {
                    /* failed to make dir, probably already there */
                    /* log error */
                    perror("failed to commit");
                    return 0;
                }
                break;
            }
            current_cursor_position++;
        }
        current_cursor_position++;
    }
    return 0;
}

/* output data 
 * return: success */
int Repo::logCommits() {
    /* return if no active repo is found*/
    if(this->r.active == false){
        return 0;
    }
    /* get commit number */
    int totalCommits = atoi(this->r.commit_number.c_str());
    
    /* general form of a commit directory */
    std::string commitDir = this->r.directory+"/commit";

    /* loop through commits, from most recent to least recent*/
    for(int i = totalCommits; i >= 1; i--){
        std::ifstream ifs;
        /* open info file */
        ifs.open((commitDir+to_string(i)+"/commit_info.cinfo").c_str());

        if(!ifs.fail()){
            /* print commit number we are reading */
            std::cout << "----------------" << std::endl;
            std::cout << "commit#"+to_string(i) << std::endl;
            std::cout << "----------------" << std::endl;

            /* read data and output it */
            ifs.seekg(0, std::ios::beg);
            std::string file_data((std::istreambuf_iterator<char>(ifs)),
                std::istreambuf_iterator<char>());
                
            std::cout << formatCommitData(file_data) << std::endl;    
        }
    }
    return 1;
}
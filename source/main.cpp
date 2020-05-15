#include <iostream>

int main(int argc, char** args){
    if(argc == 1) {
        std::cout << "Welcome to Mome! A version control system for momes" << std::endl; 
    } else {
        std::cout << "Invalid Arguements" << std::endl;
    }
    return 0;
}
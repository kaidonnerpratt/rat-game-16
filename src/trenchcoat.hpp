#ifndef TRENCHCOAT_H
#define TRENCHCOAT_H
#include <string>
#include <iostream>


namespace crm{
    void unzip(const char* path){
        std::string a = "unzip "+std::string(path);
        std::string b = a.c_str()+std::string(" -d ./temp");
        system(b.c_str());
    }
    void discard(){
        system("rm -rf ./temp");
    }
}




#endif
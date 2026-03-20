#ifndef TRENCHCOAT_H
#define TRENCHCOAT_H
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <ios>
#include <list>






namespace crm{
    struct T {
        static int local_header[4];
        static int cd_header[4];
        static int cd_footer[4];
    };
    int T::local_header[4] = {
        0x50,
        0x4b,
        0x03,
        0x04
    };
    int T::cd_header[4] = {
        0x50,
        0x4b,
        0x01,
        0x02
    };
    int T::cd_footer[4] = {
        0x50,
        0x4b,
        0x05,
        0x06
    };
    int readNext(FILE* file){
        return fgetc(file);
    }
    bool expect(FILE* file, int* expect, int len){
        bool ret = true;
        for (int i = 0; i < len; i++){
            if (feof(file)){
                return false;
            }
            if (readNext(file) != expect[i]){
                ret = false;
            }
        }
        // if (ret){
        //     puts("expected");
        // }else{
        //     puts("unexpected");
        // }
        return ret;
    }
    FILE* open(const char* path){
        FILE* f;
        f = fopen(path, "rb");

        if (expect(f, T::local_header, 4)){
            puts("GOT CRM");
        };
        
        return f;  
    }
    int* read(FILE* file, int len){
        int* data = new int[len];
        for (int i = 0; i < len; i++){
            data[i] = readNext(file);
        }
        return data;
    }
    
    long getPos(FILE* file){
        return ftell(file);
    }
    long setPos(FILE* file, long off){
        long p = getPos(file);
        fseek(file, off, SEEK_SET);
        return p;
    }
    long move(FILE* file, long off){
        long p = getPos(file);
        fseek(file, off, SEEK_CUR);
        return p;
    }
    long find(FILE* file, int* tofind, int len){
        long p = getPos(file);
        setPos(file, 0);
        while (!expect(file, tofind, len)){
            if (feof(file)){
                setPos(file, p);
                return(-1);
            }
            move(file, -len+1);
        }
        return setPos(file, p);

    }
    long findNext(FILE* file, int* tofind, int len){
        long p = getPos(file);

        while (!expect(file, tofind, len)){
            if (feof(file)){
                setPos(file, p);
                return(-1);
            }
            move(file, -len+1);
        }
        return getPos(file);
    }
    // int getFileCount(FILE* file){
    //     setPos(file, findNext(file, T::cd_header, 4));
    //     move(file, -4);
    //     return 1;
    // }
    void getCurFileName(FILE* file){
        int namesize;
        char* name;
        // inflate();
        move(file,24);
        namesize = readNext(file);
        move(file, 17);
        name = new char[namesize];
        for(int i = 0; i < namesize; i++){
            name[i] = char(readNext(file));
        }
        puts(name);
    }
    void close(FILE* file){
        fclose(file);

    }
}




#endif
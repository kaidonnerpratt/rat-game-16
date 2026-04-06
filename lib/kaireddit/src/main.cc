#include <stdio.h>
#include <cmath>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <type_traits>
template<typename T> concept arith=std::is_arithmetic_v<T>;
template<typename T> concept comp =requires(T a,T b){a<b;a>b;a==b;};
#include <types.hpp>
#include <assets.hpp>
// #ifdef DO

// void compileKR(char* name){
//     DO(strlen(name)>=128){perror("file name too long");exit(1);}

// }


int main(){

    return 0;
}



// #endif
// #endif
// #endif
// #endif
// #endif